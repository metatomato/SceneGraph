package gl.iglou.scenegraph;

import android.app.Application;
import android.content.res.Resources;
import android.util.DisplayMetrics;
import android.util.Log;

public class SceneGraph extends Application {

	static private int x_screen = 0;
	static private int y_screen = 0;
	static private float density = 1.f;
	static private int navigationBarSize = 0;
	
	
	private static DisplayMetrics displayParams;
	
	public SceneGraph(){
		
	}
	
	public void onCreate(){
		super.onCreate();
		Log.v("TOMATO","SAYS HELLO FROM SCENEGRAPH CLASS!!");
		//Init Singleton Classes
		initSingleton();
		
		displayParams = this.getApplicationContext().getResources().getDisplayMetrics(); 
		int densityDpi = displayParams.densityDpi;
		density = displayParams.density;
		
		x_screen  = displayParams.widthPixels;
		y_screen  = displayParams.heightPixels;
        
        Log.v("SCENEGRAPH","X: "+ String.valueOf(x_screen)+" 	Y: "+ String.valueOf(y_screen) + "\n" +
        					"SCREEN DPI : 	  " + String.valueOf(densityDpi) + "\n" +
							"SCREEN DENSITY : " + String.valueOf(density) );
		
        
        
        int currentapiVersion = android.os.Build.VERSION.SDK_INT;
        if (currentapiVersion >= android.os.Build.VERSION_CODES.KITKAT){
        	Resources resources = this.getApplicationContext().getResources();
            int resourceId = resources.getIdentifier("navigation_bar_height", "dimen", "android");
            if (resourceId > 0) {
            	navigationBarSize = resources.getDimensionPixelSize(resourceId);
            	Log.v("SCENEGRAPH","KITKAT DETECTED! Navigation Bar Height " + String.valueOf(navigationBarSize));
            	y_screen += navigationBarSize;
            }
        }
        
        
	}
	
	static public int getXscreen() {
		return x_screen;
	}
	
	static public int getYscreen() {
		return y_screen;
	}
	
	public static float getDensity() {
		return density;
	}

	public static void setDensity(float density) {
		SceneGraph.density = density;
	}

	
	public static int getNavigationBarSize() {
		return navigationBarSize;
	}

	public static void setNavigationBarSize(int navigationBarSize) {
		SceneGraph.navigationBarSize = navigationBarSize;
	}

	public void initSingleton(){
		
		SharedData.InitInstance(this);
	}
	
	public static DisplayMetrics getMetrics(){
		return displayParams;
	}
	
}

