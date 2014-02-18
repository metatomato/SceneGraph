//
//  GusGLObjectFactory.h
//  helloGL
//
//  Created by tomato on 1/25/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "GusGLObject.h"



class GusGLObjectFactory {
        public :
    GusGLObjectFactory();
    ~GusGLObjectFactory();
    static GusGLObject *          CreateObject(string type);
    
    static void                 AddAttrib(GusGLObject* obj, string attrib_name, string value);
    static void                 AddAttribToParent(GusGLObject* parent, GusGLObject* child);
    static void                 AddPostAttrib(GusGLObject * obj);
    static string               GetType(GusGLObject *obj);
    
    private :
    
    
};



