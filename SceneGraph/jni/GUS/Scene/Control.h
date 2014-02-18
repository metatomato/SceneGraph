//
//  Control.h
//  helloGL
//
//  Created by tomato on 2/6/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include <cmath>
#include "../Common/GUS.h"
#include "SpaceObject.h"


#ifdef IOS
	const float BOTTOM_LIMIT = 600.f;
#elif defined(ANDROID)
	const float BOTTOM_LIMIT = 1000.f;
#endif

class SceneObject;
class SceneCamera;

class Entity;


enum CONTROL{FORWARD,BACKWARD,STRIFE_L,STRIFE_R,UP,DOWN};
const int CONTROL_MAX = 6;
enum class CONTROL_TYPE{CHARACTER, CAMERA_FREE, CAMERA_FOCUS, AUTO_CONTROL};

class Control {
        public :
static float            scale;
static float            dx,dy,x,y;
static float            localX,localY,localdX,localdY,localdPhi, localdTheta;
static void             GetInput();
static void             ResetValue() { dx = 0.0f; dy = 0.0f; scale = 1.0f; } ;

    Control(SpaceObject& obj);

    void                    InitControlState();
    virtual GusVec3         GetPosition(){return objTarget.GetPosition();};
    virtual GusVec3         GetDiriction(){return objTarget.GetDirection();};

    virtual void            ComputePos();
    virtual void            ComputeDir();
    
    void                    SetControlState(CONTROL dir);

    
        protected :
     GusVec3&               pos(){return objTarget.GetPosition_m();};
     GusVec3&               dir(){return objTarget.GetDirection_m();};
    
    bool                    controlState[CONTROL_MAX];
    SpaceObject&            objTarget;
};



class CharacterControl : public Control {
        public :
                            CharacterControl();
    void                    InitControlState();
    virtual GusVec3         GetPos();
    virtual GusVec3         GetDir();
    virtual void            ComputePos();
    virtual void            ComputeDir();

};


class CameraControl : public Control {
        public :
static  float               GetFocusLength(const SpaceObject& obj);
static SpaceObject&			dummyTarget;

	CameraControl(SceneCamera& obj);

    void                    InitControlState();
    
    virtual GusVec3         GetPosition();
    virtual GusVec3         GetDirection();
    virtual GusVec3         GetUp() { return objTarget.GetUp(); };
    
    virtual void            ComputePos();
    virtual void            ComputeDir();
    virtual void            ComputeInverse();
    virtual void            UpdateControl();
    
    void                    Move(float speed, CONTROL dir);
    void                    FocusOn(const SpaceObject& entity);
    
    
        protected :
    virtual GusVec3&        up() { return objTarget.GetUp_m(); };
    GLfloat                 angle;
};


class CameraFocusControl : public CameraControl {
        public :
    CameraFocusControl(SceneCamera& obj);
    CameraFocusControl(SceneCamera& obj, const SpaceObject&);
    ~CameraFocusControl(){};
    
    virtual GusVec3         GetPosition();
    virtual GusVec3         GetDirection();
    virtual GusVec3         GetUp() ;
    
    virtual void            SetPos(GusVec3 in_pos) { pos() = in_pos; ComputeInverse();};
    
    virtual void            ComputePos();
    virtual void            ComputeDir();
    void                    ComputeUp();
    virtual void            UpdateControl();
    
    virtual void            ComputeInverse();
  
    
    void                    SetFocusOn(const SpaceObject&);
    void 					SetAutoFocusOn(const SpaceObject* obj);
    
        private :
    GusVec3                 focusPoint;
    const SpaceObject*      objectFocused;
    GLfloat                 theta,phi,rho;
};




class CameraFreeControl : public CameraControl {
    public :
    CameraFreeControl(SceneCamera& obj);
    CameraFreeControl(SceneCamera& obj, const SpaceObject&);
    ~CameraFreeControl(){};
    
    virtual GusVec3         GetPosition();
    virtual GusVec3         GetDirection();
    virtual GusVec3         GetUp() ;
    
    virtual void            ComputePos();
    virtual void            ComputeDir();
    void                    ComputeUp();
    
    virtual void            UpdateControl();
    
    virtual void            ComputeInverse();
    
    private :

    GLfloat                 theta,phi;
};




class CameraAutoControl : public CameraControl
{
    public :
    CameraAutoControl(SceneCamera& obj):CameraControl(obj){};
    
    virtual GusVec3         GetPosition() {return pos();};
    virtual GusVec3         GetDirection(){return dir();};
   // virtual GusVec3         GetUp(){return GusVec3(0.f,1.f,0.f);};
    virtual void            ComputeInverse(){};
    
    void         SetPosition(const GusVec3& in_pos)  {pos() = in_pos;};
    void         SetDirection(const GusVec3& in_dir) {dir() = in_dir;};
    void         SetUp(const GusVec3& in_up)		 {up() = in_up;};
};






























































