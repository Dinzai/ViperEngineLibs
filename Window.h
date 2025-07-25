
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
#include "SFMLEntity.h"


struct Screen {

  void SetButtons() {
    startText.SetHeaderText("Press Start", sf::Vector2f(200, 200));
    endText.SetHeaderText("Try Again?", sf::Vector2f(200, 200));
    titleButton = Dinzai::Button(startText.theHeaderText);
    endButton = Dinzai::Button(endText.theHeaderText);
  }

  Screen() : window(sf::VideoMode(ScreenWidth, ScreenHeight), "Snake") {
    chunks = new LinkedChunk();
    otherChunks = new LinkedChunk();
    SetButtons();
  }

  void UpdateButtons() { titleButton.CheckCollision(mousePos); }

  int FixedUpdateButtons() {
    if (titleButton.CanClick(event, mousePos)) {
      return 1;
    }
    return 0;
  }

  void DrawButtons() { startText.DrawHeaderTextToScreen(window); }

  void UpdateEndButtons() { endButton.CheckCollision(mousePos); }

  bool FixedUpdateEndButtons() {
    if (endButton.CanClick(event, mousePos)) {
      return true;
    }
    return false;
  }

  void DrawEndButtons() { endText.DrawHeaderTextToScreen(window); }

  void ButtonReset() {
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

  sf::RectangleShape MakeDrawableRect(EntityF &obj) {
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

  sf::CircleShape MakeDrawableCircle(Entity &obj) {
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
        chunk.setFillColor(sf::Color::Red);
        chunk.setPosition(i * chunk.getSize().x + offset,
                          j * chunk.getSize().y + offset);
        chunks->AddEntity(chunk);
      }
    }
  }

  void MakeGrid(int worldWidth, int worldHeight, float spacing,
                Viper::Vec2 &chunkSize, Viper::Vec2 buffer, LinkedChunk* theChunks, sf::Color color) {
    for (int i = 0; i < worldWidth; i++) {
      for (int j = 0; j < worldHeight; j++) {
        sf::RectangleShape chunk = MakeChunk(chunkSize);
        chunk.setFillColor(color);
        chunk.setPosition(i * (chunk.getSize().x + spacing) + buffer.x,
                          j * (chunk.getSize().y + spacing) + buffer.y);
        theChunks->AddEntity(chunk);
      }
    }
  }

  // think about draw layers

  sf::Clock mainClock;
  sf::Time deltaTime;
  sf::Event event;
  sf::RenderWindow window;
  LinkedChunk *chunks;
  LinkedChunk *otherChunks;

  Dinzai::AllText startText;
  Dinzai::AllText endText;
  sf::Vector2f mousePos;

  Dinzai::Button titleButton;
  Dinzai::Button endButton;
};