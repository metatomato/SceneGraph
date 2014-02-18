//
//  Scene.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//



#include "Scene.h"
#include <fstream>
#include <sstream>
#include "../System/Program.h"
#include "../System/GusTV.h"
#include "../Logic/Cortex.h"


//float Scene::elapsedTime = 0.0f ;
const SceneObject* Scene::selectedObject = nullptr;
const SceneObject* Scene::lastSelectedObject = nullptr;
map<int,const SceneObject*> Scene::selections;
SceneBox* Scene::sceneBox = nullptr;
vector<int> Scene::selectionHistory;


vector<int> Scene::GetSelections() {
	vector<int> results;
	for(auto pair : selections) {
		results.push_back(pair.first);
	}
	return results;
}


Scene::Scene():
		selectionBox(nullptr),
		sceneData(nullptr),
		defaultCamera(nullptr),
		skyCamera(nullptr),
		aabbMode(false),
		skyViewMode(false),
		sceneMode(true),
		lockMode(false),
		subObjectMode(false),
		matMode(false),
		entityMode(false),
		dataCentral(nullptr),
		matModePrimitive(0),
		matRef(-1),
		meshMode(false)
{
    
}

Scene::~Scene() {
    
}

void Scene::Init(SceneData* data)
{
    sceneData = data;
    defaultCamera = (SceneCamera*)(*sceneData)[StageObject::STAGE_OBJ_ID::CAMERA]->GetEntry("default_camera");

    dataCentral->CreateCamera(RENDER,"sky_camera");

    skyCamera = (*sceneData)[StageObject::STAGE_OBJ_ID::CAMERA]->GetEntry("sky_camera");
    skyCamera->LookAt(GusVec3(0.f,300.f,0.f),GusVec3(0.f,0.f,0.f),GusVec3(0.f,0.f,-1.f));

    InitStage((*sceneData)[TYPE::ENTITY]->GetVector());

    Cortex::MakeConnection(*defaultCamera);
    
    GusTV::InitScene(*this);
    GusTV::RegisterAdmin(ChannelOffice::CHANNEL::TIME, ChannelOffice::ADMINISTRATION::SCENE);
    GusTV::RegisterAdmin(ChannelOffice::CHANNEL::MOTION, ChannelOffice::ADMINISTRATION::SCENE);

    vector<SceneLight*> vec = GetLights();
    for(auto l : vec) {
    	LOGE("[SCENE][Init] Scene contains %s",l->_GetName().c_str());
    }
}



void Scene::InitStage(vector<SceneObject*> vec)
{
	LOGE("[SCENE][InitStage] STAGE INIT!");

    for (auto obj : vec) {
        objOnStage.push_back(obj);
        ((StageObject*)obj)->Print(obj->_GetName());
    }
    SetPhysics(vec);
    
    GusTV::Subscrib(ChannelOffice::CHANNEL::MOTION,*defaultCamera);
    GusTV::Subscrib(ChannelOffice::CHANNEL::TIME,*defaultCamera);
}



void Scene::LoadStage(vector<int> idx) {

	ResetSelection();
	SetMode(SCENE);

	for(auto obj : objOnStage){
			((StageObject*)obj)->SetVisible(false);
			RemovePhysics(obj);
		}

	vector<const StageObject*> vec;

	for(auto el : idx){
		StageObject* obj = (StageObject*)(*sceneData)[StageObject::STAGE_OBJ_ID::ENTITY]->GetEntry(el);
		obj->SetVisible(true);
		RegainPhysics(obj);
		vec.push_back(dynamic_cast<const StageObject*>(obj));
	}

	NewSelectionBox(vec);
	sceneBox = new SceneBox(*selectionBox);


	LOGE("[SCENE][LoadStage] selection %s", selectionBox->_GetName().c_str());
	defaultCamera->AutoFocusOn(selectionBox);
}



