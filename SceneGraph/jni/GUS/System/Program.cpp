//
//  Program.cpp
//  helloGL
//
//  Created by tomato on 10/10/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Program.h"
#include "../Common/Time.h"

PROG_TYPE Program::LastId = -1;

const map<string,int> Program::nativeMap = map<string,int>{
    {"PROG",        (PROG_TYPE) TYPE::PROG},
    {"STATE",       (PROG_TYPE) TYPE::STATE},
    {"MOTION",      (PROG_TYPE) TYPE::MOTION},
    {"ST_MOTION",   (PROG_TYPE) TYPE::ST_MOTION},
    {"DT_MOTION",   (PROG_TYPE) TYPE::DT_MOTION},
    {"ANIMATION",   (PROG_TYPE) TYPE::ANIMATOR},
    {"TIMER",       (PROG_TYPE) TYPE::TIMER},
    {"SELECTION",   (PROG_TYPE) TYPE::SELECTION},
    {"INPUT",  	    (PROG_TYPE) TYPE::INPUT}
};

RefMap<&Program::nativeMap> Program::ProgramRef;

/*
PROG_TYPE Program::uniqueProgId = (int)Program::TYPE::END;
PROG_TYPE Program::LastId = -1;
map<string, PROG_TYPE>  Program::ProgramRefs = { {"PROG",     (PROG_TYPE) TYPE::PROG},
                                                 {"STATE",    (PROG_TYPE) TYPE::STATE},
                                                 {"MOTION",   (PROG_TYPE) TYPE::MOTION},
                                                 {"DT_MOTION",(PROG_TYPE) TYPE::DT_MOTION},
                                                 {"ANIMATION",(PROG_TYPE) TYPE::ANIMATOR} };

PROG_TYPE Program::SetProgramRef(string event_name){
    cout << "[PROG][SetProgramRef] "<< uniqueProgId << endl;
    if(ProgramRefs.count(event_name) == 0){

        ProgramRefs[event_name] = ++uniqueProgId;
    }
    return ProgramRefs[event_name];
}
*/
Program::Program():
    progId(++LastId),
    type((PROG_TYPE)TYPE::PROG),
    Args(),
    targetTime(Time::GetTime()),
    lifeTime(chrono::milliseconds(0))
{
    
}


Program::Program(TYPE in_type):
    progId(++LastId),
    type((PROG_TYPE)in_type),
    Args(),
    targetTime(Time::GetTime()),
    lifeTime(chrono::milliseconds(0))
{
    
}


Program::Program(TYPE in_type,const vector<ProgramArguments>& args):
    progId(++LastId),
    type((PROG_TYPE)in_type),
    Args(),
    targetTime(Time::GetTime()),
    lifeTime(chrono::milliseconds(0))
{
        InitArgsMap(args);
}



Program::Program(TYPE in_type,const vector<ProgramArguments>& args, tp_hd tp):
    progId(++LastId),
    type((PROG_TYPE)in_type),
    Args(),
    targetTime(tp),
    lifeTime(chrono::milliseconds(0))
{
    InitArgsMap(args);
}




Program::Program(TYPE in_type,const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime):
    progId(++LastId),
    type((PROG_TYPE) in_type),
    Args(),
    targetTime(tp),
    lifeTime(lifetime)
{
    InitArgsMap(args);
}



Program::Program(string type,const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime):
    progId(++LastId),
    type(0),
    Args(),
    targetTime(tp),
    lifeTime(lifetime)
{
    InitType(type);
    InitArgsMap(args);
}



void Program::InitType(string name_type)
{
    ProgramRef.SetType(name_type);
    type = ProgramRef.GetType(name_type);
    
    cout << "[PROG][SetProgramRef] New entry of object " << progId << " is "<< type << " for " << name_type << endl;
    cout << "                      targetTime" << targetTime.time_since_epoch().count() << " " << lifeTime.count() << endl;
}



void Program::InitType(TYPE in_type)
{
    type = (PROG_TYPE) in_type;
}



