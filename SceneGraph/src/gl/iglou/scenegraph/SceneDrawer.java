package gl.iglou.scenegraph;

import android.content.Context;
import android.support.v4.widget.DrawerLayout;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;

public class SceneDrawer extends DrawerLayout {

	final public String TAG ="SCENE_DRAWER";
	
	private int drawerStartId;
	private int drawerEndId;
	
	public SceneDrawer(Context context, AttributeSet attrs) {
		super(context,attrs);
		
	}
	
	void SetDrawerId(int left, int right) {
		
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent ev) {
		Log.v(TAG,"Drawer is reponsding!!");
		if(isDrawerOpen(Gravity.END)){
			closeDrawer(Gravity.END);
		}
		
		return super.onTouchEvent(ev);
	}
}
