//
//  Light.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Light.h"
#include "GusGLObjectFactory.h"
#include "Shader.h"
#include "../Scene/SceneObject.h"

const  string attribs_list[] = {"lights[0]", "lights[1]", "lights[2]", "lights[3]"};
const  vector<string> Light::attribsList(attribs_list,attribs_list+4);


Light* Light::defaultLight = NULL;

Light::Light():
    attribs(),
    GLPackedAttrib(new GLfloat[16])
{
    
}

Light::Light(string in_name, int GL_idx):
    attribs(),
    GLPackedAttrib(new GLfloat[16])
{
    attribs.name = in_name;
    GLId = GL_idx;
}

Light::~Light(){
    
}



void Light::_SetAttrib(string attrib_name, string value){
    attribs.Call(attrib_name, value);
}



void Light::_SetPostAttrib(){
    
}



void Light::_SetAttribFromChild(GusGLObject * child) {
    cout <<  "[Light][_SetAttribsFromChild] CALL!" << endl;
}



void Light::SetGLAttribs(Shader& shader){

	Sync();

	LoadEmpty4fv(GLPackedAttrib);

	AddRow4fv(GLPackedAttrib,0,attribs.position);
	GLPackedAttrib[3] = 1.f;
	AddRow4fv(GLPackedAttrib,1,attribs.direction);
	GLPackedAttrib[7] = 0.f;
	AddRow4fv(GLPackedAttrib,2,attribs.color);
	GLPackedAttrib[11] = 1.f;

	GLPackedAttrib[12] = attribs.type;

    glUniformMatrix4fv(shader.GetLightUniformLocation(GLId), 1, GL_FALSE, GLPackedAttrib);
}



void Light::Sync()
{
    attribs.direction = stageObjRef->GetDirection();
    attribs.position = stageObjRef->GetPosition();
}




















