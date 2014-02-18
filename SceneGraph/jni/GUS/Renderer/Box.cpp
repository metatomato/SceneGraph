//
//  Box.cpp//  helloGL
//
//  Created by tomato on 4/3/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Box.h"
#include "../Scene/Scene.h"


static map<DEBUG_MODE,bool>        mode;



Box::Box():VBO(0),VBO_IDX(0){
    
}

Box::Box(Box_t& in_box):VBO(0),VBO_IDX(0){
    for (int i = 0 ; i < 24 ; i++) {
        vertex[i] = in_box.box[i];
    }
    attribs.name = "Box";
    attribs.material = new Material();
    attribs.material->SetColor(GusVec3(1.0,0.5,0.0));
    SetShader(COLOR, Shader::GetShaderObject(0x10));
    
    GenerateVBO();
}

Box::~Box(){
    delete attribs.material;
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO_IDX);
  //  cout << "[BOX][~Box] BOX DESTRUCTOR CALL!!" << endl;
}

void Box::SetGLAttribs(DEBUG_MODE mode){
    
    GetMaterial()->SetGLAttribs(shaderSet.shadersDBG[BOX_MODE]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(shaderSet.shadersDBG[BOX_MODE]->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(shaderSet.shadersDBG[BOX_MODE]->GetMeshAttribLocation(0));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_IDX);
}


void Box::SetModel(DEBUG_MODE mode){
    if (!stageObjRef) {
        LoadIdentity4fv(MODEL);
    }
    Sync();
    glUniformMatrix4fv(shaderSet.shadersDBG[BOX_MODE]->GetEntityUniformLocation(0), 1, GL_FALSE, MODEL);
}

void Box::GenerateVBO(){
    glGenBuffers(1, &VBO);
    glBindBuffer( GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, 24*sizeof( GLfloat ), (GLfloat *) vertex, GL_STATIC_DRAW );
    
    glGenBuffers(1, &VBO_IDX);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO_IDX );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 24*sizeof( GLuint ), (GLuint *) Box_t::boxIndex, GL_STATIC_DRAW );
    
}



void Box::Render(DEBUG_MODE mode){
    glDrawElements(GL_LINES,24, GL_UNSIGNED_INT, 0);
}



////////////////////////////////////////////////////////////////////////////////////////
//      BOXSET METHOD DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////



BoxSet::BoxSet(const vector<SceneObject*>& vec_obj):num(vec_obj.size()*24){
    for(auto obj : vec_obj){
        GLfloat* data = ((StageObject*)obj)->GetData();
        vector<GLfloat> ordered_vec(24,0.0f);
        for (int i = 0; i < 24; i++) {
            ordered_vec[i] = data[Box_t::boxIndex[i]];
        }
        vertex.push_back(ordered_vec);
    }
    
    attribs.name = "BoxSet";
    attribs.material = new Material();
    attribs.material->SetColor(GusVec3(1.0,0.5,0.0));
    SetShader(COLOR, Shader::GetShaderObject(0x10));
    
    GenerateVBO();
}


void BoxSet::GenerateVBO(){
    glGenBuffers(1, &VBO);
    glBindBuffer( GL_ARRAY_BUFFER, VBO);
    glBufferData( GL_ARRAY_BUFFER, num * sizeof( GLfloat ), (GLfloat *) vertex.data(), GL_STATIC_DRAW );
}


void BoxSet::SetModel(RENDER_PASS type) {
    LoadIdentity4fv(MODEL);
    glUniformMatrix4fv(shaders[type]->GetEntityUniformLocation(0), 1, GL_FALSE, MODEL);
}


void BoxSet::SetGLAttribs(RENDER_PASS type){
    
    GetMaterial()->SetGLAttribs(shaders[type]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(shaders[type]->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(shaders[type]->GetMeshAttribLocation(0));
}



void BoxSet::Render(){
    glDrawArrays(GL_LINES, 0 , num);
}














































