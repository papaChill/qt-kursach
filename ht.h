#ifndef HT_H
#define HT_H

#include <string>
#include <iostream>
#include <QString>
using namespace std;


enum SlotState { EMPTY, OCCUPIED, DELETED };

struct HashEntry {
    int data;
    string key;
    SlotState state = EMPTY;
    HashEntry() : data(), key(""), state(EMPTY) {}
};

class HashTable {
public:
    HashEntry* table;
    int size;
    int k1=1, k2=3;

    int primaryHash(const string& key) const;
    int secondaryHash(int primaryHash, int j) const;
    int resolveCollusion(int primaryIdx, string& key);

    HashTable(int sz);
    ~HashTable();

    void clear();
    bool update(const string& key, int newValue);
    bool add(string key, int id, QString* err);
    bool remove(string key, int* value);
    int search(string key);

    int getSize() const { return size; }
    bool isEmpty();

private:
};

#endif // HT_H
