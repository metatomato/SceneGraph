package gl.iglou.scenegraph;

import android.app.Activity;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.Button;
import android.widget.Checkable;
import android.widget.CheckedTextView;
import android.widget.ExpandableListView;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;



public class SceneExpandableAdapter extends BaseExpandableListAdapter{

		  static final String TAG = "SCENE";
		  
		 // static private ArrayList<SceneData.StageObject> stages;
		 // static private ArrayList<SceneData.EntityObject> entities;
		  
		  private LayoutInflater inflater;
		  private Typeface ubuntuMediumTT;
		  private Typeface ubuntuRegularTT;
		  private Toast switchModeToast;
		  static public int selectedGroup = -1;
		  static public int selectedChild = -1;
		  static private MatModeInterface matModeCallback;
		  static private SelectionInterface selectionCallback;
		  
		  static private LibraryActivity.LIB_MODE currentMode;
		  static private Drawable drawableEyeClosed;
		  static private Drawable drawableEye;
		  static private Drawable entity;
		  static private Drawable mesh;
		  static private Drawable material;
		  static private int iglouBlue;
		  static private int iglouGray;
		  static private int iglouWhite;
		  
		  
		  public SceneExpandableAdapter(Activity act, SceneData groups) {
		   // this.stages = groups.GetStages();
		   // this.entities = groups.GetEntities();
		    inflater = act.getLayoutInflater();
		    matModeCallback = (MatModeInterface) act;
		    selectionCallback = (SelectionInterface) act;
		    
		    switchModeToast = Toast.makeText(act, "SWITCHING MODE!", Toast.LENGTH_SHORT);
		    
		    String fontPath = "fonts/Ubuntu-M.ttf";
	        ubuntuMediumTT = Typeface.createFromAsset(act.getAssets(), fontPath);
	        fontPath = "fonts/Ubuntu-R.ttf";
	        ubuntuRegularTT = Typeface.createFromAsset(act.getAssets(), fontPath);
	        
	        drawableEyeClosed 	= act.getResources().getDrawable(R.drawable.scene_eye_closed);
	        drawableEye 		= act.getResources().getDrawable(R.drawable.scene_eye);
	        entity 				= act.getResources().getDrawable(R.drawable.scene_entity_selector);
	        mesh 				= act.getResources().getDrawable(R.drawable.scene_mesh_selector);
	        material 			= act.getResources().getDrawable(R.drawable.scene_mat_selector);
	        
	        iglouBlue 	= act.getResources().getColor(R.color.iglou_blue);
	        iglouGray 	= act.getResources().getColor(R.color.iglou_gray);
	        iglouWhite 	= act.getResources().getColor(android.R.color.white);
		  }

		 		 
		  @Override
		  public Object getChild(int groupPosition, int childPosition) {
			  String ret_string;
			  if(childPosition != 0) { 
				  int sub_ent_pos = (childPosition -1 ) /3; 
				  int ret_id = childPosition % 3;
				  ret_string = String.valueOf(groupPosition) + " / " + String.valueOf(childPosition) + " / " + String.valueOf(sub_ent_pos) + " / " +  String.valueOf(ret_id);
				  if(ret_id == 1) ret_string = SceneData.GetStages().valueAt(groupPosition).sub_entities.get(sub_ent_pos).name;
				  if(ret_id == 2) ret_string = SceneData.GetStages().valueAt(groupPosition).sub_entities.get(sub_ent_pos).mesh;
				  if(ret_id == 0) ret_string = SceneData.GetStages().valueAt(groupPosition).sub_entities.get(sub_ent_pos).material;
			  } else ret_string = SceneData.GetStages().valueAt(groupPosition).name;
		    return ret_string;
		  }

		  
		  @Override
		  public long getChildId(int groupPosition, int childPosition) {
		    return 0;
		  }

		  public class ChildViewHolder {
			    TextView text;
	      }		  
		  
