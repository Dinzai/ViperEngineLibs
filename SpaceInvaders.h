#include "Bucket.h"
#include "Entity.h"
#include "States.h"
#include "Window.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

struct Game {

private:
  void SetEnemyStats() {
    for (int i = 0; i <= screen.chunks->index; i++) {
      Chunk *chunk = screen.chunks->GetNode(i);
      if (!chunk)
        continue;

      screen.chunks->SetHealth(i, 1);
      screen.chunks->SetSpeed(i, 20);

      sf::Vector2f sfPos = chunk->getPosition();
      sf::Vector2f sfSize = chunk->getSize();

      chunk->SetPosition(sfPos.x, sfPos.y);
      chunk->SetSize(sfSize.x, sfSize.y);
      chunk->SetCenter(*chunk->size, *chunk->position);
    }
  }

  void SetBaseStats() {
    for (int i = 0; i <= screen.otherChunks->index; i++) {
      Chunk *chunk = screen.otherChunks->GetNode(i);
      if (!chunk)
        continue;

      screen.otherChunks->SetHealth(i, 10);
      screen.otherChunks->SetSpeed(i, 20);

      sf::Vector2f sfPos = chunk->getPosition();
      sf::Vector2f sfSize = chunk->getSize();

      chunk->SetPosition(sfPos.x, sfPos.y);
      chunk->SetSize(sfSize.x, sfSize.y);
      chunk->SetCenter(*chunk->size, *chunk->position);
    }
  }

  void SpawnEnemies() {

    Viper::Vec2 area(50, 50);
    chunkSize = grid.CalculateGridChunk(area);
    chunkSize.x = chunkSize.x / 2;
    chunkSize.y = chunkSize.y / 2;

    numChunksX = 275 / chunkSize.x;
    numChunksY = 125 / chunkSize.y;

    Viper::Vec2 buffer;
    buffer.x = 150;
    buffer.y = 100;

    screen.MakeGrid(numChunksX, numChunksY, 20, chunkSize, buffer,
                    screen.chunks, sf::Color::Red);
    screen.chunks->AddEntity(chunk);
    SetEnemyStats();
  }

  void SpawnBase() {
    Viper::Vec2 area(50, 50);
    chunkSize = grid.CalculateGridChunk(area);
    chunkSize.x = chunkSize.x;
    chunkSize.y = chunkSize.y;

    numChunksX = 200 / chunkSize.x;
    numChunksY = 50 / chunkSize.y;

    Viper::Vec2 buffer;
    buffer.x = 175;
    buffer.y = 480;

    screen.MakeGrid(numChunksX, numChunksY, 60, chunkSize, buffer,
                    screen.otherChunks, sf::Color::Green);
    screen.otherChunks->AddEntity(chunk);
    SetBaseStats();
  }

  void Init() {
    screen.ButtonReset();

    for (int i = 0; i < 2; i++) {
      direction[i] = false;
    }

    Viper::Vec2 *pos = new Viper::Vec2(ScreenWidth * 0.5f, ScreenHeight - 50);
    float size = 10;
    Viper::Vec3 *values = new Viper::Vec3(100, 0, 110);

    s.otherManyEntity.AddEntity(*pos, size, *values);
    ship = s.otherManyEntity.GetNode(1);
    ship->SetCenter(ship->radius, *ship->position);

    SpawnEnemies();
    SpawnBase();
  }

public:
  Game() { Init(); }

private:
  void GetMousePos() {
    screen.mousePos =
        screen.window.mapPixelToCoords(sf::Mouse::getPosition(screen.window));
  }

  void CheckCollision() {
    Entity *prevBullet = nullptr;
    Entity *currentBullet = b.manyEntity.head;

    while (currentBullet != nullptr) {
      Entity *nextBullet = currentBullet->next;
      bool bulletRemoved = false;

      for (int i = 0; i <= screen.chunks->index; i++) {
        Chunk *enemy = screen.chunks->GetNode(i);
        if (!enemy)
          continue;

        if (calc.RectangularToCircleCheck(*enemy, *currentBullet)) {
          b.manyEntity.RemoveNodeByPointer(currentBullet, prevBullet);
          bulletRemoved = true;
          enemy->health--;
          Viper::Println("hit enemy: ");
          Viper::Println(enemy->health);
          break;
        }
      }

      for (int i = 0; i <= screen.otherChunks->index; i++) {
        Chunk *enemy = screen.otherChunks->GetNode(i);
        if (!enemy)
          continue;

        if (calc.RectangularToCircleCheck(*enemy, *currentBullet)) {
          b.manyEntity.RemoveNodeByPointer(currentBullet, prevBullet);
          bulletRemoved = true;
          enemy->health--;
          Viper::Println("hit base: ");
          Viper::Println(enemy->health);
          break;
        }
      }

      if (!bulletRemoved)
        prevBullet = currentBullet;

      currentBullet = nextBullet;
    }
  }

  void UpdateEnemy() {

    for (int i = screen.chunks->index; i >= 0; i--) {
      Chunk *chunk = screen.chunks->GetNode(i);
      if (!chunk)
        continue;

      if (chunk->health <= 0) {
        screen.chunks->RemoveNode(i);
        Viper::Println("dead");
      }
    }
  }

