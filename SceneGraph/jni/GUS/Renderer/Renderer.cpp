//
//  Renderer.cpp
//  helloGL
//
//  Created by metatomato on 2/1/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Renderer.h"

#include "Shader.h"


int  Renderer::renderedFrame = 0;
GLint Renderer::defaultFBO = 0;
//GLfloat Renderer::aspectRatio = (float)VIEWPORT_X / (float)VIEWPORT_Y ;
GLfloat Renderer::aspectRatio = 1024.f / 600.f;
GLfloat Renderer::fov = 20.f * PIf;
//GLint Renderer::frameView[2]     = {VIEWPORT_X,VIEWPORT_Y};
GLint Renderer::frameView[2]     = {1024,600};
GLint Renderer::insetFrame[4]     = {180,340,1280,800};
int   Renderer::projectionMode   = PROJECTION;
bool Renderer::InsetMode = false;
bool Renderer::StencilMode = false;
const  string attribs_list[] = {"PROJ","MVP","MV_I","MV","MVP_SHADOW"};
const  vector<string> Renderer::attribsList(attribs_list,attribs_list+5);


Renderer::Renderer() : passToRender(),activeRenderPass(0){
    
}

Renderer::~Renderer(){
    
}

void Renderer::Render(){
  
}

void Renderer::Init(){
    
}

void Renderer::setGLState(){
    
}

void Renderer::SetFrameBuffer(){

}

void Renderer::SetMatrix(){
   
}

void Renderer::SetGLAttribs(){
   }

void Renderer::SetProjection(){
     
   // ShowMatrix(PROJ, "PROJ");
}

void Renderer::SetProjectionMode(P_MODE mode){
    projectionMode = mode;
}

void Renderer::Append(RenderPass<GusGLObject>& pass){
    passToRender.push_back(pass);
}













