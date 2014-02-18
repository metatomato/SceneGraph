//
//  SceneObject.cpp
//  helloGL
//
//  Created by tomato on 9/17/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "SceneObject.h"
#include "../Renderer/Entity.h"
#include "../System/Program.h"
#include "../Logic/State.h"
#include "../Common/Time.h"
#include "Scene.h"

SceneObject::SceneObject(){
   
}


SceneObject::~SceneObject(){

}




////////////////////////////////////////////////
//      STAGEOBJECT METHOD DEFINITIONS
////////////////////////////////////////////////

StageObject::StageObject() : group(),name(),visible(true), selected(false)
{
    
}



StageObject::StageObject(const GusVec3& pos, const GusVec3& halfvec) : SpaceObject(pos,halfvec),
    group(),
    visible(true),
    selected(false)
{
    
}



StageObject::StageObject(const GusVec3& pos, const GusVec3& halfvec, const GusVec3& dir) : SpaceObject(pos,halfvec,dir),
    group(),
    visible(true),
    selected(false)
{
    
}



StageObject::StageObject(const vector<const StageObject*>& vec_group):
    name(vec_group.front()->_GetName()),
    visible(true),
    selected(false)
{
	vector<const SpaceObject*> vec_space;

	for (auto obj : vec_group) {
	       // LOGE("[STAGEOBJECT][StageObject] position %s", obj->GetBox().position.Print().c_str());
	       // LOGE("[STAGEOBJECT][StageObject] halfvec %s", obj->GetBox().halfVec.Print().c_str());
	        vec_space.push_back(dynamic_cast<const SpaceObject*>(obj));
	    }

	for (auto obj : vec_space) {
		     //   LOGE("[STAGEOBJECT][StageObject] vec_space position %s", obj->GetBox().position.Print().c_str());
		     //   LOGE("[STAGEOBJECT][StageObject] vec_space halfvec %s", obj->GetBox().halfVec.Print().c_str());
	}

   // InitBox(Translate<StageObject,SpaceObject>(vec_group));
	 InitBox(vec_space);
    InitGroup(Translate<StageObject,GusObject>(vec_group));
}



StageObject::StageObject(const vector<const Scenable*>& vec_group):
    visible(true),
	selected(false)
{
    InitGroup(Translate<Scenable,GusObject>(vec_group));
    
    for (auto obj : vec_group) {
        obj->SetStageRef(*this);
    }
}



void StageObject::InitBox(const vector<const Box_t*>& vec_group)
{
    Box_t temp_box(*vec_group.front());

    for (auto box : vec_group)
    {
        temp_box += *box;
    }

    //LOGE("[STAGEOBJECT][InitBox] final pos %s ",temp_box.position.Print().c_str());
    //LOGE("[STAGEOBJECT][InitBox] final hv  %s ",temp_box.halfVec.Print().c_str());
    SetBox(temp_box);

}



void StageObject::InitBox(const vector<const SpaceObject*>& vec_group)
{
    vector<const Box_t*> vec_box;

    //LOGE("[STAGEOBJECT][InitBox] SpaceObj vec size %d",vec_group.size());

    for (auto obj : vec_group) {
        vec_box.push_back(&(obj->GetBox()));
      //  LOGE("[STAGEOBJECT][InitBox] position %s", obj->GetBox().position.Print().c_str());
      //  LOGE("[STAGEOBJECT][InitBox] halfvec %s", obj->GetBox().halfVec.Print().c_str());
    }

    InitBox(vec_box);

}



void StageObject::InitGroup(const vector<const GusObject*>& vec_group){
    for (auto vec : vec_group)
    {
        group.push_back(vec->_GetId());
    }
}




////////////////////////////////////////////////
//      SCENEBOX METHOD DEFINITIONS
////////////////////////////////////////////////

SceneBox::SceneBox(const vector<const StageObject*>& vec_group) : StageObject(vec_group)
{
    cout << "[SCENEBOX][SceneBox] name " << name << endl <<
    "halfvec  " << GetHalfVec().x << " " << GetHalfVec().y << " " << GetHalfVec().z << endl <<
    "position " << GetPosition().x << " " << GetPosition().y << " " << GetPosition().z << endl;
}



