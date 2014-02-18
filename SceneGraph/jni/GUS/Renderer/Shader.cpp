//
//  Shader.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Shader.h"
#include "../Scene/Scene.h"
#include "../DataCenter/Import.h"
#include "EntityDBG.h"


map<int, GLuint>  Shader :: GLRef ;
map<int, Shader*> Shader :: shadersLibrary ;
map<int, pair<string,string>> Shader :: shaderSources ;
int               Shader :: lastId = 0 ;

Shader::Shader(){

}


Shader::Shader(char* vert, char* frag, int idx, bool b){
    AssignId(idx);
    GLId = Shader::LoadShaderFromSource(vert, frag);
    ParseAllAttribs();
}


Shader::Shader(string vert, string frag, int idx){

    AssignId(idx);
    GLId = Shader::LoadShaderFromString(vert, frag);
    ParseAllAttribs();
}



Shader::Shader(string vert, string frag){

    AssignId(GenIdx(vert));
    GLId = Shader::LoadShaderFromString(vert, frag);
    ParseAllAttribs();
}



Shader::Shader(string vert, string frag, Material& mat){
    GLId = Shader::LoadShaderFromString(vert.c_str(), frag.c_str());
    ParseAllAttribs(mat);
}


Shader::Shader(int ref, GLuint gl_ref){
    ID = ref;
    GLId = gl_ref;
    GLRef[ref] = gl_ref;
    shadersLibrary[ref] = this;
}

Shader::~Shader(){
    
}


void Shader::InitShadersFromSources() {
	cout << "[SHADER][InitShadersFromSources] INIT SHADER..." << endl;
	for(auto p : shaderSources) {
		int idx = p.first;
		pair<string,string> sources = p.second;
		Shader* shader = new Shader(sources.first.c_str(),sources.second.c_str(),idx,true);
		GLRef[idx] = shader->GLId;
		shadersLibrary[idx] = shader;
	}

	PrintShaderList();
}


void Shader::InitShaders() {
    cout << "[SHADER][InitShader] INIT SHADER..." << endl;
    
    LOGE("[SHADER][InitShader] INIT SHADER...");
    string name[] = {"color0000.vs","color0000.fs",
    				 "color0001.vs","color0001.fs",
    				 "color0011.vs","color0011.fs",
    				 "color0111.vs","color0111.fs",
    				 "color1001.vs","color1001.fs",
    				 "color1011.vs","color1011.fs",
    				 "color1111.vs","color1111.fs",
    				 "color10000.vs","color10000.fs",
    				 "color10001.vs","color10001.fs",
    				 "color10011.vs","color10011.fs",
    				 "color10111.vs","color10111.fs",
    				 "shadow0001.vs","shadow0001.fs",
    				};

    for(int i =  0 ; i < 12 ; i++) {
    	Shader* shader = new Shader(name[i*2],name[i*2+1]);
    	GLRef[shader->GetID()] = shader->GLId;
    	shadersLibrary[shader->GetID()] = shader;
    }
}

void Shader::PrintShaderList(){
    map<int, Shader*>::const_iterator it = shadersLibrary.begin();
    map<int, Shader*>::const_iterator it_end = shadersLibrary.end();
    cout << "[SHADER][PrintShaderList] SHADER LIST " << endl;
    for (; it!= it_end; ++it) {
        cout << "Shader " << hex << it->first << " GLId " << it->second->GLId << endl;
    }
}


ShaderSet Shader::GetCorrespondance(Material& mat) {
    ShaderSet set;
  //  int ref = 0x01;
    
    set.shaders[SHADOW] = shadersLibrary[0x0101];
    set.shaders[COLOR] = shadersLibrary[0x10];
    
    if( mat.isEnabled(DIFFUSE) ) {
        if (mat.isEnabled(RECEIVE_SHADOW)) {
            set.shaders[COLOR] = shadersLibrary[0x09];
        } else {
            set.shaders[COLOR] = shadersLibrary[0x01];
        }
        set.shaders[GDEBUG] = shadersLibrary[0x10];
    }
    
    if( mat.isEnabled(ALBEDO_MAP) ) {
        if (mat.isEnabled(RECEIVE_SHADOW)) {
           // set.shaders[COLOR] = shadersLibrary[0x0b];
        	set.shaders[COLOR] = shadersLibrary[0x09];
        } else {
            set.shaders[COLOR] = shadersLibrary[0x03];
        }
        set.shaders[GDEBUG] = shadersLibrary[0x13];
    }
    
    if( mat.isEnabled(DIFFUSE_MAP) ) {
        if (mat.isEnabled(RECEIVE_SHADOW)) {
           // set.shaders[COLOR] = shadersLibrary[0x0f];
        	set.shaders[COLOR] = shadersLibrary[0x09];
        } else {
            set.shaders[COLOR] = shadersLibrary[0x07];
        }
        set.shaders[GDEBUG] = shadersLibrary[0x17];
    }
    if( mat.isEnabled(EMIT)){
        set.shaders[COLOR] = shadersLibrary[0x00];
    }
    
  //   cout << "[SHADER][GetCorrespondance] " << mat._GetName() << " " << ref << endl;
    return set;
}