void Scene::UpdateScene(float time)
{
    Control::GetInput();
    defaultCamera->Update();
    
    UpdatePhysicalWorld(time);
    UpdateNonPhysicals();
    
    passToRender.clear();
    objToRender.clear();
    

    for (auto obj : objOnStage) {
        if (((StageObject*)obj)->IsVisible()) {
            objToRender.push_back(obj);
        }
    }
    

    RenderPass<SceneObject> pass_color(ObjToPass());
    pass_color.SetName("STAGE PASS");
    if(subObjectMode){
    	if(matMode){
    		pass_color.SetSType(S_PRIMITIVE);
    		pass_color.SetPrimitive(matModePrimitive);
    		pass_color.SetMaterial(matRef);
    		//matRef = -1;
    	}else {
    		pass_color.SetSType(S_SUB_OBJ);
    		if(meshMode) {
				pass_color.SetRType(GDEBUG);
    		}
    	}
    }
    else {
    	pass_color.SetSType(S_OBJ);
    	if(meshMode) {
			pass_color.SetRType(GDEBUG);
		}
    }
    passToRender.push_back(pass_color);

    
    RenderPass<SceneObject> pass_shadow(ObjToPass(SHADOW));
    pass_shadow.SetName("SHADOW PASS");
    passToRender.push_back(pass_shadow);


    if (aabbMode) {
        RenderPass<SceneObject> pass_box(ObjToPass());
        pass_box.SetSType(S_BOX);
        pass_box.SetRType(COLOR);
        pass_box.SetName("BOX PASS");
        passToRender.push_back(pass_box);
    }
    
    if(selections.size() > 0){
       //RenderPass<SceneObject> pass(ObjToPass((SceneObject&)*selectedObject));
    	RenderPass<SceneObject> pass_select(ObjToPass((SceneObject&)*selectionBox));
        pass_select.SetSType(S_BOX);
        pass_select.SetRType(GDEBUG);
        pass_select.SetRDBGType(BOX_MODE);
        pass_select.SetName("SELCTION PASS");
        passToRender.push_back(pass_select);


		RenderPass<SceneObject> pass_stencil(GetSelected(),(*sceneData)[StageObject::STAGE_OBJ_ID::LIGHT]->GetVector(),defaultCamera,STENCIL);
		pass_stencil.SetName("SELCTION PASS STENCIL");
		passToRender.push_back(pass_stencil);
    }

    if(skyViewMode) {
    	skyCamera->CameraSkyMode();
    	RenderPass<SceneObject> pass_sky(objToRender,(*sceneData)[StageObject::STAGE_OBJ_ID::LIGHT]->GetVector(),skyCamera,RENDER_PASS::SCISSOR);
    	pass_sky.SetName("SKY PASS");
    	passToRender.push_back(pass_sky);
    }
}


void Scene::SetMode(MODE mode) {
	if(SCENE == mode){
		sceneMode 		= true;
		objectMode 		= false;
		subObjectMode   = false;
	}
	if(STAGE == mode){
		sceneMode		= false;
		objectMode 		= true;
		subObjectMode	= false;
	}
	if(SUB_OBJ == mode){
		sceneMode 		= false;
		objectMode 		= false;
		subObjectMode	= true;
	}
}

void Scene::SetSubMode(MODE mode){
	if(MAT == mode){
			matMode 	= true;
			entityMode 	= false;
	}
	if(ENTITY == mode){
			matMode 	= false;
			entityMode 	= true;
	}
}



void Scene::MatModePure(int mat_idx, int primitive){
	matModePrimitive = primitive;
	matRef = mat_idx;
	HideAll();
	SetMode(SUB_OBJ);
	SetSubMode(MAT);
	SpaceObject* dummy = nullptr;
	defaultCamera->AutoFocusOn(dummy);
}



void Scene::MatMode(int obj_idx, int sub_obj_idx, int primitive){
	matModePrimitive = primitive;
	OneSubObjMode(obj_idx,sub_obj_idx);
	SetSubMode(MAT);
}



void Scene::SetMatModePrimitive(int primitive){
	matModePrimitive = primitive;
}



void Scene::EntityMode(int obj_idx, int sub_obj_idx){
	OneSubObjMode(obj_idx,sub_obj_idx);
	SetSubMode(ENTITY);
}



