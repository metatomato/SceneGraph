//
//  RenderShadow.cpp
//  helloGL
//
//  Created by metatomato on 2/1/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//



#include <stdio.h>
#include "Renderer.h"
#include "Entity.h"

GLuint RenderShadow::depthMap = 0;
GLfloat RenderShadow::VIEW[16];
RenderShadow::RenderShadow(){

}


void RenderShadow::Init(){
    
    LoadIdentity4fv(MODEL);
    LoadIdentity4fv(VIEW);
    LoadIdentity4fv(PROJ);
    LoadIdentity4fv(MODELVIEW);
    LoadIdentity4fv(MVP);
    LoadIdentity3fv(MV_I);
    LoadIdentity3fv(MV3);
    
    Perspectivef(PROJ, 40.0f, 1.0f, 0.1f, 2000.0f);
    
    shadowBufferSize = 1024;
    
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowBufferSize, shadowBufferSize, 0,GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
        
    glGenFramebuffers(1, &shadowBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
}

void RenderShadow::setGLState(){
    glViewport(0,0,shadowBufferSize,shadowBufferSize);
    glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
   // glCullFace(GL_FRONT);
}

void RenderShadow::SetFrameBuffer(){
   // glBindFramebuffer(GL_FRAMEBUFFER, Renderer::defaultFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void RenderShadow::ComputeModelViewProjectionMatrix(){
    LoadIdentity4fv(MVP);
    LoadIdentity4fv(MODELVIEW);
    CopyMatrix(MODEL,currentEntity->GetModel());
    MultMatrixf(MODELVIEW , VIEW, MODEL);
    MultMatrixf(MVP, PROJ, MODELVIEW);
}

void RenderShadow::ComputeModelViewInverseMatrix(){
    
    LoadIdentity3fv(MV_I);
    
    InverseMatrix(MODELVIEW, MV_I);
    
    //TransposeMatrix3fv(MV_I);
    
    SubMatrix(MODELVIEW, MV3);
}



GusVec3 RenderShadow::ComputeUp(GusVec3 eye,GusVec3 center) {

	GusVec3 up;
	GusVec3 vec = center - eye;
	float length = vec.Length();

	float theta = vec.y / length;
	float phi = vec.z / (cosf(theta)* length);

	up.x = - cosf(theta) * sinf(phi);
	up.y = sinf(theta);
	up.z = - cosf(theta) * cosf(phi);

	return up;
}

void RenderShadow::SetCamera(){
/*
    GusVec3 pos = cameraControl.GetPos();
    GusVec3 dir = cameraControl.GetDir();
    GusVec3 up  = cameraControl.GetUp();
    
    LoadIdentity4fv(VIEW);
    LookAt(VIEW, pos.x , pos.y , pos.z, dir.x, dir.y, dir.z , up.x, up.y, up.z);
   */

	GusVec3 pos = ((Light*)light)->GetPosition();
	GusVec3 dir = GusVec3(0.f,0.f,0.f);
	GusVec3 up = ComputeUp(pos,dir);
	//pos.Normalize();

    LoadIdentity4fv(VIEW);
    LookAt(VIEW, pos, dir, up);
}

void RenderShadow::SetMatrix(){
    
}

void RenderShadow::SetGLAttribs(){
    Shader* shader = currentEntity->GetShader(SHADOW);
    
    ComputeModelViewProjectionMatrix();
    
    ComputeModelViewInverseMatrix();
    
    glUniformMatrix4fv(shader->GetRendererUniformLocation(1), 1, GL_FALSE, MVP);
    
    glUniformMatrix4fv(shader->GetRendererUniformLocation(3), 1, GL_FALSE, MODELVIEW);
    
    glUniformMatrix3fv(shader->GetRendererUniformLocation(2), 1, GL_FALSE, MV_I);
    
    glUniformMatrix4fv(shader->GetCameraUniformLocation(0), 1, GL_FALSE, VIEW);

}

void RenderShadow::Render(){
    
   // Init();
    
    SetFrameBuffer();
    
    setGLState();
    
    
    for (auto pass : passToRender) {
        
    	light = pass.GetLight().front();

		SetCamera();

        for(auto obj : pass.GetEntities()){
            
            currentEntity = (Entity*)obj;
                
                glUseProgram(currentEntity->GetShader(SHADOW)->GetGLId());
            
                currentEntity->SetModel(SHADOW);                        //MODEL
            
                currentEntity->SetGLAttribs(SHADOW);
            
                SetGLAttribs();  // VIEW MVP MV_I
            
            
                currentEntity->Render();
            
                currentEntity->GetMaterial()->SetShadowMap(depthMap);
            
            if(Renderer::renderedFrame == 0) {
               // if((*it)->_GetName().compare("Cube_bis_Cube.000") == 0){
                    cout << "[RENDERSHADOW][Render] Entity List " << obj->_GetName() << endl;
                   // ShowMatrix4fv(MVP, "MVP");
                  //  ShowMatrix4fv(MODEL, "MODEL");
                //}
            }
            
        }
    }
}




