#pragma once
#include "Trig.h"

struct EntityF : public GameObject::SimpleObject {
  EntityF() { e = nullptr; }


  EntityF *e;
};

struct LinkedEntityF {

  LinkedEntityF() {}

  ~LinkedEntityF() {}

  void AddEntity(Viper::Vec2 &position, Viper::Vec2 &size,
                 Viper::Vec3 &values) {
    EntityF *current = &start;

    while (current->e != nullptr) {
      current = current->e;
    }
    index++;
    current->e = new EntityF();
    current->e->SetIndex(index);
    current->e->SetSize(size.x, size.y);
    current->e->SetPosition(position.x, position.y);
    current->e->SetColor(values);
    current->e->SetCenter(size, position);
  }

  void AddEntity(Viper::Vec2 &position, float radius, Viper::Vec3 &values) {
    EntityF *current = &start;

    while (current->e != nullptr) {
      current = current->e;
    }
    index++;
    current->e = new EntityF();
    current->e->SetIndex(index);
    current->e->SetRadius(radius);
    current->e->SetPosition(position.x, position.y);
    current->e->SetColor(values);
    current->e->SetCenter(radius, position);
  }

  EntityF *GetNode(int index) {
    EntityF *current = &start;
    for (int i = 0; i < index; i++) {
      if (current->e == nullptr)
        return nullptr;

      current = current->e;
    }
    return current;
  }

  void RemoveNode(int index) {
    if (index < 0)
      return;

    if (index == 0) {
      EntityF *nodeToRemove = start.e;
      if (nodeToRemove != nullptr) {
        index--;
        start.e = nodeToRemove->e;
        nodeToRemove->e = nullptr;

        delete nodeToRemove;
      }
      return;
    }

    EntityF *current = &start;
    for (int i = 0; i < index; i++) {
      if (current->e == nullptr)
        return;
      current = current->e;
    }

    EntityF *nodeToRemove = current->e;
    if (nodeToRemove == nullptr)
      return;
    index--;
    current->e = nodeToRemove->e;
    nodeToRemove->e = nullptr;
    delete nodeToRemove;
  }

  int index = 0;
  EntityF start;
};

struct Entity : public GameObject::SimpleObject {
  Entity() : next(nullptr) {}


  Entity *next;

};

struct LinkedEntity {

  ~LinkedEntity() {
    Entity *current = head;
    while (current) {
      Entity *next = current->next;
      delete current;
      current = next;
    }
  }

  void AddEntity(Viper::Vec2 &position, Viper::Vec2 &size,
                 Viper::Vec3 &values) {
    Entity *newNode = new Entity();
    index++;
    newNode->SetIndex(index);
    newNode->SetSize(size.x, size.y);
    newNode->SetPosition(position.x, position.y);
    newNode->SetColor(values);
    newNode->SetCenter(size, position);

    if (!head) {
      head = newNode;
      tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
  }

  Entity *GetNode(int index) {
    Entity *current = head;
    for (int i = 0; i < index; ++i) {
      if (!current)
        return nullptr;
      current = current->next;
    }
    return current;
  }

  void RemoveNode(int& index) {
  if (index < 0 || head == nullptr)
    return;

  
  if (index == 0) {
    Entity* nodeToRemove = head;
    head = head->next;
    
    if (head == nullptr)
      tail = nullptr;
    delete nodeToRemove;
    --index;
    return;
  }

  
  Entity* current = head;
  for (int i = 0; i < index - 1; ++i) {
    if (current->next == nullptr)
      return; 
    current = current->next;
  }

  Entity* nodeToRemove = current->next;
  if (nodeToRemove == nullptr)
    return;

  current->next = nodeToRemove->next;
  
  if (nodeToRemove == tail)
    tail = current;
  
  delete nodeToRemove;
  --index;
}


  Entity *head = nullptr;
  Entity *tail = nullptr;
  int index = 0;

};
