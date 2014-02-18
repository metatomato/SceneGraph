//
//  Import.cpp
//  helloGL
//
//  Created by tomato on 9/18/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Import.h"
#include "../Renderer/Entity.h"
#include "../Renderer/Box.h"

string Import::resourcesFolder = "/storage/sdcard0/SCENEGRAPH/Res/";
string Import::documentsFolder = "/storage/sdcard0/SCENEGRAPH/Res/";


string Import::TestPath(string file){
    string path = Import::GetResourcesFolder() + "models/" + file;
    ifstream input_stream(path.c_str(), ios::in);
    
    if (input_stream) {
        input_stream.close();
    }else {
        path = Import::GetDocumentsFolder() + file;
    }
    cout << "[IMPORT][TestPath] tested path return" << path << endl;
    return path;
}


////////////////////////////////////////////////////////////////////////////
//      IMPORTOBJ METHOD DEFINITIONS
////////////////////////////////////////////////////////////////////////////

void ImportObj::ExtractMeshAttribs(string file){
    LOGE("[IMPORT][ExtractMeshAttribs] from %s", file.c_str()) ;
    string line = "";
    bool UV_enable = 0;
    bool flag = false;
    bool flag_object = false;
    bool flag_poly = false;
    int obj_count = 0;
    int face_count = 0;
    int line_count = 0;
    int mat_count = 0;
    string current_mesh = "";
    string current_poly = "";
    string current_material = "";
    map< string , int > mesh_index;
    map< string , int > vertex_index;
    map< string , vector <uint> > face_idx_data;
    map< string , GLfloat[BOX_SIZE]> box;
    vector<string> mat;
    vector < GusVec3 > temp_vertex;
    vector < GusVec3 > temp_normal;
    vector < GusVec3 > temp_tangent;
    vector < GusVec2 > temp_UV;
    vector < vector<GusVec3i> > temp_face;
    vector< GusVec3i> init;
    temp_vertex.push_back(GusVec3(0.0f,0.0f,0.0f));
    temp_normal.push_back(GusVec3(0.0f,0.0f,0.0f));
    temp_tangent.push_back(GusVec3(0.0f,0.0f,0.0f));
    temp_UV.push_back(GusVec2(0.0f,0.0f));
    init.push_back(GusVec3i(0,0,0));
    temp_face.push_back(init);
    
    int nbVert = 0 , nbUV = 0 , nbNorm = 0;
    
    //string path = Import::GetResourcesFolder() + "models/" + file;
    ifstream input_stream(file.c_str(), ios::in);
    
    if(input_stream) {
        
        
        while(!input_stream.eof() && std::getline(input_stream, line))
        {
            istringstream stringstream(line);
            string keyword;
            stringstream >> keyword;
            line_count++;
            if( keyword == "o" ) {
                obj_count++;
                flag_object = true;
                flag_poly = false;
                mat_count = 0;
                mat.clear();
                stringstream >> keyword;
                current_mesh = keyword;
                
                vertexAttribs[current_mesh] = new MeshVertexAttribs_t();
                vertexAttribs[current_mesh]->parent = current_mesh;
                
               // LOGE("[IMPORT][ExtractMeshAttribs] o %s", current_mesh.c_str()) ;

                mesh_index[current_mesh] = face_count + 1 ;
                vertex_index[current_mesh] = nbVert ;
            }
            
            if(keyword == "usemtl") {
                mat_count++;
                stringstream >> keyword;
                bool mat_exist = false;
                
                if (keyword.compare("usemtl") == 0) {
                    current_material = "default";
                } else{
                    current_material = keyword;
                }
                
                current_poly = current_mesh + "_" + current_material;
                
                vector<string>::const_iterator it_mat = mat.begin();
                vector<string>::const_iterator it_mat_end = mat.end();
                for (; it_mat!=it_mat_end; ++it_mat) {
                    if((*it_mat).compare(current_material) == 0){
                        mat_exist = true;
                        cout << "[MESH][ParseData] Find Existing Mat! " << current_material << endl;
                    }
                }
                if(!mat_exist){
                    if (mat_count > 1) {
                        flag_poly = true;
                    }
                    
                    mat.push_back(current_material);
                    
                    faceAttribs[current_poly] = new MeshFaceAttribs_t();
                    faceAttribs[current_poly]->meshData = current_mesh;
                    faceAttribs[current_poly]->material = current_material;
                    vertexAttribs[current_mesh]->child.push_back(current_poly);
                    mtlFiles.push_back(current_material);
                    if (flag_poly) {
                        vertexAttribs[current_mesh]->polyGroup = true;
                    }
                    
                    cout << "[MESH][ParseData] mat " << current_material << " poly " << current_poly
                    <<  " mesh " << current_mesh << endl;
                  //  LOGE("[IMPORT][ExtractMeshAttribs] mat %s  poly %s mesh %s ",current_material.c_str() ,  current_poly.c_str(), current_mesh.c_str()) ;
                }
            }
            
            
            if ( keyword == "v" ) {
                flag = false;
				nbVert++;
                GusVec3 temp_vec;
                stringstream >>  std::ws >> temp_vec.x  >> std::ws >> temp_vec.y  >> std::ws >> temp_vec.z;
                temp_vertex.push_back(temp_vec);
                if (flag_object) {
                    vertexAttribs[current_mesh]->AABB.InitExtrema(temp_vec);
                    flag_object = false;
                }
                vertexAttribs[current_mesh]->AABB.SetBoxExtrema(temp_vec);
			}
			if (keyword == "vt") {
                flag = false;
				nbUV++;
                UV_enable = 1;
                GusVec2 temp_vec;
                stringstream >>  std::ws >> temp_vec.x  >> std::ws >> temp_vec.y;
                temp_UV.push_back(temp_vec);
 			}
			if (keyword == "vn") {
                flag = false;
				nbNorm++;
                GusVec3 temp_vec;
                stringstream >>  std::ws >> temp_vec.x  >> std::ws >> temp_vec.y  >> std::ws >> temp_vec.z;
                temp_normal.push_back(temp_vec);
			}
			if (keyword == "f") {
                flag = true;
                
                if((mesh_index[current_mesh] - face_count) == 0) {
                    vertexAttribs[current_mesh]->vertexCount = nbVert - vertex_index[current_mesh] + 1 ;
                }
                vertexAttribs[current_mesh]->faceCount++;
                faceAttribs[current_poly]->triangleCount++;
                face_count++;
				
                GLuint q1, q2, q3, q4, q5, q6, q7, q8, q9;
				char sl;
                q1 = q2 = q3 = q4 = q5 = q6 = q7 = q8 = q9 = 0;
				q2 = q5 = q8 = 0;
				if ( UV_enable ){
                    stringstream >> std::ws >> q1 >> sl >> q2 >> sl >> q3  >> std::ws >> q4 >> sl >> q5 >> sl >> q6 >> std::ws >> q7 >> sl >> q8 >> sl >> q9;
				}
				else {
					stringstream >> std::ws >> q1 >> sl >>  sl >> q3  >> std::ws >> q4 >> sl  >> sl >> q6 >> std::ws >> q7 >> sl >>  sl >> q9;
				}
                
                vector<GusVec3i> temp_vec;
                temp_vec.push_back(GusVec3i(q1,q4,q7));
                temp_vec.push_back(GusVec3i(q2,q5,q8));
                temp_vec.push_back(GusVec3i(q3,q6,q9));
                temp_face.push_back(temp_vec);
                
                face_idx_data[current_poly].push_back(face_count);
                
			}
            
            if (stringstream.fail()) {
                std::cout << "[MESH][ParseData] Data Parsing FAILED at " << line_count << endl;
            }
        }

       // LOGE("[IMPORT][ExtractMeshAttrib] ENDING EXTRACTION BEGINING PARSING");
        Mesh::triangleTotalCount += face_count;
        
        //    cout << "[MESH][ParseData] Total Face " << triangleTotalCount << endl;
        
        vector<GusVec3> temp_t;
        temp_t.assign(nbVert, GusVec3());

        if (UV_enable) {
            
            temp_tangent.assign(nbVert+1, GusVec3());


            for(int i = 1 ; i < face_count+1 ; i++ ){


                GusVec3 v1( temp_vertex[temp_face[i][0].y].x - temp_vertex[temp_face[i][0].x].x ,
                           temp_vertex[temp_face[i][0].y].y - temp_vertex[temp_face[i][0].x].y ,
                           temp_vertex[temp_face[i][0].y].z - temp_vertex[temp_face[i][0].x].z ) ;
                
                GusVec3 v2(temp_vertex[temp_face[i][0].z].x - temp_vertex[temp_face[i][0].x].x ,
                           temp_vertex[temp_face[i][0].z].y - temp_vertex[temp_face[i][0].x].y ,
                           temp_vertex[temp_face[i][0].z].z - temp_vertex[temp_face[i][0].x].z);
                
                GusVec2 st1(temp_UV[temp_face[i][1].y].x - temp_UV[temp_face[i][1].x].x ,
                            temp_UV[temp_face[i][1].y].y - temp_UV[temp_face[i][1].x].y );
                
                GusVec2 st2(temp_UV[temp_face[i][1].z].x - temp_UV[temp_face[i][1].x].x ,
                            temp_UV[temp_face[i][1].z].y - temp_UV[temp_face[i][1].x].y);
                
                float coef = 1/ (st1.x * st2.y - st2.x * st1.y);
                GusVec3 temp = GusVec3();


                temp.x = coef * ((v1.x * st2.y)  + (v2.x * -st1.y));
                temp.y = coef * ((v1.y * st2.y)  + (v2.y * -st1.y));
                temp.z = coef * ((v1.z * st2.y)  + (v2.z * -st1.y));

                temp.Normalize();


                temp_tangent[temp_face[i][0].x] = temp;
                temp_tangent[temp_face[i][0].y] = temp;
                temp_tangent[temp_face[i][0].z] = temp;
                
            }



            vector<vector<GLuint> > tri_edge;
            
           // LOGE("[MESH][ExtractMeshAttrib] Tangent Calculation..." );
            
            for(int i = 1 ; i < nbVert+1 ; i++ ){
                vector<GLuint> tri_list;
                for(int k = 1 ; k < face_count+1 ; k++ ){
                    for (int j = 0 ; j < 3; j++) {
                        if(temp_face[k][0][j] == i  ){
                            tri_list.push_back(k);
                        }
                    }
                }
                tri_edge.push_back(tri_list);
                //   cout << "vertex " << i << " found in ";
                for (int l = 0 ; l < tri_list.size(); l++) {
                    //       cout << " " << tri_list[l];
                }
                //   cout << endl;
            }
            
            for(int i = 0 ; i < tri_edge.size() ; i++ ){
                int size = tri_edge[i].size();
                for(int j = 0 ; j < size ; j++){
                    int idx = tri_edge[i][j];
                    temp_t[i] += temp_tangent[temp_face[idx][0][0]];
                }
                temp_t[i] /= size;
                temp_t[i].Normalize();
            }
            
            LOGE("............................ DONE!" );
        }

        auto it_v = vertexAttribs.begin();
        for( ; it_v != vertexAttribs.end() ; ++it_v ) {
            it_v->second->AABB.BuildBoxFromExtrema();
        }

        auto it = faceAttribs.begin();
        for (; it != faceAttribs.end() ; ++it) {
            
            //GusVec3 originCenter = vertexAttribs[it->second->meshData]->originalCenter;
            //vertexAttribs[it->second->meshData]->AABB.position = vertexAttribs[it->second->meshData]->originalCenter;
            
            GusVec3 originCenter = vertexAttribs[it->second->meshData]->AABB.position;
            vertexAttribs[it->second->meshData]->originalCenter = originCenter;
            
            it->second->vertex    = new GLfloat [ it->second->triangleCount * 9 ];
            it->second->normal    = new GLfloat [ it->second->triangleCount * 9 ];
            it->second->tangent   = new GLfloat [ it->second->triangleCount * 9 ];
            it->second->textureUV = new GLfloat [ it->second->triangleCount * 6 ];
            
            it->second->indexTri  = new GLuint [ it->second->triangleCount * 3 ];
            it->second->indexLine = new GLuint [ it->second->triangleCount * 6 ];
            
            
            int index_length = face_idx_data[it->first].size();
            
            GLuint index_start = vertex_index[it->second->meshData] ;
            
            //cout << "[MESH][ParseData] index_start " << index_start << " for " << it->first << endl;
            
            for (int i = 0 ; i < index_length ; i++) {
                
                int idx = face_idx_data[it->first].at(i);
                
                //  cout << "[MESH][ParseData] index " << idx << " " << temp_face[idx][0].x << " for " << it->first << endl;
                
                it->second->indexTri[i*3]   = temp_face[idx][0].x - index_start ;
                it->second->indexTri[i*3+1] = temp_face[idx][0].y - index_start ;
                it->second->indexTri[i*3+2] = temp_face[idx][0].z - index_start ;
                
                it->second->indexLine[i*6]   = i*3;
                it->second->indexLine[i*6+1] = i*3+1;
                it->second->indexLine[i*6+2] = i*3+1;
                it->second->indexLine[i*6+3] = i*3+2;
                it->second->indexLine[i*6+4] = i*3+2;
                it->second->indexLine[i*6+5] = i*3;
                
                
                it->second->vertex[i*9]   = temp_vertex[temp_face[idx][0].x ].x - originCenter.x;
                it->second->vertex[i*9+1] = temp_vertex[temp_face[idx][0].x ].y - originCenter.y;
                it->second->vertex[i*9+2] = temp_vertex[temp_face[idx][0].x ].z - originCenter.z;
                it->second->vertex[i*9+3] = temp_vertex[temp_face[idx][0].y ].x - originCenter.x;
                it->second->vertex[i*9+4] = temp_vertex[temp_face[idx][0].y ].y - originCenter.y;
                it->second->vertex[i*9+5] = temp_vertex[temp_face[idx][0].y ].z - originCenter.z;
                it->second->vertex[i*9+6] = temp_vertex[temp_face[idx][0].z ].x - originCenter.x;
                it->second->vertex[i*9+7] = temp_vertex[temp_face[idx][0].z ].y - originCenter.y;
                it->second->vertex[i*9+8] = temp_vertex[temp_face[idx][0].z ].z - originCenter.z;
                
                it->second->normal[i*9]   = temp_normal[temp_face[idx][2].x ].x;
                it->second->normal[i*9+1] = temp_normal[temp_face[idx][2].x ].y;
                it->second->normal[i*9+2] = temp_normal[temp_face[idx][2].x ].z;
                it->second->normal[i*9+3] = temp_normal[temp_face[idx][2].y ].x;
                it->second->normal[i*9+4] = temp_normal[temp_face[idx][2].y ].y;
                it->second->normal[i*9+5] = temp_normal[temp_face[idx][2].y ].z;
                it->second->normal[i*9+6] = temp_normal[temp_face[idx][2].z ].x;
                it->second->normal[i*9+7] = temp_normal[temp_face[idx][2].z ].y;
                it->second->normal[i*9+8] = temp_normal[temp_face[idx][2].z ].z;

                if (UV_enable) {
                    it->second->tangent[i*9]   = temp_t[temp_face[idx][0].x ].x;
                    it->second->tangent[i*9+1] = temp_t[temp_face[idx][0].x ].y;
                    it->second->tangent[i*9+2] = temp_t[temp_face[idx][0].x ].z;
                    it->second->tangent[i*9+3] = temp_t[temp_face[idx][0].y ].x;
                    it->second->tangent[i*9+4] = temp_t[temp_face[idx][0].y ].y;
                    it->second->tangent[i*9+5] = temp_t[temp_face[idx][0].y ].z;
                    it->second->tangent[i*9+6] = temp_t[temp_face[idx][0].z ].x;
                    it->second->tangent[i*9+7] = temp_t[temp_face[idx][0].z ].y;
                    it->second->tangent[i*9+8] = temp_t[temp_face[idx][0].z ].z;
                    
                    it->second->textureUV[i*6]   = temp_UV[temp_face[idx][1].x ].x;
                    it->second->textureUV[i*6+1] = temp_UV[temp_face[idx][1].x ].y;
                    it->second->textureUV[i*6+2] = temp_UV[temp_face[idx][1].y ].x;
                    it->second->textureUV[i*6+3] = temp_UV[temp_face[idx][1].y ].y;
                    it->second->textureUV[i*6+4] = temp_UV[temp_face[idx][1].z ].x;
                    it->second->textureUV[i*6+5] = temp_UV[temp_face[idx][1].z ].y;
                }

            }
        }
    }
  //  LOGE("[IMPORT][ExtractMeshAttrib] EXITING MESH PARSING");
    input_stream.close();
    
}



