/*

  MIT Licence
Author -> Brett Rogers
Reference -> Pythagorious


this file is for reuseable vector math caluclations

*/

#include <cstdlib>
#include <math.h>

namespace GameObject {

struct Vec2 {
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

struct SimpleObject {
  SimpleObject() {
    position = new Vec2(0, 0);
    size = new Vec2(0, 0);
    center = new Vec2(0, 0);
  }


  ~SimpleObject() {
    delete position;
    delete size;
    delete center;
  }

  void SetIndex(int i) { index = i; }

  void SetPosition(float x, float y) { position = new Vec2(x, y); }

  void SetCenter(Vec2& size, Vec2& position) 
  {

    float halfWidth = size.x / 2;
    float halfHeight = size.y / 2;

    float centerX = position.x + halfWidth;
    float centerY = position.y + halfHeight;

    center = new Vec2(centerX, centerY);
  }

  void SetColor(Vec3 &values) { color = new Colour(values); }

  Vec2 GetPosition() { return *position; }

  void SetSize(float width, float height) { size = new Vec2(width, height); }

  Vec2 GetSize() { return *size; }

  void SetRadius(float value) { radius = value; }

  float GetRadius() { return radius; }

  Vec2 *position;
  Vec2 *size;

  Vec2 *center;

  Colour *color;

  float radius;

  int index;
};

} // namespace GameObject

namespace Trig {

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

  bool RectangularCheck(GameObject::SimpleObject &obj1,
                        GameObject::SimpleObject &obj2) {
    float halfWidthA = obj1.size->x / 2;
    float halfWidthB = obj2.size->x / 2;

    float halfHeightA = obj1.size->y / 2;
    float halfHeightB = obj2.size->y / 2;

    float leftEdgeA = obj1.center->x - halfWidthA;
    float rightEdgeA = obj1.center->x + halfWidthA;

    float topEdgeA = obj1.center->y -
                     halfHeightA; // minus because SFML uses - as Up + as down
    float bottomEdgeA = obj1.center->y + halfHeightA;

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
};
} // namespace Trig