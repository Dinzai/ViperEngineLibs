#pragma once
#include "Trig.h"

struct Entity : public GameObject::SimpleObject
{
    Entity()
    {
        e = nullptr;
    }

    float speed = 200;
    Entity* e;

};

struct LinkedEntity 
{

  LinkedEntity() {}

  ~LinkedEntity() {}

  void AddEntity() {
    Entity *current = &start;

    while (current->e != nullptr) 
    {
      current->e->SetSize(10, 10);
      current->e->SetPosition(200, 200);
      current = current->e;
    }

    current->e = new Entity();
    current->e->SetSize(10, 10);
    current->e->SetPosition(200, 200);
    

  }

  Entity* GetNode(int index)
  {
    Entity *current = &start;
    for (int i = 0; i < index; i++) 
    {
      if (current->e == nullptr)
        return nullptr;
     
       current = current->e;
    }
    return current;
  }

  void RemoveNode(int index) {
    if (index < 0)
      return;

    if (index == 0) 
    {
      Entity *nodeToRemove = start.e;
      if (nodeToRemove != nullptr) {
        start.e = nodeToRemove->e;
        nodeToRemove->e = nullptr;
        delete nodeToRemove;
      }
      return;
    }

    Entity *current = &start;
    for (int i = 0; i < index; i++) {
      if (current->e == nullptr)
        return;
      current = current->e;
    }

    Entity *nodeToRemove = current->e;
    if (nodeToRemove == nullptr)
      return;

    current->e = nodeToRemove->e;
    nodeToRemove->e = nullptr;
    delete nodeToRemove;
  }



  Entity start;
};