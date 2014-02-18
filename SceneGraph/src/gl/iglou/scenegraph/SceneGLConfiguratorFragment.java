package gl.iglou.scenegraph;

import java.util.ArrayList;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;


public class SceneGLConfiguratorFragment extends Fragment implements OnClickListener {

	final static String TAG = "SCENE_GL_CONFIG";
	final static byte X = 0, Y = 1, Z = 2;
	private ArrayList<SceneGLLight> lights;
	private ArrayList<RadioButton> 	lightButton;
	private ArrayList<EditText> 	lightPos;
	private ArrayList<EditText> 	lightDir;
	private	ArrayList<TextView>		dirLabel;
	private	ArrayList<TextView>		posLabel;
	
	private SeekBar	yBar;
	
	private ImageView map;
	
	private int selectedLight;
	
	 @Override 
	    public void onCreate(Bundle icicle) {
	        super.onCreate(icicle);
	    }
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
			Activity act = getActivity();
		
			selectedLight = 0;
			
			lights = new ArrayList<SceneGLLight>();
			lightButton = new ArrayList<RadioButton>();
			lightPos = new ArrayList<EditText>();
			lightDir = new ArrayList<EditText>();
			dirLabel = new ArrayList<TextView>();
			posLabel = new ArrayList<TextView>();
			
			for(int i = 0 ; i<4 ;i++){
				lights.add(new SceneGLLight(act,i));
			}
		
			View rootView = inflater.inflate(R.layout.scene_fragment_gl_configurator,container,false);
		 
			 RelativeLayout lightParams = (RelativeLayout)rootView.findViewById(R.id.scene_light_params);
			 
			yBar =(SeekBar) lightParams.findViewById(R.id.scene_light_z_bar);
			SetYBar();
		    yBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {
					
				}
				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {				
					
				}
				
