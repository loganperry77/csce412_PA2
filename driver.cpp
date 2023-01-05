/*Code cleaned and commented in Doxygen by ChatGPT*/

#include "loadbalancer.h"
#include "webserver.h"
#include "request.h"

#include <vector>
#include <iostream>
#include <random>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// A vector containing all of the web servers in the system.
vector<webserver*> servers;

// The load balancer that distributes tasks to the web servers.
loadbalancer lb;

// The current time in the simulation.
int t = 0;

// Vectors to store the latencies and size of the system at each time step.
vector<int> latencies;
vector<int> ssize;
vector<int> inactiveCount;

/**
 * Generates a unique IP address in the format "255.x.y.z", where x, y, and z
 * are integers between 0 and 255.
 *
 * @return The generated IP address.
 */
string generateUniqueIP() {
    // 255.255.255.255

    int a = 255;
    int b = rand() % 256;
    int c = rand() % 256;
    int d = rand() % 256;

    return to_string(a)+"."+to_string(b)+"."+to_string(c)+"."+to_string(d);

}

/**
 * Generates a specified number of random requests and adds them to the load balancer.
 *
 * @param i The number of requests to generate.
 */
void sendTasks(int i) {
    for(int x = 0; x < i; x++) {
        request r(generateUniqueIP(), rand() % 245 + 1, 0);
        lb.addRequest(r);
    }
}

/**
 * Generates a specified number of random requests, with a specified processing time,
 * and adds them to the load balancer.
 *
 * @param i The number of requests to generate.
 * @param t The processing time for the requests.
 */
void sendTasks(int i, int t) {
    for(int x = 0; x < i; x++) {
        request r(generateUniqueIP(), rand() % 245 + 1, t);
        lb.addRequest(r);
    }
}

/**
 * The main function that simulates the passage of time. At each time step, it
 * checks the status of each web server, adds new web servers if necessary, removes
 * excess web servers if there are too many, and assigns tasks to the web servers if
 * they are available. The simulation runs for a specified number of time steps, and
 * the maximum number of web servers that can be created is also specified.
 *
 * @param clockCycles The number of time steps to run the simulation.
 * @param max The maximum number of web servers that can be created.
 */
void runClock(int clockCycles, int max) {
    // The time when the next burst of tasks will be sent.
    int timeToSend = -1;

    // The time of the next big burst of tasks.
    int bigBurst = -1;

    // Seed the random number generator.
    srand(time(0));

    // Run
    while(t < clockCycles) {
        // Send a burst of tasks at the appropriate time.
        if(t == timeToSend) {
            sendTasks(rand() % 2, t);
            timeToSend = t + rand() % 10 + 1;
        } else if(t > timeToSend) {
            timeToSend = t + rand() % 10 + 1;
        }

        // Send a big burst of tasks at the appropriate time.
        // if(t == bigBurst) {
        //     sendTasks(rand() % 5000 + 500, t);
        //     bigBurst = t + rand() % 4000 + 1;
        // } else if(t > bigBurst) {
        //     bigBurst = t + rand() % 4000 + 1;
        // }

        // Visit all servers to check their status.

        // Initialize the latency for this time step to 0.
        latencies.push_back(0);

        for(webserver* ws : servers) {
            // Check the status of the server.
            int result = ws->checkStatus(t);

            // If the server has finished processing a task, record the latency.
            if(result == 0) {
                int latency = (t - ws->getCT().timeMade) / (ws->getCT().timeToRun);
                latencies[t] = latency;
            }

            // If the server is idle, try to assign it a task.
            if(result == 2) {
                if(lb.size() != 0) {
                    request r = lb.popRequest();
                    ws->runTask(r, t);
                } 
            }
        }

        // Check if we need to add or remove web servers.
        if(t % 1 == 0) {
            int diff = lb.size() - servers.size();

            // Add web servers if there are more tasks than servers.
            if(diff > 0) {
                // Don't add more servers than the maximum allowed.
                if(servers.size() < max) {
                    int numToAdd = (max - servers.size()) / 50;
                    // if(diff < numToAdd) {
                    //     numToAdd = diff;
                    // }
                    // if(5 < numToAdd) {
                    //     numToAdd = 5;
                    // }
                    for(int i = 0; i < numToAdd; i++) {
                        webserver* ws = new webserver(generateUniqueIP());
                        servers.push_back(ws);
                    }
                }
            }
            // Remove web servers if there are more servers than tasks.
            else {
                // Don't remove all of the servers.
                if(servers.size() > 1) {
                    int numToSub = servers.size() - 1;
                    diff *= -1;
                    if(diff < numToSub) {
                        numToSub = diff;
                    }
                    int num = 0;
                    int numToKeep = (int)max*.1;
                    for(int i = 0; i < servers.size(); i++) {
                        if(servers[i]->isIdle(t) && numToKeep--<0) {
                            delete servers[i];
                            servers.erase(servers.begin() + i);
                        
                        i--;
                            num++;
                            if(num>=numToSub) {
                                break;
                            }
                        }
                    }
                }
            }

            int inactive = 0;

            for(webserver* ws : servers) {
                if(ws->isIdle(t)) {
                    inactive++;
                }
            }

            // Record the size of the system at this time step.
            ssize.push_back(servers.size());
            inactiveCount.push_back(inactive);
        }

        // Increment the time.
        t++;
    }

}
/**
 * Prints out a graph of the latencies at each time step.
 */
