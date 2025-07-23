
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#define ScreenWidth 800
#define ScreenHeight 600

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "Basic.h"
#include "Entity.h"


struct Chunk : public sf::RectangleShape {
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

    current->c = new Chunk();
    current->c->setSize(shape.getSize());
    current->c->setPosition(shape.getPosition());
    current->c->setFillColor(sf::Color::Black);
    current->c->setOutlineThickness(2.0);
    current->c->setOutlineColor(sf::Color::White);
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

  void RemoveNode(int index) {
    if (index < 0)
      return;

    if (index == 0) {
      Chunk *nodeToRemove = start.c;
      if (nodeToRemove != nullptr) {
        start.c = nodeToRemove->c;
        nodeToRemove->c = nullptr;
        delete nodeToRemove;
      }
      return;
    }

    Chunk *current = &start;
    for (int i = 0; i < index; i++) {
      if (current->c == nullptr)
        return;
      current = current->c;
    }

    Chunk *nodeToRemove = current->c;
    if (nodeToRemove == nullptr)
      return;

    current->c = nodeToRemove->c;
    nodeToRemove->c = nullptr;
    delete nodeToRemove;
    index--;
  }

  void DrawAll(sf::RenderWindow &window) {
    Chunk *current = start.c;

    while (current != nullptr) {
      window.draw(*current);
      current = current->c;
    }
  }

  Chunk start;
};

struct Screen {

  void SetButtons() {
    startText.SetHeaderText("Press Start", sf::Vector2f(200, 200));
    endText.SetHeaderText("Try Again?", sf::Vector2f(200, 200));
    titleButton = Dinzai::Button(*startText.theHeaderText);
    endButton = Dinzai::Button(*endText.theHeaderText);
  }

  Screen() : window(sf::VideoMode(sf::Vector2u(ScreenWidth, ScreenHeight)), "Snake") {
    chunks = new LinkedChunk();
    SetButtons();
  }

  void UpdateButtons() { titleButton.CheckCollision(mousePos); }

  int FixedUpdateButtons() {
    // This method is deprecated - use titleButton.CanClick directly in event loop
    return 0;
  }

  void DrawButtons() { startText.DrawHeaderTextToScreen(window); }

  void UpdateEndButtons() { endButton.CheckCollision(mousePos); }

  bool FixedUpdateEndButtons() {
    // This method is deprecated - use endButton.CanClick directly in event loop
    return false;
  }

  void DrawEndButtons() { endText.DrawHeaderTextToScreen(window); }

  void ButtonReset()
  {
    titleButton.Reset();
    endButton.Reset();
  }

  sf::RectangleShape MakeDrawableRect(Entity &obj) {
    sf::RectangleShape shape;
    sf::Vector2f size;
    sf::Vector2f position;
    sf::Color color;

    color = sf::Color(obj.color->r, obj.color->g, obj.color->b, obj.color->a);
    size = sf::Vector2f(sf::Vector2f(obj.size->x, obj.size->y));
    position = sf::Vector2f(obj.position->x, obj.position->y);

    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color(30, 100, 30));
    shape.setOutlineThickness(1.2);

    return shape;
  }

  sf::CircleShape MakeDrawableCircle(EntityF &obj) {
    sf::CircleShape shape;
    float radius;
    sf::Vector2f position;
    sf::Color color;

    color = sf::Color(obj.color->r, obj.color->g, obj.color->b, obj.color->a);
    radius = obj.radius;
    position = sf::Vector2f(obj.position->x, obj.position->y);

    shape.setRadius(radius);
    shape.setPosition(position);
    shape.setFillColor(color);

    return shape;
  }

  sf::RectangleShape MakeChunk(Viper::Vec2 &size) {

    Viper::Vec2 *windowSize = &size;
    Euler::Grid grid;
    Viper::Vec2 newSize = grid.CalculateGridChunk(*windowSize);
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(newSize.x, newSize.y));
    shape.setFillColor(sf::Color::Black);
    shape.setOutlineThickness(0.8);
    shape.setOutlineColor(sf::Color::White);
    return shape;
  }

  void MakeGrid(int worldWidth, int worldHeight, float offset,
                Viper::Vec2 &chunkSize) {
    for (int i = 0; i < worldWidth; i++) {
      for (int j = 0; j < worldHeight; j++) {
        sf::RectangleShape chunk = MakeChunk(chunkSize);
        chunk.setPosition(sf::Vector2f(i * chunk.getSize().x + offset,
                          j * chunk.getSize().y + offset));
        chunks->AddEntity(chunk);
      }
    }
  }

  // think about draw layers

  sf::Clock mainClock;
  sf::Time deltaTime;
  sf::RenderWindow window;
  LinkedChunk *chunks;

  Dinzai::AllText startText;
  Dinzai::AllText endText;
  sf::Vector2f mousePos;

  Dinzai::Button titleButton;
  Dinzai::Button endButton;
};