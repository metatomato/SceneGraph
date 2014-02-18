//  DataCenter.cpp
//
//  helloGL
//
//  Created by tomato on 9/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//
#include <string>

#include "DataCenter.h"
#include "Import.h"
#include "../Renderer/Entity.h"
#include "../Renderer/Box.h"
#include <typeinfo>
#include "../Renderer/Shape.h"

static RenderData DataCenter::renderBaseShapeData;


/////////////////////////////////////////////////////////////////////////////////
//      DATACENTER METHOD DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////



DataCenter::DataCenter():
    renderData(),
    sceneData(),
    renderDataDebug(),
    renderDataFX(),
    stageFromFile(),
    entityFromFile()
{
    renderData[GusGLObject::GUS_OBJ_ID::ENTITY]   = new RenderLibrary();
    renderData[GusGLObject::GUS_OBJ_ID::MESH]     = new RenderLibrary();
    renderData[GusGLObject::GUS_OBJ_ID::MATERIAL] = new RenderLibrary();
    renderData[GusGLObject::GUS_OBJ_ID::LIGHT]    = new RenderLibrary();
    renderData[GusGLObject::GUS_OBJ_ID::CAMERA]   = new RenderLibrary();
    
    renderDataDebug[EntityDBG::GUS_OBJ_ID::BOX]   = new RenderLibrary();
    renderDataDebug[EntityDBG::GUS_OBJ_ID::MESH]  = new RenderLibrary();
    

    renderBaseShapeData[GusGLObject::GUS_OBJ_ID::MESH] = new RenderLibrary();
    renderBaseShapeData[GusGLObject::GUS_OBJ_ID::ENTITY] = new RenderLibrary();

    sceneData[StageObject::STAGE_OBJ_ID::ENTITY]  = new SceneLibrary();
    sceneData[StageObject::STAGE_OBJ_ID::LIGHT]   = new SceneLibrary();
    sceneData[StageObject::STAGE_OBJ_ID::CAMERA]  = new SceneLibrary();
    
    /*
    Camera * default_camera = new Camera("default_camera");
    Camera::SetCurrentCamera(default_camera);
    SceneCamera* default_sCamera = new SceneCamera(*default_camera);
    renderData[GusGLObject::GUS_OBJ_ID::CAMERA]->AddEntry((GusGLObject**) &default_camera);
    sceneData[StageObject::STAGE_OBJ_ID::CAMERA]->AddEntry((SceneObject*) default_sCamera);
    */
    CreateCamera(RENDER,"default_camera");
    //CreateCamera(RENDER,"sky_camera");

    CreateLight(RENDER,"default_light_0",0);
    CreateLight(RENDER,"default_light_1",1);
    CreateLight(RENDER,"default_light_2",2);
    CreateLight(RENDER,"default_light_3",3);
    /*
    Light* default_light = new Light("default_light");
    SceneLight* default_sLight = new SceneLight(*default_light);
    renderData[GusGLObject::GUS_OBJ_ID::LIGHT]->AddEntry((GusGLObject**) &default_light);
    sceneData[StageObject::STAGE_OBJ_ID::LIGHT]->AddEntry((SceneObject*)default_sLight);

    Light* default_light_0 = new Light("default_light_0");
    SceneLight* default_sLight_0 = new SceneLight(*default_light_0);
    renderData[GusGLObject::GUS_OBJ_ID::LIGHT]->AddEntry((GusGLObject**) &default_light_0);
    sceneData[StageObject::STAGE_OBJ_ID::LIGHT]->AddEntry((SceneObject*)default_sLight_0);
    */
}



DataCenter::~DataCenter(){
    for (auto it = renderData.begin(); it!=renderData.end(); ++it) {
        delete renderData[it->first];
    }
    for (auto it = sceneData.begin(); it!=sceneData.end(); ++it) {
        delete sceneData[it->first];
    }
}


