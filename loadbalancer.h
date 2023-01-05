/**

    @file loadbalancer.h
    @brief Header file for loadbalancer class.
    The loadbalancer class is used to manage a queue of requests. It has member functions for adding requests to the queue and removing them, as well as querying the size of the queue. The requests are stored in a priority queue, with the request that has the smallest timeToRun value being at the top of the queue.
    */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <string>
#include <vector>
#include <queue>
#include "request.h"

using namespace std;

/**

    @class comparer
    @brief Functor for comparing requests based on their timeToRun values.
    The comparer class is used as a comparator for the priority_queue in the loadbalancer class. It defines a comparison operator that compares the timeToRun values of two requests.
    */
    class comparer {
    public:
    /*
        @brief Comparison operator for requests.
        @param a First request to compare.
        @param b Second request to compare.
        @return True if the timeToRun value of request a is greater than that of request b, false otherwise.
        */
        template<typename T>
        bool operator()(T a, T b) {
        return (a.timeToRun) > (b.timeToRun);
        }
        };

/**

    @class loadbalancer

    @brief Class for managing a queue of requests.

    The loadbalancer class stores requests in a priority queue, with the request that has the smallest timeToRun value being at the top of the queue. It has member functions for adding requests to the queue and removing them, as well as querying the size of the queue.
    */
    class loadbalancer {

    private:

    /**
        @brief Priority queue of requests.
        The requests are ordered by their timeToRun values, with the request having the smallest timeToRun value being at the top of the queue.
        */
        priority_queue<request, vector<request>, comparer> requests;

    public:

    /**
        @brief Default constructor.
        Initializes the priority queue of requests.
        */
        loadbalancer() {

    }

    /**
        @brief Get the size of the queue.
        @return The number of requests in the queue.
        */
        int size() {
        return requests.size();
        }

    /**
        @brief Add a request to the queue.
        @param r Request to add to the queue.
        */
        void addRequest(request r) {
        requests.push(r);
        }

    /**
        @brief Remove and return the top request from the queue.
        @return The request at the top of the queue.
        */
        request popRequest() {
        request r = requests.top();
        requests.pop();
        return r;
        }

};

#endif