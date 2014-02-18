#include "md5Animation.h"
#include "../Scene/Scene.h"
#include "../DataCenter/Import.h"

md5Animation::md5Animation(void)
{
	loadingErrors = true;
	boundBox = new float[24];
}

md5Animation::~md5Animation(void)
{
}

void md5Animation::load(const char* animationFile)
{
	// Open file
	string fileToOpen = Import::GetResourcesFolder() + "Res/models/" ;
	fileToOpen += animationFile;

	LOGE("[MD5ANIMATION][Load] Loading %s", fileToOpen.c_str());

	FILE* fp = fopen (fileToOpen.c_str(), "rb");
	if (!fp)
    {
		printf("Error: could not open file '%s'!\n", fileToOpen.c_str());
		return ;
    }


	readVersion(fp);				//CHECK_LOADING_STATUS;
	readCommandline(fp);				//CHECK_LOADING_STATUS;
	readNumFrames(fp);				//CHECK_LOADING_STATUS;
	readNumJoints(fp);				//CHECK_LOADING_STATUS;
	readFrameRate(fp);				//CHECK_LOADING_STATUS;
	readNumAnimatedComponents(fp);			//CHECK_LOADING_STATUS;
	readHierarchy(fp);				//CHECK_LOADING_STATUS;
	readBounds(fp);				///	CHECK_LOADING_STATUS;
	readBaseFrame(fp);				//CHECK_LOADING_STATUS;
	readFrames(fp);					//CHECK_LOADING_STATUS;

	fclose(fp);
	loadingErrors = false;
}

