//
//  ProgramFactory.h
//  helloGL
//
//  Created by tomato on 10/15/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__ProgramFactory__
#define __helloGL__ProgramFactory__

#include <iostream>
#include "../Common/GUS.h"
#include "../Logic/State.h"
#include "Program.h"

template <typename T>
class ProgramFactory{
        public :
    typedef     typename T::TYPE               TYPE;
    
static Program*                    CreateProgram();
static Program*                    CreateProgram(TYPE,const vector<ProgramArguments>&);
static Program*                    CreateProgram(TYPE,const vector<ProgramArguments>&, tp_hd tp);
static Program*                    CreateProgram(TYPE,const vector<ProgramArguments>&,tp_hd tp,chrono::milliseconds lt);
    
static Program*                    CreateProgram(string,const vector<ProgramArguments>&,tp_hd tp,chrono::milliseconds lt);
    

    
    static      void                 DestroyProgram(const Program&);
    
    static Program*                  InstanciateProgram(TYPE,const vector<ProgramArguments>&,tp_hd tp,chrono::milliseconds lt);
    static     void                  RegisterProgram(const Program&);
    

};
#endif /* defined(__helloGL__ProgramFactory__) */



