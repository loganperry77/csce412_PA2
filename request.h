#ifndef REQUEST_H
#define REQUEST_H

// hold the IP in, IP out, and time for the task to run

#include <string>

struct request {
    string IPin;
    string IPout;
    int timeToRun;
};

#endif