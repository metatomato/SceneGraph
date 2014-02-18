//
//  ShadersRef.h
//  helloGL
//
//  Created by tomato on 2/25/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"

using namespace std;


struct ShadersRef {
    
    static map<int, string> InitShaderMap() {
        map<int, string> map;
    map[0x00] = "default";
    map[0x01] = "color_0001";
    map[0x02] = "color_0010";
    map[0x03] = "color_0011";
    map[0x04] = "color_0100";
    map[0x05] = "color_0101";
    map[0x06] = "color_0110";
    map[0x07] = "color_0111";
    map[0x08] = "color_1000";
    map[0x09] = "color_1001";
    map[0x0a] = "color_1010";
    map[0x0b] = "color_1011";
    map[0x0c] = "color_1100";
    map[0x0d] = "color_1101";
    map[0x0e] = "color_1110";
    map[0x0f] = "color_1111";
        return map;
    }
    
    static const map<int,string> shadersRef;
    
    static string GetShadersRef(int idx) {
        map<int, string> map = InitShaderMap();
        return map[idx];
    }
    

};



















