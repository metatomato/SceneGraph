package gl.iglou.scenegraph;

import java.util.ArrayList;
import java.util.Arrays;

import android.util.Log;
import android.util.SparseArray;

public class SceneData {

	final String TAG = "SCENEDATA";
	
	//static private ArrayList<StageObject> stages;
	//static private ArrayList<EntityObject> entities;
	
	static private SparseArray<StageObject> stages;
	static private SparseArray<EntityObject> entities;
	static private String sceneName;
	
	public SceneData(int idx) {
		int[] id = SharedData.GetScenes().get(idx).GetStage();
		int size = id.length;
		stages = new SparseArray<StageObject>();
		entities = new SparseArray<EntityObject>();
		sceneName = SharedData.GetScenes().get(idx).GetName();
		for(int i = 0; i < size ; i++) {
			 stages.append(id[i],new StageObject(id[i]));
			 int[] entity_ref = SharedData.GetStages().get(id[i]).GetStage();
			 
			 for(int j = 0 ; j < entity_ref.length ; j++) {
				 EntityObject obj = new EntityObject(entity_ref[j]);
				 entities.append(entity_ref[j],obj);
				 stages.valueAt(i).AddSubEntity(obj);
			 }
		}
	}
	
	static public String GetSceneName(){
		return sceneName;
	}

	static public void SyncSelection() {
		ClearSelection();
		int[] list = GL2JNILib.GetSelections();
		for(int idx : list) {
			SetSelected(idx);
		}
	}
	
	static public void SetSelected(int idx) {
		stages.get(idx).selected = true;
	}
	
	static public void ClearSelection() {
		for(int i = 0 ; i < stages.size(); i++) {
			stages.valueAt(i).selected = false;
		}
	}
	
	static public  SparseArray<StageObject> GetStages() {
		return stages;
	}
	
	static public  int[] GetStageValues() {
		int[] ret_array = new int[stages.size()];
		for(int i = 0 ; i < stages.size(); i++) {
			ret_array[i] = stages.valueAt(i).id;
		}
		return ret_array;
	}
	
	
	static public SparseArray<EntityObject> GetEntities() {
		return entities;
	}
	
	
	void Print() {
		for(int i = 0 ; i < stages.size() ; i++) {
        	Log.v(TAG," stage " + stages.valueAt(i).name);
        	for(SceneData.EntityObject obj : stages.valueAt(i).sub_entities) {
        		Log.v(TAG," sub_entity " + obj.name);
        	}
        }
	}
	
	//
	// STAGEOBJECT CLASS
	////////////////////////////////////////////////////////////////////////
	
	public static class StageObject {
		int id;
		String name;
		boolean visible;
		boolean selected;
		ArrayList<EntityObject> sub_entities;
		
		public StageObject(int in_id) {
			id = in_id;
			name = SharedData.GetStages().get(id).GetName();
			visible = true;
			selected = false;
			sub_entities = new ArrayList<EntityObject>();
		}
		
		ArrayList<EntityObject> GetEntities() {
			return sub_entities;
		}
		
		void SetEntities(EntityObject[] ent) {
			for(int i = 0; i < ent.length ; i ++) {
				sub_entities.add(ent[i]);
			}
		}
		
		void AddSubEntity(EntityObject obj) {
			sub_entities.add(obj);
		}	
	}
	
	
	public static class EntityObject {
		int id;
		String name;
		String mesh;
		String material;
		
		
		public EntityObject(int in_id) {
			id = in_id;
			int[] ref = SharedData.GetEntities().get(id).GetRefs();
			name	 = SharedData.GetObj().get(ref[0]);
			mesh 	 = SharedData.GetMesh().get(ref[1]);
			material = SharedData.GetMat().get(ref[2]);
			
		}
	}
}