void md5Animation::readVersion(FILE* fp)
{
	while (sscanf (buff, " MD5Version %d",& this->version) != 1)
		fgets (buff, sizeof (buff), fp);

	if (version != MD5_VERSION)
	{
		/* Bad version */
		printf ( "Error: This version (%d) is not supported.\n", version);
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	else
	{
		if (MD5_DEBUG)
			printf ( "MD5 version (%d) is supported.\n",this->version);
	}
}
void md5Animation::readCommandline(FILE* fp)
{
	while (sscanf (buff, " commandline %s", this->commandline) != 1)
		fgets (buff, sizeof (buff), fp);
	
	if (MD5_DEBUG)
		printf("Command line is: %s.\n",commandline);
	// Nothing is done with the command line
}

void md5Animation::readNumFrames(FILE* fp)
{
	while(sscanf (buff, " numFrames %d", &this->numFrames ) != 1)
		fgets (buff, sizeof (buff), fp);

	if (numFrames == 0)
	{
		printf ( "Error: Could not find the number of joints in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (MD5_DEBUG) 
		printf("Found %d numFrames declaration, allocating memory.\n",numFrames);

	joints = (md5joint**)calloc(numFrames,sizeof(md5joint*));

	//Also allocate space for bounding boxes
	boundsCount = numFrames;
	bounds = (boundElement*)calloc(boundsCount,sizeof(boundElement));
}

void md5Animation::readNumJoints(FILE* fp)
{
	while(sscanf (buff, " numJoints %d", &this->numJoints ) != 1)
		fgets (buff, sizeof (buff), fp);

	if (numJoints == 0)
	{
		printf ( "Error: Could not find the number of joints in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (MD5_DEBUG) 
		printf("Found %d joints declaration, allocating memory.\n",numJoints);

	for(int frame=0 ; frame < numFrames; frame++)
		joints[frame] = (md5joint*)calloc(numJoints,sizeof(md5joint));
	
	//Also hierarchyCount is equals to jointCount
	hierarchyCount = numJoints;
	hierarchy = (hierarchyElement*) calloc(hierarchyCount,sizeof(hierarchyElement));

	baseFrameCount = numJoints;
	baseFrame = (md5joint*)calloc(baseFrameCount,sizeof(md5joint));
}

void md5Animation::readFrameRate(FILE* fp)
{
	while(sscanf (buff, " frameRate %d", &this->frameRate ) != 1)
		fgets (buff, sizeof (buff), fp);

	if (frameRate == 0)
	{
		printf ( "Error: Could not find the frameRate in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (MD5_DEBUG) 
		printf("Found frameRate=%d.\n",this->frameRate );

	
}

void md5Animation::readNumAnimatedComponents(FILE* fp)
{
	while(sscanf (buff, " numAnimatedComponents %d", &this->numAnimatedComponents ) != 1)
		fgets (buff, sizeof (buff), fp);

	if (numFrames == 0)
	{
		printf ( "Error: Could not find the numAnimatedComponents in the model.\n");
		loadingStatus = MD5_LOADING_ERROR;
		return ;
	}
	
	if (MD5_DEBUG) 
		printf("Found numAnimatedComponents=%d.\n",this->numAnimatedComponents );

	frames = (float**)calloc(this->numFrames,sizeof(float*));
	for(int i=0;i<numFrames;i++)
		frames[i] = (float*)calloc(this->numAnimatedComponents,sizeof(float));
}

void md5Animation::readHierarchy(FILE* fp)
{
	// Seach for beginning of the joints
	while(strncmp (buff, "hierarchy {", 8) != 0)
		fgets (buff, sizeof (buff), fp);

	hierarchyElement* currentElement = hierarchy;
	for(int hierarchyCounter=0;hierarchyCounter<hierarchyCount;hierarchyCounter++,currentElement++)
	{
		fgets (buff, sizeof (buff), fp);
		sscanf (buff, "%s%d%d%d // %*s",
				currentElement->name, 
				&currentElement->parent, 
				&currentElement->flags,
				&currentElement->startIndex
				);
		
		//A bit ugly but necessary: Remove the trailing '"' character
		currentElement->name[strlen(currentElement->name)-1] = '\0';

		if (0)
			printf("Read hierarchy element:'%s', parent=%d, flags=%d ,startIndex=%d\n",
			   currentElement->name,
			   currentElement->parent,
			   currentElement->flags,
			   currentElement->startIndex);
	}
}

void md5Animation::readBounds(FILE* fp)
{
	while(strncmp (buff, "bounds {", 8) != 0)
		fgets (buff, sizeof (buff), fp);

	boundElement* currentBound = bounds;
	for(int i=0; i < boundsCount ; i++,currentBound++)
	{
		fgets (buff, sizeof (buff), fp);
		sscanf (buff, " ( %f %f %f ) ( %f %f %f ) ",
				&currentBound->min.x, 
				&currentBound->min.y, 
				&currentBound->min.z,
				&currentBound->max.x,
				&currentBound->max.y,
				&currentBound->max.z
				);
		if (0)
			printf("Read bound element: min x=%.2f, y=%.2f ,z=%.2f, max x=%.2f, y=%.2f ,z=%.2f\n",
				currentBound->min.x, 
				currentBound->min.y, 
				currentBound->min.z,
				currentBound->max.x,
				currentBound->max.y,
				currentBound->max.z
				);
	}
}


void md5Animation::renderBounds(int frameIndex, GLint vertexLoc)
{
 unsigned int cube[]={0,1,2,3,
	4,5,6,7,
	0,1,5,4,
	3,2,6,7,
	1,2,6,5,
	0,3,7,4
	
	};
 

 boundBox[0] = bounds[frameIndex].min.x;
 boundBox[1] = bounds[frameIndex].min.y;
 boundBox[2] = bounds[frameIndex].min.z; 

 boundBox[3] = bounds[frameIndex].max.x;
 boundBox[4] = bounds[frameIndex].min.y;
 boundBox[5] = bounds[frameIndex].min.z;

 boundBox[6] = bounds[frameIndex].max.x;
 boundBox[7] = bounds[frameIndex].max.y;
 boundBox[8] = bounds[frameIndex].min.z;  

 boundBox[9] = bounds[frameIndex].min.x;
 boundBox[10] = bounds[frameIndex].max.y;
 boundBox[11] = bounds[frameIndex].min.z; 

 boundBox[12] = bounds[frameIndex].min.x;
 boundBox[13] = bounds[frameIndex].min.y;
 boundBox[14] = bounds[frameIndex].max.z; 

 boundBox[15] = bounds[frameIndex].max.x;
 boundBox[16] = bounds[frameIndex].min.y;
 boundBox[17] = bounds[frameIndex].max.z; 

 boundBox[18] = bounds[frameIndex].max.x;
 boundBox[19] = bounds[frameIndex].max.y;
 boundBox[20] = bounds[frameIndex].max.z; 

 boundBox[21] = bounds[frameIndex].min.x;
 boundBox[22] = bounds[frameIndex].max.y;
 boundBox[23] = bounds[frameIndex].max.z; 

 //printf("vertexLocation in animation %d \n", vertexLoc);

 glGenBuffers(1, &boundVbo);	
 glBindBuffer(GL_ARRAY_BUFFER, boundVbo);
 glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), (float*)boundBox,GL_DYNAMIC_DRAW);
 glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0,0 );
 glEnableVertexAttribArray(vertexLoc);

 glGenBuffers(1, &boundIndex);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundIndex);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*6*sizeof(float),(unsigned int*) cube, GL_DYNAMIC_DRAW);

 glDrawElements(GL_TRIANGLES, 24 , GL_UNSIGNED_INT, 0);


}


void md5Animation::readBaseFrame(FILE* fp)
{
	while(strncmp (buff, "baseframe {", 8) != 0)
		fgets (buff, sizeof (buff), fp);

	md5joint* currentFrameElement = baseFrame;
	for (int i = 0 ; i < baseFrameCount ; i++,currentFrameElement++)
	{
		fgets (buff, sizeof (buff), fp);
		sscanf (buff, " ( %f%f%f ) (%f%f%f) ",
				&currentFrameElement->position.x, 
				&currentFrameElement->position.y, 
				&currentFrameElement->position.z,
				&currentFrameElement->orientation.x,
				&currentFrameElement->orientation.y,
				&currentFrameElement->orientation.z
				);

		currentFrameElement->orientation.computeW();

		if (MD5_DEBUG)
			printf("Read baseFrame element: p.x=%.2f, p.y=%.2f ,p.z=%.2f, orient.x=%.2f, orient.y=%.2f ,orient.z=%.2f\n",
				currentFrameElement->position.x, 
				currentFrameElement->position.y, 
				currentFrameElement->position.z,
				currentFrameElement->orientation.x,
				currentFrameElement->orientation.y,
				currentFrameElement->orientation.z
				);
	}
}	

void md5Animation::readFrames(FILE* fp)
{
	float* currentFrame = NULL;



	for (int frameCounter=0;frameCounter<numFrames;frameCounter++)
	{
		currentFrame = frames[frameCounter];
		
		while(strncmp (buff, "frame ", 5) != 0)
			fgets (buff, sizeof (buff), fp);

		//float* currentFrameElement = currentFrame;
		int read=0;
		for (int i = 0; i < numAnimatedComponents; i++)
		{
				 read+= fscanf (fp, "%f", &currentFrame[i]);
				 
		}
		while(strncmp (buff, "} ", 1) != 0)
			fgets (buff, sizeof (buff), fp);
		
		/*
		while(strncmp (buff, "} ", 1) != 0)
		{
			fgets (buff, sizeof (buff), fp);
			int valueRead = sscanf (buff, " %f%f%f%f%f%f ",currentFrameElement,currentFrameElement+1,currentFrameElement+2,currentFrameElement+3,currentFrameElement+4,currentFrameElement+5);

			if (1)
				printf("REading frame: %d, line had %d elements.\n",frameCounter,valueRead);
			//For each float, push it in frames
			currentFrameElement+= valueRead;
		}
		*/
		


		
	}


	if (MD5_DEBUG)
		{
			//currentFrame = frames[0];
			
			for (int frameCounter=0;frameCounter<numFrames;frameCounter++)
			{
				currentFrame = frames[frameCounter];

				printf("\n\nCurrentFrame Address = %p\n", currentFrame);
				float* verif = currentFrame;
				printf("verif frame:%d/%d\n",frameCounter,numFrames);
				for(int i=0;i<numAnimatedComponents;i++)
					printf("%.2f,",*verif++);
			
				printf("done with frame: %d\n",frameCounter);
				
			}
		}
}

void md5Animation::generateAllSkeletons(void)
{
	for(int i=0;i<numFrames;i++)
		generateSkeletonsForFrame(i);
}


#define POSITION_X_UPDATE 0x01
#define POSITION_Y_UPDATE 0x02
#define POSITION_Z_UPDATE 0x04
#define ORIENTAT_X_UPDATE 0x08
#define ORIENTAT_Y_UPDATE 0x10
#define ORIENTAT_Z_UPDATE 0x20

void md5Animation::generateSkeletonsForFrame(int frameIndex)
{
	float* frameInfo = frames[frameIndex];

	//printf("Building frames %d\n",frameIndex);

	md5joint* currentSkeleton = joints[frameIndex];

	for( int jointCounter = 0 ; jointCounter < numJoints ; jointCounter++)
	{
		//init with baseframe joint
		float3 translation = baseFrame[jointCounter].position;
		Quaternion rotation = baseFrame[jointCounter].orientation;

		
		//printf("BF : x=%.2f, y=%.2f ,z=%.2f, o.x=%.2f, o.y=%.2f ,o.z=%.2f ,o.w=%.2f\n",				translation.x,				translation.y,				translation.z,				rotation.x,				rotation.y,				rotation.z,				rotation.w				);

		int infoPointer = hierarchy[jointCounter].startIndex;
		int flag = hierarchy[jointCounter].flags;

		//printf("using flag %d and info offset=%d\n",flag,hierarchy[jointCounter].startIndex);
		//Checkout currentHierarchyElmt, for requiered transformation
		if (flag & 1)
		{
			//printf("updating pos.x with %.2f\n",frameInfo[infoPointer]);
			translation.x = frameInfo[infoPointer++];
		}
		if (flag & 2)
		{
			//printf("updating pos.y with %.2f\n",frameInfo[infoPointer]);
			translation.y = frameInfo[infoPointer++];
		}
		if (flag & 4)
			translation.z = frameInfo[infoPointer++];

		if (flag & 8)
			rotation.x = frameInfo[infoPointer++];

		if (flag & 16)
			rotation.y = frameInfo[infoPointer++];

		if (flag & 32)
			rotation.z = frameInfo[infoPointer++];

		//Compute w for orientation
		rotation.computeW();
		

		//printf("BFT: x=%.2f, y=%.2f ,z=%.2f, o.x=%.2f, o.y=%.2f ,o.z=%.2f ,o.w=%.2f\n",translation.x, 				translation.y, 				translation.z,				rotation.x,				rotation.y,				rotation.z,				rotation.w				);

		md5joint* parent = &currentSkeleton[hierarchy[jointCounter].parent];

		//Add transformation to baseFrame joint 
		if (hierarchy[jointCounter].parent >=0)
		{
			/*
			printf("Has a parent:%d p.x=%.2f,p.y=%.2f,p.z=%.2f o.x=%.2f,o.y=%.2f,o,z=%.2f,o.w=%.2f\n",
				hierarchy[jointCounter].parent,
				parent->position.x,
				parent->position.y,
				parent->position.z,
				parent->orientation.x,
				parent->orientation.y,
				parent->orientation.z,
				parent->orientation.w
				);
				*/
			translation = parent->orientation.rotatePoint(translation);
			translation+= parent->position;

			rotation = parent->orientation * rotation  ;
			rotation.normalize();
		}
	//	else
	//		printf("Has not parent");

		/*
			  printf("BF: x=%.2f, y=%.2f, z=%.2f, o.x=%.2f,o.y=%.2f,o.z=%.2f,o.w=%.2f \n\n",
			currentSkeleton[jointCounter].position.x,
			currentSkeleton[jointCounter].position.y,
			currentSkeleton[jointCounter].position.z,
			currentSkeleton[jointCounter].orientation.x,
			currentSkeleton[jointCounter].orientation.y,
			currentSkeleton[jointCounter].orientation.z,
			currentSkeleton[jointCounter].orientation.w);
			*/

		//Write position into currentJoint
		currentSkeleton[jointCounter].position = translation;
		currentSkeleton[jointCounter].orientation = rotation;
		currentSkeleton[jointCounter].parent = hierarchy[jointCounter].parent;

		//printf("joint:%d p.x=%.2f,p.y=%.2f,p.z=%.2f o.x=%.2f,o.y=%.2f,o,z=%.2f,o.w=%.2f\n",jointCounter,currentSkeleton[jointCounter].position.x,currentSkeleton[jointCounter].position.y,currentSkeleton[jointCounter].position.z,currentSkeleton[jointCounter].orientation.x,currentSkeleton[jointCounter].orientation.y,currentSkeleton[jointCounter].orientation.z,currentSkeleton[jointCounter].orientation.w);

	//	printf("\n\n");
	}

	/*
	md5joint* currentJoint = joints[frameIndex];
	for (int i=0;i<numJoints;i++,currentJoint++)
	{
		//printf("joint:%d p.x=%.2f,p.y=%.2f,p.z=%.2f o.x=%.2f,o.y=%.2f,o,z=%.2f,o.w=%.2f\n",i,				currentJoint->position.x,				currentJoint->position.y,				currentJoint->position.z,				currentJoint->orientation.x,				currentJoint->orientation.y,				currentJoint->orientation.z,				currentJoint->orientation.w				);
	}*/
	//if (frameIndex > 3)
	//			exit(0);
	
	//exit(0);*/
}


void md5Animation::update(int frameIndex, float percentage,md5joint* currentJoint)
{
	md5joint* previousFrame = joints[frameIndex];
	md5joint* nextFrame = joints[(frameIndex+1) % numFrames];

	//printf("updating joint starting@: %d\n",currentJoint);
	for(int i =0 ; i < numJoints ; i++,previousFrame++,nextFrame++,currentJoint++)
	{
		//printf("updating joint %d @ %d \n",i, currentJoint);
		currentJoint->position = previousFrame->position +  (nextFrame->position - previousFrame->position) * percentage;
		currentJoint->orientation = Quaternion::slerp(previousFrame->orientation,nextFrame->orientation,percentage);
	
	}

}
