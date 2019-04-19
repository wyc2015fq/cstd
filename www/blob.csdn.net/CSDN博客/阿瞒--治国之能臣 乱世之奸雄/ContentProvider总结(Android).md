# ContentProvider总结(Android) - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2017年02月09日 12:19:34[baofenny](https://me.csdn.net/jiaminbao)阅读数：697
ContentProvider
1.适用场景
1) ContentProvider为存储和读取数据提供了统一的接口
2) 使用ContentProvider，应用程序可以实现数据共享
3) [Android](http://lib.csdn.net/base/android)内置的许多数据都是使用ContentProvider形式，供开发者调用的(如视频，音频，图片，通讯录等)
2.相关概念介绍
1）ContentProvider简介
       当应用继承ContentProvider类，并重写该类用于提供数据和存储数据的方法，就可以向其他应用共享其数据。虽然使用其他方法也可以对外共享数据，但数据访问方式会因数据存储的方式而不同，如：采用文件方式对外共享数据，需要进行文件操作读写数据；采用sharedpreferences共享数据，需要使用sharedpreferences API读写数据。而使用ContentProvider共享数据的好处是统一了数据访问方式。
2）Uri类简介
      Uri uri = Uri.parse("content://com.changcheng.provider.contactprovider/contact")
      在Content Provider中使用的查询字符串有别于标准的SQL查询。很多诸如select, add, delete, modify等操作我们都使用一种特殊的URI来进行，这种URI由3个部分组成， “content://”, 代表数据的路径，和一个可选的标识数据的ID。以下是一些示例URI:
*     content://media/internal/images  这个URI将返回设备上存储的所有图片     content://contacts/people/  这个URI将返回设备上的所有联系人信息     content://contacts/people/45 这个URI返回单个结果（联系人信息中ID为45的联系人记录）*
　　尽管这种查询字符串格式很常见，但是它看起来还是有点令人迷惑。为此，Android提供一系列的帮助类（在android.provider包下），里面包含了很多以类变量形式给出的查询字符串，这种方式更容易让我们理解一点，因此，如上面content://contacts/people/45这个URI就可以写成如下形式：
　　Uri person = ContentUris.withAppendedId(People.CONTENT_URI,  45);
然后执行数据查询:
Cursor cur = managedQuery(person, null, null, null);
这个查询返回一个包含所有数据字段的游标，我们可以通过迭代这个游标来获取所有的数据：
```
package com.wissen.testApp;
public class ContentProviderDemo extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
       displayRecords();
    }
    private void displayRecords() {
        //该数组中包含了所有要返回的字段
     String columns[] = new String[] { People.NAME, People.NUMBER };
       Uri mContacts = People.CONTENT_URI;
       Cursor cur = managedQuery(
          mContacts,
          columns,  // 要返回的数据字段
       　　null,          // WHERE子句
       　　null,         // WHERE 子句的参数
      　　 null         // Order-by子句
     );
       if (cur.moveToFirst()) {
           String name = null;
           String phoneNo = null;
           do {
              // 获取字段的值
         　　 name = cur.getString(cur.getColumnIndex(People.NAME));
             phoneNo = cur.getString(cur.getColumnIndex(People.NUMBER));
             Toast.makeText(this, name + ” ” + phoneNo, Toast.LENGTH_LONG).show();
          } while (cur.moveToNext());
       }
    }
}
```
　　上例示范了一个如何依次读取联系人信息表中的指定数据列name和number。
**修改记录:**
我们可以使用ContentResolver.update()方法来修改数据，我们来写一个修改数据的方法:
```
private void updateRecord(int recNo, String name) {
    Uri uri = ContentUris.withAppendedId(People.CONTENT_URI, recNo);
    ContentValues values = new ContentValues();
    values.put(People.NAME, name);
    getContentResolver().update(uri, values, null, null);
}
```
现在你可以调用上面的方法来更新指定记录：
updateRecord(10, ”XYZ”);   //更改第10条记录的name字段值为“XYZ”
**添加记录:**要增加记录，我们可以调用ContentResolver.insert()方法，该方法接受一个要增加的记录的目标URI，以及一个包含了新记录值的Map对象，调用后的返回值是新记录的URI，包含记录号。
上面的例子中我们都是基于联系人信息簿这个标准的Content Provider，现在我们继续来创建一个insertRecord() 方法以对联系人信息簿中进行数据的添加：
```
private void insertRecords(String name, String phoneNo) {
    ContentValues values = new ContentValues();
    values.put(People.NAME, name);
    Uri uri = getContentResolver().insert(People.CONTENT_URI, values);
    Log.d(”ANDROID”, uri.toString());
    Uri numberUri = Uri.withAppendedPath(uri, People.Phones.CONTENT_DIRECTORY);
    values.clear();
    values.put(Contacts.Phones.TYPE, People.Phones.TYPE_MOBILE);
    values.put(People.NUMBER, phoneNo);
    getContentResolver().insert(numberUri, values);
}
```
这样我们就可以调用insertRecords(name, phoneNo)的方式来向联系人信息簿中添加联系人姓名和电话号码。
**删除记录:**
Content Provider中的getContextResolver.delete()方法可以用来删除记录，下面的记录用来删除设备上所有的联系人信息：
```
private void deleteRecords() {
    Uri uri = People.CONTENT_URI;
    getContentResolver().delete(uri, null, null);
}
```
你也可以指定WHERE条件语句来删除特定的记录：
getContentResolver().delete(uri, “NAME=” + “‘XYZ XYZ’”, null);
这将会删除name为‘XYZ XYZ’的记录。
**3. 创建ContentProvider**
要创建我们自己的Content Provider的话，我们需要遵循以下几步：
a. 创建一个继承了ContentProvider父类的类
b. 定义一个名为CONTENT_URI，并且是public static final的Uri类型的类变量，你必须为其指定一个唯一的字符串值，最好的方案是以类的全名称， 如:
public static final Uri CONTENT_URI = Uri.parse( “content://com.google.android.MyContentProvider”);
c. 定义你要返回给客户端的数据列名。如果你正在使用Android[数据库](http://lib.csdn.net/base/mysql)，必须为其定义一个叫_id的列，它用来表示每条记录的唯一性。
d. 创建你的数据存储系统。大多数Content Provider使用Android文件系统或SQLite数据库来保持数据，但是你也可以以任何你想要的方式来存储。
e. 如果你要存储字节型数据，比如位图文件等，数据列其实是一个表示实际保存文件的URI字符串，通过它来读取对应的文件数据。处理这种数据类型的Content Provider需要实现一个名为_data的字段，_data字段列出了该文件在Android文件系统上的精确路径。这个字段不仅是供客户端使用，而且也可以供ContentResolver使用。客户端可以调用ContentResolver.openOutputStream()方法来处理该URI指向的文件资源；如果是ContentResolver本身的话，由于其持有的权限比客户端要高，所以它能直接访问该数据文件。
f. 声明public static String型的变量，用于指定要从游标处返回的数据列。
g. 查询返回一个Cursor类型的对象。所有执行写操作的方法如insert(), update() 以及delete()都将被监听。我们可以通过使用ContentResover().notifyChange()方法来通知监听器关于数据更新的信息。
h. 在AndroidMenifest.xml中使用<provider>标签来设置Content Provider。
i. 如果你要处理的数据类型是一种比较新的类型，你就必须先定义一个新的MIME类型，以供ContentProvider.geType(url)来返回。MIME类型有两种形式:一种是为指定的单个记录的，还有一种是为多条记录的。这里给出一种常用的格式：
　　vnd.android.cursor.item/vnd.yourcompanyname.contenttype （单个记录的MIME类型）
　　比如, 一个请求列车信息的URI如content://com.example.transportationprovider/trains/122 可能就会返回typevnd.android.cursor.item/vnd.example.rail这样一个MIME类型。
　　vnd.android.cursor.dir/vnd.yourcompanyname.contenttype （多个记录的MIME类型）
　　比如, 一个请求所有列车信息的URI如content://com.example.transportationprovider/trains 可能就会返回vnd.android.cursor.dir/vnd.example.rail这样一个MIME 类型。
下列代码将创建一个Content Provider，它仅仅是存储用户名称并显示所有的用户名称（使用 SQLLite数据库存储这些数据）：
```
public class MyUsers {
    public static final String AUTHORITY  = “com.wissen.MyContentProvider”;
    // BaseColumn类中已经包含了 _id字段
   public static final class User implements BaseColumns {
        public static final Uri CONTENT_URI  = Uri.parse(”content://com.wissen.MyContentProvider”);
        // 表数据列
        public static final String  USER_NAME  = “USER_NAME”;
    }
}
```
上面的类中定义了Content Provider的CONTENT_URI，以及数据列。下面我们将定义基于上面的类来定义实际的Content Provider类：
```
public class MyContentProvider extends ContentProvider {
    private SQLiteDatabase     sqlDB;
    private DatabaseHelper    dbHelper;
    private static final String  DATABASE_NAME = “Users.db”;
    private static final int  DATABASE_VERSION= 1;
    private static final String TABLE_NAME= “User”;
    private static final String TAG = “MyContentProvider”;
    private static class DatabaseHelper extends SQLiteOpenHelper {
        DatabaseHelper(Context context) {
            super(context, DATABASE_NAME, null, DATABASE_VERSION);
        }
        @Override
        public void onCreate(SQLiteDatabase db) {
            //创建用于存储数据的表
        db.execSQL(”Create table ” + TABLE_NAME + “( _id INTEGER PRIMARY KEY AUTOINCREMENT, USER_NAME TEXT);”);
        }
        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            db.execSQL(”DROP TABLE IF EXISTS ” + TABLE_NAME);
            onCreate(db);
        }
    }
    @Override
    public int delete(Uri uri, String s, String[] as) {
        return 0;
    }
    @Override
    public String getType(Uri uri) {
        return null;
    }
    @Override
    public Uri insert(Uri uri, ContentValues contentvalues) {
        sqlDB = dbHelper.getWritableDatabase();
        long rowId = sqlDB.insert(TABLE_NAME, “”, contentvalues);
        if (rowId > 0) {
            Uri rowUri = ContentUris.appendId(MyUsers.User.CONTENT_URI.buildUpon(), rowId).build();
            getContext().getContentResolver().notifyChange(rowUri, null);
            return rowUri;
        }
        throw new SQLException(”Failed to insert row into ” + uri);
    }
    @Override
    public boolean onCreate() {
        dbHelper = new DatabaseHelper(getContext());
        return (dbHelper == null) ? false : true;
    }
    @Override
    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
        SQLiteQueryBuilder qb = new SQLiteQueryBuilder();
        SQLiteDatabase db = dbHelper.getReadableDatabase();
        qb.setTables(TABLE_NAME);
        Cursor c = qb.query(db, projection, selection, null, null, null, sortOrder);
        c.setNotificationUri(getContext().getContentResolver(), uri);
        return c;
    }
    @Override
    public int update(Uri uri, ContentValues contentvalues, String s, String[] as) {
        return 0;
    }
}
```
一个名为MyContentProvider的Content Provider创建完成了，它用于从Sqlite数据库中添加和读取记录。
Content Provider的入口需要在AndroidManifest.xml中配置:
```
<provider android:name=”MyContentProvider” android:authorities=”com.wissen.MyContentProvider” />
```
之后，让我们来使用这个定义好的Content Provider:
1）为应用程序添加ContentProvider的访问权限。
2）通过getContentResolver()方法得到ContentResolver对象。
3）调用ContentResolver类的query()方法查询数据，该方法会返回一个Cursor对象。
4）对得到的Cursor对象进行分析，得到需要的数据。
5）调用Cursor类的close()方法将Cursor对象关闭。
```
public class MyContentDemo extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        insertRecord(”MyUser”);
        displayRecords();
    }
   
    private void insertRecord(String userName) {
        ContentValues values = new ContentValues();
        values.put(MyUsers.User.USER_NAME, userName);
        getContentResolver().insert(MyUsers.User.CONTENT_URI, values);
    }
    private void displayRecords() {
        String columns[] = new String[] { MyUsers.User._ID, MyUsers.User.USER_NAME };
        Uri myUri = MyUsers.User.CONTENT_URI;
        Cursor cur = managedQuery(myUri, columns,null, null, null );
        if (cur.moveToFirst()) {
            String id = null;
            String userName = null;
            do {
                id = cur.getString(cur.getColumnIndex(MyUsers.User._ID));
                userName = cur.getString(cur.getColumnIndex(MyUsers.User.USER_NAME));
                Toast.makeText(this, id + ” ” + userName, Toast.LENGTH_LONG).show();
           } while (cur.moveToNext());
       }
    }
}
```
