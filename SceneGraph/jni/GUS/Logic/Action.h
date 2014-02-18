//
//  Action.h
//  helloGL
//
//  Created by tomato on 10/16/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__Action__
#define __helloGL__Action__

#include <iostream>
#include "../Common/GUS.h"
#include "State.h"
#include "../Common/Interpolator.h"

typedef int ACT_TYPE;

class Action {
        public :
    typedef vector<State::TYPE>   state_list;
    enum class POS{BEGIN,IN_PROGRESS,END};
    
    static const map<string,ACT_TYPE>    NativeMap;
    static RefMap<&NativeMap>            ActionRefMap;
    
    static state_list                    StateList;
    
        public :
    enum class ACTION{ACTION,WALK,WAIT,GOTO};
    
    Action();
    ~Action();
    Action(ACTION);
    Action(string);
    
    State&                 GetState();
    virtual void           StartAction();
    void                   UpdateAction();
    void                   EndAction();
    void                   NextState();
    
    void                   SetPos(POS pos)  {position = pos;};
    POS                    GetPos();
    ACT_TYPE               GetType();
    
        protected :
    virtual void           InitStates(){};
    virtual void           UpdateState(){};
    void                   CalculTargetTime();
    
    deque<State*>                       statePipe;
    deque<State*>                       activePipe;
    
    State*                              currentState;
    POS                                 position;
    
    tp_hd                       epoch;
    
        private :
    ACT_TYPE                            type;
    
    
};



class Walk : public Action {
        public :
    static const state_list         WalkStateList;
        public :
    Walk();
    
        protected :
    virtual void           InitStates();
    
        private :

    
};



class Wait : public Action {
    public :
    static const state_list         WaitStateList;
    public :
    Wait();
    
    protected :
    virtual void           InitStates();
    
    private :
    
    
};




class GoTo : public Action {
        public :
    static const state_list         GoToStateList;
    
        public :
    GoTo();
    GoTo(const GusVec3& from_pos, const GusVec3& to_pos,const GusVec3& from_dir, const GusVec3& to_dir, chrono::milliseconds in_ms);
    GoTo(const StageObject& from_obj, const StageObject& to_obj, ms in_ms);
    
    void                    SetInverse(){playInverse = true;};
    bool                    IsReversed(){return playInverse;};
    
        protected :
    virtual void            InitStates();
    virtual void            UpdateState();
    
    virtual void            StartAction();
    
    void                    InitPath(const GusVec3& from, const GusVec3& to);
    
    private :
    GusVec3                         fromPos;
    GusVec3                         toPos;
    GusVec3                         fromDir;
    GusVec3                         toDir;
    ms                              duration;
    Interpolator<GusVec3>*          interpolPos;
    Interpolator<GusVec3>*          interpolDir;
    
    const StageObject*              destinationObj;
    bool                            playInverse;
};







































#endif /* defined(__helloGL__Action__) */
