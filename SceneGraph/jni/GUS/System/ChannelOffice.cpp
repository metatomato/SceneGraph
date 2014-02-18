//
//  EventOffice.cpp
//  helloGL
//
//  Created by tomato on 10/13/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "ChannelOffice.h"


void ChannelOffice::RegisterSubscribtion(CHANNEL channel, const GusObject& obj)
{
    subscribers[channel].emplace(&obj);
    cout << "[CHANNELOFFICE][Registration] obj " << obj._GetId() << " registred for subscribtion" << endl;
}


vector<const GusObject*>  ChannelOffice::GetSubscribers(CHANNEL channel)
{
    vector<const GusObject*> vec_ptr;
    for (auto ptr : subscribers[channel]) {
        vec_ptr.push_back(ptr);
    }
    return vec_ptr;
};



vector<int>  ChannelOffice::GetAdmins(CHANNEL channel)
{
    vector<int> vec_ref;
    for (auto ref : adminSubscribtions[channel]) {
        vec_ref.push_back(ref);
    }
    return vec_ref;
};



void ChannelOffice::RegisterAdmin(CHANNEL channel, ChannelOffice::ADMINISTRATION admin)
{
    adminSubscribtions[channel].emplace((int)admin);
}





