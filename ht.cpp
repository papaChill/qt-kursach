#include "ht.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <QString>
using namespace std;

    int HashTable::primaryHash(const string& key) const {
        unsigned long hash = 0;
        for (char c : key) {
            hash = hash + c;
        }
        return hash % size;
    }

    int HashTable::secondaryHash(int primaryHash, int j) const {
        return (primaryHash + j * k1 + j * j * k2) % size;
    }

    int HashTable::resolveCollusion(int primaryIdx, string& key) {
        int i = 0, idx;
        int deletedCell = -3;
        idx = primaryIdx;
        while (i < size && table[idx].state != EMPTY) {
            if (table[idx].state == DELETED && deletedCell == -3) {
                deletedCell = idx;
            }
            if (table[idx].state == OCCUPIED && table[idx].key  == key) {
                return -1; // ошибка дупликат
            }
            idx = secondaryHash(primaryIdx, i);
            i++;
        }
        if (i == size) {
            if (deletedCell == -3) {
                return -2; // ошибка заполнено
            }
            else {
                return deletedCell;
            }
        }
        else {
            if (deletedCell == -3) {
                return idx;
            }
            else {
                return deletedCell;
            }
        }
    }

    HashTable::HashTable(int sz) {
        size = sz;
        table = new HashEntry[size];
    }

    HashTable::~HashTable() {
        delete[] table;
    }

    void HashTable::clear() {
        for (int i = 0; i < size; ++i) {
            table[i].state = EMPTY;
            table[i].data = 0;
        }
    }

    bool HashTable::add(string key, int id, QString* err) {

        int primaryIdx = primaryHash(key);

        if (table[primaryIdx].state == EMPTY || table[primaryIdx].state == DELETED) {
            table[primaryIdx].state = OCCUPIED;
            table[primaryIdx].data=id;
            table[primaryIdx].key = key;
            return true;
        }
        else {
            int idx = resolveCollusion(primaryIdx, key);
            if (idx == -1) {
                if (err) *err = "duplicate";
                return false; // дубликат
            }
            if (idx == -2) {
                if (err) *err = "overflow";
                return false; // таблица заполнена
            }
            table[idx].state = OCCUPIED;
            table[idx].data=id;
            table[idx].key = key;
            return true;
        }

    }

    bool HashTable::remove(string key, int* value) {

        int primaryIdx = primaryHash(key);

        int i = 0, idx = primaryIdx;

        while (i < size && table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].key == key) {

                *value = table[idx].data;
                table[idx].state = DELETED;
                //table[idx].data = Record();
                return true;
            }
            idx = secondaryHash(primaryIdx, ++i);
        }
        return false; // не найден
    }

    bool HashTable::update(const string& key, int newValue) {
        int primaryIdx = primaryHash(key);
        int i = 0, idx = primaryIdx;

        while (i < size && table[idx].state != EMPTY) {
            if (table[idx].state == OCCUPIED && table[idx].key == key) {
                table[idx].data = newValue;
                return true;
            }
            idx = secondaryHash(primaryIdx, ++i);
        }
        return false; // не найден
    }

    int HashTable::search(string key) {

        int count = 0, primaryIdx = primaryHash(key);

        int i = 0, idx = primaryIdx;
        while (i < size && table[idx].state != EMPTY) {
            count++;
            if (table[idx].state == OCCUPIED && table[idx].key == key) {

                // table[idx].data.hashSell = idx;
                // table[idx].data.countt = count;

                return table[idx].data;
            }
            idx = secondaryHash(primaryIdx, ++i);
        }
        return 0;
    }

    bool HashTable::isEmpty() {
        for (int i = 0; i < size; ++i) {
            if (table[i].state == OCCUPIED) return false;
        }
        return true;
    }


