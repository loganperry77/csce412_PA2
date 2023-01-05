
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
vector<int> ssize;

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


void runClock(int clockCycles, int max) {
    int timeToSend = -1;

    int bigBurst = -1;

    srand(time(0));

    while(t<clockCycles) {

        if(t==timeToSend) {
            sendTasks(rand()%10, t);
            timeToSend = t + rand()%50 + 1;
        } else if(t > timeToSend) {
            timeToSend = t + rand()%50 + 1;
        }

        if(t==bigBurst) {
            sendTasks(rand()%1000+500, t);
            bigBurst = t + rand()%4000 + 1;
        } else if(t > bigBurst) {
            bigBurst = t + rand()%4000 + 1;
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

        if(t%1==0) {

            int diff = lb.size() - servers.size();

            if(diff > 0) {
                // add webservers
                if(servers.size() < max) {
                    int numToAdd = (max-servers.size())/50;
                    if(diff < numToAdd) {
                        numToAdd = diff;
                    }
                    for(int i = 0;i<numToAdd;i++) {
                        webserver* ws = new webserver(generateUniqueIP());
                        servers.push_back(ws);
                    }
                }
            } else {
                // delete some webservers
                if(servers.size() > 1) {
                    int numToSub = servers.size()-1;
                    diff *= -1;
                    if(diff < numToSub) {
                        numToSub = diff;
                    }
                    int num = 0;
                    for(int i = 0;i<servers.size();i++) {
                        if(servers[i]->checkStatus(t) == 1) {
                            continue;
                        }
                        num++;
                        servers.erase(servers.begin() + i);
                        if(num == numToSub) {
                            break;
                        }
                    }
                }
            }
        }

        ssize.push_back(servers.size());

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
    for(int i = 0;i<latencies.size();i+=50) {
        if(latencies[i]==0) {
            continue;
        }
        cout << "Cycle " << i << " ::";
        int t = latencies[i]/5;
        for(int x = 0;x<t;x++) {
            cout << "-";
        }
        cout << endl;
        usleep(10000);
    }
}

void printServerSizeGraph() {
    cout << "================ printing graph ================" << endl;
    sleep(1);
    for(int i = 0;i<ssize.size();i+=25) {
        if(ssize[i]==0) {
            continue;
        }
        cout << "Cycle " << i << " ::";
        int t = ssize[i]/5;
        for(int x = 0;x<t;x++) {
            cout << "-";
        }
        cout << endl;
        usleep(10000);
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

    runClock(15000, ns);

    int avgLat = 0;

    for(int i = 0;i<latencies.size();i++) {
        avgLat += latencies[i];
    }
    avgLat/=latencies.size();

    cout << "Queue size - " << lb.size() << ", average latency ratio - " << avgLat << endl;
    cout << "Webserver list size - " << servers.size() << endl;
    //printLatencyGraph();

    printServerSizeGraph();

    return 0;
}


