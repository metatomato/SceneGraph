//
//  Mesh.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Mesh.h"
#include "../Scene/Scene.h"



const string attribs_list[] = { "vertex","normal","tangent","texCoord"};
const vector<string> Mesh::attribsList(attribs_list,attribs_list+4);
int Mesh::triangleTotalCount = 0;


map <int, GLuint*> Mesh::GLRef;

Mesh::Mesh():faceAttribs(new MeshFaceAttribs_t()),vertexAttribs(new MeshVertexAttribs_t()) {
    
    for( int i = 0 ; i < GUS_MAX_ATTRIBUTES ; i++ ) {
        VBO [ i ] = 0;
    }
    
   
}

Mesh::Mesh(const Mesh& mesh) {
   
    attribs = mesh.attribs;
    vertexAttribs = mesh.vertexAttribs;
    faceAttribs = mesh.faceAttribs;
}

Mesh::Mesh (const MeshVertexAttribs_t& v_attribs){
   
    attribs.name = v_attribs.parent;
    faceAttribs  = new MeshFaceAttribs_t();
    vertexAttribs = new MeshVertexAttribs_t(v_attribs);
}

Mesh::Mesh (string name):faceAttribs(new MeshFaceAttribs_t()),vertexAttribs(new MeshVertexAttribs_t()){
   
    attribs.name = name;
}

Mesh::Mesh(MeshVertexAttribs_t& mesh_arg,MeshFaceAttribs_t& face_arg,string name):
        vertexAttribs(&mesh_arg),
        faceAttribs(&face_arg)
{
    attribs.name = name;
}

Mesh::~Mesh () {
    cout << "[MESH][~Mesh] MESH DESTRUCTOR CALL" << endl;
    delete faceAttribs;
    delete vertexAttribs;
    }


void Mesh::_SetAttrib(string attrib_name, string value){
    attribs.Call(attrib_name, value);
}

void Mesh::_SetPostAttrib() {
        GenAllVBO();
        MapAllVBO();
}


void Mesh::GenVBO( MESH_ATTRIBS attrib ) {
    glGenBuffers( 1 , &VBO[ attrib ] );
}

void Mesh::BindVBO( MESH_ATTRIBS attrib ) {
    if(attrib == INDEX_LIN || attrib == INDEX_TRI) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO [ attrib ]);
    else glBindBuffer( GL_ARRAY_BUFFER, VBO[ attrib ] );
}

void Mesh::MapVBO( MESH_ATTRIBS attrib ) {
    switch (attrib) {
        case VERTEX:
            glBindBuffer( GL_ARRAY_BUFFER, VBO[attrib] );
            glBufferData( GL_ARRAY_BUFFER, faceAttribs->triangleCount*9*sizeof( GLfloat ), (GLfloat *) faceAttribs->vertex, GL_STATIC_DRAW );
            break;
            
        case NORMAL:
            glBindBuffer( GL_ARRAY_BUFFER, VBO[ attrib ] );
            glBufferData( GL_ARRAY_BUFFER, faceAttribs->triangleCount*9*sizeof( GLfloat ), (GLfloat *) faceAttribs->normal, GL_STATIC_DRAW );
            break;
        case TANGENT:
            glBindBuffer( GL_ARRAY_BUFFER, VBO[ attrib ] );
            glBufferData( GL_ARRAY_BUFFER, faceAttribs->triangleCount*9*sizeof( GLfloat ), (GLfloat *) faceAttribs->tangent, GL_STATIC_DRAW );
            break;
        case UV:
            glBindBuffer( GL_ARRAY_BUFFER, VBO[ attrib ] );
            glBufferData( GL_ARRAY_BUFFER, faceAttribs->triangleCount*6*sizeof( GLfloat ), (GLfloat *) faceAttribs->textureUV, GL_STATIC_DRAW );
            break;
        case INDEX_LIN:
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[ attrib ] );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, faceAttribs->triangleCount*6*sizeof( GLuint ), (GLuint *) faceAttribs->indexLine, GL_STATIC_DRAW );
            break;
        case INDEX_TRI:
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[ attrib ] );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, faceAttribs->triangleCount*3*sizeof( GLuint ), (GLuint *) faceAttribs->indexTri, GL_STATIC_DRAW );
            break;
        default:
            break;
    }
}

