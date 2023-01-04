
#include "loadbalancer.h"
#include "webserver.h"
#include "request.h"

#include <vector>
#include <iostream>
#include <random>
#include <unistd.h>

using namespace std;

vector<webserver*> servers;

loadbalancer lb;

int t = 0;

vector<int> latencies;

string generateUniqueIP() {
    // 255.255.255.255

    int a = 255;
    int b = rand() % 256;
    int c = rand() % 256;
    int d = rand() % 256;

    return to_string(a)+"."+to_string(b)+"."+to_string(c)+"."+to_string(d);

}

void sendTasks(int i) {
    for(int x = 0;x<i;x++) {
        request r(generateUniqueIP(), rand()%200 + 1, 0);
        lb.addRequest(r);
    }
}

void sendTasks(int i, int t) {
    for(int x = 0;x<i;x++) {
        request r(generateUniqueIP(), rand()%200 + 1, t);
        lb.addRequest(r);
    }
}


void runClock(int clockCycles) {
    int timeToSend = -1;
    while(t<clockCycles) {

        if(t==timeToSend) {

            sendTasks(rand()%10, t);

        } else if(t > timeToSend) {
            timeToSend = t + rand()%10;
        }

        // visit all servers to check status

        latencies.push_back(0);

        for(webserver* ws: servers) {
            int result = ws->checkStatus(t);

            if(result==0) {
                int latency = (t-ws->getCT().timeMade)/(ws->getCT().timeToRun);
                latencies[t] = latency;
            }

            if(result==2) {
                // try to assign a task

                if(lb.size()!=0) {
                    request r = lb.popRequest();
                    ws->runTask(r, t);
                    //cout << "From = " << r.IPin << ", To = " << ws->name() << " | time made = " << r.timeMade << " | time to run = " << r.timeToRun << endl;
                } 

                // log the task IP/server IP, made time and start time

            }
        }

        // print out the queue size

        t++;
        //cout << endl;
        //cout << "Clock cycle -> " << t << endl;
        //cout << "loadbalancer size -> " << lb.size() << endl;

    }
}

void printLatencyGraph() {
    cout << "================ printing graph ================" << endl;
    sleep(1);
    for(int i = 0;i<latencies.size();i++) {
        if(latencies[i]==0) {
            continue;
        }
        cout << "Cycle " << i << " ::";
        int t = latencies[i];
        for(int x = 0;x<t;x++) {
            cout << "-";
        }
        cout << endl;
        usleep(2500);
    }
}


int main() {

    // prompt the user for the number of servers
    cout << "Enter the number of servers to create: " << endl;
    int ns;
    cin >> ns;

    // create the servers

    for(int i = 0;i<ns;i++) {
        webserver* ws = new webserver(generateUniqueIP());
        servers.push_back(ws);
    }

    // populate the loadbalancer with an intial amount of tasks

    sendTasks(20*ns);

    runClock(5000);


    printLatencyGraph();

    return 0;
}


