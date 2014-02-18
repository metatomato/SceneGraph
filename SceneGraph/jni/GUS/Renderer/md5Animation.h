#pragma once

#include "md5definitions.h"
#include "md5.h"
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
#define READ_BUFF_SIZE 1024


struct hierarchyElement
{
	char name[64] ;
	int parent    ;
	int flags     ;
	int startIndex;
};

struct boundElement
{
	GusVec3 min;
	GusVec3 max;
};


class md5Animation
{
	public:
		md5Animation(void);
		void load(const char* animationFile);
		void generateAllSkeletons(void);
		~md5Animation(void);

		// Here are stored all the skeletons (one per frame)
		int numJoints ;
		md5joint** joints;
		bool* jointsGenerated;

		md5joint* baseFrame;
		int numFrames ;
		int frameRate ;
		void update(int frameIndex, float percentage, md5joint* joint);

		float** frames;
		bool loadingErrors;

		boundElement* bounds;
		int boundsCount;
		void renderBounds(int frameIndex, GLint vertexLoc);
		float *boundBox;

    GusVec3             GetBoxMin(int frame_idx){return bounds[frame_idx].min;};
    GusVec3             GetBoxMax(int frame_idx){return bounds[frame_idx].max;};
    
	private:
	
		//md5Object* md5object;
		
		void generateSkeletonsForFrame(int frameIndex);
		
	
		GLuint boundVbo, boundIndex;
		
		

		hierarchyElement* hierarchy;
		int hierarchyCount;

		

		
		int baseFrameCount;

		
		
		int numAnimatedComponents ;

		

		

		// Loading variables;

		char buff[READ_BUFF_SIZE]; 
		int loadingStatus ;
		int version;
		char commandline[READ_BUFF_SIZE];
		void readVersion(FILE* fp);
		void readCommandline(FILE* fp);
		void readNumFrames(FILE* fp);
		void readNumJoints(FILE* fp);
		void readFrameRate(FILE* fp);
		void readNumAnimatedComponents(FILE* fp);
		void readHierarchy(FILE* fp);
		void readBounds(FILE* fp);
		void readBaseFrame(FILE* fp);
		void readFrames(FILE* fp);
};
