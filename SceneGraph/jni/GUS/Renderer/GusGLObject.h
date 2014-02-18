//
//  GusGLObject.h
//  helloGL
//
//  Created by tomato on 1/25/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>

#include "../Common/GUS.h"



using namespace std;

class GusVec3;
class StageObject;

class GusGLObject : public GusObject {
    
        public :
    enum class GUS_OBJ_ID {BASE, ENTITY, MATERIAL, MESH, CAMERA, LIGHT, ENTITY_ANIM , MD5 };
    static const int            GusGLObjectType = 5;
    static const GUS_OBJ_ID     GetObjType(GusGLObject * obj);
    
    GusGLObject();
    virtual ~GusGLObject();

    virtual int                 _GetType() const {cout << "BASE GETTYPE CALL"<< endl; return (int)GUS_OBJ_ID::BASE;};
    virtual void                _SetAttrib(string attrib_name, string value){};
    virtual void                _SetPostAttrib(){};
    virtual void                _SetAttribFromChild(GusGLObject* child){};
    
    
        protected :
    
};


class Scenable : public GusGLObject {
        public :
    Scenable();
    virtual ~Scenable(){};
    
    virtual void            Sync(){};

    void                    SetStageRef(const StageObject& ref_ptr) const{stageObjRef = &ref_ptr;};
    const StageObject*      GetStageRef(){return stageObjRef;};
    void					ResetStageRef(){stageObjRef = nullptr;};
    
        protected :
    mutable const StageObject*                     stageObjRef;
};
