//
//  DataCenter.h
//  helloGL
//
//  Created by tomato on 9/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once 

#include "../Common/GUS.h"
#include "Library.h"
#include "../Common/RenderPass.h"
#include "../Renderer/EntityDBG.h"


typedef map<StageObject::STAGE_OBJ_ID,SceneLibrary*> SceneData;
typedef map<GusGLObject::GUS_OBJ_ID,RenderLibrary*>  RenderData;
typedef map<EntityDBG::GUS_OBJ_ID, RenderLibrary*> RenderDataDBG;


class DataCenter {
        public :
    DataCenter();
    ~DataCenter();
    
    void                                FetchDataFromFile(string file);
    int                                 AppendToLibrary(vector<GusObject*>& vec_input);
    int 								AppendToBaseShapeLibrary(vector<GusObject*>& vec_input);
 
    RenderPass<GusGLObject>             DataToRender();
    RenderPass<GusGLObject>             DataToRender(string file);
    RenderPass<GusGLObject>             DataToRender(vector<string> obj_names);
    RenderPass<SceneObject>             DataToScene();
    
    RenderPass<GusGLObject>             SceneToRender(RenderPass<SceneObject>& pass);
    vector<RenderPass<GusGLObject>>     SceneToRender(vector<RenderPass<SceneObject>>& s_pass);
    
    vector<string>                      GetLibraryList(GusGLObject::GUS_OBJ_ID type);
    
    GusGLObject*                        GetEntity(GusGLObject::GUS_OBJ_ID,int idx);
    vector<GusGLObject*>                GetEntity(GusGLObject::GUS_OBJ_ID,vector<int> ref);
    
    SceneData*                          GetSceneData(){return &sceneData;};
    
    int									CreateCamera(LIBRARY_TYPE type, string name);
    int									CreateLight(LIBRARY_TYPE type, string name,int GL_idx);

    void								FillServerMemory();
    

    vector<string>						GetLoadedFiles(){return loadedFiles;};

    vector<string>						GetEntityListFromFiles(string file);
    vector<string>                      GetStageListFromFiles(string file);

    vector<string>						GetEntityPackedDataFromFiles(string file);
    vector<string>                      GetStagePackedDataFromFiles(string file);
    vector<string>                      GetLightPackedData();

    void                                PrintEntityFromFiles();
    void                                PrintStageFromFiles();
    
    static Mesh* 						GetBaseShape(string name);

        private :
    
    RenderData                               renderData;
    SceneData                                sceneData;
    
    static RenderData                        renderBaseShapeData;

    RenderDataDBG                            renderDataDebug;
    RenderData                               renderDataFX;
    
    map<string, vector<int>>                 stageFromFile;
    map<string, vector<int>>                 entityFromFile;
    vector<string>							 loadedFiles;
    

    friend class Scene;
};
