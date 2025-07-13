/*

  MIT Licence
Author -> Brett Rogers
Reference -> Pythagorious 


this file is for reuseable vector math caluclations

*/

#include <math.h>
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


        float CalculateDistance(float a, float b)
        {
            float c = sqrt((a * a) + (b * b));
            return c;
        }

        float CalculcateAngle(float a, float b)
        {
            float angle = atan2(b, a);
            return angle;
        }

        bool RadialCheck(float r1, float r2, float a, float b)
        {
            float distance  = CalculateDistance(a, b);
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

        bool RectangularCheck(float centerPointA, float centerPointB, float widthA, float widthB, float heightA, float heightB, float a, float b)
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