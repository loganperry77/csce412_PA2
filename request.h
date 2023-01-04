#ifndef REQUEST_H
#define REQUEST_H

// hold the IP in, IP out, and time for the task to run

#include <string>

using namespace std;

class request {
    private:

    public:

    string IPin;
    string IPout;

    int timeToRun;

    int timeMade;
    int timeDone;

    request() {
        
    }

    request(string ip, int runTime, int tM) {
        IPin = ip;
        timeToRun = runTime;
        timeMade = tM;
    }
};

#endif