#include "md5.h"
#include "md5Animation.h"
#include "../Scene/Scene.h"
#include "../DataCenter/Import.h"



md5Object::md5Object():
    animation(NULL),
    animCursor(61.f),
    vertexAttribs(new MeshVertexAttribs_t()){

}

md5Object::~md5Object(){
    delete vertexAttribs;
}




void md5Object::_SetAttrib(string attrib_name, string value){
    attribs.Call(attrib_name, value);
    
};

void md5Object::_SetPostAttrib(){
    set( (attribs.mesh + ".md5mesh").c_str(), (attribs.anim[0] + ".md5anim").c_str());
    load();
    ComputeBoundingBox();
};

void md5Object::_SetAttribFromChild(GusGLObject* child){};




void md5Object::set(const char* md5mesh, const  char* md5anim)
{
	this->md5mesh = md5mesh;
	this->md5anim = md5anim;
	if (MD5_DEBUG) cout << "Fichier charger : " << this->md5mesh << "  " << this->md5anim << endl;
	
}

void md5Object::load()
{
	string fileToOpen = Import::GetResourcesFolder() + "Res/models/" ;
	fileToOpen += md5mesh;
	if (MD5_DEBUG) cout << "Fichier charger + Dossier : " << fileToOpen << endl;
	FILE* fp = fopen (fileToOpen.c_str(), "rb");
	if (!fp)
    {
		printf("Error: could not open file '%s'!\n", fileToOpen.c_str());
		return ;
    }
	LOGE("[MD5][Load] character loaded %s", fileToOpen.c_str());
	
	if (MD5_DEBUG)
		printf("Succesfully opened file '%s'.\n", md5mesh.c_str());
	
	loadMesh(fp);	

	fclose (fp);

	if (md5anim.size() != 0)
	{
		if (MD5_DEBUG) printf("Begin Animation Read from %s!\n", md5anim.c_str());
		animation = new md5Animation();
		animation->load(md5anim.c_str());
		
		//if (!animation->loadingErrors)
			animation->generateAllSkeletons();
		if (MD5_DEBUG) printf("Animation LOADING ERRORS??? : %d\n", animation->loadingErrors);
	
		//this->joints = animation->joints[100] ;
		
	}	
	
}


void md5Object::getAttributeLocation(int shaderprogram)
{

	//glUseProgram(shaderprogram);

	vertexLocation = glGetAttribLocation(shaderprogram, "vertex");
	
	normalLocation = glGetAttribLocation(shaderprogram, "normal");
/*
  	textureLocation = glGetAttribLocation(shaderprogram, "firstTexture");

  	textureLocation0 = glGetAttribLocation(shaderprogram, "secTexture");
*/
	texCoordsLocation = glGetAttribLocation(shaderprogram, "texCoord");

	tangentLocation = glGetAttribLocation(shaderprogram, "tangent");

}


