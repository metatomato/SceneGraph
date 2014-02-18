//
//  Light.h
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "GusGLObject.h"

class Shader;

struct LightAttribs_t{
    typedef void (LightAttribs_t::*fctPtr)(string);
    
    string      name;
    GusVec3     position;
    GusVec3     direction;
    GusVec3     color;
    float		energy;
    LIGHT_TYPE  type;
    float		radius;
    float		att_distance;
    float		cut_off;
    
    
    map<string, fctPtr> mapFunction;
    
    LightAttribs_t(){
        mapFunction["name"] = &LightAttribs_t::Name;
        mapFunction["position"] = &LightAttribs_t::Position;
        mapFunction["direction"] = &LightAttribs_t::Direction;
        mapFunction["type"] = &LightAttribs_t::Type;
        
        name      = "default_light";
        position  = GusVec3(10.0f,1000.0f,0.0f);
        direction = GusVec3(100.0f,10000.0f,0.0f);
        color = GusVec3(1.0f,1.0f,1.0f);
        type      = DIRECTIONAL;
        energy 	  = 1.f;
        radius 	  = 64.f;
        att_distance  = 15.f;
        cut_off   = 50.f;
    }
    
    void Name(string is) {
        char temp[64];
        sscanf(is.c_str(), "%s",temp);
        name = temp;
    }
    void Position(string is) {
        sscanf (is.c_str(),"%f %f %f",&position.x,&position.y,&position.z);
    }
    void Direction(string is) {
        sscanf (is.c_str(),"%f %f %f",&direction.x,&direction.y,&direction.z);
    }
    void Type(string is) {
        if( is == "DIRECTIONAL" ) type = DIRECTIONAL;
        if( is == "POINT" ) type = POINT;
        if( is == "SPOT" ) type = SPOT;
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



class Light : public Scenable {
    
        public :
    Light();
    Light(string name, int GL_idx);
    ~Light();
    const static vector <string>   	attribsList;
    static void                     SetDefaultLight(Light* light){defaultLight = light;};
    static Light*                   GetDefaultLight(){return defaultLight;};

    virtual void                   _SetAttrib(string attrib_name, string value);
    virtual void                   _SetPostAttrib();
    virtual void                   _SetAttribFromChild(GusGLObject* child);
    virtual int                    _GetType() const {return (int)GUS_OBJ_ID::LIGHT;};

    GusVec3                        GetPosition_v() const {return attribs.position;};
    const LIGHT_TYPE               GetLightType() const {return attribs.type;};
    void                           SetGLAttribs(Shader& shader);
    string                         _GetName()const {return attribs.name;};
    LightAttribs_t                 GetAttribs() {
    									return attribs;
    							    }
    void                           SetPosition(GLfloat x,GLfloat y,GLfloat z){
    									attribs.position.x = x;
    									attribs.position.y = y;
    									attribs.position.z = z;
    								}
    void                           SetRelativePosition(GLfloat x,GLfloat y,GLfloat z) {
    									attribs.position.x += x;
    									attribs.position.y += y;
    									attribs.position.z += z;
    								}
    
    GusVec3		                   GetDirection() const {return attribs.direction;} ;
    GusVec3&	                   GetDirection_m()  {return attribs.direction;}
    const GLfloat*                 GetDirection()  {return attribs.direction.value();} ;
    GusVec3		                   GetPosition() {return attribs.position;}
    GusVec3&	                   GetPosition_m()  {return attribs.position;}
    GLfloat						   GetEnergy(){return attribs.energy;}
    void						   SetEnergy(float value){attribs.energy = value;}
    int							   GetType(){return (int) attribs.type;}
    void						   SetType(int value){attribs.type = (LIGHT_TYPE) value;}
    void						   SetGLId(int idx){GLId = idx;}
    int							   GetGLId(){return GLId;}

    
        virtual void                    Sync();
    
        private :
    static Light*                  defaultLight;
    LightAttribs_t                 attribs;
    GLfloat*  					   GLPackedAttrib;
    int							   GLId;

};


















