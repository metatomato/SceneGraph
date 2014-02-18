//
//  Shape.h
//  helloGL
//
//  Created by tomato on 4/3/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "Entity.h"
#include "Material.h"

class Shape : public Entity {
        public :
	enum SHAPE{CUBE, SPHERE, CYLINDER, SUZANNE};

    Shape(SHAPE in_shape, string name);

    void SetMaterial(Material* mat);
    
        protected :
    SHAPE						shape;


};




Shape* CreateShape(Shape::SHAPE shape);
