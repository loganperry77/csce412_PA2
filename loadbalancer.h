#ifndef LOADBALANCER_H
#define LOADBALANCER_H

// create a queue that will hold the requests generated

// populate the queue statically at first, and then add requests at a given frequency

#include <string>
#include <deque>
#include "request.h"

class loadbalancer {
    
    deque<request> requests;
    
    int size() {
        return requests.size();
    }

    void addRequest(request r) {
        requests.push_back(r);
    }

    request popRequest() {
        if(requests.size()==0) {
            return NULL;
        } else {
            return requests.pop_front();
        }
    }

};

#endif