#ifndef LOADBALANCER_H
#define LOADBALANCER_H

// create a queue that will hold the requests generated

// populate the queue statically at first, and then add requests at a given frequency

#include <string>
#include <vector>
#include <deque>
#include <queue>
#include "request.h"

using namespace std;

class comparer {
public:
  template<typename T>
  bool operator()(T a, T b) {
    return (a.timeToRun) > (b.timeToRun);
  }
};

class loadbalancer {

    private:

    priority_queue<request, vector<request>, comparer> requests;

    public:

    loadbalancer() {
        
    }
    
    int size() {
        return requests.size();
    }

    void addRequest(request r) {
        requests.push(r);
    }

    request popRequest() {
        request r = requests.top();
        requests.pop();
        return r;
    }

};

#endif