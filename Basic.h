#pragma once
#include "Montserrat.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

namespace Dinzai {

struct SoftAssetManager {

  SoftAssetManager() {}

  sf::Font PassFont(std::string filePath) {
    sf::Font theFont;
    theFont.openFromFile(filePath);
    return theFont;
  }

  sf::Font PassEmbededFont(int num) {
    sf::Font theFont;

    if (num == 1) {
      theFont.openFromMemory(montserrat, montserrat_length);
    }

    return theFont;
  }
};

struct AllText {

  AllText() {
    asset = new SoftAssetManager();
    theHeaderFont = asset->PassEmbededFont(1);

    // Create text with the font (hack, but I don't know enough about SFML yet)
    theHeaderText = new sf::Text(theHeaderFont, "", 45);
    theHeaderText->setFillColor(sf::Color::Black);
  }

  ~AllText() { 
    delete asset; 
    delete theHeaderText;
  }

  void SetHeaderText(std::string word, sf::Vector2f position) {
    theHeaderText->setString(word);
    theHeaderText->setPosition(position);
  }

  void SetHeaderCharacterSize(int size) {
    theHeaderText->setCharacterSize(size);
  }

  void DrawHeaderTextToScreen(sf::RenderWindow &window) {
    window.draw(*theHeaderText);
  }

  sf::Text *theHeaderText;

  sf::Font theHeaderFont;

  SoftAssetManager *asset;
};

class Button {
public:
  Button() {}

  Button(sf::Text &text) { tempText = &text; }

  Button(sf::Sprite &sprite) { tempSprite = &sprite; }

  virtual void CheckCollision(sf::Vector2f mousePosition) {

    if (tempText->getGlobalBounds().contains(mousePosition)) {
      tempText->setFillColor(sf::Color::Green);
    } else {
      tempText->setFillColor(sf::Color::Black);
    }
  }

  virtual void CheckSpriteCollision(sf::Vector2f mousePosition) {

    isMousePressed = currentMouseState;

    if (tempSprite->getGlobalBounds().contains(mousePosition)) {
      tempSprite->setColor(sf::Color::Green);
    } else {
      tempSprite->setColor(sf::Color::White);
    }
  }

  virtual bool CanClick(sf::Vector2f mousePosition) {

    if (tempText->getGlobalBounds().contains(mousePosition)) {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        return true;
      }
    }
    return false;
  }

  virtual bool CanClick(sf::Event event, sf::Vector2f mousePosition) {
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
      if (mouseButtonPressed->button == sf::Mouse::Button::Left &&
          isMousePressed == false) {
        if (tempText->getGlobalBounds().contains(mousePosition)) {
          isMousePressed = true;
          currentMouseState = isMousePressed;
        }
      }
    }

    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>(); mouseButtonReleased && currentMouseState) {
      if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
        if (tempText->getGlobalBounds().contains(mousePosition)) {
          currentMouseState = false;
          return true;
        }
      }
    }

    return false;
  }

  virtual bool CanClickSprite(sf::Vector2f mousePosition) {

    if (tempSprite->getGlobalBounds().contains(mousePosition)) {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        return true;
      }
    }
    return false;
  }

  virtual bool CanClickSprite(sf::Event event, sf::Vector2f mousePosition) {

    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
      if (mouseButtonPressed->button == sf::Mouse::Button::Left &&
          isMousePressed == false) {
        if (tempSprite->getGlobalBounds().contains(mousePosition)) {
          isMousePressed = true;
          currentMouseState = isMousePressed;
        }
      }
    }

    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>(); mouseButtonReleased && currentMouseState) {
      if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
        if (tempSprite->getGlobalBounds().contains(mousePosition)) {
          currentMouseState = false;
          return true;
        }
      }
    }

    return false;
  }

  void Reset() {
    isMousePressed = false;
    currentMouseState = false;
  }

  bool currentMouseState = false;
  bool isMousePressed = false;

protected:
  sf::Text *tempText = nullptr;
  sf::Sprite *tempSprite = nullptr;
};

} // namespace Dinzai