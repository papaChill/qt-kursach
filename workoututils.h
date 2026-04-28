#ifndef WORKOUTUTILS_H
#define WORKOUTUTILS_H

#include "workout.h"

class WorkoutUtils{
public:
    static void insertWorkout(WorkOut& record, avl& avlTree, workoutStorage& storage, QString* errorReason);
    static void removeWorkout(string& fio, QDate date, avl& tree, workoutStorage& storage, QString* errorReason);
    static WorkOut findByDate(string& fio, QDate date, avl& avlTree, workoutStorage& storage, QString* errorReason);
    static void clearAvlTree(avl::node* node, workoutStorage& WorkoutStorage);
    static bool validateWorkout(const WorkOut& note, const workoutStorage& storage, QString* errorReason);
};


#endif // WORKOUTUTILS_H
