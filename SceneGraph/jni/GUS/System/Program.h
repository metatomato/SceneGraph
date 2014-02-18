//
//  Program.h
//  helloGL
//
//  Created by tomato on 10/10/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__Program__
#define __helloGL__Program__

#include <iostream>
#include "../Common/GUS.h"
#include "../Common/Time.h"
#include "InputRef.h"

struct ProgramArguments{

    union value
    {
        float               floatValue;
        int                 intValue;
        bool                boolValue;
        GusObject*          objValue;
        const GusObject*    cObjValue;
        
        value():                       intValue(0) {}
        value(float f):                floatValue(f) {}
        value(int i):                  intValue(i) {}
        value(bool b):                 boolValue(b) {}
        value(GusObject* obj):         objValue(obj) {}
        value(const GusObject* obj):   cObjValue(obj) {}
    } variable;
    
    enum TYPE{FLOAT, INT, BOOLEAN, OBJ, COBJ};
    
    TYPE type;
    
        ProgramArguments():                       variable(),   type(INT) {}
        ProgramArguments(float f):                variable(f),  type(FLOAT) {}
        ProgramArguments(int i):                  variable(i),  type(INT) {}
        ProgramArguments(bool b):                 variable(b),  type(BOOLEAN) {}
        ProgramArguments(GusObject* obj):         variable(obj),type(OBJ) {}
        ProgramArguments(const GusObject* obj):   variable(obj),type(COBJ) {}
    
    float               floatValue(){return variable.floatValue;}
    int                 intValue(){return variable.intValue;}
    bool                boolValue(){return variable.boolValue;}
    GusObject*          objValue(){return variable.objValue;}
    const GusObject*    cObjValue(){return variable.cObjValue;}
};


typedef int PROG_TYPE;

class Program
{
        public :
    static const map<string,int>    nativeMap;
    static RefMap<&nativeMap>       ProgramRef;
    
    /*
    static PROG_TYPE                                   GetType(string name){return ProgramRefs[name];};
    static PROG_TYPE                                   SetProgramRef(string event_name);
        private :
    static map<string, PROG_TYPE>                      ProgramRefs;
    static PROG_TYPE                                   uniqueProgId;
        */
        private :
    static PROG_TYPE                                   LastId;

    
        public :
    enum class TYPE{PROG, STATE, CONTROL, MOTION,ST_MOTION, DT_MOTION, ANIMATOR, TIMER, SELECTION, INPUT, END};
    
    Program();
    Program(TYPE);
    Program(TYPE,const vector<ProgramArguments>&);
    Program(TYPE,const vector<ProgramArguments>&, tp_hd);
    Program(TYPE,const vector<ProgramArguments>&, tp_hd, chrono::milliseconds);
    Program(string,const vector<ProgramArguments>&, tp_hd, chrono::milliseconds);
    
        int                         GetId() const                       {return progId;}
    PROG_TYPE                       GetType() const                     {return type;};
    tp_hd                   GetTargetTime() const               {return targetTime;};
    tp_hd                   GetTargetTime()                     {return targetTime;};
    chrono::milliseconds            GetLifeTime() const                 {return lifeTime; };
    void                            SetTargetTime(tp_hd tp)     {targetTime = tp;};
    void                            SetLifeTime(chrono::milliseconds d) {lifeTime = d; };
    
    
    
        protected :
    void                            InitArgsMap(const vector<ProgramArguments>&);
    void                            InitTimeArgs(tp_hd tp, chrono::milliseconds d);
    virtual void                    InitType(string in_type);
    virtual void                    InitType(TYPE in_type);
    
    
    int                                        progId;
    
    PROG_TYPE                                  type;
    map<int, ProgramArguments>                 Args;
    
    tp_hd                              targetTime;
    chrono::milliseconds                       lifeTime;
    
    
    
};




class MotionProgram : public Program
{
        public :
    enum class MOTION_ARGS{X_VALUE,Y_VALUE,DX_VALUE,DY_VALUE};

    MotionProgram();
    MotionProgram(float x, float y, float dx, float dy);
    MotionProgram(const vector<ProgramArguments>&);
    MotionProgram(const vector<ProgramArguments>& args, tp_hd tp);
    MotionProgram(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime);
    
    float                       GetX();
    float                       GetY();
    float                       GetDx();
    float                       GetDy();

        protected :
    void                        InitArgsMap(const vector<ProgramArguments>&);

    
};




class SimpleTapMotion : public Program
{
        public :
    enum class ST_MOTION_ARGS{X_VALUE,Y_VALUE};

    SimpleTapMotion();
    SimpleTapMotion(float x, float y);
    SimpleTapMotion(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime);

    float                       GetX() const {return Args[(int)ST_MOTION_ARGS::X_VALUE].floatValue();};
    float                       GetY() const {return Args[(int)ST_MOTION_ARGS::Y_VALUE].floatValue();};;

        protected:
    void                        InitArgsMap(const vector<ProgramArguments>& vec);
};



class DoubleTapMotion : public MotionProgram
{
        public :
    enum class DT_MOTION_ARGS{X_VALUE,Y_VALUE};
    
    DoubleTapMotion();
    DoubleTapMotion(float x, float y);
    DoubleTapMotion(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime);
    
        protected:
    void                        InitArgsMap(const vector<ProgramArguments>& vec);
};





class Timer : public Program {
    
        public :
    enum class TIMER_ARGS{PERIOD};
    
    Timer(tp_hd tp, chrono::milliseconds lifetime, int t_period);
    Timer(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime);
    
    chrono::milliseconds        GetPeriod() const {return period;};
    
        protected :
     void                       InitArgsMap(const vector<ProgramArguments>& vec);
    
    chrono::milliseconds         period;
    
};



class HintTimer : public Program {
    
        public :
    enum class TIMER_ARGS{T_PERIOD,HINT};
    
    HintTimer(tp_hd tp, chrono::milliseconds lifetime,int t_period);
    HintTimer(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime);
        private :
    void                         InitArgsMap(const vector<ProgramArguments>& vec);
    chrono::milliseconds         period;
};




class Selection : public Program
{
        public :
    enum class SELECTION_ARGS{COUNT,OBJ};
    
    Selection(vector<const GusObject*>);
    Selection(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime);
    
        protected:
    void                        InitArgsMap(const vector<ProgramArguments>& vec);
    
    vector<const GusObject*>    GetSelection();
};




class InputProg : public Program
{
    public :
    enum class INPUT_ARGS{TYPE};
    
    InputProg(INPUT_REF input_type);
    InputProg(const vector<ProgramArguments>& args, tp_hd tp, chrono::milliseconds lifetime);
    
protected:
    void                        InitArgsMap(const vector<ProgramArguments>& vec);
    
    
};






















































#endif /* defined(__helloGL__Program__) */
