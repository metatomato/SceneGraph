//
//  BroadcastingService.h
//  helloGL
//
//  Created by tomato on 10/14/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__BroadcastingService__
#define __helloGL__BroadcastingService__

#include <iostream>
#include "../Common/GUS.h"
#include "ChannelOffice.h"

class Program;

class BroadCastingService{
        public :
    typedef ChannelOffice::CHANNEL                                  CHANNEL;
    typedef map<CHANNEL,           deque<const Program*>>           map_chan_prog;
    typedef map< const GusObject*, unordered_set<const Program*>>   map_obj_prog;
    typedef map< int,              unordered_set<const Program*>>   map_ref_prog;
    
    void                    DispatchProgram(ChannelOffice::CHANNEL, const Program& prog);
    
    void                    OnTheAir();
    void                    AttachProgram(map_chan_prog&);
    void                    TransmitToSubscriber();
    void                    EndTransmission();
    bool                    IsShowTime(const Program&);
    bool                    IsReschedule(const Program&);
    
        private :
    map_chan_prog                channelListings;
    map_obj_prog                 subscriberListings;
    map_ref_prog                 adminListings;
    
    unordered_set<int>           deathList;
    
};


#endif /* defined(__helloGL__BroadcastingService__) */
