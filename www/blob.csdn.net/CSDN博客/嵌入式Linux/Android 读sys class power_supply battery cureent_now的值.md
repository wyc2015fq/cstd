
# Android 读sys-class-power_supply-battery-cureent_now的值 - 嵌入式Linux - CSDN博客

2015年02月03日 16:10:34[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4298



```python
package com.example.readsysclass;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
 Button button01;
 TextView text01;
 private int TIME = 2000;  
 static final String TAG="readsysclass";
  private static final String CHARGER_CURRENT_NOW =
             "/sys/class/power_supply/battery/current_now";
  /**
      * Read a text file into a String, optionally limiting the length.
      *
      * @param file     to read (will not seek, so things like /proc files are OK)
      * @param max      length (positive for head, negative of tail, 0 for no limit)
      * @param ellipsis to add of the file was truncated (can be null)
      * @return the contents of the file, possibly truncated
      * @throws IOException if something goes wrong reading the file
      */
     public static String readTextFile(File file, int max, String ellipsis) throws IOException {
         InputStream input = new FileInputStream(file);
         try {
             long size = file.length();
             if (max > 0 || (size > 0 && max == 0)) {  // "head" mode: read the first N bytes
                 if (size > 0 && (max == 0 || size < max)) max = (int) size;
                 byte[] data = new byte[max + 1];
                 int length = input.read(data);
                 if (length <= 0) return "";
                 if (length <= max) return new String(data, 0, length);
                 if (ellipsis == null) return new String(data, 0, max);
                 return new String(data, 0, max) + ellipsis;
             } else if (max < 0) {  // "tail" mode: keep the last N
                 int len;
                 boolean rolled = false;
                 byte[] last = null, data = null;
                 do {
                     if (last != null) rolled = true;
                     byte[] tmp = last;
                     last = data;
                     data = tmp;
                     if (data == null) data = new byte[-max];
                     len = input.read(data);
                 } while (len == data.length);
                 if (last == null && len <= 0) return "";
                 if (last == null) return new String(data, 0, len);
                 if (len > 0) {
                     rolled = true;
                     System.arraycopy(last, len, last, 0, last.length - len);
                     System.arraycopy(data, 0, last, last.length - len, len);
                 }
                 if (ellipsis == null || !rolled) return new String(last);
                 return ellipsis + new String(last);
             } else {  // "cat" mode: size unknown, read it all in streaming fashion
                 ByteArrayOutputStream contents = new ByteArrayOutputStream();
                 int len;
                 byte[] data = new byte[1024];
                 do {
                     len = input.read(data);
                     if (len > 0) contents.write(data, 0, len);
                 } while (len == data.length);
                 return contents.toString();
             }
         } finally {
             input.close();
         }
     }
 @Override
 protected void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);
  setContentView(R.layout.activity_main);
  button01 = (Button)findViewById(R.id.button01);
  text01 = (TextView)findViewById(R.id.text01);
  
  
  button01.setOnClickListener(new OnClickListener() {
   
   public void onClick(View arg0) {
    // TODO Auto-generated method stub
    handler.postDelayed(runnable, TIME); //每隔1s执行 
   }
  });
 }
 Handler handler = new Handler();  
    Runnable runnable = new Runnable() {
  @Override
  public void run() {
   // TODO Auto-generated method stub
   handler.postDelayed(this, TIME);  
   try {
    String mode1 = readTextFile(new File(CHARGER_CURRENT_NOW), 0, null).trim();
    text01.setText(mode1);
    Log.e(TAG,mode1);
   } catch (IOException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
   }
  }  
    };
}
```


