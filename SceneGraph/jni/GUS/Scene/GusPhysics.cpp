//
//  GusPhysics.cpp
//  helloGL
//
//  Created by tomato on 3/27/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "GusPhysics.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Renderer.h"
#include "SceneObject.h"

GusPhysicsEngine::GusPhysicsEngine(){
    
    collisionConfiguration = new btDefaultCollisionConfiguration();
   
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
	overlappingPairCache = new btDbvtBroadphase();
    
	solver = new btSequentialImpulseConstraintSolver;
    
	dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher,
                                                 overlappingPairCache,
                                                 solver,
                                                 collisionConfiguration);
    
	dynamicsWorld->setGravity(btVector3(0,-9.81,0));
}


void GusPhysicsEngine::UpdateTime(float seconds) {
    dynamicsWorld->stepSimulation(seconds, 32, 1/120.0f);
}

btRigidBody * GusPhysicsEngine::RegisterBox(GusVec3 halfDiag, GusVec3 position,
float in_mass)
{
    btCollisionShape *colShape =
    new btBoxShape( btVector3( halfDiag.x , halfDiag.y , halfDiag.z) ) ;
    
    //collisionShapes.push_back(colShape);
    
    btTransform startTransform;
    startTransform.setIdentity();
    
    btScalar	mass(in_mass);
    btVector3 localInertia(0,0,0);
    if(mass > 0.0f)
    {
        colShape->calculateLocalInertia(mass,localInertia);
    }
    
    startTransform.setOrigin(btVector3( position.x , position.y , position.z));
    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo( mass , myMotionState , colShape ,localInertia);
    rbInfo.m_restitution = 0.5f;
    rbInfo.m_friction = 0.99f;
    
    btRigidBody* body = new btRigidBody(rbInfo);
    
    dynamicsWorld->addRigidBody(body);
    
    return body;
}


void GusPhysicsEngine::AddBox(PhysicalObject * scene_obj)
{
    GusVec3 pos = scene_obj->GetPosition();
    int idx = scene_obj->_GetId();
    if( 0 == entities.count(idx)) {
    entities[idx] = RegisterBox(scene_obj->GetHalfVec(),pos,scene_obj->GetMass());
    entities[idx]->setUserPointer(scene_obj);
    entitiesRef[idx] = scene_obj;
    }
    
    cout << "[GUSPHYSICSENGINE][AddBox] add to the Physical World " << endl
         << scene_obj->_GetName() << endl
         << "Mass " << scene_obj->GetMass() << endl
         << "HalfVec " << scene_obj->GetHalfVec().x << " "
                       << scene_obj->GetHalfVec().y << " "
                       << scene_obj->GetHalfVec().z << " " << endl
         << " Original Center " << scene_obj->GetPosition().x << " "
                                << scene_obj->GetPosition().y << " "
                                << scene_obj->GetPosition().z << " " << endl
         << " Pos " << pos.x << " "
                    << pos.y << " "
                    << pos.z << " "
         << endl;
    
}



void GusPhysicsEngine::RemoveBox(PhysicalObject * entity) {
	btCollisionObject* obj = entities[entity->_GetId()];
	dynamicsWorld->removeCollisionObject(obj);
	PhysicalObject * ent = (PhysicalObject *)obj->getUserPointer();
}



void GusPhysicsEngine::ReAddBox(PhysicalObject * entity) {

	dynamicsWorld->addRigidBody(GetRigidBody(entity->_GetId()));
}



