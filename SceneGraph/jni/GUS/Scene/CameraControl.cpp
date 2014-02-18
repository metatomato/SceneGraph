//
//  CameraControl.cpp
//  helloGL
//
//  Created by tomato on 2/24/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Control.h"
#include "../Common/Matrix.h"
#include "../Renderer/Entity.h"


SpaceObject& CameraControl::dummyTarget = *(new SpaceObject(SpaceObject::GetDummy()));

CameraControl::CameraControl(SceneCamera& obj):Control((SpaceObject&)obj),
    angle(M_PI)
{
    ComputeInverse();
}


GusVec3 CameraControl :: GetPosition(){
    UpdateControl();
    ComputePos();
    return pos();
}
GusVec3 CameraControl :: GetDirection(){
    UpdateControl();
    ComputeDir();
    return dir();
}



void CameraControl :: UpdateControl() {
    if(fabs(localdX) > fabs(localdY)){
        localdY = 0.0f ;
        if( localY < BOTTOM_LIMIT ) {
           angle -= 0.01f * localdX;
        }
    } else {
        localdX = 0.0f ;
    } 
    
    if( localY > BOTTOM_LIMIT && localdX < 0.0f ) {
        controlState[STRIFE_L] = true;
        controlState[STRIFE_R] = false;
    } else if( localY > BOTTOM_LIMIT && localdX > 0.0f ) {
        controlState[STRIFE_R] = true;
        controlState[STRIFE_L] = false;
    } else {
        controlState[STRIFE_L] = false;
        controlState[STRIFE_R] = false;
    }
}



void CameraControl :: ComputePos ( ) {
    
    if (controlState[STRIFE_L] || controlState[STRIFE_R]) {
            pos().x -= 0.5 * localdX * cosf(angle);
            pos().z += 0.5 * localdX * sinf(angle);
    } else {
        pos().x += 0.5 * -1 * localdY * sinf(angle);
        pos().z += 0.5 * -1 * localdY * cosf(angle);
    }
    pos().y += (1.0f - scale)*100.0f;

}



void CameraControl :: ComputeDir ( )
{
    dir().x = sinf(angle) + pos().x;
    dir().z = cosf(angle) + pos().z;
    //dir().x = INF_NEG * sinf(angle);
    //dir().z = INF_NEG * cosf(angle);
    dir().y = pos().y;
}



void CameraControl::FocusOn(const SpaceObject& obj){
	SpaceObject* so;
	if(&obj){
		so = &obj;
	}else {
		LOGE("[CAMERACONTROL][FocusOn] Empty SpaceObject, FocusOn Dummy Target");
		so = &dummyTarget;
	}

		float z = GetFocusLength(*so);
		GusVec3 t_pos = so->GetPosition();
		pos() = GusVec3(t_pos.x,t_pos.y ,t_pos.z + z);
		dir() = GusVec3(t_pos.x, t_pos.y , t_pos.z + z + 1.f);
		angle = 0.f;
		//ComputePos();
		//ComputeDir();
		cout << "cameraPos " << pos().Print() << endl;
		cout << "cameraDir " << dir().Print() << endl;
		//angle = 0.f;

}



float CameraControl::GetFocusLength(const SpaceObject& obj)
{
	SpaceObject* target = &obj;

	if(!target){
		target = &dummyTarget;
	}


    GusVec3 hv = target->GetHalfVec();
    GLfloat z = max(hv.x / (Renderer::aspectRatio * tan(Renderer::fov/2.f)), hv.y/tan(Renderer::fov/2.f)) + hv.z;
    z *= 0.7f;
    return z;
}



void CameraControl::ComputeInverse()
{
    
    float z =   dir().z - pos().z;
    float x =   dir().x - pos().x;
    float length = sqrtf(x * x + z * z);
    angle = acosf(z/length);
    if(x < 0){ angle = 2 * M_PI - angle;}
    up() = GusVec3(0.f,1.f,0.f);
    
  //  cout << "[CAMERACONTROL][ComputeInverse] angle " << angle << " degre " << (int)(angle * M_1_PI * 180) % 360 << endl;
  //  cout <<                                        " pos " << pos().Print() << " dir " << dir().Print() << endl;
}



void CameraControl::Move(float speed, CONTROL dir)
{
    SetControlState(dir);
    
    if (dir == FORWARD)
    {
        localdY -=  speed * 0.1f;
        localdX =  0.f;
    }
    if (dir == BACKWARD)
    {
        localdY +=  speed * 0.1f;
        localdX =  0.f;
    }
    if (dir == STRIFE_L)
    {
        localdX +=  speed * 0.1f;
        localdY =  0.f;
    }
    if (dir == STRIFE_R)
    {
        localdX -=  speed * 0.1f;
        localdY =  0.f;
    }

    ComputePos();
}


///////////////////////////////////////////////////
//  CAMERAFOCUSCONTROL CLASS METHOD DEFINITOINS
//////////////////////////////////////////////////

//Spherical coordinates system origin is centered on the focused object!!
//Then cartesian cordinates are translated from object space to world global space


CameraFocusControl::CameraFocusControl(SceneCamera& obj):CameraControl(obj),
    focusPoint(GusVec3()),
    objectFocused(NULL),
    theta(M_PI_2),
    phi(0.f),
    rho(5.f)
{
}



CameraFocusControl::CameraFocusControl(SceneCamera& obj, const SpaceObject& focused):CameraControl(obj),
    focusPoint(focused.GetPosition()),
    objectFocused(&obj),
    theta(M_PI_2),
    phi(0.f),
    rho(5.f)
{
    FocusOn(*objectFocused);
    rho = pos().z;
}



