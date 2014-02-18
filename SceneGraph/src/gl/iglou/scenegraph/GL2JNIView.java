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

package gl.iglou.scenegraph;
/*
 * Copyright (C) 2008 The Android Open Source Project
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


import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;


/**
 * A simple GLSurfaceView sub-class that demonstrate how to perform
 * OpenGL ES 2.0 rendering into a GL Surface. Note the following important
 * details:
 *
 * - The class must use a custom context factory to enable 2.0 rendering.
 *   See ContextFactory class definition below.
 *
 * - The class must use a custom EGLConfigChooser to be able to select
 *   an EGLConfig that supports 2.0. This is done by providing a config
 *   specification to eglChooseConfig() that has the attribute
 *   EGL10.ELG_RENDERABLE_TYPE containing the EGL_OPENGL_ES2_BIT flag
 *   set. See ConfigChooser class definition below.
 *
 * - The class must select the surface's format, then choose an EGLConfig
 *   that matches it exactly (with regards to red/green/blue/alpha channels
 *   bit depths). Failure to do so would result in an EGL_BAD_MATCH error.
 */
public class GL2JNIView extends GLSurfaceView {
	
    private static String TAG = "GL2JNIView";
    private static final boolean DEBUG = false;
    private static float mPreviousX, mPreviousY;
    private static float mX = 0.f; 
    private static float mY = 0.f;
    private static float mDx = 0.f;
    private static float mDy = 0.f;
    private static float mScale = 1.f;
    private static float factor = 0.5f;
    private static int mWidth,mHeight;
    
    private static Renderer renderer;
   
    //private  EGLContext mEGLContext;
    //private  GL10 mGL;
    //private  Renderer mRenderer;
   
    public static Bitmap mBitmap;
    
    
    private ScaleGestureDetector scaleGestureDetector;
    private GestureDetector gDetector;
    
    
    public GL2JNIView(Context context, AttributeSet attrs) {
        super(context,attrs);          
        renderer = new Renderer();
        renderer.SetFrameview(SceneGraph.getXscreen(), SceneGraph.getYscreen());
        init(false, 16, 8);
        mPreviousX = 0.0f;
        mPreviousY = 0.0f;
        mX = 0.0f;
        mY = 0.0f;
        mDx = 0.0f;
        mDy = 0.0f;
        mScale = 1.0f;
        
        
        scaleGestureDetector = new ScaleGestureDetector(context, new ScaleListener());   
        gDetector = new GestureDetector(context, new MyGestureListener());
        
        Log.v(TAG,"GL2JNIView INIT!!");
    }

    public GL2JNIView(Context context, AttributeSet attrs, boolean translucent, int depth, int stencil) {
        super(context,attrs);
        init(translucent, depth, stencil);
    }


	private void init(boolean translucent, int depth, int stencil) {
    	
        /* By default, GLSurfaceView() creates a RGB_565 opaque surface.
         * If we want a translucent one, we should change the surface's
         * format here, using PixelFormat.TRANSLUCENT for GL Surfaces
         * is interpreted as any 32-bit surface with alpha by SurfaceFlinger.
         */
        if (translucent) {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }

        /* Setup the context factory for 2.0 rendering.
         * See ContextFactory class definition below
         */
        setEGLContextFactory(new ContextFactory());

        /* We need to choose an EGLConfig that matches the format of
         * our surface exactly. This is going to be done in our
         * custom config chooser. See ConfigChooser class definition
         * below.
         */
        setEGLConfigChooser( translucent ?
                             new ConfigChooser(8, 8, 8, 8, depth, stencil) :
                             new ConfigChooser(5, 6, 5, 0, depth, stencil) );

        /* Set the renderer responsible for frame rendering */
        
        setRenderer(renderer);
    }

