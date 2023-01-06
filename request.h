/*Code cleaned and commented in Doxygen by ChatGPT*/
/**

    @file request.h
    @brief Header file for request class.
    The request class is used to store information about a task that needs to be run. It holds the IP address of the sender and receiver, the time the task needs to run, and the time that the task was made and completed.
    */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

using namespace std;

/**

    @class request

    @brief Class for storing information about a task to be run.

    The request class stores the IP addresses of the sender and receiver, the time the task needs to run, and the time the task was made and completed. It has a default constructor and a constructor that takes in the IP address, run time, and time made as arguments.
    */
    class request {
    private:

    public:

    /**
        @brief IP address of the sender.
        */
        string IPin;

    /**
        @brief IP address of the receiver.
        */
        string IPout;

    

    // Public member variables

    /**
        @brief Time the task needs to run.
        */
        int timeToRun;

    /**
        @brief Time the task was made.
        */
        int timeMade;

    /**
        @brief Time the task was completed.
        */
        int timeDone;

    /**
        @brief Default constructor.
        Initializes the member variables to default values.
        */
        request() {

    }

    /**
        @brief Constructor taking in IP address, run time, and time made as arguments.
        @param ip IP address of the sender.
        @param runTime Time the task needs to run.
        @param tM Time the task was made.
        */
        request(string ip, int runTime, int tM) {
        IPin = ip;
        timeToRun = runTime;
        timeMade = tM;
        }
        };

#endif