void Program::InitTimeArgs(tp_hd tp, chrono::milliseconds d)
{
    targetTime  = tp ;
    lifeTime    = d  ;
}



void Program::InitArgsMap(const vector<ProgramArguments>& args){
    int i = 0;
    for (auto arg : args) {
        Args[i] = arg;
        i++;
    }
}





//////////////////////////////////////////////////////////////////////////////////////
//  MOTIONPROG METHODS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////

MotionProgram::MotionProgram():Program(TYPE::MOTION)
{

}



MotionProgram::MotionProgram(float x, float y, float dx, float dy):Program(TYPE::MOTION)
{
    vector<ProgramArguments> temp = {{x},{y},{dx},{dy}};
    InitArgsMap(temp);
}



MotionProgram::MotionProgram(const vector<ProgramArguments>& args):Program(TYPE::MOTION)
{
    InitArgsMap(args);
}



MotionProgram::MotionProgram(const vector<ProgramArguments>& args, tp_hd tp):Program(TYPE::MOTION)
{
    InitTimeArgs(tp, chrono::milliseconds(0));
    InitArgsMap(args);
}



MotionProgram::MotionProgram(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lt):
Program(TYPE::MOTION)
{
    InitTimeArgs(tp, lt);
    InitArgsMap(args);
}


void MotionProgram::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 4) {
    Args[(int)MOTION_ARGS::X_VALUE]   = vec.at(0);
    Args[(int)MOTION_ARGS::Y_VALUE]   = vec.at(1);
    Args[(int)MOTION_ARGS::DX_VALUE]  = vec.at(2);
    Args[(int)MOTION_ARGS::DY_VALUE]  = vec.at(3);
    }else{
        cout << "[MOTIONPROG][MotionProgram] INITIALIZATION FAILED! MotionProgram needs 4 arguments" << endl;
    }
}



float MotionProgram::GetX()
{
    return Args[(int)MotionProgram::MOTION_ARGS::X_VALUE].floatValue();
}


float MotionProgram::GetY()
{
    return Args[(int)MotionProgram::MOTION_ARGS::Y_VALUE].floatValue();
}


float MotionProgram::GetDx()
{
    return Args[(int)MotionProgram::MOTION_ARGS::DX_VALUE].floatValue();
}


float MotionProgram::GetDy()
{
    return Args[(int)MotionProgram::MOTION_ARGS::DY_VALUE].floatValue();
}



//////////////////////////////////////////////////////////////////////////////////////
// SIMPLETAPMOTION METHODS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////





SimpleTapMotion::SimpleTapMotion():Program(TYPE::ST_MOTION)
{

}



SimpleTapMotion::SimpleTapMotion(float x, float y):Program(TYPE::ST_MOTION)
{
    InitArgsMap(vector<ProgramArguments>({x,y}));
    InitTimeArgs(Time::GetTime(),ms(0));
}



SimpleTapMotion::SimpleTapMotion(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime):Program(TYPE::ST_MOTION)
{
    InitArgsMap(args);
    InitTimeArgs(tp,lifetime);
}



void SimpleTapMotion::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 2) {
        Args[(int)ST_MOTION_ARGS::X_VALUE]   = vec.at(0);
        Args[(int)ST_MOTION_ARGS::Y_VALUE]   = vec.at(1);
    }else{
        cout << "[DTMOTION[InitArgsMap] INITIALIZATION FAILED! STMotionProgram needs 2 arguments" << endl;
    }
}




//////////////////////////////////////////////////////////////////////////////////////
//  DOUBLETAPMOTION METHODS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////





DoubleTapMotion::DoubleTapMotion()
{
    InitType(TYPE::DT_MOTION);
}



DoubleTapMotion::DoubleTapMotion(float x, float y)
{
    InitType(TYPE::DT_MOTION);
    InitArgsMap(vector<ProgramArguments>({x,y}));
}



DoubleTapMotion::DoubleTapMotion(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime)
{
    InitType(TYPE::DT_MOTION);
    InitArgsMap(args);
    InitTimeArgs(tp, lifetime);
}



