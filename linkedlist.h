#include <iostream>
#include <mutex>
#include <cstddef>
#include <memory>

class Node
{
public:
    int data;
    Node* next;
    Node* previous;
    std::mutex mutex;

    Node(int n) : data(n), next(nullptr), previous(nullptr){};
    ~Node() {}

    friend std::ostream& operator<<(std::ostream& os, Node* const& node);
};

class LinkedList
{
private:
    Node* listHead;
    Node* listTail;
    size_t listSize;
    std::mutex listMutex;
    
public:
    LinkedList() : listHead(nullptr), listTail(nullptr), listSize(0){};
    ~LinkedList();

    void insert(int data);
    void remove(int key);
    bool contains(int data);
    int removeHead();
    std::size_t size();
    bool empty();

    friend std::ostream& operator<<(std::ostream& os, std::unique_ptr<LinkedList> const& list);
};
