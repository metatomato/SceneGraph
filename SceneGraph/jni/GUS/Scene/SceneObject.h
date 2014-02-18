//
//  SceneObject.h
//  helloGL
//
//  Created by tomato on 9/17/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include "../Common/GUS.h"
#include "SpaceObject.h"
#include "../Renderer/Mesh.h"
#include "Control.h"

class Entity;
class Camera;
class Light;

class SceneObject : public GusObject {
        public :
    enum class GUS_OBJ_ID {BASE,STAGE,SCENE_BOX};
    SceneObject();
    ~SceneObject();
    virtual int                 _GetType() const {return (int)GUS_OBJ_ID::BASE;};
        private :
    
};



class StageObject : public SceneObject, public SpaceObject {
        public :
    enum class STAGE_OBJ_ID{ENTITY,LIGHT, CAMERA, SCENE_BOX};
    StageObject();
    StageObject(const GusVec3& pos, const GusVec3& halfvec);
    StageObject(const GusVec3& pos, const GusVec3& halfvec, const GusVec3& dir);
    StageObject(const vector<const StageObject*>&);
    StageObject(const vector<const Scenable*>&);

    
    virtual string              _GetName() const{return name;};
    virtual int                 _GetType() const {return (int)GUS_OBJ_ID::STAGE;};
    
    void                        InitBox(const vector<const SpaceObject*>&);
    void                        InitBox(const vector<const Box_t*>& vec_group);
    void                        InitGroup(const vector<const GusObject*>&);

    virtual void                Update(){};
    
    vector<int>                 GetGroup()  			{return group;};
    vector<int>                 GetSubGroup()			{return activeSubGroup;};
    void						AddToSubGroup(int idx)	{activeSubGroup.push_back(idx);}
    void						ClearSubGroup()			{activeSubGroup.clear();}
    bool                        IsVisible()				{return visible;}
    void                        SetVisible(bool state)	{visible = state;}
    bool                        IsSelected()			{return selected;}
	void                        SetSelected(bool state)	{selected = state;}

    
        protected :
    vector<int>                         group;
    vector<int>							activeSubGroup;
    string                              name;
    bool                                visible;
    bool                                selected;
    
};



class PhysicalObject : public StageObject {
    public :
    PhysicalObject();
    PhysicalObject(const vector<const Scenable*>&);
    PhysicalObject(const Scenable&);
    
    virtual void                Update(){};
    
    float                   GetMass() const {return mass;};
    
    private :
    GusVec3                 velocity;
    float                   mass;
};



class SceneBox : public StageObject {
        public :
    SceneBox(const vector<const StageObject*>&);
    ~SceneBox(){}
    virtual int                 _GetType() const {return (int)STAGE_OBJ_ID::SCENE_BOX;};
    virtual void                Update(){};
    
    const GLfloat*					GetMODEL() const;

};



class SceneEntity : public PhysicalObject {
        public :
    SceneEntity(const vector<const StageObject*>&);
    SceneEntity(const vector<const Scenable*>&);
    SceneEntity(const Entity& obj);
    virtual int                 _GetType() const {return (int)STAGE_OBJ_ID::ENTITY;};
    virtual void                Update(){};
    
};



class SceneLight : public StageObject {
        public :
    SceneLight(Light& obj);
    virtual int                 _GetType() const {return (int)STAGE_OBJ_ID::LIGHT;};
    virtual void                Update(){};
    GLfloat						GetEnergy()				{return lightRef.GetEnergy();}
    void						SetEnergy(float value)	{lightRef.SetEnergy(value);}
    int							GetType()				{return lightRef.GetType();}
    void						SetType(int value)		{lightRef.SetType(value); LOGE("Light %s changed to %d",_GetName().c_str(),value); }
    
        private :
    Light&						lightRef;

};



class SceneCamera : public StageObject {
        public :
    SceneCamera(const Camera& obj);
    
    virtual int                 _GetType() const {return (int)STAGE_OBJ_ID::CAMERA;};
    virtual void                Update();
    virtual void                BrainOrder(const State&);
    virtual void                BrainOrder(const State&, CONTROL_TYPE type);
    
    void                        SwitchCameraMode(CONTROL_TYPE);
    void 						LookAt(GusVec3 eye, GusVec3 target, GusVec3 up);
    void 						CameraSkyMode();
    void                        FocusOn(const SpaceObject&);
    void 						AutoFocusOn(const SpaceObject* obj);
    void                        OnTV(const Program& prog);
    void 						ResetPosition();
    void						isInversed(bool state) {inversed = state;}
    

        private :

    map<CONTROL_TYPE,CameraControl*>    control;
    
    CONTROL_TYPE                        currentType;
    
    CameraControl                       cameraControl;
    CameraFocusControl                  cameraFocusControl;
    CameraFreeControl                   cameraFreeControl;
    CameraAutoControl                   cameraAutoControl;

    GusVec3								originalPosition;
    bool								inversed;
};



































































































