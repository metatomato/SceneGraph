//
//  Engine.cpp
//  helloGL
//
//  Created by tomato on 3/5/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Engine.h"


Engine::Engine():isPassEnable(),passToRender(),colorStage(),debugColorStage(),shadowStage(){
    isPassEnable[COLOR]=true;
    isPassEnable[GDEBUG]=true;
    isPassEnable[SHADOW]=true;
    isPassEnable[SCISSOR]=true;
    isPassEnable[STENCIL]=true;
    cout << "[ENGINE][Engine] CALL" << endl;
}



void Engine::InitStage()
{
	LOGE("[ENGINE][InitStage] ENGINE INIT!!!");
    colorStage.Init();
    debugColorStage.Init();
    shadowStage.Init();
}



void Engine::PushToStage(){
    PushToStage(passToRender);
}

void Engine::PushToStage(RenderPass<GusGLObject>& pass){
    vector<RenderPass<GusGLObject>> vec_pass(1,pass);
    PushToStage(vec_pass);
}

void Engine::PushToStage(vector<RenderPass<GusGLObject>>& pass_vec){

    ClearStage();
    
    for(auto pass : pass_vec)
    {
        RENDER_PASS type = pass.GetRType();
        if(type == COLOR || type == SCISSOR){
            colorStage.Append(pass);
            isPassEnable[type] = true;
        }
        if(type == GDEBUG){
            debugColorStage.Append(pass);
            isPassEnable[GDEBUG] = true;
        }
        if(type == SHADOW){
            shadowStage.Append(pass);
            isPassEnable[SHADOW] = true;
        }

        if(type == STENCIL){
			colorStage.Append(pass);
			isPassEnable[STENCIL] = true;
        }
    }
}



void Engine::ClearStage(){
    colorStage.ClearPass();
    debugColorStage.ClearPass();
    shadowStage.ClearPass();
}



void Engine::Render(){
    for (auto pair : isPassEnable) {
        if(pair.second){
            RENDER_PASS pass_type = pair.first;
            if(pass_type == SHADOW) {
                shadowStage.Render();
                colorStage.SetShadowProjMatrix(shadowStage.GetShadowMatrix());
            }
            if(pass_type == COLOR)  colorStage.OpenFullPipeline();
            if(pass_type == GDEBUG) debugColorStage.OpenFullPipeline();
        }
    }
    Control::ResetValue();
    Renderer::StencilMode = false;
    Renderer::renderedFrame++;
}

















