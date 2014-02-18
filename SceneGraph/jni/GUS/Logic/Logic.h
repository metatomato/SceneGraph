//
//  Logic.h
//  helloGL
//
//  Created by tomato on 10/16/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__Logic__
#define __helloGL__Logic__

#include <iostream>
#include "../Common/GUS.h"
#include "Action.h"
#include "../Common/Interpolator.h"


class Logic{
    public :
    typedef map<PROG_TYPE,deque<const Program*>> map_prog_type;
    
    Logic(const GusObject&);
    
    void                        DoAction();
    
    virtual void                InitAction();
    virtual void                InitActionPipe(){};
    virtual void                MakeDecision(){};
    virtual void                SendOrder(){};
    
    void                        Update();

    
    virtual void                CheckStimuli(){};
    
        protected :
    const GusObject&                    linkedObj;

    
    map<Action::ACTION, Action*>        actionMap;
    Action*                             currentAction;
    Action*                             nextAction;
    
    map_prog_type                       receivedStimuli;
    bool                                stimulated;
};


class CameraLogic : Logic{
        public :
    enum CAM_ACTION{WALK,WAIT,GOTO};
        private :
    static map<PROG_TYPE, bool> CameraStimuli;
    
        public :
    CameraLogic(const GusObject&);
    
    virtual void                InitActionPipe();
    virtual void                MakeDecision();
    virtual void                CheckStimuli();
    virtual void                SendOrder();
    
    void                        JumpInto(Action::ACTION);
    
    bool                        IsFocus(){return focused;};

    
        private :
    bool                        focused;
    bool                        objectMode;
    bool                        sceneMode;
    
    SpaceLocation               previousLoc;
    GoTo*                       goTo;
    CONTROL_TYPE                type;
};


#endif /* defined(__helloGL__Logic__) */