void Shader::ParseAllAttribs(){
	ParseAttribs();
	ParseMaterialAttribs();
}


void Shader::ParseAllAttribs(const Material& mat){
	ParseAttribs();
	ParseMaterialAttribs(mat);
}


void Shader::ParseAttribs() {
		ParseEntityAttribs();
	    ParseMeshAttribs();
	    ParseCameraAttribs();
	    ParseRendererAttribs();
	    ParseDebugAttribs();
	    ParseLightAttribs();
}




void Shader::ParseMaterialAttribs(const Material& mat) {
    for (int i = 0 ; i < Material::attribsList.size(); i++) {
        if(mat.isEnabled((MATERIAL_ATTRIBS)i)){
            materialUniformsLocation[i] = glGetUniformLocation(GLId, Material::attribsList[i].c_str());
          //  cout << "[SHADER][ParseMaterialAttribs] Uniform " << Material::attribsList[i] << " at " << materialUniformsLocation[i] << " GLProgram "<< GLId << endl;
        } else materialUniformsLocation[i] = -1;
    }
}

void Shader::ParseMaterialAttribs() {
    for (int i = 0 ; i < Material::attribsList.size(); i++) {
            materialUniformsLocation[i] = glGetUniformLocation(GLId, Material::attribsList[i].c_str());
         //   cout << "[SHADER][ParseMaterialAttribs] Uniform " << Material::attribsList[i] << " at " << materialUniformsLocation[i] << " GLProgram "<< GLId << endl;
    }
}


void Shader::ParseLightAttribs() {
    for (int i = 0 ; i < Light::attribsList.size(); i++) {
            lightUniformsLocation[i] = glGetUniformLocation(GLId, Light::attribsList[i].c_str());
         //   cout << "[SHADER][ParseLightAttribs] Uniform " << Light::attribsList[i] << " at " << lightUniformsLocation[i] << " GLProgram "<< GLId << endl;
        }
}



void Shader::ParseMeshAttribs() {
    for (int i = 0 ; i < Mesh::attribsList.size(); i++) {
            meshAttribsLocation[i] = glGetAttribLocation(GLId, Mesh::attribsList[i].c_str());
            cout << "[SHADER][ParseMeshAttribs] attrib " << Mesh::attribsList[i] << " at " << meshAttribsLocation[i] << " GLProgram "<< GLId << endl;
    }
}

void Shader::ParseEntityAttribs() {
    for (int i = 0 ; i < Entity::attribsList.size(); i++) {
        entityUniformsLocation[i] = glGetUniformLocation(GLId, Entity::attribsList[i].c_str());
     //   cout << "[SHADER][ParseEntityAttribs] Uniform " << Entity::attribsList[i] << " at " << GetEntityUniformLocation(i) << " GLProgram "<< GLId << endl;
        
    }
}

void Shader::ParseCameraAttribs() {
    for (int i = 0 ; i < Camera::attribsList.size(); i++) {
        cameraUniformsLocation[i] = glGetUniformLocation(GLId, Camera::attribsList[i].c_str());
      //  cout << "[SHADER][ParseCameraAttribs] Uniform " << Camera::attribsList[i] << " at " << cameraUniformsLocation[i] << " GLProgram "<< GLId << endl;
        
    }
}

void Shader::ParseRendererAttribs() {
    for (int i = 0 ; i < Renderer::attribsList.size(); i++) {
        rendererUniformsLocation[i] = glGetUniformLocation(GLId, Renderer::attribsList[i].c_str());
//        cout << "[SHADER][ParseRendererAttribs] Uniform " << Renderer::attribsList[i] << " at " << rendererUniformsLocation[i] << " GLProgram "<< GLId << endl;
    }
}