void printLatencyGraph() {
    cout << "================ printing graph ================" << endl;
    sleep(1);

    // Iterate through the latencies in steps of 50.
    for(int i = 0; i < latencies.size(); i += 50) {
        // Skip latencies that are zero.
        if(latencies[i] == 0) {
            continue;
        }

        cout << "Cycle " << i << " ::";

        // Calculate the number of dashes to print based on the latency.
        int t = latencies[i] / 5;
        for(int x = 0; x < t; x++) {
            cout << "-";
        }
        cout << endl;
        usleep(20000);
    }
}

/**
 * Prints out a graph of the size of the system at each time step.
 *
 * @param max The maximum size of the system.
 */
void printServerSizeGraph(int max) {
    cout << "================ printing graph ================" << endl;
    sleep(1);

    // Iterate through the sizes in steps of 25.
    for(int i = 0; i < ssize.size(); i += 25) {
        // Skip sizes that are zero.
        if(ssize[i] == 0) {
            continue;
        }
        //cout << "Cycle " << i << " ::";

        printf("Cycle %7d ::", i);

        // Calculate the number of dashes to print based on the size of the system.
        float t = (float)(ssize[i]-inactiveCount[i]) / max * 70;
        for(int x = 0; x < t; x++) {
            cout << "\033[1;32m-\033[0m";
        }
        float t1 = (float)(inactiveCount[i]) / max * 70;
        for(int x = 0; x < t1; x++) {
            cout << "\033[1;31m-\033[0m";
        }
        cout << endl;
        usleep(10000);
    }
}

/**
 * The main function that runs the simulation.
 *
 * @return 0 on success, non-zero on failure.
 */
int main() {
    // Prompt the user for the number of servers to create.
    cout << "Enter the number of servers to create: " << endl;
    int ns;
    cin >> ns;

    // Create the web servers.
    for(int i = 0; i < ns; i++) {
        webserver* ws = new webserver(generateUniqueIP());
        servers.push_back(ws);
    }

    // Generate some initial tasks and add them to the load balancer.
    sendTasks(20 * ns);
    cout << "Queue size - " << lb.size() << endl;
    cout << "Task runtime range " << 1 << " - " << 245  << endl;

    // Run the simulation for 15000 time steps, allowing up to the specified number of web servers to be created.
    runClock(10000, ns);

    // Calculate the average latency ratio.
    int avgLat = 0;
    for(int i = 0; i < latencies.size(); i++) {
        avgLat += latencies[i];
    }
    avgLat /= latencies.size();

    // Print out the queue size, average latency ratio, and number of web servers.
    cout << "Queue size - " << lb.size() << endl;
    cout << "Webserver list size - " << servers.size() << endl;

    //printServerSizeGraph(ns);

    return 0;
}


