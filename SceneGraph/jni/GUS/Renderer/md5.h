#pragma once

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#ifdef IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif


#include "md5definitions.h"

#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GusGLObject.h"
#include  "Mesh.h"

class md5Animation;

using namespace std;

#define READ_BUFF_SIZE 1024

struct md5Attribs_t{
    typedef void (md5Attribs_t::*fctPtr)(string);
    
    string          name;
    string          mesh;
    vector<string>  anim;
    
    
    map<string, fctPtr> mapFunction;
    
    md5Attribs_t(){
        mapFunction["name"]      = &md5Attribs_t::Name;
        mapFunction["mesh"]      = &md5Attribs_t::Mesh;
        mapFunction["anim"]      = &md5Attribs_t::Anim;
        
        name = "default_md5";
        mesh = "";
    }
    
    void Name(string is) {
        char temp[64];
        sscanf(is.c_str(), "%s",temp);
        name = temp;
        cout << "[MD5OBJECT][md5Attribs_t] " << name << endl;
    }
    void Mesh(string is) {
        char temp[64];
        sscanf(is.c_str(), "%s",temp);
        mesh = temp;
        cout << "[MD5OBJECT][md5Attribs_t] "  << mesh << endl;
    }
    void Anim(string is) {
        char temp[64];
        sscanf(is.c_str(), "%s",temp);
        anim.push_back(temp);
        cout << "[MD5OBJECT][md5Attribs_t] " << anim[0]  << endl;
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


class md5Object : public GusGLObject {

	//enum action {WAIT,WALK,JUMP,ATTACK,DIR_JUMP,DIR_ATTACK,DIR_JUMP_ATTACK,JUMP_ATTACK};

	public:
		md5Object();
		~md5Object();
    
    const static GUS_OBJ_ID         type = GUS_OBJ_ID::MD5;
    
    virtual void                    _SetAttrib(string attrib_name, string value);
    virtual void                    _SetPostAttrib();
    virtual void                    _SetAttribFromChild(GusGLObject* child);


    virtual GusVec3*                _GetVecPosition(){return new GusVec3();};
    string                          _GetName()const{return attribs.name;};
    
    
		void set(const char* md5mesh, const char* md5anim);
		void load();
		void getAttributeLocation(int shaderprogram);
		void render();
		void update(int action);
		void updatePosition(float flag, float flag0);
		void updateAnim();
		void RenderShadow();
		GLint tangentLoc;
    
    GusVec3             GetBoxMin(int frame_idx);
    GusVec3             GetBoxMax(int frame_idx);
    MeshVertexAttribs_t*                GetAttribs() {return vertexAttribs;};
    void                                ComputeBoundingBox();
		
	private:
        MeshVertexAttribs_t*        vertexAttribs;
        md5Attribs_t                attribs;
		enum state {REPEAT,BREAK,SWITCH,PLAY};
		GLuint oyo;

		//Model data
		md5joint* joints;
		int jointsCount;

		md5Mesh* meshes;
		int meshesCount;
	
		md5Animation* animation ;
		float animCursor;

		void renderSkin(int meshIndex);
		void prepareVertexArray(int meshIndex);
			
		//Methods used to read the mesh
		int loadingStatus ;
		int version;
		char commandline[READ_BUFF_SIZE];
		void loadMesh(FILE* fp);
		char buff[READ_BUFF_SIZE];    //Reading buffer (md5 model are read line by line, a line is not expected to be more than 512bytes)
		void readVersion(FILE* fp);
		void getJointsCount(FILE* fp);
		void getMeshesCount(FILE* fp);
		void readCommandLine(FILE* fp);
		void readJoints(FILE* fp);
		void readMeshes(FILE* fp);
	
		//Post parsing
		//void loadShaders();

		void generateNormalsAndTangents();
		

		//Anim part
		GLint vertexLocation, indicelocation, normalLocation, texCoordsLocation, tangentLocation, vertexShadowLocation;
		
				// Used for rendition in openGL, double pointer because each mesh has it's own vertex/element array
		int** elementsSkin;
        int** elementsSkin_0;
		float** vertexArraySkin;
		float** normalArraySkin;
		float** tangentArraySkin;
		float** texturArraySkin;

		float3 translatePos;

		//OpenGL VBO optimisation
		GLuint  *vboVertexId;
		GLuint  *vboNormalId;
		GLuint  *vboTangentId;
		GLuint  *vboTexturId;
		GLuint  *vboElementsId;
        GLuint  *vboElementsId_0;
		GLuint  *vaoId;
		

		string md5mesh;
		string md5anim;
};