void DataCenter::FetchDataFromFile(string file){
    LOGE("[DATACENTER][FetchDataFromFile] FETCH OF %s BEGIN!!", file.c_str());
    ImportObj importer;
    
    vector<vector<GusObject*>> data_pool = importer.ExtractData(file);

    vector<int> refs;
    vector<int> refs_entity;
    
    for (auto vec_obj : data_pool) {

    	if(file.compare("shape.obj") == 0) {
    		if(dynamic_cast<Mesh*>(vec_obj.front())){
    			LOGE("BEGIN BASE SHAPE TRANSFERT!!");
    			renderBaseShapeData[GusGLObject::GUS_OBJ_ID::MESH]->Append(Translate<GusObject,GusGLObject>(vec_obj));
    			for(auto v : renderBaseShapeData[GusGLObject::GUS_OBJ_ID::MESH]->GetVector()){

    				LOGE("BASE SHAPE ADDED : %s",v->_GetName().c_str());
    			}
    		}
    	}

        AppendToLibrary(vec_obj);
        
        
        if (dynamic_cast<SceneObject*>(vec_obj.front())) {
            for(auto obj : vec_obj){
                refs.push_back(obj->_GetId());
                cout << "[DATACENTER][FetchDataFromFile] "<< file << " as Stage " << obj->_GetId() << endl;
            }
        }
        if (dynamic_cast<Entity*>(vec_obj.front())) {
            for(auto obj : vec_obj){
                refs_entity.push_back(obj->_GetId());
                cout << "[DATACENTER][FetchDataFromFile] "<< file << " as Entity " << obj->_GetId() << endl;
            }
        }
        stageFromFile[file]  = refs;
        entityFromFile[file] = refs_entity;
    }
    loadedFiles.push_back(file);
}


int DataCenter::AppendToLibrary(vector<GusObject*>& vec_input){
    int library_ref = -1;
    if (!vec_input.empty()) {
        GusObject* obj = vec_input.front();
        cout << "[DATACENTER][AppedToLibrary] DEBUG GUS_OBJ_TYPE " << obj->_GetType() << endl;
        if(dynamic_cast<GusGLObject*>(obj)){
            pair<int,vector<GusGLObject*>> pair(VectorToLibrary<GusGLObject>(vec_input));
            cout << "[DATACENTER][AppedToLibrary] DEBUG GUS_OBJ_TYPE " << pair.second.front()->_GetType() << endl;
            renderData[(GusGLObject::GUS_OBJ_ID)pair.first]->Append(pair.second);
            library_ref = pair.first;
        }else if(dynamic_cast<SceneObject*>(obj)){
            pair<int,vector<SceneObject*>> pair(VectorToLibrary<SceneObject>(vec_input));
            sceneData[(StageObject::STAGE_OBJ_ID)pair.first]->Append(pair.second);
            library_ref = pair.first;
        }
    }
    return library_ref;
}




RenderPass<GusGLObject> DataCenter::DataToRender(){
    RenderPass<GusGLObject> pass(renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(),
                                   renderData[GusGLObject::GUS_OBJ_ID::LIGHT]->GetVector(),
                                   renderData[GusGLObject::GUS_OBJ_ID::CAMERA]->GetEntry("default_camera"),
                                    COLOR);
    return pass;
}



RenderPass<GusGLObject> DataCenter::DataToRender(string file){
    RenderPass<GusGLObject> pass(DataToRender(renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetNames(entityFromFile[file])));
    pass.SetName(file);
    return pass;
}


RenderPass<GusGLObject> DataCenter::DataToRender(vector<string> obj_names){
    return RenderPass<GusGLObject>(renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(obj_names),
                                   renderData[GusGLObject::GUS_OBJ_ID::LIGHT]->GetVector(),
                                   renderData[GusGLObject::GUS_OBJ_ID::CAMERA]->GetEntry("default_camera"),
                                   COLOR);
}



