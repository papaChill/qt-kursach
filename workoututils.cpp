#include "workoututils.h"

string WorkOut::get_key() const {
    return surname +" "+ name +" "+ patronymic;
}

bool WorkoutUtils::validateWorkout(const WorkOut& note, const workoutStorage& storage, QString* errorReason) {

    QDate currentDate = QDate::currentDate();
    QDate maxDate = currentDate.addYears(1);

    if (note.date < currentDate) {
        if (errorReason) *errorReason = "error_date_in_past";
        return false;
    }

    if (note.date > maxDate) {
        if (errorReason) *errorReason = "error_date_too_far";
        return false;
    }
    for (int i = 0; i < storage.workoutcount; ++i) {
        const WorkOut& existing = storage.workout[i];
        if (existing.date == note.date && existing.time == note.time) {

            if (existing.surname == note.surname &&
                existing.name == note.name &&
                existing.patronymic == note.patronymic)
            {
                if (errorReason) *errorReason = "coach_busy";
                return false;
            }
            if (existing.group == note.group) {
                if (errorReason) *errorReason = "group_busy";
                return false;
            }
        }
    }

    return true;
}
void WorkoutUtils::insertWorkout(WorkOut& note, avl& avlTree, workoutStorage& storage, QString* errorReason) {

    if (storage.workoutcount >= MAX_COACHES) {
        if (errorReason) *errorReason = "storage_full";
        return;
    }
    if (!validateWorkout(note, storage, errorReason)) {
        return;
    }
    int idx = storage.workoutcount;
    storage.workout[idx] = note;
    storage.workoutcount++;
    string fio = note.get_key();
    avlTree.insertt(fio, idx);
    if (errorReason) *errorReason = "";
}