void Scene::MeshMode(bool state){
	meshMode = state;
}



void Scene::AABBMode(bool state){
	aabbMode = state;
}



void Scene::OneSubObjMode(int obj_idx, int sub_obj_idx) {
	StageObject* obj = (*sceneData)[StageObject::STAGE_OBJ_ID::ENTITY]->GetEntry(obj_idx);
	if(obj){
		SetMode(SUB_OBJ);
		obj->ClearSubGroup();
		obj->AddToSubGroup(sub_obj_idx);
		HideAllExcept(*obj);
		selectedObject = obj;
		defaultCamera->AutoFocusOn(obj);
	}else LOGE("[SCENE][OneSubObjMode] Wrong idx input");
}


void Scene::OneObjMode(int obj_idx) {
	StageObject* obj = (*sceneData)[StageObject::STAGE_OBJ_ID::ENTITY]->GetEntry(obj_idx);
	if(obj){
		OneObjMode(*obj);
	}else LOGE("[SCENE][OneObjMode] Wrong idx input");
}



void Scene::OneObjMode(const StageObject& in_obj){
	SetMode(STAGE);
	HideAllExcept(in_obj);
	selectedObject = &in_obj;
	defaultCamera->AutoFocusOn(&in_obj);
}


void Scene::LockMode(bool state) {
	lockMode = state;
	if(state) {
		if(selectionBox){
			defaultCamera->AutoFocusOn(selectionBox);
			LOGE("[SCENE][LockMode] LOCK ON %s", selectionBox->_GetName().c_str());
		} else {
			LOGE("[SCENE][LockMode] We live only twice! %d", selectionBox);
			defaultCamera->AutoFocusOn(selectionBox);
		}
	}else {
		defaultCamera->SwitchCameraMode(CONTROL_TYPE::CHARACTER);
		LOGE("[SCENE][LockMode] DEFAULT MODE : CHARACTER");
	}
}



RenderPass<SceneObject> Scene::ObjToPass(){
    return ObjToPass(COLOR);
}



RenderPass<SceneObject> Scene::ObjToPass(SceneObject& obj){
    return ObjToPass(COLOR, obj);
}



RenderPass<SceneObject> Scene::ObjToPass(RENDER_PASS r_type, SceneObject& obj){
    vector<SceneObject*> vec(1,&obj);
    return RenderPass<SceneObject>(vec,(*sceneData)[StageObject::STAGE_OBJ_ID::LIGHT]->GetVector(),defaultCamera,r_type);
}



RenderPass<SceneObject> Scene::ObjToPass(RENDER_PASS r_type){

	return RenderPass<SceneObject>(objToRender,(*sceneData)[StageObject::STAGE_OBJ_ID::LIGHT]->GetVector(),defaultCamera,r_type);
}



void Scene::OnTV(const Program* prog) const
{
    if(prog->GetType() == (int)Program::TYPE::TIMER){
    	//GusVec3 pos(defaultCamera->GetPosition());
    	//LOGE("[SCENE][OnTV] watching TIMER : cameraPos at %f %f %f",pos.x,pos.y,pos.z);
    }
    if(prog->GetType() == (int)Program::TYPE::ST_MOTION){
    	const SimpleTapMotion& tap = *((SimpleTapMotion*)prog);
    	float x,y;
    	x = tap.GetX();
    	y = tap.GetY();
    	RayCast(x,y);
    	LOGE("[SCENE][OnTV] watching ST at %f %f",x,y);
    }
}



void Scene::SubcribChannel(ChannelOffice::CHANNEL)
{
    GusTV::InitScene(*this);
    GusTV::RegisterAdmin(ChannelOffice::CHANNEL::DBG, ChannelOffice::ADMINISTRATION::SCENE);
}


void Scene::SetPhysics(vector<SceneObject*>& vec){
    for (auto obj : vec) {
        PhysicalObject* phys_obj = dynamic_cast<PhysicalObject*>(obj);
        if (phys_obj) {
            physicalWorld.AddBox(phys_obj);
        }
    }
}



