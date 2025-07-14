/*

  MIT Licence
Author -> Brett Rogers
Reference -> Pythagorious 


this file is for reuseable vector math caluclations

*/

#include <cstdlib>
#include <math.h>


namespace GameObject 
{

    struct Vec2
    {
        Vec2(float a, float b)
        {
            this->x = a;
            this->y = b;
        }


        float x;
        float y;

    };

    struct SimpleObject
    {
        SimpleObject()
        {
            position = nullptr;
            size = nullptr;
        }

        ~SimpleObject()
        {
            delete position;
            delete size;
        }

        void SetPosition(float x, float y)
        {
            position = new Vec2(x, y);
        }

        void SetCenter(SimpleObject& obj)
        {
            float halfWidth = obj.size->x / 2;
            float halfHeight = obj.size->y / 2;
            
            float centerX = obj.position->x + halfWidth;
            float centerY = obj.position->y + halfHeight;

            obj.center = new Vec2(centerX, centerY);

        }

        Vec2 GetPosition()
        {
            return *position;
        }

        void SetSize(float width, float height)
        {
            size = new Vec2(width, height);
        }

        Vec2 GetSize()
        {
            return *size;
        }

        void SetRadius(float value)
        {
            radius = value;
        }

        float GetRadius()
        {
            return radius;
        }

        Vec2* position;
        Vec2* size;

        Vec2* center;

        float radius;
    };

}



namespace Trig
{

    struct Pythagorian
    {
        Pythagorian()
        {

        }

        ~Pythagorian()
        {

        }


        float CalculateDistance(GameObject::SimpleObject& obj1, GameObject::SimpleObject& obj2)
        {
            float a = abs(obj1.position->x - obj2.position->x);
            float b = abs(obj1.position->y - obj2.position->y);

            float c = sqrt((a * a) + (b * b));
            return c;
        }

        float CalculcateAngle(GameObject::SimpleObject& obj1, GameObject::SimpleObject& obj2)
        {
            float a = abs(obj1.position->x - obj2.position->x);
            float b = abs(obj1.position->y - obj2.position->y);

            float angle = atan2(b, a);
            return angle;
        }

        bool RadialCheck(GameObject::SimpleObject& obj1, GameObject::SimpleObject& obj2)
        {
            float distance  = CalculateDistance(obj1, obj2);
            float sumRadial = obj1.radius + obj2.radius;

            if(sumRadial < distance)
            {
                return false;
            }
            else
            {
                return true;
            }

        }
//redo rectanglular check, add vec
        bool RectangularCheck(GameObject::SimpleObject& obj1, GameObject::SimpleObject& obj2)
        {
            float halfWidthA = obj1.size->x / 2;
            float halfWidthB = obj2.size->x / 2;

            float halfHeightA = obj2.size->y / 2;
            float halfHeightB = obj2.size->y / 2;


            float leftEdgeA = obj1.center->x - halfWidthA;
            float rightEdgeA = obj1.center->x  + halfWidthA;

            float topEdgeA = obj1.center->y  - halfWidthA;//minus because SFML uses - as Up + as down
            float bottomEdgeA = obj1.center->y + halfWidthA;

            float leftEdgeB = obj2.center->x  - halfWidthB;
            float rightEdgeB = obj2.center->x  + halfWidthB;

            float topEdgeB = obj2.center->y - halfWidthB;
            float bottomEdgeB = obj2.center->y + halfWidthB;

            if(rightEdgeA < leftEdgeB)
            {
                return false;
            }
            if(leftEdgeA > rightEdgeB)
            {
                return false;
            }
            if(topEdgeA > bottomEdgeB)
            {
                return false; 
            }
            if(bottomEdgeA < topEdgeA)
            {
                return false;
            }
            return true;

        }


    };
}