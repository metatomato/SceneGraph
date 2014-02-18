package gl.iglou.scenegraph;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.Checkable;
import android.widget.RelativeLayout;

public class CheckableRelativeLayout extends RelativeLayout implements Checkable {
	private boolean checked;
	
	public CheckableRelativeLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
		checked = false;
	}
	
	@Override
	public boolean isChecked() {
		return checked;
	}
	
	@Override
	public void setChecked(boolean checked_state) {
		this.checked = checked_state;
	}
	
	@Override
	public void toggle() {
		if (checked) {
			checked = false;
		}else {
			checked = true;
		}
	}
}


