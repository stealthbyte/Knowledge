#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <search.h>

#define BUFFER_MAX	1024
#define MAX_EVENTS	100


/* this enumeration is used to keep track of what
 * state a client is in so we know how to handle it
 * when we get an event for it */
enum state {
	CONNECTED,
	DISCONNECTED,
};

/* this struct just encapsulates all of the variables
 * required to properly manage a variable-sized buffer */
struct buffer {
	unsigned char* data;
	int length; /* current length of valid information in buffer */
	int max_length; /* the maximum possible length (currently) */
	int position; /* buffer location currently being looked at */
};

/* this struct associates a client socket descriptor with all of the 
 * other state we need to keep for each client. In event-driven
 * systems, we need to seperate functions from the data on which they 
 * operate. */
struct client {
	struct client* next;
	struct client* prev;
	int fd; /* socket descriptor for client */
	enum state state; /* state enumeration */
	struct buffer send_buf; /* holds the data we're supposed to send to client */
	struct buffer recv_buf; /* holds the data we've received from the client */
};

/* this struct associates a server socket descriptor with all of the
 * other state we need for the server. So far it's just one thing,
 * but we can easily extend it later if needed */
struct server {
	int fd; /* listening socket */
	struct client* first_client;
	struct client* last_client;
};

int send_message(struct client* client);
int recv_message(struct client* client);
int copy_message_to_all(struct client* list, struct client* client);
int create_server_socket(char* port, int protocol);
int set_blocking(int sock, int blocking);
int send_data(struct client* client);
int recv_data(struct client* client);
struct client* accept_client(struct server* server);
void free_client(struct client* client);

int main() {
	int n;
	int nfds;
	int epoll_fd;
	struct client* client;
	struct client* cur;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];

	signal(SIGPIPE, SIG_IGN);

	/* start up listen socket on port 8080, TCP */	
	int listen_sock = create_server_socket("8080", SOCK_STREAM);
	/* create epoll file descriptor */
	if ((epoll_fd = epoll_create1(0)) == -1) {
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}

	/* make server socket nonblocking */
	set_blocking(listen_sock, 0);
	/* package the server FD up so we can use a pointer to it
 	 * in our event struct and add to it later if we need */
	struct server server = { .fd = listen_sock };

	ev.events = EPOLLIN; /* register incoming data event (level-triggered)
							with the listen FD */
	ev.data.ptr = (void*)&server; /* associate the server struct with this event */
	/* use epoll_ctl to add the listen sock to the epoller, and give it the 
	   events we want to listen for and the data we want back when we get 
	   those events by passing &ev 
	*/
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}

	while (1) {
		/* Wait for events, indefinitely (-1 for last param means wait forever).
		 * You can change this timeout value to be able to return
		 * early from epoll_wait() and clean up idle connections */
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		/* loop through all the events we got */
		for (n = 0; n < nfds; n++) {

			/* events[n].data.ptr is the same data we gave to epoll_ctl earlier
			    with our ev struct
			 */
			if (events[n].data.ptr == &server) {
				/* Incoming event for server listening socket, meaning a new 
				   client connected 
				*/

				/* this function calls accept.  If you want this to be edge-triggered, 
					you should call accept multiple times
				*/
				client = accept_client(&server);
				if (client == NULL) continue;
				if (server.first_client != NULL) {
					insque(client, server.last_client);
					server.last_client = client;
				}
				else {
					insque(client, NULL);
					server.first_client = client;
					server.last_client = client;
				}

				/* IN/OUT/HUP events and width edge-triggering*/
				ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
				ev.data.ptr = (void*)client;
				/* add new client to epoller */
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client->fd, &ev) == -1) {
					perror("epoll_ctl: client");
					exit(EXIT_FAILURE);
				}
				continue;
			}


			/* if we got this far, it means we have an event from a client */
			client = (struct client*)events[n].data.ptr;
			if (events[n].events & EPOLLIN) {
				/* input event happened */
				if (recv_message(client) == 0) {
					cur = server.first_client;
					copy_message_to_all(server.first_client, client);
					while (cur != NULL) {
						if (send_message(cur) == 1) {
							ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
							ev.data.ptr = (void*)cur;
							if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cur->fd, NULL) == -1) {
								//perror("epoll_ctl: add out client");
							}
						}
						cur = cur->next;
					}
				}
			}
			if (events[n].events & EPOLLOUT) {
				/* output event happened (our send buffer is no longer full) */
				if (send_message(client) == 0) {
					ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
					ev.data.ptr = (void*)client;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client->fd, NULL) == -1) {
						//perror("epoll_ctl: rem out client");
						//exit(EXIT_FAILURE);
					}
				}
			}
			if (events[n].events & EPOLLRDHUP) { 
				/* client shut down the writing side of its socket */
				client->state = DISCONNECTED;
			}
			if (events[n].events & EPOLLHUP) {
				/* client shut down its socket */
				client->state = DISCONNECTED;
			}
			if (events[n].events & EPOLLERR) {
				/* client socket errored */
				client->state = DISCONNECTED;
			}
			if (client->state == DISCONNECTED) {
				/* cleanup client if its now in the disconnected state */
				if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client->fd, NULL) == -1) {
					perror("epoll_ctl: removing client");
					exit(EXIT_FAILURE);
				}
				printf("client disconnected\n");
				close(client->fd); 
				/* technically this will also remove the client from epoll, 
				   so the CTL_DEL isn't necessary
				*/
				if (client == server.first_client) {
					server.first_client = client->next;
				}
				if (client == server.last_client) {
					server.last_client = client->prev;
				}
				remque(client);
				free_client(client);
			}
		}
	}
	return 0;
}