void Mesh::GenAllVBO( ) {
    //glGenVertexArraysOES(1, &VAO);
    //glBindVertexArrayOES(VAO);
    GenVBO(VERTEX);
    GenVBO(NORMAL);
    GenVBO(UV);
    GenVBO(TANGENT);
    GenVBO(INDEX_TRI);
    GenVBO(INDEX_LIN);
}


void Mesh::MapAllVBO( ) {
   // glBindVertexArrayOES(VAO);
    MapVBO(VERTEX);
    MapVBO(NORMAL);
    MapVBO(UV);
    MapVBO(TANGENT);
    MapVBO(INDEX_TRI);
    MapVBO(INDEX_LIN);
}


void Mesh::SetGLAttribs(Shader* shader){
 
    GLuint max = 100;
  //  glBindVertexArrayOES(VAO);
    if (shader->GetMeshAttribLocation(0)<max) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX]);
        glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
        glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
    }
    
    if (shader->GetMeshAttribLocation(1)<max) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL]);
    glVertexAttribPointer(shader->GetMeshAttribLocation(1), 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(shader->GetMeshAttribLocation(1));
    }
    
    if (shader->GetMeshAttribLocation(2)<max) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO[TANGENT]);
    glVertexAttribPointer(shader->GetMeshAttribLocation(2), 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(shader->GetMeshAttribLocation(2));
    }
    
    if (shader->GetMeshAttribLocation(3)<max) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO[UV]);
    glVertexAttribPointer(shader->GetMeshAttribLocation(3), 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(shader->GetMeshAttribLocation(3));
    }
 }


void Mesh::BindVAO(){
   // glBindVertexArrayOES(VAO);
}

void Mesh::GenNormTan(MeshFaceAttribs_t** attribs) const{
    *attribs = new MeshFaceAttribs_t();
    (*attribs)->vertex       = faceAttribs->vertex;
    (*attribs)->normal       = faceAttribs->normal;
    (*attribs)->tangent      = faceAttribs->tangent;
    (*attribs)->normal_line  = new GLfloat [ faceAttribs->triangleCount*9*2 ];
    (*attribs)->tangent_line = new GLfloat [ faceAttribs->triangleCount*9*2 ];
    //(*attribs)->box          = vertexAttribs->box;
    //(*attribs)->boxIndex     = vertexAttribs->boxIndex;
    
   // (*attribs)->triangleCount = vertexAttribs->triangleCount;
    (*attribs)->triangleCount = faceAttribs->triangleCount;
    
  //  cout << "[MESH][GenNormTan] " << _GetName() << " " << vertexAttribs->triangleCount << endl;
    
    GLfloat scale_factor = 0.1;
    
    for (int idx  = 0 ; idx < faceAttribs->triangleCount*3 ; idx++ ) {
        for (int i  = 0 ; i < 3; i++ ) {
            for (int j = 0 ; j < 3 ; j++) {
                
                (*attribs)->normal_line[ idx*3*2  + j   ]   = faceAttribs->vertex[idx*3 + j];
                
                (*attribs)->normal_line[ idx*3*2  + j + 3]  = faceAttribs->vertex[idx*3 + j] + scale_factor * faceAttribs->normal[idx*3 + j];
                
                
                (*attribs)->tangent_line[ idx*3*2  + j   ]   = faceAttribs->vertex[idx*3 + j];
                
                (*attribs)->tangent_line[ idx*3*2  + j + 3]  = faceAttribs->vertex[idx*3 + j] + scale_factor * faceAttribs->tangent[idx*3 + j];
            }
        }
    }
    
}


