//
//  GusTV.h
//  helloGL
//
//  Created by tomato on 10/14/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__GusTV__
#define __helloGL__GusTV__

#include <iostream>
#include "../Common/GUS.h"
#include "BroadcastingService.h"
#include "ProgramFactory.h"

class Scene;

class GusTV {
        public :
    typedef map<int, const Program*>   Program_map;
    
    friend class BroadCastingService;
    
    static void                         RegisterChannelShow();
    
    static void                         BroadCast(ChannelOffice::CHANNEL, const Program&);
    
    static void                         Subscrib(ChannelOffice::CHANNEL, GusObject&);
    static  vector<const GusObject*>    GetSubscibers(ChannelOffice::CHANNEL channel);
    static void                         RegisterAdmin(ChannelOffice::CHANNEL, ChannelOffice::ADMINISTRATION);
    static  vector<int>                 GetAdmins(ChannelOffice::CHANNEL channel);
    static void                         StartBroadCasting();
    static void                         EndBroadCasting();
    
    static void                         InitScene(const Scene& scene) {sceneRef = &scene;};
    
        private :
    static ChannelOffice               channelOffice;
    static ChannelShow                 channelShow;
    static BroadCastingService         BCService;
    
    static Program_map                  programs;
    
    static const Scene*                 sceneRef;
    
    friend void ProgramFactory<Program>::RegisterProgram(const Program&);
    friend void BroadCastingService::EndTransmission();
    friend void BroadCastingService::TransmitToSubscriber();
};


#endif /* defined(__helloGL__GusTV__) */
