//
//  RenderPass.h
//  helloGL
//
//  Created by tomato on 9/18/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include "GUS.h"
#include "../Renderer/Camera.h"
#include "../Renderer/GusGLObject.h"
#include "../Scene/SceneObject.h"

class Entity;
class Light;

template <class T>
class RenderPass{
        public :
    //static RenderPass RenderToDBG(const RenderPass& pass);
    
    RenderPass(vector<T*> entity_vec, vector<T*> light_vec, T* camera,RENDER_PASS r_type);
    
    void                        SetName(string in_name){name = in_name;};
    string                      GetName(){return name;};
    
    RENDER_PASS                 GetRType() const {return rType;};
    void                        SetRType(RENDER_PASS type){rType = type;};
    DEBUG_MODE                  GetRDBGType() const {return rDBGType;};
	void                        SetRDBGType(DEBUG_MODE type){rDBGType = type;};
    SCENE_PASS                  GetSType() const {return sType;};
    void                        SetSType(SCENE_PASS type){sType = type;};
    
    int							GetPrimitive(){return primitive;}
    void						SetPrimitive(int primitive_type){primitive = primitive_type;}
    int							GetMaterial(){return material;}
	void						SetMaterial(int mat){material = mat;}

    void                        Add(T* ent){entities.push_back(ent);};
    const vector<T*>&           GetEntities()const{return entities;};
    const vector<T*>&           GetLight()const{return lights;};
    T&                          GetCamera()const{return *camera;};
    
    
        private :
    string                      name;
    RENDER_PASS                 rType;
    DEBUG_MODE					rDBGType;
    SCENE_PASS                  sType;
    vector<T*>                  entities;
    vector<T*>                  lights;
    T*                          camera;
    int							primitive;
    int							material;
};




/*
class RenderPass{
    public :
    static RenderPass RenderToDBG(const RenderPass& pass);
    
    RenderPass();
    RenderPass(vector<GusGLObject*> collection,Light* light, Camera* camera);
    
    RENDER_PASS                 GetType() const {return type;};
    void                        Add(Entity* ent){entities.push_back(ent);};
    const vector<Entity*>&      GetEntities()const{return entities;};
    const vector<Light*>&       GetLight()const{return lights;};
    Camera&                     GetCamera()const{return *camera;};
    
    vector<Entity *> Entities();
    vector<Light *>  Lights();
    
    
    private :
    string name;
    RENDER_PASS type;
    vector<Entity *> entities;
    vector<Light *>  lights;
    Camera *         camera;
    
};
*/
