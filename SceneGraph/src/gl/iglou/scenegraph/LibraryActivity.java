package gl.iglou.scenegraph;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Typeface;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.style.StyleSpan;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;



public class LibraryActivity extends Activity {

	final public String TAG = "LIBRARY"; 
	
	enum LIB_MODE{SCENE,STAGE,MAT};
	
	private ImageView cellImage;
	
	
	private ImageView sceneModeView;
	private ImageView stageModeView;
	private ImageView matModeView;
	
	private View decorView;
	
	private ListView currentList;
	private LibraryArrayAdapter sceneAdapter;
	private LibraryArrayAdapter stageAdapter;
	private LibraryArrayAdapter matAdapter;
	
	static  private SharedData.TYPE last_known_mode;
	
	@Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.library_activity);
        
        Log.v(TAG,"Library Activity onCreate()!");
        
	    currentList = (ListView) findViewById(R.id.listview);
	    sceneModeView = (ImageView) findViewById(R.id.library_button_scene);
		stageModeView = (ImageView) findViewById(R.id.library_button_stage);
		matModeView = (ImageView) findViewById(R.id.library_button_mat);
		
	    currentList.setSelector(R.drawable.scene_data_selector);	   	  
	    
	    sceneAdapter = new LibraryArrayAdapter(this,SharedData.GetDataList(SharedData.TYPE.SCENE,SharedData.PARAMS.ID));
	    stageAdapter = new LibraryArrayAdapter(this,SharedData.GetDataList(SharedData.TYPE.STAGE,SharedData.PARAMS.ID));
	    matAdapter   = new LibraryArrayAdapter(this,SharedData.GetDataList(SharedData.TYPE.MAT,SharedData.PARAMS.NAME));
	    
	 // SharedData.GetAppData().SetMode(SharedData.TYPE.SCENE);
        SceneMode(null);
        
	    //currentList.setAdapter(sceneAdapter);
	    
	    //sceneModeView.setSelected(true);
	    
	    
	    currentList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
	        @Override
	        public void onItemClick(AdapterView<?> parent, final View view,
	            int position, long id) {
	        	int key = -1;
	        	
	        	 SharedData.TYPE current_mode = SharedData.GetAppData().GetMode();
	        	
	        	if(SharedData.TYPE.SCENE == current_mode){
	        		key = SharedData.GetScenes().keyAt(position);
	            	GL2JNILib.LoadStage(SharedData.GetScenes().get(key).GetStage());
	            	Log.v(TAG,"Scene selected " + String.valueOf(position) + " with key " + key);
	        	}else{
	        		GL2JNILib.ResetSelection();
	        	}
	        	if(SharedData.TYPE.STAGE == current_mode){
	        		int idx = SharedData.GetStages().keyAt(position);
	        		key = SharedData.GetStages().get(idx).GetParent();
	            	GL2JNILib.RenderObj(SharedData.GetStages().get(idx).GetId());
	            	Log.v(TAG,"Stage selected " + String.valueOf(position) + " with key " + key);
	        	}
	        	if(SharedData.TYPE.MAT == current_mode){
	        		int idx = SharedData.GetMat().keyAt(position);
	        		GL2JNILib.RenderMat(idx, 0);
	        		int entity_id = SharedData.GetEntities().keyAt(position);
	        		int stage_id = SharedData.GetEntities().get(entity_id).GetParent();
	        		key = SharedData.GetStages().get(stage_id).GetParent(); 
	        		Log.v(TAG,"Mat selected " + String.valueOf(position) + " with key " + key);
	        	}
	        	
	        	Log.v(TAG,"Item selected " + String.valueOf(position) + " with key " + key);
	        	
	        	Intent i = new Intent(LibraryActivity.this, GL2JNIActivity.class);
		        i.putExtra("SCENE_ID", key);
		        startActivity(i);	
	        	
	            //listview.setItemChecked(position, true);
	        }
	      });
	    
	   
	    
	    decorView = getWindow().getDecorView();
	  }
	
	
	@Override
	public void onRestart(){
		super.onRestart();
		Log.v(TAG,"Library Activity onRestart()!");
		if(last_known_mode == SharedData.TYPE.SCENE){
			SceneMode(null);
		}
		if(last_known_mode == SharedData.TYPE.STAGE){
			StageMode(null);
		}
		if(last_known_mode == SharedData.TYPE.MAT){
			MatMode(null);
		}
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
	
	
	public void SceneMode(View v) {
		 SharedData.GetAppData().SetMode(SharedData.TYPE.SCENE);
		
		sceneModeView.setSelected(true);
		stageModeView.setSelected(false);
		matModeView.setSelected(false);
		
		last_known_mode = SharedData.TYPE.SCENE;
		
		currentList.setAdapter(sceneAdapter);
	}
	
	
	public void StageMode(View v) {
		SharedData.GetAppData().SetMode(SharedData.TYPE.STAGE);
		
		sceneModeView.setSelected(false);
		stageModeView.setSelected(true);
		matModeView.setSelected(false);
		
		last_known_mode = SharedData.TYPE.STAGE;
		
		currentList.setAdapter(stageAdapter);
	}
	
	
	public void MatMode(View v) {
		SharedData.GetAppData().SetMode(SharedData.TYPE.MAT);
		
		sceneModeView.setSelected(false);
		stageModeView.setSelected(false);
		matModeView.setSelected(true);
		
		last_known_mode = SharedData.TYPE.MAT;
		
		currentList.setAdapter(matAdapter);
		
	}
	
	
	public class LibraryArrayAdapter extends ArrayAdapter<String> {
			private final Context context;
			private final String[] values;
			private Typeface ubuntuRegularTT;
			private Typeface ubuntuLightTT;
		  
			private ImageView cellImage; 
	   	  	private TextView title ;
		    private TextView obj_number ;
		    private TextView tri_number;
		    
		    public LibraryArrayAdapter(Context context, String[] values) {
		    	super(context, R.layout.library_cell_layout, values);
		    	this.context = context;
		    	this.values = values;
		    
			    String fontPath = "fonts/Ubuntu-M.ttf";
		        ubuntuRegularTT = Typeface.createFromAsset(getAssets(), fontPath);
		        fontPath = "fonts/Ubuntu-L.ttf";
		        ubuntuLightTT = Typeface.createFromAsset(getAssets(), fontPath);
		 	}
		  
		    public class ViewHolder {
		    	ImageView cellImage; 
		   	  	TextView title ;
		   	  	TextView subtitle ;
			    TextView obj_number ;
			    TextView tri_number;
	    	}
		    
		  @Override
		  public View getView(int position, View convertView, ViewGroup parent) {
			  ViewHolder view_holder;
			  
			  if(convertView == null) {
				LayoutInflater inflater = (LayoutInflater) context
		        .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			    convertView = inflater.inflate(R.layout.library_cell_layout, parent, false);
			    
			    view_holder = new ViewHolder();
			    
			    view_holder.cellImage= (ImageView)convertView.findViewById(R.id.icon);
			    view_holder.title = (TextView) convertView.findViewById(R.id.library_title);
			    view_holder.subtitle = (TextView) convertView.findViewById(R.id.library_subtitle);
			    view_holder.obj_number = (TextView) convertView.findViewById(R.id.obj_number);
			    view_holder.tri_number = (TextView) convertView.findViewById(R.id.tri_number);
			    
			    
			    view_holder.title.setTypeface(ubuntuLightTT,Typeface.BOLD);
			    view_holder.subtitle.setTypeface(ubuntuLightTT);
			    view_holder.subtitle.setTextColor(getResources().getColor(R.color.iglou_gray_A));
			    view_holder.obj_number.setTypeface(ubuntuLightTT);
			    view_holder.tri_number.setTypeface(ubuntuLightTT);			    
			    
			    convertView.setTag(view_holder);
			  }
				
			  view_holder = (ViewHolder) convertView.getTag();
			  
			  SharedData.TYPE current_mode = SharedData.GetAppData().GetMode();
			  Log.v(TAG,"current-mode "+String.valueOf(current_mode));
			  String main_text ="";
			  String subtitle_text ="";
			  SpannableString text_0 = null;
			  SpannableString text_1 = null;
			  
			  if(SharedData.TYPE.SCENE == current_mode){
				  SharedData.SceneEntity current_scene = SharedData.GetScene(Integer.parseInt(values[position]));
				  main_text = current_scene.GetName();
				  String obj = String.valueOf(current_scene.GetStage().length);
				  text_0 = new SpannableString(obj + " Object(s)");
				  text_0.setSpan(new StyleSpan(Typeface.BOLD), 0, obj.length(), 0);
				  String tri = String.valueOf(current_scene.GetTri());
				  text_1 = new SpannableString(tri + " Face(s)");
				  text_1.setSpan(new StyleSpan(Typeface.BOLD), 0, tri.length(), 0);
				  
				  int key = SharedData.GetScenePics().keyAt(position);
			      view_holder.cellImage.setImageBitmap(SharedData.GetScenePics().get(key));
			  }
			  if(SharedData.TYPE.STAGE == current_mode){
				  SharedData.SceneEntity current_stage = SharedData.GetStage(Integer.parseInt(values[position]));				  
				  main_text = current_stage.GetName();
				  subtitle_text = SharedData.GetScene(current_stage.GetParent()).GetName();
				
				  String obj = String.valueOf(current_stage.GetStage().length);
				  text_0 = new SpannableString(obj + " Object(s)");
				  text_0.setSpan(new StyleSpan(Typeface.BOLD), 0, obj.length(), 0);
				  
				  String tri = String.valueOf(current_stage.GetTri());
				  text_1 = new SpannableString(tri + " Face(s)");
				  text_1.setSpan(new StyleSpan(Typeface.BOLD), 0, tri.length(), 0);
				  
				  int key = SharedData.GetStagePics().keyAt(position);
			    	view_holder.cellImage.setImageBitmap(SharedData.GetStagePics().get(key));
			  }
			  if(SharedData.TYPE.MAT == current_mode){
				  main_text = values[position];
				  
				  int key = SharedData.GetMatPics().keyAt(position);
			    	view_holder.cellImage.setImageBitmap(SharedData.GetMatPics().get(key));
			  }
			  
			 
			  view_holder.title.setText(main_text);
			  view_holder.subtitle.setText(subtitle_text);
			  view_holder.obj_number.setText(text_0);
			  view_holder.tri_number.setText(text_1);
			  
		    return convertView;
		  }
		} 

} 

































