
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>


#include "Entity.h"

#define ScreenWidth 800
#define ScreenHeight 600

struct Screen
{

    Screen() : window(sf::VideoMode(ScreenWidth, ScreenHeight), "Example")
    {

    }


    sf::RectangleShape MakeDrawable(Entity& obj)
    {
        sf::RectangleShape shape;
        sf::Vector2f size;
        sf::Vector2f position;
        sf::Color color;

        color = sf::Color::Red;
        size = sf::Vector2f(sf::Vector2f(obj.size->x, obj.size->y));     
        position = sf::Vector2f(obj.position->x, obj.position->y);   
        
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(color);

        return shape;

    }

    //think about draw layers

    sf::Clock mainClock;
    sf::Time deltaTime;
    sf::Event event;
    sf::RenderWindow window;

};