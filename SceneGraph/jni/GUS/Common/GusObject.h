//
//  GusGLObject.h
//  helloGL
//
//  Created by tomato on 9/19/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once
#include <iostream>


using namespace std;

class GusVec3;
class Program;
class ChannelOffice;
class State;

class GusObject
{
        public :
static const int                GetObjectCount(){return _lastId+1;};

        private :
static int                      _lastId;

        public :
    GusObject(){_AssignId();};
    virtual ~GusObject(){};
    
    int                         _GetId() const {return _gusId;};
    virtual string              _GetName() const{return "";};
    virtual int                 _GetType() const {return -1;};
    
    virtual void                OnTV(const Program&);
    virtual void                BrainOrder(const State&);

        private :
    void                        _AssignId(){_gusId = ++_lastId;};
    
    int                         _gusId;
};



template <>
struct hash<GusObject> {
    int operator() (const GusObject& lhs) const
    {
        return lhs._GetId();
    }
};

template <>
struct equal_to<GusObject> : binary_function<GusObject, GusObject, bool>
{
    bool operator()(const GusObject& x, const GusObject& y) const
    {return x._GetId() == y._GetId();}
};

template <>
struct hash<GusObject*> {
    int operator() (const GusObject* const lhs) const
    {
        return lhs->_GetId();
    }
};

template <>
struct equal_to<GusObject*> : binary_function<GusObject*, GusObject*, bool>
{
    bool operator()(const GusObject* const x, const GusObject* const y) const
    {return x->_GetId() == y->_GetId();}
};