void WorkoutUtils::removeWorkout(string& fio, QDate date, avl& tree, workoutStorage& storage, QString* errorReason)
{
    if (!tree.root) {
        if (errorReason) *errorReason = "empty_tree";
        return;
    }

    // Ищем все индексы данного fio
    List arr;
    avl::search(tree.root, fio, arr);

    if (!arr.head) {
        if (errorReason) *errorReason = "not_found";
        return;
    }

    // ============================================================
    // 1) Если date пустая → УДАЛЕНИЕ ВСЕХ ЗАПИСЕЙ ДЛЯ ЭТОГО fio
    // ============================================================
    if (date.isNull() || date == QDate(0,0,0))
    {
        // Создаём отсортированный по убыванию список индексов (sorted)
        List sorted;

        for (List::Node* cur = arr.head; cur; cur = cur->pNext)
        {
            int val = cur->data;
            // вставка в sorted по убыванию
            if (!sorted.head || val > sorted.head->data) {
                sorted.head = new List::Node(sorted.head, val);
            } else {
                List::Node* t = sorted.head;
                while (t->pNext && t->pNext->data > val) t = t->pNext;
                t->pNext = new List::Node(t->pNext, val);
            }
        }

        // Проходим по отсортированному списку и удаляем индексы
        List::Node* nd = sorted.head;
        while (nd)
        {
            int idxToRemove = nd->data;

            if (idxToRemove >= 0 && idxToRemove < storage.workoutcount)
            {

                // 1) Удаляем связь fio -> idxToRemove (полностью, т.к. мы удаляем все записи fio)
                tree.root = tree.deletee(tree.root, fio, idxToRemove, true);
                int lastIndex = storage.workoutcount - 1;

                if (idxToRemove != lastIndex)
                {
                    // Перемещаем последний элемент в idxToRemove
                    WorkOut moved = storage.workout[lastIndex];
                    storage.workout[idxToRemove] = moved;
                    string movedKey = moved.get_key();

                    // ---- Надёжная логика обновления дерева для movedKey ----
                    if (movedKey == fio) {
                        // moved принадлежит тому же fio — не реинсертим его.
                        tree.root = tree.deletee(tree.root, movedKey, lastIndex, true);
                    } else {
                        bool ok = tree.update(movedKey, lastIndex, idxToRemove);
                        if (!ok) {
                            tree.insertt(movedKey, idxToRemove);
                        }
                    }
                }

                // уменьшить количество записей
                storage.workoutcount--;
            }

            nd = nd->pNext;
        }

        if (errorReason) *errorReason = "";
        return;
    }

    // -----------------------
    // УДАЛЕНИЕ ОДНОЙ ЗАПИСИ ПО ДАТЕ
    // -----------------------
    int idxToRemove = -1;
    for (List::Node* cur = arr.head; cur; cur = cur->pNext) {
        int idx = cur->data;
        if (idx >= 0 && idx < storage.workoutcount) {
            if (storage.workout[idx].date == date) {
                idxToRemove = idx;
                break;
            }
        }
    }
    if (idxToRemove == -1) {
        if (errorReason) *errorReason = "no_match_date";
        return;
    }

    int lastIndex = storage.workoutcount - 1;

    // СЛУЧАЙ 1: Удаляем последний элемент массива (перемещать ничего не нужно)
    if (idxToRemove == lastIndex) {
        bool wasLastInNode = (arr.SizeL() == 1);

        // ИСПРАВЛЕНИЕ: Если индексов много, не вызываем deletee (чтобы избежать балансировки)
        if (wasLastInNode) {
            tree.root = tree.deletee(tree.root, fio, idxToRemove, true);
        } else {
            tree.removeIdFromNode(fio, idxToRemove);
        }

        storage.workoutcount--;
        if (errorReason) *errorReason = "";
        return;
    }

    // СЛУЧАЙ 2: Удаляем из середины -> нужно переместить последний элемент (moved)
    WorkOut moved = storage.workout[lastIndex];
    string movedKey = moved.get_key();

    if (movedKey == fio) {

        avl::node* n = tree.findNode(tree.root, fio);
        storage.workout[idxToRemove] = moved;

        if (!n) {
            tree.insertt(fio, idxToRemove);
            storage.workoutcount--;
            if (errorReason) *errorReason = "";
            return;
        }

        bool replacedLast = false;
        List::Node* cur = n->ids->head;
        while (cur) {
            if (cur->data == lastIndex) {
                cur->data = idxToRemove;
                replacedLast = true;
                break;
            }
            cur = cur->pNext;
        }

        if (!replacedLast) {
            n->ids->AddToEnd(idxToRemove);
        }

        bool wasLastInNode = (arr.SizeL() == 1);
        if (wasLastInNode) {
            tree.root = tree.deletee(tree.root, fio, idxToRemove, true);
        } else {
            tree.removeIdFromNode(fio, idxToRemove);
        }

        storage.workoutcount--;
        if (errorReason) *errorReason = "";
        return;
    }


    // ============================================================
    // Обычный кейс: movedKey != fio
    // ГЛАВНОЕ ИСПРАВЛЕНИЕ ТУТ
    // ============================================================

    // 1) Вместо удаления и вставки используем update!
    // Это меняет lastIndex на idxToRemove внутри узла movedKey без перестройки дерева.
    bool updated = tree.update(movedKey, lastIndex, idxToRemove);

    if (!updated) {
        // На случай если update не сработал (чего быть не должно), делаем по старинке
        tree.root = tree.deletee(tree.root, movedKey, lastIndex, true);
        tree.insertt(movedKey, idxToRemove);
    }

    // 2) Перемещаем запись в массиве
    storage.workout[idxToRemove] = moved;

    // 3) Теперь удаляем id из узла fio (наш целевой удаляемый элемент)
    bool wasLastInNode = (arr.SizeL() == 1);

    if (wasLastInNode) {
        // Если это был единственный ID -> удаляем узел (тут балансировка неизбежна и нужна)
        tree.root = tree.deletee(tree.root, fio, idxToRemove, true);
    } else {
        // Если остались другие ID -> просто чистим список (структура дерева сохраняется!)
        tree.removeIdFromNode(fio, idxToRemove);
    }

    storage.workoutcount--;
    if (errorReason) *errorReason = "";
    return;
}



WorkOut WorkoutUtils::findByDate(string& fio, QDate date, avl& avlTree, workoutStorage& storage, QString* errorReason)
{
    WorkOut result;
    if (!avlTree.root) {
        if (errorReason) *errorReason = "empty_tree";
        return result;
    }
    List arr;
    avl::search(avlTree.root, fio, arr);
    if (!arr.head) {
        if (errorReason) *errorReason = "not_found";
        return result;
    }
    List::Node* current = arr.head;
    while (current) {
        int idx = current->data;
        if (idx >= 0 && idx < storage.workoutcount) {
            const WorkOut& w = storage.workout[idx];
            if (w.date == date) {
                if (errorReason) *errorReason = "";
                return w;
            }
        }
        current = current->pNext;
    }
    if (errorReason) *errorReason = "no_match_date";
    return result;
}

void WorkoutUtils::clearAvlTree(avl::node* node, workoutStorage& WorkoutStorage) {
    if (!node) return;
    clearAvlTree(node->left, WorkoutStorage);
    clearAvlTree(node->right, WorkoutStorage);
    if (node->ids) {
        List::Node* cur = node->ids->head;
        while (cur) {
            int idx = cur->data;
            if (idx >= 0 && idx < WorkoutStorage.workoutcount) {
                WorkoutStorage.workout[idx] = WorkOut();
            }
            cur = cur->pNext;
        }
        delete node->ids;
        node->ids = nullptr;
    }
    delete node;
}
