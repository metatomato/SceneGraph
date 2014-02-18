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


import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.drawable.Drawable;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.v4.widget.DrawerLayout;
import android.text.format.Time;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;


interface MatModeInterface {
    void EnableMatMode(boolean state);
    int GetPrimitive();
} 

interface SelectionInterface {
    void UpdateSelection();
    void EnableLockButton(boolean state);
} 

public class GL2JNIActivity extends Activity implements MatModeInterface,SelectionInterface{
	final private String TAG = "SCENE";
	static private boolean dataRefreshing = false;
	static private int 		matModePrimitive = 0;
	
	private View decorView; 
	private View snapShot;
	private ProgressBar snapshotIndicator;
	private FrameLayout snapshotContainer;
	private SceneDrawer mDrawerLayout;
	private SceneOutlinerFragment outlinerFrag;
	private SceneGLConfiguratorFragment GLConfFrag;
    private GL2JNIView GLView;
    private ImageView photo;
    private ImageView arrowL;
    private ImageView arrowR;
    private Drawable arrow_l;
    private Drawable arrow_r ;
    private ImageButton targetButton;
    private ImageButton meshButton;
    private ImageButton AABBButton;
    private TextView selection;
    private ArrowClickListener arrowListener;
    private int drawerStartId,drawerEndId;
    private View matModeView;
    private RelativeLayout mainLayout;
    private int currentSceneId = 0;
    private Toast snapshotToast;
    private View buttonGroup;
    
    private boolean isLocked = false;
    private boolean isMeshed = false;
    private boolean isAABBed = false;

    @Override
    public void EnableMatMode(boolean state) {
    	if(state){
    		matModeView.setVisibility(View.VISIBLE);
    		buttonGroup.setVisibility(View.INVISIBLE);
    	}
    	else{
    		matModeView.setVisibility(View.INVISIBLE);
    		//buttonGroup.setVisibility(View.VISIBLE);
    	}
    }
    @Override
    public int GetPrimitive(){
    	return matModePrimitive;
    }
    
    public static void SetMatModePrimitive(int p) {
    	matModePrimitive = p;
    }
    
    public static int GetMatModePrimitive(){
    	return matModePrimitive;
    }
    
    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
    
        snapshotToast = Toast.makeText(this, "", Toast.LENGTH_LONG);
        
        LayoutInflater inflater = getLayoutInflater();
        
         arrow_l  = getResources().getDrawable(R.drawable.scene_arrow_l);
    	 arrow_r = getResources().getDrawable(R.drawable.scene_arrow_r);
    	 arrowListener = new ArrowClickListener();
    	 
        currentSceneId = getIntent().getIntExtra("SCENE_ID",0);
        SharedData.TYPE current_mode = SharedData.GetMode();
        
        setContentView(R.layout.scene_activity);
        
        mainLayout = (RelativeLayout) findViewById(R.id.content_frame);
        buttonGroup = findViewById(R.id.scene_button_frame);
        snapshotContainer = (FrameLayout) findViewById(R.id.scene_snapshot_container);
        //drawerStartId = getIdAssignedByR(this,"outliner_container");
        drawerStartId = R.id.outliner_container;
        drawerEndId = getIdAssignedByR(this,"gl_configurator_container");
        
        GLView = (GL2JNIView) findViewById(R.id.content_view);
        
        arrowL = (ImageView) 	findViewById(R.id.arrow_l); 
        arrowR = (ImageView)  	findViewById(R.id.arrow_r);
        arrowL.setOnClickListener(arrowListener);
        arrowR.setOnClickListener(arrowListener);
        
        targetButton = (ImageButton) findViewById(R.id.scene_target);
        meshButton = (ImageButton) findViewById(R.id.scene_mesh_mode);
        AABBButton = (ImageButton) findViewById(R.id.scene_AABB_mode);
        
        selection = (TextView) findViewById(R.id.scene_selection);
        UpdateSelection();
        
        mDrawerLayout = (SceneDrawer) findViewById(R.id.drawer_layout);
        
        mDrawerLayout.setDrawerListener(new MyDrawerListener());
        mDrawerLayout.SetDrawerId(drawerStartId, drawerEndId);
        
        mDrawerLayout.setScrimColor(0x00ffffff);
        
        decorView = getWindow().getDecorView();       
        
        outlinerFrag = new SceneOutlinerFragment();
        GLConfFrag = new SceneGLConfiguratorFragment();
        
        matModeView = inflater.inflate(R.layout.scene_mat_mode,null);
        mainLayout.addView(matModeView);
        if(SharedData.TYPE.MAT == current_mode){
        	//matModeView.setVisibility(View.VISIBLE);
        	EnableMatMode(true);
        }else {
        	EnableMatMode(false);
        	//matModeView.setVisibility(View.INVISIBLE);
        }
        
