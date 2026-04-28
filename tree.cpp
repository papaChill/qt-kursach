
#include "tree.h"


List::Node::Node(Node* pNext, int data) : pNext(pNext), data(data) {}

List::List() : head(nullptr) {}

List::~List() {
    Node* current = head;
    while (current) {
        Node* next = current->pNext;
        delete current;
        current = next;
    }
    head=nullptr;
}

void List::AddToEnd(int data) {
    if (!head) {
        head = new Node(nullptr, data);
    } else {
        Node* current = head;
        while (current->pNext) current = current->pNext;
        current->pNext = new Node(nullptr, data);
    }
}

bool List::deletmean(int mean) {
    if (!head) return false;

    // Если голова совпадает — удаляем голову и возвращаем true
    if (head->data == mean) {
        List::Node* tmp = head;
        head = head->pNext;
        delete tmp;
        return true;
    }

    // Ищем первое вхождение в остальной части списка
    List::Node* cur = head;
    while (cur->pNext) {
        if (cur->pNext->data == mean) {
            List::Node* tmp = cur->pNext;
            cur->pNext = tmp->pNext;
            delete tmp;
            return true;
        }
        cur = cur->pNext;
    }

    // не найдено
    return false;
}

int List::SizeL() const {
    Node* cur = head;
    int cnt = 0;
    while (cur) { cnt++; cur = cur->pNext; }
    return cnt;
}

bool List::findL(int num) const {
    Node* cur = head;
    while (cur) {
        if (cur->data == num) return true;
        cur = cur->pNext;
    }
    return false;
}


avl::node::node(string k, int num) : key(move(k)), left(nullptr), right(nullptr), height(0) {
    ids = new List();
    ids->AddToEnd(num);
}

avl::avl() : root(nullptr) {}

avl::node* avl::findNode(node* nod, const string& key) {
    if (!nod) return nullptr;
    if (key < nod->key) return findNode(nod->left, key);
    if (key > nod->key) return findNode(nod->right, key);
    return nod;
}


bool avl::update(const std::string& key, int oldValue, int newValue) {
    node* current = root;
    while (current) {
        if (key < current->key) current = current->left;
        else if (key > current->key) current = current->right;
        else {

            bool found = current->ids->findL(oldValue);
            if (!found) {
                return false;
            }

            bool removed = current->ids->deletmean(oldValue);
            if (!removed) {
                return false;
            }
            current->ids->AddToEnd(newValue);
            return true;
        }
    }
    return false;
}
void avl::returnArray(node* nod, List& arr)
{
    //надо возвращать массив при ключе//обход как
    if (!nod) return;
    List::Node* current = nod->ids->head;
    while(current!=nullptr){
        arr.AddToEnd(current->data);
        current = current->pNext;
    }
    returnArray(nod->left, arr);
    returnArray(nod->right, arr);
}

void avl::insertt(string& key, int value) {
    root = insert(root, key, value);
}

avl::node* avl::insert(node* nod, string& key, int value) {
    if (!nod) return new node(key, value);

    if (key < nod->key)
        nod->left = insert(nod->left, key, value);
    else if (key > nod->key)
        nod->right = insert(nod->right, key, value);
    else {
        nod->ids->AddToEnd(value);
        return nod;
    }

    updateHeight(nod);
    return balance(nod);
}

void avl::search(node* nod, string& key, List& arr) {
    if (!nod) return;
    if (key < nod->key) return search(nod->left, key, arr);
    else if (key > nod->key) return search(nod->right, key, arr);
    else {
        List::Node* current = nod->ids->head;
        while(current!=nullptr){
            arr.AddToEnd(current->data);
            current = current->pNext;
        }
        return;
    }
}

avl::node* avl::getMin(node* nod) {
    if (!nod) return nullptr;
    while (nod->left) nod = nod->left;
    return nod;
}

avl::node* avl::getMax(node* nod) {
    if (!nod) return nullptr;
    while (nod->right) nod = nod->right;
    return nod;
}

void avl::removeIdFromNode(const string& key, int value) {
    // Используем твой существующий findNode (он ищет без рекурсии или с ней, но возвращает узел)
    node* target = findNode(root, key);

    if (target && target->ids) {
        // Просто удаляем значение из списка
        target->ids->deletmean(value);

        // ВАЖНО: Мы НЕ вызываем здесь updateHeight или balance.
        // Так как мы не удалили сам узел target, его высота и баланс не изменились.
    }
}

