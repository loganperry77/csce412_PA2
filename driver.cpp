
#include <pthread.h>
#include <mutex.h>
#include "loadbalancer.h"
#include "webserver.h"
#include "request.h"

int main() {
    // generate a thread that will send requests to the loadbalancer

    // generate n number of threads that will pop off webservers from a queue and give them requests

    return 0;
}

void handleSingleThread() {
    // while the loadbalancer queue is not empty, pop off a request and send it to a webserver

    // recieve a log from the webserver that reports when the request is done
}