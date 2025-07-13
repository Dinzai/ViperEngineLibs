#include <cstddef>
#include <iostream>


/*
  MIT Licence
Author -> Brett Rogers

Reference: Chatgpt & stackoverflow, Simple Linked List implmentation


If i could write a linked list with no references, that would be cool...

*/


namespace TreeList
{
struct Node 
{

  Node() { next = nullptr; }

  ~Node() { delete next; }

  int data;
  Node *next;
};

struct LinkedNodes 
{

  LinkedNodes() {}

  ~LinkedNodes() {}

  void AddNode(int value) {
    Node *current = &start;

    while (current->next != nullptr) {
      current = current->next;
    }

    current->next = new Node();
    current->next->data = value;
  }

  void RemoveNode(int index) {
    if (index < 0)
      return;

    if (index == 0) {
      Node *nodeToRemove = start.next;
      if (nodeToRemove != nullptr) {
        start.next = nodeToRemove->next;
        nodeToRemove->next = nullptr;
        delete nodeToRemove;
      }
      return;
    }

    Node *current = &start;
    for (int i = 0; i < index; i++) {
      if (current->next == nullptr)
        return;
      current = current->next;
    }

    Node *nodeToRemove = current->next;
    if (nodeToRemove == nullptr)
      return;

    current->next = nodeToRemove->next;
    nodeToRemove->next = nullptr;
    delete nodeToRemove;
  }

  void PrintNodes() {
    Node *current = start.next;

    while (current != nullptr) {
      std::cout << current->data << '\n';
      current = current->next;
    }
  }

  Node start;
};
}