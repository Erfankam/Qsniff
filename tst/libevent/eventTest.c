/*
 * Compile with:
 * cc -I/usr/local/include -o signal-test \
 *   signal-test.c -L/usr/local/lib -levent
 */

#include <sys/types.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/stat.h>
#ifndef WIN32
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#else
#include <windows.h>
#endif
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event.h>

int called = 0;

static void myHandler(int fd, short event, void *arg)
{
    struct event *myEvent = arg;

    printf("%s: got signal %d\n", __func__, EVENT_SIGNAL(myEvent));

    if (called >= 2)
        event_del(myEvent);

    called++;
}

int
main (int argc, char **argv)
{
    struct event mySignal;

    /* Initalize the event library */
    struct event_base* mySignalRepo = event_base_new();

    /* Initalize one event */
    event_set(&mySignal, SIGINT, EV_SIGNAL|EV_PERSIST, myHandler,
        &mySignal);
    event_base_set(mySignalRepo, &mySignal);

    event_add(&mySignal, NULL);

    event_base_dispatch(mySignalRepo);
    event_base_free(mySignalRepo);

    return (0);
}