////////////////////////////////////////////////
//      PHYSICALOBJECT METHOD DEFINITIONS
////////////////////////////////////////////////

PhysicalObject::PhysicalObject():mass(0.0f),velocity()
{
    
}


PhysicalObject::PhysicalObject(const vector<const Scenable*>& vec_group) : StageObject(vec_group),
    velocity(),
    mass(0.0f)
{
    vector<const Box_t*> box_vec;
    for (auto obj : vec_group) {
        box_vec.push_back(&((Entity*)obj)->GetMesh()->GetBox());
    }
    InitBox(box_vec);
}


PhysicalObject::PhysicalObject(const Scenable& obj) :
    mass(0.0f),
    velocity()
{
    const Box_t* box = &((Entity&)obj).GetMesh()->GetBox();
    vector<const Box_t*> vec_box(1,box);
    InitBox(vec_box);
    group.push_back(obj._GetId());
    obj.SetStageRef(*this);
}


////////////////////////////////////////////////
//      SCENE ENTITY METHOD DEFINITIONS
////////////////////////////////////////////////

SceneEntity::SceneEntity(const vector<const Scenable*>& vec) : PhysicalObject(vec)
{
    name = ((Entity*)vec.front())->GetParent();
}



SceneEntity::SceneEntity(const Entity& obj) : PhysicalObject(obj)
{
    name = obj.GetParent();
}




////////////////////////////////////////////////
//      SCENE LIGHT METHOD DEFINITIONS
////////////////////////////////////////////////

SceneLight::SceneLight(Light& obj) : StageObject(obj.GetPosition_v(),
										   GusVec3(1.0f,1.0f,1.0f)),
										   lightRef(obj)

{
    name = obj._GetName();
    group.push_back(obj._GetId());
    SetDirection(obj.GetDirection_m());
    SetPosition(obj.GetPosition_m());
    obj.SetStageRef(*this);
}





////////////////////////////////////////////////
//      SCENE CAMERA METHOD DEFINITIONS
////////////////////////////////////////////////


SceneCamera::SceneCamera(const Camera& obj) : StageObject(obj.GetPosition(),GusVec3(1.0f,1.0f,1.0f),obj.GetDirection()),
    control(),
    currentType(CONTROL_TYPE::CHARACTER),
    cameraControl(*this),
    cameraFocusControl(*this),
    cameraFreeControl(*this),
    cameraAutoControl(*this),
    originalPosition(obj.GetPosition()),
	inversed(false)
{
    name = obj._GetName();
    SetDirection(obj.GetDirection());
    group.push_back(obj._GetId());
    
    control[CONTROL_TYPE::CHARACTER]    = &cameraControl;
    control[CONTROL_TYPE::CAMERA_FOCUS] = &cameraFocusControl;
    control[CONTROL_TYPE::CAMERA_FREE]  = &cameraFreeControl;
    control[CONTROL_TYPE::CAMERA_FREE]  = &cameraFreeControl;
    control[CONTROL_TYPE::AUTO_CONTROL]  = &cameraAutoControl;
    
    
    obj.SetStageRef(*this);
}



void SceneCamera::Update(){

    if(inversed) {
    	SetUp(control[currentType]->GetUp() * -1);
    	//SetPosition(control[currentType]->GetPosition() * -1);
		//SetDirection(control[currentType]->GetDirection() *-1 );
    } else {
    	SetUp(control[currentType]->GetUp());
    	SetPosition(control[currentType]->GetPosition());
		SetDirection(control[currentType]->GetDirection());
    }

    
 //   cout << "[SCENECAMERA][Update] controltype " << (int)currentType << endl;
}



void SceneCamera::SwitchCameraMode(CONTROL_TYPE type)
{
    currentType = type;
    
    if (type == CONTROL_TYPE::CAMERA_FOCUS)
    {
        FocusOn((StageObject&)Scene::GetSelection());
    }
    
    control[type]->ComputeInverse();
}



void SceneCamera::LookAt(GusVec3 eye, GusVec3 target, GusVec3 up) {
	currentType = CONTROL_TYPE::AUTO_CONTROL;
	((CameraAutoControl*)control[currentType])->SetPosition(eye);
	((CameraAutoControl*) control[currentType])->SetDirection(target);
	((CameraAutoControl*) control[currentType])->SetUp(up);
}