RenderPass<SceneObject> DataCenter::DataToScene(){
    return RenderPass<SceneObject>(sceneData[StageObject::STAGE_OBJ_ID::ENTITY]->GetVector(),
                                   sceneData[StageObject::STAGE_OBJ_ID::LIGHT]->GetVector(),
                                   sceneData[StageObject::STAGE_OBJ_ID::CAMERA]->GetEntry("default_camera"),
                                   COLOR);
}



vector<RenderPass<GusGLObject>> DataCenter::SceneToRender(vector<RenderPass<SceneObject>>& s_vec_pass){
    vector<RenderPass<GusGLObject>> r_vec_pass;
    for (auto pass : s_vec_pass) {
        r_vec_pass.push_back(SceneToRender(pass));
    }
    return r_vec_pass;
}



RenderPass<GusGLObject> DataCenter::SceneToRender(RenderPass<SceneObject>& s_pass){
    vector<int> entity_ref;
    vector<int> light_ref;
    int camera_ref = -1;
    RENDER_PASS type;
    DEBUG_MODE DBGtype = BOX_MODE;
    vector<GusGLObject*> ent_vec;
    
    if (s_pass.GetSType() == S_OBJ || s_pass.GetSType() == S_SUB_OBJ) {
    	vector<int> group_id;
		for (auto ent : s_pass.GetEntities()) {
			if (s_pass.GetSType() == S_OBJ) {
				group_id = ((StageObject*)ent)->GetGroup();
			}
			if (s_pass.GetSType() == S_SUB_OBJ) {
				group_id = ((StageObject*)ent)->GetSubGroup();
			}
			for (auto ref : group_id) {
				entity_ref.push_back(ref);
			}
		}

        if(s_pass.GetRType() == COLOR || s_pass.GetRType() == SCISSOR || s_pass.GetRType() == SHADOW) {
        	ent_vec = renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(entity_ref);
    	}
        if(s_pass.GetRType() == STENCIL) {
			ent_vec = renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(entity_ref);
				Renderer::StencilMode = true;
		}
        if(s_pass.GetRType() == GDEBUG) {
			for(auto v : renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(entity_ref)){
				ent_vec.push_back(((Entity*)v)->GetEntityDBG());
			}
			DBGtype = LINE_MODE;
		}

    	type = s_pass.GetRType();
    }
    
/*
    if (s_pass.GetSType() == S_SUB_OBJ) {
            for (auto ent : s_pass.GetEntities()) {
                for (auto ref : ((StageObject*)ent)->GetSubGroup()) {
                    entity_ref.push_back(ref);
                }
            }
            if(s_pass.GetRType() == COLOR) {
            	ent_vec = renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(entity_ref);
            }
            if(s_pass.GetRType() == GDEBUG) {
				for(auto v : renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(entity_ref)){
					ent_vec.push_back(((Entity*)v)->GetEntityDBG());
				//	LOGE("[DATACENTER][SceneToRender] Fetching EntityDBG %s",v->_GetName().c_str());
				}
				DBGtype = LINE_MODE;
			}

            type = s_pass.GetRType();
        }
*/

    if (s_pass.GetSType() == S_BOX) {
        for (auto ent : s_pass.GetEntities()) {
            int stage_ref = ent->_GetId();
            GusGLObject* obj = renderDataDebug[EntityDBG::GUS_OBJ_ID::BOX]->GetEntry(stage_ref);
            if(!obj) {
                Box_t box_t = ((StageObject*)ent)->GetBox();
                Box* box = new Box(box_t);
                box->SetStageRef(*((StageObject*)ent));
              //  cout << "[DATACENTER][SceneTorender] stageRef for Box " << ent->_GetId() << endl;
                renderDataDebug[EntityDBG::GUS_OBJ_ID::BOX]->AddEntry(stage_ref, box);
                obj = box;
            }
            ent_vec.push_back(obj);
            type = GDEBUG;
            DBGtype = BOX_MODE;
        }
    }
    

    if (s_pass.GetSType() == S_PRIMITIVE) {

    			int primitive = (int) s_pass.GetPrimitive();
    			GusGLObject* obj = renderBaseShapeData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetEntry(primitive);

    			if(!obj) {
					Shape* shape = CreateShape((Shape::SHAPE)primitive);
					renderBaseShapeData[GusGLObject::GUS_OBJ_ID::ENTITY]->AddEntry(primitive, shape);
					obj = shape;
				}
    			//LOGE("[DATACENTER][SceneToRender] S_PRIMITIVE : Setup Shape Object with %s", obj->_GetName().c_str());

    			int ent_ref = 0;

    			if(s_pass.GetEntities().size() != 0){
    				SceneObject* stage = s_pass.GetEntities().front();
    				int stage_ref = stage->_GetId();
    				//LOGE("[DATACENTER][SceneToRender] S_PRIMITIVE : Setup Stage Object %s",stage->_GetName().c_str());
					ent_ref = ((StageObject*)stage)->GetGroup().front();
					((Shape*)obj)->SetStageRef(*((StageObject*)stage));
    			} else {
    				((Shape*)obj)->ResetStageRef();
    			//	LOGE("[DATACENTER][SceneToRender] S_PRIMITIVE : Setup Stage Object to NULLPTR");
    			}


    			Material* mat = nullptr;
    			int s_pass_mat = s_pass.GetMaterial();
    			if(s_pass_mat == -1 && ent_ref != 0){
    				mat = ((Entity*)renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetEntry(ent_ref))->GetMaterial();
    			//	LOGE("[DATACENTER][SceneToRender] S_PRIMITIVE : Setup Mat from stage : %s", mat->_GetName().c_str());
    			} else if(s_pass_mat != -1) {
    				mat = renderData[GusGLObject::GUS_OBJ_ID::MATERIAL]->GetEntry(s_pass_mat);
    			//	LOGE("[DATACENTER][SceneToRender] S_PRIMITIVE : Setup Mat from s_pass : %s", mat->_GetName().c_str());
    			} else {
    				mat = ((Shape*)obj)->GetMaterial();
    			//	LOGE("[DATACENTER][SceneToRender] S_PRIMITIVE : Setup Mat from default : %s", mat->_GetName().c_str());
    			}

                ((Shape*)obj)->SetMaterial(mat);
               // LOGE("[DATACENTER][SceneToRender] S_PRIMITIVE : Setup Mat Object");

                ent_vec.push_back(obj);
                type = s_pass.GetRType();
        }
     
    for (auto light : s_pass.GetLight()) {
        light_ref.push_back(light->_GetId());
    }

    for(auto ent : ent_vec){
    	((Entity*)ent)->Sync();
    }

    camera_ref = ((StageObject*)&s_pass.GetCamera())->GetGroup().front();
    
    RenderPass<GusGLObject> r_pass(ent_vec,
                                   renderData[GusGLObject::GUS_OBJ_ID::LIGHT]->GetVector(),
                                   renderData[GusGLObject::GUS_OBJ_ID::CAMERA]->GetEntry(camera_ref),
                                   type);
    r_pass.SetName(s_pass.GetName());
    r_pass.SetRDBGType(DBGtype);

    return r_pass;
}




