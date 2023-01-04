// generate n number of threads that will pop off webservers from a queue and give them requests

// while the loadbalancer queue is not empty, pop off a request and send it to a webserver

// recieve a log from the webserver that reports when the request is done


#include <pthread.h>
#include <mutex.h>
#include "loadbalancer.h"
#include "request.h"

int main() {

    return 0;
}

void handleSingleThread() {

}