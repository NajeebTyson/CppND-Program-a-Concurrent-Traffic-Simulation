#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <random>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum class TrafficLightPhase {
    green,
    red
};


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

class MessageQueue
{
public:
    void send(TrafficLightPhase &&lightPhase);

    TrafficLightPhase receive();

private:
    std::mutex _mutex;
    std::condition_variable _condition;
    std::deque<TrafficLightPhase> _queue;
    
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 

class TrafficLight: public TrafficObject, public std::enable_shared_from_this<TrafficLight>
{
public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();

    void simulate();

    // miscellaneous
    std::shared_ptr<TrafficLight> get_shared_this() { return shared_from_this(); }

private:
    [[noreturn]] // typical behaviour methods
    void cycleThroughPhases();

    // helper functions
    static int getRandomNumber(int start, int last);

    static std::random_device _rd;
    static std::mt19937 _eng;

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.
    MessageQueue _message_queue;

    std::condition_variable _condition;
    std::mutex _mutex;

    TrafficLightPhase _currentPhase;
};

#endif