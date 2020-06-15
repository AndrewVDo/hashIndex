#include <iostream>
#include "ExtensibleHashTable.h"
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <fstream>

using namespace std;

void defaultConstructorTest(int numRuns) {
    ExtensibleHashTable* hTable = new ExtensibleHashTable();
    for(int i=0; i<numRuns; i++) {
        hTable->insert(i);
    }
    for(int i=0; i<numRuns; i++) {
        bool result = hTable->find(i);

        if(result == false) {
            hTable->print();
            throw runtime_error("Could Not find " + to_string(i));
        }
    }

    hTable->print();
    cout << __func__ << ": Success" << endl;
}

void customConstructorTest(int keysPerBucket, int numRuns) {
    ExtensibleHashTable* hTable = new ExtensibleHashTable(keysPerBucket);
    for(int i=0; i<numRuns; i++) {
        hTable->insert(i);
    }
    for(int i=0; i<numRuns; i++) {
        bool result = hTable->find(i);

        if(result == false) {
            hTable->print();
            throw runtime_error("Could Not find " + to_string(i));
        }
    }

    hTable->print();
    cout << __func__ << ": Success" << endl;
}

void randomInsertTest(int keysPerBucket, int numRuns) {
    vector<int> insertedValues;
    ExtensibleHashTable* hTable = new ExtensibleHashTable(keysPerBucket);

    try {
        for(int i=0; i<numRuns; i++) {
            int randInt = rand() % 997;
            insertedValues.push_back(randInt);
            hTable->insert(randInt);
        }

        for(int i=0; i<numRuns; i++) {
            bool result = hTable->find(insertedValues[i]);

            if(result == false) {
                hTable->print();
                throw runtime_error("Could Not find " + to_string(i));
            }
        }
    }
    catch(const std::exception &exc) {
        ofstream LOG("error-log.txt");
        LOG << "Vector: { ";
        for(int i=0; i<insertedValues.size(); i++) {
            LOG << insertedValues[i] << " ";
        }
        LOG << "} " << insertedValues.size() << endl;
        hTable->print(LOG);

        cerr << exc.what() << endl;
        return;
    }

    hTable->print();
    cout << __func__ << ": Success" << endl;
}

void deleteTest(int keysPerBucket, int numRuns) {
    vector<int> insertedValues;
    ExtensibleHashTable* hTable = new ExtensibleHashTable(keysPerBucket);

    try {
        for(int i=0; i<numRuns; i++) {
            int randInt = rand() % 997;
            insertedValues.push_back(randInt);
            hTable->insert(randInt);
        }

        for(int i=0; i<997; i++) {
            bool result = hTable->find(i);
            bool result2 = hTable->remove(i);
            bool result3 = hTable->find(i);

            if(
                (result == true && result2 == true && result3 == true)
            ) {
                hTable->print();
                throw runtime_error("Could Not find " + to_string(i));
            }
        }
    }
    catch(const std::exception &exc) {
        ofstream LOG("error-log.txt");
        LOG << "Vector: { ";
        for(int i=0; i<insertedValues.size(); i++) {
            LOG << insertedValues[i] << " ";
        }
        LOG << "} " << insertedValues.size() << endl;
        hTable->print(LOG);

        cerr << exc.what() << endl;
        return;
    }

    hTable->print();
    cout << __func__ << ": Success" << endl;
}

void copyTest(int numRuns) {
    ExtensibleHashTable* hTable = new ExtensibleHashTable();
    for(int i=0; i<numRuns; i++) {
        hTable->insert(i);
    }
    for(int i=0; i<numRuns; i++) {
        bool result = hTable->find(i);

        if(result == false) {
            hTable->print();
            throw runtime_error("Could Not find " + to_string(i));
        }
    }

    ExtensibleHashTable* hTable2 = new ExtensibleHashTable(*hTable);

    for(int i=0; i<numRuns; i++) {
        hTable->remove(i);
    }

    hTable->print();
    hTable2->print();
    cout << __func__ << ": Success" << endl;
}

int main() {
    copyTest(1000);

    return 0;
}