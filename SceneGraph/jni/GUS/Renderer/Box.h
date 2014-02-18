//
//  Box.h
//  helloGL
//
//  Created by tomato on 4/3/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "EntityDBG.h"

class Box : public EntityDBG {
        public :
static map<DEBUG_MODE,bool>        mode;
    
        public :
    Box();
    Box(Box_t& in_box);
    ~Box();

    virtual void                SetModel(DEBUG_MODE mode);
    virtual void                SetGLAttribs(DEBUG_MODE mode);
    virtual void                Render(DEBUG_MODE mode);
    virtual void                GenerateVBO();
    
    
        protected :
    GLfloat                     vertex[24];
    GLuint                      VBO;
    GLuint                      VBO_IDX;
};


class BoxSet : public Box {
        public :
    BoxSet(const vector<SceneObject*>&);
    void                SetGLAttribs(RENDER_PASS type);
    void                GenerateVBO();
    void                SetModel(RENDER_PASS type);
    void                Render();
    
        private :
    vector<vector<GLfloat>>               vertex;
    int                                   num;
};