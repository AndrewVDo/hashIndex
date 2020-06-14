#include "ExtensibleHashTable.h"
#include <memory>
#include <cmath>
#include <iostream>

using namespace std;

ExtensibleHashTable::ExtensibleHashTable() {
    ExtensibleHashTable(4);
    //not working
}

ExtensibleHashTable::ExtensibleHashTable(int keysPerBucket) {
    this->globalDepth = 1;
    this->directory = unique_ptr< shared_ptr<Bucket> []>(new shared_ptr<Bucket> [2]);

    for(int i=0; i<2; i++) {
        this->directory[i] = shared_ptr<Bucket>(new Bucket(keysPerBucket, 1));
    }
}

ExtensibleHashTable::~ExtensibleHashTable() {

}

bool ExtensibleHashTable::find(int key) {
    //int hashedKey = this->hash(key);
    int numDirectories = pow(2, this->globalDepth);

    for(int i=0; i<numDirectories; i++) {
        shared_ptr<Bucket> currentBucket = this->directory[i];
        int result = currentBucket->find(key);

        if(result >= 0) {
            return true;
        }
    }

    return false;
}

void ExtensibleHashTable::insert(int key) {
    //int hashedKey = this->hash(key);
    int numTries = 10;
    while(numTries > 0) {
        //shallow means it grabs the targetBucket but is not aware of localDepth (how many bits are currently being used)
        int shallowTargetIndex = maskBits(key, this->globalDepth);
        shared_ptr<Bucket> targetBucket = this->directory[shallowTargetIndex];
        int deepTargetIndex = maskBits(key, targetBucket->getLocalDepth());
        bool result = targetBucket->insert(key);
        //cout << "result " << result << " deepTargetIndex " << deepTargetIndex << endl;

        if(result == false) {
            numTries--;
            //splitBucket automatically increments globalDepth if needed
            this->splitBucket(deepTargetIndex);
        }
        else {
            break;
        }
    }

    if(numTries == 0) {
        throw runtime_error("insert: ran out of tries, too many duplicate keys");
    }
}

// ...0000 - 0
//         -> ...0010 - 2
//         -> ...0000 - 0
void ExtensibleHashTable::splitBucket(int bucketIndex) {
    shared_ptr<Bucket> existingBucket = this->directory[bucketIndex];

    int oldLocalDepth = existingBucket->getLocalDepth();
    int newBucketIndex = bucketIndex + pow(2, oldLocalDepth);
    shared_ptr<Bucket> newBucket = existingBucket->splitBucket(bucketIndex, newBucketIndex, *this->maskBits);

    if(oldLocalDepth == this->globalDepth) {
        this->incrementGlobalDepth();
    }
    this->directory[newBucketIndex] = newBucket;

    //update all descending directories
    int numDirectories = pow(2, this->globalDepth);
    for(int i=newBucketIndex+1; i<numDirectories; i++) {
        int testMaskBits = maskBits(i, oldLocalDepth+1);
        if(testMaskBits == newBucketIndex) {
            this->directory[i] = newBucket;
        }
    }
}

int ExtensibleHashTable::maskBits(int hashedKey, int numBits) {
    int bitMask = pow(2, numBits) - 1;
    int maskedHashKey = hashedKey & bitMask;

    return maskedHashKey;
}

void ExtensibleHashTable::incrementGlobalDepth() {
    int oldGlobalDepth = this->globalDepth;
    int newGlobalDepth = this->globalDepth + 1;

    int oldDirectoryCapacity = pow(2, oldGlobalDepth);
    int newDirectoryCapacity = pow(2, newGlobalDepth);

    unique_ptr< shared_ptr<Bucket> []> newDirectory
        = unique_ptr< shared_ptr<Bucket> []>(new shared_ptr<Bucket> [newDirectoryCapacity]);

    for(int i=0; i<oldDirectoryCapacity; i++) {
        newDirectory[i] = this->directory[i];
    }

    for(int i=oldDirectoryCapacity; i<newDirectoryCapacity; i++) {
        int existingBucketIndex = this->maskBits(i, oldGlobalDepth);
        newDirectory[i] = this->directory[existingBucketIndex];
    }

    this->directory = move(newDirectory);
    this->globalDepth = newGlobalDepth;
}

bool ExtensibleHashTable::remove(int key) {
    int targetBucketIndex = this->maskBits(key, this->globalDepth);

    bool result = true;
    while(result == true) {
        result = this->directory[targetBucketIndex]->remove(key);
    }
}

void ExtensibleHashTable::print() {
    int numDirectories = pow(2, this->globalDepth);

    for(int i=0; i<numDirectories; i++) {
        cout << i << ": " << this->directory[i].get() << " --> " << this->directory[i]->print() << endl;
    }

    for(int i=0; i<numDirectories; i++) {
        this->directory[i]->resetPrintStatus();
    }
}