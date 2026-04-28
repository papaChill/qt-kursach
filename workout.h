#ifndef WORKOUT_H
#define WORKOUT_H
#include <QString>
#include "constants.h"
#include "tree.h"
#include <QDate>

struct WorkOut{
    string name;
    string surname;
    string patronymic;
    string group;
    string time;
    QDate date;

    string get_key() const;
};
struct workoutStorage{
    WorkOut workout[MAX_COACHES];
    int workoutcount = 0;
};


#endif // WORKOUT_H
