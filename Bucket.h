#pragma once
#include "Entity.h"


struct Bucket
{

    Bucket()
    {

    }


    void SetEntity(GameObject::Vec2& position, GameObject::Vec2& size, GameObject::Vec3& values) 
    {
        manyEntity.AddEntity(position, size, values);
           
    }

    void RemoveEntity(int index)
    {
        manyEntity.RemoveNode(index);
    }

    void SetEntity(GameObject::Vec2& position, float radius, GameObject::Vec3& values) 
    {
        manyEntity.AddEntity(position, radius, values);
           
    }



    LinkedEntity manyEntity;


};
