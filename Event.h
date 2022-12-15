//
// Created by audre on 12/7/2022.
//

#ifndef SIMULATIONFINAL_EVENT_H
#define SIMULATIONFINAL_EVENT_H
#include <string>

using namespace std;
class Event {


public:
    Event();
    Event(int At, string Type, int NumCars);
    double at;
    string type;
    int num_cars;
    int order_number;
    inline bool operator<(const Event& someEvent) const {
        if(type=="Delivery" && someEvent.type=="Delivery"){
            return order_number<someEvent.order_number;
        }
        return at < someEvent.at;
    }

    inline bool operator>(const Event& someEvent) const {
        if(type=="Delivery" && someEvent.type=="Delivery"){
            return order_number>someEvent.order_number;
        }
        return at > someEvent.at;
    }
};


#endif //SIMULATIONFINAL_EVENT_H
