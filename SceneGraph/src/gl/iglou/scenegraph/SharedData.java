package gl.iglou.scenegraph;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;
import android.util.SparseArray;
import android.view.ViewGroup.LayoutParams;

public class SharedData {

	private static SharedData data;
	
	enum TYPE{SCENE,STAGE,ENTITY,OBJ,MESH,MAT};
	enum PARAMS{NAME,ID};
	
	private SparseArray<SceneEntity> scenes;
	private SparseArray<SceneEntity> stages;
	private SparseArray<Entity> 	 entities;
	private SparseArray<String> 	 obj;
	private SparseArray<String> 	 mat;
	private SparseArray<String> 	 mesh;
	private SparseArray<SceneLight>  light;
	 
	private SparseArray<Bitmap> 	 scenePics;
	private SparseArray<Bitmap> 	 stagePics;
	private SparseArray<Bitmap> 	 matPics;
	
	private TYPE currentMode;
	
	private SharedData(Context context){
		scenes 		  	 = new SparseArray<SceneEntity>();
		stages			 = new SparseArray<SceneEntity>();
		entities		 = new SparseArray<Entity>();
		obj				 = new SparseArray<String>();
		mat				 = new SparseArray<String>();
		mesh 			 = new SparseArray<String>();
		light 			 = new SparseArray<SceneLight>();
		
		scenePics = new SparseArray<Bitmap>();
		stagePics = new SparseArray<Bitmap>();
		matPics   = new SparseArray<Bitmap>();
	}
	
	
	public static void InitInstance(Context context) {
		if(data == null){
			data = new SharedData(context);
		}else
			Log.e("SharedData","Instance of SharedData already exist!");
	}
	
	public static void PostInit() {
		data.ParseTriangleFromEntities();
	}
	
	public static SharedData GetAppData(){
		return data;
	}
	
	public static SparseArray<SceneEntity> 		 GetScenes()		 {return data.scenes;};
	public static SparseArray<SceneEntity>		 GetStages()		 {return data.stages;};
	public static SparseArray<Entity> 			 GetEntities()		 {return data.entities;};
	public static SparseArray<String>			 GetObj() 			 {return data.obj;};
	public static SparseArray<String> 		  	 GetMesh() 			 {return data.mesh;};
	public static SparseArray<String> 			 GetMat()			 {return data.mat;};
	public static SparseArray<SceneLight>		 GetLight()			 {return data.light;};
	
	public static SparseArray<Bitmap>			 GetScenePics()		 {return data.scenePics;};
	public static SparseArray<Bitmap>			 GetStagePics() 	 {return data.stagePics;};
	public static SparseArray<Bitmap>			 GetMatPics() 	 {return data.matPics;};
	
	
	public static SceneEntity	GetScene(int id)		{return data.scenes.get(id);}
	public static SceneEntity	GetStage(int id)		{return data.stages.get(id);}
	
	public static TYPE GetMode() {return data.currentMode;}	
	public static void SetMode(TYPE mode) {data.currentMode = mode;}
		
	public boolean isSceneMode()	{return (data.currentMode == TYPE.SCENE);}
	public boolean isStageMode()	{return (data.currentMode == TYPE.STAGE);}
	public boolean isEntityMode()	{return (data.currentMode == TYPE.ENTITY);}
	public boolean isMeshMode()		{return (data.currentMode == TYPE.MESH);}
	public boolean isMatMode()  	{return (data.currentMode == TYPE.MAT);}
	
	public static String[] GetDataList(TYPE type, PARAMS params) {
		String[] ret; 
		int key,size;
		switch(type) {
			case SCENE :
				size = data.scenes.size();
				ret = new String[size];
				for(int i = 0 ; i < size ; i++) {
					key = data.scenes.keyAt(i);
					if(PARAMS.NAME == params){
						ret[i] = data.scenes.get(key).name;
					}
					if(PARAMS.ID == params){
						ret[i] = String.valueOf(key);
					}
				}
			break;
			case STAGE :
				size = data.stages.size();
				ret = new String[size];
				for(int i = 0 ; i < size ; i++) {
					key = data.stages.keyAt(i);
					if(PARAMS.NAME == params){
						ret[i] = data.stages.get(key).name;
					}
					if(PARAMS.ID == params){
						ret[i] = String.valueOf(key);
					}
				}
			break;
			case ENTITY :
				size = data.entities.size();
				ret = new String[size];
				for(int i = 0 ; i < size ; i++) {
					key = data.entities.keyAt(i);
					if(PARAMS.NAME == params){
						ret[i] = data.obj.get(data.entities.get(key).obj);
					}
					if(PARAMS.ID == params){
						ret[i] = String.valueOf(key);
					}
					
				}
			break;
			case OBJ :
				size = data.obj.size();
				ret = new String[size];
				for(int i = 0 ; i < size ; i++) {
					key = data.obj.keyAt(i);
					ret[i] = data.obj.get(key);
				}
			break;
			case MESH :
				size = data.mesh.size();
				ret = new String[size];
				for(int i = 0 ; i < size ; i++) {
					key = data.mesh.keyAt(i);
					ret[i] = data.mesh.get(key);
				}
			break;
			case MAT :
				size = data.mat.size();
				ret = new String[size];
				for(int i = 0 ; i < size ; i++) {
					key = data.mat.keyAt(i);
					ret[i] = data.mat.get(key);
				}
			break;
			default :
				ret = new String[1];
				ret[0] = "";
		}
		return ret;
	}
	
	
	static String[] GetStageFromScene(int id) {
		int[] id_list = data.scenes.get(id).GetStage();
		int size = id_list.length;
		String[] ret_vec = new String[size];
		for(int i = 0 ; i < size ; i++) {
			ret_vec[i] = data.stages.get(id_list[i]).GetName();
		}
		return ret_vec;
	}
	
	
	static String[] GetEntityFromStage(int id) {
		int[] id_list = data.stages.get(id).GetStage();
		int size = id_list.length;
		String[] ret_vec = new String[size];
		for(int i = 0 ; i < size ; i++) {
			ret_vec[i] = data.obj.get(id_list[i]);
		}
		return ret_vec;
	}
	
	
	
