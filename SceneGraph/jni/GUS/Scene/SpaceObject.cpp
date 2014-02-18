//
//  SpaceObject.cpp
//  helloGL
//
//  Created by tomato on 9/19/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "SpaceObject.h"
#include "../Common/Matrix.h"

const GLuint Box_t::boxIndex[24] = { 0,1,1,2,2,3,3,0,
                                     4,5,5,6,6,7,7,4,
                                     0,4,1,5,3,7,2,6 };


SpaceObject::SpaceObject():
    box(),
    direction(GusVec3(0.0f,0.0f,1.0f)),
    up(GusVec3(0.0f,1.0f,0.0f)),
    MODEL()
{
    LoadIdentity4fv(MODEL);
                            
}


SpaceObject::SpaceObject(const GusVec3& pos, const GusVec3& halfvec):
    box(pos,halfvec),
    direction(GusVec3(0.0f,0.0f,1.0f)),
    up(GusVec3(0.0f,1.0f,0.0f))
{
    LoadIdentity4fv(MODEL);
    
}



SpaceObject::SpaceObject(const GusVec3& pos, const GusVec3& halfvec, const GusVec3& dir):
	box(pos,halfvec),
	direction(dir),
	up(GusVec3(0.0f,1.0f,0.0f))
{
    LoadIdentity4fv(MODEL);
    
}



SpaceObject::SpaceObject(Box_t& in_box):
    box(in_box),
    direction(GusVec3(0.0f,0.0f,1.0f)),
    up(GusVec3(0.0f,1.0f,0.0f))
{
    LoadIdentity4fv(MODEL);
    
}



SpaceObject SpaceObject::GetDummy()
{
	return SpaceObject(GusVec3(0.0,0.0,0.0),GusVec3(1.0,1.0,1.0));
}
