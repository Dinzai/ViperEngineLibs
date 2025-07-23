#include "Bucket.h"
#include "Entity.h"
#include "States.h"
#include "Window.h"

struct Game {

private:
  void SpawnFruit() {

    if (fruit == nullptr) {
      int cellX = random.RandomInt(0, numChunksX - 1);
      int cellY = random.RandomInt(0, numChunksY - 1);

      float posX = cellX * chunkSize.x;
      float posY = cellY * chunkSize.y;
      // random by grid
      Viper::Vec2 *posF = new Viper::Vec2(posX, posY);

      fruit = f.otherManyEntity.GetNode(1);
      fruit->SetPosition(posF->x, posF->y);
      fruit->SetCenter(fruit->radius, *posF);
    }
  }


  void Init()
  {
    screen.ButtonReset();
    Viper::Vec2 windowSize(ScreenWidth, ScreenHeight); // set the grid
    chunkSize = grid.CalculateGridChunk(windowSize);
    chunkSize.x = chunkSize.x / 8;
    chunkSize.y = chunkSize.y / 8;

    numChunksX = ScreenWidth / chunkSize.x;
    numChunksY = ScreenHeight / chunkSize.y;

    screen.MakeGrid(numChunksX, numChunksY, 0, chunkSize);

    for (int i = 0; i < 4; i++) {
      direction[i] = false;
    }

    // snake head
    Viper::Vec2 *pos = new Viper::Vec2(25, 25);
    Viper::Vec2 *size = new Viper::Vec2(20, 20);
    Viper::Vec3 *values = new Viper::Vec3(0, 255, 0);
    Viper::Vec2 *tailPos =
        new Viper::Vec2(pos->x - size->x - tailOffset, pos->y);

    // fruit
    Viper::Vec2 *posF = new Viper::Vec2(400, 200);
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

    SpawnFruit();

  }

public:
  Game() 
  {
    Init();

  }

private:
  void GetMousePos() {
    screen.mousePos =
        screen.window.mapPixelToCoords(sf::Mouse::getPosition(screen.window));
  }

  void AddTail() {
    Viper::Vec3 *values =
        new Viper::Vec3(snake->color->r, snake->color->g, snake->color->b);
    if (direction[0]) {
      Viper::Vec2 *newPos = new Viper::Vec2(-100, -100);

      b.manyEntity.AddEntity(*newPos, *tail->size, *values);
    }
    if (direction[1]) {
      Viper::Vec2 *newPos = new Viper::Vec2(-100, -100);
      b.manyEntity.AddEntity(*newPos, *tail->size, *values);
    }
    if (direction[2]) {
      Viper::Vec2 *newPos = new Viper::Vec2(-100, -100);

      b.manyEntity.AddEntity(*newPos, *tail->size, *values);
    }
    if (direction[3]) {
      Viper::Vec2 *newPos = new Viper::Vec2(-100, -100);

      b.manyEntity.AddEntity(*newPos, *tail->size, *values);
    }
  }

  void UpdateTail() {
    const int maxSnakeLength = 1000;
    Viper::Vec2 *oldPositions = new Viper::Vec2[maxSnakeLength];

    Entity *current = b.manyEntity.head;
    int index = 0;

    while (current != nullptr) {
      oldPositions[index] = *current->position;
      current = current->next;
      index++;
    }

    current = b.manyEntity.head->next;
    index = 0;

    while (current != nullptr) {
      *current->position = oldPositions[index];
      current->SetCenter(*current->size, *current->position);

      current = current->next;
      index++;
    }

    delete[] oldPositions;
  }

  void UpdateHead() {

    if (direction[0]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x + snake->size->x + tailOffset, snake->position->y);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
    }

