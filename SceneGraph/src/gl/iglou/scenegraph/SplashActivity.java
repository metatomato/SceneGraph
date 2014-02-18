package gl.iglou.scenegraph;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;


 
public class SplashActivity extends Activity {
	private static final String TAG = "SPLASH_ACTIVITY";  
	
	private static int SPLASH_TIME_OUT = 2000;
	private static String text = "";
	
    private ArrayList<String> files; 
    private String resPath = "";
    private String APP_PATH = "";
    
    private View decorView;
    private static GL2JNIView.Renderer	glRenderer;
    private static GLOffScreenView offView;
    private ImageView   photo;
    private PrefetchData prefetchData;
    private TextView	textView;
    
    private AssetManager mgr;
    private ArrayList<String> assetList;
    private ArrayList<String> shaderSources;
    
    private HashMap< String, String[] > sceneData;
    private HashMap< String, String[] >  entityData;
 
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.splash_layout);
        
        mgr = getResources().getAssets();
        
        files = new ArrayList<String>();
        sceneData  = new HashMap< String, String[] >();
        entityData = new HashMap< String,String[] >();
        
        decorView  = getWindow().getDecorView();
        //photo = (ImageView)findViewById(R.id.snapshot);
        
       // glView.setVisibility(View.INVISIBLE);
        
        SharedData.InitInstance(this);
       
        initFileSystem();
        
        prefetchData = new PrefetchData();
        prefetchData.execute();
        
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                // This method will be executed once the timer is over
                // Start your app main activity
            }
        }, SPLASH_TIME_OUT);   
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
   
    public void NextActivity() {
    	Intent i = new Intent(SplashActivity.this, LibraryActivity.class);
        startActivity(i);
       // close this activity
       finish();
    }
    
    public void Snapshot(View v) {
    	Log.v("TOMATO","Souriez!!!");
    	//mView.Shoot();
    	GL2JNILib.TakeSnapshot();
    	GL2JNILib.ReloadGLData();
    }
    
    public void Show(View v) {
    	Log.v("TOMATO","C'est affiché!!!");
    	photo.setImageBitmap(offView.getBitmap());
    }
    
    public void Next(View v){
    	Intent i = new Intent(SplashActivity.this, LibraryActivity.class);
        startActivity(i);
       // close this activity
       finish();
    }
 
    public boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            return true;
        }
        return false;
    }   
    
    
    public void InitEnginePath() {
    	APP_PATH = Environment.getExternalStorageDirectory() + "/SCENEGRAPH/";
    	resPath = Environment.getExternalStorageDirectory() + "/SCENEGRAPH/Res/";
    	GL2JNILib.InitPath(resPath);
    }
    
    
    public void checkFileSystem(String[] filenames) {
    	 //CHECK EITHER APP RESOURCE EXIST, NEITHER CREATE IT
    	for(String s : filenames){
	        final String PATH = Environment.getExternalStorageDirectory() + "/" + s +"/";
	
	        
	       // onScreenText.setText("checking path " + PATH);
	        
	        if(!(new File(PATH)).exists()){
	        	new File(PATH).mkdirs();
	        //	onScreenText.setText("creating path " + PATH);
	        }
    	}
    }
    
    
    public void InitSharedData(){
    	File PATH = new File(Environment.getExternalStorageDirectory() + "/SCENEGRAPH/Res/models/");
        String[] list = PATH.list();
        for(String s : list){
        	if(s.endsWith(".obj")){
        		String temp_s = s.substring(0, s.length()-4);
        		//SharedData.GetAppData().AddObj(temp_s);
        		files.add(s);
        		Log.e(TAG,"Obj added : " + temp_s);
        	}
        	if(s.endsWith(".mtl")){
        		//SharedData.GetAppData().AddObj(s.substring(0, s.length()-4));
        		Log.e(TAG,"Mat added : " + s.substring(0, s.length()-4));
        	}
        }
    }
    
    
    public void initFileSystem(){
    	InitEnginePath();
    	CheckAssetsContent();
    	String[] dir_list = {"SCENEGRAPH","SCENEGRAPH/Res/maps","SCENEGRAPH/Res/models","SCENEGRAPH/Res/Shaders","SCENEGRAPH/Res/textures","SCENEGRAPH/Res/banana"};
    	checkFileSystem(dir_list);
    	InitSharedData();
    }
    
    
    
    public void CheckAssetsContent() {
    	assetList = new ArrayList<String>();
    	shaderSources = new ArrayList<String>();
    	    	
    	copyAssetsFolder("Res");
            	
    	try{
    		String[] list = mgr.list("Shaders");
    		for(String s : list) {
    			assetList.add(s);
    		}
    	}
    	catch(IOException e){
    		Log.v(TAG,"AsetManager Throw an exception!!");
    	}
  
    	for(String s :assetList) {
    		try{
    		InputStream is = mgr.open("Shaders/" + s);
    		String content = ConvertStreamToString(is);
    		shaderSources.add(s);
    		shaderSources.add(content);
    		//Log.v(TAG, s + " contains " + content);
    		}
    		catch(IOException e){
    			Log.v(TAG,"AsetManager Throw an exception for stream!!");
    		}
    		//Log.v(TAG,"Found Asset " + s);
    	}
    }
    
    
    private boolean copyAssetsFolder(String asset_path) {
    	
    	String[] files = null;
    	try{
    		 files = mgr.list(asset_path);
    	} catch(IOException e) {
            Log.e(TAG, asset_path + " do not exist!", e);
            return false;
        } 
    	
    	for(String sub_files : files){
    		Log.v(TAG, "FOUND IN " + asset_path + " : " + sub_files);
    	}
    	
    	for(String sub_files : files){
    		String concat_file = asset_path + "/" + sub_files;
    		if(!copyAssetsFile(concat_file)){
    			Log.v(TAG,"CREATE NEW DIR " + APP_PATH + concat_file);
    			File new_directory = new File(APP_PATH + concat_file);
    			new_directory.mkdirs();
    			copyAssetsFolder(concat_file);
    		}
    	}
        return true;
    }
    
    
    private boolean copyAssetsFile(String asset_path) {   
    	Log.v(TAG, "COPY FILE " + asset_path + " TO SD CARD");
    	InputStream in = null;
        OutputStream out = null;
        
        try {
          in = mgr.open(asset_path);
          Log.v(TAG, "File to SD " +  APP_PATH + asset_path);
          
          out = new FileOutputStream(APP_PATH + asset_path);
          
          copyFile(in, out);
          in.close();
          in = null;
          out.flush();
          out.close();
          out = null;
          
        } catch(IOException e) {
        	Log.e(TAG, asset_path + " do not exist or is a folder!", e);
            return false;
        }       
    	return true;
    }
 
    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
          out.write(buffer, 0, read);
        }
    }
    
    
    public void initMedia(String file) {
    		Log.v(TAG, "Loading " + file);
    		
    		GL2JNILib.LoadData(file);
    		
    		if(!file.contentEquals("shape.obj")){
    			entityData.put(file,GL2JNILib.GetEntityListFromFile(file));
    			sceneData.put(file, GL2JNILib.GetStageListFromFile(file));
    		}
        	/*
        	for(String s : sceneData) {
        		Log.v(TAG, "scene : sate/id/size/{id}" + s);
        	}
        	for(String s : entityData) {
        		Log.v(TAG, "entity : obj/id/mesh/id/mat/id " + s);
        	}
        	*/
    }
    
    
    public void ParseData() {
    	ParseSceneEntities();
    	ParseEntities();
    	ParseLight();
    	SharedData.PostInit();  		
    }

    public void ParseSceneEntities() {
    	Log.v(TAG,"Parsing Data...");
    	
    	int scene_id = 0;
    	
    	for(Map.Entry<String,String[]> pair : sceneData.entrySet()){
    		ArrayList<Integer> stage_id = new ArrayList<Integer>();
    		
    		String[] temp_array = (String[]) pair.getValue();
    		int new_stage = 0;
        	int id = 0;
        	int group_lenght = 0;
    		
    		for(int i  = 0 ; i < temp_array.length ; i++) {    			
	    		if(i == new_stage) {
	    			String stage_name = temp_array[i];
	    			id = Integer.parseInt(temp_array[i+1]);
	    			stage_id.add(id);
	    			group_lenght = Integer.parseInt(temp_array[i+2]);
	    			
	    			ArrayList<Integer> list = new ArrayList<Integer>();
	    			
	    			for(int j = new_stage + 3 ; j < new_stage + 3 + group_lenght ; j++){
	    				list.add(Integer.parseInt(temp_array[j]));
	    			}
	    			
	    			SharedData.GetStages().put(id,new SharedData.SceneEntity(stage_name, id, list, scene_id));	    			    		
	    			//Log.v(TAG,"Stage Added to SharedData  " + stage_name + " " + String.valueOf(id) + " with parent " + scene_id);
	    			//Log.v(TAG,"Stage Added to SharedData  " + SharedData.GetStages().get(id).GetName()
	    			//		+ " " + String.valueOf(id) + " with parent " + SharedData.GetStages().get(id).GetParent());
	    			for(Integer in : list){
	    				//Log.v(TAG,String.valueOf(in));
	    			}
	    			new_stage = i+3+group_lenght;
	    		}	    		
    		}
    		String scene_name = (String)pair.getKey();
    		SharedData.GetScenes().put(scene_id,new SharedData.SceneEntity(scene_name.substring(0, scene_name.length()-4), scene_id, stage_id,scene_id));
    		//Log.v(TAG,"Scene Added to SharedData  " + scene_name.substring(0, scene_name.length()-4) + " " + String.valueOf(scene_id) );
			for(Integer in : stage_id){
				//Log.v(TAG,String.valueOf(in));
			}
    		scene_id++;
    	}
    }
    	
    
    public void ParseEntities() {
    	
    	int obj_id,mesh_id,mat_id,stage_id,tri_count;
    	obj_id = mesh_id = mat_id = stage_id = tri_count = 0;
    	String obj_name, mesh_name, mat_name, stage_name;
    	obj_name = mesh_name = mat_name = stage_name = "";
    	
    	for(Map.Entry<String,String[]> pair : entityData.entrySet()) {
    		String[] temp_array = (String[]) pair.getValue();
    		for(int i = 0 ; i < temp_array.length/9 ; i++) {
    			obj_name  = temp_array[i*9];
    			mesh_name = temp_array[i*9+2];
    			mat_name  = temp_array[i*9+4];
    			stage_name  = temp_array[i*9+6];
    			
    			obj_id   = Integer.parseInt(temp_array[i*9+1]);
    			mesh_id  = Integer.parseInt(temp_array[i*9+3]);
    			mat_id   = Integer.parseInt(temp_array[i*9+5]);
    			stage_id = Integer.parseInt(temp_array[i*9+7]);
    			tri_count = Integer.parseInt(temp_array[i*9+8]);
    		
	    		SharedData.GetEntities().put(obj_id,new SharedData.Entity(obj_id, mesh_id, mat_id, stage_id,tri_count));
	    		SharedData.GetObj().put(obj_id,obj_name);
	    		SharedData.GetMesh().put(mesh_id,mesh_name);
	    		SharedData.GetMat().put(mat_id,mat_name);
    		}
    	}
    }
    
    
    public void ParseLight() {
    	ArrayList<String> list = new ArrayList<String>(Arrays.asList(GL2JNILib.GetLightList()));
    	
    	for(int i = 0 ; i < list.size()/10 ; i++) {
    		SharedData.SceneLight light = new SharedData.SceneLight();
    		light.name = list.get(i*10);
    		light.id = Integer.parseInt(list.get(i*10+1));
    		light.pos[0] = Integer.parseInt(list.get(i*10+2));
    		light.pos[1] = Integer.parseInt(list.get(i*10+3));
    		light.pos[2] = Integer.parseInt(list.get(i*10+4));
    		light.dir[0] = Integer.parseInt(list.get(i*10+5));
    		light.dir[1] = Integer.parseInt(list.get(i*10+6));
    		light.dir[2] = Integer.parseInt(list.get(i*10+7));
    		light.energy = Float.parseFloat(list.get(i*10+8));
    		light.type = SharedData.SceneLight.LIGHT_TYPE.values()[Integer.parseInt(list.get(i*10+9))];
    		
    		SharedData.GetLight().append(light.id, light);
    		
    		
    	}
    	
    }             
    
    private class PrefetchData extends AsyncTask<Void, Void, Void> {
    	
    	private TextView	onScreenText; 
    	
    	
    	public void SetText(String s) {
    		onScreenText.setText(s);
    	}
    	
    	
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            // before making http calls        
            Log.v(TAG,"LOADING TEXT ON SCREEN..");
            onScreenText = (TextView) findViewById(R.id.on_screen_text);
            onScreenText.setText("Loading .OBJ files...");
        }
 
        @Override
        protected Void doInBackground(Void... arg0) {
        	
        	Log.v(TAG,"BEGIN SCREEN DIMEN INIT");
        	GL2JNILib.InitScreenDimen(SceneGraph.getXscreen(), SceneGraph.getYscreen(), SceneGraph.getDensity());
        	
        	
        	Log.v(TAG,"BEGIN ENGINE");
        	 GL2JNILib.InitEngine();
        	 
        	 Log.v(TAG,"BEGIN INIT MEDIA");
        	 
        	 for(String s : files){
        		 text = s;
        		 /*
        		 runOnUiThread(new Runnable() {
        	         @Override
        	             public void run() {
        	             SetText(text);        
        	         }
        	        });
        	        */
        		 initMedia(s);
        	 }
        	 
        	 ParseData();
        	 
        	 
        	// SetText("Loading .OBJ files... DONE!");
        	 
            return null;
        }
 
        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            // After completing http call
            // will close this activity and lauch main activity
            
            GL2JNILib.LoadShadersSources((String[])shaderSources.toArray(new String[shaderSources.size()]));
            
            GL2JNILib.InitStage();
            
            glRenderer = new GL2JNIView.Renderer();
            
            float ratio = (float) SceneGraph.getXscreen() / SceneGraph.getDensity();
                       
            offView = new GLOffScreenView(480,270);                      
            
            offView.setRenderer(glRenderer);
            
            for(int i = 0 ; i < SharedData.GetScenes().size() ; i++) { 
            	int key = SharedData.GetScenes().keyAt(i);
            	GenerateScenePic(key);
            	int[] stages =  SharedData.GetScenes().get(key).GetStage();
            	for(int j = 0 ; j < stages.length ; j++) {
            		GenerateStagePic(stages[j]);
            		//Log.v(TAG,"Gen pics for " + SharedData.GetStages().get(stages[j]).GetName());
            		int[] entities = SharedData.GetStages().get(stages[j]).GetStage();
            		for(int k = 0 ; k < entities.length ; k++){
            			GenerateMatPic(SharedData.GetEntities().get(entities[k]).GetMat(),0);
            		}
            	}
            }
           
            
            NextActivity();
        }
 
        private void GenerateScenePic(int id){
        	//GL2JNILib.RenderObj(15);
        	GL2JNILib.LoadStage(SharedData.GetScenes().get(id).GetStage());
        	Bitmap snapshot = offView.getBitmap();
        	SharedData.GetScenePics().put(id,snapshot);
        	//photo.setImageBitmap(snapshot);
        }
        
        private void GenerateStagePic(int id){
        	//GL2JNILib.RenderObj(15);
        	GL2JNILib.RenderObj(id);
        	Bitmap snapshot = offView.getBitmap();
        	SharedData.GetStagePics().put(id,snapshot);
        	//photo.setImageBitmap(snapshot);
        }
        
        private void GenerateMatPic(int mat_id, int primitive){
        	GL2JNILib.RenderMat(mat_id, primitive);
        	Bitmap snapshot = offView.getBitmap();
        	SharedData.GetMatPics().put(mat_id,snapshot);
        	//photo.setImageBitmap(snapshot);
        }
 
}
    
    
    private String ConvertStreamToString(InputStream is) {
        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
        StringBuilder sb = new StringBuilder();

        String line = null;
        try {
            while ((line = reader.readLine()) != null) {
                sb.append(line + "\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return sb.toString();
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
