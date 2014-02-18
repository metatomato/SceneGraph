//
//  GLUS.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//


#pragma once

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#define  LOG_TAG    "ENGINE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

#ifdef IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define LOGE(...) printf(__VA_ARGS__)
#endif

#include <iostream>
#include <stdio.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <queue>
#include <math.h>
#include <stack>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "GusObject.h"
#include "Matrix.h"
#include "../System/InputRef.h"



#define GUS_MAX_TEXTURES 6
const int GUS_MAX_ATTRIBUTES = 6;
#define GUS_LIGHT_TYPE 3
const int GUS_MATERIAL_ATTRIBS_TYPE=10;
#define GUS_MESH_ATTRIBS_TYPE 4
const int LIGHT_ATTRIBS = 4;
#define GUS_ENTITY_ATTRIBS_TYPE 1
#define GUS_CAMERA_ATTRIBS_TYPE 1
const int GUS_RENDERER_ATTRIBS_TYPE = 5;

#define GUS_DEBUG 0

const int RENDER_PASS_MAX = 5;
const int DBG_ATTRIB_MAX = 9;
const int DBG_UNIFORM_MAX = 1;

#define ALPHA 0x1906
#define RGB 0x1907
#define RGBA 0x1908
/*

#ifdef IOS
#define SCREEN_X 1024
#define SCREEN_Y 768
#endif

#ifdef ANDROID
const int SCREEN_X = 1920;
const int SCREEN_Y = 1200;
#endif
*/

using namespace std;


enum RENDER_PASS {SHADOW, COLOR,  ENV, PEELING, GDEBUG, SCISSOR, STENCIL};

enum SCENE_PASS {S_OBJ, S_BOX , S_SUB_OBJ, S_PRIMITIVE};

enum LIGHT_TYPE { DIRECTIONAL , POINT , SPOT };

enum MESH_ATTRIBS { VERTEX , NORMAL , TANGENT , UV , INDEX_TRI , INDEX_LIN , ALL , BOX};

enum MATERIAL_ATTRIBS { ALBEDO ,      DIFFUSE,       SPECULAR ,    
                        ALBEDO_MAP  , DIFFUSE_MAP ,  SPECULAR_MAP,
                        CAST_SHADOW , SELF_SHADOW ,  RECEIVE_SHADOW,
                        EMIT};

enum TEXTURE_TYPE { COLOR_MAP  , BUMP_MAP ,   LIGHT_MAP,
                    SHADOW_MAP , ENV_MAP};

enum VBO_DBG { VERTEX_DBG, NORMAL_DBG , TANGENT_DBG ,NORMAL_LINE_DBG , TANGENT_LINE_DBG , BOX_DBG, BOX_IDX_DBG, INDEX_LIN_DBG , INDEX_TRI_DBG };

enum DEBUG_MODE {NONE, POINT_MODE,LINE_MODE,TRI_MODE,BOX_MODE, NORMAL_MODE, TANGENT_MODE, RAY_MODE};

enum class AXIS { X_POS , X_NEG , Y_POS , Y_NEG , Z_POS , Z_NEG };

struct GusVec3{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    
    GusVec3(GLfloat i, GLfloat j, GLfloat k):
        x(i),
        y(j),
        z(k)
    {
    }
    
    GusVec3():
    x(0.f),
    y(0.f),
    z(0.f)
    {
    }
    
    GLfloat Length()const{
        return sqrtf(x*x+y*y+z*z);
    }
    
