# Android中使用SQLiteOpenHelper管理SD卡中的数据库 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年08月21日 15:10:10[boonya](https://me.csdn.net/boonya)阅读数：1179标签：[android																[sqlite																[api																[class																[database](https://so.csdn.net/so/search/s.do?q=database&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)





原文地址：[http://www.cr173.com/html/23317_1.html](http://www.cr173.com/html/23317_1.html)

使用Android中自带的SQLiteOpenHelper可以完成[数据库](http://www.cr173.com/k/sql/)的创建与管理，但有两点局限：



（1）数据库创建在内存卡中，大小受限，创建位置位于/data/data/应用程序名/databases中（可使用Eclispe的DDMS查看）。

（2）如果无法获取Root权限，则无法直接查看创建的数据库。

 鉴于上述限制及实际需要，打算使用SQLiteOpenHelper管理SD卡上的数据库，通过研究SQLiteOpenHelper的源码，发现其创建或打开数据库的代码位于getWritableDatabase()函数中(getReadableDatabase本身也是调用了getWritableDatabase)：

```java
<span style="font-size:12px;"> if (mName == null) {
     db = SQLiteDatabase.create(null);
     } 
else {
     db = mContext.openOrCreateDatabase(mName, 0, mFactory);
     }</span>
```

分析上述代码发现，当数据库名字为非空时，创建数据库或打开由mContext完成，这个mContext由SQLiteOpenHelper的构造函数传入：SQLiteOpenHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version)。那么我们对于传入的context，重载其openOrCreateDatabase函数，使其将数据库创建到SD卡中就可完成我们的目标了~。

对应的SQLiteOpenHelper实现类SdCardDBHelper

```java
<span style="font-size:12px;">import android.content.Context;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

/**
 * 数据库管理和维护类
**/
public class SdCardDBHelper extends SQLiteOpenHelper{
    
    public static final String TAG = "SdCardDBHelper";
    /**
     * 数据库名称
    **/
    public static String DATABASE_NAME = "sddb.db";
    
    /**
     * 数据库版本
    **/
    public static int DATABASE_VERSION = 1;
        
    /**
     * 构造函数
     * 
     * @param    context 上下文环境
    **/
    public SdCardDBHelper(Context context){
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }    
     
    /**
     * 创建数据库时触发，创建离线存储所需要的数据库表
     *
     * @param    db
    **/
    @Override
    public void onCreate(SQLiteDatabase db) {
         Log.e(TAG, "开始创建数据库表");
        try{
            //创建用户表(user)
             db.execSQL("create table if not exists user" +
                     "(_id integer primary key autoincrement,name varchar(20),password varchar(20),role varchar(10),updateTime varchar(20))");
            Log.e(TAG, "创建离线所需数据库表成功");
        }
        catch(SQLException se){
            se.printStackTrace();
            Log.e(TAG, "创建离线所需数据库表失败");
        }        
    }
    
    /** 更新数据库时触发，
     *
     * @param    db
     * @param    oldVersion
     * @param    newVersion
    **/
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        //db.execSQL("ALTER TABLE person ADD COLUMN other STRING");  
     }
}</span>
```

**重载的openOrCreateDatabase在sd卡上创建数据库的Context**

```java
<span style="font-size:12px;">import java.io.File;
import java.io.IOException;
import android.content.Context;
import android.content.ContextWrapper;
import android.database.DatabaseErrorHandler;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.util.Log;

/**
 * 用于支持对存储在SD卡上的数据库的访问
**/
public class DatabaseContext extends ContextWrapper {        
           
        /**
         * 构造函数
         * @param    base 上下文环境
         */
        public DatabaseContext(Context base){
            super(base);
        }
     
        /**
         * 获得数据库路径，如果不存在，则创建对象对象
         * @param    name
         * @param    mode
         * @param    factory
         */
        @Override
        public File getDatabasePath(String name) {
            //判断是否存在sd卡
            boolean sdExist = android.os.Environment.MEDIA_MOUNTED.equals(android.os.Environment.getExternalStorageState());
            if(!sdExist){//如果不存在,
                Log.e("SD卡管理：", "SD卡不存在，请加载SD卡");
                return null;
            } 
            else{//如果存在
                //获取sd卡路径
                String dbDir=android.os.Environment.getExternalStorageDirectory().getAbsolutePath();
                dbDir += "/database";//数据库所在目录
                String dbPath = dbDir+"/"+name;//数据库路径
                //判断目录是否存在，不存在则创建该目录
                File dirFile = new File(dbDir);
                if(!dirFile.exists())
                    dirFile.mkdirs();
                
                //数据库文件是否创建成功
                boolean isFileCreateSuccess = false; 
                //判断文件是否存在，不存在则创建该文件
                File dbFile = new File(dbPath);
                if(!dbFile.exists()){
                    try {                    
                        isFileCreateSuccess = dbFile.createNewFile();//创建文件
                    } catch (IOException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }
                else     
                    isFileCreateSuccess = true;
                
                //返回数据库文件对象
                if(isFileCreateSuccess)
                    return dbFile;
                else 
                    return null;
            }
        }
     
        /**
         * 重载这个方法，是用来打开SD卡上的数据库的，android 2.3及以下会调用这个方法。
         * 
         * @param    name
         * @param    mode
         * @param    factory
         */
        @Override
        public SQLiteDatabase openOrCreateDatabase(String name, int mode, 
                SQLiteDatabase.CursorFactory factory) {
            SQLiteDatabase result = SQLiteDatabase.openOrCreateDatabase(getDatabasePath(name), null);
            return result;
        }
        
        /**
         * Android 4.0会调用此方法获取数据库。
         * 
         * @see android.content.ContextWrapper#openOrCreateDatabase(java.lang.String, int, 
         *              android.database.sqlite.SQLiteDatabase.CursorFactory,
         *              android.database.DatabaseErrorHandler)
         * @param    name
         * @param    mode
         * @param    factory
         * @param     errorHandler
         */
        @Override
        public SQLiteDatabase openOrCreateDatabase(String name, int mode, CursorFactory factory,
                DatabaseErrorHandler errorHandler) {
            SQLiteDatabase result = SQLiteDatabase.openOrCreateDatabase(getDatabasePath(name), null);
            return result;
        }
    } </span>
```

**调用程序：**

```java
<span style="font-size:12px;">DatabaseContext dbContext = new DatabaseContext(this);
SdCardDBHelper dbHelper = new SdCardDBHelper(dbContext);</span>
```

这里尤其值得注意的是，不同版本的android API会调用不同的openOrCreateDatabase函数。

当然也可直接使用SQLiteDatabase创建SD卡上的数据库，或者直接修改SQLiteOpenHelper的源码重新编译，不过前者没有对数据库进行一些检验容错处理，也不及SQLiteOpenHelper对数据库操作方便。后者工作量较大，不建议采用。

最后注意记得加入对SD卡的读写权限：



```java
<span style="font-size:12px;"><uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"></uses-permission></span>
```

SQLite桌面查看工具：[sqlite administrator](http://www.cr173.com/soft/4138.html)、[sqlite
 man](http://www.cr173.com/soft/42320.html)或者[firefox](http://www.cr173.com/k/firefox/)插件sqlite manager等。](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




