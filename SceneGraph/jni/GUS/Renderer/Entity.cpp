//
//  Entity.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Entity.h"
#include "../Scene/Scene.h"
#include "EntityDBG.h"
#include "../Common/Matrix.h"

const  string attribs_list[] = {"MODEL"};
const  vector<string> Entity::attribsList(attribs_list,attribs_list+1);


Entity::Entity():
    attribs(),
    MODEL(ID_MAT4),
    MODEL_m(new float[16]),
    shaders(),
    entityDBG(nullptr),
    scale(1.f),
    isScale(false),
    isOutlined(false)
{
}



Entity::Entity(string name):
    attribs(),
    MODEL(ID_MAT4),
    MODEL_m(new float[16]),
    shaders(),
    entityDBG(nullptr),
    scale(1.f),
	isScale(false),
	isOutlined(false)
{
    attribs.name = name;
}



Entity::Entity(Mesh* mesh, Material* material):
    attribs(),
    MODEL(ID_MAT4),
    MODEL_m(new float[16]),
    shaders(),
    entityDBG(nullptr),
    scale(1.f),
	isScale(false),
	isOutlined(false)
{
    attribs.mesh = mesh;
    attribs.material = material;
                                                    
    if (mesh->IsPolyGroup()) {
        attribs.name = mesh->_GetName();
        }else attribs.name = mesh->GetParent();
}



Entity::~Entity(){
  //  LOGE("[ENTITY][~Entity] ENTITY DESTRUCTOR CALL \n");
}



void Entity::_SetAttrib(string attrib_name, string value){
    attribs.Call(attrib_name, value);
}



void Entity::_SetPostAttrib(){
    //*(attribs.position) += attribs.mesh->GetOriginalCenter();
    InitShader();

    if(!entityDBG){
    	entityDBG = new EntityDBG(*this);
    }
}

void Entity::InitShader(){
	if (attribs.material) {
	        ShaderSet set = Shader::GetCorrespondance(*(attribs.material));
	        SetShader(COLOR, set.shaders[COLOR]);
	        SetShader(GDEBUG, set.shaders[GDEBUG]);
	        SetShader(SHADOW, set.shaders[SHADOW]);
	        //cout << "[ENTITY][_SetPostAttribs] Shader Activé " << set.shaders[COLOR]->GetGLId()<< " sur " << attribs.name << endl;
	    }
}

void Entity::_SetAttribFromChild(GusGLObject * child) {
    string type = GusGLObjectFactory::GetType(child);
    if( type == "Mesh" ){
        attribs.mesh = (Mesh *) child;
        if(attribs.name == ""){
            if (attribs.mesh->IsPolyGroup()) {
                    attribs.name = child->_GetName();
            }else attribs.name = attribs.mesh->GetParent();

        }
    }
    if(type == "Material")  { attribs.material  = (Material *)  child; }
}



void Entity::SetModel(RENDER_PASS type){
	if (!GetStageRef()) {
	            LoadIdentity4fv(MODEL);
	}

	if(isScale) {
		CopyMatrixDeeply(MODEL,MODEL_m);
		Scale4x4(MODEL_m,scale);
		glUniformMatrix4fv(shaders[type]->GetEntityUniformLocation(0), 1, GL_FALSE, MODEL_m);
		//LOGE("[ENTITY][SetModel] Scale mode activated for %s with factor %f",_GetName().c_str(),scale);
	} else {
		glUniformMatrix4fv(shaders[type]->GetEntityUniformLocation(0), 1, GL_FALSE, MODEL);
		//LOGE("[ENTITY][SetModel] Scale mode disable for %s",_GetName().c_str());
	}
}



const GLfloat* Entity::GetModel() const{
	if(isScale){
		return MODEL_m;
	}else{
		return MODEL;
	}
	return 0;
}



void Entity::SetGLAttribs(RENDER_PASS type){

    GetMaterial()->SetGLAttribs(shaders[type]);
    
    attribs.mesh->SetGLAttribs(shaders[type]);


}



void Entity :: Render(){
    
   // glDrawElements(GL_TRIANGLES,attribs.mesh->GetTriangleCount()*3,GL_UNSIGNED_INT,0);
    
    //cout << "[ENTITY][RENDER] Mesh rendered " << attribs.mesh->_GetName() << " with tri count " << attribs.mesh->GetTriangleCount()*3 << endl;
    
     glDrawArrays(GL_TRIANGLES, 0 , attribs.mesh->GetTriangleCount()*3);
   
}



void Entity::Sync(){
	if(stageObjRef){
		MODEL = stageObjRef->GetMODEL();
		SetOutlined(stageObjRef->IsSelected());
	}
}


/*
string Entity::_ToString() {
    ostringstream description;
    description << endl;
    description << "CLASS " << _Class() << endl;
    description << "    ID       " << _GetId() << endl;
    description << "    Name     " << attribs.name << endl;
    description << "    Position " << attribs.position->x << " " << attribs.position->y << " " << attribs.position->z << endl;
    description << "    MESH     " << attribs.mesh->_GetId() << endl;
    description << "    MATERIAL " << attribs.material->_GetId() << endl;
    return description.str();
}
*/





































