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


struct map_info map_info_to_net(struct map_info *i)
{
    return
        (struct map_info) {htonl(i->seed), htons(i->length), htons(i->height)};
}

struct map_info map_info_from_net(struct map_info *i)
{
    return
        (struct map_info) {ntohl(i->seed), ntohs(i->length), ntohs(i->height)};
}
