#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>

#define MSG_BUF_SIZE	1024
#define TMP_BUF_SIZE	1024

/* network functions */
int connect_to_server(char* host, char* port);
ssize_t recv_str(int sock, char** str_ptr);
ssize_t recv_line(int sock, char** str_ptr);
ssize_t recv_until(int sock, unsigned char* needle, size_t needle_len,
	unsigned char** buf_ptr);
ssize_t send_str(int sock, char* str);

/* support functions */
unsigned char* memmem(unsigned char* haystack, size_t haystack_len,
	 unsigned char* needle, size_t needle_len);
void print_usage(char* name);
void sig_handler(int signum);
void chat(char* name);

int recv_chat(int sock);
int send_chat(int sock, char* name);

int main(int argc, char* argv[]) {
	struct sigaction sa = {
		.sa_handler = sig_handler
	};

	if (argc < 2) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		return EXIT_FAILURE;
	}

	chat(argv[1]);

	return EXIT_SUCCESS;
}

int connect_to_server(char* host, char* port) {
	struct addrinfo hints = { 0 };
	struct addrinfo* ai_result;
	struct addrinfo* ai;
	int ret;
	int sock;

	hints.ai_family = AF_INET; /* IPv4 */
	hints.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(host, port, &hints, &ai_result);
	if (ret != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		return -1;
	}

	for (ai = ai_result; ai != NULL; ai = ai->ai_next) {
		sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (sock == -1) {
			perror("socket");
			continue;
		}

		if (connect(sock, ai->ai_addr, ai->ai_addrlen) == -1) {
			perror("connect");
			close(sock);
			continue;
		}
		break; /* if we made it here, we were sucessful */
	}

	freeaddrinfo(ai_result);

	if (ai == NULL) {
		fprintf(stderr, "Could not find a suitable address\n");
		return -1;
	}

	return sock;
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

ssize_t send_str(int sock, char* str) {
	ssize_t bytes_sent;
	int bytes_to_send = strlen(str) + 1;
	int total_bytes_sent = 0;
	while (bytes_to_send > 0) {
		bytes_sent = send(sock, str + total_bytes_sent, bytes_to_send, 0);
		if (bytes_sent == -1) {
			perror("send");
			return -1;
		}
		total_bytes_sent += bytes_sent;
		bytes_to_send -= bytes_sent;
	}
	return total_bytes_sent;
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

void chat(char* name) {
	fd_set active_fd_set;
	fd_set read_fd_set;
	int sock;

	sock = connect_to_server("localhost", "8080");
	if (sock == -1) {
		fprintf(stderr, "Could not connect to server\n");
		exit(EXIT_FAILURE);
	}

	FD_ZERO(&active_fd_set);
	FD_SET(STDIN_FILENO, &active_fd_set);
	FD_SET(sock, &active_fd_set);
	
	while (1) {
		read_fd_set = active_fd_set;
		if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(EXIT_FAILURE);
		}

		if (FD_ISSET(STDIN_FILENO, &read_fd_set)) {
			if (send_chat(sock, name) == -1) {
				break;
			}
		}
		if (FD_ISSET(sock, &read_fd_set)) {
			if (recv_chat(sock) == -1) {
				break;
			}
		}
	}
	close(sock);
	return;
}

int recv_chat(int sock) {
	int bytes_read;
	unsigned char recv_buffer[MSG_BUF_SIZE];
	bytes_read = recv(sock, recv_buffer, MSG_BUF_SIZE-1, 0);
	if (bytes_read == -1) {
		return -1;
	}
	recv_buffer[bytes_read] = '\0';
	printf("%s", recv_buffer);
	return bytes_read;
}

int send_chat(int sock, char* name) {
	char send_buffer[MSG_BUF_SIZE * 2];
	char in_buffer[MSG_BUF_SIZE];
	fgets(in_buffer, MSG_BUF_SIZE, stdin);
	snprintf(send_buffer, MSG_BUF_SIZE * 2, "%s: %s", name, in_buffer);
	return send_str(sock, send_buffer);
}

void sig_handler(int signum) {
	_exit(0);
	return;
}

void print_usage(char* name) {
	printf("Usage: %s <your name>\n", name);
	return;
}