	public static class Entity {
		private int obj  = -1;
		private int mesh = -1;
		private int mat  = -1;
		private int parent = -1;
		private int tri = -1;
		
		public Entity(int obj_id, int mesh_id ,int mat_id, int stage_id, int tri_id){
			obj = obj_id;
			mesh = mesh_id;
			mat = mat_id;
			parent = stage_id;
			tri = tri_id;
		}
		
		public int[] GetRefs() {
			int[] ref = {obj,mesh,mat};
			return ref;
		}
		
		public int GetMat(){
			return mat;
		}
		
		public int GetParent() {
			return parent;
		}
		
		public int GetTri() {
			return tri;
		}
		
		public void SetParent(int in_parent) {
			parent = in_parent;
		}
	}
	
	public static class SceneEntity {
		
		private ArrayList<Integer> entity;
		private String	name;
		private int id;
		private int parent;
		private int tri;
		
		public SceneEntity(String in_name, int in_id, int[] ent_id_list, int in_parent) {
			name = in_name;
			id = in_id;
			parent = in_parent;
			int size = ent_id_list.length;
			Integer[] int_array = new Integer[size];
			for(int i = 0 ; i < size ; i++){
				int_array[i] = ent_id_list[i];
			}
			entity = new ArrayList<Integer>(Arrays.asList(int_array));
		}
		
		public SceneEntity(String in_name, int in_id, List<Integer> ent_id_list, int in_parent) {
			name = in_name;
			id = in_id;
			parent = in_parent;
			entity = new ArrayList<Integer>(ent_id_list);
		}
		
		public int GetId() {
			return id;
		}
		
		public String GetName() {
			return name;
		}
		
		public int GetParent() {
			return parent;
		}
		
		public int[] GetStage() {
			int[] ent_array = new int[entity.size()];
			int j = 0;
			for(Integer i : entity){
				ent_array[j++] = i; 
			}
			return ent_array;
		}
		
		public int GetTri(){
			return tri;
		}
		
		public void SetTri(int count){
			tri = count;
		}
		
	}
	
	public static class SceneLight {
		final public String TAG = "SCENE_GL_LIGHT";
		static float ratio = 0.f;
		
		enum LIGHT_TYPE{SUN,POINT,SPOT};
		enum COORD{X,Y,Z};
		
		public String name;
		public int id;
		public float[] pos;
		public float[] dir;
		public float energy;
		public LIGHT_TYPE type;
		
		public SceneLight(){
			id = -1;
			type = LIGHT_TYPE.SUN;
			energy = 1.f;
			pos = new float[3];
			dir = new float[3];
			
			for(float f : pos) { f = 0.f; }
			for(float f : dir) { f = 0.f; }
		}
	
	}
	
	
	public void ParseTriangleFromEntities() {
		
		for(int i = 0 ; i < scenes.size() ; i++) {
			SceneEntity current_scene = scenes.valueAt(i); 
			int[] stage_refs = current_scene.GetStage();
			int scene_tri = 0;
			for(int j = 0 ; j < stage_refs.length ; j++) {
				SceneEntity current_stage = stages.get(stage_refs[j]);
				int[] entity_refs = current_stage.GetStage();
				int stage_tri = 0;
				for(int k = 0 ; k < entity_refs.length ; k++){
					stage_tri += entities.get(entity_refs[k]).GetTri();
				}
				current_stage.SetTri(stage_tri);
				scene_tri += stage_tri;
			}
			current_scene.SetTri(scene_tri);
		}
	}
	
	
	
	//private SparseArray<SceneEntity> scenes;
	//private SparseArray<SceneEntity> stages;
	//private SparseArray<Entity> 	 entities;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