map<int,string> GusPhysicsEngine::GetEntityList(){
	map<int,string> ret_map;
	for(auto pair : entities) {
		ret_map[pair.first] = ((PhysicalObject*)pair.second->getUserPointer())->_GetName();
	}

	for (int i= dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
		{
				btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
				PhysicalObject * ent = (PhysicalObject *)obj->getUserPointer();
			   // dynamicsWorld->removeCollisionObject(obj);
				LOGE("[SCENE][LoadStage] Ent remove physicalWorld %d %s",ent->_GetId(), ent->_GetName().c_str());
		}
	LOGE("[SCENE][LoadStage] Physical World contains %d entities",dynamicsWorld->getNumCollisionObjects());

	return ret_map;
}


void GusPhysicsEngine::AddCharacter(PhysicalObject* charac) {
    int idx  = charac->_GetId();
    characters[idx] = new GusPhysicsCharacter(charac);
    entities[idx] = characters[idx]->GetGhostObject();
    entities[idx]->setUserPointer(charac);
    
    overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    dynamicsWorld -> addCollisionObject(characters[idx]->GetGhostObject(),
                                        btBroadphaseProxy::CharacterFilter,
                                        btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
    dynamicsWorld -> addAction(characters[idx]->GetCharacterController());
    dynamicsWorld -> getDispatchInfo().m_allowedCcdPenetration=0.0001f;
}


void GusPhysicsEngine::SetTransformMatrix(int object_idx)
{
    GLfloat* result = entitiesRef[object_idx]->GetMODEL_v();
    LoadIdentity4fv(result);
    
    //btRigidBody *body = GetRigidBody(object);
        //      NSLog(@"physicsTransformForModel:%p", obj);
    btCollisionObject * col_obj = entities[object_idx];
    
        if(NULL != col_obj)
        {
            btTransform trans;
            //body->getMotionState()->getWorldTransform(trans);
            trans = col_obj->getWorldTransform();
            
            result[0] = trans.getBasis()[0].x();
            result[1] = trans.getBasis()[0].y();
            result[2] = trans.getBasis()[0].z();
            
            result[4] = trans.getBasis()[1].x();
            result[5] = trans.getBasis()[1].y();
            result[6] = trans.getBasis()[1].z();
            
            result[8]  = trans.getBasis()[2].x();
            result[9]  = trans.getBasis()[2].y();
            result[10] = trans.getBasis()[2].z();
            
            result[12] =  trans.getOrigin().x();
            result[13] =  trans.getOrigin().y();
            result[14] =  trans.getOrigin().z();
        }
}


GusVec3 GusPhysicsEngine::GetVelocity(int object_idx)
{
    GusVec3 result = GusVec3();
    btRigidBody *body = GetRigidBody(object_idx);
        
        if(NULL != body)
        {
            btVector3 velocity = body->getLinearVelocity();
            
            result.x = velocity.x();
            result.y = velocity.y();
            result.z = velocity.z();
        }
    return result;
}


void GusPhysicsEngine::SetVelocity(GusVec3 in_velocity,int object_idx)
{
    if(1 == entities.count(object_idx)) {
        btCollisionObject *obj = entities[object_idx];
        btRigidBody *body = btRigidBody::upcast(obj);
        
        //NSLog(@"physicsSetVelocity:%p {%f, %f, %f}",
        //   obj, aVelocity.x, aVelocity.y, aVelocity.z);
        
        if(NULL != body) {
            btVector3 velocity( in_velocity.x, in_velocity.y, in_velocity.z);
            body->setLinearVelocity(velocity);
        }
    }
}

GusVec3 GusPhysicsEngine::GetPosition(int object_idx){
        GusVec3 result = GusVec3();
    
        btRigidBody *body = GetRigidBody(object_idx);
        
        if(NULL != body)
        {
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);
            
            result.x = trans.getOrigin().x();
            result.y = trans.getOrigin().y();
            result.z = trans.getOrigin().z();
        }
    return result;
}


void GusPhysicsEngine::SetPosition(GusVec3 position, int object_idx) {
    
    btRigidBody *body = GetRigidBody(object_idx);
    btCollisionObject* obj = entities[object_idx];
    
    if(body)
    {
        float aMass = 1.0f;
        
		if (body && body->getMotionState())
		{
            aMass = body->getInvMass();
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject( obj );
		delete obj;
        
       // RegisterBox(object->GetHalfVec(), position, aMass);
    }
}



btRigidBody* GusPhysicsEngine::GetRigidBody(int object_idx) {
    btRigidBody *body = NULL;
    if(1 == entities.count(object_idx))
    {   
        btCollisionObject *obj = entities[object_idx];
        body = btRigidBody::upcast(obj);
    }
    return body;
}

btKinematicCharacterController*  GusPhysicsEngine::GetCharacterController(int object_idx){
    return characters[object_idx]->GetCharacterController();
}

void GusPhysicsEngine::UpdateEntity() {
    map<int , btCollisionObject*>::const_iterator it = entities.begin();
    for(;it != entities.end() ; ++it){
        SetTransformMatrix(it->first);
    }
}


PhysicalObject* GusPhysicsEngine::RayCast(btVector3 from, btVector3 to)
{
	 PhysicalObject* ent = NULL;
	 //btVector3 grav = dynamicsWorld->getGravity();
	 //LOGE("RAYCAST!!!!!!! %f %f %f",grav.x(),grav.y(),grav.z());

	    btCollisionWorld::ClosestRayResultCallback rayCallback(from,to);
	    dynamicsWorld->rayTest(from, to, rayCallback);
	    if(rayCallback.hasHit()){
	       ent = (PhysicalObject *)rayCallback.m_collisionObject->getUserPointer();
	     LOGE( "[GUSPHYSICSENGINE][RayCast] Entity Touched! %s" ,ent->_GetName().c_str() );
	    }

	    return ent;
}


/////////////////////////////////////////////////////////////
//  GUSPHYSICSCHARACTER METHOD DEFINITIONS
/////////////////////////////////////////////////////////////


GusPhysicsCharacter::GusPhysicsCharacter(){
    
}


GusPhysicsCharacter::GusPhysicsCharacter(PhysicalObject* entity){
    
    GusVec3 pos = entity->GetPosition();
    //GusVec3* halfVec = entity->GetHalfVec();
    
    
     cout << "[GUSPHYSICSENGINE][GusPhysicsCharacter] add to the Physical World " << endl
     << entity->_GetName() << endl
     << "Mass " << entity->GetMass() << endl
     << "HalfVec " << entity->GetHalfVec().x << " "
     << entity->GetHalfVec().y << " "
     << entity->GetHalfVec().z << " " << endl
     << " Pos " << pos.x << " "
     << pos.y << " "
     << pos.z << " "
     << endl;
     

    
    
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin (btVector3(pos.x, pos.y, pos.z));
    
    // Le Ghost, c'est lui qui va gèrer les collisions
    ghostObject = new btPairCachingGhostObject();
    ghostObject->setWorldTransform(startTransform);
    //sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    
    // Hauteur: c'est en m, mais c'est une capsule, donc les extrémités dependent du Width
    btScalar characterHeight = 8.5f ;
    
    // Rayon de la largeur
    btScalar characterWidth = 3.1f  ;
    
    // La capsule
    btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
    ghostObject->setCollisionShape (capsule);
    ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
    
    // La hauteur de marche maximum
    btScalar stepHeight = btScalar(0.60f);
    
    
    characterController = new btKinematicCharacterController(ghostObject, capsule, stepHeight);
    characterController -> setMaxSlope(btScalar(0.872664626));
    
    characterController->setWalkDirection(btVector3(0.f,0.f,1.f));
    characterController->setVelocityForTimeInterval(btVector3(0.f,0.f,10.f), 30.f);
    
}


GusPhysicsCharacter::~GusPhysicsCharacter(){

}


btVector3	GusPhysicsTools::getRayTo(float x,float y)
{
    GusVec3 target = Camera::GetCameraTargetPosition();
	GusVec3 pos    = Camera::GetCameraPosition();
    GusVec3 up     = Camera::GetCameraUp();

    LOGE("[GUSPHYSICSTOOLS][getRayTo] CAMERA INPUT 	target %f %f %f",target.x,target.y,target.z);
    LOGE("										 	pos    %f %f %f",pos.x,pos.y,pos.z);
    LOGE("										    up     %f %f %f",up.x,up.y,up.z);

    /*
	if (m_ortho)
	{
        
		btScalar aspect;
		btVector3 extents;
		aspect = m_glutScreenWidth / (btScalar)m_glutScreenHeight;
		extents.setValue(aspect * 1.0f, 1.0f,0);
		
		extents *= m_cameraDistance;
		btVector3 lower = m_cameraTargetPosition - extents;
		btVector3 upper = m_cameraTargetPosition + extents;
        
		btScalar u = x / btScalar(m_glutScreenWidth);
		btScalar v = (m_glutScreenHeight - y) / btScalar(m_glutScreenHeight);
		
		btVector3	p(0,0,0);
		p.setValue((1.0f - u) * lower.getX() + u * upper.getX(),(1.0f - v) * lower.getY() + v * upper.getY(),m_cameraTargetPosition.getZ());
		return p;
	}
    */
    
//	float top = 1.f;
//	float bottom = -1.f;
//	float nearPlane = 1.f;
//	float tanFov = (top-bottom)*0.5f / nearPlane;
	//float fov = btScalar(2.0) * btAtan(tanFov);
    const GLfloat PIf = 0.0174532925199;
    float fov = 40.f * PIf;
    
	btVector3	rayFrom = btVector3(pos.x,pos.y,pos.z);
	btVector3 rayForward = btVector3(target.x,target.y,target.z) - rayFrom;
	rayForward.normalize();
	float farPlane = 10000.f;
	rayForward*= farPlane;
    
	btVector3 vertical = btVector3(up.x,up.y,up.z);
    
	btVector3 hor;
	hor = rayForward.cross(vertical);
	hor.normalize();
	vertical = hor.cross(rayForward);
	vertical.normalize();
    
	float tanfov = tanf(0.5f*fov);
    
    
	hor *= 2.f * farPlane * tanfov;
	vertical *= 2.f * farPlane * tanfov; 
    
	btScalar aspect;
	
	aspect = Renderer::frameView[0] / (btScalar)Renderer::frameView[1];
	
	LOGE("[GUSPHYSICSTOOLS][getRayTo] frameview %f %d %d", aspect, Renderer::frameView[0],Renderer::frameView[1]);

	hor*=aspect;
    
    
	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor /float(Renderer::frameView[0]);
	btVector3 dVert = vertical /float(Renderer::frameView[1]);
    
    
	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;
	return rayTo;
     
}



