avl::node* avl::deletee(node* nod, string& key, int value, bool fullDelete)
{
    if (!nod) return nullptr;

    if (key < nod->key) {
        nod->left = deletee(nod->left, key, value, fullDelete);
        updateHeight(nod);
        return balance(nod);
    }
    else if (key > nod->key) {
        nod->right = deletee(nod->right, key, value, fullDelete);
        updateHeight(nod);
        return balance(nod);
    }

    // Узел найден
    // 1. Пытаемся удалить значение из списка
    if (nod->ids) {
        nod->ids->deletmean(value);
    }

    // 2. Проверяем текущий размер списка
    int sz = nod->ids ? nod->ids->SizeL() : 0;

    // Если список НЕ пуст, мы просто обновляем баланс и выходим
    // (даже если fullDelete=true, но список не пуст — удалять узел нельзя, данные потеряются)
    if (sz > 0) {
        updateHeight(nod);
        return balance(nod);
    }

    // 3. Если список ПУСТ (sz == 0), узел нужно удалить из дерева,
    //    соблюдая правила BST (сохраняя детей).

    // Случай А: Нет детей или только один ребенок
    if (!nod->left || !nod->right) {
        node* temp = nod->left ? nod->left : nod->right;
        if (nod->ids) delete nod->ids;
        delete nod;
        return temp;
    }

    // Случай Б: Два ребенка. Ищем преемника (минимальный в правом поддереве)
    node* minInRight = getMin(nod->right);
    string succKey = minInRight->key;

    // Переносим данные преемника в текущий узел
    delete nod->ids;
    nod->ids = new List();
    // Копируем список индексов преемника
    if (minInRight->ids && minInRight->ids->head) {
        List::Node* curr = minInRight->ids->head;
        while(curr) {
            nod->ids->AddToEnd(curr->data);
            curr = curr->pNext;
        }
    }
    nod->key = succKey;

    // Рекурсивно удаляем старый узел преемника
    nod->right = deleteNodeCompletely(nod->right, succKey);

    updateHeight(nod);
    return balance(nod);
}



avl::node* avl::deleteNodeCompletely(node* nod, const string& key) {
    if (!nod) return nullptr;

    if (key < nod->key) {
        nod->left = deleteNodeCompletely(nod->left, key);
        updateHeight(nod);
        return balance(nod);
    } else if (key > nod->key) {
        nod->right = deleteNodeCompletely(nod->right, key);
        updateHeight(nod);
        return balance(nod);
    } else {
        // нашли узел который хотим стереть целиком
        if (!nod->left || !nod->right) {
            node* temp = nod->left ? nod->left : nod->right;
            delete nod->ids;
            delete nod;
            return temp;
        } else {
            // два ребёнка: заменяем текущий узел минимальным в правом поддереве
            node* minInRight = getMin(nod->right);
            if (!minInRight) {
                node* temp = nod->left ? nod->left : nod->right;
                delete nod->ids;
                delete nod;
                return temp;
            }
            string succKey = minInRight->key;


            delete nod->ids;
            nod->key = succKey;

            nod->ids = new List();
            for (List::Node* c = minInRight->ids->head; c; c = c->pNext) {
                nod->ids->AddToEnd(c->data);
            }

            nod->right = deleteNodeCompletely(nod->right, succKey);
            updateHeight(nod);
            return balance(nod);
        }
    }
}


void avl::updateHeight(node* nod) {
    nod->height = max(getHeight(nod->left), getHeight(nod->right)) + 1;
}

int avl::getHeight(node* nod) {
    return nod ? nod->height : -1;
}

int avl::getBalance(node* nod) {
    return nod ? (getHeight(nod->right) - getHeight(nod->left)) : 0;
}

avl::node* avl::balance(node* nod) {
    int b = getBalance(nod);
    if (b == -2) {
        if (getBalance(nod->left) == 1)
            nod->left = leftRotate(nod->left);
        return rightRotate(nod);
    } else if (b == 2) {
        if (getBalance(nod->right) == -1)
            nod->right = rightRotate(nod->right);
        return leftRotate(nod);
    }
    return nod;
}

avl::node* avl::rightRotate(node* nod) {
    node* newRoot = nod->left;
    nod->left = newRoot->right;
    newRoot->right = nod;
    updateHeight(nod);
    updateHeight(newRoot);
    return newRoot;
}

avl::node* avl::leftRotate(node* nod) {
    node* newRoot = nod->right;
    nod->right = newRoot->left;
    newRoot->left = nod;
    updateHeight(nod);
    updateHeight(newRoot);
    return newRoot;
}

void avl::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return;
    }
    int value;
    string key;
    while (fin >> key >> value) {
        insertt(key, value);
    }
    fin.close();
}


