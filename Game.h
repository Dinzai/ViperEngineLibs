
#include "Bucket.h"
#include "Entity.h"
#include "Window.h"
#include "iostream"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>

struct Game {

  Game() {
    GameObject::Vec2 *pos = new GameObject::Vec2(200, 200);
    GameObject::Vec2 *size = new GameObject::Vec2(20, 20);
    GameObject::Vec3 *values = new GameObject::Vec3(0, 255, 0);

    GameObject::Vec2 *posF = new GameObject::Vec2(600, 300);
    float sizeF = 10; //new GameObject::Vec2(20, 20);
    GameObject::Vec3 *valuesF = new GameObject::Vec3(255, 0, 0);

    b.SetEntity(*pos, *size, *values);
    b.SetEntity(*posF, sizeF, *valuesF);

    player = b.manyEntity.GetNode(1);
    fruit = b.manyEntity.GetNode(2);
  }

private:
  void CheckCollision() {
    if (fruit) {
      if (calc.RectangularToCircleCheck(*player, *fruit)) {
        std::cout << "hit!" << '\n';
        int i = fruit->index;
        b.RemoveEntity(i);
        fruit = nullptr;
      }
    }
  }

  void Update() {
    CheckCollision();
    screen.deltaTime = screen.mainClock.getElapsedTime();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      player->position->x -= player->speed * screen.deltaTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      player->position->x += player->speed * screen.deltaTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      player->position->y -= player->speed * screen.deltaTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      player->position->y += player->speed * screen.deltaTime.asSeconds();
    }
    player->SetCenter(*player->size, *player->position);
    screen.mainClock.restart();

    
    if(fruit)
        std::cout << "Fruit center:" << fruit->center->x << ' ' << fruit->center->y << '\n';
  }

  void FixedUpdate() {
    while (screen.window.pollEvent(screen.event)) {
      if (screen.event.type == sf::Event::Closed) {
        screen.window.close();
      }
    }
  }

  void DrawToScreen() {
    screen.window.clear();
    screen.window.draw(screen.MakeDrawableRect(*player));
    if (fruit)
      screen.window.draw(screen.MakeDrawableCircle(*fruit));
    screen.window.display();
  }

  void Loop() {
    while (screen.window.isOpen()) {

      Update();
      FixedUpdate();
      DrawToScreen();
    }
  }

public:
  void Run() { Loop(); }

private:
  Screen screen;
  Bucket b;
  Entity *player;
  Entity *fruit;
  Trig::Pythagorian calc;
};
