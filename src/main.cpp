#include <iostream>
#include "Bucket.h"

using namespace std;

int main() {
    //Bucket* a = new Bucket(0, 2);
    //Bucket* b = new Bucket(2, 0);
    Bucket* c = new Bucket(5, 1);
    cout << c->print() << endl;
    c->insert(5);
    c->insert(10);
    c->insert(15);
    c->insert(20);
    c->insert(25);
    cout << c->print() << endl;
    c->remove(20);
    cout << c->print() << endl;
    c->remove(10);
    cout << c->print() << endl;
    c->remove(5);
    cout << c->print() << endl;
    c->insert(100);
    cout << c->print() << endl;

    cout << "success" << endl;
}