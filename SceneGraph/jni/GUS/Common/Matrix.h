//
//  Matrix.h
//  helloGL
//
//  Created by metatomato on 2/4/13.
//  Copyright (c) 2013 tomato. All rights reserved.
//
#pragma once

#include <stdio.h>
#include <math.h>
#include "GUS.h"
#include <iostream>




const GLfloat INF_NEG = -100000000.f;
const GLfloat INF_POS =  100000000.f;
const GLfloat PIf = 0.0174532925199;
const GLfloat ID_MAT4[16] = { 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 };


enum AXE { X_AXE , Y_AXE , Z_AXE };

GLvoid LoadIdentity4fv(const GLfloat* mat4);

GLvoid LoadEmpty4fv(GLfloat matrix[16]);

GLvoid LoadIdentity4fv(GLfloat matrix[16]);

GLvoid LoadIdentity3fv(GLfloat matrix[9]);

void Translatef(GLfloat matrix[16], GLfloat x, GLfloat y, GLfloat z);

void Translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z);

void Rotate(GLfloat *matrix, GLfloat angle, AXE axis);

void Rotate3fv(GusVec3* vec, GLfloat angle, AXE axis);

GLvoid LookAt(GLfloat result[16], const GusVec3& eye, const GusVec3& center, const GusVec3& up);

GLvoid Frustumf(GLfloat result[16], GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal);

void Multiply4x4fv(GLfloat *mat, GusVec3* vec);

void Multiply4x4(GLfloat *m1, GLfloat *m2);

void Scale4x4(GLfloat* mat, GLfloat factor);

GLvoid Perspectivef(GLfloat result[16], GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

GLvoid VecCrossf(GLfloat result[3], const GLfloat vector0[3], const GLfloat vector1[3]);

GLvoid VecNormalizef(GLfloat vector[3]);

GLfloat VecAmountf(const GLfloat vector[3]);

void  MultMatrixf(float matrix[16], const float matrix0[16], const float matrix1[16]);

GLvoid ShowMatrix4fv(GLfloat matrix[16],string name);

GLvoid ShowMatrix3fv(GLfloat matrix[9],string name);

GLvoid TransposeMatrix3fv(GLfloat matrix[9]);

GLfloat Det3x3(GLfloat matrix[9]);

void AddRow3fv(GLfloat matrix[9], int row_id, GusVec3 row);

void AddRow4fv(GLfloat matrix[16], int row_id, GusVec3 row);

void SubMatrix(GLfloat matrix1[16],GLfloat matrix2[9]);

void InverseMatrix(GLfloat matrix1[16],GLfloat matrix2[9]);

void CopyMatrix(GLfloat matrix_copy[16], const GLfloat matrix2[16]);

void CopyMatrix(const GLfloat* matrix1, const GLfloat* matrix2);

void CopyMatrixDeeply(const GLfloat* matrix1, GLfloat* matrix2);

































