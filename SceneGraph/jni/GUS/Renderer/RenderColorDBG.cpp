//
//  RenderColorDBG.cpp
//  helloGL
//
//  Created by tomato on 3/5/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Renderer.h"
#include "Entity.h"
#include "EntityDBG.h"
#include "Camera.h"

map<DEBUG_MODE,bool> RenderColorDBG::isModeEnable;

bool RenderColorDBG::shadowOn     = true;
bool RenderColorDBG::textureOn    = true;
GusVec3 RenderColorDBG::rayFrom = GusVec3(0.f,5.f,0.f);
GusVec3 RenderColorDBG::rayTo   = GusVec3(0.f,5.f,-1000000.f);

//DEBUG_MODE {NONE, POINT_MODE,LINE_MODE,TRI_MODE,BOX_MODE, NORMAL_MODE, TANGENT_MODE, RAY_MODE};



RenderColorDBG::RenderColorDBG() :
    currentMode(NONE),
    rayVBO(0)
{
    InitModeCapability();
}



void  RenderColorDBG::InitModeCapability()
{
    isModeEnable[POINT_MODE]   = false;
    isModeEnable[LINE_MODE]    = true;
    isModeEnable[TRI_MODE]     = false;
    isModeEnable[BOX_MODE]     = true;
    isModeEnable[NORMAL_MODE]  = false;
    isModeEnable[TANGENT_MODE] = false;
    isModeEnable[RAY_MODE]     = false;
}



void RenderColorDBG::Init()
{
    RenderColor::Init();
    glGenBuffers(1, &rayVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rayVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6, NULL, GL_DYNAMIC_DRAW);
}



void RenderColorDBG::setGLState()
{
    RenderColor::setGLState();
}



void RenderColorDBG::SetFrameBuffer(){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBindFramebuffer(GL_FRAMEBUFFER, Renderer::defaultFBO);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}

void RenderColorDBG::SetMatrix(){
    
    ComputeModelViewProjectionMatrix();
    
    ComputeModelViewInverseMatrix();

}

void RenderColorDBG::SetGLAttribs(){
    
 //   EntityDBG * entityDBG = (EntityDBG*) currentEntity;
    
//    Shader* shader = entityDBG->shaderSet.shadersDBG[currentMode];
    //Shader* shader = entityDBG->shaderSet.shadersDBG[POINT_MODE];
    Shader* shader = Shader::GetShaderObject(0x10);
    
    SetMatrix();
    
    glUniformMatrix4fv(shader->GetRendererUniformLocation(1), 1, GL_FALSE, MVP);
    
    glUniformMatrix4fv(shader->GetRendererUniformLocation(3), 1, GL_FALSE, MODELVIEW);
    
    glUniformMatrix3fv(shader->GetRendererUniformLocation(2), 1, GL_FALSE, MV_I);
        
    glUniformMatrix4fv(shader->GetCameraUniformLocation(0), 1, GL_FALSE, VIEW);
}



void RenderColorDBG::RenderRay(GusVec3 from, GusVec3 to){
    Shader* shader = Shader::GetShaderObject(0x10);
    glUseProgram(shader->GetGLId());
    
    GLfloat vertex[6] = {from.x,from.y,from.z,to.x,to.y,to.z};

    glBindBuffer(GL_ARRAY_BUFFER, rayVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), (GLfloat*) vertex);
    glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
    
    glUniform1i(shader->GetDebugUniformLocation(0), 5);
    
    currentCamera = Camera::GetCurrentCamera();
    currentCamera->SetCamera();
    CopyMatrix(VIEW , currentCamera->GetViewMatrix());
    
    LoadIdentity4fv(MVP);
    MultMatrixf(MVP, PROJ, VIEW);
    glUniformMatrix4fv(shader->GetRendererUniformLocation(1), 1, GL_FALSE, MVP);
    glDrawArrays(GL_LINES, 0, 2);
}



void RenderColorDBG::Render(EntityDBG* entityDBG, DEBUG_MODE mode){
    
        glUseProgram(entityDBG->shaderSet.shadersDBG[mode]->GetGLId());
    
        entityDBG->SetModel(mode);              //MODEL
    
        SetGLAttribs();                         // VIEW MVP MV_I
    
        entityDBG->SetGLAttribs(mode);
        entityDBG->Render(mode);
}



void RenderColorDBG::OpenFullPipeline(){

    SetFrameBuffer();
    
    setGLState();
    
    if(!InsetMode){
		for (auto pass : passToRender) {

			currentCamera = (Camera*)&pass.GetCamera();

			currentCamera->SetCamera();

			currentMode = pass.GetRDBGType();

			for (auto pair : isModeEnable) {

				if (pair.second) {
				   if(currentMode == pair.first){

					for(auto ent : pass.GetEntities()){
						if(currentMode == LINE_MODE){
							//LOGE("[RENDERCOLORDBG][OpenFullPipeline] rendering %s", ent->_GetName().c_str());
						}
							currentEntity = (EntityDBG*)ent;

							Render((EntityDBG*)ent,pair.first);

							if(currentMode)

						if(Renderer::renderedFrame == 0) {
							cout << "[RENDERCOLORDBG][Render] EntityDBG List " << ent->_GetName() << endl;
						}
					}
				  }
				}
			}
		}
		if (isModeEnable[RAY_MODE]) {
			RenderRay(rayFrom, rayTo);
		}
    }
}










