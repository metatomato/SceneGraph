/*
 * JNIUtils.h
 *
 *  Created on: Nov 16, 2013
 *      Author: tomato
 */

#ifndef JNIUTILS_H_
#define JNIUTILS_H_

#include <jni.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>


#include <android/log.h>
#define  LOG_TAG    "JNI_UTILS"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace std;

string  CastjStringToString(JNIEnv *env, jobject object, jstring java_string);
vector<string>  CastStringArrayToVector(JNIEnv *env, jobject object, jobjectArray stringArray);
jobjectArray  CastStringVectorToArray(JNIEnv *env, jobject object, vector<string>& vec);

vector<int>  CastIntArrayToVector(JNIEnv *env, jobject object, jintArray int_array);
jintArray  CastIntVectorToArray(JNIEnv *env, jobject object, vector<int> vec);

#endif /* JNIUTILS_H_ */