void md5Object::loadMesh(FILE* fp)
{
	readVersion(fp);
	
	
	readCommandLine(fp);
	
	
	getJointsCount(fp);
	
	getMeshesCount(fp);
	
	readJoints(fp);
	
	readMeshes(fp);

	
	//loadShaders();

	generateNormalsAndTangents();

	
	// VBO optimizations

	vaoId = new GLuint[meshesCount];
	if (MD5_DEBUG) printf("meshesCount : %d\n", meshesCount);
//	glGenVertexArrays(meshesCount, vaoId);

	md5Mesh* currentMesh = NULL;
	for(int i = 0 ; i <meshesCount ; i++)
	{
				
	//	glBindVertexArray(vaoId[i]);

        	if (MD5_DEBUG) printf("vao de md5Object : %d\n", vaoId[i]);

		currentMesh = &meshes[i];
		//if (MD5_DEBUG) printf("mesh id=%d name=%s \n",i,currentMesh->shaderName);
		glGenBuffers(1, &vboVertexId[i]);
		//if (MD5_DEBUG) printf("	vboVertexId =%d size=%d\n",vboVertexId[i],currentMesh->verticesCount*3);
		glBindBuffer(GL_ARRAY_BUFFER,vboVertexId[i]);
		//glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->facesCount * 3 * 3 * sizeof(float),vertexArray[i],GL_DYNAMIC_DRAW_ARB);


		glGenBuffers(1, &vboNormalId[i]);
		glBindBuffer(GL_ARRAY_BUFFER,vboNormalId[i]);
		//if (MD5_DEBUG) printf("	vboNormalId =%d aize=%d\n",vboNormalId[i],currentMesh->verticesCount*3);
		//glBufferDataARB(GL_ARRAY_BUFFER_ARB,currentMesh->facesCount * 3 * 3 * sizeof(float),normalArray[i],GL_DYNAMIC_DRAW_ARB);
		glGenBuffers(1, &vboTangentId[i]);
		glBindBuffer(GL_ARRAY_BUFFER,vboTangentId[i]);


		glGenBuffers(1, &vboTexturId[i] );
		glBindBuffer(GL_ARRAY_BUFFER,vboTexturId[i]);
		glBufferData(GL_ARRAY_BUFFER,currentMesh->verticesCount * 2 * sizeof(int),texturArraySkin[i],GL_STATIC_DRAW);
        glVertexAttribPointer(texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(texCoordsLocation);   
        
		glGenBuffers(1,&vboElementsId[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboElementsId[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,currentMesh->facesCount * 3 *  sizeof(int),elementsSkin[i],GL_STATIC_DRAW);
       
   //    glBindVertexArrayOES(0);
        
        glGenBuffers(1,&vboElementsId_0[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboElementsId_0[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,currentMesh->facesCount * 6 *  sizeof(int),elementsSkin_0[i],GL_STATIC_DRAW);
         
	}
	//glBindBuffer(GL_ARRAY_BUFFER,0);
	


}


//Private methods
// Read data from file
void md5Object::readVersion(FILE* fp)
{	
	 
	
	while (sscanf (buff, " MD5Version %d",& this->version) != 1)
		fgets (buff, sizeof (buff), fp);

	if (MD5_DEBUG) cout << "MD5 VERSION " << this->version << endl;

	if (version != MD5_VERSION)
	{
		/* Bad version */
		if (MD5_DEBUG) printf ( "Error: This version (%d) is not supported.\n", version);
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	else
	{
		if (MD5_DEBUG)
			printf ( "MD5 version (%d) is supported.\n",this->version);
	}
}

void md5Object::getJointsCount(FILE* fp)
{
	
	while(sscanf (buff, " numJoints %d", &this->jointsCount ) != 1)
		fgets (buff, sizeof (buff), fp);

	if (MD5_DEBUG) cout << "nombre de joints : " << this->jointsCount << endl;

	if (jointsCount == 0)
	{
		if (MD5_DEBUG) printf( "Error: Could not find the number of joints in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (MD5_DEBUG) 
		printf("Found %d joints declaration, allocating memory.\n",jointsCount);
	joints = (md5joint*)calloc( jointsCount , sizeof(md5joint) );
	
}

void md5Object::getMeshesCount(FILE* fp)
{
	while(sscanf (buff, " numMeshes %d", &this->meshesCount ) != 1)
		fgets (buff, sizeof (buff), fp);
	if (MD5_DEBUG) cout << "nombre de mesh : " << this->meshesCount << endl;
	if (meshesCount == 0)
	{
		printf( "Error: Could not find the number of meshes in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (MD5_DEBUG)
		printf("Found %d meshes declaration, allocating memory.\n",meshesCount);
	
	// Now that we know how many meshes will be described we can allocated memory
	this->meshes = (md5Mesh*)calloc( meshesCount , sizeof(md5Mesh) );
	
	// Also allocating memory for the skeleton and skin array, used with VertexArrays;
	this->vertexArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );
	this->elementsSkin = (int**)calloc( meshesCount, sizeof(int*)  );
    this->elementsSkin_0 = (int**)calloc( meshesCount, sizeof(int*)  );
	this->normalArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );
	this->tangentArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );
	this->texturArraySkin = (float**)calloc( meshesCount, sizeof(float*)  );

	vboVertexId = (GLuint*)calloc( meshesCount,sizeof(GLuint)  );
	vboNormalId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	vboTangentId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	vboTexturId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	vboTexturId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	vboElementsId = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
    vboElementsId_0 = (GLuint*)calloc( meshesCount, sizeof(GLuint)  );
	
}

void md5Object::readCommandLine(FILE* fp)
{
	while (sscanf (buff, " commandline %s", this->commandline) != 1)
		fgets (buff, sizeof (buff), fp);
	
	if (MD5_DEBUG)
		printf("Command line is: %s.\n",commandline);
	// Nothing is done with the command line
}
void md5Object::readJoints(FILE* fp)
{
	// Seach for beginning of the joints
	while(strncmp (buff, "joints {", 8) != 0)
		fgets (buff, sizeof (buff), fp);
	
	// CurrentJoint is a pointer in the meshArray, we iterate through the join rows and update the pointer location
	md5joint* currentJoint = joints;
	for(int jointCounter=0 ; jointCounter < jointsCount; jointCounter++,currentJoint++)
	{
		fgets (buff, sizeof (buff), fp);
		sscanf (buff, "%s %d ( %f %f %f ) ( %f %f %f )",
				currentJoint->name, 
				&(currentJoint->parent), 
				&(currentJoint->position.x),
				&(currentJoint->position.y), 
				&(currentJoint->position.z),
				
				&currentJoint->orientation.x,
				&currentJoint->orientation.y, 
				&currentJoint->orientation.z);
		
		// Unpacking the last quaternion component
		currentJoint->orientation.computeW();
		
		if (MD5_DEBUG)
			printf("Read joint name:'%s', parent=%d, pos.x=%.2f ,pos.y=%.2f , pos.z=%.2f , q.x=%.2f ,q.y=%.2f ,q.z=%.2f\n",
			   currentJoint->name,
			   currentJoint->parent,
			   currentJoint->position.x,
			   currentJoint->position.y,
			   currentJoint->position.z,
			   currentJoint->orientation.x,
			   currentJoint->orientation.y,
			   currentJoint->orientation.z);
	}
}


void md5Object::readMeshes(FILE* fp)
{
	for( int meshCounter=0; meshCounter<meshesCount; meshCounter++)
	{
	if (MD5_DEBUG) cout << "Mesh n° " << meshCounter << endl;
		if (MD5_DEBUG)
			printf("Reading mesh %d.\n",meshCounter);
		
		md5Mesh* currentMesh = &meshes[meshCounter];
		
		while(strncmp (buff, "mesh {", 6) != 0)
			fgets (buff, sizeof (buff), fp);
/*
		// Read shader name
		while(sscanf (buff, " shader \"%s\" ", currentMesh->shaderName ) != 1)
			fgets (buff, sizeof (buff), fp);
		
		//A bit ugly but necessary: Remove the trailing '"' character
		currentMesh->shaderName[strlen(currentMesh->shaderName)-1] = '\0';

		if (MD5_DEBUG)
			printf("Shader name is '%s'.\n",currentMesh->shaderName);

		//Check if shader exist
		md5material* material= md5MaterialsLibrary::getInstance()->getMaterial(currentMesh->shaderName );
		if (material != NULL)
		{
			if (MD5_DEBUG)
			{
				printf("Shader was found in the library:\n");
				printf("	map:'%s'\n",material->mapFileName.c_str());
				printf("	diffuse:'%s'\n",material->diffuseMapFilename.c_str());
				printf("	specular:'%s'\n",material->specularMapFilename.c_str());
				printf("	normal:'%s'\n",material->normalMapFilename.c_str());
				printf("	height:'%s'\n",material->heightMapFilename.c_str());
			}
		}		
*/
		//Read vertices
			while(sscanf (buff, " numverts %d", &currentMesh->verticesCount ) != 1)
				fgets (buff, sizeof (buff), fp);
		
			if (MD5_DEBUG)
				printf("%d vertices.\n",currentMesh->verticesCount);
		
			//Allocate space with this new information
			currentMesh->vertices = (md5Vertex*)calloc(currentMesh->verticesCount,sizeof(md5Vertex));


		
			md5Vertex* currentVertex = currentMesh->vertices;
			for(int vertexCounter=0;vertexCounter < currentMesh->verticesCount; )
			{
				int valueRead = sscanf (buff, " vert %*s ( %f %f ) %d %d", 
						//&currentVertex->textureCoor.u,
						&currentVertex->textureCoor.u, 
						&currentVertex->textureCoor.v, 
						&currentVertex->startWeightIndex, 
						&currentVertex->weightCount);
				
				
				
				
				fgets (buff, sizeof (buff), fp);
				
				//printf("Read value %d\n",valueRead);
				if (valueRead != 0) 
				{
					if (MD5_DEBUG)
						printf("Read vertex u=%.4f, v=%.4f, wStart=%d,wCount=%d.\n",currentVertex->textureCoor.u,currentVertex->textureCoor.v,currentVertex->startWeightIndex,currentVertex->weightCount);
					currentVertex++;
					vertexCounter++;
				}
				else
				{
					if (MD5_DEBUG)
						printf("Could not read value from mesh #%d, line#%d\n.",meshCounter,vertexCounter);
				}
			}
		//Read faces
			while(sscanf (buff, " numtris %d", &currentMesh->facesCount ) != 1)
				fgets (buff, sizeof (buff), fp);
		
			if (MD5_DEBUG)
				printf("%d faces.\n",currentMesh->facesCount);
			
			//Allocate space with this new information
			currentMesh->faces = (md5Face*)calloc(currentMesh->facesCount,sizeof(md5Face));
		
			md5Face* currentFace = currentMesh->faces;
            
        
			for(int faceCounter=0;faceCounter < currentMesh->facesCount; )
			{
				int valueRead = sscanf (buff, " tri %*s %d %d %d", 
						//&currentFace->index,
						&currentFace->point1Index, 
						&currentFace->point2Index, 
						&currentFace->point3Index);
							
				//printf("Read tri indexes %d, %d, %d\n",currentFace->point1Index,currentFace->point3Index,currentFace->point3Index);
				
				fgets (buff, sizeof (buff), fp);
				
				if (valueRead != 0) 
				{
					if (MD5_DEBUG)
						printf("Reading face: %d, indexes=%d,%d,%d\n",faceCounter,currentFace->point1Index,currentFace->point2Index,currentFace->point3Index);
					
					// De-reference
					//currentFace->point1 = currentMesh->vertices[currentFace->point1Index];
					//currentFace->point2 = currentMesh->vertices[currentFace->point2Index];
					//currentFace->point3 = currentMesh->vertices[currentFace->point3Index];
					
					faceCounter++,currentFace++;					
				}
				else
				{
					if (MD5_DEBUG)
						printf("Could not read value from mesh #%d, line#%d\n.",meshCounter,faceCounter);
				}
			}
		
		//Read weights
			while(sscanf (buff, " numweights %d", &currentMesh->weightsCount ) != 1)
				fgets (buff, sizeof (buff), fp);
		
			if (MD5_DEBUG)
				printf("%d weight.\n",currentMesh->weightsCount);

			//Allocate space with this new information
				currentMesh->weights = (md5Weight*)calloc(currentMesh->weightsCount,sizeof(md5Weight));
		
			md5Weight* currentWeight = currentMesh->weights;
			for(int weightCounter=0; weightCounter< currentMesh->weightsCount ;)
			{
				int readValue = sscanf (buff, " weight %*s %d %f ( %f %f %f )",
						//&currentWeight->index,
						&currentWeight->jointIndex, 
						&currentWeight->bias,
						&currentWeight->position.x, 
						&currentWeight->position.y, 
						&currentWeight->position.z);
				
              //  vertexAttribs->SetBoxExtrema(GusVec3(currentWeight->position.x,currentWeight->position.y,currentWeight->position.z));
                
				if (readValue != 0) 
				{
					if (MD5_DEBUG)
						printf("Reading weight: %d, jointIndex=%d,bias=%.4f\n",weightCounter,currentWeight->jointIndex,currentWeight->bias);
					
					// Dereferencing the joint  (Faitessss touuuurner !!!)
					//currentWeight->joint = &joints[currentWeight->jointIndex];
					
					weightCounter++;
					currentWeight++;
				}
				else
				{
					if (MD5_DEBUG)
						printf("Could not read value from mesh #%d, line#%d\n.",meshCounter,weightCounter);
				}
				
				fgets (buff, sizeof (buff), fp);
                
			}
    }
    /*
    vertexAttribs->BuildBox();
    cout << "[MD5][ReadMeshes] BOX " << endl
                                     << vertexAttribs->halfVec->x << " "
                                     << vertexAttribs->halfVec->y << " "
                                     << vertexAttribs->halfVec->z << endl;
    */
	// Mesh file has been read, allocating space for openGL calls, vertexArray and elementArray
	// Looping to read all meshes.
	float** currentVertexArraySkin = vertexArraySkin;
	float** currentNormalArraySkin = normalArraySkin;
	float** currentTangentArraySkin = tangentArraySkin;
	int** currentElementSkin = elementsSkin;
    int** currentElementSkin_0 = elementsSkin_0;
	float** currenttextureArraySkin = texturArraySkin;
	
	if (MD5_DEBUG) cout << "allocation temporaire des pointeurs" << endl;

	for( int meshCounter=0; meshCounter< meshesCount; meshCounter++)
	{		
	
		md5Mesh* currentMesh = &meshes[meshCounter];
		
		*currentVertexArraySkin = (float*)calloc(currentMesh->verticesCount * 3,sizeof(float));
		currentVertexArraySkin++;
		
		*currentNormalArraySkin = (float*)calloc( currentMesh->verticesCount * 3, sizeof(float));
		currentNormalArraySkin++;

		*currentTangentArraySkin = (float*)calloc( currentMesh->verticesCount * 3, sizeof(float));
		currentTangentArraySkin++;
		
		*currentElementSkin = (int*)calloc(currentMesh->facesCount * 3,sizeof(int));
		currentElementSkin++;
        
        *currentElementSkin_0 = (int*)calloc(currentMesh->facesCount * 6,sizeof(int));
		currentElementSkin_0++;

		*currenttextureArraySkin = (float*)calloc(currentMesh->verticesCount * 2, sizeof(float) );
		currenttextureArraySkin++;
	}
		
	//Optimization, the element index to draw in openGL never change, we can set it up here:

	// Put elements inside elements array
	for (int meshIndex=0 ; meshIndex < meshesCount ; meshIndex++)
	{
		md5Mesh* currentMesh = &meshes[meshIndex];
		int* currentIndex = elementsSkin[meshIndex];
        int* currentIndex_0 = elementsSkin_0[meshIndex];
		for (int i=0; i < currentMesh->facesCount; i++ )
		{
			*currentIndex = currentMesh->faces[i].point1Index; currentIndex++;
			*currentIndex = currentMesh->faces[i].point2Index; currentIndex++;
			*currentIndex = currentMesh->faces[i].point3Index; currentIndex++;
			
            *currentIndex_0 = currentMesh->faces[i].point1Index; currentIndex_0++;
			*currentIndex_0 = currentMesh->faces[i].point2Index; currentIndex_0++;
			*currentIndex_0 = currentMesh->faces[i].point2Index; currentIndex_0++;
            *currentIndex_0 = currentMesh->faces[i].point3Index; currentIndex_0++;
			*currentIndex_0 = currentMesh->faces[i].point3Index; currentIndex_0++;
			*currentIndex_0 = currentMesh->faces[i].point1Index; currentIndex_0++;
            
			if (MD5_DEBUG)
			{
				printf("face Indexes p1=%d, p2=%d, p3=%d\n",currentMesh->faces[i].point1Index,currentMesh->faces[i].point2Index,currentMesh->faces[i].point3Index);
				printf("Reverse face Indexes p1=%d, p2=%d, p3=%d\n",*(currentIndex-3),*(currentIndex-2),*(currentIndex-1));
			}
		}
	}
	
	//Put texture coordinates in the array
	for (int meshIndex=0 ; meshIndex < meshesCount ; meshIndex++)
	{
		md5Mesh* currentMesh = &meshes[meshIndex];
		float* currentTextureArray = texturArraySkin[meshIndex];
		for(int i=0; i < currentMesh->verticesCount; i++)
		{
			md5Vertex* currentVertex = &currentMesh->vertices[i];
			*currentTextureArray++ = currentVertex->textureCoor.u;
			*currentTextureArray++ = currentVertex->textureCoor.v;
		}
	}
}



void md5Object::ComputeBoundingBox(){
    for (int i = 0; i < meshesCount ; i++) {
        md5Mesh* currentMesh = &meshes[i];
        prepareVertexArray(i);
        float* currentFloat = vertexArraySkin[i];
        for(int j = 0 ; j < currentMesh->verticesCount ; j++ ){
            vertexAttribs->AABB.SetBoxExtrema(GusVec3(*currentFloat++,*currentFloat++,*currentFloat++));
        }
    }
    
    vertexAttribs->AABB.BuildBoxFromExtrema();
}



void md5Object::prepareVertexArray(int meshIndex)
{
	
	md5Mesh* currentMesh = &meshes[meshIndex];
	
	if (MD5_DEBUG)
		printf("Preparing meshes #%d\n.",meshIndex);
	
	//cout  << "Preparing vertex..." << endl;
	// Setup vertices 
	float* currentFloat = vertexArraySkin[meshIndex];
	float* currentNormal = normalArraySkin[meshIndex];
	float* currentTangent = tangentArraySkin[meshIndex];

	//cout  << "Entering the loop..." << endl;
	md5Vertex* currentVertex = currentMesh->vertices;
	for (int i = 0; i < currentMesh->verticesCount; i++, currentVertex++)
    {
		
		float3 finalVertex ;
		float3 finalNormal;
		float3 finalTangent;
		// Calculate final vertex to draw with weights 
		for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
		{
			md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
			md5joint* joint= &joints[weight->jointIndex];
			
			// Calculate transformed vertex for this weight 
			if (MD5_DEBUG)
			{
				printf("weight->position x=%.2f,y=%.2f,z=%.2f\n",weight->position.x,weight->position.y,weight->position.z);
				printf("join->orientation x=%.2f,y=%.2f,z=%.2f,w=%.2f\n",joint->orientation.x,joint->orientation.y,joint->orientation.z,joint->orientation.w);
			}
			float3 displacement = joint->orientation.rotatePoint(weight->position);
			
			if (MD5_DEBUG)
				printf("displacement x=%.2f,y=%.2f,z=%.2f\n",displacement.x,displacement.y,displacement.z);
			//Quat_rotatePoint (joint->orient, weight->pos, wv);
			
			// The sum of all weight->bias should be 1.0 
			float3 t = joint->position + displacement;
			
			

			finalVertex += t * weight->bias;
			

			finalNormal += joint->orientation.rotatePoint(weight->normal) * weight->bias;
			//printf("final vertex : %f       final normal : %f\n",finalVertex.x, finalNormal.x);
			finalTangent+= joint->orientation.rotatePoint(weight->tangent) * weight->bias;


		}

		finalNormal.normalize();
		finalTangent.normalize();
		
		if (MD5_DEBUG)
			printf("finalVertex x=%.2f,y=%.2f,z=%.2f\n",finalVertex.x,finalVertex.y,finalVertex.z);
		/*
		*currentFloat++ = finalVertex.x; //currentFloat++;
		*currentFloat++ = finalVertex.y; //currentFloat++;
		*currentFloat++ = finalVertex.z; //currentFloat++;	
		*/
		*currentFloat++ = finalVertex.x; //currentFloat++;
		*currentFloat++ = finalVertex.y; //currentFloat++;
		*currentFloat++ = finalVertex.z; //currentFloat++;	


		*currentNormal++ = finalNormal.x;
		*currentNormal++ = finalNormal.y;
		*currentNormal++ = finalNormal.z;

		*currentTangent++ = finalTangent.x;
		*currentTangent++ = finalTangent.y;
		*currentTangent++ = finalTangent.z;
        
        

		if (MD5_DEBUG)
			printf("Back finalVertex x=%.2f,y=%.2f,z=%.2f\n",*(currentFloat-3),*(currentFloat-2),*(currentFloat-1));

		
    }

	
    //printf("meshes Index : %d\n", meshIndex);
	//glBindVertexArrayOES(vaoId[meshIndex]);
    
	//Upload vertex array
	glBindBuffer(GL_ARRAY_BUFFER,vboVertexId[meshIndex]);
	glBufferData(GL_ARRAY_BUFFER,currentMesh->verticesCount * 3  * sizeof(float),vertexArraySkin[meshIndex],GL_DYNAMIC_DRAW);
    
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0,0 );
	glEnableVertexAttribArray(vertexLocation);


	//Upload normal array
	glBindBuffer(GL_ARRAY_BUFFER,vboNormalId[meshIndex]);
	glBufferData(GL_ARRAY_BUFFER,currentMesh->verticesCount * 3  * sizeof(float),normalArraySkin[meshIndex],GL_DYNAMIC_DRAW);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLocation);
	

	glBindBuffer(GL_ARRAY_BUFFER,vboTangentId[meshIndex]);
	glBufferData(GL_ARRAY_BUFFER,currentMesh->verticesCount * 3  * sizeof(float),tangentArraySkin[meshIndex],GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,vboTexturId[meshIndex]);
    glVertexAttribPointer(texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texCoordsLocation);
}



void md5Object::renderSkin(int i)
{
	

		//glBindTexture(GL_TEXTURE_2D,0);
		//glBindVertexArrayOES(vaoId[i]);
/*
		glBindBuffer(GL_ARRAY_BUFFER,vboVertexId[i]);
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0,0 );
		glEnableVertexAttribArray(vertexLocation);
		
		glBindBuffer(GL_ARRAY_BUFFER,vboNormalId[i]);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(normalLocation);

		glBindBuffer(GL_ARRAY_BUFFER,vboTangentId[i]);
		glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(tangentLocation);


		 glBindBuffer(GL_ARRAY_BUFFER,vboTexturId[i]);
		 glVertexAttribPointer(texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
		 glEnableVertexAttribArray(texCoordsLocation);


		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboElementsId[i]);
*/
    
  //  if (RenderColorDBG::isModeEnable[TRI_MODE]) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboElementsId[i]);
		glDrawElements (GL_TRIANGLES, meshes[i].facesCount * 3 , GL_UNSIGNED_INT, 0);
  //  }
    
    if (RenderColorDBG::isModeEnable[POINT_MODE]) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboElementsId[i]);
        glDrawElements (GL_POINTS, meshes[i].facesCount * 3 , GL_UNSIGNED_INT, 0);
    }
    
    if (RenderColorDBG::isModeEnable[LINE_MODE]) {
        //Render::drawPolyOn = false;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboElementsId_0[i]);
		glDrawElements (GL_LINES, meshes[i].facesCount * 6 , GL_UNSIGNED_INT, 0);
    }
}


void md5Object::render()
{
	for(int i=0 ; i <   meshesCount ; i++)
	{
		renderSkin(i);
	}
}

void md5Object::RenderShadow()
{
	for(int i=0 ; i <   meshesCount ; i++)
		{
		prepareVertexArray(i);
		glBindBuffer(GL_ARRAY_BUFFER,vboVertexId[i]);
		glVertexAttribPointer(vertexShadowLocation, 3, GL_FLOAT, GL_FALSE, 0,0 );
		glEnableVertexAttribArray(vertexShadowLocation);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboElementsId[i]);
		glDrawElements (GL_TRIANGLES, meshes[i].facesCount * 3 , GL_UNSIGNED_INT, 0);
		}
}

void md5Object::generateNormalsAndTangents()
{	
	md5Mesh* currentMesh = NULL;
	for(int meshCounter=0; meshCounter < meshesCount ; meshCounter++)
	{
		currentMesh = &meshes[meshCounter];
	if (MD5_DEBUG) printf("Succesfully opened anim file '%d'.\n", meshCounter);
		// Calculate the vertex positions
		//float3* vertexFinalPosition = (float3*)malloc(currentMesh->verticesCount * sizeof(float3)); 
		//memset(vertexFinalPosition,0,sizeof(float3));
		float3* vertexFinalPosition = (float3*)calloc(currentMesh->verticesCount,sizeof(float3)); 
		
		md5Vertex* currentVertex = NULL;
		for(int verticesCounter=0 ; verticesCounter < currentMesh->verticesCount ; verticesCounter++)
		{
			currentVertex = &currentMesh->vertices[verticesCounter];
			float3 finalVertex;
			for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
			{
				md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
				md5joint* joint= &joints[weight->jointIndex];

				float3 displacement = joint->orientation.rotatePoint(weight->position);

				float3 t = joint->position + displacement;
				finalVertex += t * weight->bias;

				//Also zero normal for later use
				weight->normal = float3();
			}

			vertexFinalPosition[verticesCounter] = finalVertex;
		}

		// We now have all the vertex in final position

		// Calculate the nornals
		float3* normalsFinalPosition = (float3*)calloc(currentMesh->verticesCount, sizeof(float3)); 
		//memset(normalsFinalPosition,0,sizeof(float3));

		float3* tangentFinalPosition = (float3*)calloc(currentMesh->verticesCount, sizeof(float3)); 
		//memset(tangentFinalPosition,0,sizeof(float3));


		md5Face* currentFace = NULL;
		for(int facesCounter = 0; facesCounter < currentMesh->facesCount ; facesCounter++)
		{
			currentFace = &currentMesh->faces[facesCounter];

			// Normal part
			float3 v1 = vertexFinalPosition[currentFace->point3Index] - vertexFinalPosition[currentFace->point1Index];
			float3 v2 = vertexFinalPosition[currentFace->point2Index] - vertexFinalPosition[currentFace->point1Index];
			float3 normal = v1.crossProduct(v2);

			normalsFinalPosition[currentFace->point1Index] += normal;
			normalsFinalPosition[currentFace->point2Index] += normal;
			normalsFinalPosition[currentFace->point3Index] += normal;
			// End normal part


			// The following part is from "Mathematic for 3D programming" by Eric Lengyel
			// Tangent part

			float2 st1 = currentMesh->vertices[currentFace->point3Index].textureCoor - currentMesh->vertices[currentFace->point1Index].textureCoor;
			float2 st2 = currentMesh->vertices[currentFace->point2Index].textureCoor - currentMesh->vertices[currentFace->point1Index].textureCoor;

			// We now have a :

			//	[v1.x		v1.y		v1.z	]	=			[st1.s	st1.t][Tx		Ty		Tz]
			//	[v2.x		v2.y		v2.z	]				[st2.s	st2.t][Bx		By		Bz]

			//[Tx		Ty		Tz]	=			1/(st1.s * st2.t - st2.s * st1.t) *	[	 st2.t	-st1.t	][v1.x		v1.y		v1.z	]
			//[Bx		By		Bz]													[	-st2.s	 st1.s	][v2.x		v2.y		v2.z	]
				
			
			float coef = 1/ (st1.u * st2.v - st2.u * st1.v);
			float3 tangent;

			tangent.x = coef * ((v1.x * st2.v)  + (v2.x * -st1.v));
			tangent.y = coef * ((v1.y * st2.v)  + (v2.y * -st1.v));
			tangent.z = coef * ((v1.z * st2.v)  + (v2.z * -st1.v));


			tangentFinalPosition[currentFace->point1Index] += tangent;
			tangentFinalPosition[currentFace->point2Index] += tangent;
			tangentFinalPosition[currentFace->point3Index] += tangent;
			// End tangent part
		}

		// Normalize normals and tangent
		for(int verticesCounter=0 ; verticesCounter < currentMesh->verticesCount ; verticesCounter++)
		{
			normalsFinalPosition[verticesCounter].normalize();
			tangentFinalPosition[verticesCounter].normalize();
		}

		// Translating the normal orientation from object to joint space and Store normals inside weights, 
		for(int verticesCounter=0 ; verticesCounter < currentMesh->verticesCount ; verticesCounter++)
		{
			currentVertex = &currentMesh->vertices[verticesCounter];
			for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
			{
				md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
				md5joint* joint= &joints[weight->jointIndex];

				float3 jointSpaceNormal = joint->orientation.copyAndInverse().rotatePoint(normalsFinalPosition[verticesCounter]);
				weight->normal += jointSpaceNormal;

				float3 jointSpaceTangent = joint->orientation.copyAndInverse().rotatePoint(tangentFinalPosition[verticesCounter]);
				weight->tangent += jointSpaceTangent; 
			}
		}

		// Normalize weight normals
		for (int weightCounter = 0; weightCounter < currentVertex->weightCount; weightCounter++)
		{
			md5Weight* weight = &currentMesh->weights[currentVertex->startWeightIndex + weightCounter];
			weight->normal.normalize();
			weight->tangent.normalize();
		}
		
		free(vertexFinalPosition);
		free(normalsFinalPosition);
		free(tangentFinalPosition);
	}
}

void md5Object::updatePosition(float indZ, float indX)
{

}

void md5Object::updateAnim()
{
	int previousFrame = (int)animCursor;
	float percentageProgression = (animCursor - previousFrame);
	animation->update(previousFrame,percentageProgression,joints);
}

void md5Object::update(int objAction)
{

 if (animation != NULL)
	{
        if (animCursor > 179.0) {
            animCursor = 61.0;
        }
	 animCursor = (animCursor + Time::GetDt()/1000.f*60.f);
    }
 	for(int i=0 ; i <   meshesCount ; i++)
		prepareVertexArray(i);

}


GusVec3 md5Object::GetBoxMin(int frame_idx){
    cout << "[MD5][GetBoxMin]" << endl
    << animation->GetBoxMin(frame_idx).x << " "
    << animation->GetBoxMin(frame_idx).y << " "
    << animation->GetBoxMin(frame_idx).z << endl;
    return animation->GetBoxMin(frame_idx);
}


GusVec3 md5Object::GetBoxMax(int frame_idx){
    cout << "[MD5][GetBoxMax]" << endl
    << animation->GetBoxMax(frame_idx).x << " "
    << animation->GetBoxMax(frame_idx).y << " "
    << animation->GetBoxMax(frame_idx).z << endl;
    return animation->GetBoxMax(frame_idx);
}