GusVec3 CameraFocusControl::GetPosition(){
    UpdateControl();
    ComputePos();
    return pos();
}



GusVec3 CameraFocusControl::GetDirection(){
    UpdateControl();
    ComputeDir();
    return dir();
}



GusVec3 CameraFocusControl::GetUp() {
    UpdateControl();
    ComputeUp();
    return up();
}



void CameraFocusControl::UpdateControl()
{
    theta += 0.01f * localdY;
    phi   += 0.01f * localdX;
    rho   += (1.0f - scale)*100.0f;
}



void CameraFocusControl::ComputePos()
{
    pos().x = rho * sinf(theta) * sinf(phi) + focusPoint.x;
    pos().y = rho * cosf(theta)             + focusPoint.y;
    pos().z = rho * sinf(theta) * cosf(phi) + focusPoint.z;
}



void CameraFocusControl::ComputeDir()
{
    dir() = focusPoint;
}



void CameraFocusControl::ComputeUp()
{
    up().x = - cosf(theta) * sinf(phi);
    up().y = sinf(theta);
    up().z = - cosf(theta) * cosf(phi);
}



void CameraFocusControl::ComputeInverse()
{
    //rho = pos().Lenght();
    //phi = acosf(pos().z/rho);
    //theta = acosf(pos().y/rho);
    
   // rho = pos().z - focusPoint.z;
   // phi = 0.f;
   // theta = M_PI_2;
    
    GusVec3 vec =  pos() - focusPoint;
    rho = (vec).Length();
    theta = acosf(vec.y/rho);
    phi = acosf(vec.z/(rho*sinf(theta)));
    if (pos().x < 0) {
        phi = 2 * M_PI - phi;
    }

}



void CameraFocusControl::SetFocusOn(const SpaceObject& obj)
{
    objectFocused = &obj;
    //FocusOn(obj);
    focusPoint = obj.GetPosition();
    ComputeInverse();
}


void CameraFocusControl::SetAutoFocusOn(const SpaceObject* obj)
{
	const SpaceObject* target;

	if(obj) {
		target = obj;
		LOGE("[CAMERACONTROL][SetAutoFocus] Target EXIST!");
	}else {
		target = &dummyTarget;
		LOGE("[CAMERACONTROL][SetAutoFocus] Target DOES NOT EXIST!");
	}
	objectFocused = target;
    FocusOn(*target);
    focusPoint = target->GetPosition();
    ComputeInverse();
}






///////////////////////////////////////////////////
//  CAMERAFREECONTROL CLASS METHOD DEFINITOINS
//////////////////////////////////////////////////



CameraFreeControl::CameraFreeControl(SceneCamera& obj):CameraControl(obj),
theta(M_PI_2),
phi(M_PI)
{
    ComputeInverse();
}


GusVec3 CameraFreeControl::GetPosition(){
    UpdateControl();
    ComputePos();
    return pos();
}



GusVec3 CameraFreeControl::GetDirection(){
    UpdateControl();
    ComputeDir();
    return dir();
}



GusVec3 CameraFreeControl::GetUp() {
    UpdateControl();
    ComputeUp();
    return up();
}



void CameraFreeControl::UpdateControl()
{
    if(localX < (Renderer::frameView[0] / 2) )
    {
        if(fabs(localdX) > fabs(localdY))
        {
            localdY = 0.0f ;
        } else
        {
            localdX = 0.0f ;
        }
        
        if( localY > 600.0f && localdX < 0.0f ) {
            controlState[STRIFE_L] = true;
            controlState[STRIFE_R] = false;
        } else if( localY > 600.0f && localdX > 0.0f ) {
            controlState[STRIFE_R] = true;
            controlState[STRIFE_L] = false;
        } else {
            controlState[STRIFE_L] = false;
            controlState[STRIFE_R] = false;
        }
    }else
    {
        if(fabs(localdX) > fabs(localdY))
        {
            localdTheta = 0.0f ;
            phi -= 0.01f * localdPhi;
        }else
        {
            localdPhi = 0.0f ;
            theta -= 0.01f * localdTheta;
        }
        localdY = 0.f;
        localdX = 0.f;
    }
}



void CameraFreeControl::ComputePos()
{
    if (controlState[STRIFE_L] || controlState[STRIFE_R])
    {
        pos().x -= 0.5 * localdY * sinf(theta) * sinf(phi);;
        pos().z -= 0.5 * localdY * sinf(theta) * cosf(phi);
    } else
    {
        pos().x -= 0.5 * localdY * sinf(theta) * sinf(phi);
        pos().z -= 0.5 * localdY * sinf(theta) * cosf(phi);
        pos().y -= 0.5 * localdY * cosf(theta);
    }
}



void CameraFreeControl::ComputeDir()
{
    dir().x = sinf(theta) * sinf(phi)  + pos().x;
    dir().z = sinf(theta) * cosf(phi)  + pos().z;
    dir().y = cosf(theta)              + pos().y;
}



void CameraFreeControl::ComputeUp()
{
    up().x = - cosf(theta) * sinf(phi);
    up().y = sinf(theta);
    up().z = - cos(theta) * cosf(phi);
}



void CameraFreeControl::ComputeInverse()
{
    GusVec3 vec =  dir() - pos();
    GLfloat rho = (vec).Length();
    theta = acosf(vec.y/rho);
    phi = acosf(vec.z/(rho*sinf(theta)));
}
















































































