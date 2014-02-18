//
//  Camera.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Camera.h"
#include "GusGLObjectFactory.h"
#include "../Scene/Scene.h"


const  string attribs_list[] = {"VIEW"};
const  vector<string> Camera::attribsList(attribs_list,attribs_list+1);

Camera* Camera::currentCamera = NULL;

Camera::Camera():
    attribs()
{
  
}

Camera::Camera(string in_name):
    attribs()
{
  attribs.name = in_name;
}


Camera::~Camera(){

}

void Camera::_SetAttrib(string attrib_name, string value)
{
    attribs.Call(attrib_name, value);
}

void Camera::_SetPostAttrib(){
    
}

void Camera::_SetAttribFromChild(GusGLObject * child)
{
    string type = GusGLObjectFactory::GetType(child);
}



void Camera::Sync()
{
    attribs.position    = stageObjRef->GetPosition();
    attribs.direction   = stageObjRef->GetDirection();
    attribs.up          = stageObjRef->GetUp();

}

void Camera::SetCamera()
{
    Sync();
    LoadIdentity4fv(VIEW);
    LookAt(VIEW, attribs.position , attribs.direction , attribs.up);
}

void Camera::SetGLAttribs(Shader& shader)
{
    glUniformMatrix4fv(shader.GetCameraUniformLocation(0), 1, GL_FALSE, VIEW);
}











