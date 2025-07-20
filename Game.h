
#include "Bucket.h"
#include "Entity.h"
#include "Window.h"
#include "iostream"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>

struct Game {
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

public:
  Game() {

    for (int i = 0; i < 4; i++) {
      direction[i] = false;
    }

    // snake head
    Viper::Vec2 *pos = new Viper::Vec2(200, 200);
    Viper::Vec2 *size = new Viper::Vec2(20, 20);
    Viper::Vec3 *values = new Viper::Vec3(0, 255, 0);
    Viper::Vec2 *tailPos =
        new Viper::Vec2(pos->x - size->x - tailOffset, pos->y);

    // fruit
    Viper::Vec2 *posF = new Viper::Vec2(600, 300);
    float sizeF = 10;
    Viper::Vec3 *valuesF = new Viper::Vec3(255, 0, 0);

    b.SetEntity(*pos, *size, *values);
    b.SetEntity(*tailPos, *size, *values);
    snake = b.manyEntity.GetNode(0);
    tail = b.manyEntity.GetNode(1);

    // set fruit
    f.SetEntity(*posF, sizeF, *valuesF);

    fruit = nullptr;
    direction[0] = true;
    snakeSize = b.manyEntity.index - 1;
    SpawnFruit();
  }

private:
  Viper::Vec2 *OutHeadPos() {
    Viper::Vec2 *headPos;

    if (direction[0]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x - snake->size->x - tailOffset, snake->position->y);
      headPos = newPos;
    }
    if (direction[1]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x + snake->size->x + tailOffset, snake->position->y);
      headPos = newPos;
    }
    if (direction[2]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x, snake->position->y - snake->size->y - tailOffset);
      headPos = newPos;
    }
    if (direction[3]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x, snake->position->y + snake->size->y + tailOffset);
      headPos = newPos;
    }
    return headPos;
  }

  Viper::Vec2 *OutLastTailPos() {
    Viper::Vec2 *tailPos;

    if (direction[0]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          tail->position->x - tail->size->x - tailOffset, tail->position->y);
      tailPos = newPos;
    }
    if (direction[1]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          tail->position->x + tail->size->x + tailOffset, tail->position->y);
      tailPos = newPos;
    }
    if (direction[2]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          tail->position->x, tail->position->y - tail->size->y - tailOffset);
      tailPos = newPos;
    }
    if (direction[3]) {
      Viper::Vec2 *newPos = new Viper::Vec2(
          tail->position->x, tail->position->y + tail->size->y + tailOffset);
      tailPos = newPos;
    }
    return tailPos;
  }

  void AddTail()
  {
    Viper::Vec3 *values =
          new Viper::Vec3(snake->color->r, snake->color->g, snake->color->b);
      if (direction[0]) {
        Viper::Vec2 *newPos =
            new Viper::Vec2(tail->position->x - tail->size->x - tailOffset,
                            tail->position->y);
        b.manyEntity.AddEntity(*newPos, *tail->size, *values);
        tail = b.manyEntity.GetNode(b.manyEntity.index - 1);
      }
      if (direction[1]) {
        Viper::Vec2 *newPos =
            new Viper::Vec2(tail->position->x + tail->size->x + tailOffset,
                            tail->position->y);
        b.manyEntity.AddEntity(*newPos, *tail->size, *values);
        tail = b.manyEntity.GetNode(b.manyEntity.index - 1);
      }
      if (direction[2]) {
        Viper::Vec2 *newPos =
            new Viper::Vec2(tail->position->x,
                            tail->position->y - tail->size->y - tailOffset);
        b.manyEntity.AddEntity(*newPos, *tail->size, *values);
        tail = b.manyEntity.GetNode(b.manyEntity.index - 1);
      }
      if (direction[3]) {
        Viper::Vec2 *newPos =
            new Viper::Vec2(tail->position->x,
                            tail->position->y + tail->size->y + tailOffset);
        b.manyEntity.AddEntity(*newPos, *tail->size, *values);
        tail = b.manyEntity.GetNode(b.manyEntity.index - 1);
      }
  }

 void UpdateTail() {

    const int maxSnakeLength = 100;
    Viper::Vec2* oldPositions = new Viper::Vec2[maxSnakeLength]; 

    Entity* current = b.manyEntity.head;
    int index = 0;

    while (current != nullptr ) {
        oldPositions[index] = *current->position;
        current = current->next;
        index++;
    }

    current = b.manyEntity.head->next;
    index = 0;
    while (current != nullptr) {
        *current->position = oldPositions[index];
        current = current->next;
        index++;
    }
}

  void UpdateHead(sf::Time deltaTime) {

    if (direction[0]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x + snake->size->x + tailOffset, snake->position->y);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
      UpdateTail();
      
    }

    if (direction[1]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x - snake->size->x - tailOffset, snake->position->y);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
      UpdateTail();
    }

    if (direction[2]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x, snake->position->y - snake->size->y - tailOffset);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
      UpdateTail();
    }

    if (direction[3]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x, snake->position->y + snake->size->y + tailOffset);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
      UpdateTail();
      
    }
  }

  void DrawSnake(Screen &screen, LinkedEntity &snakeList) {
    Entity *current = snakeList.head;
    while (current != nullptr) {
      screen.window.draw(screen.MakeDrawableRect(*current));
      current = current->next;
    }
  }

  void CheckCollision() {
    if (fruit && calc.RectangularToCircleCheck(*snake, *fruit)) {

      fruit = nullptr;
      AddTail();
      SpawnFruit();
    }
  }

  void Update() {

    CheckCollision();
    screen.deltaTime = screen.mainClock.getElapsedTime();
    if (screen.deltaTime.asSeconds() > 0.12) {
      UpdateHead(screen.deltaTime);

      screen.mainClock.restart();
    }
  }

  void FixedUpdate() {
    while (screen.window.pollEvent(screen.event)) {
      if (screen.event.type == sf::Event::Closed) {
        screen.window.close();
      }

      if (screen.event.type == sf::Event::KeyPressed) {
        if (screen.event.key.code == sf::Keyboard::W) {

          direction[0] = false;
          direction[1] = false;
          direction[2] = true;
          direction[3] = false;
        }

        if (screen.event.key.code == sf::Keyboard::S) {

          direction[0] = false;
          direction[1] = false;
          direction[2] = false;
          direction[3] = true;
        }

        if (screen.event.key.code == sf::Keyboard::A) {

          direction[0] = false;
          direction[1] = true;
          direction[2] = false;
          direction[3] = false;
        }

        if (screen.event.key.code == sf::Keyboard::D) {

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

    DrawSnake(screen, b.manyEntity);

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

  Entity *snake;
  Entity *tail;
  EntityF *fruit;

  Trig::Pythagorian calc;
  PRandom::Random random;

  sf::RectangleShape chunk;

  bool direction[4];

  int tailOffset = 5;

  int snakeSize = 0;

  float delay = 0;

  // 0,     1,    2,  3
  // right, left, up, down
};