		  @Override
		  public View getChildView(final int groupPosition, final int childPosition, boolean isLastChild, View convertView, ViewGroup parent) {
		    
			final String children = (String) getChild(groupPosition, childPosition);
			final int group_pos = groupPosition;
			ChildViewHolder view_holder;
			
		    if (convertView == null) {
		      convertView = inflater.inflate(R.layout.scene_entity_list, null);
		      view_holder = new ChildViewHolder();
		      view_holder.text = (TextView) convertView.findViewById(R.id.entity_name);
		      convertView.setTag(view_holder);
		    }
		    
		    view_holder = (ChildViewHolder) convertView.getTag();
		    
		    if (childPosition == 0) {
		    	view_holder.text.setCompoundDrawablesWithIntrinsicBounds(R.drawable.scene_stage_selector,0,0,0);
		    	view_holder.text.setPadding(0,0,0,0);		    	 
		    }
		    else{
			    SetChildIcon(view_holder.text,childPosition);
			    view_holder.text.setPadding(30,0,0,0);
		    }
		    
		    if(selectedGroup == groupPosition && selectedChild == childPosition) {
		    	view_holder.text.setSelected(true);
		    }else {
		    	view_holder.text.setSelected(false);
		    }
		    
		    view_holder.text.setTypeface(ubuntuRegularTT);
		    view_holder.text.setText(children);
		    
		    
		    
		    convertView.setOnClickListener(new OnClickListener() {
		      @Override
		      public void onClick(View v) {
		    	  
		    	  SharedData.TYPE mode = SharedData.TYPE.SCENE;
		    	  int idx = group_pos;
    			  int sub_idx = 0;
    			  String toast_text = "";
		    	  
    			  selectedGroup = groupPosition;
    			  selectedChild = childPosition;
    			  notifyDataSetChanged();
    			  //v.setSelected(true);
    			  
		    	  if(childPosition == 0) {
		    		  mode = SharedData.TYPE.STAGE;
		    		  idx = SceneData.GetStages().valueAt(group_pos).id;
		    		  toast_text = "STAGE MODE";
		    	  }
		    	  if((childPosition)%3 == 1) {
		    		  mode = SharedData.TYPE.ENTITY;
		    		  idx = SceneData.GetStages().valueAt(group_pos).id;
		    		  sub_idx =  SceneData.GetStages().valueAt(group_pos).sub_entities.get((childPosition - 1)/3).id;
		    		  toast_text = "ENTITY MODE";
		    	  }
		    	  if((childPosition)%3 == 2) {
		    		  mode = SharedData.TYPE.MESH;
		    		  idx = SceneData.GetStages().valueAt(group_pos).id;
		    		  sub_idx =  SceneData.GetStages().valueAt(group_pos).sub_entities.get((childPosition - 1)/3).id;
		    		  toast_text = "MESH MODE";
		    		  
		    	  }
		    	  
		    	  if((childPosition)%3 == 0 && childPosition != 0) {
		    		  mode = SharedData.TYPE.MAT;
		    		  idx = SceneData.GetStages().valueAt(group_pos).id;
		    		  int ent_idx =  SceneData.GetStages().valueAt(group_pos).sub_entities.get((childPosition - 1)/3).id;
		    		  sub_idx = SharedData.GetEntities().get(ent_idx).GetMat();
		    		  toast_text = "MATERIAL MODE";
		    	  }
		    	  
		    	  switchModeToast.setText(toast_text);
		    	  switchModeToast.show();
		    	 
		    	  SwitchMode(mode, idx, sub_idx);
		      }
		    });
		    
		    return convertView;
		  }

		  @Override
		  public int getChildrenCount(int groupPosition) {
		    return (1 + 3*(SceneData.GetStages().valueAt(groupPosition).sub_entities.size()));
		  }
		  

		  @Override
		  public Object getGroup(int groupPosition) {
		    return SceneData.GetStages().valueAt(groupPosition);
		  }

		  @Override
		  public int getGroupCount() {
		    return SceneData.GetStages().size();
		  }

		  @Override
		  public void onGroupCollapsed(int groupPosition) {
		    super.onGroupCollapsed(groupPosition);
		  }

		  @Override
		  public void onGroupExpanded(int groupPosition) {
		    super.onGroupExpanded(groupPosition);
		  }

		  @Override
		  public long getGroupId(int groupPosition) {
		    return 0;
		  }

