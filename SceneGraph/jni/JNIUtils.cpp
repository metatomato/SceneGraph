/*
 * JNIUtils.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: tomato
 */

#include "JNIUtils.h"


string  CastjStringToString(JNIEnv *env, jobject object, jstring java_string) {

	    	const char *char_ptr;

	        char_ptr = env->GetStringUTFChars(java_string, NULL);

	        string cpp_string(char_ptr);

	        env->ReleaseStringUTFChars(java_string,char_ptr);

	    return cpp_string;
}



vector<string>  CastStringArrayToVector(JNIEnv *env, jobject object, jobjectArray stringArray) {

		vector<string> received_files;

	    for (int i = 0; i < env->GetArrayLength(stringArray); i++)
	    {
	    	const char *char_ptr;

	        jstring java_string = (jstring) env->GetObjectArrayElement(stringArray, i);

	        char_ptr = env->GetStringUTFChars(java_string, NULL);

	        received_files.push_back(string(char_ptr));

	        // Don't forget to call `ReleaseStringUTFChars` when you're done.

	        env->ReleaseStringUTFChars(java_string,char_ptr);
	    }

	    return received_files;
}



jobjectArray  CastStringVectorToArray(JNIEnv *env, jobject object, vector<string>& vec) {

		jsize size = vec.size();
		jobjectArray ret;

	    ret= (jobjectArray)env->NewObjectArray(size, env->FindClass("java/lang/String"), env->NewStringUTF(""));

	    for(int i = 0 ; i < size ; i++) {
	        env->SetObjectArrayElement( ret, i, env->NewStringUTF(vec.at(i).c_str()));
	    }

	    return ret;
}





vector<int>  CastIntArrayToVector(JNIEnv *env, jobject object, jintArray int_array) {

		vector<int> received_files;

		int* c_array = env->GetIntArrayElements(int_array, NULL);

	    for (int i = 0; i < env->GetArrayLength(int_array); i++)
	    {
	        received_files.push_back(c_array[i]);
	    }

	    env->ReleaseIntArrayElements(int_array,c_array,0);

	    return received_files;
}



jintArray  CastIntVectorToArray(JNIEnv *env, jobject object, vector<int> vec) {

		jsize size = vec.size();
		int idx[size];
		jintArray ret;

	    ret= (jintArray)env->NewIntArray(size);

	    for(int i = 0 ; i < size ; i++) {
	    	idx[i] = vec.at(i);
	    	LOGE("[JNIUTILS] Put idx %d in array",idx[i]);
	    }

	    env->SetIntArrayRegion( ret, 0, size, idx);

	    return ret;
}
























