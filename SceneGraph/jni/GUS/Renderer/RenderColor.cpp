//
//  RenderColor.c
//  helloGL
//
//  Created by metatomato on 2/1/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//


#include <stdio.h>
#include "Renderer.h"
#include "Entity.h"
#include "../Scene/Control.h"
#include "Mesh.h"
#include "../Scene/Scene.h"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))



RenderColor::RenderColor() : currentEntity(nullptr),
                             currentShader(nullptr),
                             currentCamera(nullptr),
                             currentLight(nullptr),
                             currentLight0(nullptr) {
  Init();
}



void RenderColor::Init(){
     cout << "[RenderColor][Init] CALL" << endl;
    LoadIdentity4fv(MODEL);
    LoadIdentity4fv(VIEW);
    LoadIdentity4fv(PROJ);
    LoadIdentity4fv(MODELVIEW);
    LoadIdentity4fv(MVP);
    LoadIdentity4fv(MVP_SHADOW);
    LoadIdentity3fv(MV_I);
    LoadIdentity3fv(MV3);
    
    float ratio = (float)Renderer::frameView[0] / (float)Renderer::frameView[1];

    Perspectivef(PROJ, 40.0f, ratio, 0.1f, 2000.0f);
}

void RenderColor::setGLState(){
    // glViewport(0,0,Renderer::frameView[0],Renderer::frameView[1]);
    glViewport(0,0,Renderer::frameView[0],Renderer::frameView[1]);
    glEnable(GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


void RenderColor::SetFrameBuffer(){
    glClearColor(0.0, 0.847, 1.0, 1.0);
    glBindFramebuffer(GL_FRAMEBUFFER, Renderer::defaultFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}



void RenderColor :: SetMatrix() {
    ComputeModelViewProjectionMatrix();
    ComputeModelViewInverseMatrix();
    ComputeShadowProjectionMatrix();
};



void RenderColor::ComputeModelViewProjectionMatrix(){
    CopyMatrix(VIEW , currentCamera->GetViewMatrix());
    LoadIdentity4fv(MVP);
    LoadIdentity4fv(MODELVIEW);
    CopyMatrix(MODEL,currentEntity->GetModel());
    MultMatrixf(MODELVIEW , VIEW, MODEL);
    MultMatrixf(MVP, PROJ, MODELVIEW);
}



void RenderColor::ComputeModelViewInverseMatrix(){
    LoadIdentity3fv(MV_I);
    InverseMatrix(MODELVIEW, MV_I);
    SubMatrix(MODELVIEW, MV3);
}



void RenderColor::ComputeShadowProjectionMatrix(){

    LoadIdentity4fv(MVP_SHADOW);
    //LookAt(MVP_SHADOW, GusVec3(0.0f,80.0f,0.0f), GusVec3(0.0f, 0.0f, 0.0f),GusVec3(0.0f,0.0f, -1.0f));
    CopyMatrix(MVP_SHADOW,RenderShadow::GetShadowView());

    GLfloat temp[16];
    
    Perspectivef(temp, 40.0f, 1.0f, 0.1f, 2000.0f);
    
    GLfloat coef_bias = 0.5f;
    
    GLfloat bias[] = { coef_bias,   0.0f,       0.0f,       0.0f,
                       0.0f,        coef_bias,  0.0f,       0.0f,
                       0.0f,        0.0f,       coef_bias,  0.0f,
                       coef_bias,   coef_bias,  coef_bias,  1.0f };
       
    MultMatrixf(MVP_SHADOW , MVP_SHADOW, MODEL);
    
    MultMatrixf(MVP_SHADOW ,temp, MVP_SHADOW);
    
    MultMatrixf(MVP_SHADOW, bias , MVP_SHADOW);
}

void RenderColor::SetGLAttribs(){
    
    Shader* shader = currentEntity->GetShader(COLOR);
    

	for(auto l : passLights) {
		((Light*)l)->SetGLAttribs(*shader);
	}
    //currentLight->SetGLAttribs(*shader);
    //currentLight0->SetGLAttribs(*shader);
    /*
    glUniform3fv(shader->GetLightUniformLocation(0), 1, currentLight->GetDirection());
    glUniform3fv(shader->GetLightUniformLocation(1), 1, currentLight->GetPosition());
    glUniform3fv(shader->GetLightUniformLocation(3), 1, currentLight0->GetPosition());
*/
    SetMatrix();
    
    glUniformMatrix4fv(shader->GetRendererUniformLocation(1), 1, GL_FALSE, MVP);
    
    glUniformMatrix4fv(shader->GetRendererUniformLocation(3), 1, GL_FALSE, MODELVIEW);
    
    glUniformMatrix3fv(shader->GetRendererUniformLocation(2), 1, GL_FALSE, MV_I);
    
    glUniformMatrix4fv(shader->GetRendererUniformLocation(4), 1, GL_FALSE, MVP_SHADOW);
    
    glUniformMatrix4fv(shader->GetCameraUniformLocation(0), 1, GL_FALSE, VIEW);
}

void RenderColor::Render(Entity* entity){
    
            glUseProgram(entity->GetShader(COLOR)->GetGLId());
        
            entity->SetModel(COLOR);                //MODEL
        
            SetGLAttribs();                         // VIEW MVP MV_I
    
            entity->SetGLAttribs(COLOR);
            
            entity->Render();
}

void RenderColor::OpenFullPipeline(){

    SetFrameBuffer();
    
    setGLState();


    if(InsetMode){

        	glEnable(GL_SCISSOR_TEST);
        	//glScissor(180,340,1280,800);
        	//glViewport(180,340,1280,800);
        	glScissor(	Renderer::insetFrame[0],Renderer::insetFrame[1],
        				Renderer::insetFrame[2],Renderer::insetFrame[3]);

        	glViewport(	Renderer::insetFrame[0],Renderer::insetFrame[1],
        				Renderer::insetFrame[2],Renderer::insetFrame[3]);

        	glClearColor(0.9, 0.9, 0.9, 1.0);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        	for (auto pass : passToRender) {
        		if(RENDER_PASS::SCISSOR == pass.GetRType()){
        	        currentCamera = (Camera*)&pass.GetCamera();
        	        currentCamera->SetCamera();

        	        for(auto ent : pass.GetEntities()){

        	            currentEntity = (Entity*)ent;
        	            Render((Entity*)ent);

        	            //LOGE( "[RENDERCOLOR][OpenFullPipeline] Inset Mode %s", currentEntity->_GetName().c_str());
        	            if(Renderer::renderedFrame == 0) {
        	                cout << ent->_GetName()  << " mat " <<  currentEntity->GetMaterial()->_GetName() << endl;
        	            }
        	        }
        	    }
        	}
        	glDisable(GL_SCISSOR_TEST);
        }else {

		if(StencilMode){
			for (auto pass : passToRender) {
				if(RENDER_PASS::STENCIL == pass.GetRType()){

					currentCamera = (Camera*)&pass.GetCamera();
					passLights = pass.GetLight();
					currentCamera->SetCamera();

					for(auto ent : pass.GetEntities()){
						glEnable(GL_STENCIL_TEST);
						glStencilFunc(GL_ALWAYS,1,1);
						glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
						glStencilMask(1);
						glClearStencil(0);
						glClear(GL_STENCIL_BUFFER_BIT);

						currentEntity = (Entity*)ent;
						Render(currentEntity);

						glStencilFunc(GL_EQUAL,0,1);
						glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
						glStencilMask(0x00);

						currentEntity->ScaleModel(1.01f);
						currentEntity->SetShader(COLOR,Shader::GetShaderObject(0x10));
						Render(currentEntity);
						currentEntity->ResetScale();
						currentEntity->InitShader();
					}
					glDisable(GL_STENCIL_TEST);
				}
			}

		}


		for (auto pass : passToRender) {
			if(RENDER_PASS::COLOR == pass.GetRType()){
				if(Renderer::renderedFrame == 0) {
					cout << "[RENDERCOLOR][Render] Entity List from " << pass.GetName() << endl;
				}

				currentCamera = (Camera*)&pass.GetCamera();
				currentCamera->SetCamera();
				passLights = pass.GetLight();

				for(auto ent : pass.GetEntities()){
					currentEntity = (Entity*)ent;
					if(!currentEntity->IsOutlined()){
						Render((Entity*)ent);
					}

					if(Renderer::renderedFrame == 0) {
						cout << ent->_GetName()  << " mat " <<  currentEntity->GetMaterial()->_GetName() << endl;
					}
				}
			}
		}
	}





}








