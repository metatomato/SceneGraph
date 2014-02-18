//
//  CharacterControl.cpp
//  helloGL
//
//  Created by tomato on 2/24/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Control.h"



void CharacterControl :: ComputePos ( ) {
    if(fabs(dx) > fabs(dy)){
        pos().x = dx;
    } else {
        pos().y = dy;
    }
}

void CharacterControl :: ComputeDir ( ) {
    
}