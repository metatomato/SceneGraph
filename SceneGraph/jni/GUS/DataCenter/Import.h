//
//  Import.h
//  helloGL
//
//  Created by tomato on 9/18/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#ifndef __helloGL__Import__
#define __helloGL__Import__

#include "../Common/GUS.h"
#include "../Renderer/Mesh.h"
#include "Library.h"


class Import {
        public :
    enum FILE_TYPE{OBJ,MTL,MAP,TGA};
    
    static  void                    SetDocumentsFolder(string path)   {LOGE("[IMPORT][SetDocumentsFolder] INIT IMPROT PATH");
    										documentsFolder = path.c_str();
    										//documentsFolder="/mnt/sdcard/SCENEGRAPH/Res/";
    								}
    static  string                  GetDocumentsFolder(){return documentsFolder;};
    static  void                    SetResourcesFolder(string path)   {
    										LOGE("[IMPORT][SetResourceFolder] INIT IMPROT PATH");
    										resourcesFolder = path.c_str();
    										//resourcesFolder="/mnt/sdcard/SCENEGRAPH/Res/";
    										}
    static  string                  GetResourcesFolder(){return resourcesFolder;};
    
    static string                   GetTypeCorrespondance(FILE_TYPE type);
    static FILE_TYPE                GetTypeCorrespondance(string type);
    
    static string                   TestPath(string file);
    
    
    static map<string, Mesh*>              ParseData(ifstream& input_stream);
    static vector<vector<GusObject*>>      ParseData(string file);
    
    
        private :
    static    string                    resourcesFolder;
    static    string                    documentsFolder;
    
};

#endif /* defined(__helloGL__Import__) */


class ImportObj {
        public :
    ImportObj():vertexAttribs(),faceAttribs(),mtlFiles(),rMesh(),rMaterial(),rEntity(),sEntity(){};
    
    void                            ExtractMeshAttribs(string file);
    void                            ExtractMaterialAttribs(string file);
    void                            GenerateObject();
    vector<vector<GusObject*>>      ExtractData(string file);

    
    
    
    void                PrintMaterialList(){auto it = mtlFiles.begin(); for(;it!=mtlFiles.end();++it){cout << "[IMPORTOBJ][PrintMaterial]" <<*it << endl;}}

    private :
    string                                  currentFile;
    
    map<string,MeshVertexAttribs_t*>        vertexAttribs;
    map<string,MeshFaceAttribs_t*>          faceAttribs;
    vector<string>                          mtlFiles;
    map< string, MaterialAttribs_t* >       materialAttribs;
    
    vector<GusObject*>                      rMesh;
    vector<GusObject*>                      rMaterial;
    vector<GusObject*>                      rEntity;
    
    vector<GusObject*>                      sEntity;

};