/*
 * Returns 0 when there is no more reading to do 
 * Returns 1 when we still need more data to rely a message
 *
*/
int recv_message(struct client* client) {
	/* drain all data available and put it in client recv buffer */
	if (recv_data(client) != 0) {
		/* if an unhandled error occurred, stop */
		client->state = DISCONNECTED;
		return 0;
	}

	if (client->recv_buf.length > 0) {
		printf("client buffer: %.*s", client->recv_buf.length, client->recv_buf.data);
	}

	return 0;
}

int copy_message_to_all(struct client* list, struct client* client) {
	struct client* cur = list;
	int msg_length;
	int old_buf_length;

	msg_length = client->recv_buf.length;
	while (cur != NULL) {
		/* create a send buffer for the client. We're just echoing
		 * what it sent to us */
		old_buf_length = cur->send_buf.length;
		cur->send_buf.length += msg_length;
		/* create space for the message, if it doesn't exist already */
		if (cur->send_buf.max_length < cur->send_buf.length) {
			cur->send_buf.data = realloc(cur->send_buf.data, cur->send_buf.length);
			cur->send_buf.max_length = cur->send_buf.length;
		}
		memcpy(cur->send_buf.data + old_buf_length, client->recv_buf.data, msg_length);

		cur = cur->next;
	}

	/* reset the client's receive buffer */
	client->recv_buf.length = 0;
	client->recv_buf.position = 0;
	return 0;
}

/*
 * Returns 0 when all messages have been sent
 * Returns 1 when there is still data to be sent
 *
*/
int send_message(struct client* client) {
	/* send all the data we possibly can */
	if (send_data(client) == 1) {
		/* this means we weren't able to send everything.
		 * return 1 to indicate there's still more to send */
		return 1;
	}

	/* if we got here then the send buffer has all been sent
	 * let's go ahead and clear it. */
	client->send_buf.length = 0;
	client->send_buf.position = 0;
	/* notice we're just invalidating the current send buffer
	 * by setting length and position to 0. we're going to keep
	 * the same memory for efficiency purposes */


	return 0;
}

/* accept_client gets a new client connection using accept(),
 * makes the client socket nonblocking, and places that socket
 * descriptor into a freshly initialized client struct
 *
 * returns a pointer to the new client struct and NULL on error */
