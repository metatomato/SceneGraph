//
//  Interpolator.h
//  helloGL
//
//  Created by tomato on 10/9/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__Interpolator__
#define __helloGL__Interpolator__

#include <iostream>
#include "GUS.h"
#include "Time.h"



template<class T>
class Interpolator {
    
        public :
    Interpolator(T origin, T target, ms lifetime);
    
    void                    Start();
    void                    Stop();
    T                       GetValue();
    void                    Interpolate(float dt);
    float                   GetElapsedTime() {return elapsedTime;};
    T                       GetTarget(){return valueTarget;};
    
        private :
    T                       value;
    T                       valueOrigin;
    T                       valueTarget;
    T                       slope;
    float                   elapsedTime;
    ms                      lifeTime;
    bool                    isRunning;

    
    tp_hd                   epoch;
    
};






#endif /* defined(__helloGL__Interpolator__) */
