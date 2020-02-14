#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libircclient.h>

#define BOT_NICK	"DEFBOT"
#define BOT_USER	"defbot"
#define BOT_INFO	"The Bot Template"

#define HELLOMSG	"Hey everyone! I am a default bot."
#define MAXHELLOSZ	0x400

#define DEFAULTCHAN	"#botsrule"

#define INVITEREPLY	"Use the /bot command to instantiate a copy of me"

#define DEFAULT_PORT	"6667"

#define USAGELINE	"\nUsage: %s [-h][options] server [port]\n"

#define HELPLINE	USAGELINE \
			"\t-h         : Print this information\n"\
			"\t-n <nick>  : Bot's Nickname (default " BOT_NICK ")\n"\
			"\t-u <user>  : Bot's Username (default " BOT_USER ")\n"\
			"\t-t <#chan> : Target (channel or username) to talk to\n"\
			"\t-m \"msg\" : The hello string the bot uses after connecting\n"\
			"\tserver     : Address of the irc server\n"\
			"\tport       : Optional port of irc server (default " DEFAULT_PORT ")\n"

typedef struct _bot_ctx {
	// fill out as needed
	int unk;
} bot_ctx;

// Callback functions

static void connect_handler(
	irc_session_t* session,
	const char* event,
	const char* origin,
	const char** params,
	unsigned int count
);
static void msg_handler(
	irc_session_t* session,
	const char* event,
	const char* origin,
	const char** params,
	unsigned int count
);
static void invite_handler(
	irc_session_t* session,
	const char* event,
	const char* origin,
	const char** params,
	unsigned int count
);
static void numeric_handler(
	irc_session_t* session,
	unsigned int event,
	const char* origin,
	const char** params,
	unsigned int count
);

// Global variables
char* gtarget = DEFAULTCHAN; // user or channel the bot talks to
char ghello[MAXHELLOSZ] = HELLOMSG;

int main(int argc, char** argv) {
	int ret = 0;
	char* server_addr = NULL;
	short server_port = 0;
	char* nickname = BOT_NICK;
	char* username = BOT_USER;
	char* infoname = BOT_INFO;
	irc_callbacks_t callbacks = {0};
	irc_session_t* session = NULL;
	bot_ctx* ctx = NULL;
	int c = 0;

	// parse arguments
	opterr = 0;
	while ((c = getopt(argc, argv, "hn:u:t:m:")) != -1) {
		switch (c) {
		case 'h':
			fprintf(stderr, HELPLINE, argv[0]);
			exit(0);
		case 'n':
			nickname = optarg;
			break;
		case 'u':
			username = optarg;
			break;
		case 't':
			gtarget = optarg;
			break;
		case 'm':
			strncpy(ghello, optarg, MAXHELLOSZ-1);
			break;
		case '?':
			fprintf(stderr, "Unknown option -%c\n", optopt);
			fprintf(stderr, USAGELINE, argv[0]);
			ret = -1;
			goto END;
		case ':':
			fprintf(stderr, "Missing an argument for -%c\n", optopt);
			fprintf(stderr, USAGELINE, argv[0]);
			ret = -1;
			goto END;
		default:
			fprintf(stderr, "Argument issue\n");
			fprintf(stderr, USAGELINE, argv[0]);
			ret = -1;
			goto END;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "Required argument \"server\"\n");
		fprintf(stderr, USAGELINE, argv[0]);
		ret = -1;
		goto END;
	}

	server_addr = argv[optind];

	fprintf(stderr, "dbg: %d %d\n", optind, argc);
	if ((optind+1) >= argc) {
		server_port = strtoul(DEFAULT_PORT, NULL, 0);
	} else {
		server_port = strtoul(argv[optind+1], NULL, 0);
	}

	// start irc client
	// register more callbacks here if needed
	callbacks.event_connect = connect_handler;
	//callbacks.event_nick = event_handler;
	//callbacks.event_quit = event_handler;
	//callbacks.event_join = event_handler;
	//callbacks.event_part = event_handler;
	//callbacks.event_mode = event_handler;
	//callbacks.event_umode = event_handler;
	//callbacks.event_topic = event_handler;
	//callbacks.event_kick = event_handler;
	callbacks.event_channel = msg_handler;
	callbacks.event_privmsg = msg_handler;
	//callbacks.event_notice = event_handler;
	//callbacks.event_channel_notice = event_handler;
	callbacks.event_invite = invite_handler;
	//callbacks.event_unknown = event_handler;
	callbacks.event_numeric = numeric_handler;
	
	session = irc_create_session(&callbacks);
	if (!session) {
		fprintf(stderr, "Unable to create session. %s\n", irc_strerror(irc_errno(session)));
		ret = -1;
		goto END;
	}

	irc_option_set(session, LIBIRC_OPTION_SSL_NO_VERIFY);
	irc_option_set(session, LIBIRC_OPTION_STRIPNICKS);
	//irc_option_set(session, LIBIRC_OPTION_DEBUG);
	
	ctx = (bot_ctx*)calloc(1, sizeof(bot_ctx));
	if (!ctx) {
		fprintf(stderr, "Unable to allocate bot context\n");
		ret = -1;
		goto END;
	}

	irc_set_ctx(session, (void*)ctx);

	fprintf(stderr, "Connecting to %s on port %u\n", server_addr, server_port);
	
	if (irc_connect(session, server_addr, server_port, NULL, nickname, username, infoname)) {
		fprintf(stderr, "Unable to connect to server. %s\n", irc_strerror(irc_errno(session)));
		ret = -1;
		goto END;
	}

	if (irc_run(session)) {
		fprintf(stderr, "Error with connection to server. %s\n", irc_strerror(irc_errno(session)));
		ret = -1;
		goto END;
	}

END:
	if (ctx) {
		free(ctx);
	}
	if (session) {
		irc_destroy_session(session);
	}

	fprintf(stderr, "Done\n");

	return ret;
}