struct client* accept_client(struct server* server) {
	struct sockaddr_storage addr;
	int sock = server->fd;
	socklen_t addr_len = sizeof(struct sockaddr_storage);

	/* get a new client socket descriptor */
	int new_fd = accept(sock, (struct sockaddr*)&addr, &addr_len);
	if (new_fd == -1) {
		perror("accept");
		return NULL;
	}

	/* make the new socket descriptor nonblocking */
	set_blocking(new_fd, 0);

	/* create new client structure */
	struct client* client = (struct client*)calloc(1, sizeof(struct client));
	client->fd = new_fd;

	/* initialize client state to receiving */
	client->state = CONNECTED;
	
	/* the recv and send buffers for the client were all 
	 * zeroed out by the previouc calloc() */
	printf("got a connection\n");
	return client;
}

/* free_client frees a heap-allocated client structure
 * and corresponding buffer data, if present */
void free_client(struct client* client) {
	if (client->recv_buf.data) free(client->recv_buf.data);
	if (client->send_buf.data) free(client->send_buf.data);
	free(client);
	return;
}

/* recv_data receives all the data currently in the receieve buffer.
 * It returns when:
 * 	there is no data left
 * 	an error other than EINTR is returned by recv.
 * 	the client shuts down the writing half of its socket (or disconnects)
 *
 * 	returns 0 when there is no more data to read
 * 	returns 1 when an error occurred
 */
int recv_data(struct client* client) {
	char temp_buffer[BUFFER_MAX];
	int bytes_read;
	while (1) {
		bytes_read = recv(client->fd, temp_buffer, BUFFER_MAX, 0);
		if (bytes_read == -1) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				break; /* We've read all we can for the moment, stop */
			}
			else if (errno == EINTR) {
				/* continue upon interrupt
				 * this doesn't necessarily have to be what you do */
				continue;
			}
			else {
				/* some other error occurred, let's print it */
				perror("recv");
				return 1;
			}
		}
		else if (bytes_read == 0) {
			/* the client won't be sending us any more data (ever)
			 * but it may still be wanting us to send things back */
			break;
		}

		/* Realloc the recv buffer of the client if needed */
		int new_length = client->recv_buf.length + bytes_read;
		if (client->recv_buf.max_length < new_length) {
			/* since we just reached the max size of the buffer, let's double it 
			 * so we are less likely to run into this situation again */
			client->recv_buf.data = realloc(client->recv_buf.data, new_length * 2);
			client->recv_buf.max_length = new_length * 2;
		}

		/* now append the new data recieved to the client's recv buffer */
		memcpy(&(client->recv_buf.data)[client->recv_buf.length], temp_buffer, bytes_read);
		client->recv_buf.length += bytes_read;
	}
	return 0;
}

/* send_data attempts to send all of the data left in a client's
 * send buffer to the client socket. It starts at the position of
 * the send buffer and tries to send all remaining bytes from that
 * point. send_data returns when:
 * 
 * 	all data has been sent
 * 	an error other than EINTR occurs
 * 	the OS send buffer is full (EAGAIN/EWOULDBLOCK)
 *
 *  Returns 0 if all data in the send buffer was sent
 *  Returns 1 if there was still data left in the buffer */
int send_data(struct client* client) {
	char* buffer = (char*)client->send_buf.data;
	int bytes_sent;
	int bytes_left = client->send_buf.length - client->send_buf.position;
	while (bytes_left > 0) {
		bytes_sent = send(client->fd, &buffer[client->send_buf.position], bytes_left, 0);
		if (bytes_sent == -1) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				return 1; /* We've sent all we can for the moment */
			}
			else if (errno == EINTR) {
				continue; /* continue upon interrupt */
			}
			else if (errno == EPIPE || errno == ECONNRESET) {
				/* client disconnected, we can't send any more data */
				client->state = DISCONNECTED;
				return 1;
			}
			else {
				perror("send");
				client->state = DISCONNECTED;
				return 1;
			}
		}
		bytes_left -= bytes_sent;
		client->send_buf.position += bytes_sent;
	}
	return 0;
}

