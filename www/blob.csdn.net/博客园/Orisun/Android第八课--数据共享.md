# Android第八课--数据共享 - Orisun - 博客园







# [Android第八课--数据共享](https://www.cnblogs.com/zhangchaoyang/articles/1959833.html)





共享preference可在一个应用程序的各个组件之间共享。

在MainActivity里面创建并保存preference:

package zcy.org;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {
	public static final String MYPREFS="mySharedPreference";
	EditText et;
	Button bt;
	float f;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        et=(EditText)findViewById(R.id.myEditText);  
        bt=(Button)findViewById(R.id.myButton);
        bt.setOnClickListener(new OnClickListener(){
			public void onClick(View arg0) {
				Intent intent=new Intent(MainActivity.this,SecActivity.class);
				savePreference();
				startActivity(intent);
			}
        });
    }
    
    protected void savePreference(){
    	int mode=Activity.MODE_PRIVATE;
    	//创建或检索共享的preference对象
    	SharedPreferences mySharedPreference=getSharedPreferences(MYPREFS,mode);
    	//找一个编辑器来修改共享的preference
    	SharedPreferences.Editor editor=mySharedPreference.edit();
    	//在共享的preference对象中存储新的原语类型
    	editor.putBoolean("isTrue",true);
    	f=Float.parseFloat(et.getText().toString());
    	editor.putFloat("lastFloat",f);
    	//提交更改
    	editor.commit();
    }
}


在SecActivity里面读取preference

package zcy.org;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.TextView;

public class SecActivity extends Activity{
	private TextView tv;
	boolean isTrue;
	float lastFloat;
	@Override
	public void onCreate(Bundle outState){
		super.onCreate(outState);
		setContentView(R.layout.main2);
		tv=(TextView)findViewById(R.id.tv);
		loadPreference();
		if(isTrue){
			tv.setText(Float.toString(lastFloat));
		}
	}
	public void loadPreference(){
		int mode=Activity.MODE_PRIVATE;
		//获取存储的preference
		SharedPreferences mySharedPreference=getSharedPreferences(MainActivity.MYPREFS,mode);
		//检索保存的值
		isTrue=mySharedPreference.getBoolean("isTrue",false);
		lastFloat=mySharedPreference.getFloat("lastFloat",0f);
	}

}


![](https://pic002.cnblogs.com/images/2011/103496/2011022116091868.png)![](https://pic002.cnblogs.com/images/2011/103496/2011022116092646.png)

以上讲的方法可以在一个应用程序的不同Activity之间共享变量，如果只是想保存一个活动的状态，而不需要和其他活动共享，则可以使用Activity.getPreference()。

getPreferences()只需要一个mode参数，而getSharedPreferences()还需要另外指定名称。

package zcy.org;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {
	EditText et;
	Button bt;
	float f;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        et=(EditText)findViewById(R.id.myEditText);  
        //获取存储的preference
		SharedPreferences mySharedPreference=getPreferences(Activity.MODE_PRIVATE);
		//检索保存的值
		f=mySharedPreference.getFloat("lastFloat",0f);
		et.setText(Float.toString(f));
        bt=(Button)findViewById(R.id.myButton);
        bt.setOnClickListener(new OnClickListener(){
			public void onClick(View arg0) {
				Intent intent=new Intent(MainActivity.this,SecActivity.class);
				//在活动跳转之前保存本活动的preference
				savePreference();
				startActivity(intent);
			}
        });
    }
    
    protected void savePreference(){
    	int mode=Activity.MODE_PRIVATE;
    	//创建或检索活动的preference对象
    	SharedPreferences mySharedPreference=getPreferences(mode);
    	//找一个编辑器来修改共享的preference
    	SharedPreferences.Editor editor=mySharedPreference.edit();
    	//在共享的preference对象中存储新的原语类型
    	f=Float.parseFloat(et.getText().toString());
    	editor.putFloat("lastFloat",f);
    	//提交更改
    	editor.commit();
    }
}


当从其他活动跳转回MainActivity时，EditText中的值是之前输入的值。

通过重写onSaveInstanceState来保存活动实例的值,把修改过的Bundle传递给超类的处理函数。

当活动已完成它的生命周期但还没有显示的结束时，就会触发

onSaveInstanceState这个函数。这样就可以保证在用户一次会话的生命周期中保持一致的活动状态。

如果活动被强制重启，那么已保存的Bundle就会被传递到onRestoreInstanceState和onCreate方法中。

注意只有当活动变为不活动状态的时候才会调用onRestoreInstanceState，而当活动通过调用finish或用户按返回键而被关闭时是不会调用onRestoreInstanceState的。

package zcy.org;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class SecActivity extends Activity{
	private Button bt;
	EditText tv;
	private static final String TEXTVIEW_STATE_KEY="TEXTVIEW_STATE_KEY";
	@Override
	public void onSaveInstanceState(Bundle outState){
		//检索View
		tv=(EditText)findViewById(R.id.sectv);
		//保存View的状态
		outState.putString(TEXTVIEW_STATE_KEY,tv.getText().toString());
		super.onSaveInstanceState(outState);
	}
	@Override
	public void onCreate(Bundle icicle){
		super.onCreate(icicle);
		setContentView(R.layout.main2);
		tv=(EditText)findViewById(R.id.sectv);
		String text="";
		if(icicle!=null&&icicle.containsKey(TEXTVIEW_STATE_KEY))
			text=icicle.getString(TEXTVIEW_STATE_KEY);
		tv.setText(text);
		bt=(Button)findViewById(R.id.bt);
        bt.setOnClickListener(new OnClickListener(){
			public void onClick(View arg0) {
				Intent intent=new Intent(SecActivity.this,MainActivity.class);
				startActivity(intent);
			}
        });
	}
}

![](https://pic002.cnblogs.com/images/2011/103496/2011022116404312.png)
在SecActivity中输入“zcy”，如果紧接着重启应用程序，则直接进入
SecActivity页面，并且EditText中还显示"zcy"。

而如果在

SecActivity中按back按钮（

 这时SecActivity就调用了finish()）回到MainActivity，再从

MainActivity进入

SecActivity时，EditText中就为空了。

文件

除了标准的Java I/O类外，Android提供openFileInput和openFileOutput以简化对本地文件流的读写。这些方法只支持当前文件夹下的文件，指定文件时给出路径会抛出异常。

String FILE_NAME="tempfile";

FileOutputStream fos=openFileOutput(FILE_NAME,Context.MODE_PRIVATE);

FileInputStream fis=openFileInput(FILE_NAME);

Context.MODE_PRIVATE指明文件为当前应用程序私有,文件不存在时会创建，存在时会覆盖;

Context.MODE_APPEND追加的形式;

Context.MODE_WORLD_READABLE,Context.MODE_WORLD_WRITEABLE其他程序可访问。
将静态文件作为资源：

把外部文件放在res/raw下面

Resource myResource=getResources();

InputStream myFile=myResource.openRawResource(R.raw.filename);


Android还提供deleteFile(删除当前应用程序创建的文件)和fileList(返回当前应用程序创建的文件列表的字符串数组)。












