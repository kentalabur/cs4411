/* network test program 5
*/

#include "defs.h"
#include "minithread.h"
#include "minimsg.h"
#include "synch.h"
#include "read.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 512

int receive(int* arg);

char* hostname;


int
receive(int* arg) {
    char buffer[BUFFER_SIZE];
    int length;
    miniport_t port;
    miniport_t from;

    port = miniport_create_unbound(1);

    while(1){
        length = BUFFER_SIZE;
        minimsg_receive(port, &from, buffer, &length);
        printf("%s", buffer);
        //miniport_destroy(from);
    }

    return 0;
}

int
transmit(int* arg) {
    char buffer[BUFFER_SIZE];
    int length;
    network_address_t addr;
    miniport_t port;
    miniport_t dest;

    AbortOnCondition(network_translate_hostname(hostname, addr) < 0,
                     "Could not resolve hostname, exiting.");

    port = miniport_create_unbound(0);
    dest = miniport_create_bound(addr, 1);

    printf("Welcome to our lovely chat service.");
    minithread_fork(receive, NULL);

    while(1){
      miniterm_read(buffer, BUFFER_SIZE);
      length = strlen(buffer) + 1;
      minimsg_send(port, dest, buffer, length);
    }

    return 0;
}

int
main(int argc, char** argv) {

    if (argc > 1) {
        hostname = argv[1];
        minithread_system_initialize(transmit, NULL);
    }
    else {
        minithread_system_initialize(receive, NULL);
    }

    return -1;
}

