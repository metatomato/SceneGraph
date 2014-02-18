//
//  Logic.cpp
//  helloGL
//
//  Created by tomato on 10/16/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Logic.h"
#include "Cortex.h"
#include "../Scene/Scene.h"





Logic::Logic(const GusObject& obj):
    linkedObj(obj),
    actionMap(),
    currentAction(nullptr),
    nextAction(nullptr),
    receivedStimuli(),
    stimulated(false)
{
    
}


void Logic::Update()
{
    CheckStimuli();
    MakeDecision();
    DoAction();
    SendOrder();
    
    receivedStimuli.clear();
}


void Logic::DoAction()
{
    
    if(currentAction->GetPos() == Action::POS::BEGIN)
    {
        InitAction();
    }
    else if(currentAction->GetPos() == Action::POS::IN_PROGRESS)
    {
        currentAction->UpdateAction();
    }
}



void Logic::InitAction()
{
    currentAction->StartAction();
}




////////////////////////////////////////////////////////////////////////////
//          CAMERALOGIC METHOD IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////

map<PROG_TYPE, bool> CameraLogic::CameraStimuli = {{(int)Program::TYPE::DT_MOTION,true},
                                                   {(int)Program::TYPE::SELECTION,true}};

CameraLogic::CameraLogic(const GusObject& obj):Logic(obj),
    focused(false),
    sceneMode(true),
    objectMode(false),
    previousLoc(),
    goTo(nullptr),
    type(CONTROL_TYPE::CHARACTER)
{
    InitActionPipe();
}



void CameraLogic::InitActionPipe()
{
    actionMap[Action::ACTION::WALK] = new Walk();
    actionMap[Action::ACTION::WAIT] = new Wait();

    
    currentAction = actionMap[Action::ACTION::WAIT];
    
 }



void CameraLogic::MakeDecision()
{
    if(currentAction->GetPos() == Action::POS::END){
        ACT_TYPE action_type = currentAction->GetType();
        
        if(action_type == (int)Action::ACTION::WALK){
            currentAction = actionMap[Action::ACTION::WAIT];
    
        }
        
        if(action_type == (int)Action::ACTION::GOTO){
            focused = false;
            currentAction = actionMap[Action::ACTION::WAIT];
            type = CONTROL_TYPE::CAMERA_FOCUS;
            if(goTo->IsReversed())
            {
                type = CONTROL_TYPE::CHARACTER;
            }else
            {
                type = CONTROL_TYPE::CAMERA_FOCUS;
            }
        }
        
        if(action_type == (int)Action::ACTION::WAIT){

        }
        if (nextAction) {
            currentAction = nextAction;
            nextAction = nullptr;
        }
        currentAction->SetPos(Action::POS::BEGIN);
    }
    
    if (stimulated) {
        for (auto pair : receivedStimuli)
        {
            if(pair.first == (int)Program::TYPE::DT_MOTION)
            {
                if(sceneMode){
                    if(((const StageObject*)&(Scene::GetSelection())) != nullptr)
                    {
                        sceneMode = false;
                        objectMode = true;
                        
                        if(goTo) delete goTo;
                        goTo = new GoTo((StageObject&)linkedObj,(StageObject&)Scene::GetSelection(),ms(1000));
                        actionMap[Action::ACTION::GOTO] = goTo;
                        currentAction = goTo;
                        
                        previousLoc.InitArgsMap(((StageObject&)linkedObj).GetPosition(),((StageObject&)linkedObj).GetDirection());
                        
                        type = CONTROL_TYPE::AUTO_CONTROL;
                    }
                }else if (objectMode)
                {
                    objectMode = false;
                    sceneMode = true;
                    
                  // goTo = new GoTo(((StageObject&)linkedObj).GetPosition(),previousLoc.GetPos(),((StageObject&)linkedObj).GetDirection(),previousLoc.GetDir(),ms(1000));
                    goTo->SetPos(Action::POS::BEGIN);
                    goTo->SetInverse();
                    actionMap[Action::ACTION::GOTO] = goTo;
                    currentAction = goTo;
                    type = CONTROL_TYPE::AUTO_CONTROL;
                }
                
            }
            
            if(pair.first == (int)Program::TYPE::SELECTION)
            {
              //  Selection* select = (Selection*) receivedStimuli[(int)Program::TYPE::SELECTION].front();
             //   cout << "[CAMERALOGIC][MakeDecison] is Stimulated by selection " << select->GetId() << endl;
            }
        }
    }
}





void CameraLogic::CheckStimuli()
{
    stimulated = false ;
    
    for (auto pair : CameraStimuli)
    {
        if (Cortex::stimuli.count(pair.first) != 0 )
        {
            for (auto prog : Cortex::stimuli[pair.first])
            {
                receivedStimuli[pair.first].push_back(prog);
            }
            stimulated = true;
        }
    }
}




void CameraLogic::SendOrder()
{
    State* state = &(currentAction->GetState());
    
     //if (currentAction->GetType() == (int)Action::ACTION::GOTO) {
    //    type = CONTROL_TYPE::AUTO_CONTROL;
   // }
    const SceneCamera& obj = dynamic_cast<const SceneCamera&>(linkedObj);
    const_cast<SceneCamera&>(obj).BrainOrder(*state,type);
}


void CameraLogic::JumpInto(Action::ACTION act)
{
    if (act == Action::ACTION::GOTO)
    {
        GoTo* goTo = new GoTo((StageObject&)linkedObj,(StageObject&)Scene::GetSelection(),ms(1000));
        actionMap[Action::ACTION::GOTO] = goTo;
        currentAction = goTo;
        
  //      cout << "[CAMERALOGIC][JumpInto] pos FROM " << from_pos.Print()  << " TO " << to_pos.Print() << endl;
  //      cout << "[CAMERALOGIC][JumpInto] dir FROM " << from_dir.Print()  << " TO " << to_dir.Print() << endl;
        
    }
}





































































