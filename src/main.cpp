#include <iostream>
#include "ExtensibleHashTable.h"

using namespace std;

int main() {
    ExtensibleHashTable* hTable = new ExtensibleHashTable(3);
    // hTable->insert(1);
    // hTable->insert(2);
    // hTable->insert(3);
    // hTable->insert(4);
    // hTable->insert(5);
    // hTable->insert(6);
    // hTable->insert(7);
    // hTable->insert(8);
    // hTable->insert(9);
    // hTable->insert(2);
    // hTable->insert(2);
    // hTable->insert(2);
    // hTable->insert(10);
    //hTable->insert(2);

    hTable->insert(64);
    hTable->insert(200);
    hTable->insert(153);
    hTable->insert(66);
    hTable->insert(218);
    hTable->insert(67);
    hTable->insert(13);
    hTable->insert(253);
    hTable->insert(109);
    hTable->print();

    cout << "success" << endl;
}