    void Normalize(){
        float norm = sqrtf(x*x+y*y+z*z);
        x /= norm;
        y /= norm;
        z /= norm;
    }
    const GLfloat* value() const {
//        GLfloat values[]={x,y,z};
        return &x;
    }
    GLfloat operator[](int i){
        return *(&x+i);
    }
    GusVec3 operator-(GusVec3 vec) const{
        return GusVec3(x-vec.x,y-vec.y,z-vec.z);
    }
    GusVec3 operator+(GusVec3 vec) const{
       return GusVec3(x+vec.x,y+vec.y,z+vec.z);
    }
    GusVec3 operator/(GLfloat div)const{
      return GusVec3(x/div,y/div,z/div);
    }
    GusVec3 operator*(GLfloat div) const{
        return GusVec3(x*div,y*div,z*div);
    }
    GusVec3& operator+=(GusVec3 vec){
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }
    GusVec3& operator+=(float f){
        x += f;
        y += f;
        z += f;
        return *this;
    }
    GusVec3& operator/=(GLfloat div){
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }
    GusVec3& operator*=(GLfloat scalar){
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    GusVec3& operator*=(GusVec3 vec){
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;
        return *this;
    }
    string Print(){
        ostringstream output;
        output << x << " " << y << " " << z;
        return output.str();
    }
    
static GusVec3 ChangeLengthBy(const GusVec3& from, const GusVec3& to, float by) {
        float factor = by/(to - from).Length();
        return GusVec3((to * (1 + 1/factor) - from) * factor);
    }
};

struct GusVec2{
    GLfloat x;
    GLfloat y;
    
    GusVec2(GLfloat i, GLfloat j){
        x = i ;
        y = j ;
    }
    
    GusVec2(){
        x = 0.0f ;
        y = 0.0f ;
    }
};

struct GusVec3i{
    GLuint x;
    GLuint y;
    GLuint z;
    
    GusVec3i(GLuint i, GLuint j, GLuint k){
        x = i ;
        y = j ;
        z = k ;
    }
    
    GusVec3i(){
        x = 0 ;
        y = 0 ;
        z = 0 ;
    }
    
    GusVec3i(const GusVec3i& vec){
        x = vec.x ;
        y = vec.y ;
        z = vec.z ;
    }
const GLuint* value() const {
        return &x;
    }
    
    GLuint operator[](int i){
        return *(&x+i);
    }
};



template <class U, class T>
vector<T*> Translate(const vector<U*>& vec) {
    vector<T*> trans_vec;
    T* trans_obj = dynamic_cast<T*>(vec.front());
    if (trans_obj) {
        trans_vec = vector<T*>((T**)vec.data(),(T**)vec.data()+vec.size());
    }
    return trans_vec;
}



template <class U, class T>
vector<const T*> Translate(const vector<const U*>& vec) {
    vector<const T*> trans_vec;
    const T* trans_obj = dynamic_cast<const T*>(vec.front());
    if (trans_obj) {
        trans_vec = vector<const T*>((T**)vec.data(),(T**)vec.data()+vec.size());
    }
    return trans_vec;
}



template <class T>
pair<int,vector<T*> > VectorToLibrary(const vector<GusObject*>& vec){
    vector<T*> trans_vec;
    pair<int, vector<T*> > vec_pair;
    T* trans_obj = dynamic_cast<T*>(vec.front());
    if (trans_obj) {
        vec_pair = pair<int, vector<T*> >(trans_obj->_GetType(),Translate<GusObject,T>(vec));
    }
    return vec_pair;
}



template <const map<string,int>* map_ptr>
class RefMap {
        public:
    RefMap():lastId(map_ptr->size())
    {
        for(auto p : *map_ptr) { refMap.insert(p); }
    };
    
    void    SetType(string s) { if(refMap.count(s)==0){ refMap[s] = lastId++; } };
    int     GetType(string s){ return refMap[s]; };
    
        private :
    int    lastId;
    map<string, int> refMap;
};



namespace GUS {
    
typedef struct GLtgaimage_t
{
	GLushort	width;
	GLushort	height;
	GLubyte*	data;
	GLenum 	format;
} GLtgaimage_t;

    char* readShader(string file);
    GLvoid destroyTgaImage(GUS::GLtgaimage_t* tgaimage);
    GLvoid swapColorChannel(GLuint width, GLuint height, GLenum format, GLubyte* data);
    GLboolean  loadTgaImage(const char* filename, GUS::GLtgaimage_t* tgaimage);

}















