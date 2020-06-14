#include "Bucket.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cmath>

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
    this->recentlyPrinted = false;
};

Bucket::~Bucket() {
    
}

int Bucket::find(int hashedKey) {
    for(int i=0; i<this->occupancy; i++) {
        if(this->data[i] == hashedKey) {
            return i;
        }
    }

    return -1;
}

bool Bucket::insert(int hashedKey) {
    if(this->occupancy == this->capacity) {
        return false;
    }

    int insertLocation = this->occupancy;
    this->incrementOccupancy();
    this->setData(insertLocation, hashedKey);
    return true;
}

void Bucket::setData(int index, int value) {
    if(index >= this->occupancy || index < 0) {
        throw runtime_error("setData: invalid index, did you incrementOccupancy?");
    }

    this->data[index] = value;
}

void Bucket::incrementOccupancy() {
    if(this->occupancy == this->capacity) {
        throw runtime_error("incrementOccupancy: bucket full");
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
        int lastValue = this->data[occupancy-1];
        this->setData(keyLocation, lastValue);
        this->decrementOccupancy();
    }

    return true;
}

string Bucket::print() {
    if(recentlyPrinted == true) {
        return "";
    }
    stringstream output;
    
    string bucketContents = "[";
    for(int i=0; i<this->capacity; i++) {
        if(i < this->occupancy) {
            bucketContents += to_string(this->data[i]) + ",";
        }
        else {
            bucketContents += "-,";
        }
    }
    bucketContents.replace(bucketContents.length()-1, bucketContents.length(), "] ");
    output << bucketContents;
    output << "(" << to_string(this->localDepth) << ")";

    this->recentlyPrinted = true;
    return output.str();
}

void Bucket::resetPrintStatus() {
    this->recentlyPrinted = false;
}

shared_ptr<Bucket> Bucket::splitBucket(int naughtKeyPattern, int oneKeyPattern, int(*maskBits)(int, int)) {
    if(this->occupancy != this->capacity) {
        throw runtime_error("splitBucket: trying to split a bucket that is not full");
    }
    if(naughtKeyPattern + pow(2, this->localDepth) != oneKeyPattern) {
        throw invalid_argument("splitBucket: split paramaters are wrong");
    }

    int newLocalDepth = this->localDepth + 1;
    this->localDepth = newLocalDepth;
    shared_ptr<Bucket> newBucket = shared_ptr<Bucket>(new Bucket(this->capacity, newLocalDepth));

    for(int i=this->occupancy-1; i>=0; i--) {
        int keyPattern = maskBits(this->data[i], newLocalDepth);

        if(keyPattern == oneKeyPattern) {
            newBucket->insert(this->data[i]);
            this->remove(this->data[i]);
        }
        else if(keyPattern != naughtKeyPattern) {
            throw runtime_error("splitBucket: contents not consistent");
        }
    }

    return newBucket;
}

int Bucket::getLocalDepth() {
    return this->localDepth;
}