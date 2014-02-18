package gl.iglou.scenegraph;


import android.content.Context;
import android.util.Log;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageView;


public class SceneGLLight extends ImageView {

	final public String TAG = "SCENE_GL_LIGHT";
	
	static float ratio = 0.f;
	
	private SharedData.SceneLight lights;
	
	
	public SceneGLLight(Context context, int idx) {
		super(context);
		
		lights = SharedData.GetLight().valueAt(idx);
		
		this.setImageResource(R.drawable.ic_maps_indicator_current_position);
	 	this.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT));
	 	
	 	SetImagePos();
	}
	
	public int GetId() {
		return lights.id;
	}
	
	public int GetType() {
		return lights.type.ordinal();
	}
	
	public void SetType(int type) {
		lights.type = SharedData.SceneLight.LIGHT_TYPE.values()[type];
	}
	
	public void SetPosX(float value) {
		lights.pos[0] = value;
		UpdateGLLight();
	}
	public void SetPosY(float value) {
		lights.pos[1] = value;
		UpdateGLLight();
	}
	public void SetPosZ(float value) {
		lights.pos[2] = value;
		UpdateGLLight();
	}
	
	public void SetDirX(float value) {
		lights.dir[0] = value;
	}
	public void SetDirY(float value) {
		lights.dir[1] = value;
	}
	public void SetDirZ(float value) {
		lights.dir[2] = value;
	}
	
	public float GetX() {
		return lights.pos[0];
	}
	public float GetY() {
		return lights.pos[1];
	}
	public float GetZ() {
		return lights.pos[2];
	}
	
	public float GetDirX() {
		return lights.dir[0];
	}
	public float GetDirY() {
		return lights.dir[1];
	}
	public float GetDirZ() {
		return lights.dir[2];
	}
	
	public void UpdateGLLight(){
		GL2JNILib.SetLightPos(lights.id, lights.pos[0], lights.pos[1], lights.pos[2]);
	}
	
	public void SetImagePos() {
		setX(SceneGLConfiguratorFragment.ChangeInverseBaseX(lights.pos[0]));
	 	setY(SceneGLConfiguratorFragment.ChangeInverseBaseY(lights.pos[2]));
	}
}
































