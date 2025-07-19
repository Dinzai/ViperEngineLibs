#pragma once
#include "Entity.h"


struct Bucket
{

    Bucket()
    {

    }


    void SetEntity(Viper::Vec2& position, Viper::Vec2& size, Viper::Vec3& values) 
    {
        manyEntity.AddEntity(position, size, values);
           
    }

    void RemoveEntity(int index)
    {
        otherManyEntity.RemoveNode(index);
    }

    void SetEntity(Viper::Vec2& position, float radius, Viper::Vec3& values) 
    {
        otherManyEntity.AddEntity(position, radius, values);
           
    }



    LinkedEntity manyEntity;
    LinkedEntityF otherManyEntity;


};
