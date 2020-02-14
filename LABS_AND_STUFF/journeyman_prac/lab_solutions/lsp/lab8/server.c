#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>

#define TMP_BUF_SIZE    1024

/* network functios */
int create_server_socket(char* port, int type);
int accept_clients(int s_sock);
int handle_client(int sock);
ssize_t recv_str(int sock, char** str_ptr);
ssize_t recv_line(int sock, char** str_ptr);
ssize_t recv_until(int sock, unsigned char* needle, size_t needle_len,
	unsigned char** buf_ptr);

/* support functions */
void sig_handler(int signum);
unsigned char* memmem(unsigned char* haystack, size_t haystack_len,
	 unsigned char* needle, size_t needle_len);

int main(int argc, char* argv[]) {
	int sock;
    struct sigaction sa = {
        .sa_handler = sig_handler
    };

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    sock = create_server_socket("8080", SOCK_STREAM);
	if (sock == -1) {
		return EXIT_FAILURE;
	}

    accept_clients(sock);

	return EXIT_SUCCESS;
}

int create_server_socket(char* port, int type) {
	struct addrinfo hints = { 0 };
	struct addrinfo* ai_result;
	struct addrinfo* ai_cur;
	int ret;
	int sock;
	int optval = 1;

	hints.ai_family = AF_INET; /* IPv4 */
	hints.ai_socktype = type; /* SOCK_STREAM or SOCK_DGRAM */
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;

	ret = getaddrinfo(NULL, port, &hints, &ai_result);
	if (ret != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		return -1;
	}

	for (ai_cur = ai_result; ai_cur != NULL; ai_cur = ai_cur->ai_next) {
		sock = socket(ai_cur->ai_family, ai_cur->ai_socktype,
			ai_cur->ai_protocol);
		if (sock == -1) {
			continue;
		}

		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, 
			sizeof(optval)) == -1) {
			close(sock);
			continue;
		}

		if (bind(sock, ai_cur->ai_addr, ai_cur->ai_addrlen) == -1) {
			close(sock);
			continue;
		}
		break; /* if we made it here, we were sucessful */
	}

	freeaddrinfo(ai_result);

	if (ai_cur == NULL) {
		fprintf(stderr, "Could not find a suitable address\n");
		return -1;
	}

	if (type == SOCK_STREAM) {
		if (listen(sock, SOMAXCONN) == -1) {
			perror("listen");
			close(sock);
			return -1;
		}
	}

	return sock;
}

int accept_clients(int s_sock) {
	struct sockaddr_storage client_addr;
	socklen_t addr_len;
	int c_sock;
	pid_t pid;

    while (1) {
        addr_len = sizeof(client_addr);
    	c_sock = accept(s_sock, (struct sockaddr*)&client_addr, &addr_len);
    	if (c_sock == -1) {
            if (errno == EINTR) continue;
            perror("accept");
    		return -1;
    	}
		pid = fork();
		if (pid == -1) {
			perror("fork");
			return -1;
		}
		if (pid == 0) {
			handle_client(c_sock);
			exit(EXIT_SUCCESS);
		}
    }
    return 0;
}

int handle_client(int sock) {
	char* message;
	while (1) {
		message = NULL;
		if (recv_line(sock, &message) == -1) {
			break;
		}
		printf("Received: %s\n", message);
		send(sock, message, strlen(message), 0);
		free(message);
	}
	close(sock);

	return 0;
}

ssize_t recv_line(int sock, char** str_ptr) {
	return recv_until(sock, 
            (unsigned char*)"\n",
            1, 
            (unsigned char**)str_ptr);
}

ssize_t recv_str(int sock, char** str_ptr) {
	return recv_until(sock, 
            (unsigned char*)"\0", 
            1, 
            (unsigned char**)str_ptr);
}

ssize_t recv_until(int sock, unsigned char* needle, size_t needle_len, 
	unsigned char** buf_ptr) {

	ssize_t total_bytes_recvd = 0;
	ssize_t bytes_read;
	unsigned char* buffer = NULL;

	while (1) {
		buffer = (unsigned char*)realloc(buffer, 
				total_bytes_recvd ? total_bytes_recvd * 2 : TMP_BUF_SIZE);
		if (buffer == NULL) {
			return -1;
		}
		bytes_read = recv(sock, buffer+total_bytes_recvd, TMP_BUF_SIZE, 0);
		if (bytes_read == 0) {
            free(buffer);
			return -1;
		}
		total_bytes_recvd += bytes_read;
		if (memmem(buffer, total_bytes_recvd, needle, needle_len) != NULL) {
			break;
		}
	}

	*buf_ptr = buffer;
	return total_bytes_recvd;	
}

unsigned char* memmem(unsigned char* haystack, size_t haystack_len,
	 unsigned char* needle, size_t needle_len) {

	size_t i;
	if (needle_len == 0 || haystack_len < needle_len) {
		return NULL;
	}

	haystack_len -= (haystack_len % needle_len);

	for (i = 0; i < haystack_len; i++) {
		if (memcmp(&haystack[i], needle, needle_len) == 0) {
			return &haystack[i];
		}
	}
	return NULL;
}

void sig_handler(int signum) {
    if (signum == SIGCHLD) {
		while (waitpid(-1, NULL, WNOHANG) > 0) {};
	}
    return;
}
