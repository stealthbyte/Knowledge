#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define HELLOMSG	"Hello!\n"

#define HOST		"::1"
#define PORT		8080

/* This example connects, sends, and recieves from HOST/PORT */
/* To host a server to try this against, use "ncat -kl 8080" */

int main() {
	int sk;
	int err;
	ssize_t amt;
	char recvbuf[0x400];
	struct sockaddr_in6 addr = {0};

	sk = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (sk == -1) {
		perror("socket");
		exit(-1);
	}

	err = inet_pton(AF_INET6, HOST, &addr.sin6_addr);
	if (err != 1) {
		perror("inet_pton");
		exit(-1);
	}
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(PORT);

	err = connect(sk, (struct sockaddr*) &addr, sizeof(addr));
	if (err == -1) {
		perror("connect");
		exit(-1);
	}

	amt = send(sk, HELLOMSG, sizeof(HELLOMSG)-1, MSG_NOSIGNAL);
	if (amt == -1) {
		perror("send");
		exit(-1);
	}

	amt = recv(sk, recvbuf, sizeof(recvbuf)-1, 0);
	if (amt == -1) {
		perror("recv");
		exit(-1);
	}

	recvbuf[amt] = '\0';

	printf("Got: %s\n", recvbuf);

	close(sk);

	return 0;
}
