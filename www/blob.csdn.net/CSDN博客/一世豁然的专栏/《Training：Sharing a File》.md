# 《Training：Sharing a File》 - 一世豁然的专栏 - CSDN博客





2017年04月22日 14:46:04[一世豁然](https://me.csdn.net/Explorer_day)阅读数：293标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。







一旦您设置了应用程序以使用内容URI共享文件，您可以回应其他应用程序对这些文件的请求。响应这些请求的一种方法是从其他应用程序可以调用的服务器app提供文件选择界面。这种方法允许客户端应用程序让用户从服务器应用程序中选择一个文件，然后接收所选文件的内容URI。





本课程向您展示如何在应用程序中创建文件选择活动，以响应文件请求。








一、接收文件请求

要接收来自客户端应用程序的文件请求并使用内容URI进行响应，app应提供文件选择活动。客户端应用程序通过使用包含动作ACTION_PICK的Intent来调用startActivityForResult（）启动此Activity。当客户端应用程序调用startActivityForResult（）时，您的应用程序可以以用户选择的文件的内容URI的形式将结果返回给客户端应用程序。





要了解如何在客户端应用程序中实现文件请求，请参阅“请求共享文件”课程。








二、创建文件选择活动

要设置文件选择活动，请首先指定清单中的活动，以及与ACTION_PICK操作和CATEGORY_DEFAULT类别以及CATEGORY_OPENABLE类别匹配的意图过滤器。




```
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    ...
        <application>
        ...
            <activity
                android:name=".FileSelectActivity"
                android:label="@File Selector" >
                <intent-filter>
                    <action
                        android:name="android.intent.action.PICK"/>
                    <category
                        android:name="android.intent.category.DEFAULT"/>
                    <category
                        android:name="android.intent.category.OPENABLE"/>
                    <data android:mimeType="text/plain"/>
                    <data android:mimeType="image/*"/>
                </intent-filter>
            </activity>
```





一）、在代码中定义文件选择活动

接下来，定义一个Activity子类，显示在内部存储中来自你的应用程序的files/ images /目录可用的文件，并允许用户选择所需的文件。以下代码段演示了如何定义此活动并响应用户的选择：




```java
public class MainActivity extends Activity {
    // The path to the root of this app's internal storage
    private File mPrivateRootDir;
    // The path to the "images" subdirectory
    private File mImagesDir;
    // Array of files in the images subdirectory
    File[] mImageFiles;
    // Array of filenames corresponding to mImageFiles
    String[] mImageFilenames;
    // Initialize the Activity
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        ...
        // Set up an Intent to send back to apps that request a file
        mResultIntent =
                new Intent("com.example.myapp.ACTION_RETURN_FILE");
        // Get the files/ subdirectory of internal storage
        mPrivateRootDir = getFilesDir();
        // Get the files/images subdirectory;
        mImagesDir = new File(mPrivateRootDir, "images");
        // Get the files in the images subdirectory
        mImageFiles = mImagesDir.listFiles();
        // Set the Activity's result to null to begin with
        setResult(Activity.RESULT_CANCELED, null);
        /*
         * Display the file names in the ListView mFileListView.
         * Back the ListView with the array mImageFilenames, which
         * you can create by iterating through mImageFiles and
         * calling File.getAbsolutePath() for each File
         */
         ...
    }
    ...
}
```



三、响应文件选择

一旦用户选择共享文件，您的应用程序必须确定选择了哪个文件，然后生成文件的内容URI。由于活动显示ListView中可用文件的列表，当用户单击文件名时，系统将调用onItemClick（）方法，您可以在其中获取所选文件。





在运行Android 6.0（API级别23）及以后的设备上执行此操作需要特别小心，因为该版本的Android版权限模式发生变化，特别是READ_EXTERNAL_STORAGE成为接收应用程序可能缺少的危险许可。





考虑到这些考虑，我们建议您避免使用Uri.fromFile（），这表现了一些缺点。 这种方法：


1、不允许跨配置文件共享文件。

2、要求您的应用在运行在具有WRITE_EXTERNAL_STORAGE权限的Android 4.4（API级别19）或更低版本的设备上。

3、要求接收应用具有READ_EXTERNAL_STORAGE权限，对于没有该权限的重要共享目标（如Gmail）将失败。




取而代之使用Uri.fromFile（），您可以使用URI权限来授予其他应用程序访问特定的URI。 虽然URI权限不适用于由Uri.fromFile（）生成的file：// URI，但它们对与内容提供商关联的URI有效。 FileProvider API可以帮助您创建这样的URI。 此方法也适用于不在外部存储中的文件，但在发送意图的应用程序的本地存储中。





在onItemClick（）中，获取所选文件的文件名的File对象，并将其作为参数传递给getUriForFile（），以及在FileProvider的<provider>元素中指定的权限。 生成的内容URI包含权限，对应于文件目录（如XML元数据中指定的）的路径段以及包含其扩展名的文件的名称。 如何将FileProvider根据XML元数据将目录映射到路径段在“指定可分离目录”一节中进行了说明。





以下片段显示了如何检测所选文件并获取内容URI：




```java
protected void onCreate(Bundle savedInstanceState) {
        ...
        // Define a listener that responds to clicks on a file in the ListView
        mFileListView.setOnItemClickListener(
                new AdapterView.OnItemClickListener() {
            @Override
            /*
             * When a filename in the ListView is clicked, get its
             * content URI and send it to the requesting app
             */
            public void onItemClick(AdapterView<?> adapterView,
                    View view,
                    int position,
                    long rowId) {
                /*
                 * Get a File for the selected file name.
                 * Assume that the file names are in the
                 * mImageFilename array.
                 */
                File requestFile = new File(mImageFilename[position]);
                /*
                 * Most file-related method calls need to be in
                 * try-catch blocks.
                 */
                // Use the FileProvider to get a content URI
                try {
                    fileUri = FileProvider.getUriForFile(
                            MainActivity.this,
                            "com.example.myapp.fileprovider",
                            requestFile);
                } catch (IllegalArgumentException e) {
                    Log.e("File Selector",
                          "The selected file can't be shared: " +
                          clickedFilename);
                }
                ...
            }
        });
        ...
    }
```


请记住，您只能在目录中为驻留在包含<paths>元素的元数据文件中指定的文件生成内容URI，如“指定可擦除目录”一节中所述。如果在未指定的路径中为文件调用getUriForFile（），则会收到IllegalArgumentException。







四、授予文件的权限

现在，您有要与其他应用程序共享文件的内容URI，您需要允许客户端应用访问该文件。要允许访问，通过将内容URI添加到Intent，然后在Intent上设置权限标志，向客户端应用程序授予权限。您授予的权限是临时的，并且在接收应用程序的任务堆栈完成时自动过期。





以下代码片段显示了如何设置文件的读取权限：




```java
protected void onCreate(Bundle savedInstanceState) {
        ...
        // Define a listener that responds to clicks in the ListView
        mFileListView.setOnItemClickListener(
                new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView,
                    View view,
                    int position,
                    long rowId) {
                ...
                if (fileUri != null) {
                    // Grant temporary read permission to the content URI
                    mResultIntent.addFlags(
                        Intent.FLAG_GRANT_READ_URI_PERMISSION);
                }
                ...
             }
             ...
        });
    ...
    }
```


注意：调用setFlags（）是使用临时访问权限来安全授予访问文件的唯一方法。避免为文件的内容URI调用Context.grantUriPermission（）方法，因为此方法只能通过调用Context.revokeUriPermission（）撤销的访问权限。




不要使用Uri.fromFile（）。 它强制接收具有READ_EXTERNAL_STORAGE权限的应用程序，如果您尝试在用户之间共享，并且设备版本低于4.4（API等级19），此方法将无法使用，这将需要您的应用有WRITE_EXTERNAL_STORAGE权限。而真正重要的分享目标（例如Gmail应用程式）则没有READ_EXTERNAL_STORAGE权限，这将导致该通话失败。 相反，您可以使用URI权限授予其他应用程序访问特定URI。 虽然URI权限不适用于由Uri.fromFile（）生成的file：//
 URI，但它们在与内容提供器相关联的Uris上工作。 您可以使用在文件共享中说明的FileProvider，而不是为此实现自己（文件提供器）类。








五、与请求的应用程序共享文件

要与请求的应用程序共享文件，请将包含内容URI以及权限的Intent传递给setResult（）。 当您刚刚定义的Activity完成后，系统会将包含内容URI的Intent发送到客户端应用程序。 以下代码段显示如何执行此操作：




```java
protected void onCreate(Bundle savedInstanceState) {
        ...
        // Define a listener that responds to clicks on a file in the ListView
        mFileListView.setOnItemClickListener(
                new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView,
                    View view,
                    int position,
                    long rowId) {
                ...
                if (fileUri != null) {
                    ...
                    // Put the Uri and MIME type in the result Intent
                    mResultIntent.setDataAndType(
                            fileUri,
                            getContentResolver().getType(fileUri));
                    // Set the result
                    MainActivity.this.setResult(Activity.RESULT_OK,
                            mResultIntent);
                    } else {
                        mResultIntent.setDataAndType(null, "");
                        MainActivity.this.setResult(RESULT_CANCELED,
                                mResultIntent);
                    }
                }
        });
```


一旦选择了文件，用户就可以立即返回到客户端应用程序，这是一个方法。另 一种方法是提供一个复选标记或完成按钮。 使用按钮的android：onClick属性将方法与按钮相关联。 在该方法中，调用finish（）。 例如：



```java
public void onDoneClick(View v) {
        // Associate a method with the Done button
        finish();
    }
```