    private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            Log.w(TAG, "creating OpenGL ES 2.0 context");
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext mEGLContext = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
           // mGL = (GL10) mEGLContext.getGL();
            checkEglError("After eglCreateContext", egl);
            return mEGLContext;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }

    private static void checkEglError(String prompt, EGL10 egl) {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
            Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }

    private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {

        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        /* This EGL config specification is used to specify 2.0 rendering.
         * We use a minimum size of 4 bits for red/green/blue, but will
         * perform actual matching in chooseConfig() below.
         */
        private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
            EGL10.EGL_RED_SIZE, 4,
            EGL10.EGL_GREEN_SIZE, 4,
            EGL10.EGL_BLUE_SIZE, 4,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL10.EGL_NONE
        };

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

            /* Get the number of minimally matching EGL configurations
             */
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

            int numConfigs = num_config[0];

            if (numConfigs <= 0) {
                throw new IllegalArgumentException("No configs match configSpec");
            }

            /* Allocate then read the array of minimally matching EGL configs
             */
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

            if (DEBUG) {
                 printConfigs(egl, display, configs);
            }
            /* Now return the "best" one
             */
            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs) {
            for(EGLConfig config : configs) {
                int d = findConfigAttrib(egl, display, config,
                        EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config,
                        EGL10.EGL_STENCIL_SIZE, 0);

                // We need at least mDepthSize and mStencilSize bits
                if (d < mDepthSize || s < mStencilSize)
                    continue;

                // We want an *exact* match for red/green/blue/alpha
                int r = findConfigAttrib(egl, display, config,
                        EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config,
                            EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config,
                            EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config,
                        EGL10.EGL_ALPHA_SIZE, 0);

                if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) {

            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }

        private void printConfigs(EGL10 egl, EGLDisplay display,
            EGLConfig[] configs) {
            int numConfigs = configs.length;
            Log.w(TAG, String.format("%d configurations", numConfigs));
            for (int i = 0; i < numConfigs; i++) {
                Log.w(TAG, String.format("Configuration %d:\n", i));
                printConfig(egl, display, configs[i]);
            }
        }

        private void printConfig(EGL10 egl, EGLDisplay display,
                EGLConfig config) {
            int[] attributes = {
                    EGL10.EGL_BUFFER_SIZE,
                    EGL10.EGL_ALPHA_SIZE,
                    EGL10.EGL_BLUE_SIZE,
                    EGL10.EGL_GREEN_SIZE,
                    EGL10.EGL_RED_SIZE,
                    EGL10.EGL_DEPTH_SIZE,
                    EGL10.EGL_STENCIL_SIZE,
                    EGL10.EGL_CONFIG_CAVEAT,
                    EGL10.EGL_CONFIG_ID,
                    EGL10.EGL_LEVEL,
                    EGL10.EGL_MAX_PBUFFER_HEIGHT,
                    EGL10.EGL_MAX_PBUFFER_PIXELS,
                    EGL10.EGL_MAX_PBUFFER_WIDTH,
                    EGL10.EGL_NATIVE_RENDERABLE,
                    EGL10.EGL_NATIVE_VISUAL_ID,
                    EGL10.EGL_NATIVE_VISUAL_TYPE,
                    0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                    EGL10.EGL_SAMPLES,
                    EGL10.EGL_SAMPLE_BUFFERS,
                    EGL10.EGL_SURFACE_TYPE,
                    EGL10.EGL_TRANSPARENT_TYPE,
                    EGL10.EGL_TRANSPARENT_RED_VALUE,
                    EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                    EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                    0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                    0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                    0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                    0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                    EGL10.EGL_LUMINANCE_SIZE,
                    EGL10.EGL_ALPHA_MASK_SIZE,
                    EGL10.EGL_COLOR_BUFFER_TYPE,
                    EGL10.EGL_RENDERABLE_TYPE,
                    0x3042 // EGL10.EGL_CONFORMANT
            };
            String[] names = {
                    "EGL_BUFFER_SIZE",
                    "EGL_ALPHA_SIZE",
                    "EGL_BLUE_SIZE",
                    "EGL_GREEN_SIZE",
                    "EGL_RED_SIZE",
                    "EGL_DEPTH_SIZE",
                    "EGL_STENCIL_SIZE",
                    "EGL_CONFIG_CAVEAT",
                    "EGL_CONFIG_ID",
                    "EGL_LEVEL",
                    "EGL_MAX_PBUFFER_HEIGHT",
                    "EGL_MAX_PBUFFER_PIXELS",
                    "EGL_MAX_PBUFFER_WIDTH",
                    "EGL_NATIVE_RENDERABLE",
                    "EGL_NATIVE_VISUAL_ID",
                    "EGL_NATIVE_VISUAL_TYPE",
                    "EGL_PRESERVED_RESOURCES",
                    "EGL_SAMPLES",
                    "EGL_SAMPLE_BUFFERS",
                    "EGL_SURFACE_TYPE",
                    "EGL_TRANSPARENT_TYPE",
                    "EGL_TRANSPARENT_RED_VALUE",
                    "EGL_TRANSPARENT_GREEN_VALUE",
                    "EGL_TRANSPARENT_BLUE_VALUE",
                    "EGL_BIND_TO_TEXTURE_RGB",
                    "EGL_BIND_TO_TEXTURE_RGBA",
                    "EGL_MIN_SWAP_INTERVAL",
                    "EGL_MAX_SWAP_INTERVAL",
                    "EGL_LUMINANCE_SIZE",
                    "EGL_ALPHA_MASK_SIZE",
                    "EGL_COLOR_BUFFER_TYPE",
                    "EGL_RENDERABLE_TYPE",
                    "EGL_CONFORMANT"
            };
            int[] value = new int[1];
            for (int i = 0; i < attributes.length; i++) {
                int attribute = attributes[i];
                String name = names[i];
                if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
                    Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
                } else {
                    // Log.w(TAG, String.format("  %s: failed\n", name));
                    while (egl.eglGetError() != EGL10.EGL_SUCCESS);
                }
            }
        }

        // Subclasses can adjust these values:
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
    }

    
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // RENDERER CLASS
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    
    public static class Renderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
        	GL2JNILib.input(mX,mY,mDx, mDy, mScale);
            GL2JNILib.step();
            resetScale();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
        	mHeight = height;
        	mWidth = width;
           // GL2JNILib.init(width, height);
        	
        	
        	
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // Do nothing.
        	Log.v(TAG,"GL2JNIView Renderer onSurface Created!!");
        	GL2JNILib.ReloadGLData();
        }
        
        public void SetFrameview(int x, int y) {
        	GL2JNILib.SetViewport(x,y);
        }
    }
    
    //////////////////////////////////////////////////////////
    // TOMATO IMPLEMENTATION BEGINS HERE!! 
    //////////////////////////////////////////////////////////
    public static void resetScale()
    {
    	mScale = 1.0f;
    }
    
    public static void resetControl()
    {
    	mDx = 0.0f;
    	mDy = 0.0f;
    	mX = 0.0f;
    	mY = 0.0f;
    	mPreviousX = 0.0f;
        mPreviousY = 0.0f;
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // MotionEvent reports input details from the touch screen
        // and other input controls. In this case, you are only
        // interested in events where the touch position changed.
    	
    	gDetector.onTouchEvent(e);
       
             if (e.getPointerCount() > 1) 
             {
            	 scaleGestureDetector.onTouchEvent(e);
             }else
             {
            	 mX = e.getX();
                 mY = e.getY();
 
		        switch (e.getActionMasked()) 
		        {
		        	case MotionEvent.ACTION_UP :
		        		Log.e("GL2JNIVIEW", "ACTION UP!!");
		        		resetControl();
		        	break;
		        	case MotionEvent.ACTION_DOWN :
		        		mPreviousX = mX;
		        	    mPreviousY = mY;
		        		Log.e("GL2JNIVIEW", "ACTION DOWN!!");
		        	break;
		        	
		        	case MotionEvent.ACTION_POINTER_DOWN :
		        		Log.e("GL2JNIVIEW", "MULTITOUCH DETECTED!!");
		        		
		        	break;
		        	
		        	case MotionEvent.ACTION_MOVE :
		        		if ( mPreviousX  > 200.f && mPreviousX  < 1720.f)
		        		{
		                 mDx = (mPreviousX - mX) * factor;
		                 mDy = (mPreviousY - mY) * factor;
		        		}
		                 //Log.e("GL2JNIVIEW", "ACTION MOVE!!");
		           break;		
		        }
		        mPreviousX = mX;
		        mPreviousY = mY;
             }
       
        return true;
    }
    
    
  private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
  @Override
  public boolean onScale(ScaleGestureDetector detector) {
    mScale *= detector.getScaleFactor();

    // Don't let the object get too small or too large.
    mScale = Math.max(0.1f, Math.min(mScale, 5.0f));

    Log.e("onScale","Scale is " + Float.toString(mScale));
    return true;
   }
 }

  private class MyGestureListener extends GestureDetector.SimpleOnGestureListener {

	@Override
	public boolean onDoubleTap(MotionEvent e) {
		// TODO Auto-generated method stub
		Log.e("MOTION","DT DETECTED!");
		GL2JNILib.RegisterDoubleTap(e.getX(),e.getY());
	//        [self LaunchRay:loc];
		return true;
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e) {
		// TODO Auto-generated method stub
		Log.e("MOTION","ST DETECTED!");
		GL2JNILib.RegisterTap(e.getX(),e.getY());
		return true;
	}	 
  }
  
  public void oYo() {
	  Log.v(TAG,"oYo!!");
  }
  
  public void getBitmapJNI() {
		
	  byte[] b_array = new byte[SceneGraph.getXscreen()*SceneGraph.getYscreen()*4];
	  	 
	  GL2JNILib.CopyBuffer(b_array);
     	 
	  ByteBuffer buffer = ByteBuffer.allocate(SceneGraph.getXscreen()*SceneGraph.getYscreen()*4);
	  
	  buffer.put(b_array);
     
	  buffer.rewind();
       /*
       // vertical mirror image
       int[] swapArray = new int[mWidth];
       int[] array = buffer.array();
       for (int i = 0; i < mHeight / 2; i++) {
               System.arraycopy(array, i * mWidth, swapArray, 0, mWidth);
               System.arraycopy(array, (mHeight - i - 1) * mWidth, array, i * mWidth, mWidth);
               System.arraycopy(swapArray, 0, array, (mHeight - i - 1) * mWidth, mWidth);
       }
       // copy to Bitmap
       */
      mBitmap = Bitmap.createBitmap(SceneGraph.getXscreen(), SceneGraph.getYscreen(), Bitmap.Config.ARGB_8888);
      
      mBitmap.copyPixelsFromBuffer(buffer);
      
      GL2JNILib.SnapshotMode(false);
      
      setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
  }
}

  
  
  
  
  
  
  
  
  