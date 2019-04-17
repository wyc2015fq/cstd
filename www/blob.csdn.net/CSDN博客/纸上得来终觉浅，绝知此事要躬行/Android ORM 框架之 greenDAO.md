# Android ORM 框架之 greenDAO - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月11日 15:14:01[boonya](https://me.csdn.net/boonya)阅读数：559标签：[java																[android																[orm](https://so.csdn.net/so/search/s.do?q=orm&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)





From:[http://blog.csdn.net/kpioneer123/article/details/51354436](http://blog.csdn.net/kpioneer123/article/details/51354436)


GreenDao是一个用于Android开发的对象/关系映射(ORM)工具。它向SQLite数据库提供了一个对象导向的接口。像GreenDao这样的ORM工具不仅为你省去了很多的重复工作，而且提供了更简便的操作接口。

## greenDAO 代码生成的工程结构图

![](https://img-blog.csdn.net/20160509170859095?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









### GREENDAO 设计的主要目标
- 
一个精简的库

- 
性能最大化

- 
内存开销最小化

- 
易于使用的 APIs

- 
对 [Android](http://lib.csdn.net/base/15) 进行高度优化


### GREENDAO 设计的主要特点
- 
greenDAO 性能远远高于同类的 [ORMLite](http://ormlite.com/)。

- 
greenDAO 支持 [protocol buffer(protobuf)](https://github.com/google/protobuf) 协议数据的直接存储，如果你通过
 protobuf 协议与服务器交互，将不需要任何的映射。

- 
与 ORMLite 等使用注解方式的 ORM 框架不同，greenDAO 使用「Code generation」的方式，这也是其性能能大幅提升的原因。


### DAO CODE GENERATION PROJECT

![](https://img-blog.csdn.net/20160509171514000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这是其核心概念：为了在我们的 Android 工程中使用 greenDAO ，我们需要另建一个纯 Java Project，用于自动生成后继 Android 工程中需要使用到的 Bean、DAO、DaoMaster、DaoSession 等类。




为了在你的Android项目中使用GreenDao，你需要创建一个二级工程：“generator project”，它的任务就是为你的domain生成具体的代码。这个生成器工程就是一个普通的java工程。确保greenDao 的greenDao-generator.jar和 freemarker.jar 在classpath中。创建一个可执行的java类，构建你的实体模型并触发代码生成器，更多细节，可以参看 modelling文档。



### 核心类

> 
一旦生成了指定的代码，就可以在你的android工程中使用greenDao了。别忘记在你的android工程中引入greenDao的核心jar包：greenDao.jar。以下是GreenDao的一些必要接口。



DaoMaster：


> 
daomaster以一定的模式持有数据库对象（SQLiteDatabase）并管理一些DAO类（而不是对象）。

有一个静态的方法创建和drop数据库表。它的内部类OpenHelper和DevOpenHelper是SQLiteOpenHelper的实现类，用于创建SQLite数据库的模式。



DaoSession：


> 
管理指定模式下所有可用的DAO对象，你可以通过某个get方法获取到。DaoSession提供一些通用的持久化方法，比如对实体进行插入，加载，更新，刷新和删除。最后DaoSession对象会跟踪identity scope，更多细节，可以参看 session文档。



DAOs（Data access objects）:


> 
数据访问对象，用于实体的持久化和查询。对于每一个实体，greenDao会生成一个DAO，相对于DaoSession它拥有更多持久化的方法，比如：加载全部，插入（insertInTx，语境不明了，暂且简单的翻译成插入）。



具体操作步骤：

**Step1：**在 ANDROID 工程中配置「GREENDAO
 GENERATOR」模块

1.在 .src/main 目录下新建一个与 java 同层级的「java-gen」目录，用于存放由 greenDAO 生成的 Bean、DAO、DaoMaster、DaoSession
 等类。


   new->Directory  (mingjava-gen)

![](https://img-blog.csdn.net/20160509173441051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2.配置
 Android 工程（app）的 build.gradle，在android{}结构体中分别添加 [sourceSets](https://docs.gradle.org/current/dsl/org.gradle.api.tasks.SourceSet.html) 与[dependencies](https://docs.gradle.org/1.8-rc-1/userguide/dependency_management.html)。 
![](https://img-blog.csdn.net/20160509174103373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








**[java]**[view plain](http://blog.csdn.net/kpioneer123/article/details/51354436#)[copy](http://blog.csdn.net/kpioneer123/article/details/51354436#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- sourceSets {  
-        main {  
-            java.srcDirs = ['src/main/java', 'src/main/java-gen']  
-        }  
-    }  
- 
- compile 'org.greenrobot:greendao:2.2.0'




**Step2：**新建「GREENDAO
 GENERATOR」模块 (纯 JAVA 工程）

**1.通过 File -> New -> New Module -> Java Library -> 填写相应的包名与类名 -> Finish.**





![](https://img-blog.csdn.net/20160509174748827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


通过这种方式生成的Module结构(我起名为greendao_generator )与一般情况不同




**2.配置greendao_generator 工程的 build.gradle，添加 dependencies.**


**[java]**[view plain](http://blog.csdn.net/kpioneer123/article/details/51354436#)[copy](http://blog.csdn.net/kpioneer123/article/details/51354436#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- compile 'org.greenrobot:greendao-generator:2.2.0'




**3.编写 MyClass类，注意： 我们的 Java 工程只有一个类，它的内容决定了「GreenDao Generator」的输出，你可以在这个类中通过对象、关系等创建数据库结构，下面我将以注释的形式详细讲解代码内容。**




**[java]**[view plain](http://blog.csdn.net/kpioneer123/article/details/51354436#)[copy](http://blog.csdn.net/kpioneer123/article/details/51354436#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- package com.example;  
- 
- import de.greenrobot.daogenerator.DaoGenerator;  
- import de.greenrobot.daogenerator.Entity;  
- import de.greenrobot.daogenerator.Schema;  
- 
- publicclass MyClass{  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- // 正如你所见的，你创建了一个用于添加实体（Entity）的模式（Schema）对象。
- // 两个参数分别代表：数据库版本号与自动生成代码的包路径。
-         Schema schema = new Schema(1, "com.xionghu.greendao");  
- //      当然，如果你愿意，你也可以分别指定生成的 Bean 与 DAO 类所在的目录，只要如下所示：
- //      Schema schema = new Schema(1, "me.itangqi.bean");
- //      schema.setDefaultJavaPackageDao("me.itangqi.dao");
- 
- // 模式（Schema）同时也拥有两个默认的 flags，分别用来标示 entity 是否是 activie 以及是否使用 keep sections。
- // schema2.enableActiveEntitiesByDefault();
- // schema2.enableKeepSectionsByDefault();
- 
- // 一旦你拥有了一个 Schema 对象后，你便可以使用它添加实体（Entities）了。
-         addNote(schema);  
- 
- // 最后我们将使用 DAOGenerator 类的 generateAll() 方法自动生成代码，此处你需要根据自己的情况更改输出目录（既之前创建的 java-gen)。
- // 其实，输出目录的路径可以在 build.gradle 中设置，有兴趣的朋友可以自行搜索，这里就不再详解。
- new DaoGenerator().generateAll(schema, "D:\\android_studio_4_6\\MyGreenDAO\\app\\src\\main\\java-gen");  
-     }  
- 
- /**
-      * @param schema
-      */
- privatestaticvoid addNote(Schema schema) {  
- // 一个实体（类）就关联到数据库中的一张表，此处表名为「Note」（既类名）
-         Entity note = schema.addEntity("Note");  
- // 你也可以重新给表命名
- // note.setTableName("NODE");
- 
- // greenDAO 会自动根据实体类的属性值来创建表字段，并赋予默认值
- // 接下来你便可以设置表中的字段：
-         note.addIdProperty();  
-         note.addStringProperty("text").notNull();  
- // 与在 Java 中使用驼峰命名法不同，默认数据库中的命名是使用大写和下划线来分割单词的。
- // For example, a property called “creationDate” will become a database column “CREATION_DATE”.
-         note.addStringProperty("comment");  
-         note.addDateProperty("date");  
-     }  
- 
- }  


**Step3：**
### 生成 DAO 文件（数据库）


执行
 generator 工程，如一切正常，你将会在控制台看到如下日志，并且在主工程「java-gen」下会发现生成了DaoMaster、DaoSession、NoteDao、Note共4个类文件。

在这里需要对运行条件进行配置[Android
 Studio 运行java程序](http://my.oschina.net/u/2444750/blog/614059)





**![](https://img-blog.csdn.net/20160509180512076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**





![](https://img-blog.csdn.net/20160509180702102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如图 再点run ，生成以下程序







![](https://img-blog.csdn.net/20160509181020509?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





运行时的输出框的正确信息

![](https://img-blog.csdn.net/20160509181210272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





Step4：在
 ANDROID 工程中进行数据库操作



**[java]**[view plain](http://blog.csdn.net/kpioneer123/article/details/51354436#)[copy](http://blog.csdn.net/kpioneer123/article/details/51354436#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- package com.cloudhome.mygreendao;  
- 
- import android.app.ListActivity;  
- import android.database.Cursor;  
- import android.database.sqlite.SQLiteDatabase;  
- import android.os.Bundle;  
- import android.util.Log;  
- import android.view.View;  
- import android.widget.EditText;  
- import android.widget.ListView;  
- import android.widget.SimpleCursorAdapter;  
- 
- import com.xionghu.greendao.Note;  
- import com.xionghu.greendao.NoteDao;  
- 
- import java.text.DateFormat;  
- import java.util.Date;  
- import java.util.List;  
- import de.greenrobot.dao.query.Query;  
- import de.greenrobot.dao.query.QueryBuilder;  
- 
- 
- 
- publicclass MainActivity extends ListActivity {  
- private EditText editText;  
- private Cursor cursor;  
- publicstaticfinal String TAG = "DaoExample";  
- private String orderBy;  
- private String textColumn;  
- private   SimpleCursorAdapter adapter;  
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.activity_main);  
- 
-         textColumn = NoteDao.Properties.Text.columnName;  
- 
-         orderBy=textColumn+" COLLATE LOCALIZED ASC";  
- 
-         cursor = getDb().query(getNoteDao().getTablename(), getNoteDao().getAllColumns(), null, null, null, null, orderBy);  
- 
- 
-         String[] from = {textColumn, NoteDao.Properties.Comment.columnName};  
- 
- int[] to = {android.R.id.text1, android.R.id.text2};  
- 
- 
-         adapter = new SimpleCursorAdapter(this, android.R.layout.simple_list_item_2, cursor, from,  
-                 to);  
- 
-         setListAdapter(adapter);  
- 
-         adapter.notifyDataSetChanged();  
- 
- 
-         editText = (EditText) findViewById(R.id.editTextNote);  
-     }  
- 
- private NoteDao getNoteDao() {  
- // 通过 BaseApplication 类提供的 getDaoSession() 获取具体 Dao
- return ((BaseApplication) this.getApplicationContext()).getDaoSession().getNoteDao();  
-     }  
- 
- private SQLiteDatabase getDb() {  
- // 通过 BaseApplication 类提供的 getDb() 获取具体 db
- return ((BaseApplication) this.getApplicationContext()).getDb();  
-     }  
- 
- /**
-      * Button 点击的监听事件
-      *
-      * @param view
-      */
- publicvoid onMyButtonClick(View view) {  
- switch (view.getId()) {  
- case R.id.buttonAdd:  
-                 addNote();  
- break;  
- case R.id.buttonQuery:  
-                 search();  
- break;  
- default:  
-                 ToastUtils.show(getApplicationContext(), "What's wrong ?");  
- break;  
-         }  
-     }  
- 
- privatevoid addNote() {  
-         String noteText = editText.getText().toString();  
-         editText.setText("");  
- 
- final DateFormat df = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM);  
-         String comment = "Added on " + df.format(new Date());  
- 
- if (noteText == null || noteText.equals("")) {  
-             ToastUtils.show(getApplicationContext(), "Please enter a note to add");  
-         } else {  
- // 插入操作，简单到只要你创建一个 Java 对象
-             Note note = new Note(null, noteText, comment, new Date());  
-             getNoteDao().insert(note);  
-             Log.d(TAG, "Inserted new note, ID: " + note.getId());  
- 
- 
- 
-             cursor = getDb().query(getNoteDao().getTablename(), getNoteDao().getAllColumns(), null, null, null, null, orderBy);  
- 
-             String[] from = {textColumn, NoteDao.Properties.Comment.columnName};  
- 
- int[] to = {android.R.id.text1, android.R.id.text2};  
- 
- 
-             adapter = new SimpleCursorAdapter(this, android.R.layout.simple_list_item_2, cursor, from,  
-                     to);  
- 
-             setListAdapter(adapter);  
- 
-             adapter.notifyDataSetChanged();  
- 
- 
-         }  
- 
-     }  
- 
- privatevoid search() {  
-         String noteText = editText.getText().toString();  
-         editText.setText("");  
- if (noteText == null || noteText.equals("")) {  
-             ToastUtils.show(getApplicationContext(), "Please enter a note to query");  
-         } else {  
- // Query 类代表了一个可以被重复执行的查询
-             Query query = getNoteDao().queryBuilder()  
-                     .where(NoteDao.Properties.Text.eq(noteText))  
-                     .orderAsc(NoteDao.Properties.Date)  
-                     .build();  
- // 查询结果以 List 返回
-             List notes = query.list();  
-             ToastUtils.show(getApplicationContext(), "There have " + notes.size() + " records");  
-         }  
- // 在 QueryBuilder 类中内置两个 Flag 用于方便输出执行的 SQL 语句与传递参数的值
-         QueryBuilder.LOG_SQL = true;  
-         QueryBuilder.LOG_VALUES = true;  
-     }  
- 
- /**
-      * ListView 的监听事件，用于删除一个 Item
-      *
-      * @param l
-      * @param v
-      * @param position
-      * @param id
-      */
- @Override
- protectedvoid onListItemClick(ListView l, View v, int position, long id) {  
- 
- // 删除操作，你可以通过「id」也可以一次性删除所有
- 
-         getNoteDao().deleteByKey(id);  
- 
- //       getNoteDao().deleteAll();
- 
-         ToastUtils.show(getApplicationContext(), "Deleted note, ID: " + id);  
- 
-         Log.d(TAG, "Deleted note, ID: " + id);  
- 
-         cursor = getDb().query(getNoteDao().getTablename(), getNoteDao().getAllColumns(), null, null, null, null, orderBy);  
- 
-         String[] from = {textColumn, NoteDao.Properties.Comment.columnName};  
- 
- int[] to = {android.R.id.text1, android.R.id.text2};  
- 
- 
-         adapter = new SimpleCursorAdapter(this, android.R.layout.simple_list_item_2, cursor, from,  
-                 to);  
- 
-         setListAdapter(adapter);  
- 
-         adapter.notifyDataSetChanged();  
- 
- 
-     }  
- 
- 
- }  



### **Step5：**运行结果

![](https://img-blog.csdn.net/20160509181719811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











[demo下载](http://download.csdn.net/detail/kpioneer123/9514777)





参考资料：

[http://www.2cto.com/kf/201604/498026.html](http://www.2cto.com/kf/201604/498026.html)


[https://github.com/greenrobot/greenDAO](https://github.com/greenrobot/greenDAO)


[https://github.com/tangqi92/MyGreenDAO](https://github.com/tangqi92/MyGreenDAO)


[http://www.w2bc.com/Article/20726](http://www.w2bc.com/Article/20726)


[http://blog.csdn.net/h3c4lenovo/article/details/43566169](http://blog.csdn.net/h3c4lenovo/article/details/43566169)

[http://my.oschina.net/u/2444750/blog/614059](http://my.oschina.net/u/2444750/blog/614059)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




