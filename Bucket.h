#pragma once
#include "Entity.h"

struct Bucket {

  Bucket() {}

  void SetEntity(Viper::Vec2 &position, Viper::Vec2 &size,
                 Viper::Vec3 &values) {
    manyEntity.AddEntity(position, size, values);
  }

  void SetHP(int index, float hp)
  {
    manyEntity.SetSpeed(index, hp);
  }

  void SetSpeed(int index, float speed)
  {
    manyEntity.SetSpeed(index, speed);
  }

  void RemoveEntity(int index) { manyEntity.RemoveNode(index); }

  void ClearEntities() {
    Entity *current = manyEntity.head;
    while (current) {
      Entity *toDelete = current;
      current = current->next;
      toDelete->next = nullptr;
      delete toDelete;
    }
    manyEntity.head = nullptr;
    manyEntity.tail = nullptr;
    manyEntity.index = 0;
  }

  void SetEntity(Viper::Vec2 &position, float radius, Viper::Vec3 &values) {
    otherManyEntity.AddEntity(position, radius, values);
  }

  void ClearOtherEntities() {
    EntityF *current = otherManyEntity.start.e; 
    while (current) {
      EntityF *toDelete = current;
      current = current->e;
      delete toDelete;
    }
    otherManyEntity.start.e = nullptr; 
    otherManyEntity.index = 0;        
  }

  LinkedEntity manyEntity;
  LinkedEntityF otherManyEntity;
};
