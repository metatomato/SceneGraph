/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <JNIUtils.h>
#include <android/asset_manager_jni.h>


#include "GUS/Common/RenderPass.h"
#include "GUS/Common/Time.h"
#include "GUS/DataCenter/DataCenter.h"
#include "GUS/DataCenter/Import.h"
#include "GUS/Logic/Cortex.h"
#include "GUS/Renderer/Engine.h"
#include "GUS/Renderer/GusGLObject.h"
#include "GUS/Renderer/Renderer.h"
#include "GUS/Scene/Control.h"
#include "GUS/Scene/Scene.h"
#include "GUS/System/ChannelOffice.h"
#include "GUS/System/GusTV.h"

#ifdef ANDROID_TEGRA
	#define HELLO_TEGRA "Hello Adreno 320!!"
#else
	#define HELLO_TEGRA "Generic device!!"
#endif

//#define RESOURCE_PATH "/storage/emulated/0/SCENEGRAPH/Res/"
#define RESOURCE_PATH "/storage/sdcard0/SCENEGRAPH/Res/"

#define  LOG_TAG    "GL_CODE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

int xScreen = 0;
int yScreen = 0;
float density = 1.0;

void renderFrame();
void InitInsetFrame();

const string* filePath;

Scene scene;
Engine renderEngine;
DataCenter dataCentral;

GLuint framebuffer;
GLuint colorRenderbuffer;
GLuint depthRenderbuffer;

GLuint defaultFrameBuffer;

int8_t *data = nullptr;

bool snapshot = false;

int prevLoc_x = 0;
int prevLoc_y = 0;


enum SNAPSHOT_MODE{INVERSE, INVERSE_TOP};

int fps_frame = 0;
float fps_time = 0.f;

void InitEngine() {

	LOGE("BEGIN SCENE INIT!!!");

	Renderer::frameView[0] = xScreen;
	Renderer::frameView[1] = yScreen;
	Renderer::aspectRatio = (float)xScreen/ (float)yScreen;

	InitInsetFrame();

	LOGE("CORTEX INIT!!! %d %d %f",Renderer::frameView[0],Renderer::frameView[1],Renderer::aspectRatio);

	scene.SetDataLink(dataCentral);

	Time::InitTime();

	string path = *filePath;

	Import::SetResourcesFolder(path.c_str());
	Import::SetDocumentsFolder(path.c_str());

	renderEngine.InitStage();


	Cortex::ConnectChannel(ChannelOffice::CHANNEL::MOTION);
	GusTV::RegisterChannelShow();


}


void InitPath(string path){
	filePath = new string(path);
}


void InitStage() {
	scene.Init(dataCentral.GetSceneData());
}


void InitInsetFrame(){
	float dp_ratio = ((float)xScreen) / density;
	if(dp_ratio >= 1280.f ){
		LOGE("[GL_CODE][InitInsetFrame] INIT FOR %f", 1280 * density);
		Renderer::insetFrame[0] = (int)(110 * density);
		Renderer::insetFrame[1] = yScreen - (int)(30  * density + 480 * density);
		Renderer::insetFrame[2] = (int)(768 * density);
		Renderer::insetFrame[3] = (int)(480 * density);
	} else
	if(dp_ratio >= 960.f){
		LOGE("[GL_CODE][InitInsetFrame] INIT FOR %f", 960 * density);
		Renderer::insetFrame[0] = (int)(75 * density);
		Renderer::insetFrame[1] = yScreen - (int)(15  * density + 400 * density);
		Renderer::insetFrame[2] = (int)(640 * density);
		Renderer::insetFrame[3] = (int)(400 * density);
	}else
	if(dp_ratio >= 640.f){
		LOGE("[GL_CODE][InitInsetFrame] INIT FOR %f", 640 * density);
		Renderer::insetFrame[0] = (int)(55 * density);
		Renderer::insetFrame[1] = yScreen - (int)(15  * density + 200 * density);
		Renderer::insetFrame[2] = (int)(320 * density);
		Renderer::insetFrame[3] = (int)(200 * density);
	}
}

void LoadData(vector<string> file_list) {
	LOGE("BEGIN SCENE LOAD_DATA!!!");

	for(auto el : file_list){
		dataCentral.FetchDataFromFile(el);
	}
	//dataCentral.PrintEntityFromFiles();
	//dataCentral.PrintStageFromFiles();

}


bool setupGraphics(int w, int h) {

}