void Shader::ParseDebugAttribs() {
    for (int i = 0 ; i < EntityDBG::attribsList.size(); i++) {
        debugUniformsLocation[i] = glGetUniformLocation(GLId, EntityDBG::attribsList[i].c_str());
       // cout << "[SHADER][ParseEntityAttribs] Uniform " << EntityDBG::attribsList[i] << " at " << GetDebugUniformLocation(i) << " GLProgram "<< GLId << endl;
        
    }
}
/*
void Shader::ParseRendererAttribs() {
        rendererUniformsLocation[0] = glGetUniformLocation(GLId, "PROJ");
        rendererUniformsLocation[1] = glGetUniformLocation(GLId, "MVP");
        cout << "[SHADER][ParseRendererAttribs] Uniform " << " PROJ " << " at " << rendererUniformsLocation[0] << " GLProgram "<< GLId << endl;
            cout << "[SHADER][ParseRendererAttribs] Uniform " << " MVP " << " at " << rendererUniformsLocation[1] << " GLProgram "<< GLId << endl;
}
*/

uint Shader::LoadShaderFromString(string vert,string frag)
{
	LOGE("[SHADER][InitShader] FETCH FILES...\n");

	GLuint program;

	string path = Import::GetResourcesFolder() + "Shaders/";

    string fileToOpen =  path + vert;
    char * vertexsource = GUS::readShader(fileToOpen.c_str());
    
    LOGE( "FICHIER OUVERT %s \n", fileToOpen.c_str() );

    fileToOpen = path + frag;
    char * fragmentsource = GUS::readShader(fileToOpen.c_str());
    
   return LoadShaderFromSource(vertexsource,fragmentsource);
/*
 	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    
  	glShaderSource(vertexshader, 1, (const char**)&vertexsource, 0);
	glShaderSource(fragmentshader, 1, (const char**)&fragmentsource, 0);
    
	glCompileShader(vertexshader);
    
	glCompileShader(fragmentshader);
    
 	program = glCreateProgram();
    
 	//printf("[SHADER][LoadShader]shaderProgram ID : %d \n", program);
    
 	glAttachShader(program, vertexshader);
    
 	glAttachShader(program, fragmentshader);
    
	glLinkProgram(program);
    
	glUseProgram(program);
    
	return program;
	*/

}




uint Shader::LoadShaderFromSource(char* vertexsource,char* fragmentsource)
{
	LOGE("[SHADER][LoadShaderFromSource] LOAD SHADERS...\n");

	GLuint program;

 	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

  	glShaderSource(vertexshader, 1, (const char**)&vertexsource, 0);
	glShaderSource(fragmentshader, 1, (const char**)&fragmentsource, 0);

	glCompileShader(vertexshader);

	glCompileShader(fragmentshader);

 	program = glCreateProgram();

 	//printf("[SHADER][LoadShader]shaderProgram ID : %d \n", program);

 	glAttachShader(program, vertexshader);

 	glAttachShader(program, fragmentshader);

	glLinkProgram(program);

	glUseProgram(program);

	return program;

}



int MaskConverter(RENDER_PASS type, int value) {
    int results = -1;
	int tens = value%10;
	int hundreds = (value%100)/10;
	int thousands = (value%1000)/100;
	int ten_thousands = (value%10000)/1000;

	int hundred_thousands = (value%100000)/10000;

    cout << "10 : " << tens << endl <<"100 : " << hundreds << endl <<"1000 : " << thousands << endl <<"10000 : " << ten_thousands << endl <<"100000 : " << hundred_thousands << endl;

    if(type == RENDER_PASS::COLOR) {
    	 results = tens + hundreds * 2 + thousands * 4 + ten_thousands * 8 + hundred_thousands * 16 ;
    }
    if(type == RENDER_PASS::SHADOW) {
         results = tens + hundreds * 2 + thousands * 4 + ten_thousands * 8 + hundred_thousands * 16 + 256;
    }

    return results;
}



int GenIdx(string name) {

	int ret_idx;

	string temp_s = name.substr(0,5);

	if(temp_s.compare("color") == 0){

		ret_idx = MaskConverter(RENDER_PASS::COLOR, atoi(name.substr(5,name.size()-8).c_str()));
	}

	temp_s = name.substr(0,6);

	if(temp_s.compare("shadow") == 0){
		ret_idx = MaskConverter(RENDER_PASS::SHADOW, atoi(name.substr(6,name.size()-9).c_str()));
	}

	return ret_idx;
}











































































