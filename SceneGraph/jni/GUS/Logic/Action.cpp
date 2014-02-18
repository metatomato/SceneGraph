//
//  Action.cpp
//  helloGL
//
//  Created by tomato on 10/16/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Action.h"
#include "../System/ProgramFactory.h"
#include "../Common/Time.h"

const map<string,ACT_TYPE>           Action::NativeMap = map<string,ACT_TYPE>({{"ACTION", (ACT_TYPE)ACTION::ACTION},
                                                                               {"WALK",   (ACT_TYPE)ACTION::WALK},
                                                                               {"WAIT",   (ACT_TYPE)ACTION::WAIT} });
RefMap<&Action::NativeMap>           Action::ActionRefMap;

Action::state_list                   Action::StateList;


Action::Action() :
    statePipe(),
    activePipe(),
    type((int)ACTION::ACTION),
    currentState(nullptr),
    position(POS::BEGIN)
{
    
}



Action::~Action(){
    
}



Action::Action(ACTION in_type):
    statePipe(),
    activePipe(),
    currentState(nullptr),
    position(POS::BEGIN),
    epoch(),
    type((int)in_type)
{
    
}


Action::Action(string action_name):
    statePipe(),
    activePipe(),
    type(0),
    currentState(nullptr),
    position(POS::BEGIN)
{
    ActionRefMap.SetType(action_name);
    type = ActionRefMap.GetType(action_name);
}



State& Action::GetState()
{
    return *currentState;
}



void Action::StartAction()
{
    
    epoch = Time::GetTime();
    currentState = statePipe.front();
    currentState->SetTargetTime(epoch);
    currentState->SetNew(true);
    CalculTargetTime();
    
    position = POS::IN_PROGRESS;
}



void Action::UpdateAction()
{
    tp_hd t = Time::GetTime();
    tp_hd tt_state = currentState->GetTargetTime();
    tp_hd end_t = tt_state + currentState->GetLifeTime();
    
    
    if ( tt_state >= epoch )  {position = POS::IN_PROGRESS;}
    
    UpdateState();
    currentState->SetNew(false);
    
    if ( t > end_t )
    {
        NextState();
    }
    
    
}



void Action::EndAction()
{
    position = POS::END;
}



void Action::NextState()
{
    if(currentState->GetId() != activePipe.back()->GetId()){
        activePipe.pop_front();
        currentState = activePipe.front();
        currentState->SetNew(true);

    }else
    {
        EndAction();
        activePipe = statePipe;
    }
}



Action::POS Action::GetPos(){
    return position;
}



ACT_TYPE Action::GetType()
{
    return type;
}



void Action::CalculTargetTime()
{
    tp_hd tp = statePipe.front()->GetTargetTime();
    auto it = statePipe.begin();
    
    for(++it ; it != statePipe.end(); ++it)
    {
        tp += (*(--it))->GetLifeTime();
        it++;

        (*it)->SetTargetTime(tp);
    }
}

//////////////////////////////////////////////////////////////////////////////
//          WALK METHOD DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

const Action::state_list Walk::WalkStateList = Action::state_list({State::TYPE::MOTION});


Walk::Walk():Action(ACTION::WALK)
{
    InitStates();
}



void Walk::InitStates(){
    
        State* state = (State*) ProgramFactory<State>::CreateProgram(State::TYPE::MOTION,vector<ProgramArguments>({ProgramArguments(1.f),ProgramArguments(0)}),Time::GetTime(),chrono::milliseconds(1000));
        statePipe.push_back(state);
    
            state = (State*) ProgramFactory<State>::CreateProgram(State::TYPE::MOTION,vector<ProgramArguments>({ProgramArguments(1.f),ProgramArguments(2)}),Time::GetTime(),chrono::milliseconds(1500));
        statePipe.push_back(state);
    
            state = (State*) ProgramFactory<State>::CreateProgram(State::TYPE::MOTION,vector<ProgramArguments>({ProgramArguments(1.f),ProgramArguments(1)}),Time::GetTime(),chrono::milliseconds(1000));
        statePipe.push_back(state);
    
            state = (State*) ProgramFactory<State>::CreateProgram(State::TYPE::MOTION,vector<ProgramArguments>({ProgramArguments(1.f),ProgramArguments(3)}),Time::GetTime(),chrono::milliseconds(1500));
            statePipe.push_back(state);
    
    activePipe = statePipe;
}




