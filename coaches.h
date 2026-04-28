#ifndef COACHES_H
#define COACHES_H
#include <QString>
#include "constants.h"
#include "ht.h"

struct Record {
    string surname;
    string name;
    string patronymic;
    string spec;
    int exp;

    string get_key() const;
};

struct coachesStorage{
    Record coaches[MAX_COACHES];
    int coachesCount = 0;
};

#endif // COACHES_H