/* Create server socket establishes a server socket on
 * the given port for the given protocol. The resulting
 * socket is bound to all local addresses, both IPv4 and
 * IPv6, and has the REUSEADDR option set. If SOCK_STREAM
 * is specified,the socket is turned into a listening
 * socket.
 *
 * port can be any string representing a numeric port
 * protocol can be either SOCK_DGRAM or SOCK_STREAM
 *
 * returns a server socket descriptor on success
 * exits on failure
 * */
int create_server_socket(char* port, int protocol) {
	int sock;
	int ret;
	int optval = 1;
	struct addrinfo hints;
	struct addrinfo* addr_ptr;
	struct addrinfo* addr_list;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = protocol;
	/* AI_PASSIVE for filtering out addresses on which we
	 * can't use for servers
	 *
	 * AI_ADDRCONFIG to filter out address types the system
	 * does not support
	 *
	 * AI_NUMERICSERV to indicate port parameter is a number
	 * and not a string
	 *
	 * */
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
	/*
	 *  On Linux binding to :: also binds to 0.0.0.0
	 *  Null is fine for TCP, but UDP needs both
	 *  See https://blog.powerdns.com/2012/10/08/on-binding-datagram-udp-sockets-to-the-any-addresses/
	 */
	ret = getaddrinfo(protocol == SOCK_DGRAM ? "::" : NULL, port, &hints, &addr_list);
	if (ret != 0) {
		fprintf(stderr, "Failed in getaddrinfo: %s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	for (addr_ptr = addr_list; addr_ptr != NULL; addr_ptr = addr_ptr->ai_next) {
		sock = socket(addr_ptr->ai_family, addr_ptr->ai_socktype, addr_ptr->ai_protocol);
		if (sock == -1) {
			perror("socket");
			continue;
		}

		// Allow us to quickly reuse the address if we shut down (avoiding timeout)
		ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		if (ret == -1) {
			perror("setsockopt");
			close(sock);
			continue;
		}

		ret = bind(sock, addr_ptr->ai_addr, addr_ptr->ai_addrlen);
		if (ret == -1) {
			perror("bind");
			close(sock);
			continue;
		}
		break;
	}
	freeaddrinfo(addr_list);
	if (addr_ptr == NULL) {
		fprintf(stderr, "Failed to find a suitable address for binding\n");
		exit(EXIT_FAILURE);
	}

	if (protocol == SOCK_DGRAM) {
		return sock;
	}
	// Turn the socket into a listening socket if TCP
	ret = listen(sock, SOMAXCONN);
	if (ret == -1) {
		perror("listen");
		close(sock);
		exit(EXIT_FAILURE);
	}

	return sock;
}

/* set_blocking turns a socket into a blocking
 * or nonblocking socket, if it is not already
 * set to the indicated mode.
 *
 * returns 0 on success
 * returns -1 on failure
 *
 * sock is a valid, open socket descriptor
 * blocking is either 0 or 1
 * 	0 for nonblocking
 * 	1 for blocking
 */
int set_blocking(int sock, int blocking) {
	int flags;
	/* Get flags for socket */
	if ((flags = fcntl(sock, F_GETFL)) == -1) {
		perror("fcntl get");
		return -1;
	}
	/* Only change flags if they're not what we want */
	if (blocking && (flags & O_NONBLOCK)) {
		if (fcntl(sock, F_SETFL, flags & ~O_NONBLOCK) == -1) {
			perror("fcntl set block");
			return -1;
		}
		return 0;
	}
	/* Only change flags if they're not what we want */
	if (!blocking && !(flags & O_NONBLOCK)) {
		if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
			perror("fcntl set nonblock");
			return -1;
		}
		return 0;
	}
	return 0;
}

