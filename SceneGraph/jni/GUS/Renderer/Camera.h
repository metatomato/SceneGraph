//
//  Camera.h
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "../Scene/Control.h"
#include "GusGLObject.h"

class Shader;
class Entity;


struct CameraAttribs_t{
    typedef void (CameraAttribs_t::*fctPtr)(string);
    
    string      name;
    GusVec3     position;
    GusVec3     direction;
    GusVec3     up;

    
    map<string, fctPtr> mapFunction;
    
    CameraAttribs_t(){
        mapFunction["name"] = &CameraAttribs_t::Name;
        mapFunction["position"] = &CameraAttribs_t::Position;
              
        name = "default_camera";
        position = GusVec3(0.f,5.f,20.f);
        direction = GusVec3(0.f,5.f,19.f);
        up = GusVec3(0.0,1.0,0.0);
    }
    
    void Name(string is) {
        if(is.compare("") == 0){name = "default_camera";}
        else{ name = is;}
    }
    void Position(string is) {
        sscanf (is.c_str(),"%f %f %f",&position.x,&position.y,&position.z);
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



class Camera : public Scenable {
    
        public :
const static  vector <string>           attribsList;
const static GUS_OBJ_ID                 type = GUS_OBJ_ID::CAMERA;
static  void                            SetCurrentCamera(Camera* cam){currentCamera = cam;};
static  Camera*                         GetCurrentCamera()
                                            {return currentCamera;};
static  GusVec3                         GetCameraPosition()
                                            {return currentCamera->GetPosition();};
static  GusVec3                         GetCameraTargetPosition()
                                            {return currentCamera->GetDirection();};
static  GusVec3                         GetCameraUp()
                                            {return currentCamera->GetUp();};
        private :
static Camera*                      currentCamera;
    
        public :
    Camera();
    Camera(string name);
    ~Camera();

    virtual void                    _SetAttrib(string attrib_name, string value);
    virtual void                    _SetPostAttrib();
    virtual void                    _SetAttribFromChild(GusGLObject* child);
    virtual int                     _GetType() const {return (int)GUS_OBJ_ID::CAMERA;};
    virtual GusVec3*                _GetVecPosition(){return &attribs.position;};
    virtual string                  _GetName()const{return attribs.name;};
    
    virtual void                    Sync();

    void                            SetCamera();
    void                            SetGLAttribs(Shader& shader);

    GusVec3                         GetPosition() const {return attribs.position;};
    GusVec3                         GetPosition() {return attribs.position;};
    
    GusVec3                         GetDirection() const {return attribs.direction;};
    void                            SetDirection(GusVec3 dir) {attribs.direction = dir;};
    
    GusVec3                         GetUp() const {return attribs.up;};
    void                            SetUp(GusVec3 in_up) {attribs.up = in_up;};

    
    GLfloat*                        GetViewMatrix(){return VIEW;};
    void                            FocusOn(SpaceObject* entity);
    
    
        private :
    CameraAttribs_t                     attribs;
    GLfloat                             VIEW[16];
};

























