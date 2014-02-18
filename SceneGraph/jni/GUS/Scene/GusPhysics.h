//
//  GusPhysics.h
//  helloGL
//
//  Created by tomato on 3/27/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "../Renderer/Entity.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

class PhysicalObject;

class GusPhysicsCharacter;

class GusPhysicsEngine {
        public :
    GusPhysicsEngine();
 void                                    UpdateTime(float seconds);
 btRigidBody *                           RegisterBox(GusVec3 halfDiag, GusVec3 position,float mass);
 void                                    AddBox(PhysicalObject * entity);
 void									 RemoveBox(PhysicalObject * entity);
 void									 ReAddBox(PhysicalObject * entity);
 void                                    AddCharacter(PhysicalObject* entity);
 void                                    SetTransformMatrix(int object_idx);
 GusVec3                                 GetVelocity(int object_idx);
 void                                    SetVelocity(GusVec3 velocity,int object_idx);
 GusVec3                                 GetPosition(int object_idx);
 btRigidBody*                            GetRigidBody(int object_idx);
 void                                    SetPosition(GusVec3 position, int object_idx);
 void                                    UpdateEntity();
 btKinematicCharacterController*         GetCharacterController(int object_idx);

 btVector3								 GetGravity(){return dynamicsWorld->getGravity();};
 map<int,string>						 GetEntityList();
    
 PhysicalObject*                                 RayCast(btVector3 from, btVector3 to);

        private :
    
     btDefaultCollisionConfiguration *       collisionConfiguration;
     btCollisionDispatcher *                 dispatcher;
     btBroadphaseInterface *                 overlappingPairCache;
     btSequentialImpulseConstraintSolver *   solver;
     btDiscreteDynamicsWorld *               dynamicsWorld;
     btAlignedObjectArray<btCollisionShape*> collisionShapes;
    
     std::map<int, btCollisionObject*>      entities;
     std::map<int, GusPhysicsCharacter*>    characters;
     std::map<int, PhysicalObject*>         entitiesRef;
};




class GusPhysicsCharacter {
        public :
    GusPhysicsCharacter();
    GusPhysicsCharacter(PhysicalObject* entity);
    ~GusPhysicsCharacter();
    
    btKinematicCharacterController*        GetCharacterController(){return characterController;};
    btPairCachingGhostObject*              GetGhostObject(){return ghostObject;};
    btRigidBody*                           GetCapsuleBody(){return capsuleBody;};
    
        private :
    btKinematicCharacterController* characterController;
    btPairCachingGhostObject* ghostObject;
    btRigidBody* capsuleBody;
    
    btVector3       walkingDirection;
    btScalar        walkingSpeed;
};




class GusPhysicsTools {
        public :
    
    static btVector3                getRayTo(float x,float y);
    
        private :
    
};
















