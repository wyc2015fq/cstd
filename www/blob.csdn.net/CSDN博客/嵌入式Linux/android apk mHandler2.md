
# android apk mHandler2 - 嵌入式Linux - CSDN博客

2014年06月25日 09:03:03[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：550个人分类：[Android																](https://blog.csdn.net/weiqifa0/article/category/1752017)



```python
package com.example.helloworld;
import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;
import android.os.Handler;
public class MainActivity extends ActionBarActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    	Message msg2 = new Message();
    	msg2.what = 1;
    	mHandler2.sendMessage(msg2);
        
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
    }
    private Handler mHandler2 = new Handler(){
    	public void handleMessage(Message msg){
    		int what = msg.what;
    		boolean down = (what==1?true:false);
    		Toast toast=Toast.makeText(getApplicationContext(), "今天天气很好"+down, Toast.LENGTH_SHORT); 
        	toast.show();
    		}
    	};

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
        public PlaceholderFragment() {
        }
        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container, false);
            return rootView;
        }
    }
}
```


