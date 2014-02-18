//
//  ProgramFactory.cpp
//  helloGL
//
//  Created by tomato on 10/15/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "ProgramFactory.h"
#include "GusTV.h"
#include "../Common/Time.h"


//TYPE : PROG, MOTION, ANIMATOR
template <class T>
Program* ProgramFactory<T>::CreateProgram()
{
    Program* prog = new Program();
    return prog;
}



template <class T>
Program* ProgramFactory<T>::CreateProgram(TYPE type, const vector<ProgramArguments>& args){
    return CreateProgram(type,args,Time::GetTime(), chrono::milliseconds(0));
}



template <class T>
Program* ProgramFactory<T>::CreateProgram(TYPE type,const vector<ProgramArguments>& args, tp_hd tp)
{
    return CreateProgram(type,args,tp, chrono::milliseconds(0));
}



template <class T>
Program* ProgramFactory<T>::CreateProgram(TYPE type,const vector<ProgramArguments>& args,tp_hd tp,chrono::milliseconds lt)
{
    Program* prog = InstanciateProgram(type,args,tp,lt);
    
    RegisterProgram(*prog);
    
    return prog;
}



template <class T>
Program* ProgramFactory<T>::CreateProgram(string type_name,const vector<ProgramArguments>& args,tp_hd tp,chrono::milliseconds lt)
{
    Program* prog = new Program(type_name,args,tp,lt);
    
    RegisterProgram(*prog);

    return prog;
}


template <class T>
void ProgramFactory<T>::DestroyProgram(const Program& prog)
{
    delete &prog;
}



////////////////////////////////////////////////////////////////////////////////////////////////
//      TEMPLATE FUNCTIONS SPECIALIZATION
////////////////////////////////////////////////////////////////////////////////////////////////



template<>
Program* ProgramFactory<Program>::InstanciateProgram(TYPE type,const vector<ProgramArguments>& args,tp_hd tp,chrono::milliseconds lt){
    
    Program* prog = nullptr;
    
    if (type == TYPE::PROG) {
        prog = new Program(type, args, tp, lt);
    }
    if (type == TYPE::MOTION) {
        prog = new MotionProgram(args, tp, lt);
    }
    if (type == TYPE::ST_MOTION) {
            prog = new SimpleTapMotion(args, tp, lt);
        }
    if (type == TYPE::DT_MOTION) {
        prog = new DoubleTapMotion(args, tp, lt);
    }
    if (type == TYPE::ANIMATOR) {
        prog = new Program(type, args, tp, lt);
    }
    if (type == TYPE::TIMER) {
        prog = new Timer(args, tp, lt);
    }
    if (type == TYPE::SELECTION) {
        prog = new Selection(args, tp, lt);
    }
    if (type == TYPE::INPUT) {
        prog = new Selection(args, tp, lt);
    }
    
    return prog;
}



template <>
void ProgramFactory<Program>::RegisterProgram(const Program& prog)
{
    GusTV::programs[prog.GetId()] = &prog;
}



template<>
Program* ProgramFactory<State>::InstanciateProgram(TYPE type,const vector<ProgramArguments>& args,tp_hd tp,chrono::milliseconds lt){
    
    Program* prog = nullptr;
    
    if (type == TYPE::STATE) {
        prog = new State(type, args, tp, lt);
    }
    if (type == TYPE::MOTION) {
        prog = new Motion(args, tp, lt);
    }
    if (type == TYPE::PAUSE) {
        prog = new Pause(args, tp, lt);
    }
    if (type == TYPE::SPACE_LOC) {
        prog = new SpaceLocation(args, tp, lt);
    }
    
    return prog;
}


template <>
void ProgramFactory<State>::RegisterProgram(const Program& prog)
{
    
}


template class ProgramFactory<Program>;

template class ProgramFactory<State>;





































