#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;


struct List {
    struct Node {
        int data;
        Node* pNext;
        Node(Node* pNext = nullptr, int data = 0);
    };

    Node* head;
    List();
    ~List();

    void AddToEnd(int data);
    bool deletmean(int mean);
    int SizeL() const ;
    bool findL(int num) const ;
};

struct avl {
    struct node {
        string key;
        List* ids;
        int height;
        node* left;
        node* right;
        node(string k, int num);
    };

    node* root = nullptr;
    avl();

    static void returnArray(node* nod, List& arr);
    node* findNode(node* nod, const string& key);
    bool update(const string& key, int oldValue, int newValue);

    void insertt(string& key, int value);
    node* insert(node* nod, string& key, int value);
    static void search(node* nod, string& key, List& arr);

    node* getMin(node* nod);
    node* getMax(node* nod);

    void removeIdFromNode(const string& key, int value);
    node* deletee(node* nod, string& key, int value, bool fullDelete);
    node* deleteNodeCompletely(node* nod, const string& key);

    void updateHeight(node* nod);
    int getHeight(node* nod);
    int getBalance(node* nod);
    node* balance(node* nod);
    node* rightRotate(node* nod);
    node* leftRotate(node* nod);

    void loadFromFile(const string& filename);
};

#endif // AVL_H
