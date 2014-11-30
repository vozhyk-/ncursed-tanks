#include "common.h"

/* function pointer to send or recv */
typedef ssize_t (*socket_io_fun)(int, void *, size_t, int);

/* do_all: sends/receives the whole buffer.
 *         Do not use outside this file.
 *
 * action:            function pointer to either send or recv
 * socket, data, len: standard arguments to send/recv
 * check_for_0:       recv() returns 0 when the connection is closed,
 *                    we want to check for this when using recv as action
 *
 * Returns the number of bytes actually processed, or -1 on failure
 */
int do_all(socket_io_fun action, int socket, void *data, int len,
           int check_for_0)
{
    int processed = 0;
    int bytesleft = len; // how many we have left to send/receive
    int n;

    while (processed < len)
    {
        n = action(socket, data + processed, bytesleft, 0);
        if (n == -1 || check_for_0 && n == 0)
            break;
        processed += n;
        bytesleft -= n;
    }

    return n == -1 ? -1 : processed;
}

/* sendall: sends the whole buffer. See do_all */
int sendall(int socket, void *data, int len)
{
    /* typecasting because of slightly mismatching argument types */
    return do_all((socket_io_fun)send, socket, data, len, 0);
}

/* sendall: receives the whole buffer. See do_all */
int recvall(int socket, void *data, int len)
{
    return do_all(recv, socket, data, len, 1);
}

int send_int8(int socket, int8_t i)
{
    return sendall(socket, &i, sizeof(i));
}

int send_int16(int socket, int16_t i)
{
    i = htons(i);

    return sendall(socket, &i, sizeof(i));
}

int send_int32(int socket, int32_t i)
{
    i = htonl(i);

    return sendall(socket, &i, sizeof(i));
}

int recv_int8(int socket, int8_t *i)
{
    return recvall(socket, i, sizeof(*i));
}

int recv_int16(int socket, int16_t *i)
{
    int test = recvall(socket, i, sizeof(*i));

    if (test != -1 && test != 0)
        *i = htons(*i);
    return test;
}

int recv_int32(int socket, int32_t *i)
{
    int test = recvall(socket, i, sizeof(*i));

    if (test != -1 && test != 0)
        *i = htonl(*i);
    return test;
}

/* returns 0 on success, -1 on failure */
int send_string(int socket, char *str)
{
    int16_t size = strlen(str) + 1;
    if (sendall(socket, &size, sizeof(size)) == -1)
        return -1;
    if (sendall(socket, str, size) == -1)
        return -1;
    return 0;
}

/* receive a string sent with send_string */
char *recv_string(int socket)
{
    int received;
    int16_t size;
    char *str;

    if ((received = recvall(socket, &size, sizeof(size))) == -1
        || received == 0)
        return NULL;
    /* TODO free */
    str = malloc(size);
    if ((received = recvall(socket, str, size)) == -1
        || received == 0)
        return NULL;

    return str;
}


int send_map_info(int socket, struct map_info *i)
{
    if (send_int32(socket, i->seed) == -1   ||
        send_int16(socket, i->length) == -1 ||
        send_int16(socket, i->height) == -1)
        return -1;
    return 0;
}

struct map_info *recv_map_info(int socket)
{
    struct map_info *result = malloc(sizeof(*result));
    int test;

    if ((test = recv_int32(socket, &result->seed)) == -1   || test == 0 ||
        (test = recv_int16(socket, &result->length)) == -1 || test == 0 ||
        (test = recv_int16(socket, &result->height)) == -1 || test == 0)
    {
        free(result);
        return NULL;
    }
    return result;
}

int send_player(int socket, struct player *p)
{
    if (send_string(socket, p->nickname) == -1 ||
        send_int8(socket, p->state) == -1      ||
        send_int16(socket, p->hitpoints) == -1 ||
        send_int16(socket, p->pos_x) == -1     ||
        send_int16(socket, p->pos_y) == -1)
        return -1;
    return 0;
}

struct player *recv_player(int socket)
{
    struct player *result = malloc(sizeof(*result));
    int test;
    u_int8_t state_net;

    if ((result->nickname = recv_string(socket)) == NULL                   ||
        /* problems with putting int8 into enum? */
        (test = recv_int8(socket, &state_net)) == -1          || test == 0 ||
        (test = recv_int16(socket, &result->hitpoints)) == -1 || test == 0 ||
        (test = recv_int16(socket, &result->pos_x)) == -1     || test == 0 ||
        (test = recv_int16(socket, &result->pos_y)) == -1     || test == 0)
    {
        free(result);
        return NULL;
    }
    else
    {
        result->state = state_net;
        return result;
    }
}
