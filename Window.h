
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


#include "Entity.h"




struct Screen
{

    Screen() : window(sf::VideoMode(ScreenWidth, ScreenHeight), "Example")
    {

    }


    sf::RectangleShape MakeDrawableRect(Entity& obj)
    {
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

        return shape;

    }

    sf::CircleShape MakeDrawableCircle(Entity& obj)
    {
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

    sf::RectangleShape MakeChunk()
    {
        GameObject::Vec2* windowSize = new GameObject::Vec2(ScreenWidth, ScreenHeight);
        Euler::Grid grid;
        GameObject::Vec2 newSize = grid.CalculateGridChunk(*windowSize);
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(newSize.x, newSize.y));
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineThickness(0.8);
        shape.setOutlineColor(sf::Color::White);
        return shape;
    }


	void MakeGrid(int worldWidth, int worldHeight, float offset)
	{
		for (int i = 0; i < worldWidth; i++)
		{
			for (int j = 0; j < worldHeight; j++)
			{
				sf::RectangleShape chunk = MakeChunk();
				chunk.setPosition(i * chunk.getSize().x + offset, j * chunk.getSize().y + offset);
                //Look at Entity.h, implmement chunks the same way
			}
		}

	}

    

    //think about draw layers

    sf::Clock mainClock;
    sf::Time deltaTime;
    sf::Event event;
    sf::RenderWindow window;

};