//
//  Time.h
//  helloGL
//
//  Created by tomato on 10/9/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__Time__
#define __helloGL__Time__

#include <iostream>
#include <time.h>
#include <chrono>
#include "GUS.h"
#include "../Renderer/Renderer.h"

typedef chrono::high_resolution_clock clock_hd;
typedef chrono::time_point<clock_hd> tp_hd;
typedef chrono::nanoseconds ns;
typedef chrono::milliseconds ms;

class Time{
        public :
    
    static void             InitTime(){
                                game_epoch = clock_hd::now();
                                time  = game_epoch;
                                dT = 0.0;
                            };
    
    static void             UpdateClock(){
                                tp_hd temp_tp = clock_hd::now();
                                SetDt(temp_tp);
                                time = temp_tp;
                            };
    static void             SetDt(tp_hd& tp){
                                dT = ConvertToMs(tp, time);
                            };
    static double           GetDt(){
                                return dT;                  //in millisecond
                            };
    
    static double           ConvertToMs(const tp_hd& tp0, const tp_hd& tp1){            // in millisecond
                                return (double)(tp0.time_since_epoch().count() - tp1.time_since_epoch().count())/1000000;
                            };
    static tp_hd            GetTime(){                                      // in nanosecond
                                return time;
                            };
    static double           GetTimeFromRef(const tp_hd& t)
                            {
                                return ConvertToMs(t, game_epoch);
                            };
    
    static const tp_hd      END_OF_TIME;
    static const ms         UNTIL_END;
    
    
        public :
    
    Time():epoch(clock_hd::now()){};
    
    float                   GetLocalTime() const {                          // in second
                                tp_hd temp = epoch;
                                return ConvertToMs(time, temp)/1000.f;
                            };
    
    void                    PrintTime(){
                                cout << "[TIME][PrintTime] " << GetLocalTime() << endl;
                            };
    
    
        private :
    const tp_hd                epoch;
    float                      scaleTime;
    bool                       isPause;
    
    static    tp_hd            game_epoch;
    static    tp_hd            time;
    static    double           dT;
};


#endif /* defined(__helloGL__Time__) */
