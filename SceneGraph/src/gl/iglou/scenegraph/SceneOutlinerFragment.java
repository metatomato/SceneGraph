package gl.iglou.scenegraph;

import android.app.Activity;
import android.app.Fragment;
import android.graphics.Typeface;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;



public class SceneOutlinerFragment extends Fragment {

	final private String TAG = "SCENE";
	
	private MatModeInterface matModeCallback;
	private SelectionInterface selectionCallback;
    private ExpandableListView outlinerListView;
    private SceneExpandableAdapter outlinerAdapter;   
    private Button toggleExpandButton;
    private Button toggleVisibleButton;
    private boolean isExpanded;
    private boolean isVisible;
    private TextView textHeader;    
    
    private SceneData sceneData;
    
    private int currentSceneId;

    private int lastSelected;
    
    @Override 
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
    }
        
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        	
        Activity act = getActivity();        
        isExpanded = false;
        isVisible = true;
        
        matModeCallback = (MatModeInterface) act;
        selectionCallback = (SelectionInterface) act;
        
        currentSceneId = act.getIntent().getIntExtra("SCENE_ID",0);
        sceneData = new SceneData(currentSceneId);
        
        outlinerAdapter = new SceneExpandableAdapter(act,sceneData);
        
        View rootView = inflater.inflate(R.layout.scene_fragment_outliner,container,false);           
        
        textHeader = (TextView) rootView.findViewById(R.id.scene_outliner_header);
        String fontPath = "fonts/Ubuntu-M.ttf";
        Typeface ubuntuMediumTT = Typeface.createFromAsset(act.getAssets(), fontPath);
        textHeader.setTypeface(ubuntuMediumTT);
        
        toggleExpandButton  = (Button) rootView.findViewById(R.id.scene_all_expand);
        toggleVisibleButton = (Button) rootView.findViewById(R.id.scene_all_visible);      
        
        outlinerListView = (ExpandableListView) rootView.findViewById(R.id.outliner_item_list);
        outlinerListView.setAdapter(outlinerAdapter);
        outlinerListView.setOnGroupClickListener(new OutlinerItemClickListener());
        
        toggleVisibleButton.setOnClickListener(new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		int size = outlinerListView.getExpandableListAdapter().getGroupCount();        		
        		if(isVisible){
	        		for(int i = 0 ; i< size ; i++) {
	        			SceneData.GetStages().valueAt(i).visible = false;
	        			GL2JNILib.SetStageVisibility(SceneData.GetStages().valueAt(i).id,false);
	        			isVisible = false;	        			
	        		}
        		}else {
        			for(int i = 0 ; i< size ; i++) {
        				SceneData.GetStages().valueAt(i).visible = true;
        				GL2JNILib.SetStageVisibility(SceneData.GetStages().valueAt(i).id,true);
        				isVisible = true;        				
	        		}
        		}
        		SetHeaderIcon();
        		((SceneExpandableAdapter)outlinerListView.getExpandableListAdapter()).notifyDataSetChanged();
        	}
        });
        
        toggleExpandButton.setOnClickListener(new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		int size = outlinerListView.getExpandableListAdapter().getGroupCount();        		
        		if(isExpanded){
	        		for(int i = 0 ; i< size ; i++) {
	        			outlinerListView.collapseGroup(i);
	        			isExpanded = false;
	        		}
        		}else {
        			for(int i = 0 ; i< size ; i++) {
        				outlinerListView.expandGroup(i);
	        			isExpanded = true;
	        		}
        		}
        		SetHeaderIcon();
        	}
        });
        
        return rootView;
    }

    void SetHeaderIcon() {
    	TextView tv = (TextView)textHeader.findViewById(R.id.scene_outliner_header);
    	if(!isExpanded && isVisible) {tv.setCompoundDrawablesWithIntrinsicBounds(R.drawable.expander_open_holo_light, 0, R.drawable.scene_eye, 0);}
    	if(isExpanded && !isVisible) {tv.setCompoundDrawablesWithIntrinsicBounds(R.drawable.expander_close_holo_light, 0, R.drawable.scene_eye_closed, 0);}
    	if(isExpanded && isVisible) {tv.setCompoundDrawablesWithIntrinsicBounds(R.drawable.expander_close_holo_light, 0, R.drawable.scene_eye, 0);}
    	if(!isExpanded && !isVisible) {tv.setCompoundDrawablesWithIntrinsicBounds(R.drawable.expander_open_holo_light, 0, R.drawable.scene_eye_closed, 0);}
    }
    
    
    public void SetSelections(int[] idx_array) {
    
    }
    
    public void RefreshData(){
    	((SceneExpandableAdapter)outlinerListView.getExpandableListAdapter()).notifyDataSetChanged();
    }
    
    private class OutlinerItemClickListener implements ExpandableListView.OnGroupClickListener {
        @Override
        public boolean onGroupClick (ExpandableListView parent, View v, int groupPosition, long id) {
        	
        	Log.v(TAG,"GROUP CLICK!!");
        	
        	lastSelected = groupPosition;
        	
        	TextView view = (TextView) v.findViewById(R.id.stage_name);
        	
        	if(SharedData.GetAppData().isSceneMode()){
	        	SceneExpandableAdapter.ToggleStageSelection((TextView)view,groupPosition);
	        	int stage_id = sceneData.GetStages().valueAt(groupPosition).id;        
	        	GL2JNILib.SelectStage(stage_id);
	        	selectionCallback.UpdateSelection();
        	} else {
        		GL2JNILib.LoadStage(SceneData.GetStageValues());
        		SharedData.SetMode(SharedData.TYPE.SCENE);
        		sceneData.ClearSelection();
        		RefreshData();
        		selectionCallback.EnableLockButton(true);
        		matModeCallback.EnableMatMode(false);
        		GL2JNILib.MeshMode(false);
        		Toast switchModeToast = Toast.makeText(getActivity(), "SCENE MODE!", Toast.LENGTH_SHORT);
        		switchModeToast.show();
        	}
        	
        	SceneExpandableAdapter.selectedGroup = groupPosition;
        	SceneExpandableAdapter.selectedChild = -1;
        	RefreshData();
        	
        	return true;
        }
    }
    
   
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}

