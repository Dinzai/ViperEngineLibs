#pragma once
#include "Trig.h"

struct Entity : public GameObject::SimpleObject
{
    Entity()
    {
        e = nullptr;
    }

    float speed = 500;
    Entity* e;

};

struct LinkedEntity 
{

  LinkedEntity() {}

  ~LinkedEntity() {}

  void AddEntity(GameObject::Vec2& position, GameObject::Vec2& size, GameObject::Vec3& values) 
  {
    Entity *current = &start;

    while (current->e != nullptr) 
    {
      current = current->e;
    }
      index++;
      current->e = new Entity();
      current->e->SetIndex(index);
      current->e->SetSize(size.x, size.y);
      current->e->SetPosition(position.x, position.y);
      current->e->SetColor(values);
      current->e->SetCenter(size, position);
    

  }

  void AddEntity(GameObject::Vec2& position, float radius, GameObject::Vec3& values) 
  {
    Entity *current = &start;

    while (current->e != nullptr) 
    {
      current = current->e;
    }
      index++;
      current->e = new Entity();
      current->e->SetIndex(index);
      current->e->SetRadius(radius);
      current->e->SetPosition(position.x, position.y);
      current->e->SetColor(values);
      current->e->SetCenter(radius, position);
    

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
        index--;
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
    index--;
  }


  int index = 0;
  Entity start;
};