  void UpdateBases() {

    for (int i = screen.otherChunks->index; i >= 0; i--) {
      Chunk *chunk = screen.otherChunks->GetNode(i);
      if (!chunk)
        continue;

      if (chunk->health <= 0) {
        screen.otherChunks->RemoveNode(i);
        Viper::Println("dead");
      }
    }
  }

  void Shoot() {
    float bulletRadius = 5;
    Viper::Vec3 *value = new Viper::Vec3(0, 0, 200);
    b.manyEntity.AddEntity(*ship->center, bulletRadius, *value);
  }

  void MoveBullet() {
    for (int i = 0; i < b.manyEntity.index; i++) {
      b.manyEntity.GetNode(i)->position->y -=
          bulletSpeed * screen.deltaTime.asSeconds();
      Entity *bullet = b.manyEntity.GetNode(i);
      b.manyEntity.GetNode(i)->SetCenter(bullet->radius, *bullet->position);
    }
  }

  void MoveShip() {
    if (direction[0]) {
      ship->position->x += speed * screen.deltaTime.asSeconds();
    }
    if (direction[1]) {
      ship->position->x -= speed * screen.deltaTime.asSeconds();
    }
    ship->SetCenter(ship->radius, *ship->position);
  }

  void WallCheck() {}

  void GameUpdate() {
    screen.deltaTime = screen.mainClock.getElapsedTime();
    CheckCollision();
    UpdateEnemy();
    UpdateBases();
    MoveShip();
    MoveBullet();
    screen.mainClock.restart();
  }

  void GameFixedUpdate() {

    while (screen.window.pollEvent(screen.event)) {
      if (screen.event.type == sf::Event::Closed) {
        screen.window.close();
      }
      if (screen.event.type == sf::Event::KeyPressed) {
        if (screen.event.key.code == sf::Keyboard::D) {
          direction[1] = false;
          direction[0] = true;
        }

        if (screen.event.key.code == sf::Keyboard::A) {
          direction[0] = false;
          direction[1] = true;
        }

        if (screen.event.key.code == sf::Keyboard::E) {
          Shoot();
        }
      }
      if (screen.event.type == sf::Event::KeyReleased) {
        direction[0] = false;
        direction[1] = false;
      }
    }
  }

  void DrawBullets(Screen &screen, LinkedEntity &bulletList) {
    Entity *current = bulletList.head;
    while (current != nullptr) {
      screen.window.draw(screen.MakeDrawableCircle(*current));
      current = current->next;
    }
  }

  void DrawEnemies(Screen &screen, LinkedEntity &enemyList) {
    Entity *current = enemyList.head;
    while (current != nullptr) {
      screen.window.draw(screen.MakeDrawableCircle(*current));
      current = current->next;
    }
  }

  void GameDrawToScreen() {
    screen.window.clear();
    screen.chunks->DrawAll(screen.window);
    screen.otherChunks->DrawAll(screen.window);

    if (ship) {
      screen.window.draw(screen.MakeDrawableCircle(*ship));
    }

    DrawBullets(screen, b.manyEntity);

    screen.window.display();
  }
  //-------------------------
  void TitleUpdate() {
    GetMousePos();
    screen.UpdateButtons();
  }

  void TitleFixedUpdate() {
    while (screen.window.pollEvent(screen.event)) {
      if (screen.event.type == sf::Event::Closed) {
        screen.window.close();
      }

      if (screen.FixedUpdateButtons()) {
        state.currentGameState = state.Game;
      }
    }
  }

  void TitleDrawToScreen() {
    screen.window.clear(sf::Color::Blue);
    screen.DrawButtons();
    screen.window.display();
  }
  //---------------------------

  void Reset() {
    s.ClearEntities();
    b.ClearEntities();
    e.ClearEntities();
    Init();
  }

  void EndUpdate() {
    GetMousePos();
    screen.UpdateEndButtons();
  }

  void EndFixedUpdate() {
    while (screen.window.pollEvent(screen.event)) {
      if (screen.event.type == sf::Event::Closed) {
        screen.window.close();
      }
      if (screen.FixedUpdateEndButtons()) {
        Reset();
        state.currentGameState = state.Game;
      }
    }
  }

  void EndDrawToScreen() {
    screen.window.clear(sf::Color::Blue);
    screen.DrawEndButtons();
    screen.window.display();
  }

  void Loop() {
    while (screen.window.isOpen()) {

      if (state.currentGameState == state.Title) {
        TitleUpdate();
        TitleFixedUpdate();
        TitleDrawToScreen();
      }

      if (state.currentGameState == state.Game) {
        GameUpdate();
        GameFixedUpdate();
        GameDrawToScreen();
      }

      if (state.currentGameState == state.End) {
        EndUpdate();
        EndFixedUpdate();
        EndDrawToScreen();
      }
    }
  }

public:
  void Run() { Loop(); }

private:
  //-------------
  // sfml implmentation
  Screen screen;

  // container class
  Bucket s;
  Bucket b;
  Bucket e;
  // my libs

  EntityF *ship;

  float speed = 100;
  float bulletSpeed = 175;

  Trig::Pythagorian calc;
  PRandom::Random random;
  Euler::Grid grid;
  Snake::GameState state;

  bool direction[2];

  sf::RectangleShape chunk;

  Viper::Vec2 chunkSize;
  int numChunksX = 0;
  int numChunksY = 0;
};
