#include "ExtensibleHashTable.h"
#include <memory>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

ExtensibleHashTable::ExtensibleHashTable() {
    this->initialize(4);
}

ExtensibleHashTable::ExtensibleHashTable(int keysPerBucket) {
    this->initialize(keysPerBucket);
}

void ExtensibleHashTable::initialize(int keysPerBucket) {
    this->keysPerBucket     =   keysPerBucket;
    this->globalDepth       =   1;
    this->numDirectories    =   pow(2, this->globalDepth);
    this->directory         =   unique_ptr< shared_ptr<Bucket> []>(new shared_ptr<Bucket> [2]);

    for(int i=0; i<2; i++) {
        this->directory[i]  =   shared_ptr<Bucket>(new Bucket(keysPerBucket, 1));
    }
}

ExtensibleHashTable::ExtensibleHashTable(const ExtensibleHashTable &copy) {
    int keysPerBucket = copy.keysPerBucket;
    this->initialize(copy.keysPerBucket);
    while(this->globalDepth != copy.globalDepth) {
        this->incrementGlobalDepth();
    }

    for(int i=0; i<copy.numDirectories; i++) {
        shared_ptr<Bucket> existingBucket   =   copy.directory[i];

        for(int j=1; j<=copy.globalDepth; j++) {
            int parentIndex                 =   maskBits(i, j);
            shared_ptr<Bucket> bucketParent =   copy.directory[parentIndex];

            if(bucketParent == existingBucket) {
                this->directory[i]          =   this->directory[parentIndex];
                break;
            }
        }
        this->directory[i]                  =   shared_ptr<Bucket>(new Bucket(*(existingBucket.get())));
    }
}

ExtensibleHashTable::~ExtensibleHashTable() {
    //no need to do anything since all resources are managed by smart-pointers
}

bool ExtensibleHashTable::find(int key) {
    for(int i=0; i<this->numDirectories; i++) {
        shared_ptr<Bucket> currentBucket = this->directory[i];
        int result = currentBucket->find(key);

        if(result >= 0) {
            return true;
        }
    }

    return false;
}

void ExtensibleHashTable::insert(int key) {
    int numTries = 10;
    while(numTries > 0) {
        int globalDepthIndex                    =   maskBits(key, this->globalDepth);
        shared_ptr<Bucket> globalDepthBucket    =   this->directory[globalDepthIndex];
        int localDepthIndex                     =   maskBits(key, globalDepthBucket->getLocalDepth());
        bool result                             =   globalDepthBucket->insert(key);

        if(result == true) {
            break;
        }

        // bucket was full
        numTries--;
        this->splitBucket(localDepthIndex);
    }

    if(numTries == 0) {
        throw runtime_error("insert: ran out of tries, too many duplicate keys");
    }
}

void ExtensibleHashTable::splitBucket(int bucketIndex) {
    shared_ptr<Bucket> existingBucket   =   this->directory[bucketIndex];

    int oldLocalDepth                   =   existingBucket->getLocalDepth();
    int newBucketIndex                  =   bucketIndex + pow(2, oldLocalDepth);
    shared_ptr<Bucket> newBucket        =   existingBucket->splitBucket(bucketIndex, newBucketIndex, *this->maskBits);

    if(oldLocalDepth == this->globalDepth) {
        this->incrementGlobalDepth();
    }
    this->directory[newBucketIndex] = newBucket;

    this->updateDirectory(newBucketIndex, oldLocalDepth);
}

void ExtensibleHashTable::updateDirectory(int newBucketIndex, int oldLocalDepth) {
    for(int i=newBucketIndex+1; i<this->numDirectories; i++) {
        int testMaskBits = this->maskBits(i, oldLocalDepth+1);
        if(testMaskBits == newBucketIndex) {
            this->directory[i] = this->directory[newBucketIndex];
        }
    }
}

int ExtensibleHashTable::maskBits(int hashedKey, int numBits) {
    int bitMask = pow(2, numBits) - 1;
    int maskedHashKey = hashedKey & bitMask;

    return maskedHashKey;
}

void ExtensibleHashTable::incrementGlobalDepth() {
    int oldGlobalDepth          =   this->globalDepth;
    int newGlobalDepth          =   this->globalDepth + 1;

    int oldDirectoryCapacity    =   pow(2, oldGlobalDepth);
    int newDirectoryCapacity    =   pow(2, newGlobalDepth);

    unique_ptr< shared_ptr<Bucket> []> newDirectory
        = unique_ptr< shared_ptr<Bucket> []>(new shared_ptr<Bucket> [newDirectoryCapacity]);

    for(int i=0; i<oldDirectoryCapacity; i++) {
        newDirectory[i]     =   this->directory[i];
    }

    for(int i=oldDirectoryCapacity; i<newDirectoryCapacity; i++) {
        int existingBucketIndex     =   maskBits(i, oldGlobalDepth);
        newDirectory[i]             =   this->directory[existingBucketIndex];
    }

    this->directory         =   move(newDirectory);
    this->globalDepth       =   newGlobalDepth;
    this->numDirectories    =   pow(2, this->globalDepth);
}

bool ExtensibleHashTable::remove(int key) {
    int targetBucketIndex   =   maskBits(key, this->globalDepth);

    bool result             =   true;
    int numRemoved          =   0;

    while(result == true) {
        result  =   this->directory[targetBucketIndex]->remove(key);
        
        if(result == true) {
            numRemoved++;
        }
    }

    return static_cast<bool>(numRemoved);
}

void ExtensibleHashTable::print() {
    //remove this
    cout << this->directory.get() << endl;

    for(int i=0; i<this->numDirectories; i++) {
        cout << i << ": " << this->directory[i].get() << " --> " << this->directory[i]->print() << endl;
    }

    for(int i=0; i<this->numDirectories; i++) {
        this->directory[i]->resetPrintStatus();
    }
}

void ExtensibleHashTable::print(ofstream& outFile) {
    for(int i=0; i<this->numDirectories; i++) {
        outFile << i << ": " << this->directory[i].get() << " --> " << this->directory[i]->print() << endl;
    }

    for(int i=0; i<this->numDirectories; i++) {
        this->directory[i]->resetPrintStatus();
    }
}