        snapShot = inflater.inflate(R.layout.scene_snapshot_frame, null);
        snapshotContainer.addView(snapShot);
        photo  = (ImageView) snapShot.findViewById(R.id.snapshot);
        snapshotIndicator = (ProgressBar) snapShot.findViewById(R.id.scene_snapshot_progress_bar);
        snapshotContainer.setVisibility(View.INVISIBLE);
        snapshotIndicator.setVisibility(View.INVISIBLE);
        
        FragmentManager fm = getFragmentManager();
        FragmentTransaction ft = fm.beginTransaction();
        ft.add(R.id.outliner_container,outlinerFrag);
        ft.commit();  
        
        FragmentTransaction ft0 = fm.beginTransaction();
        ft0.add(R.id.gl_configurator_container,GLConfFrag);
        ft0.commit(); 
        Log.v(TAG,"Activity ON_CREATE CALL");
        
    }

    
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
            super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            decorView.setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);}
    }
    
    /*
    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }
*/
    
    
    public void ScreenShot(View v) {
    	Log.v(TAG,"Souriez!!!");
    	//mView.Shoot();
    	//GL2JNILib.TakeSnapshot();
    	//while(!GL2JNILib.IsSnapshotReady()) { Log.v(TAG,"Waiting for snapshot!!"); }
    	    	
    	GLView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    	GL2JNILib.SnapshotMode(true);
    	GL2JNILib.TakeSnapshot();
    	GLView.requestRender();
    	
    	
    	Handler handler = new Handler(); 
        handler.postDelayed(new Runnable() { 
             public void run() { 
            	GLView.getBitmapJNI();
             	photo.setImageBitmap(GLView.mBitmap); 
             	snapshotContainer.setVisibility(View.VISIBLE);
             	
             } 
        }, 100); 	
    } 	
    
    
    public void SaveSnapshot(View v) {
    	
    	snapshotIndicator.setVisibility(View.VISIBLE);
    	 
    	new Thread() {
            public void run() {
                try {
                	String iconsStoragePath = Environment.getExternalStorageDirectory() + "/SCENEGRAPH/Snapshots/";
            		File sdIconStorageDir = new File(iconsStoragePath);
            		Time now = new Time();
            		now.setToNow();
            		String pic_name = SceneData.GetSceneName() + "_" + now.format("%Y_%m_%d_%H_%M_%S");
            		
            		//create storage directories, if they don't exist
            		sdIconStorageDir.mkdirs();
            		String path = sdIconStorageDir.toString() + "/" + pic_name + ".png";
            		
            		try {
            			
            			FileOutputStream fileOutputStream = new FileOutputStream(path);

            			BufferedOutputStream bos = new BufferedOutputStream(fileOutputStream);

            			//choose another format if PNG doesn't suit you
            			GLView.mBitmap.compress(CompressFormat.PNG, 100, bos);

            			bos.flush();
            			bos.close();

            			snapshotToast.setText(path);
                		snapshotToast.show();
                		
                		runOnUiThread(new Runnable() {
               	         @Override
               	             public void run() {
               	        	 	HideSnapshot();        
               	         }
               	        });
                		
                		
            		} catch (FileNotFoundException e) {
            			Log.w("TAG", "Error saving image file: " + e.getMessage());
            		} catch (IOException e) {
            			Log.w("TAG", "Error saving image file: " + e.getMessage());    		
            		}
                } catch (Exception e) {
                	Log.w("TAG", "Error saving image file: " + e.getMessage());
                }       
                
        		
            }
        }.start();
	}

    
    
    public void HideSnapshot(){
    	snapshotIndicator.setVisibility(View.INVISIBLE);
		snapshotContainer.setVisibility(View.INVISIBLE);
    }
    
    
    public void DiscardSnapshot(View v) {
    	snapshotContainer.setVisibility(View.INVISIBLE);
    }


    public void UpdateSelection() {
    	String selection_name = "-";
    	Log.v(TAG,"UpdateSelection "  + String.valueOf(GL2JNILib.GetSelected()));
    	
    	int id = GL2JNILib.GetSelected();
    	
    	if(-1 != id){
    		selection_name = SharedData.GetStage(id).GetName();
    	}
    	selection.setText(selection_name);
    }
    
    
    public void EnableLockButton(boolean state){
    		targetButton.setEnabled(state);   
    }
    
    
    public void SwitchCameraMode(View v) {
    	if(!isLocked){
    		isLocked = true;
    	}else {
    		isLocked = false;
    	}
    	targetButton.setSelected(isLocked);
    	GL2JNILib.LockMode(isLocked);
    }
    
    public void SwitchMeshMode(View v) {
    	if(!isMeshed){
    		isMeshed = true;
    	}else {
    		isMeshed = false;
    	}
    	meshButton.setSelected(isMeshed);
    	GL2JNILib.MeshMode(isMeshed);
    }
    
    public void SwitchAABBMode(View v) {
    	if(!isAABBed){
    		isAABBed = true;
    	}else {
    		isAABBed = false;
    	}
    	AABBButton.setSelected(isAABBed);
    	GL2JNILib.AABBMode(isAABBed);
    }
    
    
    public void ZoomOut(View v) {
    	Log.v(TAG, "ZOOM OUT!");
    	GL2JNILib.Zoom(50.f);
    	SceneGLConfiguratorFragment.Y_cam += 50.f;
    }
    
    
    public void ZoomIn(View v) {
    	Log.v(TAG, "ZOOM IN!");
    	GL2JNILib.Zoom(-50.f);
    	SceneGLConfiguratorFragment.Y_cam -= 50.f;
    }
    
    
    private class MyDrawerListener extends DrawerLayout.SimpleDrawerListener{
		@Override
		public void onDrawerClosed(View drawerView) {
			// TODO Auto-generated method stub
			Log.e("libgl2jni", "drawer is Closed!");
			
			if(SharedData.TYPE.MAT != SharedData.GetMode()){
				buttonGroup.setVisibility(View.VISIBLE);
			}
			
			if(drawerView.getId() == drawerEndId) {
				mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_UNLOCKED,Gravity.END);
				mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_UNLOCKED,Gravity.START);
				arrowR.setImageDrawable(arrow_r);
				arrowL.setVisibility(View.VISIBLE);
				SetInsetMode(false);
			}
			if(drawerView.getId() == drawerStartId) {				
				arrowL.setImageDrawable(arrow_l);
				arrowR.setVisibility(View.VISIBLE);
				dataRefreshing = false;
			}
		}

		@Override
		public void onDrawerOpened(View drawerView) {
			// TODO Auto-generated method stub
			Log.e("libgl2jni", "drawer is Open!");
			
			if(drawerView.getId() == drawerEndId) {
				mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_OPEN,Gravity.END);
				mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED,Gravity.START);
				arrowR.setImageDrawable(arrow_l);
				 mDrawerLayout.requestDisallowInterceptTouchEvent(true);
				 arrowL.setVisibility(View.INVISIBLE);
				SetInsetMode(true);
			}
			if(drawerView.getId() == drawerStartId) {				
				arrowL.setImageDrawable(arrow_r);
				arrowR.setVisibility(View.INVISIBLE);
			}
		}
		
		@Override
    	public void onDrawerSlide (View drawerView, float slideOffset) {
    		//Log.v(TAG," Drawer is sliding %f" + String.valueOf(slideOffset));
			float width = drawerView.getWidth();
			float X = drawerView.getX();
			
			buttonGroup.setVisibility(View.INVISIBLE);
			
			if(drawerView.getId() == drawerStartId) {
				arrowL.setX(drawerView.getX() + 20.f + width);
				arrowR.setVisibility(View.INVISIBLE);
				if(!dataRefreshing) {
					SceneData.SyncSelection();
					outlinerFrag.RefreshData();
					dataRefreshing = true;
				}
			}
			if(drawerView.getId() == drawerEndId) {
				arrowR.setX(drawerView.getX() - 20.f - arrowR.getWidth());
				arrowL.setVisibility(View.INVISIBLE);
				if(X > 128.f){
					SetInsetMode(false);
				}
			}
			
    	}
    	
    }
    
    void SetInsetMode(boolean state) {
    	if(!state) {
    		GL2JNILib.SetInsetMode(false);
    		GLConfFrag.GetMap().setImageResource(R.drawable.iglou_splash_2k);
    	}
    	if(state) {
    		GL2JNILib.SetInsetMode(true);
    		GLConfFrag.GetMap().setImageResource(R.drawable.transparent_frame);
    	}
    }
    
    
    
    public class ArrowClickListener implements View.OnClickListener {

		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			Log.v(TAG,"ArrowButton Responds!!");
		}
    	
    }
    
    public int getIdAssignedByR(Context pContext, String pIdString)
    {
        // Get the Context's Resources and Package Name
        Resources resources = pContext.getResources();
        String packageName  = pContext.getPackageName();

        // Determine the result and return it
        int result = resources.getIdentifier(pIdString, "id", packageName);
        return result;
    }
    
    public float ConvertDpToPx(float dp){
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, this.getResources().getDisplayMetrics());
    }

    public void SetMatCube(View v) {
    	matModePrimitive = 0;
    	GL2JNILib.ChangePrimitive(0);
    }
    public void SetMatSphere(View v) {
    	matModePrimitive = 1;
    	GL2JNILib.ChangePrimitive(1);
    }
    public void SetMatCylinder(View v) {
    	matModePrimitive = 2;
    	GL2JNILib.ChangePrimitive(2);
    }
    public void SetMatSuzanne(View v) {
    	matModePrimitive = 3;
    	GL2JNILib.ChangePrimitive(3);
    }
   
}


























