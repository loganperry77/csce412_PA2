#ifndef LOADBALANCER_H
#define LOADBALANCER_H

// create a queue that will hold the requests generated

// populate the queue statically at first, and then add requests at a given frequency

#include <string>
#include <deque>
#include "request.h"

using namespace std;

class loadbalancer {

    private:

    deque<request> requests;

    public:

    loadbalancer() {
        
    }
    
    int size() {
        return requests.size();
    }

    void addRequest(request r) {
        requests.push_back(r);
    }

    request popRequest() {
        request r = requests.front();
        requests.pop_front();
        return r;
    }

};

#endif