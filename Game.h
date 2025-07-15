
#include "iostream"
#include "Window.h"
#include "Bucket.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>

struct Game
{

    Game()
    {
        b.SetEntity();
        player = *b.manyEntity.GetNode(1);
    }

private:

    void Update()
    {
        screen.deltaTime = screen.mainClock.getElapsedTime();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.position->x -= player.speed * screen.deltaTime.asSeconds();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.position->x += player.speed * screen.deltaTime.asSeconds();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player.position->y -= player.speed * screen.deltaTime.asSeconds();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.position->y += player.speed * screen.deltaTime.asSeconds();
        }
        screen.mainClock.restart();
        std::cout << "The position X: " << player.position->x << '\n';

    }

    void FixedUpdate()
    {
        while (screen.window.pollEvent(screen.event)) 
            {
                if(screen.event.type == sf::Event::Closed)
                {
                    screen.window.close();
                }
            }
    }

    void DrawToScreen()
    {
        screen.window.clear();
        screen.window.draw(screen.MakeDrawable(player));
        screen.window.display();
    }

    void Loop()
    {
        while(screen.window.isOpen())
        {
            
            Update();
            FixedUpdate();
            DrawToScreen();

        }
    }

public:
    void Run()
    {
        Loop();
    }

private:
    Screen screen;
    Bucket b;
    Entity player;

};

