
#include "coachesutils.h"
#include "constants.h"

string Record::get_key() const {
    return surname +" "+ name +" "+ patronymic;
}

bool CoachesUtils::insertCoaches(const Record& note, HashTable& ht, coachesStorage& storage, QString* errorReason){
    int idx = -1;

    if (storage.coachesCount < MAX_COACHES){
        idx = storage.coachesCount++;
        storage.coaches[idx] = note;
    } else {
        if (errorReason) *errorReason = "storage_full";
        return false;
    }
    string fio = note.get_key();
    bool inserted = ht.add(fio, idx, errorReason);
    if (!inserted) {
        storage.coachesCount--;
    }
    return inserted;
}

bool CoachesUtils::removeCoaches(const Record& note, HashTable& ht, coachesStorage& storage) {
    string fio = note.get_key();
    int value = -1;
    int temp = ht.search(fio);
    if (temp < 0 || temp >= storage.coachesCount) {
        return false;
    }
    Record& coach = storage.coaches[temp];
    if (note.spec != coach.spec || note.exp != coach.exp) {
        return false;
    }
    bool deleted = ht.remove(fio, &value);
    if (!deleted) {
        return false;
    }
    if (value != storage.coachesCount - 1) {
        storage.coaches[value] = storage.coaches[storage.coachesCount - 1];
        string movedFio = storage.coaches[value].get_key();
        ht.update(movedFio, value);
    }
    storage.coachesCount--;
    return true;
}

Record* CoachesUtils::find(const string& key, HashTable& ht, coachesStorage& storage){
    int idx = ht.search(key);
    if (idx < 0 || idx >= storage.coachesCount) return nullptr;
    return &storage.coaches[idx];
}


