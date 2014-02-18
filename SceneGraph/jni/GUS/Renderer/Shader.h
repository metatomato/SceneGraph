//
//  Shader.h
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "Light.h"
#include "Material.h"
#include "ShadersRef.h"

class Material;

int MaskConverter(RENDER_PASS type, int value);
int GenIdx(string name);

struct ShaderSet {
    map<RENDER_PASS , Shader *> shaders;
};


class Shader{
        public :
    static string               _Class() {return "Shader";};
    static Shader *             GetShaderObject(int idx){return shadersLibrary[idx];};
    static uint                 LoadShaderFromString(string vert,string frag);
    static uint                 LoadShaderFromSource(char* vert,char* frag);
    static ShaderSet            GetCorrespondance(Material& mat);
    static void 				InitShadersFromSources();
    static void                 InitShaders();
    static int                  lastId;
    static void                 PrintShaderList();
    static void					AddSource(pair<string,string> p,int idx){shaderSources.emplace(idx,p);}
    
    

    Shader();
    Shader(char* vert, char* frag, int idx, bool b);
    Shader(string vert, string frag, int idx);
    Shader(string vert, string frag);
    Shader(int ref, GLuint gl_ref);
    Shader(string vert, string frag, Material& mat);

    ~Shader();
    string                      ToString();
    void                        AssignId(int idx){ID = idx;};
    void						ParseAllAttribs();
    void						ParseAllAttribs(const Material& mat);
    void						ParseAttribs();
    void                        ParseMaterialAttribs(const Material& mat);
    void                        ParseMaterialAttribs();
    void                        ParseMeshAttribs();
    void                        ParseLightAttribs();
    void                        ParseEntityAttribs();
    void                        ParseCameraAttribs();
    void                        ParseRendererAttribs();
    void                        ParseDebugAttribs();
    
    GLuint                      GetMeshAttribLocation(int idx)      {return meshAttribsLocation[idx];};
    GLuint                      GetMaterialUniformLocation(int idx) {return materialUniformsLocation[idx];};
    GLuint                      GetEntityUniformLocation(int idx)   {return entityUniformsLocation[idx];};
    GLuint                      GetCameraUniformLocation(int idx)   {return cameraUniformsLocation[idx];};
    GLuint                      GetLightUniformLocation(int idx)    {return lightUniformsLocation[idx];};
    GLuint                      GetRendererUniformLocation(int idx) {return rendererUniformsLocation[idx];};
    GLuint                      GetDebugUniformLocation(int idx) {return debugUniformsLocation[idx];};
    
    GLuint                      GetID(){return ID;};
    GLuint                      GetGLId(){return GLId;};
    

        private :
    static map<int, GLuint>   			GLRef;
    static map<int, Shader *>   		shadersLibrary;
    static map<int, pair<string,string>> 	shaderSources;
    
    int                        ID;
    GLuint                     GLId;
    GLuint                     materialUniformsLocation[GUS_MATERIAL_ATTRIBS_TYPE];
    GLuint                     meshAttribsLocation[GUS_MAX_ATTRIBUTES];
    GLuint                     entityUniformsLocation[GUS_ENTITY_ATTRIBS_TYPE];
    GLuint                     lightUniformsLocation[LIGHT_ATTRIBS];
    GLuint                     cameraUniformsLocation[GUS_CAMERA_ATTRIBS_TYPE];
    GLuint                     rendererUniformsLocation[GUS_RENDERER_ATTRIBS_TYPE];
    GLuint                     debugUniformsLocation[DBG_UNIFORM_MAX];
   
};

struct ShaderSetDBG {
    map<DEBUG_MODE , Shader *> shadersDBG;
    ShaderSetDBG(){
        shadersDBG[POINT_MODE] = Shader::GetShaderObject(0x10);
        shadersDBG[LINE_MODE] = Shader::GetShaderObject(0x10);
        shadersDBG[NORMAL_MODE] = Shader::GetShaderObject(0x10);
        shadersDBG[TANGENT_MODE] = Shader::GetShaderObject(0x10);
        shadersDBG[TRI_MODE] = Shader::GetShaderObject(0x11);
        shadersDBG[BOX_MODE] = Shader::GetShaderObject(0x10);
    }
};



















