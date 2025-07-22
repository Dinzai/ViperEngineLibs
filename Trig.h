/*

  MIT Licence
Author -> Brett Rogers
Reference -> Pythagorious


this file is for reuseable vector math caluclations

*/

#include <cstdlib>
#include <math.h>

#include "Debug.h"

namespace Viper
{
  struct Vec2 { 
  Vec2() : x(0), y(0) {}  
  Vec2(float a, float b) {
    this->x = a;
    this->y = b;
  }

  float x;
  float y;
};

struct Vec3 {
  Vec3(float a, float b, float c) {
    this->x = a;
    this->y = b;
    this->z = c;
  }

  float x;
  float y;
  float z;
};

struct Colour {
  Colour(Vec3 values) {
    this->r = values.x;
    this->g = values.y;
    this->b = values.z;
    this->a = 255;
  }

  float r, g, b, a;
};

 inline void Println(const char* s) {
    Debug::Print{}(s);
    Debug::Print{}("\n");
  }

  inline void Println(int n) {
    Debug::Print{}(n);
    Debug::Print{}("\n");
  };


} 


namespace GameObject {

struct SimpleObject {
  SimpleObject() {
    position = new Viper::Vec2(0, 0);
    size = new Viper::Vec2(0, 0);
    center = new Viper::Vec2(0, 0);
  }


  ~SimpleObject() {
    delete position;
    delete size;
    delete center;
  }

  void SetIndex(int i) { index = i; }

  void SetPosition(float x, float y) { position = new Viper::Vec2(x, y); }

  void SetCenter(Viper::Vec2& size, Viper::Vec2& position) 
  {

    float halfWidth = size.x / 2;
    float halfHeight = size.y / 2;

    float centerX = position.x + halfWidth;
    float centerY = position.y + halfHeight;

    center = new Viper::Vec2(centerX, centerY);
  }

  void SetCenter(float radius, Viper::Vec2& position)
  {
    float centerX = position.x + radius;
    float centerY = position.y + radius;
    center = new Viper::Vec2(centerX, centerY);
  }

  void SetColor(Viper::Vec3 &values) { color = new Viper::Colour(values); }

  Viper::Vec2 GetPosition() { return *position; }

  void SetSize(float width, float height) { size = new Viper::Vec2(width, height); }

  Viper::Vec2 GetSize() { return *size; }

  void SetRadius(float value) { radius = value; }

  float GetRadius() { return radius; }

  Viper::Vec2 *position;
  Viper::Vec2 *size;

  Viper::Vec2 *center;

  Viper::Colour *color;

  float radius;

  int index;
};

} // namespace GameObject


namespace Euler
{

    struct Grid
    {

        Viper::Vec2 CalculateGridChunk(Viper::Vec2& windowSize)
        {   
          
            if(windowSize.x == windowSize.y)
            {
              return windowSize;
            }
            Viper::Vec2 newChunk = Viper::Vec2(0, 0);
            if(windowSize.y < windowSize.x)
            {
              float nextWidth = windowSize.x - windowSize.y;
              newChunk = Viper::Vec2(nextWidth, windowSize.y);


            }

            if(windowSize.x < windowSize.y)
            {
              float nextHeight = windowSize.y - windowSize.x;
              newChunk = Viper::Vec2(windowSize.x, nextHeight);


            }

          
            return CalculateGridChunk(newChunk);

        }

    };

}



namespace Trig 
{

struct Pythagorian {
  Pythagorian() {}

  ~Pythagorian() {}

  float CalculateDistance(GameObject::SimpleObject &obj1,
                          GameObject::SimpleObject &obj2) {
    float a = abs(obj1.position->x - obj2.position->x);
    float b = abs(obj1.position->y - obj2.position->y);

    float c = sqrt((a * a) + (b * b));
    return c;
  }

  float CalculcateAngle(GameObject::SimpleObject &obj1,
                        GameObject::SimpleObject &obj2) {
    float a = abs(obj1.position->x - obj2.position->x);
    float b = abs(obj1.position->y - obj2.position->y);

    float angle = atan2(b, a);
    return angle;
  }

  bool RadialCheck(GameObject::SimpleObject &obj1,
                   GameObject::SimpleObject &obj2) {
    float distance = CalculateDistance(obj1, obj2);
    float sumRadial = obj1.radius + obj2.radius;

    if (sumRadial < distance) {
      return false;
    } else {
      return true;
    }
  }

    //distance check for collision detection between rectangles 

  bool RectangularCheck(GameObject::SimpleObject &obj1, GameObject::SimpleObject &obj2) 
  {
    //Object A
    float halfWidthA = obj1.size->x / 2;
    float halfHeightA = obj1.size->y / 2;

    float leftEdgeA = obj1.center->x - halfWidthA;
    float rightEdgeA = obj1.center->x + halfWidthA;

    float topEdgeA = obj1.center->y - halfHeightA;
    float bottomEdgeA = obj1.center->y + halfHeightA;

    //Object B

    float halfHeightB = obj2.size->y / 2;
    float halfWidthB = obj2.size->x / 2; 

    float leftEdgeB = obj2.center->x - halfWidthB;
    float rightEdgeB = obj2.center->x + halfWidthB;

    float topEdgeB = obj2.center->y - halfHeightB;
    float bottomEdgeB = obj2.center->y + halfHeightB;

    if (rightEdgeA < leftEdgeB) {
      return false;
    }
    if (leftEdgeA > rightEdgeB) {
      return false;
    }
    if (topEdgeA > bottomEdgeB) {
      return false;
    }
    if (bottomEdgeA < topEdgeB) {
      return false;
    }
    return true;
  }

  //distance check for collision detection between rectangles and circles
  bool RectangularToCircleCheck(GameObject::SimpleObject &obj1,
                        GameObject::SimpleObject &obj2)
  {

    //object A

    float halfWidthA = obj1.size->x / 2;
    float halfHeightA = obj1.size->y / 2;
 
    float leftEdgeA = obj1.center->x - halfWidthA;
    float rightEdgeA = obj1.center->x + halfWidthA;

    float topEdgeA = obj1.center->y - halfHeightA; 
    float bottomEdgeA = obj1.center->y + halfHeightA;

    //Object B

    float radius = obj2.radius;

    float centerX = obj2.center->x;
    float centerY = obj2.center->y;

    float leftEdgeB = centerX - radius;
    float rightEdgeB = centerX +  radius;

    float topEdgeB = centerY - radius;
    float bottomEdgeB = centerY + radius;

    if (rightEdgeA < leftEdgeB) {
      return false;
    }
    if (leftEdgeA > rightEdgeB) {
      return false;
    }
    if (topEdgeA > bottomEdgeB) {
      return false;
    }
    if (bottomEdgeA < topEdgeB) {
      return false;
    }
    return true;
  }

};
} 

namespace PRandom
{

  struct Random
  {
      int RandomInt(int min, int max)
      {
        int difference = max - min;
        int random = rand() % difference + min;
        return random; 
      }

      Viper::Vec2 RandomVec(Viper::Vec2& min, Viper::Vec2& max)
      {

        int differenceX = max.x - min.x;
        int differenceY = max.y - min.y;

        int randomX = rand() % differenceX + min.x;
        int randomY = rand() % differenceY + min.y;

        Viper::Vec2 randomVec = Viper::Vec2(randomX, randomY);
        return randomVec;
      }
  };

  
}