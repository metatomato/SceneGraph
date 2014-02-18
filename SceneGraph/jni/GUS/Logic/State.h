//
//  State.h
//  helloGL
//
//  Created by tomato on 10/16/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__State__
#define __helloGL__State__

#include <iostream>
#include "../Common/GUS.h"
#include "../System/Program.h"
#include "../Scene/Control.h"

typedef int STATE_TYPE;

class State : public Program {
        public :
    static const map<string,STATE_TYPE>        NativeMap;
    static RefMap<&NativeMap>                  StateRefMap;
    
        public :
    
    enum class TYPE {STATE,MOTION,PAUSE,SPACE_LOC};
    
    State(State::TYPE);
    State(State::TYPE,const vector<ProgramArguments>&, tp_hd, chrono::milliseconds);
    State(string,const vector<ProgramArguments>&, tp_hd, chrono::milliseconds);
    
    virtual void                    InitArgsMap(const vector<ProgramArguments>&){};
    void                            InitType(string in_type);
    void                            InitType(State::TYPE in_type);
    
    STATE_TYPE                      GetStateType() const  {return stateType;};
    
    bool                            IsNew() const {return newState;};
    void                            SetNew(bool b){newState = b;};
    
        protected :
    STATE_TYPE                      stateType;
    bool                            newState;
};



class Motion : public State {
        public :
    enum class MOTION_ARGS{SPEED,DIR};
    
    Motion();
    Motion(const vector<ProgramArguments>&, tp_hd, chrono::milliseconds);
    
    virtual void                    InitArgsMap(const vector<ProgramArguments>&);
    
    float                           GetSpeed(){return (float)Args[(int)MOTION_ARGS::SPEED].floatValue();};
    CONTROL                         GetDir()  {return (CONTROL)Args[(int)MOTION_ARGS::DIR].intValue();};
    
    private :
    
};




class Pause : public State {
    public :
    enum class PAUSE_ARGS{DURATION};
    
    Pause();
    Pause(const vector<ProgramArguments>&, tp_hd, chrono::milliseconds);
    
    virtual void                    InitArgsMap(const vector<ProgramArguments>&);
    
    private :
    
};


class SpaceLocation : public State {
    public :
    enum class SPACE_LOC_ARGS{POS_X,POS_Y,POS_Z,DIR_X,DIR_Y,DIR_Z,UP_X,UP_Y,UP_Z};
    
    SpaceLocation();
    SpaceLocation(const GusVec3& pos,const GusVec3& dir, tp_hd, chrono::milliseconds);
    SpaceLocation(const vector<ProgramArguments>&, tp_hd, chrono::milliseconds);
    
    virtual void                    InitArgsMap(const vector<ProgramArguments>&);
            void                    InitArgsMap(const GusVec3& pos, const GusVec3& dir);
    
    GusVec3                         GetPos()
                                    {
                                        return GusVec3(Args[(int)SPACE_LOC_ARGS::POS_X].floatValue(),
                                                       Args[(int)SPACE_LOC_ARGS::POS_Y].floatValue(),
                                                       Args[(int)SPACE_LOC_ARGS::POS_Z].floatValue());
                                    };
    GusVec3                         GetDir()
                                    {
                                        return GusVec3(Args[(int)SPACE_LOC_ARGS::DIR_X].floatValue(),
                                                       Args[(int)SPACE_LOC_ARGS::DIR_Y].floatValue(),
                                                       Args[(int)SPACE_LOC_ARGS::DIR_Z].floatValue());
                                    };
    GusVec3                         GetUp()
                                    {
                                        return GusVec3(Args[(int)SPACE_LOC_ARGS::UP_X].floatValue(),
                                                       Args[(int)SPACE_LOC_ARGS::UP_Y].floatValue(),
                                                       Args[(int)SPACE_LOC_ARGS::UP_Z].floatValue());
                                    };
    private :
    
};











































#endif /* defined(__helloGL__State__) */
