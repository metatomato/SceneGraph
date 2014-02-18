//
//  GusGLObject.cpp
//  helloGL
//
//  Created by tomato on 9/19/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "GusObject.h"
#include "System/Program.h"
#include "Logic/State.h"

int GusObject::_lastId = -1;


void GusObject::OnTV(const Program& prog)
{
    cout << "[GUSOBJECT][OnTv] Obj "<< _GetId() << " is watching TV!" << endl;
    cout << "prog " << prog.GetId() << " on chanel " << prog.GetType() <<endl;
};



void GusObject::BrainOrder(const State& state) 
{
    cout << "[GUSOBJECT][BrainOrder] state type "<< state.GetStateType();
    if (state.GetStateType() == (int)State::TYPE::MOTION) {
        cout << " SPEED "<< ((Motion&)state).GetSpeed();
    }
     cout << endl;
}