void Scene::RemovePhysics(vector<SceneObject*>& vec) {
    for (auto obj : vec) {
        RemovePhysics(obj);
    }
}



void Scene::RemovePhysics(SceneObject* obj) {
	PhysicalObject* phys_obj = dynamic_cast<PhysicalObject*>(obj);
	        if (phys_obj) {
	            physicalWorld.RemoveBox(phys_obj);
	           // LOGE("[SCENE][RemovePhysics] Object remove from PhysicalsWorls %s",phys_obj->_GetName().c_str());
	        }
}



void Scene::RegainPhysics(SceneObject* obj) {
	PhysicalObject* phys_obj = dynamic_cast<PhysicalObject*>(obj);
		        if (phys_obj) {
		            physicalWorld.ReAddBox(phys_obj);
		           // LOGE("[SCENE][RemovePhysics] Object remove from PhysicalsWorls %s",phys_obj->_GetName().c_str());
		        }
}

void Scene::RayCast(float x, float y){

    btVector3 ray = GusPhysicsTools::getRayTo(x, y);

    GusVec3 pos    = Camera::GetCameraPosition();


    StageObject* selection = physicalWorld.RayCast(btVector3(pos.x,pos.y,pos.z), ray);

   // selectedObject = physicalWorld.RayCast(btVector3(pos.x,pos.y,pos.z), ray);

    if(selection)
    {
    	SetSelected(*selection);

    	//selectedObject = selection;

    //	LOGE("[SCENE][RayCast] Entity %s hitted from %f %f %f to %f %f %f",selectedObject->_GetName().c_str(),pos.x,pos.y,pos.z,ray.x(),ray.y(),ray.z());

   //     Program* prog = ProgramFactory<Program>::CreateProgram(Program::TYPE::SELECTION,vector<ProgramArguments>({1,selectedObject}),Time::GetTime(),chrono::milliseconds(0));
   //     GusTV::BroadCast(ChannelOffice::CHANNEL::SELECTION, *prog);
        
        //SwitchCameraMode();
    }


    RenderColorDBG::rayFrom = pos;
    RenderColorDBG::rayTo = GusVec3(ray.x(),ray.y(),ray.z());
}



void Scene::UpdatePhysicalWorld(float time) {
    physicalWorld.UpdateTime(time);
    physicalWorld.UpdateEntity();
}



void Scene::UpdateNonPhysicals() {

	if(selectionBox){
		/*
		vector<int> idx_vec = selectionBox->GetGroup();
		vector<SceneObject*> refs = (*sceneData)[StageObject::STAGE_OBJ_ID::ENTITY]->GetVector(idx_vec);
		GusVec3 pos;

		for(auto obj : refs) {
			pos += ((StageObject*)obj)->GetPosition();
		}
		pos /= refs.size();

		selectionBox->SetPosition(pos);
		*/
		GusVec3 pos = selectionBox->GetPosition();
		GLfloat* model = selectionBox->GetMODEL_v();
		LoadIdentity4fv(model);
		Translate(model,pos.x, pos.y, pos.z);
	}
}



void Scene::SwitchCameraMode(){

    defaultCamera->FocusOn(*(StageObject*)selectedObject);
    
}



void Scene::ResetCamera() {
	defaultCamera->ResetPosition();
}



void Scene::ZoomSkyCamera(float distance){
	skyCamera->SetPosition(skyCamera->GetPosition() + GusVec3(0.f,distance,0.f));
}



