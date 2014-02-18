//
//  EntityDBG.h
//  helloGL
//
//  Created by tomato on 3/5/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "Entity.h"



class EntityDBG : public Entity {
        public :
    const static vector<string> attribsList;
    enum class GUS_OBJ_ID{MESH,BOX};

    EntityDBG(){};
    EntityDBG(const Entity& entity);
    ~EntityDBG();
    
    virtual void                SetModel(DEBUG_MODE mode);
    virtual void                SetGLAttribs(DEBUG_MODE mode);
    virtual void                Render(DEBUG_MODE mode);
  
    ShaderSetDBG                shaderSet;
    
        private :
    const Entity*               entityRef;
    
};

