//
//  EntityAnimated.cpp
//  helloGL
//
//  Created by tomato on 4/4/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "EntityAnimated.h"
#include "Mesh.h"


EntityAnimated::EntityAnimated():OFFSET(new GLfloat[16]){
   
    LoadIdentity4fv(OFFSET);
    Translate(OFFSET, 0.f, 0.3f, 0.f);
}

EntityAnimated::EntityAnimated(string name, md5Object& md5_obj):OFFSET(new GLfloat[16]){
   
    LoadIdentity4fv(OFFSET);
    Translate(OFFSET, 0.f, 0.3f, 0.f);
}

void EntityAnimated::_SetAttrib(string attrib_name, string value){
    attribs.Call(attrib_name, value);
}

void EntityAnimated::_SetPostAttrib(){
    attribs.mesh = new Mesh("md5->"+mesh->_GetName());
    attribs.mesh->SetAttribs(mesh->GetAttribs());
    
    //GusVec3 correction = GusVec3(0.f, mesh->GetBoxMin(10).y/2.f ,0.f);
    //GusVec3 correction = GusVec3();
    //attribs.mesh->GetAttribs()->SetBoxExtrema(mesh->GetBoxMax(10) );
    //attribs.mesh->GetAttribs()->SetBoxExtrema(mesh->GetBoxMin(10) );
    //attribs.mesh->GetAttribs()->BuildBox();
    
   // Rotate3fv(attribs.mesh->GetHalfVec(),90.f,X_AXE);
   // Rotate3fv(attribs.mesh->GetHalfVec(),180.f,Y_AXE);
    
    Entity::_SetPostAttrib();
    
    
}

void EntityAnimated::_SetAttribFromChild(GusGLObject * child) {
    cout <<  "[EntityAnimated][_SetAttribsFromChild] CALL!" << endl;
    string type = GusGLObjectFactory::GetType(child);
    if(type == "md5Object"    )  { mesh              = (md5Object *) child; }
    if(type == "Material")       { attribs.material  = (Material *)  child; }
    
}

void EntityAnimated::SetModel(RENDER_PASS type){
    if (!GetStageRef()) {
        LoadIdentity4fv(MODEL);
    }
    
    Sync();
    
    //glUniformMatrix4fv(shaders[type]->GetEntityUniformLocation(0), 1, GL_FALSE, OFFSET);
}

void EntityAnimated::SetGLAttribs(RENDER_PASS type){
    
  //  cout <<  "[EntityAnimated][SetGLAttribs] CALL!" << endl;
    
    mesh->getAttributeLocation(shaders[type]->GetGLId());
    
    GetMaterial()->SetGLAttribs(shaders[type]);
    
  //  attribs.mesh->SetGLAttribs(shaders[type]);
}

void EntityAnimated :: Render(){
    
   // mesh->getAttributeLocation(shaders[COLOR]->GetGLId());
    
    mesh->update(1);
    
    mesh->updateAnim();
    
    mesh->render();

}

