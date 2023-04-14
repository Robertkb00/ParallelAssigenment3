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

    void LinkedList::insert(int data);
    void LinkedList::remove(int key);
    bool LinkedList::contains(int data);
    int LinkedList::removeHead();
    std::size_t size();
    bool LinkedList::empty();

    friend std::ostream& operator<<(std::ostream& os, std::unique_ptr<LinkedList> const& list);
};
