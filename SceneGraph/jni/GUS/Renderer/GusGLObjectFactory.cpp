//
//  GusGLObjectFactory.cpp
//  helloGL
//
//  Created by tomato on 1/25/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "GusGLObjectFactory.h"
#include "Entity.h"
#include "EntityAnimated.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"

GusGLObjectFactory::GusGLObjectFactory() {
    
}

GusGLObject * GusGLObjectFactory::CreateObject(string type) {
    GusGLObject * obj;
    if(type == "Entity") {
        Entity * entity = new Entity();
        obj = entity;
    }
    if(type == "EntityAnimated") {
        EntityAnimated * entity = new EntityAnimated();
        obj = entity;
    }
    if(type == "Camera") {
        Camera * camera = new Camera();
        obj = camera;
    }
    if(type == "Light") {
        Light * light = new Light();
        obj = light;
    }
    if(type == "Mesh") {
        Mesh * mesh = new Mesh();
        obj = mesh;
    }
    if(type == "md5Object") {
        md5Object * mesh = new md5Object();
        obj = mesh;
    }
    if(type == "Material") {
        Material * material = new Material();
        obj = material;
    }
    return obj;
}

void GusGLObjectFactory::AddAttrib(GusGLObject * obj, string attrib_name, string value) {
        obj->_SetAttrib(attrib_name,value);
}

void GusGLObjectFactory::AddAttribToParent(GusGLObject* parent, GusGLObject* child) {
    parent->_SetAttribFromChild(child);
}

void GusGLObjectFactory::AddPostAttrib(GusGLObject * obj) {
    obj->_SetPostAttrib();
}

string GusGLObjectFactory::GetType(GusGLObject* arg) {
    string type;
    Entity*                ent      = dynamic_cast<Entity*>(arg);
    EntityAnimated*    entAnim      = dynamic_cast<EntityAnimated*>(arg);
    Mesh*                  mesh     = dynamic_cast<Mesh*>(arg);
    md5Object*             md5      = dynamic_cast<md5Object*>(arg);
    Material *             material = dynamic_cast<Material *>(arg);
    Camera*                camera   = dynamic_cast<Camera*>(arg);
    Light*                 light    = dynamic_cast<Light*>(arg);

    if (entAnim)    { type = "EntityAnimated"; }
    else if (ent) { type = "Entity"; }
    else if ( mesh )     { type = "Mesh"; }
    else if ( md5 )      { type = "md5Object"; }
    else if ( material ) { type = "Material"; }
    else if ( camera )   { type = "Camera"; }
    else if ( light )    { type = "Light"; }
    else                 { type = "GusGLObject"; }
    
   // cout << "[GUSOBJECTFACTORY][GetType] Ask type of " << arg->_GetName() << " " << type << endl;
    
    return type;
};
