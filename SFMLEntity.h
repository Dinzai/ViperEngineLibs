#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

struct Chunk : public sf::RectangleShape, public GameObject::SimpleObject {
  Chunk() { c = nullptr; }

  Chunk *c;
};

struct LinkedChunk {

  LinkedChunk() {}

  ~LinkedChunk() {
    Chunk *current = start.c;
    while (current != nullptr) {
      Chunk *next = current->c;
      delete current;
      current = next;
    }
    start.c = nullptr;
  }

  void AddEntity(sf::RectangleShape &shape) {
    Chunk *current = &start;

    while (current->c != nullptr) {
      current = current->c;
    }
    index++;
    current->c = new Chunk();
    current->c->setSize(shape.getSize());
    current->c->setPosition(shape.getPosition());
    current->c->setFillColor(shape.getFillColor());
    current->c->setOutlineThickness(shape.getOutlineThickness());
    current->c->setOutlineColor(sf::Color::White);
  }

  void SetHealth(int index, float hp) {
    Chunk *current = GetNode(index);
    current->SetHealth(hp);
  }

  void SetSpeed(int index, float speed) {
    Chunk *current = GetNode(index);
    current->SetSpeed(speed);
  }

  Chunk *GetNode(int index) {
    Chunk *current = &start;
    for (int i = 0; i < index; i++) {
      if (current->c == nullptr)
        return nullptr;

      current = current->c;
    }
    return current;
  }

  void RemoveNode(int i) {
    if (i < 0 || i >= index)
      return;

    if (i == 0) {
      Chunk *nodeToRemove = start.c;
      if (nodeToRemove != nullptr) {
        start.c = nodeToRemove->c;
        delete nodeToRemove;
        --index; 
      }
      return;
    }

    Chunk *current = &start;
    for (int j = 0; j < i - 1; ++j) {
      if (current->c == nullptr)
        return;
      current = current->c;
    }

    Chunk *nodeToRemove = current->c;
    if (nodeToRemove == nullptr)
      return;

    current->c = nodeToRemove->c;
    delete nodeToRemove;
    --index; 
  }
  

  void DrawAll(sf::RenderWindow &window) {
    Chunk *current = start.c;

    while (current != nullptr) {
      window.draw(*current);
      current = current->c;
    }
  }

  Chunk start;
  int index = 0;
};