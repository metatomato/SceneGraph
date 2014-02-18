//
//  Library.cpp
//  helloGL
//
//  Created by tomato on 9/9/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Library.h"
#include "../Scene/Scene.h"
#include "Import.h"








/////////////////////////////////////////////////
// LIBRARY CLASS METHOD DEFINITIONS
////////////////////////////////////////////////

/*
Library<T>::Library():renderLibrary(),sceneLibrary(),defaultCamera(-1),defaultLight(-1){
    Camera* default_camera = new Camera();
    renderLibrary[GusGLObject::CAMERA].AddEntry((GusGLObject**) &default_camera);
    defaultCamera = default_camera->_GetId();
    Camera* default_light = new Camera();
    renderLibrary[GusGLObject::LIGHT].AddEntry((GusGLObject**) &default_light);
    defaultLight = default_light->_GetId();
}
*/

template<class T>
Library<T>::~Library(){
    cout << "[COLLECTION][~Collection] Destructor call!" << endl;
    
    typename map<int,T*>::iterator it = collection.begin();
    for( ; it != collection.end() ; ++it)
    {
        cout << "[COLLECTION][~Collection] Called for destruction " << it->second->_GetName() << endl;
        delete it->second;
    }
    
}



template<class T>
void Library<T>::AddEntry(int idx, T* obj) {
        collection[idx] = obj;
        //    cout << "[SCENE][AddSceneObject] " << library->Meshes[(*obj)->_GetId()]->_GusGLObjectoString() << endl;
}



template<class T>
void Library<T>::AddEntry(T** obj) {
    int ref = IsInCollection((*obj)->_GetName());
    if( ref == -1 ){
        collection[(*obj)->_GetId()] = *obj;
        //    cout << "[SCENE][AddSceneObject] " << library->Meshes[(*obj)->_GetId()]->_GusGLObjectoString() << endl;
    } else {
        delete *obj;
        *obj = GetEntry(ref);
    }
}



template<class T>
void Library<T>::AddEntry(T* obj) {
        collection[obj->_GetId()] = obj;
}




template<class T>
T* Library<T>::GetEntry(int obj_id) {
    return collection[obj_id];
}


template<class T>
T* Library<T>::GetEntry(string obj_name) {
    T* obj = NULL;
    int ref = IsInCollection(obj_name);
    if(ref != -1){
        obj = GetEntry(ref);
    }
    //  cout << "[SCENE][GetSceneObject] return " << obj->_GetName() << endl;
    return obj;
}



template<class T>
int Library<T>::IsInCollection(const string& obj_name){
    int flag = -1;
    typename std::map<int,T*>::iterator it = collection.begin();
    for (; it != collection.end(); ++it) {
        if (it->second->_GetName().compare(obj_name) == 0) {
            flag = it->first;
            //  cout << "[SCENE][IsInLibrary] Existing Entity object " << name << endl;
        }
    }
    return flag;
};



template<class T>
vector<T*> Library<T>::GetVector() const{
    vector<T*> vec;
    typename map<int,T*>::const_iterator it     = collection.begin();
    typename map<int,T*>::const_iterator it_end = collection.end();
    for (; it!=it_end; ++it) {
        vec.push_back(it->second);
    }
    return vec;
}



template<class T>
vector<T*> Library<T>::GetVector(vector<string>& obj_names){
    vector<T*> vec;
    for (auto it = obj_names.begin(); it!=obj_names.end(); ++it) {
        vec.push_back(GetEntry(*it));
    }
    return vec;
}



template<class T>
vector<T*> Library<T>::GetVector(vector<int>& obj_idx){
    vector<T*> vec;
    for (auto idx : obj_idx) {
        vec.push_back(collection[idx]);
    }
    return vec;
}



template<class T>
void Library<T>::Append(vector<T*> vec){
    typename vector<T*>::iterator it     = vec.begin();
    typename vector<T*>::iterator it_end = vec.end();
    for (; it!=it_end; ++it) {
        collection[(*it)->_GetId()] = *it;
    }
}


template<class T>
vector<string> Library<T>::GetNamesOfCollection() const{
    vector<string> list;
    for(auto it = collection.begin(); it != collection.end() ; ++it) {
        list.push_back(it->second->_GetName());
    }
    return list;
}


template<class T>
vector<string> Library<T>::GetNames(vector<int> vec){
    vector<string> names;
    for (auto obj : vec) {
        names.push_back(collection[obj]->_GetName());
    }
    return names;
}




///////////////////////////////////////////////
// RENDERLIBRARY METHOD DEFINITIONS
///////////////////////////////////////////////


RenderLibrary::RenderLibrary(){

}



///////////////////////////////////////////////
// SCENELIBRARY METHOD DEFINITIONS
///////////////////////////////////////////////


SceneLibrary::SceneLibrary(){

    

}





template class Library<GusGLObject>;
template class Library<SceneObject>;


















