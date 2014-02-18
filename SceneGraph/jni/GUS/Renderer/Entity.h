//
//  Entity.h
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include "../Common/GUS.h"
#include "Mesh.h"
#include "Material.h"
#include "GusGLObjectFactory.h"
#include "Renderer.h"
#include "../Common/Matrix.h"
#include "../Scene/SpaceObject.h"

class EntityDBG;


using namespace std;

struct EntityAttribs_t{
    typedef void (EntityAttribs_t::*fctPtr)(string);
    
    string      name;
    Mesh *      mesh;
    Material *  material;
    
    map<string, fctPtr> mapFunction;
    
    EntityAttribs_t(){
        mapFunction["name"    ] = &EntityAttribs_t::Name;
        mapFunction["mesh"    ] = &EntityAttribs_t::MaterialFunc;
        mapFunction["material"] = &EntityAttribs_t::MeshFunc;
        
        name = "";

        mesh = nullptr;
        material = nullptr;
    }
    
    void Name(string is) {
        char temp[64];
        sscanf(is.c_str(), "%s",temp);
        name = temp;
    }

    void MeshFunc(string is) {
       // sscanf(is.c_str(), "%d",&mesh);
    }
    void MaterialFunc(string is) {
    //    sscanf(is.c_str(), "%d",&material);
    }
    
    vector<string> GetAttribsList() {
        vector<string> vec;
        for (auto pair : mapFunction) {
            vec.push_back(pair.first);
        }
        return vec;
    }
    
    void Call( const string & s, string is ) {
    	fctPtr fp = mapFunction[s];
    	return (this->*fp)(is);
    }
};



class Entity : public Scenable {
    
        public :
    Entity();
    Entity(string name);
    Entity(Mesh* mesh, Material* material);
    virtual ~Entity();
    
    const static  vector <string>   attribsList;
    const static GUS_OBJ_ID         type = GUS_OBJ_ID::ENTITY;
    
    virtual void                    _SetAttrib(string attrib_name, string value);
    virtual void                    _SetPostAttrib();
    virtual void                    _SetAttribFromChild(GusGLObject* child);
    virtual int                     _GetType() const 	{return (int)GUS_OBJ_ID::ENTITY;};
    string                          _GetName()const		{return attribs.name;};
    
    virtual const Mesh *            GetMesh()const		{return attribs.mesh;};
    Material *                      GetMaterial()const	{return attribs.material;};
    
    void                            SetShader(int pass_type, Shader* shader)	{shaders[pass_type]=shader;};
    virtual Shader *                GetShader(RENDER_PASS pass_type)const		{return shaders[pass_type];};
    void							InitShader();

    virtual void                    SetGLAttribs(RENDER_PASS type);
    virtual void                    SetModel(RENDER_PASS type);
    virtual void                    Render();

    string                          GetParent() const			{return attribs.mesh->GetParent();}
    EntityDBG*						GetEntityDBG()				{return entityDBG;}

    virtual void                    Sync();
    const GLfloat*                  GetModel() const;
    void							ScaleModel(GLfloat factor)	{scale = factor; isScale = true;}
    void							ResetScale()				{scale = 1.f; isScale = false;}

    void							SetOutlined(bool state) {isOutlined = state;}
    bool							IsOutlined() 			{return isOutlined;}

        protected :
    const GLfloat*                  MODEL;
    GLfloat*	                    MODEL_m;
    GLfloat							scale;
    bool							isScale;
    bool							isOutlined;
    EntityAttribs_t                 attribs;
    Shader *                        shaders[RENDER_PASS_MAX];
    EntityDBG*						entityDBG;
};