//////////////////////////////////////////////////////////////////////////////
//          WAIT METHOD DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

const Action::state_list Wait::WaitStateList = Action::state_list({State::TYPE::PAUSE});


Wait::Wait():Action(ACTION::WAIT)
{
    InitStates();
}



void Wait::InitStates(){
    for (auto st : WaitStateList) {
        State* state = (State*) ProgramFactory<State>::CreateProgram(st,vector<ProgramArguments>({ProgramArguments(10000)}),tp_hd(),chrono::milliseconds(1000));
        statePipe.push_back(state);
    }
    activePipe = statePipe;
}





//////////////////////////////////////////////////////////////////////////////
//          GOTO METHOD DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

const Action::state_list GoTo::GoToStateList = Action::state_list({State::TYPE::SPACE_LOC});


GoTo::GoTo():Action(ACTION::GOTO),
    fromPos(),
    toPos(),
    fromDir(),
    toDir(),
    duration(),
    interpolPos(nullptr),
    interpolDir(nullptr),
    destinationObj(nullptr),
    playInverse(false)
{
    InitStates();
}



GoTo::GoTo(const GusVec3& from_pos, const GusVec3& to_pos,const GusVec3& from_dir, const GusVec3& to_dir,chrono::milliseconds in_ms):Action(ACTION::GOTO),
    fromPos(from_pos),
    toPos(to_pos),
    fromDir(from_dir),
    toDir(to_dir),
    duration(in_ms),
    interpolPos(nullptr),
    interpolDir(nullptr),
    destinationObj(nullptr),
    playInverse(false)
{
    InitStates();
}



GoTo::GoTo(const StageObject& from_obj, const StageObject& to_obj, ms in_ms):
    Action(ACTION::GOTO),
    fromPos(from_obj.GetPosition()),
    toPos(to_obj.GetPosition()),
    fromDir(from_obj.GetDirection()),
    toDir(to_obj.GetPosition()),
    duration(in_ms),
    interpolPos(nullptr),
    interpolDir(nullptr),
    destinationObj(&to_obj),
    playInverse(false)
{
    InitStates();
}



void GoTo::InitStates()
{
    vector<ProgramArguments> vec({fromPos.x,fromPos.y,fromPos.z,fromDir.x,fromDir.y,fromDir.z,0.f,1.f,0.f});
    
    State* state = (State*) ProgramFactory<State>::CreateProgram(State::TYPE::SPACE_LOC, vec, Time::GetTime(), duration);
    
    statePipe.push_back(state);
    
    activePipe = statePipe;
}



void GoTo::UpdateState()
{
  //  cout << "[GoTo][UpdateState] POS" << interpolPos->GetValue().Print() << endl;
  //  cout << "[GoTo][UpdateState] DIR" << interpolDir->GetValue().Print() << endl;
    ((SpaceLocation*)statePipe.front())->InitArgsMap(interpolPos->GetValue(),interpolDir->GetValue());
}




void GoTo::StartAction()
{
    GusVec3 final_pos = toPos;
    GusVec3 final_dir = toDir;
    
    if(destinationObj)
    {
        final_pos = GusVec3::ChangeLengthBy(fromPos, toPos, -1*CameraControl::GetFocusLength((const SpaceObject&)*destinationObj));
        final_dir = destinationObj->GetPosition();
        cout << "[GOTO][StartAction] pos TO FINAL " << final_pos.Print() << endl;
        cout << "[GOTO][StartAction] dir TO " << destinationObj->GetPosition().Print() << endl;
    }


    if (playInverse) {
        cout << "[GOTO][StartAction] GOTO INVERSED!!" << endl;
        interpolPos = new Interpolator<GusVec3>(final_pos,fromPos,duration);
        interpolDir = new Interpolator<GusVec3>(final_dir,fromDir,duration);
    }else
    {
        interpolPos = new Interpolator<GusVec3>(fromPos,final_pos,duration);
        interpolDir = new Interpolator<GusVec3>(fromDir,final_dir,duration);
    }
    interpolPos->Start();
    interpolDir->Start();
    
    cout << "[GOTO][StartAction] target " << interpolDir->GetTarget().Print() << endl;
    
    Action::StartAction();
}



void GoTo::InitPath(const GusVec3 &from, const GusVec3 &to)
{
    
}































































































