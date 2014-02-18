//
//  Renderer.h
//  helloGL
//
//  Created by metatomato on 2/1/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include <iostream>
#include "../Common/GUS.h"
#include "../Common/Matrix.h"
#include "../renderer/Shader.h"
#include "../Scene/Control.h"
#include "../Common/RenderPass.h"

/*
#ifdef IOS
const uint VIEWPORT_X = 2 * SCREEN_X;
const uint VIEWPORT_Y = 2 * SCREEN_Y;
#else
const uint VIEWPORT_X = SCREEN_X;
const uint VIEWPORT_Y = SCREEN_Y;
#endif
*/

class EntityDBG;
class Entity;
class Camera;
class Light;
class Shader;   
class Mesh;

enum P_MODE{PROJECTION,ORTHOGRAPHIC};


class Renderer {
        public :
static int                          renderedFrame;
static GLint                        frameView[2];
static GLfloat                      aspectRatio;
static GLfloat                      fov;
static GLint                        defaultFBO;
static const  vector<string>        attribsList;
static void                         SetProjectionMode(P_MODE mode);
static bool							InsetMode;
static bool							StencilMode;
static GLint						insetFrame[4];
        private :
static int                          projectionMode;

        public :
                                    Renderer();
                                    ~Renderer();
    virtual void                    Init();
    virtual void                    Render();
    virtual void                    setGLState();
    virtual void                    SetFrameBuffer();
    virtual void                    SetMatrix();
    virtual void                    SetGLAttribs();
            void                    SetProjection();
    void                            Append(RenderPass<GusGLObject>&);
    void                            ClearPass(){passToRender.clear();};
    
        protected :
    vector<RenderPass<GusGLObject>>     passToRender;
    RenderPass<GusGLObject>*            activeRenderPass;
    GLfloat                             PROJ[16];



};


class RenderShadow : public Renderer {
        public :
    static   GLuint                         GetShadowMap(){return depthMap;}
    static GLfloat                          VIEW[16];
    static GLfloat*							GetShadowView(){return VIEW;}
    RenderShadow();

    virtual void                            Init();
    virtual void                            Render();
    virtual void                            setGLState();
    virtual void                            SetFrameBuffer();
    virtual void                            SetMatrix();
    virtual void                            SetGLAttribs();
    void                                    SetCamera();
    GusVec3 								ComputeUp(GusVec3 eye,GusVec3 center);
    void                                    ComputeModelViewProjectionMatrix();
    void                                    ComputeModelViewInverseMatrix();
    GLfloat*                                GetShadowMatrix(){return VIEW;};
    
        private :
    GLuint                                  shadowBuffer;
    GLuint                                  shadowBufferSize;
 static   GLuint                                  depthMap;
    GLuint                                  depthRenderbuffer;
    
    Entity*                                 currentEntity;
    
    GLfloat                                 MODEL[16];
    GLfloat                                 PROJ[16];
    GLfloat                                 MODELVIEW[16];
    GLfloat                                 MVP[16];
    GLfloat                                 MV_I[9];
    GLfloat                                 MV3[9];
    GLfloat                                 xPos, yPos,zPos;
    GLfloat                                 xDir, yDir, zDir;
    GLfloat                                 angle;
    GLuint                                  pointCount;

    GusGLObject*							light;
};



class RenderColor : public Renderer {
        public :
    RenderColor();
   
    virtual void                            Init();
    virtual void                            Render(Entity* entity);
    virtual void                            setGLState();
    virtual void                            SetFrameBuffer();
    virtual void                            SetMatrix();
    virtual void                            SetGLAttribs();
    
    void                                    OpenFullPipeline();
    void                                    TranslateModel(GLfloat dx, GLfloat dy, GLfloat dz);
    void                                    ComputeModelViewMatrix();
    void                                    ComputeModelViewInverseMatrix();
    void                                    ComputeModelViewProjectionMatrix();
    void                                    ComputeShadowProjectionMatrix();
    void                                    initWithEntity();
    void                                    SetShadowProjMatrix(GLfloat* mvp)
                                                {CopyMatrix(MVP_SHADOW, mvp);};
    
        protected :

    GLfloat                                 MODEL[16];
    GLfloat                                 VIEW[16];
    GLfloat                                 PROJ[16];
    GLfloat                                 MODELVIEW[16];
    GLfloat                                 MVP[16];
    GLfloat                                 MV_I[9];
    GLfloat                                 MV3[9];
    GLfloat                                 xPos, yPos,zPos;
    GLfloat                                 xDir, yDir, zDir;
    GLfloat                                 angle;
    
    GLfloat                                 MVP_SHADOW[16];
    
    
    Entity*                                 currentEntity;
    Shader*                                 currentShader;
    Camera*                                 currentCamera;
    Light*                                  currentLight;
    Light*                                  currentLight0;

    vector<GusGLObject*>					passLights;
};





class RenderColorDBG : public RenderColor {
        public :

    static  bool                    shadowOn;
    static  bool                    textureOn;
    static map<DEBUG_MODE,bool>     isModeEnable;
    
    static  void                    InitModeCapability();
    static  GusVec3                 rayFrom;
    static  GusVec3                 rayTo;
    
    RenderColorDBG();
    
    virtual void                            Init();
    virtual void                            Render(EntityDBG* entity, DEBUG_MODE mode);
    void                                    RenderRay(GusVec3 from, GusVec3 to);
    virtual void                            setGLState();
    virtual void                            SetFrameBuffer();
    virtual void                            SetMatrix();
    virtual void                            SetGLAttribs();
    void                                    OpenFullPipeline();
    
           private :
    DEBUG_MODE                              currentMode;
    GLuint                                  rayVBO;
    GLfloat                                 rayVertex[6];

};



