void SceneCamera::CameraSkyMode(){

	StageObject& so = (StageObject&)(Scene::GetSelection());
	if(&so){
		//LOGE("[SCENEOBJECT][CameraSkyMode] POSITION %f %f %f", so.GetPosition().x, so.GetPosition().y,so.GetPosition().z);
		float y = cameraControl.GetFocusLength(so);
		GusVec3 pos = so.GetPosition();
		LookAt(pos + GusVec3(0.f,y,0.f), pos, GusVec3(0.f,0.f,-1.f));
		//LOGE("[SCENEOBJECT][CameraSkyMode] LOOKING AT OBJECT %s", ((SceneObject&)so)._GetName().c_str());

	}else {
		StageObject* so_default = (StageObject*)(Scene::GetSceneBox());
		float y = cameraControl.GetFocusLength(*so_default);
		GusVec3 pos = so_default->GetPosition();
		LookAt(pos + GusVec3(0.f,y,0.f), pos, GusVec3(0.f,0.f,-1.f));
		//LOGE("[SCENEOBJECT][CameraSkyMode] LOOKING AT DEFAULT");
	}
}


void SceneCamera::FocusOn(const SpaceObject& obj){
    
    //cameraControl.FocusOn(obj);

    cameraFocusControl.SetFocusOn(obj);
    currentType = CONTROL_TYPE::CAMERA_FOCUS;
}


void SceneCamera::AutoFocusOn(const SpaceObject* obj){

	ResetPosition();
    cameraFocusControl.SetAutoFocusOn(obj);
    currentType = CONTROL_TYPE::CAMERA_FOCUS;
}


void SceneCamera::OnTV(const Program& prog)
{
    if(prog.GetType() == (int)Program::TYPE::DT_MOTION)
    {
       // DoubleTapMotion* e = (DoubleTapMotion*)&prog;
      //  cout << "[SCENECAMERA][OnTV] CAMERA GET NEW INPUT!  X " << e->GetX() << " Y " << e->GetY() << endl;
       // FocusOn(Scene::GetSelection());
    }
    if(prog.GetType() == (int)Program::TYPE::TIMER)
    {
     //   cout << "[SCENECAMERA][OnTV] CAMERA GET A TICK!" << Time::GetTimeFromRef(Time::GetTime()) <<endl;
    }
}




void SceneCamera::BrainOrder(const State& state, CONTROL_TYPE type)
{
    if (state.IsNew())
    {
        SwitchCameraMode(type);
    }
    if (state.GetStateType() == (int)State::TYPE::SPACE_LOC ) {
        
        SpaceLocation* space_loc = (SpaceLocation*)&state;
        
        ((CameraAutoControl*)control[currentType])->SetPosition(space_loc->GetPos());
        ((CameraAutoControl*) control[currentType])->SetDirection(space_loc->GetDir());
    }

}


void SceneCamera::BrainOrder(const State& state)
{
    if (state.GetStateType() == (int)State::TYPE::PAUSE )
    {
        if (state.IsNew())
        {
            SwitchCameraMode(CONTROL_TYPE::CHARACTER);
        }
    }
    
    if (state.GetStateType() == (int)State::TYPE::MOTION )
    {
        if (state.IsNew())
        {
            SwitchCameraMode(CONTROL_TYPE::CHARACTER);
        }
        
        Motion* motion = (Motion*)&state;
        
        cameraControl.Move(motion->GetSpeed(), motion->GetDir());
    }
    
     if (state.GetStateType() == (int)State::TYPE::SPACE_LOC ) {
         
         SpaceLocation* space_loc = (SpaceLocation*)&state;
         
         if (state.IsNew())
         {
             SwitchCameraMode(CONTROL_TYPE::AUTO_CONTROL);
         }
  
            ((CameraAutoControl*)control[currentType])->SetPosition(space_loc->GetPos());
            ((CameraAutoControl*) control[currentType])->SetDirection(space_loc->GetDir());
     }
}




void SceneCamera::ResetPosition(){

	SetPosition(originalPosition);
}





