void connect_handler(
	irc_session_t* session,
	const char* event,
	const char* origin,
	const char** params,
	unsigned int count
) {
	(void)event;
	(void)count;
	(void)params;
	(void)origin;

	// join target channel (if it is a channel)
	if (gtarget && gtarget[0] == '#') {
		// join the target channel
		// NULL secret key
		irc_cmd_join(session, gtarget, NULL);
	}

	// say hello, if we have a "target"
	if (gtarget) {
		irc_cmd_msg(session, gtarget, ghello);
	}
}

void msg_handler(
	irc_session_t* session,
	const char* event,
	const char* origin,
	const char** params,
	unsigned int count
) {
	(void)event;

	char lastchar = 0;

	bot_ctx* ctx;
	char* msg = NULL;

	if (count < 2) {
		fprintf(stderr, "Error, msg handler called with param count of < 2\n");
		return;
	}
	
	ctx = irc_get_ctx(session);
	(void)ctx;

	msg = irc_color_strip_from_mirc(params[1]);

	lastchar = msg[strlen(msg)-1];
	if (lastchar == '\r' || lastchar == '\n') {
		msg[strlen(msg)-1] = '\0';
	}
	
	//TODO handle messages
	fprintf(stderr, "Got message from %s%s \"%s\"\n", origin, (params[0][0] == '#') ? params[0] : "", msg);

	if (msg) {
		free(msg);
	}
}

void invite_handler(
	irc_session_t* session,
	const char* event,
	const char* origin,
	const char** params,
	unsigned int count
) {
	(void)event;
	(void)params;
	(void)count;

	// tell the inviter to use the "/bot" command
	if (irc_cmd_msg(session, origin, INVITEREPLY)) {
		fprintf(stderr, "Error replying to invite. %s\n", irc_strerror(irc_errno(session)));
	}
}

void numeric_handler(
	irc_session_t* session,
	unsigned int event,
	const char* origin,
	const char** params,
	unsigned int count
) {
	(void)session;
	(void)event;
	(void)origin;
	(void)params;
	(void)count;
	// handle numeric server responses here
	// like to see results of a command
	return;
}

