//
//  Engine.h
//  helloGL
//
//  Created by tomato on 3/5/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "Renderer.h"

class Engine  {
        public :
    Engine();
    
    void                            InitStage();
    void                            PushToStage(vector<RenderPass<GusGLObject>>& pass);
    void                            PushToStage(RenderPass<GusGLObject>& pass);
    void                            PushToStage();
    void                            ClearStage();
    void                            Render();

    void                            EnablePass(RENDER_PASS pass){isPassEnable[pass]=true;};
    void                            DisablePass(RENDER_PASS pass){isPassEnable[pass]=false;};
    
    
    
        protected :
    
    
        private :
    map<RENDER_PASS,bool>               isPassEnable;
    
    vector<RenderPass<GusGLObject>>     passToRender;
    
    RenderColor                         colorStage ;
    RenderColorDBG                      debugColorStage ;
    RenderShadow                        shadowStage ;
    
    
};

