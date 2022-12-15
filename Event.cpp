//
// Created by audre on 12/7/2022.
//

#include "Event.h"
Event::Event() {}

Event::Event(int At, string Type, int NumCars) {
    at=At;
    type=Type;
    num_cars=NumCars;
}