void ImportObj::ExtractMaterialAttribs(string file){
    string line = "";
    int nbMat = 0;
    map<string, MaterialAttribs_t*> material_data;
    string current_mat;
    
    file.replace(file.end()-4, file.end(), ".mtl");
    //string path = Import::GetResourcesFolder() + "models/" + file;
  //  LOGE("[IMPORTOBJ][ExtractMaterialAttribs] LOADING %s \n",file.c_str());
    ifstream input_stream(file.c_str(), ios::in);
    
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
                materialAttribs[current_mat] = new MaterialAttribs_t();
                materialAttribs[current_mat]->name = current_mat;
            }
            if(keyword == "Ns") {
                stringstream >> ws >> keyword;
                materialAttribs[current_mat]->Call("shininess", keyword);
            }
            if ( keyword == "Ka" ) {
                getline(stringstream, keyword);
                materialAttribs[current_mat]->Call("albedoColor",keyword);
			}
			if (keyword == "Kd") {
                getline(stringstream, keyword);
                materialAttribs[current_mat]->Call("diffuseColor",keyword);
			}
			if (keyword == "Ks") {
                getline(stringstream, keyword);
                materialAttribs[current_mat]->Call("specularColor",keyword);
			}
            if (keyword == "map_Kd") {
                stringstream >> ws >> keyword;
                materialAttribs[current_mat]->Call("albedoMap", keyword);
			}
			if (keyword == "map_Bump") {
                stringstream >> ws >> keyword;
                materialAttribs[current_mat]->Call("diffuseMap", keyword);
			}
            if (keyword == "map_Ks") {
                stringstream >> ws >> keyword;
                materialAttribs[current_mat]->Call("specularMap", keyword);
			}
            if (keyword == "receive_shadow") {
                stringstream >> ws >> keyword;
                materialAttribs[current_mat]->Call("receiveShadow", keyword);
                //   cout << "[MATERIAL][ParseData] Receive_shadow " << keyword << endl;
			}
            if (keyword == "cast_shadow") {
                stringstream >> ws >> keyword;
                materialAttribs[current_mat]->Call("castShadow", keyword);
                //   cout << "[MATERIAL][ParseData] cast_shadow " << keyword << endl;
			}
        }
    }
    input_stream.close();
}



