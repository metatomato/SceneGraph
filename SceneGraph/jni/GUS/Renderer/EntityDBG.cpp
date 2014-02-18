//
//  EntityDBG.cpp
//  helloGL
//
//  Created by tomato on 3/5/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "EntityDBG.h"
#include "../Scene/Scene.h"

const  string attribs_list[] = {"MODE"};
const  vector<string> EntityDBG::attribsList(attribs_list,attribs_list+1);


EntityDBG::EntityDBG(const Entity& entity):entityRef(&entity){
    attribs.name = entity._GetName() + "_DBG";
    attribs.mesh = new Mesh();
    attribs.material = new Material();
    CopyMatrix(MODEL, entity.GetModel());
    //MODEL = entity.GetMODEL();
    SetStageRef(*(entity.GetStageRef()));
    entity.GetMesh()->CopyVertexAttribsDBG(attribs.mesh);
    
    attribs.mesh->SetPostAttribDBG();
}

EntityDBG::~EntityDBG(){
    LOGE("[ENTITYDBG][~EntityDBG] ENTITYDBG DESTRUCTOR CALL \n");
}

void EntityDBG::SetModel(DEBUG_MODE mode){
    if (!GetStageRef()) {
        LoadIdentity4fv(MODEL);
    }else {
    	Sync();
    }
    glUniformMatrix4fv(shaderSet.shadersDBG[mode]->GetEntityUniformLocation(0), 1, GL_FALSE, MODEL);
}


void EntityDBG::SetGLAttribs(DEBUG_MODE mode){
  
    GetMaterial()->SetGLAttribs(shaderSet.shadersDBG[mode]);
  
}

//Memo : enum DEBUG_MODE {POINT_MODE,LINE_MODE,TRI_MODE,BOX_MODE, NORMAL_MODE, TANGENT_MODE};

void EntityDBG::Render(DEBUG_MODE mode){
         attribs.mesh->SetGLAttribsDBG(shaderSet.shadersDBG[mode],mode);
         GLuint vertexCount = attribs.mesh->GetTriangleCount() * 3 ;
 
        glUniform1i(shaderSet.shadersDBG[mode]->GetDebugUniformLocation(0), mode);
    
    switch (mode) {
        case POINT_MODE:
            glDrawArrays(GL_POINTS, 0 , attribs.mesh->GetTriangleCount()*3);
            break;
        case LINE_MODE:
            glDrawElements(GL_LINES, vertexCount*2, GL_UNSIGNED_INT, 0);
            break;
        case TRI_MODE:
            glDrawArrays(GL_TRIANGLES, 0 , attribs.mesh->GetTriangleCount()*3);
            break;
        case BOX_MODE:
            glDrawElements(GL_LINES,24, GL_UNSIGNED_INT, 0);
            break;
        case NORMAL_MODE:
            glDrawArrays(GL_LINES, 0 , attribs.mesh->GetTriangleCount()*3*2);
            break;
        case TANGENT_MODE:
            glDrawArrays(GL_LINES, 0 , attribs.mesh->GetTriangleCount()*3*2);
            break;
            
        default:
            break;
    }
    
}