void Scene::SetSelected(const SceneObject& obj) {
	if(&obj){
		int obj_id = obj._GetId();

		if(selections.count(obj_id) == 0){
			selections[obj_id] = &obj;
			selectionHistory.push_back(obj_id);
			((StageObject&)obj).SetSelected(true);
			LOGE("Obj %s added to list", obj._GetName().c_str());
		}
		else {
			if(!selectionHistory.empty()){
				selectionHistory.erase(selectionHistory.begin()+GetSelectionOrder(obj_id));
			}
			selections.erase(obj._GetId());
			((StageObject&)obj).SetSelected(false);
			LOGE("Obj %sremoved to list", obj._GetName().c_str());
		}

		if(selections.size() > 0) {
			vector<const StageObject*> refs;
			LOGE("[SCENE][SetSelected] Is in selection ");
			for(auto pair : selections) {
				refs.push_back(pair.second);
				LOGE("					 %s",pair.second->_GetName().c_str());
			}
			NewSelectionBox(refs);
		}
		if(selectionHistory.size() > 1){
			lastSelectedObject = GetStageFromIndex(selectionHistory.at(selectionHistory.size()-2));
			//selectedObject = GetStageFromIndex(selectionHistory.back());
			selectedObject = selectionBox;
		} else if(1 == selectionHistory.size()){
			lastSelectedObject = nullptr;
			selectedObject = GetStageFromIndex(selectionHistory.back());
		} else {
			lastSelectedObject = nullptr;
			selectedObject = nullptr;
			vector<const StageObject*> stages;
			for(auto i : sceneBox->GetGroup()){
				stages.push_back((StageObject*)GetStageFromIndex(i));
			}
			NewSelectionBox(stages);
		}

	}
	if(selectedObject){
		LOGE("[SCENE][SetSelected] current selectedObject is %s",selectedObject->_GetName().c_str());
	}else {
		LOGE("[SCENE][SetSelected] current selectedObject -");
	}
	LockMode(lockMode);
}



void Scene::SetSelected(int idx) {
	SetSelected(*GetStageFromIndex(idx));
}



vector<SceneObject*> Scene::GetSelected() {
	vector<SceneObject*> ret_vec;
	for(auto stg_pair : selections) {
		ret_vec.push_back(const_cast<SceneObject*>(stg_pair.second));
	}
	return ret_vec;
}



void Scene::ResetSelection() {
	selectedObject = nullptr;
	lastSelectedObject = nullptr;
	for(auto stg_pair : selections){
		((StageObject*)stg_pair.second)->SetSelected(false);
	}
	selections.clear();
	selectionHistory.clear();
	if(selectionBox) {
		LOGE("[SCENE][ResetSelection] Last words before the end??! for %s",selectionBox->_GetName().c_str());
		delete selectionBox;
		selectionBox = nullptr;
	}
}



int Scene::GetSelectedObject(){
	if(selectedObject){
		if(selectedObject == selectionBox){
			return selectionHistory.back();
		} else {
			return selectedObject->_GetId();
		}
	}
	return -1;
}



int	Scene::GetSelectionOrder(int idx){
	int token = 0;
	for(auto e : selectionHistory){
		if(idx == e){
			return token;
		}
		token++;
	}
	return -1;
}

void Scene::NewSelectionBox(const vector<const StageObject*>& obj_vec) {

	if(selectionBox) {
		delete selectionBox;
	}

	selectionBox = new SceneBox(obj_vec);
}



vector<SceneObject*> Scene::GetStageFromIndex(vector<int> idx) {

	return (*sceneData)[StageObject::STAGE_OBJ_ID::ENTITY]->GetVector(idx);
}



vector<int>	 Scene::GetIndexFromStage(vector<SceneObject*> stages) {

	vector<int> results;
	for(auto stage : stages){
		results.push_back(stage->_GetId());
	}
	return results;
}



SceneObject* Scene::GetStageFromIndex(int idx) {

	return (*sceneData)[StageObject::STAGE_OBJ_ID::ENTITY]->GetEntry(idx);
}


void Scene::SetStageVisibility(int idx, bool state){

	((StageObject*)GetStageFromIndex(idx))->SetVisible(state);
}



void Scene::HideAllExcept(const StageObject& in_obj) {
	for(auto obj : objOnStage){
			((StageObject*)obj)->SetVisible(false);
		}
		in_obj.SetVisible(true);
}


void Scene::HideAll() {
	for(auto obj : objOnStage){
			((StageObject*)obj)->SetVisible(false);
		}
}
















































































