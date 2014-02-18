//
//  Matrix.h
//  helloGL
//
//  Created by metatomato on 2/2/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//

#include "Matrix.h"

#define EMPTY_MATRIX4  { 0.0, 0.0, 0.0, 0.0,\
0.0, 0.0, 0.0, 0.0,\
0.0, 0.0, 0.0, 0.0,\
0.0, 0.0, 0.0, 0.0 }

#define IDENTITY_MATRIX4 { 1.0, 0.0, 0.0, 0.0,\
0.0, 1.0, 0.0, 0.0,\
0.0, 0.0, 1.0, 0.0,\
0.0, 0.0, 0.0, 1.0 }

using namespace std;

GLvoid LoadIdentity4fv(const GLfloat* mat4){
    mat4 = ID_MAT4;
}

GLvoid LoadEmpty4fv(GLfloat matrix[16])
{
	matrix[0] = 0.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 0.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 0.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 0.0f;
}

GLvoid LoadIdentity4fv(GLfloat matrix[16])
{
	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;
    
	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;
    
	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;
    
	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}

GLvoid LoadIdentity3fv(GLfloat matrix[9])
{
	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
    
	matrix[3] = 0.0f;
	matrix[4] = 1.0f;
	matrix[5] = 0.0f;
    
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;
	matrix[8] = 1.0f;

}


void Translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;
    
    newmatrix[12] = x;
    newmatrix[13] = y;
    newmatrix[14] = z;
    
    Multiply4x4(matrix, newmatrix);
}

void Translatef(GLfloat matrix[16], GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat temp[16];
    
	LoadIdentity4fv(temp);
    
	temp[12] = x;
	temp[13] = y;
	temp[14] = z;
	
	MultMatrixf(matrix, matrix, temp);
}


void Rotate(GLfloat *matrix, GLfloat angle, AXE axis)
{
    const GLfloat d2r = 0.0174532925199; /* PI / 180 */
    const int cos1[3] = { 5, 0, 0 };
    const int cos2[3] = { 10, 10, 5 };
    const int sin1[3] = { 6, 2, 1 };
    const int sin2[3] = { 9, 8, 4 };
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;
    
    newmatrix[cos1[axis]] = cos(d2r * angle);
    newmatrix[sin1[axis]] = -sin(d2r * angle);
    newmatrix[sin2[axis]] = -newmatrix[sin1[axis]];
    newmatrix[cos2[axis]] = newmatrix[cos1[axis]];
    
    Multiply4x4(matrix, newmatrix);
}

void Rotate3fv(GusVec3* vec, GLfloat angle, AXE axis)
{
    const GLfloat d2r = 0.0174532925199; /* PI / 180 */
    const int cos1[3] = { 5, 0, 0 };
    const int cos2[3] = { 10, 10, 5 };
    const int sin1[3] = { 6, 2, 1 };
    const int sin2[3] = { 9, 8, 4 };
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;
    
    newmatrix[cos1[axis]] = cos(d2r * angle);
    newmatrix[sin1[axis]] = -sin(d2r * angle);
    newmatrix[sin2[axis]] = -newmatrix[sin1[axis]];
    newmatrix[cos2[axis]] = newmatrix[cos1[axis]];
    
    Multiply4x4fv(newmatrix, vec);
}


GLvoid LookAt(GLfloat result[16], const GusVec3& eye, const GusVec3& center, const GusVec3& in_up)
{
    GLfloat forward[3], side[3], up[3];
    GLfloat matrix[16];
    
    forward[0] = center.x - eye.x;
    forward[1] = center.y - eye.y;
    forward[2] = center.z - eye.z;
	VecNormalizef(forward);
    
    up[0] = in_up.x;
    up[1] = in_up.y;
    up[2] = in_up.z;
    
    VecNormalizef(up);
    
	VecCrossf(side, forward, up);
    
	VecNormalizef(side);
    
	VecCrossf(up, side, forward);
    
	matrix[0] = side[0];
	matrix[1] = up[0];
	matrix[2] = -forward[0];
	matrix[3] = 0.0f;
	matrix[4] = side[1];
	matrix[5] = up[1];
	matrix[6] = -forward[1];
	matrix[7] = 0.0f;
	matrix[8] = side[2];
	matrix[9] = up[2];
	matrix[10] = -forward[2];
	matrix[11] = 0.0f;
	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
    
	
	LoadIdentity4fv(result);
	MultMatrixf(result,result, matrix);
    
	Translatef(result, -eye.x, -eye.y, -eye.z);
}


