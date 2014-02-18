//
//  BroadcastingService.cpp
//  helloGL
//
//  Created by tomato on 10/14/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "BroadcastingService.h"
#include "Program.h"
#include "../Common/Time.h"
#include "GusTV.h"
#include "../Logic/Cortex.h"
#include "../Scene/Scene.h"
#include "ChannelOffice.h"


void BroadCastingService::DispatchProgram(ChannelOffice::CHANNEL channel, const Program& prog)
{
    channelListings[channel].push_back(&prog);
    //cout << "[BCS][Dispatch] Added to channel " << (int)channel << " , progId " << prog.GetId() << endl;
}


void BroadCastingService::OnTheAir()
{
    AttachProgram(channelListings);
    
    TransmitToSubscriber();
}



void BroadCastingService::AttachProgram(map_chan_prog& listings)
{
    for (auto pair : listings)
    {
        for (auto prog : pair.second)
        {
            if(IsShowTime(*prog))
            {
                for (auto obj : GusTV::GetSubscibers(pair.first))
                {
                    subscriberListings[obj].emplace(prog);
                }
                for (auto obj : GusTV::GetAdmins(pair.first))
                {
                    adminListings[obj].emplace(prog);
                }
                
                if (IsReschedule(*prog))
                {
                    channelListings[pair.first].push_back(prog);
                }else
                {
                    deathList.emplace(prog->GetId());
                }
            } else
            {
                channelListings[pair.first].push_back(prog);
            }
            channelListings[pair.first].pop_front();
        }
    }
}



void BroadCastingService::TransmitToSubscriber()
{
    for(auto pair : subscriberListings){
        GusObject* obj = const_cast<GusObject*>(pair.first);
        for(auto prog : pair.second){
            obj->OnTV(*prog);
        }
    }
    
    for( auto pair : adminListings){
        if(pair.first == (int)ChannelOffice::ADMINISTRATION::CORTEX){
            for(auto prog : pair.second){
                Cortex::RecieveStimulus(prog);
            }
        }
        if(pair.first == (int)ChannelOffice::ADMINISTRATION::SCENE){
            for(auto prog : pair.second){
                GusTV::sceneRef->OnTV(prog);
            }
        }
    }
}



void BroadCastingService::EndTransmission()
{
    subscriberListings.clear();
    adminListings.clear();
    
    for (auto obj : deathList) {
        if(GusTV::programs.count(obj) != 0){
            ProgramFactory<Program>::DestroyProgram(*GusTV::programs[obj]);
            GusTV::programs.erase(obj);
        }
        
    }
   // cout << "[BROADCASTINGSERVICE][EndTransmission] program left " << GusTV::programs.size() << endl;
}



bool BroadCastingService::IsShowTime(const Program& prog)
{
    if ( prog.GetTargetTime() < Time::GetTime() ) {return true;}
    
    return false;
}



bool BroadCastingService::IsReschedule(const Program& prog)
{
    tp_hd now = Time::GetTime();
    
    if ( (prog.GetTargetTime() > now) || ((prog.GetTargetTime()+prog.GetLifeTime()) > now) )
    {
        if (prog.GetType() == (int)Program::TYPE::TIMER)
        {
            if((prog.GetTargetTime() < now)){
                const_cast<Program*>(&prog)->SetTargetTime(now + ((Timer&)prog).GetPeriod());
            }
            
        }
        return true;
    }
    
    
    
    return false;
}














































