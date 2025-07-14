/*

  MIT Licence
Author -> Brett Rogers
Reference -> Pythagorious 


this file is for reuseable vector math caluclations

*/

#include <cstdlib>
#include <math.h>
namespace Trig
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



    struct Pythagorian
    {
        Pythagorian()
        {

        }

        ~Pythagorian()
        {

        }


        float CalculateDistance(Vec2& obj1, Vec2& obj2)
        {
            float a = abs(obj1.x - obj2.x);
            float b = abs(obj1.y - obj2.y);

            float c = sqrt((a * a) + (b * b));
            return c;
        }

        float CalculcateAngle(Vec2 obj1, Vec2 obj2)
        {
            float a = abs(obj1.x - obj2.x);
            float b = abs(obj1.y - obj2.y);

            float angle = atan2(b, a);
            return angle;
        }

        bool RadialCheck(float r1, float r2, Vec2& obj1, Vec2& obj2)
        {
            float distance  = CalculateDistance(obj1, obj2);
            float sumRadial = r1 + r2;

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
        bool RectangularCheck(float centerPointA, float centerPointB, float widthA, float widthB, float heightA, float heightB)
        {
            float halfWidthA = widthA / 2;
            float halfWidthB = widthB / 2;

            float halfHeightA = heightA / 2;
            float halfHeightB = heightB / 2;


            float leftEdgeA = centerPointA - halfWidthA;
            float rightEdgeA = centerPointA + halfWidthA;

            float topEdgeA = centerPointA - halfWidthA;//minus because SFML uses - as Up + as down
            float bottomEdgeA = centerPointA + halfWidthA;

            float leftEdgeB = centerPointB - halfWidthB;
            float rightEdgeB = centerPointB + halfWidthB;

            float topEdgeB = centerPointB - halfWidthB;
            float bottomEdgeB = centerPointB + halfWidthB;

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