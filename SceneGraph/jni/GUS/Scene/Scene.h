//
//  Scene.h
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once

#include "../Renderer/Entity.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Light.h"
#include "../Common/GUS.h"
#include "GusPhysics.h"
#include "SceneObject.h"
#include "../Common/RenderPass.h"
#include "../DataCenter/DataCenter.h"
#include "../Common/Interpolator.h"
#include "../Common/Time.h"
#include "../System/ChannelOffice.h"


using namespace std;

typedef StageObject::STAGE_OBJ_ID TYPE;


class Scene {
        public :
	enum MODE{SCENE,STAGE, SUB_OBJ, ENTITY, MAT,LOCK};
    //static  float                   elapsedTime;
    static  const SceneObject&      GetSelection(){return *selectedObject;};
    static  SceneBox*				GetSceneBox(){return sceneBox;}
    static  vector<int>		        GetSelections();
    

                                    Scene();
                                    ~Scene();
    void                            SetDataLink(DataCenter& data_ref){dataCentral = &data_ref;}
    void                            Init(SceneData* data);
    void                            InitStage(vector<SceneObject*>);
    void 							LoadStage(vector<int> idx);
    void                            UpdateScene(float time);
    void							SetMode(MODE mode);
    void							SetSubMode(MODE mode);
    void 							OneSubObjMode(int obj_idx, int sub_obj_idx);
    void							MatModePure(int mat_idx, int primitive);
    void 							MatMode(int obj_idx, int sub_obj_idx, int primitive = 0);
    void 							SetMatModePrimitive(int primitive = 0);
    void 							EntityMode(int obj_idx, int sub_obj_idx);
    void 							MeshMode(bool state);
    void 							AABBMode(bool state);
    void 							OneObjMode(int obj_idx);
    void 							OneObjMode(const StageObject& in_obj);
    void							LockMode(bool state);
    void							SnapshotMode(bool state){GetCamera().isInversed(state);}
    void							SkyViewMode(bool state){skyViewMode = state;}

    
    vector<RenderPass<SceneObject>>&        GetPass(){return passToRender;};
    void                                    SetPass(vector<RenderPass<SceneObject>> pass_in){passToRender = pass_in;};
    RenderPass<SceneObject>                 ObjToPass();
    RenderPass<SceneObject>                 ObjToPass(SceneObject&);
    RenderPass<SceneObject>                 ObjToPass(RENDER_PASS r_type, SceneObject&);
    RenderPass<SceneObject>                 ObjToPass(RENDER_PASS r_type);

    void                            OnTV (const Program*) const ;
    void                            SubcribChannel(ChannelOffice::CHANNEL);

    void                            SetPhysics(vector<SceneObject*>& vec);
    void 							RemovePhysics(vector<SceneObject*>& vec);
    void 							RemovePhysics(SceneObject* vec);
    void 							RegainPhysics(SceneObject* vec);
    void                            RayCast(float x, float y);
    void                            UpdatePhysicalWorld(float time);
    void 							UpdateNonPhysicals();
    
    const SceneCamera&              GetCamera() const {return *defaultCamera;};
    void                            SwitchCameraMode();
    void							ResetCamera();
    void							ZoomSkyCamera(float mag);

    SceneLight*						GetLight(int idx) {return (SceneLight*)(*sceneData)[StageObject::STAGE_OBJ_ID::LIGHT]->GetEntry(idx);}
    vector<SceneLight*> 			GetLights() {return Translate<SceneObject,SceneLight>((*sceneData)[StageObject::STAGE_OBJ_ID::LIGHT]->GetVector());}

    void							SetSelected(const SceneObject&);
    void							SetSelected(int idx);
    vector<SceneObject*>			GetSelected();
    void							NewSelectionBox(const vector<const StageObject*>& obj_vec);
    void 							ResetSelection();
    int								GetSelectedObject();
    int								GetSelectionOrder(int idx);
    

    vector<SceneObject*>			GetStageFromIndex(vector<int> idx);
    SceneObject* 					GetStageFromIndex(int idx);
    vector<int> 					GetIndexFromStage(vector<SceneObject*> stages);
    void							SetStageVisibility(int idx, bool state);
    void							HideAllExcept(const StageObject& obj);
    void							HideAll();

        private :

static const SceneObject*                       lastSelectedObject;
static const SceneObject*                       selectedObject;
static map<int,const SceneObject*>				selections;
static vector<int> 								selectionHistory;
static SceneBox*								sceneBox;

	DataCenter*									dataCentral;

	SceneBox*									selectionBox;

    vector<SceneObject*>                        objToRender;
    vector<SceneObject*>                        objOnStage;

    SceneData*                                  sceneData;
    
    SceneCamera*                                defaultCamera;
    SceneCamera*                                skyCamera;

    vector<RenderPass<SceneObject>>             passToRender;
    
    GusPhysicsEngine                            physicalWorld;
    
    bool                                        boxOn;
    
    Time                                        sceneTime;

    bool										skyViewMode;

    bool										sceneMode;
    bool										lockMode;
    bool										subObjectMode;
    bool										objectMode;
    bool										meshMode;
    bool										aabbMode;
    bool										matMode;
    int											matModePrimitive;
    int											matRef;
    bool										entityMode;
};





