void TakeSnapshot() {
		GLsizei width  = xScreen;
		GLsizei height = yScreen;

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenRenderbuffers(1, &colorRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

		glGenRenderbuffers(1, &depthRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
		if(status != GL_FRAMEBUFFER_COMPLETE) {
			LOGE("failed to make complete framebuffer object %x", status);
		}

		Renderer::defaultFBO = framebuffer;

		LOGE("[GL_CODE] set render to framebuffer %d" ,framebuffer);

		scene.SnapshotMode(true);

		renderFrame();
}

jbyte* CopyBuffer()
{
	GLsizei width  = xScreen;
	GLsizei height = yScreen;
	GLsizei x = 0;
	GLsizei y = 0;
	int fbo_id;

	data = new int8_t[width * height * 4];

	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	glReadPixels(x, y, width, height, GL_RGBA,GL_UNSIGNED_BYTE,data);

	Renderer::defaultFBO = 0;

	LOGE("[GL_CODE] Copying Buffer %d" ,fbo_id);

	return data;
}


void ClearData() {
	delete[] data;
}



void ComputeFPS() {
	if(fps_frame == 30){
		float fps = 30.f/fps_time;
		//LOGE("[GL_CODE][ComputeFPS] FPS %f",fps);
		fps_frame = 0;
		fps_time = 0.f;
	}

	fps_frame++;
	fps_time += Time::GetDt()/1000.f;
}



void renderFrame() {

	Time::UpdateClock();

	    GusTV::StartBroadCasting();


	    if(Cortex::firstInit)
	    {
	        Cortex::InitNervousSystem();
	        Cortex::firstInit = false;
	    }

	   // Cortex::Stimulate();

	    scene.UpdateScene(Time::GetDt()/1000.f);

	    vector<RenderPass<GusGLObject>> vec_pass = dataCentral.SceneToRender(scene.GetPass());

	    renderEngine.PushToStage(vec_pass);

	    renderEngine.Render();

	    GusTV::EndBroadCasting();

	    if(snapshot) {
	    	LOGE("[GL_CODE] Photo from GL!!");
	    	snapshot = false;
	    }
	    ComputeFPS();
}



void touchesMoved(float input_x, float input_y,float input_dx, float input_dy)
{
            Control::dx = input_dx;
            Control::dy = input_dy;

            Control::x = input_x;
            Control::y = input_y;

          //  LOGE("Inputs %f %f %f %f", input_x, input_y, input_dx, input_dy);
}

void scaleChanged(float s)
{
	Control::scale = s;
}

extern "C"
{
	//RENDER CALL
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitEngine(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_step(JNIEnv * env, jobject obj);

    //INPUT & SELECTION FUNCTIONS
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_input(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat dx, jfloat dy, jfloat s);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RegisterTap(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RegisterDoubleTap(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SelectStage(JNIEnv * env, jobject obj, jint stage_id);
    JNIEXPORT jintArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetSelections(JNIEnv *env, jobject object);
    JNIEXPORT jint JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetSelected(JNIEnv *env, jobject object);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_ResetSelection(JNIEnv *env, jobject object);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LockMode(JNIEnv *env, jobject object, jboolean state);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetStageVisibility(JNIEnv *env, jobject object, jint idx, jboolean state);

    ///SNAPSHOT MODE
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SnapshotMode(JNIEnv * env, jobject obj, jboolean state);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_CopyBuffer(JNIEnv * env, jobject obj, jbyteArray a);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_TakeSnapshot(JNIEnv * env, jobject obj);


    ///INIT
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitScreenDimen(JNIEnv *env, jobject object, jint x_screen, jint y_screen, jfloat in_density);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitPath(JNIEnv *env, jobject object, jstring java_string);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitShaders(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_ReloadGLData(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LoadShadersSources(JNIEnv * env, jobject obj, jobjectArray stringArray);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitStage(JNIEnv * env, jobject obj);

    ///SPLASH ACTIVITY
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LoadData(JNIEnv *env, jobject object, jstring java_string);
    JNIEXPORT jobjectArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetStageListFromFile(JNIEnv *env, jobject object, jstring string_string);
    JNIEXPORT jobjectArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetEntityListFromFile(JNIEnv *env, jobject object, jstring java_string);
    JNIEXPORT jobjectArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetLightList(JNIEnv *env, jobject object);

    //RENDER COMMAND
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderObj(JNIEnv *env, jobject object, jint obj_idx);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderSubEntity(JNIEnv *env, jobject object, jint obj_idx, jint sub_obj_idx);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderSubMat(JNIEnv *env, jobject object, jint obj_idx, jint sub_obj_idx, jint primitive);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderMat(JNIEnv *env, jobject object,jint mat_idx, jint primitive);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_ChangePrimitive(JNIEnv *env, jobject object, jint primitive);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitCamera(JNIEnv *env, jobject object);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LoadStage(JNIEnv *env, jobject object, jintArray idx_array);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetInsetMode(JNIEnv *env, jobject object, jboolean state);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_MeshMode(JNIEnv *env, jobject object, jboolean state);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_AABBMode(JNIEnv *env, jobject object, jboolean state);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetViewport(JNIEnv *env, jobject object, jint x, jint y);

    //SCENE GL COMMAND
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightPos(JNIEnv *env, jobject object, jint light_idx, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightDir(JNIEnv *env, jobject object, jint light_idx, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightType(JNIEnv *env, jobject object, jint light_idx, jint type);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightEnergy(JNIEnv *env, jobject object, jint light_idx, jint energy);
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_Zoom(JNIEnv *env, jobject object, jfloat distance);
};


//RENDER CALL FUNCTIONS

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitEngine(JNIEnv * env, jobject obj)
{
    InitEngine();
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}


//INPUT & SELECTION FUNCTIONS

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_input(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat dx, jfloat dy, jfloat s)
{
	touchesMoved(x, y, dx, dy);
	scaleChanged(s);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RegisterTap(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
	 Program* prog = ProgramFactory<Program>::CreateProgram(Program::TYPE::ST_MOTION,vector<ProgramArguments> ({x,y}));
	 GusTV::BroadCast(ChannelOffice::CHANNEL::MOTION, *prog);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RegisterDoubleTap(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
	 Program* prog = ProgramFactory<Program>::CreateProgram(Program::TYPE::DT_MOTION,vector<ProgramArguments> ({x,y}));
	 GusTV::BroadCast(ChannelOffice::CHANNEL::MOTION, *prog);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SelectStage(JNIEnv * env, jobject obj, jint stage_id) {
	scene.SetSelected(stage_id);
}

JNIEXPORT jintArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetSelections(JNIEnv *env, jobject object) {

	return CastIntVectorToArray(env,object,scene.GetSelections());
}

JNIEXPORT jint JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetSelected(JNIEnv *env, jobject object) {

	return scene.GetSelectedObject();
}


JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_ResetSelection(JNIEnv *env, jobject object) {
	scene.ResetSelection();
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LockMode(JNIEnv *env, jobject object, jboolean state) {
	scene.LockMode(state);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetStageVisibility(JNIEnv *env, jobject object, jint idx, jboolean state) {
	scene.SetStageVisibility(idx, state);
}


//SNAPSHOT FUNCTIONS

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SnapshotMode(JNIEnv * env, jobject obj, jboolean state) {
	scene.SnapshotMode(state);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_CopyBuffer(JNIEnv * env, jobject obj, jbyteArray a)
{
		env->SetByteArrayRegion(a, 0, xScreen * yScreen * 4, CopyBuffer());
		//ClearData();
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_TakeSnapshot(JNIEnv * env, jobject obj){
	snapshot = true;
	TakeSnapshot();
}




//INIT FUNCTIONS

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitScreenDimen(JNIEnv *env, jobject object, jint x_screen, jint y_screen, jfloat in_density){
	xScreen = x_screen;
	yScreen = y_screen;
	density = in_density;
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitPath(JNIEnv *env, jobject object, jstring java_string){

	string file = CastjStringToString(env,object,java_string);
	InitPath(file);
	LOGE("[GL_CODE][GL2JNI_InitPath] path set to %s",(*filePath).c_str());
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitShaders(JNIEnv * env, jobject obj){
	//Shader::InitShaders();
	Shader::InitShadersFromSources();
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_ReloadGLData(JNIEnv * env, jobject obj){
	//Shader::InitShaders();
	Shader::InitShadersFromSources();
	dataCentral.FillServerMemory();
	renderEngine.InitStage();
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LoadShadersSources(JNIEnv * env, jobject obj, jobjectArray stringArray){
	vector<string> sources = CastStringArrayToVector(env,obj,stringArray);

	for(int i = 0 ; i < sources.size()/4 ; i++) {
		Shader::AddSource(pair<string,string>(sources[i*4+3],sources[i*4+1]), GenIdx(sources[i*4+2]));
	}
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitStage(JNIEnv * env, jobject obj){
	InitStage();
}



//SPASH ACTIVITY

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LoadData(JNIEnv *env, jobject object, jstring java_string) {

	string file = CastjStringToString(env,object,java_string);

	vector<string> fileToLoad({file});

	LoadData(fileToLoad);
}


JNIEXPORT jobjectArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetStageListFromFile(JNIEnv *env, jobject object, jstring java_string){

	string file = CastjStringToString(env,object,java_string);

	vector<string> packed_data = dataCentral.GetStagePackedDataFromFiles(file);

	/*
	LOGE("Stage Packed_data for %s",file.c_str());
	for(auto el : packed_data){
		LOGE("%s",el.c_str());
	}
	 */
    return CastStringVectorToArray(env,object,packed_data);
}


JNIEXPORT jobjectArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetEntityListFromFile(JNIEnv *env, jobject object, jstring java_string){

	string file = CastjStringToString(env,object,java_string);

	vector<string> packed_data = dataCentral.GetEntityPackedDataFromFiles(file);
/*
	LOGE("Entity Packed_data for %s",file.c_str());
	for(auto el : packed_data){
		LOGE("%s",el.c_str());
	}
*/
    return CastStringVectorToArray(env,object,packed_data);
}


JNIEXPORT jobjectArray JNICALL Java_gl_iglou_scenegraph_GL2JNILib_GetLightList(JNIEnv *env, jobject object){


	vector<string> packed_data = dataCentral.GetLightPackedData();

    return CastStringVectorToArray(env,object,packed_data);
}


//RENDER COMMAND


JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderObj(JNIEnv *env, jobject object, jint obj_idx) {

	scene.OneObjMode(obj_idx);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderSubEntity(JNIEnv *env, jobject object, jint obj_idx, jint sub_obj_idx) {

	scene.EntityMode(obj_idx, sub_obj_idx);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderSubMat(JNIEnv *env, jobject object, jint obj_idx, jint sub_obj_idx, jint primitive) {

	scene.MatMode(obj_idx, sub_obj_idx, primitive);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_RenderMat(JNIEnv *env, jobject object,jint mat_idx, jint primitive) {
	scene.MatModePure(mat_idx, primitive);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_ChangePrimitive(JNIEnv *env, jobject object, jint primitive){
	scene.SetMatModePrimitive(primitive);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_InitCamera(JNIEnv *env, jobject object) {

	scene.ResetCamera();
}


JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_LoadStage(JNIEnv *env, jobject object, jintArray idx_array) {

	scene.LoadStage(CastIntArrayToVector(env,object,idx_array));
}


JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetInsetMode(JNIEnv *env, jobject object, jboolean state){

		Renderer::InsetMode = state;
		scene.SkyViewMode(state);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_MeshMode(JNIEnv *env, jobject object, jboolean state){

		scene.MeshMode(state);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_AABBMode(JNIEnv *env, jobject object, jboolean state){

	scene.AABBMode(state);
}

JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetViewport(JNIEnv *env, jobject object, jint x, jint y){
	Renderer::frameView[0] = x;
	Renderer::frameView[1] = y;
}





//SCENE GL COMMAND
    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightPos(JNIEnv *env, jobject object, jint light_idx, jfloat x, jfloat y, jfloat z){
    	scene.GetLight(light_idx)->SetPosition(GusVec3(x,y,z));
    	GusVec3 pos = scene.GetLight(light_idx)->GetPosition();
    	//LOGE("[GL_CODE] Light %d set to %f %f %f",light_idx, pos.x, pos.y, pos.z);
    }

    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightDir(JNIEnv *env, jobject object, jint light_idx, jfloat x, jfloat y, jfloat z){

    }

    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightType(JNIEnv *env, jobject object, jint light_idx, jint type){
    	scene.GetLight(light_idx)->SetType(type);
    }

    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_SetLightEnergy(JNIEnv *env, jobject object, jint light_idx, jint energy){

    }

    JNIEXPORT void JNICALL Java_gl_iglou_scenegraph_GL2JNILib_Zoom(JNIEnv *env, jobject object, jfloat distance){
    	scene.ZoomSkyCamera(distance);
    }
















