GLvoid Frustumf(GLfloat result[16], GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal)
{
	result[0] = 2.0f*nearVal/(right-left);
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = 2.0f*nearVal/(top-bottom);
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = (right+left)/(right-left);
	result[9] = (top+bottom)/(top-bottom);
	result[10] = -(farVal+nearVal)/(farVal-nearVal);
	result[11] = -1.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = -(2.0f*farVal*nearVal)/(farVal-nearVal);
	result[15] = 0.0f;
}

GLvoid Perspectivef(GLfloat result[16], GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;
    const GLfloat PIf = 0.0174532925199;
	ymax = zNear * tanf(fovy * PIf / 2.0f);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    
    Frustumf(result, xmin, xmax, ymin, ymax, zNear, zFar);
}

void Multiply4x4(GLfloat *m1, GLfloat *m2)
{
    GLfloat temp[16];
    
    int x,y;
    
    for (x=0; x < 4; x++)
    {
        for(y=0; y < 4; y++)
        {
            temp[y + (x*4)] = (m1[x*4] * m2[y]) +
            (m1[(x*4)+1] * m2[y+4]) +
            (m1[(x*4)+2] * m2[y+8]) +
            (m1[(x*4)+3] * m2[y+12]);
        }
    }
    
    memcpy(m1, temp, sizeof(GLfloat) << 4);
}


void Multiply4x4fv(GLfloat *mat, GusVec3* vec)
{
    GLfloat temp[4];
    
    for (int x = 0 ; x < 4 ; x++)
    {
            temp[x] = ( mat[ 0  + x ] * vec->x) +
                      ( mat[ 4  + x ] * vec->y) +
                      ( mat[ 8  + x ] * vec->z) +
                      ( mat[ 12 + x ] * 1.0   );
    }
    vec->x = temp[0];
    vec->y = temp[1];
    vec->z = temp[2];
}


void Scale4x4(GLfloat* mat, GLfloat factor){
	mat[0]  *= factor;
	mat[5]  *= factor;
	mat[10] *= factor;
}



GLvoid VecCrossf(GLfloat result[3], const GLfloat vector0[3], const GLfloat vector1[3])
{
	GLuint i;
    
	GLfloat	temp[3];
	
	temp[0] = vector0[1]*vector1[2] - vector0[2]*vector1[1];
	temp[1] = vector0[2]*vector1[0] - vector0[0]*vector1[2];
	temp[2] = vector0[0]*vector1[1] - vector0[1]*vector1[0];
	
	for (i = 0; i < 3; i++)
	{
    	result[i] = temp[i];
	}
}

GLvoid VecNormalizef(GLfloat vector[3])
{
	GLuint i;
    
	GLfloat a = VecAmountf(vector);
    
    if (a == 0.0f)
	{
    	return;
	}
    
	for (i = 0; i < 3; i++)
	{
    	vector[i] /= a;
	}
}

