#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define USAGELINE	"Usage: %s server_address server_port \"bot_path_1 [bot_path_2 [...]]\"\n"
#define RECVBUFSZ	0x1000
#define BADCHARS	"$`&|;"

void handleRequest(int sclient, const char* supported);


int main(int argc, char* argv[]) {
	int err = 0;
	char* server_addr = NULL;
	char* server_port = 0;
	struct addrinfo hints = {0};
	struct addrinfo* bindres = NULL;
	struct addrinfo* res = NULL;
	struct sockaddr_in remaddr = {0};
	socklen_t addrlen = 0;
	struct sockaddr_in* srvaddr = {0};
	ssize_t amt = 0;
	ssize_t tot = 0;
	ssize_t amt_to_send = 0;
	int enable = 1;
	int s = -1;
	int sclient = -1;

	if (argc != 4) {
		fprintf(stderr, USAGELINE, argv[0]);
		goto END;
	}

	server_addr = argv[1];
	server_port = argv[2];

	// get the addr for the server
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	if ((err = getaddrinfo(server_addr, server_port, &hints, &res))) {
		fprintf(stderr, "getaddrinfo: %d\n", err);
		goto END;
	}

	srvaddr = (struct sockaddr_in*)res->ai_addr;

	// contact the server, telling them what bot type(s) we support 
	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		perror("socket");
		goto END;
	}

	if (connect(s, (struct sockaddr*)srvaddr, res->ai_addrlen)) {
		perror("connect");
		goto END;
	}

	// send a NULL terminated, space delimited, bunch of bots
	amt_to_send = strlen(argv[3]) + 1;

	while (1) {
		amt = send(s, argv[3] + tot, amt_to_send - tot, 0);
		if (amt <= 0) {
			perror("send");
			goto END;
		}
		
		tot += amt;
		
		if (tot == amt_to_send) {
			break;
		}
	}

	close(s);
	s = -1;

	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		perror("socket");
		goto END;
	}

	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
		perror("setsockopt");
		goto END;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((err = getaddrinfo(NULL, argv[2], &hints, &bindres))) {
		fprintf(stderr, "getaddrinfo: %d\n", err);
		goto END;
	}

	if (bind(s, res->ai_addr, res->ai_addrlen)) {
		perror("bind");
		goto END;
	}

	// listen for requests
	if (listen(s, 5)) {
		perror("listen");
	}
	
	fprintf(stderr, "accepting connections for instantiating bots \"%s\"\n", argv[3]);
	while (1) {
		// accept request from server
		addrlen = sizeof(remaddr);
		sclient = accept(s, (struct sockaddr*)&remaddr, &addrlen);
		if (sclient == -1) {
			perror("accept");
			goto END;
		}

		if (memcmp(&(remaddr.sin_addr), srvaddr, sizeof(remaddr.sin_addr))) {
			fprintf(stderr, "Got a connection from someone not the server!\n");
			goto END;
		}

		handleRequest(sclient, argv[3]);
		
		close(sclient);
		sclient = -1;
	}

END:	
	if (res != NULL) {
		freeaddrinfo(res);
	}

	if (bindres != NULL) {
		freeaddrinfo(bindres);
	}
	
	return 0;
}

void handleRequest(int sclient, const char* supported) {
	char buf[RECVBUFSZ];
	const char* cursor = NULL;
	ssize_t bplen = 0;
	int match = 0;
	ssize_t amt = 0;
	ssize_t tot = 0;
	size_t i = 0;

	// recv the request until we get a NULL, expecting a botpath and arguments
	// we do a brief check to make sure there isn't any bad chars, but
	// this is hella exploitable if students can get to here
	// but, you know, good on em if they do
	
	while (1) {
		amt = recv(sclient, buf+tot, sizeof(buf)-tot, 0);
		if (amt == -1) {
			perror("sclient recv");
			return;
		} else if (amt == 0) {
			fprintf(stderr, "didn't get end of request\n");
			return;
		}

		tot += amt;

		if (!memchr(buf, '\0', tot)) {
			continue;
		}

		break;
	}

	// got request, now process it
	// first check for bad characters
	for (i=0; i < strlen(BADCHARS); i++) {
		if (strchr(buf, BADCHARS[i])) {
			fprintf(stderr, "found a bad char %c in request\n", BADCHARS[i]);
			return;
		}
	}

	// check that the first argument is a supported botpath
	cursor = supported;
	while ((cursor = strchr(cursor, ' '))) {
		cursor = cursor+1;
		bplen = strchr(cursor, ' ') - cursor;
		if (bplen < 0) {
			bplen = strlen(cursor);
		}
		if (strncmp(cursor, buf, bplen) == 0) {
			match = 1;
			break;
		}
	}

	if (match) {
		// ok, assume this is a good command line/path for the bot
		system(buf);
	}
}
