/*
 * Copyright (C) 2007 The Android Open Source Project
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

package gl.iglou.scenegraph;

import android.content.res.AssetManager;

// Wrapper for native library

public class GL2JNILib {

     static {
    	 System.loadLibrary("bullet");
    	 System.loadLibrary("gus");
         System.loadLibrary("scenegraph");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void InitEngine();
     public static native void step();
     
     public static native void 	input(float x, float y, float dx, float dy, float scale);
     public static native void 	RegisterTap(float x, float y);
     public static native void 	RegisterDoubleTap(float x, float y);
     public static native void 	SelectStage(int id);
     public static native int[] GetSelections();
     public static native int   GetSelected();
     public static native void  ResetSelection();
     public static native void 	LockMode(boolean state);
     public static native void 	SetStageVisibility(int id, boolean state);
     
     public static native void SnapshotMode(boolean state);
     public static native void CopyBuffer(byte[] a);
     public static native void TakeSnapshot();
     
     public static native void InitScreenDimen(int x, int y, float density);
     public static native void InitPath(String path);
     public static native void InitShaders();
     public static native void ReloadGLData();
     public static native void LoadShadersSources(String[] names);
     public static native void InitStage();
     
     public static native void LoadData(String file);
     public static native String[] GetStageListFromFile(String file);
     public static native String[] GetEntityListFromFile(String file);
     public static native String[] GetLightList();
     
     public static native void RenderObj(int obj_idx);
     public static native void RenderSubEntity(int obj_id, int obj_sub_idx);
     public static native void RenderSubMat(int obj_id, int obj_sub_idx, int primitive);
     public static native void RenderMat(int mat_id, int primitive);
     public static native void ChangePrimitive(int primitive);
     public static native void InitCamera();
     public static native void LoadStage(int[] idx_array);
     public static native void SetInsetMode(boolean state);
     public static native void MeshMode(boolean state);
     public static native void AABBMode(boolean state);
     public static native void SetViewport(int x, int y);
     
     public static native void SetLightPos(int light_idx, float x, float y , float z);
     public static native void SetLightDir(int light_idx, float x, float y , float z);
     public static native void SetLightType(int light_idx, int type);
     public static native void SetLightEnergy(int light_idx, float e);
     public static native void Zoom(float mag);
     
}


































