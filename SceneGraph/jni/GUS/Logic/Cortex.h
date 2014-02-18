//
//  Cortex.h
//  helloGL
//
//  Created by tomato on 10/13/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__Cortex__
#define __helloGL__Cortex__

#include <iostream>
#include "../Common/GUS.h"
#include "Logic.h"
#include "../System/ChannelOffice.h"

class Program;

class Cortex {
         public :
    typedef map<PROG_TYPE,deque<const Program*>> map_prog_type;
    
    Cortex(){};
    
    static void            MakeConnection(const GusObject&);
    static void            DisconnectAll();
    static void            Disconnect(const GusObject&);
    static void            Stimulate();
    
    static void            ConnectChannel(ChannelOffice::CHANNEL);
    static void            RecieveStimulus(const Program*);
    static void            InitNervousSystem();
    
    static bool            firstInit;
    
        private :
    static  map<const GusObject*, Logic*>          neuronalConnection;
    
    static map_prog_type                           stimuli;
    
    
    friend class Logic;
    friend class CameraLogic;
    

    
};




#endif /* defined(__helloGL__Cortex__) */
