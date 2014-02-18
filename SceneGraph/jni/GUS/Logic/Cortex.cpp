//
//  Cortex.cpp
//  helloGL
//
//  Created by tomato on 10/13/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Cortex.h"
#include "../Scene/SceneObject.h"
#include "../System/GusTV.h"

map<const GusObject*,Logic*>          Cortex::neuronalConnection;

Cortex::map_prog_type                 Cortex::stimuli;

bool                                  Cortex::firstInit = true;



void Cortex::MakeConnection(const GusObject& obj)
{
    Logic* logic = nullptr;
    if (obj._GetType() == (int)StageObject::STAGE_OBJ_ID::CAMERA) {
        logic = (Logic*)new CameraLogic(obj);
    }
    neuronalConnection.emplace(&obj,logic);
    
    cout << "[CORTEX][MakeConnection] NeuronnalConnection established with " << obj._GetName() << endl;
}


void Cortex::DisconnectAll()
{
    for(auto pair : neuronalConnection){
        Disconnect(*(pair.first));
    }
}


void Cortex::Disconnect(const GusObject& obj)
{
    delete neuronalConnection.at(&obj);
}



void Cortex::ConnectChannel(ChannelOffice::CHANNEL)
{
    GusTV::RegisterAdmin(ChannelOffice::CHANNEL::MOTION,    ChannelOffice::ADMINISTRATION::CORTEX);
    GusTV::RegisterAdmin(ChannelOffice::CHANNEL::SELECTION, ChannelOffice::ADMINISTRATION::CORTEX);
}


void Cortex::RecieveStimulus(const Program* prog)
{
    if(stimuli.size() > 100){
        stimuli.clear();
    }

   // cout << "[CORTEX][ReceiveStimulus] Recieve stimulus from prog " << prog->GetId()  << " of type " << prog->GetType() << endl;
    
    stimuli[prog->GetType()].push_back(prog);
    
}

void Cortex::Stimulate()
{
    for(auto pair : neuronalConnection) {
        pair.second->Update();
    }
    stimuli.clear();
}



void Cortex::InitNervousSystem()
{
    for(auto pair : neuronalConnection) {
      //  pair.second->InitAction();
    }
}
