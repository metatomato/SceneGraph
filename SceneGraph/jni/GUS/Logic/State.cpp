//
//  State.cpp
//  helloGL
//
//  Created by tomato on 10/16/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "State.h"



const map<string, STATE_TYPE>  State::NativeMap = {     {"STATE",          (STATE_TYPE) TYPE::STATE},
                                                        {"MOTION",         (STATE_TYPE) TYPE::MOTION},
                                                        {"WAIT",           (STATE_TYPE) TYPE::PAUSE}  };

RefMap<&State::NativeMap>      State::StateRefMap;



State::State(State::TYPE in_type):Program(Program::TYPE::STATE),
    newState(true)
{
    InitType(in_type);
}



State::State(State::TYPE in_type,const vector<ProgramArguments>& args, tp_hd tt, chrono::milliseconds lt):Program(Program::TYPE::STATE),
    newState(true)
{
    InitType(in_type);
    InitTimeArgs(tt, lt);
    Program::InitArgsMap(args);
}



State::State(string in_type,const vector<ProgramArguments>& args, tp_hd tt, chrono::milliseconds lt):Program(Program::TYPE::STATE),
    newState(true)
{
    InitType(in_type);
    InitTimeArgs(tt, lt);
    Program::InitArgsMap(args);
}



void State::InitType(string name_type)
{
    StateRefMap.SetType(name_type);
    stateType = StateRefMap.GetType(name_type);
}



void State::InitType(TYPE in_type)
{
    stateType = (STATE_TYPE) in_type;
}




//////////////////////////////////////////////////////////////////////////////
//          MOTION METHOD DEFINITIONS
//////////////////////////////////////////////////////////////////////////////



Motion::Motion():State(TYPE::MOTION)
{

}



Motion::Motion(const vector<ProgramArguments>& args, tp_hd tt, chrono::milliseconds lt):State(TYPE::MOTION)
{
    InitTimeArgs(tt, lt);
    InitArgsMap(args);
}


void Motion::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 2) {
        Args[(int)MOTION_ARGS::SPEED]   = vec.at(0);
        Args[(int)MOTION_ARGS::DIR]     = vec.at(1);
    }else{
        cout << "[MOTION][InitArgsMap] INITIALIZATION FAILED! MoveForward needs 2 argument(s)" << endl;
    }

}




//////////////////////////////////////////////////////////////////////////////
//          PAUSE METHOD DEFINITIONS
//////////////////////////////////////////////////////////////////////////////



Pause::Pause():State(TYPE::PAUSE)
{
        InitType(TYPE::PAUSE);
}



Pause::Pause(const vector<ProgramArguments>& args, tp_hd tt, chrono::milliseconds lt):State(TYPE::PAUSE)
{
    InitType(TYPE::PAUSE);
    InitTimeArgs(tt, lt);
    InitArgsMap(args);
}


void Pause::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 1) {
        Args[(int)PAUSE_ARGS::DURATION] = vec.at(0);
    }else{
        cout << "[WAIT][InitArgsMap] INITIALIZATION FAILED! Wait needs 1 argument(s)" << endl;
    }
    
}





//////////////////////////////////////////////////////////////////////////////
//          SPACELOCATION METHOD DEFINITIONS
//////////////////////////////////////////////////////////////////////////////



SpaceLocation::SpaceLocation():State(TYPE::SPACE_LOC)
{
    
}



SpaceLocation::SpaceLocation(const GusVec3& pos,const GusVec3& dir, tp_hd tp, chrono::milliseconds lt):State(TYPE::SPACE_LOC)
{
    InitArgsMap(pos,dir);
    InitTimeArgs(tp, lt);
}




SpaceLocation::SpaceLocation(const vector<ProgramArguments>& args, tp_hd tt, chrono::milliseconds lt):State(TYPE::SPACE_LOC)
{
    InitTimeArgs(tt, lt);
    InitArgsMap(args);
}



void SpaceLocation::InitArgsMap(const GusVec3& pos, const GusVec3& dir)
{
    vector<ProgramArguments> vec = {pos.x,pos.y,pos.z,dir.x,dir.y,dir.z,0.f,1.f,0.f};
    InitArgsMap(vec);
}



void SpaceLocation::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 9) {
        Args[(int)SPACE_LOC_ARGS::POS_X]    = vec.at(0);
        Args[(int)SPACE_LOC_ARGS::POS_Y]    = vec.at(1);
        Args[(int)SPACE_LOC_ARGS::POS_Z]    = vec.at(2);
        Args[(int)SPACE_LOC_ARGS::DIR_X]    = vec.at(3);
        Args[(int)SPACE_LOC_ARGS::DIR_Y]    = vec.at(4);
        Args[(int)SPACE_LOC_ARGS::DIR_Z]    = vec.at(5);
        Args[(int)SPACE_LOC_ARGS::UP_X]     = vec.at(6);
        Args[(int)SPACE_LOC_ARGS::UP_Y]     = vec.at(7);
        Args[(int)SPACE_LOC_ARGS::UP_Z]     = vec.at(8);
        
    }else{
        cout << "[MOTION][InitArgsMap] INITIALIZATION FAILED! MoveForward needs 9 argument(s)" << endl;
    }
    
}





























