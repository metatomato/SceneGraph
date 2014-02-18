//
//  EntityAnimated.h
//  helloGL
//
//  Created by tomato on 4/4/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "Entity.h"
#include "md5.h"



class EntityAnimated : public Entity {
        public :
    EntityAnimated();
    EntityAnimated(string name, md5Object&);

    virtual void                    _SetAttrib(string attrib_name, string value);
    virtual void                    _SetPostAttrib();
    virtual void                    _SetAttribFromChild(GusGLObject* child);
    
    void                            SetGLAttribs(RENDER_PASS type);
    void                            SetModel(RENDER_PASS type);
    void                            Render();
    
        private :
    md5Object*                      mesh;
    GLfloat*                        OFFSET;
};


