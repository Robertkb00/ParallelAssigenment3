#include "linkedlist.h"
#include <climits>
#include <iostream>
#include <string>
#include <utility>

LinkedList::~LinkedList()
{
    Node* temp;

    while (nodeHead != nullptr)
    {
        temp = nodeHead;
        nodeHead = nodeHead->next;
        delete temp;
    }
}

void LinkedList::insert(int data)
{
    nodeMutex.lock();

    Node* newNode = new Node(data);

    if (nodeHead == nullptr)
    {
        nodeHead = newNode;
        nodeTail = newNode;
    }
    else if (nodeHead->data >= newNode->data)
    {
        newNode->next = nodeHead;
        newNode->next->prev = newNode;
        nodeHead = newNode;
    }
    else
    {
        Node* curr = m_head;

        while (curr->next != nullptr && curr->next->data < newNode->data)
        {
            curr = curr->next;
        }

        newNode->next = curr->next;

        if (curr->next != nullptr) {
            newNode->next->prev = newNode;
        }

        curr->next = newNode;
        newNode->prev = curr;

        // In this case, we've reached the end of the list so
        // we need to update the tail node
        if (newNode->next == nullptr) {
            nodeTail = newNode;
        }
    }

    nodeSize++;
    nodeMutex.unlock();
}

void LinkedList::remove(int key)
{
    if (m_head == nullptr)
    {
        return;
    }
  
    Node* current = nodeHead;
    nodeMutex.lock();
  
    if (currnet->data == key)
    {
        Node* temp = nodeHead;
        nodeHead = nodeHead->next;

        delete temp;

        if (nodeHead != nullptr) {
            nodeHead->previous = nullptr;
        }

        nodeSize--;
    }
    else
    {
        while (curr->next != nullptr)
        {
            if (curr->next->data == key)
            {
                Node* temp = curr->next;
                curr->next = curr->next->next;

                if (curr->next != nullptr)
                {
                    curr->next->prev = curr;
                }

                delete temp;
                nodeSize--;
                break;
            }
            curr = curr->next;
        }
    }
    nodeMutex.unlock();
}

bool ConcurrentLinkedList::contains(int key)
{
    nodeMutex.lock();

    if (nodeHead == nullptr)
    {
        nodeMutex.unlock();
        return false;
    }

    Node* temp = nodeHead;

    while (temp != nullptr)
    {
        if (temp->data == key)
        {
            nodeMutex.unlock();
            return true;
        }

        temp = temp->next;
    }

    nodeMutex.unlock();
    return false;
}


int LinkedList::removeHead()
{
    nodeMutex.lock();

    if (nodeHead == nullptr)
    {
        nodeMutex.unlock();
        return INT_MIN;
    }

    int value = nodeHead->data;
    Node* temp = nodeHead;

    nodeHead = nodeHead->next;

    delete temp;

    if (nodeHead != nullptr)
    {
        nodeHead->prev = nullptr;
    }

    nodeSize--;
    nodeMutex.unlock();
    return value;
}

std::size_t LinkedList::size()
{
    return listtSize;
}

bool LinkedList::empty()
{
    return listHead == nullptr;
}

