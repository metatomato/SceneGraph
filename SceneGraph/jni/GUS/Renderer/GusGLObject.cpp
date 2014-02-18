//
//  GusGLObject.cpp
//  helloGL
//
//  Created by tomato on 1/25/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "GusGLObject.h"
#include "GusGLObjectFactory.h"

GusGLObject::GusGLObject() {
    
}

GusGLObject::~GusGLObject(){
  //  LOGE("[GUSGLOBJECT][~GusGLObject] GusGLObject DESTRUCTOR CALL \n");
}

const GusGLObject::GUS_OBJ_ID GusGLObject::GetObjType(GusGLObject *obj) {
    GUS_OBJ_ID result;
    string type = GusGLObjectFactory::GetType(obj);
    if(type.compare("Entity") == 0 )           result = GUS_OBJ_ID::ENTITY;
    if(type.compare("Material") == 0 )         result = GUS_OBJ_ID::MATERIAL;
    if(type.compare("Mesh") == 0 )             result = GUS_OBJ_ID::MESH;
    if(type.compare("Camera") == 0 )           result = GUS_OBJ_ID::CAMERA;
    if(type.compare("Light") == 0 )            result = GUS_OBJ_ID::LIGHT;
    if(type.compare("EntityAnimated") == 0 )   result = GUS_OBJ_ID::ENTITY_ANIM;
    if(type.compare("md5Object") == 0 )        result = GUS_OBJ_ID::MD5;
    return result;
}




//////////////////////////////////////////////////////////
// SCENEBLE METHOD DEFINITIONS
//////////////////////////////////////////////////////////



Scenable::Scenable(){
    
}