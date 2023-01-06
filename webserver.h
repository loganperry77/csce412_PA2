/*Code cleaned and commented in Doxygen by ChatGPT*/
/**

    @file webserver.h
    @brief Header file for webserver class.
    The webserver class represents a web server that can run tasks in the form of requests. It has member functions for running a task, checking the status of the current task, and querying whether the server is idle.
    */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include "request.h"
#include <iostream>

using namespace std;

/**

    @class webserver

    @brief Class representing a web server.

    The webserver class represents a web server that can run tasks in the form of requests. It has member functions for running a task, checking the status of the current task, and querying whether the server is idle.
    */
    class webserver {
    private:

    /**
        @brief IP address of the web server.
        */
        string IP;

    /**
        @brief Time the current task started running.
        */
        int startTime = -1;

    /**
        @brief Time the current task is expected to finish running.
        */
        int endTime = -1;

    /**
        @brief Current task being run by the web server.
        */
        request currentTask;

    public:

    /**
        @brief Constructor taking in the IP address of the web server.
        @param ip IP address of the web server.
        */
        webserver(string ip) {
        IP = ip;
        }

    /**
        @brief Run a task on the web server.
        @param r Request to run as a task.
        @param start Time the task starts running.
        */
        void runTask(request r, int start) {
        r.IPout = IP;
        startTime = start;
        endTime = start+r.timeToRun;
        currentTask = r;
        }

    /**
        @brief Get the current task being run by the web server.
        @return The current task being run by the web server.
        */
        request getCT() {
        return currentTask;
        }

    /**
        @brief Check the status of the current task.
        @param t Current time.
        @return 0 if the task is complete, 1 if the task is running, 2 if the task has not started yet.
        */
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

    /**
        @brief Check if the web server is idle.
        @param t Current time.
        @return True if the web server is idle, false if it is running a task.
        */
        bool isIdle(int t) {
        if(checkStatus(t)!=1) {
        return true;
        }
        return false;
        }

        /**
        * @brief Get the IP address of the web server.
        * 
        * @return The IP address of the web server.
        */
        string name() {
            return IP;
        }

        };

#endif