#include "Bucket.h"
#include "Entity.h"
#include "SFMLEntity.h"
#include "States.h"
#include "Window.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>

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

      screen.otherChunks->SetHealth(i, 1);
      screen.otherChunks->SetSpeed(i, 20);

      sf::Vector2f sfPos = chunk->getPosition();
      sf::Vector2f sfSize = chunk->getSize();

      chunk->SetPosition(sfPos.x, sfPos.y);
      chunk->SetSize(sfSize.x, sfSize.y);
      chunk->SetCenter(*chunk->size, *chunk->position);
    }
    ship->health = 1;
  }

  void SortEnemy(Viper::Vec2 buffer, float spacing) {

    int columnCount = 0;
    int col = 0;
    int lastCol = 0;

    for (int i = 0; i < screen.chunks->index; i++) {
      Chunk *chunk = screen.chunks->GetNode(i);
      col = chunk->position->x / chunkSize.x;

      if (col != lastCol) {
        columnCount++;
      }

      lastCol = col;
    }

    for (int c = 0; c < columnCount; c++) {
      Chunk *bottomEnemy = nullptr;
      float maxYPosition = -1;

      for (int i = 0; i < screen.chunks->index; i++) {
        Chunk *chunk = screen.chunks->GetNode(i);
        int thisCol = (chunk->position->x - buffer.x) / (chunkSize.x + spacing);

        if (thisCol == c && chunk->position->y > maxYPosition) {
          maxYPosition = chunk->position->y;
          bottomEnemy = chunk;
        }
      }

      if (bottomEnemy) {

        bottomEnemies.AddEntity(bottomEnemy);
      }
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

    float spacing = 20;

    screen.MakeGrid(numChunksX, numChunksY, spacing, chunkSize, buffer,
                    screen.chunks, sf::Color::Red);
    screen.chunks->AddEntity(chunk);
    SetEnemyStats();            // setting enemy stats(hp, speed)
    SortEnemy(buffer, spacing); // locating bottom enmies in grid

  }

  void SpawnBase() {
    Viper::Vec2 area(25, 25);
    chunkSize = grid.CalculateGridChunk(area);
    chunkSize.x = chunkSize.x / 10;
    chunkSize.y = chunkSize.y / 10;

    numChunksX = 25 / chunkSize.x;
    numChunksY = 25 / chunkSize.y;

    Viper::Vec2 buffer;
    buffer.x = 175;
    buffer.y = 460;

    for (int i = 0; i < 4; i++) {
      screen.MakeGrid(numChunksX, numChunksY, 0.5, chunkSize, buffer,
                      screen.otherChunks, sf::Color::Green);
      buffer.x += 120;
    }

    screen.otherChunks->AddEntity(otherChunk);
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

  void CheckPlayerBulletCollision() {
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
          numBullet = 0;

          break;
        }
      }

      for (int i = 0; i <= screen.otherChunks->index; i++) {
        Chunk *bases = screen.otherChunks->GetNode(i);

        if (!bases)
          continue;

        if (calc.RectangularToCircleCheck(*bases, *currentBullet)) {
          b.manyEntity.RemoveNodeByPointer(currentBullet, prevBullet);
          bulletRemoved = true;
          bases->health--;
          numBullet = 0;
          break;
        }
      }

      if (!bulletRemoved)
        prevBullet = currentBullet;

      currentBullet = nextBullet;
    }
  }


  void CheckEnemyBulletCollision() {
    Entity *prevBullet = nullptr;
    Entity *currentBullet = e.manyEntity.head;

    while (currentBullet != nullptr) {
      Entity *nextBullet = currentBullet->next;
      bool bulletRemoved = false;

      for (int i = 0; i <= screen.chunks->index; i++) {
        Chunk *enemy = screen.chunks->GetNode(i);
        if (!enemy)
          continue;

        if (calc.RectangularToCircleCheck(*ship, *currentBullet)) {
          e.manyEntity.RemoveNodeByPointer(currentBullet, prevBullet);
          bulletRemoved = true;
          ship->health--;
       

          break;
        }
      }

      for (int i = 0; i <= screen.otherChunks->index; i++) {
        Chunk *bases = screen.otherChunks->GetNode(i);

        if (!bases)
          continue;

        if (calc.RectangularToCircleCheck(*bases, *currentBullet)) {
          e.manyEntity.RemoveNodeByPointer(currentBullet, prevBullet);
          bulletRemoved = true;
          bases->health--;
          
          break;
        }
      }

      if (!bulletRemoved)
        prevBullet = currentBullet;

      currentBullet = nextBullet;
    }
  }

  void CheckCollision()
  {
    CheckPlayerBulletCollision();
    CheckEnemyBulletCollision();
  }


  void EnemyShoot(sf::Time deltaTime) {

    float bulletRadius = 4;
    Viper::Vec3 value = Viper::Vec3(0, 200, 0);
    Viper::Vec2 randomPos = {0, 0};
    enemyBulletTimer += deltaTime.asSeconds();
    if (enemyBulletTimer >= 2) {
      for (int i = 0; i < bottomEnemies.index; i++) {
        Chunk *temp = bottomEnemies.GetNode(i);

        Viper::Vec2 minPos =
            Viper::Vec2(temp->position->x - 400, temp->position->y);
        Viper::Vec2 maxPos =
            Viper::Vec2(temp->position->x + 50, temp->position->y);
        randomPos = random.RandomVec(minPos, maxPos);
        randomPos.x = randomPos.x + 12;
        randomPos.y = temp->position->y + 20;
      }
      e.manyEntity.AddEntity(randomPos, bulletRadius, value);
      enemyBulletTimer = 0;
    }

  }

  void MoveEnemyBullet()
  {
    for (int i = 0; i < e.manyEntity.index; i++) {
      Entity *bullet = e.manyEntity.GetNode(i);

      bullet->position->y += enemyBulletSpeed * screen.deltaTime.asSeconds();
      bullet->SetCenter(bullet->radius, *bullet->position);

      if (bullet->position->y >= ScreenHeight) {
        Entity *prevBullet = nullptr;
        Entity *currentBullet = e.manyEntity.head;
        while (currentBullet != nullptr) 
        {
          Entity *nextBullet = currentBullet->next;
          bool bulletRemoved = false;

          e.manyEntity.RemoveNodeByPointer(currentBullet, prevBullet);
          bulletRemoved = true;
          if (!bulletRemoved)
            prevBullet = currentBullet;

          currentBullet = nextBullet;

        }
      }
    }
  }

  void UpdateEnemy() {

    for (int i = screen.chunks->index; i >= 0; i--) {
      Chunk *chunk = screen.chunks->GetNode(i);
      if (!chunk)
        continue;

      if (chunk->health <= 0) {
        screen.chunks->RemoveNode(i);
      }
    }
    EnemyShoot(screen.deltaTime);
    MoveEnemyBullet();
  }

  void UpdateBases() {

    for (int i = screen.otherChunks->index; i >= 0; i--) {
      Chunk *chunk = screen.otherChunks->GetNode(i);
      if (!chunk)
        continue;

      if (chunk->health <= 0) {
        screen.otherChunks->RemoveNode(i);
      }
    }
  }

  void Shoot() {
    if (numBullet <= 0) {
      float bulletRadius = 3;
      Viper::Vec3 value = Viper::Vec3(0, 0, 200);
      b.manyEntity.AddEntity(*ship->center, bulletRadius, value);
      numBullet++;
    }
  }

  void MoveBullet() {
    for (int i = 0; i < b.manyEntity.index; i++) {
      Entity *bullet = b.manyEntity.GetNode(i);
      bullet->position->y -= bulletSpeed * screen.deltaTime.asSeconds();
      bullet->SetCenter(bullet->radius, *bullet->position);
      if (bullet->position->y <= 0) {
        Entity *prevBullet = nullptr;
        Entity *currentBullet = b.manyEntity.head;
        while (currentBullet != nullptr) {
          Entity *nextBullet = currentBullet->next;
          bool bulletRemoved = false;

          b.manyEntity.RemoveNodeByPointer(currentBullet, prevBullet);
          bulletRemoved = true;
          if (!bulletRemoved)
            prevBullet = currentBullet;

          currentBullet = nextBullet;
          numBullet = 0; // reset bullet
        }
      }
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

        if (screen.event.key.code == sf::Keyboard::E && !canShoot) {
          Shoot();
          canShoot = true;
        }
      }
      if (screen.event.type == sf::Event::KeyReleased) {
        direction[0] = false;
        direction[1] = false;
        canShoot = false;
      }
    }
  }

  void DrawEnemyBullets(Screen &screen, LinkedEntity &eBulletList) {
    Entity *current = eBulletList.head;
    while (current != nullptr) {
      screen.window.draw(screen.MakeDrawableCircle(*current));
      current = current->next;
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
    DrawEnemyBullets(screen, e.manyEntity);

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
  Bucket s; // ships list
  Bucket b; // bullet list
  Bucket e; // enemy bullet List

  LinkedChunk bottomEnemies;
  // my libs

  EntityF *ship;

  float speed = 200;
  float bulletSpeed = 750;

  float enemyBulletSpeed = 450;
  float enemyBulletTimer = 0;

  Trig::Pythagorian calc;
  PRandom::Random random;
  Euler::Grid grid;
  Snake::GameState state;

  bool direction[2];

  sf::RectangleShape chunk;
  sf::RectangleShape otherChunk;

  Viper::Vec2 chunkSize;
  int numChunksX = 0;
  int numChunksY = 0;

  bool canShoot = false;
  int numBullet = 0;
};
