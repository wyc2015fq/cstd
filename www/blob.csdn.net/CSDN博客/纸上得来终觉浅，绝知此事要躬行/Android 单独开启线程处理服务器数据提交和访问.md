# Android 单独开启线程处理服务器数据提交和访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年11月15日 23:23:09[boonya](https://me.csdn.net/boonya)阅读数：4701








问题描述：在模拟器上Android应用成功访问本地局域网网络，所有功能正常，于是换到真机上测试结果只能登录成功，后续的操作都是失败的。起初怀疑是否是登录cookie的问题，但是在模拟机上都可以说明不是cookie的问题，真机浏览器可以访问局域网应用。最后想到登录是new了线程的，登录没有问题，但是后面执行网络访问无论如何都访问不到数据，就是因为没有开启新的线程。

问题原因：Android在与服务器实现数据访问和处理的时候，直接在UI线程内部处理是不允许的，这样会导致请求失败，推测这与Android的网络访问机制有关。

处理方法：开启一个新线程（Thread）来处理UI主线程需要访问和处理的服务器数据。



```java
package com.boonya.wtms.activities;

import java.io.Serializable;
import java.util.List;
import com.boonya.baidumap.R;
import com.boonya.wtms.pojo.DrillingTaskPojo;
import com.boonya.wtms.service.DrillingTaskService;
import com.boonya.wtms.utils.Constant;
import com.boonya.wtms.utils.ShowToast;
import com.boonya.wtms.utils.SpinnerSelectedListener;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

public class DrillingSearchingActivity extends Activity
{

    private TextView taskProcessStatusV;

    private Button searchButton;

    private Spinner processSpinner;

    private ArrayAdapter<String> processAdapter;

    private EditText measureLineNo;

    private EditText wellsNo;

    private DrillingTaskService drillingTaskService;

    Context tag = DrillingSearchingActivity.this;

    private ShowToast showToast;

    private List<DrillingTaskPojo> taskPojos;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.app_drilling_searching);

        drillingTaskService = new DrillingTaskService(tag);

        searchButton = (Button) findViewById(R.id.searchingBtn);

        showToast = new ShowToast(tag);

        taskProcessStatusV = (TextView) findViewById(R.id.taskProcessStatusV);

        measureLineNo = (EditText) findViewById(R.id.field_measureline_no);
        wellsNo = (EditText) findViewById(R.id.field_wells_no);

        // 下拉框数据填充
        processSpinner = (Spinner) findViewById(R.id.taskProcessStatus);
        // 将可选内容与ArrayAdapter连接起来
        processAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, Constant.spinnerProcessV);
        // 设置下拉列表的风格
        processAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // 将adapter 添加到spinner中
        processSpinner.setAdapter(processAdapter);
        // 添加事件Spinner事件监听
        processSpinner.setOnItemSelectedListener(new SpinnerSelectedListener(taskProcessStatusV, Constant.spinnerProcessV));
        // 设置默认值
        processSpinner.setVisibility(View.VISIBLE);

        searchButton.setOnClickListener(new OnClickListener()
        {

            @Override
            public void onClick(View arg0)
            {
                Thread serviceThread = new Thread(new ServiceThread());
                serviceThread.start();
                try
                {
                    // 等待线程将数据查询完成
                    Thread.sleep(2000);
                    // 查询到数据做相应处理
                    if (taskPojos != null && taskPojos.size() > 0)
                    {
                        Intent intent = new Intent();
                        intent.putExtra("tasks", (Serializable) taskPojos);
                        intent.setClass(DrillingSearchingActivity.this, DrillingSearchingListActivity.class);
                        startActivity(intent);
                    } else
                    {
                        showToast.showToast("未查询到任何钻井任务记录");
                    }
                } catch (InterruptedException e)
                {
                    Log.d("log", "ServiceThread task search exception:"+e.getMessage());
                }

            }
        });

    }

    /**
     * 自定义类：ServiceThread线程类(内部类)
     */
    class ServiceThread implements Runnable
    {
        @Override
        public void run()
        {
            int processStatus = -1;
            
            String measureLineNoVal = measureLineNo.getText().toString();
            
            String wellsNoVal = wellsNo.getText().toString();
            
            processStatus = processSpinner.getSelectedItemPosition() - 1;
            
            taskPojos = drillingTaskService.findTasksFromServer(measureLineNoVal, wellsNoVal, processStatus);
            
            Log.d("log", "钻井查询，共查询到记录:" + (taskPojos == null ? 0 : taskPojos.size()) + "条.");

        }
    }
}
```


注：ServiceThread 中run函数的代码如果直接放在searchButton的OnclickListener的OnClick方法内部执行是不成功的。




