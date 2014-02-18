//
//  Material.cpp
//  helloGL
//
//  Created by metatomato on 1/21/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Material.h"
#include "../Scene/Scene.h"
#include "../DataCenter/Import.h"

const string attribs_list[] = {   "albedoColor" , "diffuseColor" , "specularColor",
                                  "albedoMap"   , "diffuseMap"   , "specularMap",
                                  "castShadow"  , "selfShadow"   , "receiveShadow"};
const vector<string> Material::attribsList(attribs_list,attribs_list+9);

map <int, GLuint*> Material::GLRef;


Material::Material() {
    textures[ALBEDO]=0;
    textures[DIFFUSE]=0;
    textures[SPECULAR]=0;
    
    attribs = new MaterialAttribs_t();
   
   
}

Material::~Material(){
   //     cout << "[MATERIAL][~Material] MATERIAL DESTRUCTOR CALL" << endl;
    delete attribs;
}

void Material::_SetAttrib(string attrib_name, string value){
    attribs->Call(attrib_name, value);
}

void Material::_SetPostAttrib() {
    if(attribs->isEnabled[ALBEDO_MAP])  LoadImage(COLOR_MAP);
    if(attribs->isEnabled[DIFFUSE_MAP]) LoadImage(BUMP_MAP);
    
}
void Material::SetGLUniforms(){
    
}

void Material::LoadImage(TEXTURE_TYPE type) {
    string file_to_open = Import::GetResourcesFolder() + "textures/";
    if(type == COLOR_MAP) file_to_open += attribs->albedoMap;
    if(type == BUMP_MAP)  file_to_open += attribs->diffuseMap;
    if(type == LIGHT_MAP) file_to_open += attribs->specularMap;
       
    GUS::GLtgaimage_t image;
    
    if(loadTgaImage(file_to_open.c_str(), &image)) {
        glGenTextures(1, &textures[type]);
        glBindTexture(GL_TEXTURE_2D, textures[type]);
        glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GUS::destroyTgaImage(&image);
        
    //    cout << "[MATERIAL][LoadImage] image open " << file_to_open << endl;
    } 
}

bool Material::isEnabled(MATERIAL_ATTRIBS type) const {
    return attribs->isEnabled.find(type)->second;;
}


void Material::SetGLAttribs(Shader* shader){
    for (int i = 0; i < attribsList.size(); i++) {
        if(isEnabled((MATERIAL_ATTRIBS)i)){
            if(i == ALBEDO)
            {
                glUniform3fv(shader->GetMaterialUniformLocation(i), 1, attribs->albedoColor.value());
            }
            if(i == DIFFUSE)
            {
                glUniform3fv(shader->GetMaterialUniformLocation(i), 1, attribs->diffuseColor.value());
            }
            if(i == SPECULAR)
            {
                glUniform3fv(shader->GetMaterialUniformLocation(i), 1, attribs->specularColor.value());
            }
            if(i == ALBEDO_MAP)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[COLOR_MAP]);
                glUniform1i(shader->GetMaterialUniformLocation(i), 0);
            }
            if(i == DIFFUSE_MAP)
            {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, textures[BUMP_MAP]);
                glUniform1i(shader->GetMaterialUniformLocation(i), 1);
            }
            if (i == RECEIVE_SHADOW)
            {
                textures[SHADOW_MAP] = RenderShadow::GetShadowMap();
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, textures[SHADOW_MAP]);
                glUniform1i(shader->GetMaterialUniformLocation(i), 2);
            } /*else {
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, textures[COLOR_MAP]);
                glUniform1i(shader->GetMaterialUniformLocation(i), 2);
            }
             */
        }
    }
}


map<string, MaterialAttribs_t*> Material::ParseData(ifstream& input_stream){
    string line = "";
    int nbMat = 0;
    map<string, MaterialAttribs_t*> material_data;
    string current_mat;
    
    if(input_stream) {
        while(!input_stream.eof() && std::getline(input_stream, line))
        {
            istringstream stringstream(line);
            string keyword;
            stringstream >> keyword;
            
            if( keyword == "newmtl" ) {
                nbMat++;
                stringstream >> keyword;
                if(keyword.compare("newmtl") == 0) current_mat = "default";
                else current_mat = keyword;
                material_data[current_mat] = new MaterialAttribs_t();
                material_data[current_mat]->name = current_mat;
            }
            
            if(keyword == "Ns") {
                stringstream >> ws >> keyword;
                material_data[current_mat]->Call("shininess", keyword);
            }
            
            if ( keyword == "Ka" ) {
                getline(stringstream, keyword);
                material_data[current_mat]->Call("albedoColor",keyword);
			}
			if (keyword == "Kd") {
                getline(stringstream, keyword);
                material_data[current_mat]->Call("diffuseColor",keyword);
			}
			if (keyword == "Ks") {
                getline(stringstream, keyword);
                material_data[current_mat]->Call("specularColor",keyword);
			}
            if (keyword == "map_Kd") {
                stringstream >> ws >> keyword;
                material_data[current_mat]->Call("albedoMap", keyword);
			}
			if (keyword == "map_Bump") {
                stringstream >> ws >> keyword;
                material_data[current_mat]->Call("diffuseMap", keyword);
			}
            if (keyword == "map_Ks") {
                stringstream >> ws >> keyword;
                material_data[current_mat]->Call("specularMap", keyword);
			}
            if (keyword == "receive_shadow") {
                stringstream >> ws >> keyword;
                material_data[current_mat]->Call("receiveShadow", keyword);
             //   cout << "[MATERIAL][ParseData] Receive_shadow " << keyword << endl;
			}
            if (keyword == "cast_shadow") {
                stringstream >> ws >> keyword;
                material_data[current_mat]->Call("castShadow", keyword);
             //   cout << "[MATERIAL][ParseData] cast_shadow " << keyword << endl;
			}

        }
    }
    return material_data;
}