GLfloat VecAmountf(const GLfloat vector[3])
{
	return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void  MultMatrixf(float matrix[16], const float matrix0[16], const float matrix1[16])
{
	int i;
	int k;
    
	float temp[16];
    
	for (i = 0; i < 16; i++)
	{
		temp[i] = 0.0f;
		
		for(k = 0; k < 4; k++)
		{
			//			  		row   column   		   row column
			temp[i] += matrix0[(i%4)+(k*4)] * matrix1[(k)+((i/4)*4)];
		}
	}
    
	for (i = 0; i < 16; i++)
	{
		matrix[i] = temp[i];
	}
}

void SubMatrix(GLfloat matrix1[16],GLfloat matrix2[9])
{
	matrix2[0] = matrix1[0];
	matrix2[1] = matrix1[1];
	matrix2[2] = matrix1[2];
    
	matrix2[3] = matrix1[4];
	matrix2[4] = matrix1[5];
	matrix2[5] = matrix1[6];
    
	matrix2[6] = matrix1[8];
	matrix2[7] = matrix1[9];
	matrix2[8] = matrix1[10];
}

void InverseMatrix(GLfloat matrix1[16],GLfloat matrix2[9])
{
	GLfloat temp[9];
	SubMatrix(matrix1,temp);
	GLfloat Det = Det3x3(temp);
	
	matrix2[0] = 1.0/Det*(temp[4]*temp[8]-temp[5]*temp[7]);
	matrix2[1] = 1.0/Det*(temp[7]*temp[2]-temp[1]*temp[8]);
	matrix2[2] = 1.0/Det*(temp[1]*temp[5]-temp[4]*temp[2]);
    
	matrix2[3] = 1.0/Det*(temp[6]*temp[5]-temp[8]*temp[3]);
	matrix2[4] = 1.0/Det*(temp[0]*temp[8]-temp[2]*temp[6]);
	matrix2[5] = 1.0/Det*(temp[3]*temp[2]-temp[0]*temp[5]);
    
	matrix2[6] = 1.0/Det*(temp[3]*temp[7]-temp[6]*temp[4]);
	matrix2[7] = 1.0/Det*(temp[6]*temp[1]-temp[0]*temp[7]);
	matrix2[8] = 1.0/Det*(temp[0]*temp[4]-temp[1]*temp[3]);
}

GLfloat Det3x3(GLfloat matrix[9])
{
	return(  matrix[0]* (matrix[4]*matrix[8]-matrix[5]*matrix[7])
           - matrix[1]* (matrix[3]*matrix[8]-matrix[5]*matrix[6])
           + matrix[2]* (matrix[3]*matrix[7]-matrix[4]*matrix[6]) );
    
}


void AddRow3fv(GLfloat matrix[9], int row_id, GusVec3 row) {
	matrix[3 * row_id] 	  = row.x;
	matrix[3 * row_id +1] = row.y;
	matrix[3 * row_id +2] = row.z;
}


void AddRow4fv(GLfloat matrix[16], int row_id, GusVec3 row) {
	matrix[4 * row_id] 	  = row.x;
	matrix[4 * row_id +1] = row.y;
	matrix[4 * row_id +2] = row.z;
}


GLvoid TransposeMatrix3fv(GLfloat matrix[9]){
    float temp = 0.0f;
    temp = matrix[1];
    matrix[1] = matrix[3];
    matrix[3] = temp;
    temp = matrix[2];
    matrix[2] = matrix[6];
    matrix[6] = temp;
    temp = matrix[5];
    matrix[5] = matrix[7];
    matrix[7] = temp;
}



GLvoid ShowMatrix4fv(GLfloat matrix[16], string name)
{
    ostringstream description;
    description.precision(8);
    description << "MATRIX " << name << endl;
    for(int i=0; i<4; i++)
    {
        for (int j = 0 ; j < 4; j++) {

            description << matrix[i + j * 4] << " ";
        }
        description << endl;
    }
   cout << description.str() << endl;
}



GLvoid ShowMatrix3fv(GLfloat matrix[9], string name)
{
    ostringstream description;
    description << "MATRIX " << name << endl;
    for(int i=0; i<3; i++)
    {
        for (int j = 0 ; j < 3; j++) {
            
            description << setw(9) << matrix[i + j * 3] << "   ";
        }
        description << endl;
    }
    cout << description.str() << endl;
}



void CopyMatrix(GLfloat matrix_copy[16],const GLfloat matrix[16]) {
    for(int i=0; i<16; i++) {
        matrix_copy[i] = matrix[i];
    }
}



void CopyMatrix(const GLfloat* matrix1,const GLfloat* matrix2){
    matrix2 = matrix1;
}

void CopyMatrixDeeply(const GLfloat* matrix1, GLfloat* matrix2) {
	for(int i = 0 ; i < 16 ; i++) {
		matrix2[i] = matrix1[i];
	}
}



































