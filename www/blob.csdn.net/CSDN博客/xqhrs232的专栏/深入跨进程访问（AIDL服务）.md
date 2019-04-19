# 深入跨进程访问（AIDL服务） - xqhrs232的专栏 - CSDN博客
2012年02月27日 22:43:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：401标签：[string																[import																[button																[service																[class																[float](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cyqdata.com/cnblogs/article-detail-35231](http://www.cyqdata.com/cnblogs/article-detail-35231)
AIDL服务只支持有限的数据类型，因此，如果用AIDL服务传递一些复杂的数据就需要做更一步处理。AIDL服务支持的数据类型如下：
1.Java的简单类型（int、char、boolean等）。不需要导入（import）。
2.String和CharSequence。不需要导入（import）。
3.List和Map。但要注意， List和Map对象的元素类型必须是AIDL服务支持的数据类型。不需要导入（import）。
4.AIDL自动生成的接口。需要导入（import）。
5.实现android.os.Parcelable接口的类。需要导入（import）。
以下是需要import进行导入的操作。
Step 1:
先建立一个IMyService.aidl文件：
```
package org.aidl.service;
import org.aidl.service.Product;
interface IMyService{
    //in 表示这个值被客户端设置；out 表示这个值被服务器设置；inout表示这个值既能被客户端设置，又能被服务器设置。
    Map getMap(in String country, in Product product);
    Product getProduct();
}
```
Step 2:
编写Product类：
```
package org.aidl.service;
import android.os.Parcel;
import android.os.Parcelable;
public class Product implements Parcelable{
    private int id;
    private String name;
    private float price;
    
    public static final Parcelable.Creator<Product> CREATOR = new Parcelable.Creator<Product>() {
        @Override
        public Product createFromParcel(Parcel in) {
            return new Product(in);
        }
        @Override
        public Product[] newArray(int size) {
            return new Product[size];
        }
    };
    
    public Product(){
        
    }
    
    public Product(Parcel in){
        readFromParcel(in);
    }
    
    public void readFromParcel(Parcel in) {
        id = in.readInt();
        name = in.readString();
        price = in.readFloat();
    }
    
    @Override
    public void writeToParcel(Parcel dest, int flags){
        dest.writeInt(id);
        dest.writeString(name);
        dest.writeFloat(price);
    }
    
    @Override
    public int describeContents() {
        return 0;
    }
    
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public float getPrice() {
        return price;
    }
    public void setPrice(float price) {
        this.price = price;
    }
    public static Parcelable.Creator<Product> getCreator() {
        return CREATOR;
    }
}
```
Step 3:
建立一个Product.aidl文件，并输入以下内容：
```
parcelable Product;
```
Step 4:
编写一个MyService类：
```
package org.aidl.service;
import java.util.HashMap;
import java.util.Map;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
public class MyService extends Service {
    
    public class MyServiceImpl extends IMyService.Stub{
        @Override
        public Map getMap(String country, Product product)
                throws RemoteException {
            Map map = new HashMap<String, String>();
            map.put("country", country);
            map.put("id", product.getId());
            map.put("name", product.getName());
            map.put("price", product);
            return map;
        }
        @Override
        public Product getProduct() throws RemoteException {
            Product product = new Product();
            product.setId(1234);
            product.setName("汽车");
            product.setPrice(31000);
            return product;
        }
        
        
    }
    
    @Override
    public IBinder onBind(Intent arg0) {
        return new MyServiceImpl();
    }
}
```
Step 5:
在AndroidManifest.xml文件中配置MyService类，代码如下：
```
<service android:name=".MyService">
        <intent-filter>
            <action android:name="org.aidl.service.IMyService" />
        </intent-filter>
    </service>
```
上面是server端，以下是client端：
首先，你应该把IMyService.java和Product.java文件复制到客户端工程，IMyService.java文件不能直接拷，你应该先在client项目中建一个存放IMyService.aidl跟server端一样的包，然后把IMyService.aidl文件拷到那个包里，这样程序会自动给你创建一个IMyService.java文件。
现在可以在Activity里创建并调用服务了，代码如下：
```
package org.aidl.client;
import org.aidl.service.IMyService;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
public class Main extends Activity implements OnClickListener{
    private IMyService myService = null;
    private Button btnInvokeAIDLService;
    private Button btnBindAIDLService;
    private TextView textView;
    private ServiceConnection serviceConnection = new ServiceConnection()
    {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service)
        {
            myService = IMyService.Stub.asInterface(service);
            btnInvokeAIDLService.setEnabled(true);
        }
        @Override
        public void onServiceDisconnected(ComponentName name)
        {
        }
    };
    public void onClick(View view)
    {
        switch (view.getId())
        {
            case R.id.btnBindAIDLService:
                bindService(new Intent("org.aidl.service.IMyService"),
                        serviceConnection, Context.BIND_AUTO_CREATE);
                break;
            case R.id.btnInvokeAIDLService:
                try
                {
                    String s = "";
                    s = "Product.id = " + myService.getProduct().getId() + "\n";
                    s += "Product.name = " + myService.getProduct().getName()
                            + "\n";
                    s += "Product.price = " + myService.getProduct().getPrice()
                            + "\n";
                    
                    s += myService.getMap("China", myService.getProduct()).toString();
                    textView.setText(s);
                }
                catch (Exception e)
                {
                }
                break;
        }
    }
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        btnInvokeAIDLService = (Button) findViewById(R.id.btnInvokeAIDLService);
        btnBindAIDLService = (Button) findViewById(R.id.btnBindAIDLService);
        btnInvokeAIDLService.setEnabled(false);
        textView = (TextView) findViewById(R.id.textview);
        btnInvokeAIDLService.setOnClickListener(this);
        btnBindAIDLService.setOnClickListener(this);
    }
}
```
好了，你在运行的时候，先绑定AIDL服务，然后再调用服务，这样的话，你就会发现一些从服务器端读的数据。就写到这吧！晚安 for me!
![](http://www.cnblogs.com/juniorblanhan/aggbug/1990907.html?type=1)
