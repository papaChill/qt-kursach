#ifndef COACHESUTILS_H
#define COACHESUTILS_H
#include "coaches.h"
#include "ht.h"

class CoachesUtils{
public:
    static bool insertCoaches(const Record& r, HashTable& ht, coachesStorage& c, QString* errorReason = nullptr);
    static bool removeCoaches(const Record& note, HashTable& ht, coachesStorage& storage);
    static Record* find(const std::string& key, HashTable& ht, coachesStorage& storage);
};

#endif // COACHESUTILS_H