void ImportObj::GenerateObject(){
    
    map<string, Material*>      material_table;
    map<string, vector<const Scenable*>> entity_table;
    
   // LOGE("[IMPORTOBJ][GenerateObject] MESH \n");
    for (auto it = faceAttribs.begin(); it!=faceAttribs.end(); ++it) {
        Mesh *mesh_ptr = new Mesh(*vertexAttribs[it->second->meshData],*it->second,it->first);
        //mesh_ptr->GenAllVBO();
        //mesh_ptr->MapAllVBO();
        rMesh.push_back(mesh_ptr);
    //    LOGE("  [IMPORTOBJ][GenerateObject] type devired %d \n",mesh_ptr->_GetType());
    //    LOGE("  [IMPORTOBJ][GenerateObject] type base %d \n",rMesh.back()->_GetType());
    //    LOGE("  [IMPORTOBJ][GenerateObject] %s \n",mesh_ptr->_GetName().c_str());
    }
    //LOGE("[IMPORTOBJ][GenerateObject] MATERIAL \n");
    for (auto it = materialAttribs.begin(); it!=materialAttribs.end(); ++it) {
        Material* material_ptr = new Material(it->second);
       // material_ptr->_SetPostAttrib();
        rMaterial.push_back(material_ptr);
        material_table[material_ptr->_GetName()] = material_ptr;
     //   LOGE("  [IMPORTOBJ][GenerateObject] %s \n",material_ptr->_GetName().c_str());
    }
    //LOGE("[IMPORTOBJ][GenerateObject] ENTITY \n");
    for (auto obj : rMesh) {
        Entity*   entity_ptr   = new Entity((Mesh*) obj,material_table[((Mesh*)obj)->GetMaterial()]);
        //entity_ptr->_SetPostAttrib();
        rEntity.push_back(entity_ptr);
        entity_table[entity_ptr->GetParent()].push_back(entity_ptr);
    //    LOGE("  [IMPORTOBJ][GenerateObject] %s %d\n",entity_ptr->_GetName().c_str(),entity_ptr->_GetId());
    }
    //LOGE("[IMPORTOBJ][GenerateObject] STAGE \n");
    vector<StageObject*> parent_list;
    for (auto pair : entity_table) {
        StageObject* stage_ptr = new SceneEntity(pair.second);
        sEntity.push_back(stage_ptr);
    //    LOGE("  [IMPORTOBJ][GenerateObject] %s \n",stage_ptr->_GetName().c_str());
        parent_list.push_back(stage_ptr);
    }
    
   // LOGE("[IMPORTOBJ][GenerateObject] SCENEBOX \n");
    	//SceneBox* scene_box = new SceneBox(parent_list);
    	//scene_box->GetBox().PrintBox(currentFile);
    
}



vector<vector<GusObject*>> ImportObj::ExtractData(string file){
    
    currentFile = file;
    
    string path = Import::TestPath(file);
        
    ExtractMeshAttribs(path);
    
    ExtractMaterialAttribs(path);
    
    GenerateObject();
    
    vector<vector<GusObject*>> vec_to_return;
    vec_to_return.push_back(rMesh);
    vec_to_return.push_back(rMaterial);
    vec_to_return.push_back(rEntity);
    vec_to_return.push_back(sEntity);
    
    return vec_to_return;
}





















