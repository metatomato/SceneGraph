
#pragma once


#include "../Common/Quaternion.h"
#include "../Common/float3.h"
#include "../Common/float2.h"

#define MD5_VERSION 10
#define MD5_LOADING_ERROR 0
#define CHECK_LOADING_STATUS if (loadingStatus == MD5_LOADING_ERROR) return 
#define MD5_DEBUG 0




struct md5joint
{
	char name[64];
	int parent;
	float3 position;
	Quaternion orientation;
};

struct md5Vertex
{
	float2 textureCoor;
	
	int startWeightIndex; /* start weight */
	int weightCount     ; /* weight count */
};

struct md5Face
{
	int point1Index;
	int point2Index;
	int point3Index;
};

struct md5Line
{
	int point1Index;
	int point2Index;
};


struct md5Weight
{
	//md5joint* joint;
	int jointIndex;
	float bias;
	
	float3 normal;

	float3 tangent;
	
	float3 position;
};

struct md5Bbox
{
	float3 min;
	float3 max;
};


struct md5Mesh
{
	

	md5Vertex* vertices;
	int verticesCount;

	md5Face* faces;
	int facesCount;

	md5Weight* weights;
	int weightsCount;
    
    md5Line* lines;
	int linesCount;
    
	
};

