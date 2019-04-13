
# Android上层怎样读写proc节点（示例） - 嵌入式Linux - CSDN博客

2014年12月29日 15:52:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4771


import android.app.Activity;
import android.os.Bundle;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ToggleButton;
import android.os.SystemProperties;
import android.util.Log;
import android.widget.Toast; //wang
import java.util.Timer;
import java.util.TimerTask;
import android.os.PowerManager;
import java.io.*;
import android.os.Looper;


public class DebugOptionActivity extends Activity
{
private static final String TAG = "tpDebug";
private int checkValue = 0;

@Override
public void onCreate(Bundle savedInstanceState)
{
super.onCreate(savedInstanceState);
setContentView(R.layout.debug);
ToggleButton toggle = (ToggleButton)findViewById(R.id.toggle);
try{
File readFile = new File("proc/tp_debug/debug_switch");
FileReader inCmd = new FileReader(readFile);
try{
checkValue = inCmd.read();
if(checkValue != 0)
{
toggle.setChecked(true);
}
else
{
toggle.setChecked(false);
}
}catch (IOException e){
e.printStackTrace();
}
} catch (FileNotFoundException e){
e.printStackTrace();
}
//ToggleButton toggle = (ToggleButton)findViewById(R.id.toggle);
toggle.setOnCheckedChangeListener(new OnCheckedChangeListener()
{
public void onCheckedChanged(CompoundButton arg0, boolean arg1)
{
if (arg1)
{
//open
File awakeTimeFile = new File("proc/tp_debug/debug_switch");
FileWriter fr;
try {
fr = new FileWriter(awakeTimeFile);
fr.write("1");
fr.close();
}
catch (IOException e) {
e.printStackTrace();
}
}
else
{
//close
File awakeTimeFile = new File("proc/tp_debug/debug_switch");
FileWriter fr;
try
{
fr = new FileWriter(awakeTimeFile);
fr.write("0");
fr.close();
} catch (IOException e) {
e.printStackTrace();
}
}
}
});

}
}