vector<string> DataCenter::GetLibraryList(GusGLObject::GUS_OBJ_ID type){
    return renderData[type]->GetNamesOfCollection();
}




GusGLObject* DataCenter::GetEntity(GusGLObject::GUS_OBJ_ID type,int idx){
    return renderData[type]->GetEntry(idx);
}



vector<GusGLObject*> DataCenter::GetEntity(GusGLObject::GUS_OBJ_ID type,vector<int> ref){
    return renderData[type]->GetVector(ref);
}



int DataCenter::CreateCamera(LIBRARY_TYPE type, string name) {
	int ret = -1;
	 Camera * camera = new Camera(name);
	 LOGE("[DATACENTER] NEW CAMERA WITH ID %d",camera->_GetId());
	 Camera::SetCurrentCamera(camera);
	 SceneCamera* s_camera = new SceneCamera(*camera);
	 LOGE("[DATACENTER] NEW	SCAMERA WITH ID %d BINDED TO %d",s_camera->_GetId(), camera->_GetId());

	 renderData[GusGLObject::GUS_OBJ_ID::CAMERA]->AddEntry((GusGLObject**) &camera);
	 sceneData[StageObject::STAGE_OBJ_ID::CAMERA]->AddEntry((SceneObject*) s_camera);

	 if(type == RENDER){
		 ret = camera->_GetId();
	 }
	 else if(type == SCENE){
		 ret = s_camera->_GetId();
	 }

	 return ret;
}



