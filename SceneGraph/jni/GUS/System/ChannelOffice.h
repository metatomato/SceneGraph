//
//  EventOffice.h
//  helloGL
//
//  Created by tomato on 10/13/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__EventOffice__
#define __helloGL__EventOffice__

#include <iostream>
#include "../Common/GUS.h"





class ChannelOffice {
    
        public :
    enum class CHANNEL{ANIMATOR,MOTION,SELECTION,DBG,TIME};
    enum class ADMINISTRATION{SYSTEM,CORTEX,SCENE,DATACENTER,RENDERER};
    
    typedef map<CHANNEL, unordered_set<const GusObject*>> map_channel;
    typedef map<CHANNEL, unordered_set<int>>   map_channel_admin;
    
    void                                RegisterAdmin(CHANNEL channel, ADMINISTRATION admin);
    void                                RegisterSubscribtion(CHANNEL, const GusObject&);
    vector<const GusObject*>            GetSubscribers(CHANNEL channel);
    vector<int>                         GetAdmins(CHANNEL channel);
    
        private :
    map_channel          subscribers;
    map_channel          goldenMenbers;
    map_channel_admin    adminSubscribtions;
    
};

typedef ChannelOffice::CHANNEL CHANNEL;


class ChannelShow {
        public :
    typedef map<CHANNEL, deque<const Program*>> map_chan_prog;
    
    void                                RegisterShow(CHANNEL chan, const Program* prog){regularShow[chan].push_back(prog);};
    
        private :
    map_chan_prog                       regularShow;
    
    friend class BroadCastingService;
};
#endif /* defined(__helloGL__EventOffice__) */
