//
//  GusTV.cpp
//  helloGL
//
//  Created by tomato on 10/14/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "GusTV.h"
#include "../Common/Time.h"

ChannelOffice GusTV::channelOffice;
BroadCastingService GusTV::BCService;
GusTV::Program_map GusTV::programs;
ChannelShow GusTV::channelShow;

const Scene*       GusTV::sceneRef = nullptr;


void GusTV::RegisterChannelShow()
{
    Timer* timer = new Timer(vector<ProgramArguments>({ProgramArguments(1000)}),Time::GetTime(),Time::UNTIL_END);
    channelShow.RegisterShow(CHANNEL::TIME, timer);
    BCService.DispatchProgram(CHANNEL::TIME, *timer);
}


void GusTV::BroadCast(ChannelOffice::CHANNEL channel, const Program& prog)
{
    programs[prog.GetId()] = &prog;
    BCService.DispatchProgram(channel, prog);
}



void GusTV::Subscrib(ChannelOffice::CHANNEL channel, GusObject& obj)
{
    channelOffice.RegisterSubscribtion(channel, obj);
}



vector<const GusObject*>  GusTV::GetSubscibers(ChannelOffice::CHANNEL channel)
{
    return channelOffice.GetSubscribers(channel);
};



void GusTV::RegisterAdmin(ChannelOffice::CHANNEL channel, ChannelOffice::ADMINISTRATION admin)
{
    channelOffice.RegisterAdmin(channel, admin);
}



vector<int>  GusTV::GetAdmins(ChannelOffice::CHANNEL channel)
{
    return channelOffice.GetAdmins(channel);
};



void GusTV::StartBroadCasting()
{
    BCService.OnTheAir();
}



void GusTV::EndBroadCasting()
{
    BCService.EndTransmission();
}





