		  @Override
		  public View getGroupView(int groupPosition, boolean isExpanded,
		      View convertView, ViewGroup parent) {
			  
			 final int pos = groupPosition;
			 final boolean expanded = isExpanded;
			 final ExpandableListView elv = (ExpandableListView) parent;
			 
		    if (convertView == null) {
		      convertView = inflater.inflate(R.layout.scene_stage_list, null);
		    }
		    
		    CheckedTextView current_view = (CheckedTextView) convertView.findViewById(R.id.stage_name);
		    ImageView eye_button = (ImageView) convertView.findViewById(R.id.scene_eye_button);
		    Button expand_button = (Button) convertView.findViewById(R.id.scene_expand_button);
		    
		    if(selectedGroup == groupPosition && selectedChild == -1) {
		    	current_view.setCompoundDrawablesWithIntrinsicBounds(R.drawable.scene_scene_light,0,0,0);
		    } else {
		    	current_view.setCompoundDrawablesWithIntrinsicBounds(R.drawable.scene_scene,0,0,0);
		    }
		    
		    View divider = convertView.findViewById(R.id.scene_data_divider);
		    if(isExpanded) {
		    	divider.setBackgroundColor(iglouBlue);
		    } else divider.setBackgroundColor(0xffffffff);
		    
		    SceneData.StageObject currentGroup = (SceneData.StageObject) getGroup(groupPosition);
		    
		    ((Checkable) convertView).setChecked(isExpanded);
		     convertView.setSelected(isExpanded);
		    
		    current_view.setTypeface(ubuntuMediumTT);
		    current_view.setText(currentGroup.name);
		    SetStageSelection(current_view,SceneData.GetStages().valueAt(pos).selected);
		    
		    current_view.setChecked(isExpanded);
		    current_view.setSelected(isExpanded);
		    
		    SetIcon(eye_button,pos,currentGroup.visible);
		    
		    eye_button.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					GL2JNILib.SetStageVisibility(SceneData.GetStages().valueAt(pos).id, ToggleEyeIcon((ImageView)v,pos));
				}
			});
		    
		    expand_button.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					if(expanded){
						elv.collapseGroup(pos);
					}else {
						elv.expandGroup(pos);
					}
				}
			});
		   	
		    
		    return convertView;
		  }

		  @Override
		  public boolean hasStableIds() {
		    return false;
		  }

		  @Override
		  public boolean isChildSelectable(int groupPosition, int childPosition) {
		    return true;
		  }
		  
		  
		  static public boolean ToggleEyeIcon(ImageView v, int pos) {
			  boolean ret;
			  if(SceneData.GetStages().valueAt(pos).visible) {
				  SceneData.GetStages().valueAt(pos).visible = false;
					v.setImageDrawable(drawableEyeClosed);
					ret = false;
			  } else {
					SceneData.GetStages().valueAt(pos).visible = true;
					v.setImageDrawable(drawableEye);
					ret = true;
			  }
			  return ret;
		  }
		  
		  
		  static public void SetIcon(ImageView v, int pos, boolean is_open) {
			  if(is_open) {
				  v.setImageDrawable(drawableEye);
				  SceneData.GetStages().valueAt(pos).visible = true;
			  } else {
				  v.setImageDrawable(drawableEyeClosed);
				  SceneData.GetStages().valueAt(pos).visible = false;
			  }
		  }
		  
		  
		  static public void ToggleStageSelection(TextView v, int pos) {
			  if(SceneData.GetStages().valueAt(pos).selected) {
				  SceneData.GetStages().valueAt(pos).selected = false;
					v.setTextColor(iglouWhite);
			  } else {
				  SceneData.GetStages().valueAt(pos).selected = true;
					v.setTextColor(iglouBlue);
			  }
		  }
		  
		  
		  static public void SetStageSelection(TextView v, boolean is_selected) {
			  if(is_selected) {
				  v.setTextColor(iglouBlue);
			  } else {
				  v.setTextColor(iglouWhite);
			  }
		  }
		  
		  
		  static public void SetChildIcon(TextView v, int position) {
			  if((position%3) == 1) { v.setCompoundDrawablesWithIntrinsicBounds(R.drawable.scene_entity_selector,0,0,0); Log.v(TAG,"Set ent icon at "+ String.valueOf((position+1)%3));}
			  if((position%3) == 2) { v.setCompoundDrawablesWithIntrinsicBounds(R.drawable.scene_mesh_selector,0,0,0);  Log.v(TAG,"Set mesh icon at "+String.valueOf((position+1)%3));}
			  if((position%3) == 0) { v.setCompoundDrawablesWithIntrinsicBounds(R.drawable.scene_mat_selector,0,0,0);  Log.v(TAG,"Set mat icon at "+String.valueOf((position+1)%3));}
			  
		  }
		  
		  
		  public void SwitchMode(SharedData.TYPE mode, int stage_idx, int sub_stage_idx){
			  if(SharedData.TYPE.SCENE == mode){
	            	GL2JNILib.LoadStage(SceneData.GetStageValues());
	            	SharedData.SetMode(SharedData.TYPE.SCENE);
	            	Log.v(TAG,"SWITCH TO SCENE WITH " + String.valueOf(stage_idx));
	            	selectionCallback.EnableLockButton(true);
	        	}else{
	        		
	        		SceneData.ClearSelection();
	        		GL2JNILib.ResetSelection();
	        		selectionCallback.UpdateSelection();
	        		selectionCallback.EnableLockButton(false);
	        	}
	        	if(SharedData.TYPE.STAGE == mode){
	        		Log.v(TAG,"SWITCH TO STAGE WITH " + String.valueOf(stage_idx));
	            	GL2JNILib.RenderObj(stage_idx);
	            	SharedData.SetMode(SharedData.TYPE.STAGE);
	        	}
	        	if(SharedData.TYPE.ENTITY == mode){
	        		Log.v(TAG,"SWITCH TO ENTITY WITH " + String.valueOf(sub_stage_idx));
	            	GL2JNILib.RenderSubEntity(stage_idx, sub_stage_idx);
	            	SharedData.SetMode(SharedData.TYPE.ENTITY);
	        	}
	        	if(SharedData.TYPE.MESH == mode){
	        		Log.v(TAG,"SWITCH TO MESH WITH " + String.valueOf(sub_stage_idx));
	        		GL2JNILib.MeshMode(true);
	            	GL2JNILib.RenderSubEntity(stage_idx, sub_stage_idx);
	            	SharedData.SetMode(SharedData.TYPE.ENTITY);
	        	}else {
	        		GL2JNILib.MeshMode(false);
	        	}
	        	if(SharedData.TYPE.MAT == mode){
	        		Log.v(TAG,"SWITCH TO MAT WITH " + String.valueOf(sub_stage_idx));
	        		matModeCallback.EnableMatMode(true);
	            	GL2JNILib.RenderMat(sub_stage_idx,matModeCallback.GetPrimitive());
	            	SharedData.SetMode(SharedData.TYPE.MAT);
	        	}else{
	        		matModeCallback.EnableMatMode(false);
	        	}
		  }
} 





