				@Override
				public void onProgressChanged(SeekBar seekBar, int progress,boolean fromUser) {
					 lights.get(selectedLight).SetPosY((progress -50) * 20);
					 SetTextPos();
				}
			});
			
		   
		    
			lightButton.add( (RadioButton) rootView.findViewById(R.id.scene_light0) );
			lightButton.add( (RadioButton) rootView.findViewById(R.id.scene_light1) );
			lightButton.add( (RadioButton) rootView.findViewById(R.id.scene_light2) );
			lightButton.add( (RadioButton) rootView.findViewById(R.id.scene_light3) );
			
			lightPos.add( (EditText) lightParams.findViewById(R.id.scene_light_pos_x));
			lightPos.add( (EditText) lightParams.findViewById(R.id.scene_light_pos_y));
			lightPos.add( (EditText) lightParams.findViewById(R.id.scene_light_pos_z));
			
			posLabel.add( (TextView) lightParams.findViewById(R.id.scene_light_pos_x_label));
			posLabel.add( (TextView) lightParams.findViewById(R.id.scene_light_pos_y_label));
			posLabel.add( (TextView) lightParams.findViewById(R.id.scene_light_pos_z_label));
			
			lightDir.add( (EditText) lightParams.findViewById(R.id.scene_light_dir_x));
			lightDir.add( (EditText) lightParams.findViewById(R.id.scene_light_dir_y));
			lightDir.add( (EditText) lightParams.findViewById(R.id.scene_light_dir_z));
			
			dirLabel.add( (TextView) lightParams.findViewById(R.id.scene_light_dir_x_label));
			dirLabel.add( (TextView) lightParams.findViewById(R.id.scene_light_dir_y_label));
			dirLabel.add( (TextView) lightParams.findViewById(R.id.scene_light_dir_z_label));
			
			// EnableDirText(false);
			// EnablePosText(false); 
			
			OnKeyListener keyListener  = new OnKeyListener() {
				public boolean onKey(View v, int keyCode, KeyEvent event) {
					if ((event.getAction() == KeyEvent.ACTION_DOWN)
							&& (keyCode == KeyEvent.KEYCODE_ENTER)){
						Log.v(TAG,"NEW Value set up " + ((TextView)v).getText());
						float x = Float.parseFloat((lightPos.get(X).getText().toString()));
						float y = Float.parseFloat((lightPos.get(Y).getText().toString()));
						float z = Float.parseFloat((lightPos.get(Z).getText().toString()));
						
						Log.v(TAG,"get X " + String.valueOf(x));
						Log.v(TAG,"get Y " + String.valueOf(y));
						Log.v(TAG,"get Z " + String.valueOf(z));
						
						SetPos();
						SetYBar();
						lights.get(selectedLight).SetImagePos();
						//SetPos();
					//	SetDir();
						//SetTextPos();
						//SetTextDir();
					}
					return false;
				}
			};
			
			SetTextPos();
			SetTextDir();
			
			for(EditText et : lightPos) {
				et.setOnKeyListener(keyListener);
			}
			
			
			for(RadioButton rb : lightButton) {
				rb.setOnClickListener(this);
			}
			
			map = (ImageView) rootView.findViewById(R.id.scene_map);
			
		 
		 	FrameLayout map_frame = (FrameLayout) rootView.findViewById(R.id.scene_map_frame);
		 	
		 	for(SceneGLLight iv : lights) {
		 		map_frame.addView(iv);
		 	}
		 
		 	
		 	Spinner spinner = (Spinner) lightParams.findViewById(R.id.scene_light_type);		
			 ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(getActivity(),
			         R.array.light_type, android.R.layout.simple_spinner_item);			
			 adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);			
			 spinner.setAdapter(adapter);
		 	spinner.setOnItemSelectedListener( new  OnItemSelectedListener() {

				@Override
				public void onItemSelected(AdapterView<?> parent, View v,
						int position, long id) {
					lights.get(selectedLight).SetType(position);
					Log.v(TAG, "Light Type set to " + String.valueOf(position));
					GL2JNILib.SetLightType(lights.get(selectedLight).GetId(), position);
				}

				@Override
				public void onNothingSelected(AdapterView<?> parent) {
					// TODO Auto-generated method stub
					
				}
		 		
		 	});
			 
			 
		 map_frame.setOnTouchListener(new View.OnTouchListener() {
			 @Override
			 public boolean onTouch(View v, MotionEvent e) {
				 float x = 0.f,z = 0.f;
				 Log.v(TAG,"map_frame get click at X " + String.valueOf(e.getX()) + " Y " + String.valueOf(e.getY()));
				 Log.v(TAG,"map_frame WIDTH " + String.valueOf(v.getWidth()) + " HEIGHT " + String.valueOf(v.getHeight()));
				 Log.v(TAG,"map_frame X " + String.valueOf(v.getX()) + " Y " + String.valueOf(v.getY()));
				 
				 float x_screen  = e.getX();
				 float y_screen  = e.getY();
				 x = ChangeBaseX(x_screen);
			 	 z = ChangeBaseY(y_screen);
				 
				 switch(e.getAction()) {
				 	case MotionEvent.ACTION_MOVE :
				 		x = ChangeBaseX(e.getX());
				 		z = ChangeBaseY(e.getY());
				 		Log.v(TAG,"ACTION_MOVE");
					break;
				 	case MotionEvent.ACTION_DOWN :
				 		x = ChangeBaseX(e.getX());
				 		z = ChangeBaseY(e.getY());
				 		Log.v(TAG,"ACTION_DOWN");
				 	break;
				 	default :
				 		x = ChangeBaseX(e.getX());
				 		z = ChangeBaseY(e.getY());
				 }
				 
				
				 
				 lights.get(selectedLight).SetPosX(x);
				 lights.get(selectedLight).SetPosZ(z);
				 GL2JNILib.SetLightPos(lights.get(selectedLight).GetId(), x, lights.get(selectedLight).GetY(), z);
				 SetTextPos();
				 SetTextDir();
				 //lights.get(selectedLight).SetImagePos();
				 lights.get(selectedLight).setX(x_screen);
				 lights.get(selectedLight).setY(y_screen);
				 return false;
			 }
		 });
		 
		 return rootView;
	}
	
	private float x_max = 1280.f;
	private float y_max = 800.f;
	public static float Y_cam = 300.f;
	private double Y_max = 2.f * Y_cam * Math.tan(20.f*Math.PI/(180.f));
	private double X_max = x_max/y_max * Y_max;
	
	public void ComputeBaseParameter() {
		Y_max = 2.f * Y_cam * Math.tan(20.f*Math.PI/(180.f));
	    X_max = x_max/y_max * Y_max;
	}
	
	public float ChangeBaseX(float x) {
		ComputeBaseParameter();
		return (float)((x/x_max - 0.458f) * X_max);
	}
	
	public float ChangeBaseY(float y) {
		ComputeBaseParameter();
		return (float)((y/y_max - 0.458f) * Y_max);
	}
	
	public static float ChangeInverseBaseX(float X) {
		float cam_y = 300.f;
		double X_max = 2.f * cam_y * Math.tan(20.f*Math.PI/(180.f));
		return (float)(X * 1280.f/X_max + 640.f);
	}
	
	public static float ChangeInverseBaseY(float Y) {
		float cam_y = 300.f;
		double Y_max = 2.f * cam_y * Math.tan(20.f*Math.PI/(180.f));
		return (float)( Y * 800.f/Y_max + 400.f);
	}
	
	ImageView GetMap() {
		return map;
	}
	
	 @Override
	    public void onClick(View v) {
		 Log.v(TAG, "GLConfigurator responds!");
	        switch (v.getId()) {
	        case R.id.scene_light0 :
	        	selectedLight = 0;
	        	SetTextPos();
	        	SetYBar();
	            break;
	        case R.id.scene_light1 :
	        	selectedLight = 1;
	        	SetTextPos();
	        	SetYBar();
	            break;
	        case R.id.scene_light2 :
	        	selectedLight = 2;
	        	SetTextPos();
	        	SetYBar();
	            break;
	        case R.id.scene_light3 :
	        	selectedLight = 3;
	        	SetTextPos();
	        	SetYBar();
	            break;
	        }
	    }

	 
	 
	 
	 public void SetYBar() {
		 int progress = (int) ((lights.get(selectedLight).GetY()/5.f)+50.f);
		 Log.v(TAG,"SetYbar to " + String.valueOf(progress));
		 yBar.setProgress(progress);
	 }
	 
	public void SetTextPos(){
		lightPos.get(X).setText(String.valueOf(lights.get(selectedLight).GetX()));
		lightPos.get(Y).setText(String.valueOf(lights.get(selectedLight).GetY()));
		lightPos.get(Z).setText(String.valueOf(lights.get(selectedLight).GetZ()));
	}
	 
	public void SetTextDir(){
		lightDir.get(X).setText(String.valueOf(lights.get(selectedLight).GetDirX()));
		lightDir.get(Y).setText(String.valueOf(lights.get(selectedLight).GetDirY()));
		lightDir.get(Z).setText(String.valueOf(lights.get(selectedLight).GetDirZ()));
	}
	 
	 
	public void SetPos(){
		lights.get(selectedLight).SetPosX(Float.parseFloat((lightPos.get(X).getText().toString())));
		lights.get(selectedLight).SetPosY(Float.parseFloat((lightPos.get(Y).getText().toString())));
		lights.get(selectedLight).SetPosZ(Float.parseFloat((lightPos.get(Z).getText().toString())));
	}
	 
	public void SetDir(){
		lights.get(selectedLight).SetPosX(Float.parseFloat((lightDir.get(X).getText().toString())));
		lights.get(selectedLight).SetPosY(Float.parseFloat((lightDir.get(Y).getText().toString())));
		lights.get(selectedLight).SetPosZ(Float.parseFloat((lightDir.get(Z).getText().toString())));
	}
	 
	 
	 public void EnableDirText(boolean state) {
		 	
			lightDir.get(0).setEnabled(state);
			lightDir.get(1).setEnabled(state);
			lightDir.get(2).setEnabled(state);
			
			dirLabel.get(0).setEnabled(state);
			dirLabel.get(1).setEnabled(state);
			dirLabel.get(2).setEnabled(state);

	 }
	 
	 public void EnablePosText(boolean state) {
		 	
			lightPos.get(0).setEnabled(state);
			lightPos.get(1).setEnabled(state);
			lightPos.get(2).setEnabled(state);
			
			posLabel.get(0).setEnabled(state);
			posLabel.get(1).setEnabled(state);
			posLabel.get(2).setEnabled(state);

	 }
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	 
	 
	 
	 
}































