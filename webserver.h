
#ifndef WEBSERVER_H
#define WEBSERVER_H

// sleep for the time of a request

#include <string>
#include "request.h"
#include <iostream>

using namespace std;

class webserver {
    private:

    string IP;

    int startTime = -1;
    int endTime = -1;

    request currentTask;

    public:

    webserver(string ip) {
        IP = ip;
    }

    void runTask(request r, int start) {
        r.IPout = IP;
        startTime = start;
        endTime = start+r.timeToRun;
        currentTask = r;
    }

    request getCT() {
        return currentTask;
    }

    int checkStatus(int t) {
        if(t==endTime) {
            cout << "Task completed -> " << currentTask.IPin << " at server " << name() << ", latency ratio = " << (t-currentTask.timeMade)/currentTask.timeToRun << endl;
            return 0;
        }
        if(t<endTime) {
            return 1;
        } else {
            return 2;
        }
    }

    bool isIdle(int t) {
        if(checkStatus(t)!=1) {
            return true;
        }
        return false;
    }

    string name() {
        return IP;
    }

};

#endif