void DoubleTapMotion::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 2) {
        Args[(int)DT_MOTION_ARGS::X_VALUE]   = vec.at(0);
        Args[(int)DT_MOTION_ARGS::Y_VALUE]   = vec.at(1);
    }else{
        cout << "[DTMOTION[InitArgsMap] INITIALIZATION FAILED! MotionProgram needs 2 arguments" << endl;
    }
}




//////////////////////////////////////////////////////////////////////////////////////
//  TIMER METHODS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////


Timer::Timer(tp_hd tp, chrono::milliseconds lt, int t_period):Program(TYPE::TIMER,vector<ProgramArguments>(),tp,lt)
{
    period = chrono::milliseconds(t_period);
}



Timer::Timer(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime):Program(TYPE::TIMER)
{
    SetTargetTime(tp);
    SetLifeTime(lifetime);
    InitArgsMap(args);
    period = chrono::milliseconds(Args[(int)TIMER_ARGS::PERIOD].intValue());
}

void Timer::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 1) {
            Args[(int)TIMER_ARGS::PERIOD]  = vec.at(0);
    }else{
        cout << "[TIMER][InitArgsMap] INITIALIZATION FAILED! MotionProgram needs 1 arguments" << endl;
    }
}






//////////////////////////////////////////////////////////////////////////////////////
//  HINTTIMER METHODS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////


HintTimer::HintTimer(tp_hd tp, chrono::milliseconds lt, int t_period):Program(TYPE::TIMER,vector<ProgramArguments>(),tp,lt)
{
    period = chrono::milliseconds(t_period);
}



HintTimer::HintTimer(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime):Program(TYPE::TIMER)
{
    InitArgsMap(args);
    period = chrono::milliseconds(Args[(int)Timer::TIMER_ARGS::PERIOD].intValue());
}

void HintTimer::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 2) {
        Args[(int)TIMER_ARGS::T_PERIOD]     = vec.at(0);
        Args[(int)TIMER_ARGS::HINT]         = vec.at(1);
    }else{
        cout << "[HINTTIMER][InitArgsMap] INITIALIZATION FAILED! MotionProgram needs 2 arguments" << endl;
    }
}





//////////////////////////////////////////////////////////////////////////////////////
//  SELECTION METHODS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////


Selection::Selection(vector<const GusObject*> vec_obj):Program(TYPE::SELECTION)
{
    vector<ProgramArguments> vec;
    vec.push_back(ProgramArguments((int)vec_obj.size()));
    for(auto obj : vec_obj)
    {
        vec.push_back(ProgramArguments(obj));
    }
    InitArgsMap(vec);
}



Selection::Selection(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime):Program(TYPE::SELECTION)
{
    InitArgsMap(args);
    InitTimeArgs(tp, lifetime);
}



void Selection::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() >= 2)
    {
        Args[(int)SELECTION_ARGS::COUNT]     = vec.at(0);
        int i = 1;
        for(auto obj : vec)
        {
            Args[i++]  = obj;
        }

    }else
    {
        cout << "[SELECTION][InitArgsMap] INITIALIZATION FAILED! Selection needs at least 2 arguments" << endl;
    }
}



vector<const GusObject*> Selection::GetSelection()
{
    vector<const GusObject*> vec;
    for(int i = 1 ; i < Args[(int)SELECTION_ARGS::COUNT].intValue(); i++)
    {
        vec.push_back(Args[1].cObjValue());
    }
    return vec;
}





//////////////////////////////////////////////////////////////////////////////////////
//  CONTROLPROG METHODS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////


InputProg::InputProg(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime):Program(TYPE::INPUT)
{
    InitArgsMap(args);
    InitTimeArgs(tp, lifetime);
}



void InputProg::InitArgsMap(const vector<ProgramArguments>& vec)
{
    if (vec.size() == 1)
    {
        Args[(int)INPUT_ARGS::TYPE] = vec.at(0);
        
    }else
    {
        cout << "[CONTROLPROG][InitArgsMap] INITIALIZATION FAILED! Selection needs 1 arguments" << endl;
    }
}





















