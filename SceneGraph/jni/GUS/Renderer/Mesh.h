//
//  Mesh.h
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#pragma once


#include <iostream>
#include "../Common/GUS.h"
#include "Shader.h"
#include "GusGLObject.h"
#include "Mesh.h"
#include "../Scene/SpaceObject.h"

using namespace std;

enum MESH_TYPE {OBJ, M5D};
enum BOX { X_MAX , X_MIN , Y_MAX , Y_MIN , Z_MAX , Z_MIN };



const uint BOX_SIZE = 6 ;

struct MeshAttribs_t {
    typedef void (MeshAttribs_t::*fctPtr)(string);
    string name;
    map<string, fctPtr> mapFunction;
    
    MeshAttribs_t(){
         mapFunction["name"] = &MeshAttribs_t::Name;
        
    }
    void Name(string is) {
        char temp[64];
        sscanf(is.c_str(), "%s",temp);
        name = temp;
    }
    vector<string> GetAttribsList() {
        vector<string> vec;
        map<string, fctPtr>::iterator it = mapFunction.begin();
        for (; it != mapFunction.end(); ++it) {
            vec.push_back(it->first);
        }
        return vec;
    }
    void Call( const string & s, string is ) {
    	fctPtr fp = mapFunction[s];
    	return (this->*fp)(is);
    }
};


struct MeshFaceAttribs_t {
    string                              meshData;
    string                              material;
   
    GLfloat *                           vertex ;
    GLfloat *                           normal ;
    GLfloat *                           tangent ;
    GLfloat *                           normal_line;
    GLfloat *                           tangent_line;
    GLfloat *                           textureUV ;
    GLuint *                            indexLine;
    GLuint *                            indexTri;
    GLuint                              triangleCount ;
    
    MeshFaceAttribs_t(){
        meshData = "";
        material = "";
        vertex = NULL ;
        normal = NULL ;
        tangent = NULL ;
        normal_line = NULL;
        tangent_line = NULL;
        textureUV = NULL;
        indexTri = indexLine = NULL;
        triangleCount = 0 ;
    }
     ~MeshFaceAttribs_t(){
         delete vertex;
         delete normal;
         delete tangent;   
         delete normal_line;
         delete tangent_line;
         delete textureUV;
         delete indexLine;
         delete indexTri;
     }
};

struct MeshVertexAttribs_t {
    string                              parent;
    bool                                polyGroup;
    vector<string>                      child;
    
    GLuint                              vertexCount;
    GLuint                              faceCount;
    
    Box_t                               AABB;
    GusVec3                             originalCenter;
    
    MeshVertexAttribs_t(){
        vertexCount = 0 ;
        faceCount = 0 ;
        polyGroup = false;
        parent = "";
        originalCenter = GusVec3();
    }
};



class Mesh : public GusGLObject{
        public :
    static const GLuint *               GetGLRef(int idx) { return GLRef[idx]; };
    const static vector<string>         attribsList;
    static int                          triangleTotalCount;
        private :
    static map <int, GLuint*>           GLRef;
    
        public :
    Mesh () ;
    Mesh (string name) ;
    Mesh (const Mesh&) ;
    Mesh (const MeshVertexAttribs_t&) ;
    Mesh (MeshVertexAttribs_t&, MeshFaceAttribs_t&,string name);
    ~Mesh () ;
    virtual void                        _SetAttrib(string attrib_name, string value);
    virtual void                        _SetPostAttrib();
    virtual GusVec3*                    _GetVecPosition() const;
    virtual int                         _GetType() const {return (int)GUS_OBJ_ID::MESH;};
    string                              _GetName()const{return attribs.name;};

    void                                GenVBO( MESH_ATTRIBS attrib );
    void                                BindVBO( MESH_ATTRIBS attrib );
    void                                BindVAO();
    void                                MapVBO( MESH_ATTRIBS attrib );
    void                                GenAllVBO();
    void                                MapAllVBO();
    void                                SetGLAttribs(Shader* shader);
    
    const GLuint                        GetTriangleCount()const{return faceAttribs->triangleCount;};
    const GLuint                        GetVertexCount()const{return vertexAttribs->vertexCount;};
    void                                PrintData(MESH_ATTRIBS data_type);
    GLfloat*                            GetVertex(){return faceAttribs->vertex;};
    GLfloat*                            GetNormal(){return faceAttribs->normal;};

    void                                CopyVertexAttribute(MeshVertexAttribs_t* const attribs){vertexAttribs = attribs;};
    void                                GenNormTan(MeshFaceAttribs_t** attribs) const;
    bool                                IsNested();
    bool                                IsPolyGroup(){return vertexAttribs->polyGroup;};
    string                              GetParent() const { return vertexAttribs->parent;};
    string                              GetMaterial(){ return faceAttribs->material;};
    const MeshVertexAttribs_t*          GetAttribs() const {return vertexAttribs;};
    MeshVertexAttribs_t*                GetAttribs() {return vertexAttribs;};
    void                                SetAttribs(MeshVertexAttribs_t* ptr){vertexAttribs = ptr;};
    
    
    void                                CopyVertexAttribsDBG(Mesh * in_mesh)const ;
    void                                SetPostAttribDBG();
    void                                SetGLAttribsDBG(Shader* shader, DEBUG_MODE mode);
    GusVec3*                            GetHalfVec();
    GusVec3                             GetOriginalCenter(){return vertexAttribs->originalCenter;};
    const Box_t&                        GetBox() const {return vertexAttribs->AABB;};

        private :
    int                                 data_type;
    GLuint                              VBO [ GUS_MAX_ATTRIBUTES ] ;
    GLuint                              VAO;
    MeshVertexAttribs_t*                vertexAttribs;
    MeshFaceAttribs_t*                  faceAttribs;
    MeshAttribs_t                       attribs;
    
     GLuint                             VBO_DBG[DBG_ATTRIB_MAX];
     GLuint                             VAO_DBG;
};