int DataCenter::CreateLight(LIBRARY_TYPE type, string name, int GL_idx) {
	int ret = -1;
	 Light * light = new Light(name, GL_idx);

	 SceneLight* s_light = new SceneLight(*light);

	 renderData[GusGLObject::GUS_OBJ_ID::LIGHT]->AddEntry((GusGLObject**) &light);
	 sceneData[StageObject::STAGE_OBJ_ID::LIGHT]->AddEntry((SceneObject*) s_light);

	 if(type == RENDER){
		 ret = light->_GetId();
	 }
	 else if(type == SCENE){
		 ret = s_light->_GetId();
	 }

	 return ret;
}



void DataCenter::FillServerMemory()
{
	for(auto obj : renderData[GusGLObject::GUS_OBJ_ID::MESH]->GetVector())
	{
		obj->_SetPostAttrib();
	//	LOGE("[DATACENTER][FillServerMemory] mesh %s has been updated in server DB!",obj->_GetName().c_str());
	}
	for(auto obj : renderData[GusGLObject::GUS_OBJ_ID::MATERIAL]->GetVector())
	{
		obj->_SetPostAttrib();
	//	LOGE("[DATACENTER][FillServerMemory] material %s has been updated in server DB!",obj->_GetName().c_str());
	}
	for(auto obj : renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector())
	{
		obj->_SetPostAttrib();
	//	LOGE("[DATACENTER][FillServerMemory] entity %s has been updated in server DB!",obj->_GetName().c_str());
	}
}



void DataCenter::PrintEntityFromFiles(){

    for (auto pair : entityFromFile) {
        LOGE("[DATACENTER][PrintData] file %s contains",pair.first.c_str());
        vector<int> temp = pair.second;
        for (auto el : temp) {
        LOGE("  						   %s", renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetEntry(el)->_GetName().c_str());
        }
    }
}



void DataCenter::PrintStageFromFiles(){

    for (auto pair : stageFromFile) {
        LOGE("[DATACENTER][PrintData] file %s contains",pair.first.c_str());
        vector<int> temp = pair.second;
        for (auto el : temp) {
        LOGE("  						   %s", sceneData[StageObject::STAGE_OBJ_ID::ENTITY]->GetEntry(el)->_GetName().c_str());
        }
    }
}



vector<string> DataCenter::GetEntityListFromFiles(string file){
	vector<string> ret_vec;
	for(auto ent : renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(entityFromFile[file])){
		ret_vec.push_back(ent->_GetName());
	}
	return ret_vec;
}

vector<string> DataCenter::GetStageListFromFiles(string file){
	vector<string> ret_vec;
		for(auto ent : sceneData[StageObject::STAGE_OBJ_ID::ENTITY]->GetVector(stageFromFile[file])){
			ret_vec.push_back(ent->_GetName());
		}
		return ret_vec;
}



