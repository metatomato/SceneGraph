//
//  Interpolator.cpp
//  helloGL
//
//  Created by tomato on 10/9/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Interpolator.h"
#include "Time.h"

template <class T>
Interpolator<T>::Interpolator(T origin, T target, ms lifetime) :
    value(origin),
    valueOrigin(origin),
    valueTarget(target),
    slope((target - origin)/lifetime.count()),
    elapsedTime(0.f),
    lifeTime(lifetime),
    isRunning(false),
    epoch()
{
    
}



template <class T>
void Interpolator<T>::Start()
{
    epoch = Time::GetTime();
    isRunning = true;
}



template <class T>
void Interpolator<T>::Stop()
{
    isRunning = false;
}


template <class T>
T Interpolator<T>::GetValue(){
    if (isRunning)
    {
        elapsedTime += Time::GetDt();
        Interpolate(elapsedTime);
    }
    if (elapsedTime > lifeTime.count()) {
        value = valueTarget;
        isRunning = false;
    }
    return value;
}

template <class T>
void Interpolator<T>::Interpolate(float t)
{
    value = (slope * t ) + valueOrigin;
}



template class Interpolator<float>;
template class Interpolator<GusVec3>;