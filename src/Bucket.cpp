#include "Bucket.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

using namespace std;

Bucket::Bucket(int capacity, int localDepth) {
    if(capacity < 1) {
        throw invalid_argument("invalid capacity");
    }
    if(localDepth < 1) {
        throw invalid_argument("invalid localDepth");
    }

    this->localDepth = localDepth;
    this->data = unique_ptr<int []>(new int[capacity]());
    this->occupancy = 0;
    this->capacity = capacity;
};

int Bucket::find(int hashedKey) {
    int* dataArray = this->data.get();

    for(int i=0; i<this->occupancy; i++) {
        if(dataArray[i] == hashedKey) {
            return i;
        }
    }

    return -1;
}

bool Bucket::insert(int hashedKey) {
    if(occupancy == capacity) {
        return false;
    }

    int insertLocation = this->occupancy;
    this->incrementOccupancy();
    this->setData(insertLocation, hashedKey);
    return true;
}

void Bucket::setData(int index, int value) {
    if(index >= this->occupancy || index < 0) {
        throw invalid_argument("setData: invalid index, did you incrementOccupancy?");
    }

    int* dataArray = this->data.get();
    dataArray[index] = value;
}

void Bucket::incrementOccupancy() {
    if(this->occupancy == this->capacity) {
        throw range_error("incrementOccupancy: bucket full");
    }

    this->occupancy += 1;
}

void Bucket::decrementOccupancy() {
    if(this->occupancy == 0) {
        throw range_error("decrementOccupancy: bucket empty");
    }

    this->occupancy -= 1;
}

bool Bucket::remove(int hashedKey) {
    int keyLocation = this->find(hashedKey);
    if(keyLocation < 0) {
        return false;
    }

    if(this->occupancy-1 == keyLocation) {
        this->decrementOccupancy();
    }
    else {
        int* dataArray = this->data.get();
        int lastValue = dataArray[occupancy-1];
        this->setData(keyLocation, lastValue);
        this->decrementOccupancy();
    }

    return true;
}

string Bucket::print() {
    stringstream output;
    
    int* dataArray = this->data.get();
    string bucketContents = "[";
    for(int i=0; i<this->capacity; i++) {
        if(i < this->occupancy) {
            bucketContents += to_string(dataArray[i]) + ",";
        }
        else {
            bucketContents += "-,";
        }
    }
    bucketContents.replace(bucketContents.length()-1, bucketContents.length(), "] ");
    output << bucketContents;
    output << "(" << to_string(this->localDepth) << ")";

    return output.str();
}