void Mesh::CopyVertexAttribsDBG(Mesh *in_mesh) const{
    GenNormTan(&(in_mesh->faceAttribs));
    in_mesh->vertexAttribs = vertexAttribs;
    in_mesh->attribs.name = attribs.name + "_DBG";
    in_mesh->VBO_DBG[VERTEX_DBG] = VBO[VERTEX];
    in_mesh->VBO_DBG[NORMAL_DBG] = VBO[NORMAL];
    in_mesh->VBO_DBG[TANGENT_DBG] = VBO[TANGENT];
    in_mesh->VBO_DBG[INDEX_LIN_DBG] = VBO[INDEX_LIN];
    in_mesh->VBO_DBG[INDEX_TRI_DBG] = VBO[INDEX_TRI];
}


bool Mesh::IsNested() {
    bool result;
    if(vertexAttribs->parent != "") {
        result = true;
    } else {
        result = false;
    }
    return result;
}


void Mesh::SetPostAttribDBG(){
    glGenBuffers(1, &VBO_DBG[NORMAL_LINE_DBG]);
    glGenBuffers(1, &VBO_DBG[TANGENT_LINE_DBG]);
    glGenBuffers(1, &VBO_DBG[BOX_DBG]);
    glGenBuffers(1, &VBO_DBG[BOX_IDX_DBG]);

   // glGenVertexArraysOES(1, &VAO_DBG);
    
   // glBindVertexArrayOES(VAO_DBG);
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO_DBG[NORMAL_LINE_DBG] );
    glBufferData( GL_ARRAY_BUFFER, faceAttribs->triangleCount*9*2*sizeof( GLfloat ), (GLfloat *) faceAttribs->normal_line, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO_DBG[TANGENT_LINE_DBG] );
    glBufferData( GL_ARRAY_BUFFER, faceAttribs->triangleCount*9*2*sizeof( GLfloat ), (GLfloat *) faceAttribs->tangent_line, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO_DBG[BOX_DBG] );
    glBufferData( GL_ARRAY_BUFFER, 24*sizeof( GLfloat ), (GLfloat *) vertexAttribs->AABB.box, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO_DBG[BOX_IDX_DBG] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 24*sizeof( GLuint ), (GLuint *) vertexAttribs->AABB.boxIndex, GL_STATIC_DRAW );
}

//enum DEBUG_MODE {POINT_MODE,LINE_MODE,TRI_MODE,BOX_MODE, NORMAL_MODE, TANGENT_MODE};

void Mesh::SetGLAttribsDBG(Shader* shader, DEBUG_MODE mode){
    
   
    switch (mode) {
        case NORMAL_MODE :
            glBindBuffer(GL_ARRAY_BUFFER, VBO_DBG[NORMAL_LINE_DBG]);
            glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
            break;
            
        case TANGENT_MODE :
            glBindBuffer(GL_ARRAY_BUFFER, VBO_DBG[TANGENT_LINE_DBG]);
            glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
            break;
            
        case BOX_MODE :
            glBindBuffer(GL_ARRAY_BUFFER, VBO_DBG[BOX_DBG]);
            glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_DBG[BOX_IDX_DBG]);
            break;
            
        case POINT_MODE :
            glBindBuffer(GL_ARRAY_BUFFER, VBO_DBG[VERTEX_DBG]);
            glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
            
            break;
        case LINE_MODE :
            //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO_DBG[INDEX_LIN_DBG] );
            glBindBuffer(GL_ARRAY_BUFFER, VBO_DBG[VERTEX_DBG]);
            glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_DBG[INDEX_LIN_DBG]);
            break;
        case TRI_MODE :
            glBindBuffer(GL_ARRAY_BUFFER, VBO_DBG[VERTEX_DBG]);
            glVertexAttribPointer(shader->GetMeshAttribLocation(0), 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(shader->GetMeshAttribLocation(0));
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO_DBG[NORMAL_DBG]);
            glVertexAttribPointer(shader->GetMeshAttribLocation(1), 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(shader->GetMeshAttribLocation(1));
            
            break;
        case NONE:
            break;
        case RAY_MODE:
            break;
       
    }
     
}

GusVec3* Mesh::_GetVecPosition() const {
GusVec3* pos = new GusVec3();
return pos;
}


GusVec3* Mesh::GetHalfVec(){
    return &(vertexAttribs->AABB.halfVec);
}