    if (direction[1]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x - snake->size->x - tailOffset, snake->position->y);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
    }

    if (direction[2]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x, snake->position->y - snake->size->y - tailOffset);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
    }

    if (direction[3]) {

      Viper::Vec2 *newPos = new Viper::Vec2(
          snake->position->x, snake->position->y + snake->size->y + tailOffset);

      snake->SetPosition(newPos->x, newPos->y);
      snake->SetCenter(*snake->size, *snake->position);
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
      shouldGrow = true;
      SpawnFruit();
    }
  }

  void SelfCollisionCheck() {
    if (!snake || !snake->next)
      return;

    Entity *current = snake->next;
    int index = 1;
    int safeIgnore = 2;

    while (current != nullptr) {
      if (index >= safeIgnore) {
        if (calc.RectangularCheck(*snake, *current)) {
          state.currentGameState = state.End;

          return;
        }
      }
      current = current->next;
      index++;
    }
  }

  void WallCheck() {
    // bottom
    if (snake->position->y > ScreenHeight - (snake->size->y * 2)) {
      direction[3] = false;
      direction[2] = false;
      direction[1] = false;
      direction[0] = true;
      currentBounds = Bounds::Down;
    }
    // top
    if (snake->position->y < snake->size->y) {
      direction[2] = false;
      direction[0] = false;
      direction[3] = false;
      direction[1] = true;
      currentBounds = Bounds::Up;
    }
    // right
    if (snake->position->x > ScreenWidth - (snake->size->x * 2)) {
      direction[0] = false;
      direction[1] = false;
      direction[3] = false;
      direction[2] = true;
      currentBounds = Bounds::Right;
    }
    // left
    if (snake->position->x < snake->size->x) {
      direction[1] = false;
      direction[0] = false;
      direction[2] = false;
      direction[3] = true;
      currentBounds = Bounds::Left;
    }
    // bottom right, go up
    if (snake->position->x > ScreenWidth - (snake->size->x * 2) &&
        snake->position->y > ScreenHeight - (snake->size->y * 2)) {
      direction[0] = false;
      direction[1] = false;
      direction[3] = false;
      direction[2] = true;
      currentBounds = Bounds::Right;
    }
    // top right, go left
    if (snake->position->x > ScreenWidth - (snake->size->x * 2) &&
        snake->position->y < snake->size->y) {
      direction[0] = false;
      direction[2] = false;
      direction[3] = false;
      direction[1] = true;
      currentBounds = Bounds::Up;
    }
    // top left, go down
    if (snake->position->x < snake->size->x &&
        snake->position->y < snake->size->y) {
      direction[0] = false;
      direction[1] = false;
      direction[2] = false;
      direction[3] = true;
      currentBounds = Bounds::Left;
    }
    ////bottom left, go right
    if (snake->position->x < snake->size->x &&
        snake->position->y > ScreenHeight - (snake->size->y * 2)) {
      direction[3] = false;
      direction[1] = false;
      direction[2] = false;
      direction[0] = true;
      currentBounds = Bounds::Right;
    }
  }

  void TailCheck() {
    if (shouldGrow) {
      AddTail();
      shouldGrow = false;
    }
  }

  void DebugPrintSnake() {
    Entity *cur = b.manyEntity.head;
    int idx = 0;
    while (cur) {
      Viper::Println("Segment ");
      Viper::Println(idx++);
      Viper::Println(": (");
      Viper::Println(cur->position->x);
      Viper::Println(", ");
      Viper::Println(cur->position->y);
      Viper::Println(")\n");

      cur = cur->next;
    }
  }

  void GameUpdate() {
    CheckCollision();
    screen.deltaTime = screen.mainClock.getElapsedTime();
    if (screen.deltaTime.asSeconds() > time) {
      TailCheck();
      UpdateHead();
      UpdateTail();
      WallCheck();
      SelfCollisionCheck();
      screen.mainClock.restart();
    }
  }

  void GameFixedUpdate() {
    while (auto event = screen.window.pollEvent()) {
      if (event->getIf<sf::Event::Closed>()) {
        screen.window.close();
      }

      if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::W &&
            currentBounds != Bounds::Up) {
          currentBounds = 0;
          direction[0] = false;
          direction[1] = false;
          direction[3] = false;
          direction[2] = true;
        }

        if (keyPressed->code == sf::Keyboard::Key::S &&
            currentBounds != Bounds::Down) {
          currentBounds = 0;
          direction[0] = false;
          direction[1] = false;
          direction[2] = false;
          direction[3] = true;
        }

        if (keyPressed->code == sf::Keyboard::Key::A &&
            currentBounds != Bounds::Left) {
          currentBounds = 0;
          direction[0] = false;
          direction[2] = false;
          direction[3] = false;
          direction[1] = true;
        }

        if (keyPressed->code == sf::Keyboard::Key::D &&
            currentBounds != Bounds::Right) {

          currentBounds = 0;
          direction[1] = false;
          direction[2] = false;
          direction[3] = false;
          direction[0] = true;
        }
      }
    }
  }

  void GameDrawToScreen() {
    screen.window.clear();
    // screen.chunks->DrawAll(screen.window);

    DrawSnake(screen, b.manyEntity);

    if (fruit)
      screen.window.draw(screen.MakeDrawableCircle(*fruit));

    screen.window.display();
  }
  //-------------------------
  void TitleUpdate() {
    GetMousePos();
    screen.UpdateButtons();
    
    
  }

  void TitleFixedUpdate() {
    while (auto event = screen.window.pollEvent()) {
      if (event->getIf<sf::Event::Closed>()) {
        screen.window.close();
      }

      if (screen.titleButton.CanClick(*event, screen.mousePos)) {
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
    b.ClearEntities();
    f.ClearEntities();
    Init();
   
  }

  void EndUpdate() {
    GetMousePos();
    screen.UpdateEndButtons();
    

  }

  void EndFixedUpdate() {
    while (auto event = screen.window.pollEvent()) {
      if (event->getIf<sf::Event::Closed>()) {
        screen.window.close();
      }
      if (screen.endButton.CanClick(*event, screen.mousePos)) {
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
  Bucket b;
  Bucket f;
  // my libs
  Entity *snake;
  Entity *tail;
  EntityF *fruit;

  Trig::Pythagorian calc;
  PRandom::Random random;
  Euler::Grid grid;
  Snake::GameState state;

private:
  //---------------------
  // grid
  sf::RectangleShape chunk;

  Viper::Vec2 chunkSize;
  int numChunksX = 0;
  int numChunksY = 0;

private:
  // movement
  bool direction[4];

  int tailOffset = 5;

  float time = 0.16;

private:
  bool shouldGrow = false;
  int currentBounds = 0;
  enum Bounds { Up = 1, Down, Left, Right };

};