vector<string> DataCenter::GetEntityPackedDataFromFiles(string file) {
	//Formated Data for Entity : NAME/ID/MESH/ID/MATERIAL/ID/STAGE/ID/TRI_COUNT

	vector<string> ret_vec;
		for(auto ent : renderData[GusGLObject::GUS_OBJ_ID::ENTITY]->GetVector(entityFromFile[file])){

			stringstream ss,ss0,ss1,ss2,ss3;

			ret_vec.push_back(ent->_GetName());
			ss << ((Entity*)ent)->_GetId();
			ret_vec.push_back(ss.str());

			ret_vec.push_back(((Entity*)ent)->GetMesh()->_GetName());
			ss0 << ((Entity*)ent)->GetMesh()->_GetId();
			ret_vec.push_back(ss0.str());

			ret_vec.push_back(((Entity*)ent)->GetMaterial()->_GetName());
			ss1 << ((Entity*)ent)->GetMaterial()->_GetId();
			ret_vec.push_back(ss1.str());

			StageObject* stage = ((Scenable*)ent)->GetStageRef();
			if(stage) {
				ret_vec.push_back(stage->_GetName());
				ss2 << stage->_GetId();
				ret_vec.push_back(ss2.str());
				LOGE("[DATACENTER][GetEntityPackedData] added stage ref %s %d",stage->_GetName().c_str(),stage->_GetId());
			} else {
				ret_vec.push_back("null");
				ret_vec.push_back("-1");
			}

			ss3 << ((Entity*)ent)->GetMesh()->GetTriangleCount();
			ret_vec.push_back(ss3.str());
		}
		return ret_vec;
}


vector<string> DataCenter::GetStagePackedDataFromFiles(string file) {
	//Formated Data for StageEntity : NAME/ID/GROUP_SIZE/GROUP_ID/...
	vector<string> ret_vec;
			for(auto ent : sceneData[StageObject::STAGE_OBJ_ID::ENTITY]->GetVector(stageFromFile[file])){
				vector<int> group = ((StageObject*)ent)->GetGroup();
				stringstream ss,ss0,ss1;
				ss << group.size();
				ret_vec.push_back(ent->_GetName());
				ss0 << ent->_GetId();
				ret_vec.push_back(ss0.str());
				ret_vec.push_back(ss.str());
				for(auto el : group){
					stringstream ss0;
					ss0<<el;
					ret_vec.push_back(ss0.str());
				}
			}
			return ret_vec;
}



vector<string> DataCenter::GetLightPackedData() {

	vector<string> ret_vec;
		for(auto light : sceneData[StageObject::STAGE_OBJ_ID::LIGHT]->GetVector()){

				stringstream id;
				stringstream x,y,z;
				stringstream dir_x,dir_y,dir_z;
				stringstream energy;
				stringstream type;

				ret_vec.push_back(light->_GetName());

				id << ((SceneLight*)light)->_GetId();
				ret_vec.push_back(id.str());

				GusVec3 pos = ((SceneLight*)light)->GetPosition();
				x << pos.x;
				y << pos.y;
				z << pos.z;
				ret_vec.push_back(x.str());
				ret_vec.push_back(y.str());
				ret_vec.push_back(z.str());

				GusVec3 dir = ((SceneLight*)light)->GetDirection();
				dir_x << dir.x;
				dir_y << dir.y;
				dir_z << dir.z;
				ret_vec.push_back(dir_x.str());
				ret_vec.push_back(dir_y.str());
				ret_vec.push_back(dir_z.str());

				energy << ((SceneLight*)light)->GetEnergy();
				ret_vec.push_back(energy.str());

				type << ((SceneLight*)light)->GetType();
				ret_vec.push_back(type.str());
			}
			return ret_vec;
}


Mesh* DataCenter::GetBaseShape(string in_name){
	Mesh* ret = (Mesh*)renderBaseShapeData[GusGLObject::GUS_OBJ_ID::MESH]->GetEntry(in_name);
	return ret;
}

































































































































































