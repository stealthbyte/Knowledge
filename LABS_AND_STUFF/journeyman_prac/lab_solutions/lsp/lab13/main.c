#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <time.h>

#define RST  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YLW  "\x1B[33m"
#define BLU  "\x1B[34m"
#define BLD  "\x1B[1m"

#define NUM_COLORS  3
#define NUM_NAMES   3

void* thread_func(void* arg);
struct thread_args* create_args(int num_threads);
pthread_t* create_threads(int num_threads, struct thread_args* args);
void wait_for_threads(int num_threads, pthread_t* tids);
void print_usage(char* name);
int printf_color(char* color, char* fmt, ...);
void timespec_diff(struct timespec* start, struct timespec* stop,
                   struct timespec* result);

struct thread_args {
    int id;
    char name[32];
    char color[32];
};

volatile int g_counter;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t g_sem;
pthread_cond_t g_red_cond;
pthread_mutex_t g_cvar_mutex;
int g_red_start = 0;

const char* names[NUM_NAMES] = { 
    "what",
    "who",
    "Slim Shady"
};

const char* colors[NUM_COLORS] = {
    "red",
    "blue",
    "green"
};

int main(int argc, char* argv[]) {
	struct timespec before;
	struct timespec after;
	struct timespec elapsed;
    int num_threads;
    pthread_t* tids;
	char key;
    struct thread_args* targs;

    if (argc < 2) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    num_threads = strtol(argv[1], NULL, 10);
    if (num_threads == LONG_MIN || num_threads == LONG_MAX) {
        fprintf(stderr, "Invalid number of threads\n");
        exit(EXIT_FAILURE);
    }

    targs = create_args(num_threads);
    if (targs == NULL) {
        fprintf(stderr, "failed to create thread params\n");
        exit(EXIT_FAILURE);
    }

	sem_init(&g_sem, 0, 0);
    pthread_mutex_init(&g_cvar_mutex, NULL);
    pthread_cond_init(&g_red_cond, NULL);

    printf_color("yellow", "Counter is %d\n", g_counter);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &before);
    tids = create_threads(num_threads, targs);
    if (tids == NULL) {
        fprintf(stderr, "failed to create threads\n");
        exit(EXIT_FAILURE);
    }

	while (1) {
		key = (char)getc(stdin);
		if (key == 'q') {
			break;
		}
		else if (key == 'r') {
		    pthread_mutex_lock(&g_cvar_mutex);
		    g_red_start = 1;
		    pthread_mutex_unlock(&g_cvar_mutex);
		    pthread_cond_broadcast(&g_red_cond);
		}
		else {
			sem_post(&g_sem);
		}
	}


    wait_for_threads(num_threads, tids);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &after);
	timespec_diff(&before, &after, &elapsed);
	/*printf("%lld.%.9ld\n", (long long)elapsed.tv_sec, elapsed.tv_nsec);*/

    printf_color("yellow", "Counter is %d\n", g_counter);
	pthread_mutex_destroy(&g_mutex);
	pthread_mutex_destroy(&g_cvar_mutex);
	pthread_cond_destroy(&g_red_cond);
	sem_destroy(&g_sem);
    free(tids);
    free(targs);
	return EXIT_SUCCESS;
}

void print_usage(char* name) {
    printf("Usage: %s num_threads\n", name);
    return;
}

struct thread_args* create_args(int num_threads) {
    int i;
    struct thread_args* args;
    args = (struct thread_args*)malloc(
            num_threads * sizeof(struct thread_args));
    if (args == NULL) {
        return NULL;
    }
    for (i = 0; i < num_threads; i++) {
        strcpy(args[i].name, names[i % NUM_NAMES]);
        strcpy(args[i].color, colors[i % NUM_NAMES]);
        args[i].id = i;
    }
    return args;
}

pthread_t* create_threads(int num_threads, struct thread_args* args) {
    int i;
    int ret;
    pthread_t* tids;
    tids = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    if (tids == NULL) {
        return NULL;
    }
    for (i = 0; i < num_threads; i++) {
        ret = pthread_create(&tids[i], NULL, thread_func, &args[i]);
        if (ret != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(ret));
            free(tids);
            return NULL;
        }
    }
    return tids;
}

void wait_for_threads(int num_threads, pthread_t* tids) {
    int i;    
    for (i = 0; i < num_threads; i++) {
		pthread_kill(tids[i], SIGINT);
        pthread_join(tids[i], NULL);
    }
    return;
}

void* thread_func(void* arg) {
    int i;
    struct thread_args* targs = (struct thread_args*)arg;

	if (strcmp(targs->color, "red") != 0) {
		sem_wait(&g_sem);
	}
	else {
	    pthread_mutex_lock(&g_cvar_mutex);
	    while (g_red_start == 0) {
	        pthread_cond_wait(&g_red_cond, &g_cvar_mutex);
	    }
	    pthread_mutex_unlock(&g_cvar_mutex);
	}

    printf_color(targs->color, "Thread %d incrementing...\n", targs->id);
    for (i = 0; i < 100; i++) {
		pthread_mutex_lock(&g_mutex);
        g_counter++;
		pthread_mutex_unlock(&g_mutex);
    }
    printf_color(targs->color, "Counter is now %d\n", g_counter);
    return NULL;
}

int printf_color(char* color, char* fmt, ...) {
    char* color_fmt;
    char* color_code;
    va_list args;
    int ret;

    if (strcmp(color, "red") == 0) {
        color_code = RED;
    }
    else if (strcmp(color, "green") == 0) {
        color_code = GRN;
    }
    else if (strcmp(color, "yellow") == 0) {
        color_code = YLW;
    }
    else {
        color_code = BLU;
    }

    color_fmt = malloc(strlen(fmt) + strlen(BLD) + strlen(color_code)
            + strlen(RST) + strlen(RST) + 1);
    if (color_fmt == NULL) {
        return 0;
    }

    sprintf(color_fmt, "%s%s%s%s%s", BLD, color_code, fmt, RST, RST);

    va_start(args, fmt);
    ret = vprintf(color_fmt, args);
    va_end(args);

    free(color_fmt);

    return ret;
}

void timespec_diff(struct timespec* start, struct timespec* stop,
                   struct timespec* result) {
	if ((stop->tv_nsec - start->tv_nsec) < 0) {
		result->tv_sec = stop->tv_sec - start->tv_sec - 1;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000UL;
	}
	else {
		result->tv_sec = stop->tv_sec - start->tv_sec;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return;
}
