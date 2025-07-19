
#include "Bucket.h"
#include "Entity.h"
#include "Window.h"
#include "iostream"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>

struct Game {

  Game() {

    for (int i = 0; i < 4; i++) {
      direction[i] = false;
    }

    Viper::Vec2 *pos = new Viper::Vec2(200, 200);
    Viper::Vec2 *size = new Viper::Vec2(20, 20);
    Viper::Vec3 *values = new Viper::Vec3(0, 255, 0);

    Viper::Vec2 *posF = new Viper::Vec2(600, 300);
    float sizeF = 10;
    Viper::Vec3 *valuesF = new Viper::Vec3(255, 0, 0);

    // set player
    b.SetEntity(*pos, *size, *values);
    player = b.manyEntity.GetNode(0);

    // set fruit
    f.SetEntity(*posF, sizeF, *valuesF);

    fruit = nullptr;
    direction[0] = true;
  }

private:
  void SpawnFruit() {
    if (fruit == nullptr) {
      Viper::Vec2 min(40, 40);
      Viper::Vec2 max(760, 560);
      Viper::Vec2 result = random.RandomVec(min, max);

      Viper::Vec2 *posF = &result;

      fruit = f.otherManyEntity.GetNode(1);
      fruit->SetPosition(posF->x, posF->y);
      fruit->SetCenter(fruit->radius, *posF);
    }
  }

  void CheckCollision() {
    if (fruit && calc.RectangularToCircleCheck(*player, *fruit)) {

      fruit = nullptr;

      Entity *prevTail = b.manyEntity.tail;
      Viper::Vec2 offSet = Viper::Vec2(30, 0);
      Viper::Vec2 pos = prevTail->GetPosition();
      Viper::Vec2 actualPos = Viper::Vec2(pos.x - offSet.x, pos.y + offSet.y);
      Viper::Vec2 *size = new Viper::Vec2(20, 20);
      Viper::Vec3 *values = new Viper::Vec3(0, 255, 0);

      b.manyEntity.AddEntity(actualPos, *size, *values);
    }
  }

  void Update() {
    CheckCollision();
    screen.deltaTime = screen.mainClock.getElapsedTime();
    delay += screen.deltaTime.asSeconds();

    for (int i = 0; i < b.manyEntity.index; i++) {

      if (b.manyEntity.GetNode(i)->position->x <
          ScreenWidth - b.manyEntity.GetNode(i)->size->x)
        if (direction[0]) {
          b.manyEntity.GetNode(i)->position->x +=
              player->speed * screen.deltaTime.asSeconds();
        }
      if (b.manyEntity.GetNode(i)->position->x > 0) {
        if (direction[1]) {
          b.manyEntity.GetNode(i)->position->x -=
              player->speed * screen.deltaTime.asSeconds();
        }
      }
      if (b.manyEntity.GetNode(i)->position->y > 0) {
        if (direction[2]) {
          b.manyEntity.GetNode(i)->position->y -=
              player->speed * screen.deltaTime.asSeconds();
        }
      }
      if (b.manyEntity.GetNode(i)->position->y <
          ScreenHeight - b.manyEntity.GetNode(i)->size->y)
        if (direction[3]) {
          b.manyEntity.GetNode(i)->position->y +=
              player->speed * screen.deltaTime.asSeconds();
        }
    }
    /*
    if (player->position->x < ScreenWidth - player->size->x) {
      if (direction[0]) {
        player->position->x += player->speed * screen.deltaTime.asSeconds();
      }
    }
    if (player->position->x > 0) {
      if (direction[1]) {
        player->position->x -= player->speed * screen.deltaTime.asSeconds();
      }
    }
    if (player->position->y > 0) {
      if (direction[2]) {
        player->position->y -= player->speed * screen.deltaTime.asSeconds();
      }
    }
    if (player->position->y < ScreenHeight - player->size->y) {
      if (direction[3]) {
        player->position->y += player->speed * screen.deltaTime.asSeconds();
      }
    }
    */

    player->SetCenter(*player->size, *player->position);

    screen.mainClock.restart();

    SpawnFruit();
  }

  void FixedUpdate() {
    while (screen.window.pollEvent(screen.event)) {
      if (screen.event.type == sf::Event::Closed) {
        screen.window.close();
      }

      if (screen.event.type == sf::Event::KeyPressed) {
        if (screen.event.key.code == sf::Keyboard::W) {
          delay = 0;
          direction[0] = false;
          direction[1] = false;
          direction[2] = true;
          direction[3] = false;
        }

        if (screen.event.key.code == sf::Keyboard::S) {
          delay = 0;
          direction[0] = false;
          direction[1] = false;
          direction[2] = false;
          direction[3] = true;
        }

        if (screen.event.key.code == sf::Keyboard::A) {
          delay = 0;
          direction[0] = false;
          direction[1] = true;
          direction[2] = false;
          direction[3] = false;
        }

        if (screen.event.key.code == sf::Keyboard::D) {
          delay = 0;
          direction[0] = true;
          direction[1] = false;
          direction[2] = false;
          direction[3] = false;
        }
      }
    }
  }

  void DrawToScreen() {
    screen.window.clear();

    screen.DrawSnake(screen.window, screen, b.manyEntity);

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
  Bucket f;

  Entity *player;
  EntityF *fruit;

  Trig::Pythagorian calc;
  PRandom::Random random;

  sf::RectangleShape chunk;

  bool direction[4];

  float delay;

  // 0,     1,    2,  3
  // right, left, up, down
};
