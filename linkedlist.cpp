#include "linkedlist.h"
#include <climits>
#include <iostream>
#include <string>
#include <utility>

LinkedList::~LinkedList()
{
    Node* temp;

    while (listHead != nullptr)
    {
        temp = listHead;
        listHead = listHead->next;
        delete temp;
    }
}

void LinkedList::insert(int data)
{
    listMutex.lock();

    Node* newNode = new Node(data);

    if (listHead == nullptr)
    {
        listHead = newNode;
        listTail = newNode;
    }
    else if (listHead->data >= newNode->data)
    {
        newNode->next = listHead;
        newNode->next->previous = newNode;
        listHead = newNode;
    }
    else
    {
        Node* curr = listHead;

        while (curr->next != nullptr && curr->next->data < newNode->data)
        {
            curr = curr->next;
        }

        newNode->next = curr->next;

        if (curr->next != nullptr) {
            newNode->next->previous = newNode;
        }

        curr->next = newNode;
        newNode->previous = curr;

        // In this case, we've reached the end of the list so
        // we need to update the tail node
        if (newNode->next == nullptr) {
            listTail = newNode;
        }
    }

    listSize++;
    listMutex.unlock();
}

void LinkedList::remove(int key)
{
    if (listHead == nullptr)
    {
        return;
    }
  
    Node* current = listHead;
    listMutex.lock();
  
    if (current->data == key)
    {
        Node* temp = listHead;
        listHead = listHead->next;

        delete temp;

        if (listHead != nullptr) {
            listHead->previous = nullptr;
        }

        listSize--;
    }
    else
    {
        while (current->next != nullptr)
        {
            if (current->next->data == key)
            {
                Node* temp = current->next;
                current->next = current->next->next;

                if (current->next != nullptr)
                {
                    current->next->previous = current;
                }

                delete temp;
                listSize--;
                break;
            }
            current = current->next;
        }
    }
    listMutex.unlock();
}

bool LinkedList::contains(int key)
{
    listMutex.lock();

    if (listHead == nullptr)
    {
        listMutex.unlock();
        return false;
    }

    Node* temp = listHead;

    while (temp != nullptr)
    {
        if (temp->data == key)
        {
            listMutex.unlock();
            return true;
        }

        temp = temp->next;
    }

    listMutex.unlock();
    return false;
}


int LinkedList::removeHead()
{
    listMutex.lock();

    if (listHead == nullptr)
    {
        listMutex.unlock();
        return INT_MIN;
    }

    int value = listHead->data;
    Node* temp = listHead;

    listHead = listHead->next;

    delete temp;

    if (listHead != nullptr)
    {
        listHead->previous = nullptr;
    }

    listSize--;
    listMutex.unlock();
    return value;
}

std::size_t LinkedList::size()
{
    return listSize;
}

bool LinkedList::empty()
{
    return listHead == nullptr;
}

