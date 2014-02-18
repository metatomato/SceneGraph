//
//  Control.cpp
//  helloGL
//
//  Created by tomato on 2/6/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Control.h"
#include "../Common/Matrix.h"

float Control::scale = 1.0f;
float Control::dx = 0.0f;
float Control::dy = 0.0f;
float Control::x = 0.0f;
float Control::y = 0.0f;
float Control::localdX = 0.0f;
float Control::localdY = 0.0f;
float Control::localX = 0.0f;
float Control::localY = 0.0f;
float Control::localdPhi = 0.0f;
float Control::localdTheta = 0.0f;


Control :: Control (SpaceObject& obj) : objTarget(obj)
{
    InitControlState() ;
    
    localdX = 0.0f;
    localdY = 0.0f;
    localX = 0.0f;
    localY = 0.0f;
}



void Control :: InitControlState () {
    for (int i = 0; i < CONTROL_MAX; i++) {
        controlState[i] = false;
    }
}



void Control::GetInput(){
    localdX = dx;
    localdY = dy;
    localX = x;
    localY = y;
    localdPhi = dx;
    localdTheta = dy;
}



void Control :: ComputePos ( )
{
}



void Control :: ComputeDir ( )
{
}


void Control::SetControlState(CONTROL dir)
{
    for(int i = 0 ; i < CONTROL_MAX ; i++)
    {
        if (i == dir) {
            controlState[i] = true;
        }else
        {
            controlState[i] = false;
        }
    }
}
