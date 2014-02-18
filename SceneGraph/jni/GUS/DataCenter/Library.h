//
//  Library.h
//  helloGL
//
//  Created by tomato on 9/17/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include "../Common/GUS.h"
#include "../Scene/SceneObject.h"
#include "../Renderer/EntityDBG.h"

enum LIBRARY_TYPE{RENDER,SCENE};

template <class T>
class Library {
        public :
    Library():collection(){};
    ~Library();
    
    void                            AddEntry(int idx, T* obj);
    void                            AddEntry(T** obj);
    void                            AddEntry(T* obj);
    T*                              GetEntry(int obj_id);
    T*                              GetEntry(string obj_name);
    int                             IsInCollection(const string& obj_name);
    const map<int, T*>&             GetCollection() const {return collection;};
    void                            SetCollection(map<int, T*>);
    vector<T*>                      GetVector() const;
    vector<T*>                      GetVector(vector<string>&);
    vector<T*>                      GetVector(vector<int>&);
    void                            Append(vector<T*>);
    
    vector<string>                  GetNamesOfCollection() const;
    vector<string>                  GetNames(vector<int> vec);
    
    
        protected :
    map <int,T*>                    collection;
    
        private :
    
};



class RenderLibrary : public Library<GusGLObject> {
        public :
    RenderLibrary();
    
        private :
};


class SceneLibrary : public Library<SceneObject> {
        public :
    SceneLibrary();
    
    private :
};





















