//
//  Material.h
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//
#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "Shader.h"
#include "GusGLObject.h"


const int MATERIAL_MAX_ATTRIBS = 10;

using namespace std;



struct MaterialAttribs_t {
    
    typedef void (MaterialAttribs_t::*fctPtr)(string);
    
    map<string, fctPtr>                 mapFunction;
    map<MATERIAL_ATTRIBS, bool>         isEnabled;
    string                              name;
    bool                                emit;
    bool                                castShadow;
    bool                                receiveShadow;
    GusVec3                             albedoColor;
    GusVec3                             diffuseColor;
    GusVec3                             specularColor;
    float                               shininess;
    string                              albedoMap;
    string                              diffuseMap;
    string                              specularMap;
    
    MaterialAttribs_t() {
        mapFunction[ "name"          ] = &MaterialAttribs_t::Name;
        mapFunction[ "castShadow"    ] = &MaterialAttribs_t::CastShadow;
        mapFunction[ "receiveShadow" ] = &MaterialAttribs_t::ReceiveShadow;
        mapFunction[ "albedoColor"   ] = &MaterialAttribs_t::AlbedoColor;
        mapFunction[ "diffuseColor"  ] = &MaterialAttribs_t::DiffuseColor;
        mapFunction[ "specularColor" ] = &MaterialAttribs_t::SpecularColor;
        mapFunction[ "shininess"     ] = &MaterialAttribs_t::Shininess;
        mapFunction[ "albedoMap"     ] = &MaterialAttribs_t::AlbedoMap;
        mapFunction[ "diffuseMap"    ] = &MaterialAttribs_t::DiffuseMap;
        mapFunction[ "specularMap"   ] = &MaterialAttribs_t::SpecularMap;
        mapFunction[ "emit"          ] = &MaterialAttribs_t::Emit;
        
        for (int i = 0; i < MATERIAL_MAX_ATTRIBS; i++) {
            isEnabled[(MATERIAL_ATTRIBS)i] = false;
        }
        isEnabled[DIFFUSE] = true;
        diffuseColor = GusVec3(0.0,0.5,0.5);
        isEnabled[ALBEDO] = true;
        albedoColor = GusVec3(0.0,0.1,0.1);
        isEnabled[SPECULAR] = true;
        specularColor = GusVec3(1.0,1.0,1.0);
        shininess = 50.0f;
        albedoMap = "";
        diffuseMap = "";
        specularMap = "";
    }
    void Name(string is) {
        char temp[64];
        sscanf(is.c_str(), "%s",temp);
        name = temp;
    }
    void CastShadow(string is) {
        int i = 0;
        sscanf(is.c_str(), "%d",&i);
        if(i == 1) isEnabled[CAST_SHADOW]=true;
    }
    void ReceiveShadow(string is) {
        int i = 0;
        sscanf(is.c_str(), "%d",&i);
        if(i == 1) isEnabled[RECEIVE_SHADOW]=true;
    }
    void Emit(string is) {
        int i = 0;
        sscanf(is.c_str(), "%d",&i);
        if(i == 1) isEnabled[EMIT]=true;
    }
    void AlbedoColor(string is) {
        sscanf (is.c_str(),"%f %f %f",&albedoColor.x,&albedoColor.y,&albedoColor.z);
        isEnabled[ALBEDO]=true;
    }
    void DiffuseColor(string is) {
        sscanf (is.c_str(),"%f %f %f",&diffuseColor.x,&diffuseColor.y,&diffuseColor.z);
        isEnabled[DIFFUSE]=true;
    }
    void SpecularColor(string is) {
        sscanf (is.c_str(),"%f %f %f",&specularColor.x,&specularColor.y,&specularColor.z);
        isEnabled[SPECULAR] =true;
    }
    void Shininess(string is) {
        sscanf(is.c_str(), "%f",&shininess);
        if(castShadow) isEnabled[CAST_SHADOW]=true;
    }
    void AlbedoMap(string is) {
        char temp[64];
        sscanf (is.c_str(),"%s",temp);
        albedoMap = temp;
        isEnabled[ALBEDO_MAP]=true;
    }
    void DiffuseMap(string is) {
        char temp[64];
        sscanf (is.c_str(),"%s",temp);
        diffuseMap = temp;
        isEnabled[DIFFUSE_MAP]=true;
    }
    void SpecularMap(string is) {
        char temp[64];
        sscanf (is.c_str(),"%s",temp);
        specularMap = temp;
        isEnabled[SPECULAR_MAP] =true;
    }
    

    vector<string> GetAttribsList() {
        vector<string> vec;
        map<string, fctPtr>::iterator it = mapFunction.begin();
        for (; it != mapFunction.end(); ++it) {
            vec.push_back(it->first);
        }
        return vec;
    }
    

    
    void Call( const string & s, string is ) {
    	fctPtr fp = mapFunction[s];
    	return (this->*fp)(is);
    }
};




class Shader;


class Material : public GusGLObject{
        public :
      static const int                       maxAttribs = MATERIAL_MAX_ATTRIBS;
      static map<string, MaterialAttribs_t*> ParseData(ifstream& input_stream);
const static vector<string>                  attribsList;
    
        Material();
        Material(MaterialAttribs_t* in_attribs):attribs(in_attribs){};
    virtual                         ~Material();
    
    virtual void                    _SetAttrib(string attrib_name, string value);
    virtual void                    _SetPostAttrib();
    virtual int                     _GetType() const {return (int)GUS_OBJ_ID::MATERIAL;};

    void                            SetShader(LIGHT_TYPE type, int shader_ref){shaders[type] = shader_ref;};
    const int *                     GetShader()const {return shaders;};
    void                            SetGLUniforms();
    bool                            isEnabled(MATERIAL_ATTRIBS type) const;
    void                            SetGLAttribs(Shader* shader);
    string                          _GetName()const{return attribs->name;};
    void                            CopyAttribs(MaterialAttribs_t* in_attribs){attribs = in_attribs;};
    void                            LoadImage(TEXTURE_TYPE type);
    void                            SetShadowMap(GLuint tex_ref)
                                        { textures[SHADOW_MAP] = tex_ref;};
    
    void                            SetColor(GusVec3 color){attribs->diffuseColor = color;
                                                            attribs->albedoColor = color;};
    
        private :
    static map <int, GLuint*>       GLRef;
    
    int                             shaders[GUS_LIGHT_TYPE];
    GLuint                          textures[GUS_MAX_TEXTURES];
    MaterialAttribs_t*              attribs;
    GLuint                          GLMaterialAttrib[GUS_MATERIAL_ATTRIBS_TYPE];    
};








