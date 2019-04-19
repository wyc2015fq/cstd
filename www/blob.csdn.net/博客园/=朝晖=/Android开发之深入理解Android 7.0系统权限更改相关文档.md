# Android开发之深入理解Android 7.0系统权限更改相关文档 - =朝晖= - 博客园
# [Android开发之深入理解Android 7.0系统权限更改相关文档](https://www.cnblogs.com/dhcn/p/7131091.html)
http://www.cnblogs.com/dazhao/p/6547811.html
## 摘要：
Android 6.0之后的版本增加了运行时权限，应用程序在执行每个需要系统权限的功能时，需要添加权限请求代码（默认权限禁止），否则应用程序无法响应；Android 7.0在Android 6.0的基础上，对系统权限进一步更改，这次的权限更改包括三个方面：
- APP应用程序的私有文件不再向使用者放宽
- Intent组件传递`file://URI`的方式可能给接收器留下无法访问的路径，触发`FileUriExposedException`异常，推荐使用`FileProvider`
- `DownloadManager`不再按文件名分享私人存储的文件。旧版应用在访问`COLUMN_LOCAL_FILENAME`时可能出现无法访问的路径。面向 Android 7.0 或更高版本的应用在尝试访问 COLUMN_LOCAL_FILENAME 时会触发 SecurityException
简单的三句话，无法让TeachCourse真正理解Android 7.0系统权限更改的含义，如果不按照文档的方式去做，*API 24*开发的应用程序是否就用不了？
![Android 7.0系统权限变更](https://images2015.cnblogs.com/blog/595061/201703/595061-20170314115746041-343689878.png)
## 一、深入理解FileProvider
`FileProvider`属于Android 7.0新增的一个类，该类位于v4包下，详情可见`android.support.v4.content.FileProvider`，使用方法类似与`ContentProvider`，简单概括为三个步骤，这里先以调用系统相机拍照并保存**sdcard**公共目录为例，演示使用过程：
- 在资源文件夹`res/xml`下新建`file_provider.xml`文件，文件声明权限请求的路径，代码如下：
```xml
<?xml version="1.0" encoding="utf-8"?>
    <paths xmlns:android="http://schemas.android.com/apk/res/android">
        <!--3、对应外部内存卡根目录：Environment.getExternalStorageDirectory()-->
        <external-path name="ext_root" path="/" />
    </paths>
```
- 在`AndroidManifest.xml`添加组件`provider`相关信息，类似组件`activity`，指定`resource`属性引用上一步创建的xml文件（后面会详细介绍各个属性的用法），代码如下：
```xml
<!-- 定义FileProvider -->
    <provider
        android:name="android.support.v4.content.FileProvider"
        android:authorities="@string/install_apk_path"
        android:exported="false"
        android:grantUriPermissions="true">
        <meta-data
            android:name="android.support.FILE_PROVIDER_PATHS"
            android:resource="@xml/file_provider" />
    </provider>
```
- 最后一步，Java代码申请权限，使用新增的方法`getUriForFile()`和`grantUriPermission()`，代码如下（后面会详细介绍方法对应参数的使用）：
```java
if (Build.VERSION.SDK_INT > 23) {
        /**Android 7.0以上的方式**/
        Uri contentUri = getUriForFile(this, getString(R.string.install_apk_path), file);
        grantUriPermission(getPackageName(), contentUri, Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, contentUri);
    }
```
- 修改`build.gradle`文件*compileSdkVersion*大于或等于24，*targetSdkVersion*等于24，使用Android 7.0模拟器运行Demo，效果图：
![Android 7.0系统权限更改](https://images2015.cnblogs.com/blog/595061/201703/595061-20170314115859057-1768230830.gif)
那么，我们已经了解Android 7.0系统权限申请的步骤，接下来说明每一个步骤需要注意的事项、相关方法参数的说明、属性的含义以及可以的申请权限目录（最后下载相关Demo）。
### 1.1 定义一个`FileProvider`
直接使用`FileProvider`本身或者它的子类，需要在`AndroidManifest.xml`文件中声明组件的相关属性，包括：
- `android:name`，对应属性值：`android.support.v4.content.FileProvider`或者子类完整路径
- `android:authorities`，对应属性值是一个常量，通常定义的方式`packagename.fileprovider`，例如：`cn.teachcourse.fileprovider`
- `android:exported`，对应属性值是一个boolean变量，设置为`false`
- `android:grantUriPermissions`，对应属性值也是一个boolean变量，设置为`true`，允许获得文件临时的访问权限
```xml
<manifest>
    ...
    <application>
        ...
        <provider
            android:name="android.support.v4.content.FileProvider"
            android:authorities="com.mydomain.fileprovider"
            android:exported="false"
            android:grantUriPermissions="true">
            ...
        </provider>
        ...
    </application>
</manifest>
```
想要关联`res/xml`文件夹下创建的`file_provider.xml`文件，需要在`<provider>`标签内，添加`<meta-data>`子标签，设置`<meta-data>`标签的属性值，包括：
- `android:name`，对应属性值是一个固定的系统常量`android.support.FILE_PROVIDER_PATHS`
- `android:resource`，对应属性值指向我们的xml文件`@xml/file_provider`
```xml
<provider
    android:name="android.support.v4.content.FileProvider"
    android:authorities="com.mydomain.fileprovider"
    android:exported="false"
    android:grantUriPermissions="true">
    <meta-data
        android:name="android.support.FILE_PROVIDER_PATHS"
        android:resource="@xml/file_provider" />
</provider>
```
### 1.2 指定授予临时访问权限的文件目录
上一步说明了怎么定义一个`FileProvider`，这一步主要说明怎么定义一个`@xml/file_provider`文件。Android Studio或Eclipse开发工具创建Android项目的时候默认不会创建`res/xml`文件夹，需要开发者手动创建，点击`res`文件夹新建目录，命名`xml`，如下图：
![Android Studio新建xml目录](https://images2015.cnblogs.com/blog/595061/201703/595061-20170314115946026-1716302658.png)
然后，在`xml`文件夹下新建一个xml文件，文件命名`file_provider.xml`，指定根标签为`paths`，如下图：
![xml新建file_provider.xml](https://images2015.cnblogs.com/blog/595061/201703/595061-20170314120012666-1225654349.png)
在xml文件中指定文件存储的区块和区块的相对路径，在`<paths>`根标签中添加`<files-path>`子标签（稍后详细列出所有子标签），设置子标签的属性值，包括：
- `name`，是一个虚设的文件名（可以自由命名），对外可见路径的一部分，隐藏真实文件目录
- `path`，是一个相对目录，相对于当前的子标签`<files-path>`根目录
- `<files-path>`，表示内部内存卡根目录，对应根目录等价于`Context.getFilesDir()`，查看完整路径：
`/data/user/0/cn.teachcourse.demos/files`
- 代码如下：
```xml
<paths xmlns:android="http://schemas.android.com/apk/res/android">
    <files-path name="my_images" path="images/"/>
    ...
</paths>
```
`<paths>`根标签下可以添加的子标签也是有限的，参考官网的开发文档，除了上述的提到的`<files-path>`这个子标签外，还包括下面几个：
- 
`<cache-path>`，表示应用默认缓存根目录，对应根目录等价于`getCacheDir()`，查看完整路径：`/data/user/0/cn.teachcourse.demos/cache`
- 
`<external-path>`，表示外部内存卡根目录，对应根目录等价于
`Environment.getExternalStorageDirectory()`，
查看完整路径：`/storage/emulated/0`
- 
`<external-files-path>`，表示外部内存卡根目录下的APP公共目录，对应根目录等价于
`Context#getExternalFilesDir(String) Context.getExternalFilesDir(null)`，
查看完整路径：
`/storage/emulated/0/Android/data/cn.teachcourse.demos/files/Download`
- 
`<external-cache-path>`，表示外部内存卡根目录下的APP缓存目录，对应根目录等价于`Context.getExternalCacheDir()`，查看完整路径：
`/storage/emulated/0/Android/data/cn.teachcourse.demos/cache`
最终，在`file_provider.xml`文件中，添加上述5种类型的临时访问权限的文件目录，代码如下：
```xml
<?xml version="1.0" encoding="utf-8"?>
<paths xmlns:android="http://schemas.android.com/apk/res/android">
    <!--
    1、name对应的属性值，开发者可以自由定义；
    2、path对应的属性值，当前external-path标签下的相对路径
    比如：/storage/emulated/0/92Recycle-release.apk
    sdcard路径：/storage/emulated/0(WriteToReadActivity.java:176)
                      at cn.teachcourse.nougat.WriteToReadActivity.onClick(WriteToReadActivity.java:97)
                      at android.view.View.performClick(View.java:5610)
                      at android.view.View$PerformClick.run(View.java:22265)
    相对路径：/
    -->
    <!--1、对应内部内存卡根目录：Context.getFileDir()-->
    <files-path
        name="int_root"
        path="/" />
    <!--2、对应应用默认缓存根目录：Context.getCacheDir()-->
    <cache-path
        name="app_cache"
        path="/" />
    <!--3、对应外部内存卡根目录：Environment.getExternalStorageDirectory()-->
    <external-path
        name="ext_root"
        path="pictures/" />
    <!--4、对应外部内存卡根目录下的APP公共目录：Context.getExternalFileDir(String)-->
    <external-files-path
        name="ext_pub"
        path="/" />
    <!--5、对应外部内存卡根目录下的APP缓存目录：Context.getExternalCacheDir()-->
    <external-cache-path
        name="ext_cache"
        path="/" />
</paths>
```
### 1.3 生成指定文件的Content URI
Content URI方便与另一个APP应用程序共享同一个文件，共享的方式通过`ContentResolver.openFileDescriptor`获得一个`ParcelFileDescriptor`对象，读取文件内容。那么，如何生成一条完整的Content URI呢？TeachCourse总结后，概括为三个步骤，**第一步：**明确上述5种类型中的哪一种，**第二步：**明确指定文件的完整路径（包括目录、文件名），**第三步：**调用`getUriForFile()`方法生成URI
```java
File imagePath = new File(Environment.getExternalStorageDirectory(), "download");
File newFile = new File(imagePath, "default_image.jpg");
Uri contentUri = getUriForFile(getContext(), "cn.teachcourse.fileprovider", newFile);
```
### 1.4 授予Content URI临时访问权限
上一步获得的Content URI，并没有获得指定文件的读写权限，想要获得文件的读写权限需要调用`Context.grantUriPermission(package, Uri, mode_flags)`方法，该方法向指定包名的应用程序申请获得读取或者写入文件的权限，参数说明如下：
- `package`，指定应用程序的包名，Android Studio真正的包名指`build.gradle`声明的*applicationId*属性值；`getPackageName()`指`AndroidManifest.xml`文件声明的*package*属性值，如果两者不一致，就不能提供`getPackageName()`获取包名，否则报错！
- `Uri`，指定请求授予临时权限的URI，例如：`contentUri`
- `mode_flags`，指定授予临时权限的类型，选择其中一个常量或两个：`Intent.FLAG_GRANT_READ_URI_PERMISSION`，`Intent.FLAG_GRANT_WRITE_URI_PERMISSION`
授予文件的临时读取或写入权限，如果不再需要了，TeachCourse该如何撤销授予呢？撤销权限有两种方式：**第一种：**通过调用`revokeUriPermission()`撤销，**第二种：**重启系统后自动撤销
### 1.5 对外提供可访问的Content URI
有多种方式可以向客户端APP提供可访问文件的Content URI，其中一种常用的方式是通过发送`Intent`给需要启动的`Activity`，在重写的`startActivityResult()`方法中获取授予临时权限的Content URI或向用户提供可访问的接口来获取文件，后面的这种方式获取文件后转换成Content URI，以文章开头拍照的功能为例，TeachCourse想要在**sdcard**的公共目录`pictures/`查看已保存的照片，实现过程：
- 请求授予访问公共目录的权限，代码如下：
```java
if (Build.VERSION.SDK_INT > 23) {
        /**Android 7.0以上的方式**/
        mStorageManager = this.getSystemService(StorageManager.class);
        StorageVolume storageVolume = mStorageManager.getPrimaryStorageVolume();
        Intent intent = storageVolume.createAccessIntent(Environment.DIRECTORY_PICTURES);
        startActivityForResult(intent, REQUEST_CODE_GRAINT_URI);
    }
```
- 在重写的`startActivityResult()`方法中获取授予临时权限的Content URI，代码如下：
```java
@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case REQUEST_CODE_GRAINT_URI:
                updateDirectoryEntries(data.getData());
                Log.d(TAG, "onActivityResult:Uri= "+data.getData());
                break;
        }
    }
```
- 查询`Environment.DIRECTORY_PICTURES`目录，返回的Content URI包含的文件和文件类型相关信息，代码如下：
```java
private static final String[] DIRECTORY_SELECTION = new String[]{
            DocumentsContract.Document.COLUMN_DISPLAY_NAME,
            DocumentsContract.Document.COLUMN_MIME_TYPE,
            DocumentsContract.Document.COLUMN_DOCUMENT_ID,
    };
    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    private void updateDirectoryEntries(Uri uri) {
        ContentResolver contentResolver = this.getContentResolver();
        Uri docUri = DocumentsContract.buildDocumentUriUsingTree(uri,
                DocumentsContract.getTreeDocumentId(uri));
        Uri childrenUri = DocumentsContract.buildChildDocumentsUriUsingTree(uri,
                DocumentsContract.getTreeDocumentId(uri));
        try (Cursor docCursor = contentResolver
                .query(docUri, DIRECTORY_SELECTION, null, null, null)) {
            while (docCursor != null && docCursor.moveToNext()) {
                mPath_tv.setText(docCursor.getString(docCursor.getColumnIndex(
                        DocumentsContract.Document.COLUMN_DISPLAY_NAME)));
            }
        }
        try (Cursor childCursor = contentResolver
                .query(childrenUri, DIRECTORY_SELECTION, null, null, null)) {
            while (childCursor != null && childCursor.moveToNext()) {
                String fileName = childCursor.getString(childCursor.getColumnIndex(
                        DocumentsContract.Document.COLUMN_DISPLAY_NAME));
                String mimeType = childCursor.getString(childCursor.getColumnIndex(
                        DocumentsContract.Document.COLUMN_MIME_TYPE));
                Log.e(TAG, "updateDirectoryEntries: "+fileName+"\n"+mimeType);
            }
        }
    }
```
运行Demo，控制台打印效果图：
![Android 7.0访问sdcard](https://images2015.cnblogs.com/blog/595061/201703/595061-20170314120105526-1949567413.png)
更多说明，可以参考Google提供的例子
## 二、深入理解DownloadManager
同样，为了方便理解`DownloadManager`的用法，首先以一个简单例子开始：从指定的url下载资源，然后显示下载资源的相关信息，运行Demo的效果图：
![DownloadManager详解](https://images2015.cnblogs.com/blog/595061/201703/595061-20170314120125385-292090101.png)
Android 7.0系统权限更改的第三点，简单的说：*通过访问**COLUMN_LOCAL_FILENAME**，在Android 7.0系统上可能无法获取Demo效果图fileName对应的文件路径*，这时候可能触发异常`SecurityException`，打印的log信息，如下：
```
Caused by: java.lang.SecurityException: COLUMN_LOCAL_FILENAME is deprecated; use ContentResolver.openFileDescriptor() instead
      at android.app.DownloadManager$CursorTranslator.getString(DownloadManager.java:1499)
      at cn.teachcourse.download.DownloadManagerActivity.query(DownloadManagerActivity.java:244)
      at cn.teachcourse.download.DownloadManagerActivity.access$100(DownloadManagerActivity.java:34)
      at cn.teachcourse.download.DownloadManagerActivity$1.onReceive(DownloadManagerActivity.java:186)
      at android.app.LoadedApk$ReceiverDispatcher$Args.run(LoadedApk.java:1122)
      at android.os.Handler.handleCallback(Handler.java:751) 
      at android.os.Handler.dispatchMessage(Handler.java:95) 
      at android.os.Looper.loop(Looper.java:154) 
      at android.app.ActivityThread.main(ActivityThread.java:6077) 
      at java.lang.reflect.Method.invoke(Native Method) 
      at com.android.internal.os.ZygoteInit$MethodAndArgsCaller.run(ZygoteInit.java:865) 
      at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:755)
```
### 2.1 关于DownloadManager
`DownloadManager`是一个用于处理长时间HTTP请求的系统服务，客户端请求的URI可能是将要下载的指定的文件，处于后台的下载管理器将控制着下载的任务，并监测下载的状态，在下载失败或连接改变以及系统重启后尝试重新下载。
- 如何初始化`DownloadManager`实例？首先调用`getSystemService(String)`方法，传入`DOWNLOAD_SERVICE`常量，来初始化`DownloadManager`实例，代码如下：
`mDownloadManager = (DownloadManager) getSystemService(DOWNLOAD_SERVICE);`- 如何配置请求参数？首先需要使用到内部类`DownloadManager.Request`，查看源码学习该类的各个方法的使用，TeachCourse简单总结：该类主要用于配置一条新下载任务相关内容，这些内容包括下载任务的保存路径，下载任务所处的网络状态（WiFi或流量状态）和下载任务通知栏显示样式等等，代码如下：
```java
/**
 * 设置请求下载的数据
 */
private void initData() {
    //Request内部类配置新下载任务相关内容，比如：保存路径，WiFi或流量状态，下载通知栏样式
    request = new DownloadManager.Request(Uri.parse(mUrl + mFileName));
    request.setDestinationInExternalPublicDir(Environment.DIRECTORY_DOWNLOADS, mFileName);
    request.setAllowedNetworkTypes(DownloadManager.Request.NETWORK_MOBILE);
    request.setTitle("正在下载应用程序");
    request.setDescription("92回收，就爱回收APP");
    request.setNotificationVisibility(DownloadManager.Request.VISIBILITY_VISIBLE);
}
```
- 如何开启下载任务？下载任务参数配置完成后，就可以开启后台服务下载，同一个`DownloadManager`实例，可以开启多个下载任务，需要上一步中配置多条URI，每个下载任务分配唯一的id，代码如下：
```java
/**
 * 下载任务的唯一标识ID，用于查询下载文件的相关信息
 */
private void start() {
    mDownloadUniqueId = mDownloadManager.enqueue(request);
    mDownloadManager_btn.setText("正在下载。。。");
    mDownloadManager_btn.setClickable(false);
}
```
- `DownloadManager`通过两种状态的广播，第一种：任务下载完成后发送，广播拦截器过滤action是`DownloadManager.ACTION_DOWNLOAD_COMPLETE`（关于广播的知识，不懂的可以参考TeachCourse博客另外的几篇文章）；第二种：点击通知栏进度条后发送，广播拦截器过滤action是`DownloadManager.ACTION_NOTIFICATION_CLICKED`，代码如下：
```java
/**
 * 注册下载完成广播接收器，还可以注册其它监听器，比如：DownloadManager.ACTION_NOTIFICATION_CLICKED
 */
private void registerReceiverCompleted() {
    IntentFilter intentFilter = new IntentFilter(DownloadManager.ACTION_DOWNLOAD_COMPLETE);
    registerReceiver(mBroadcastReceiver, intentFilter);
}
/**
 * 接收下载完成广播
 */
private BroadcastReceiver mBroadcastReceiver = new BroadcastReceiver() {
    @Override
    public void onReceive(Context context, Intent intent) {
        long reference = intent.getLongExtra(DownloadManager.EXTRA_DOWNLOAD_ID, -1);
        if (mDownloadUniqueId == reference) {
            query(reference);
            mShowInformation_tv.setText(information);
            mDownloadManager_btn.setText("点击下载");
            mDownloadManager_btn.setClickable(true);
        }
    }
};
```
- 如何查询下载任务的相关信息？首先需要使用到内部类`DownloadManager.Query`，查看源码学习该类各个方法的使用，TeachCourse简单总结：该类正如文章开头样式的例子，通过分配的id查询下载任务相关的信息，这些信息包括文件类型、文件的Uri和文件的长度等，代码如下：
```java
/**
 * 查询下载任务相关的信息，比如：文件名、文件大小、文件类型等
 *
 * @param reference
 */
private void query(long reference) {
    DownloadManager.Query query = new DownloadManager.Query();
    /**指定查询条件**/
    query.setFilterById(reference);
    /**查询正在等待、运行、暂停、成功、失败状态的下载任务**/
    query.setFilterByStatus(DownloadManager.STATUS_SUCCESSFUL);
    Cursor cursor = mDownloadManager.query(query);
    if (cursor.moveToFirst()) {
        int fileId = cursor.getColumnIndex(DownloadManager.COLUMN_ID);
        int fileTitleId = cursor.getColumnIndex(DownloadManager.COLUMN_TITLE);
        int fileDescriptionId = cursor.getColumnIndex(DownloadManager.COLUMN_DESCRIPTION);
        int fileTypeId = cursor.getColumnIndex(DownloadManager.COLUMN_MEDIA_TYPE);
        int fileLengthId = cursor.getColumnIndex(DownloadManager.COLUMN_TOTAL_SIZE_BYTES);
        int fileUriId = cursor.getColumnIndex(DownloadManager.COLUMN_LOCAL_URI);
        /**过时的方式：DownloadManager.COLUMN_LOCAL_FILENAME**/
        int fileNameId = cursor.getColumnIndex(DownloadManager.COLUMN_LOCAL_FILENAME);
        int statusCodeId = cursor.getColumnIndex(DownloadManager.COLUMN_STATUS);
        int statusReasonId = cursor.getColumnIndex(DownloadManager.COLUMN_REASON);
        int downloadSizeId = cursor.getColumnIndex(DownloadManager.COLUMN_BYTES_DOWNLOADED_SO_FAR);
        int lastModifiedTimeId = cursor.getColumnIndex(DownloadManager.COLUMN_LAST_MODIFIED_TIMESTAMP);
        int mediaUriId = cursor.getColumnIndex(DownloadManager.COLUMN_MEDIAPROVIDER_URI);
        String id = cursor.getString(fileId);
        String fileTitle = cursor.getString(fileTitleId);
        String description = cursor.getString(fileDescriptionId);
        String type = cursor.getString(fileTypeId);
        String length = cursor.getString(fileLengthId);
        String statusCode = cursor.getString(statusCodeId);
        String statusReason = cursor.getString(statusReasonId);
        String downloadSize = cursor.getString(downloadSizeId);
        String modifiedTime = cursor.getString(lastModifiedTimeId);
        String mediaUri = cursor.getString(mediaUriId);
        String fileUri = cursor.getString(fileUriId);
        String fileName = null;
        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.M) {
            openFile(type, Uri.parse(fileUri));
            fileName = Uri.parse(fileUri).getPath();
        } else {
            /**Android 7.0以上的方式：请求获取写入权限，这一步报错**/
            fileName = cursor.getString(fileNameId);
            openFile(type, Uri.parse(fileUri));
        }
        /**清空StringBuffer存储的数据**/
        mStringBuffer.delete(0, mStringBuffer.length());
        mStringBuffer.append("id：" + id + "\n");
        mStringBuffer.append("fileTitle：" + fileTitle + "\n");
        mStringBuffer.append("description：" + description + "\n");
        mStringBuffer.append("type：" + type + "\n");
        mStringBuffer.append("length：" + length + "\n");
        mStringBuffer.append("fileName：" + fileName + "\n");
        mStringBuffer.append("fileUri：" + fileUri + "\n");
        mStringBuffer.append("statusCode：" + statusCode + "\n");
        mStringBuffer.append("statusReason：" + statusReason + "\n");
        mStringBuffer.append("downloadSize：" + downloadSize + "\n");
        mStringBuffer.append("modifiedTime：" + modifiedTime + "\n");
        mStringBuffer.append("mediaUri：" + mediaUri + "\n");
        information = mStringBuffer.toString();
    }
    cursor.close();
}
```
- 代码加入判断语句，如果非Android 7.0系统继续访问`COLUMN_LOCAL_FILENAME`获得文件存储的绝对路径（上面中间部分代码），`openFile()`方法代码如下：
```java
/**
 * 根据文件的类型，指定可以打开的应用程序
 *
 * @param type
 * @param uri
 */
private void openFile(String type, Uri uri) {
    if (type.contains("image/")) {
        try {
            ParcelFileDescriptor descriptor = getContentResolver().openFileDescriptor(uri, "r");
            FileDescriptor fileDescriptor = descriptor.getFileDescriptor();
            Bitmap bitmap = BitmapFactory.decodeFileDescriptor(fileDescriptor);
            mShowPic_iv.setVisibility(View.VISIBLE);
            mShowPic_iv.setImageBitmap(bitmap);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
```
现在，我们已经掌握了`DownloadManager`怎么实例化、怎么配置下载任务、怎么开启后台服务以及如何查询任务相关信息，想要实现一个应用程序版本更新就变得很简单，实现多任务下载也不是难事，完整源码参考文章后台通过的Demo。
## 三、关于ParcelFileDescriptor和FileDescriptor总结
官网的文档推荐我们使用`ContentResolver.openFileDescriptor()`方法，获得一个`ParcelFileDescriptor`对象，再通过`getFileDescriptor()`方法返回一个`FileDescriptor`，它们之间的关系参考上面的代码。
`FileDescriptor`通常被称为*文件描述符*，可以理解成本地的一个文件，通过流的方式读取文件内容以及通过流的方式写入数据到文件，这里是读取或写入数据到`FileDescriptor`中，假如我们的Uri表示的是一个txt文件，获取`FileDescriptor`对象后，通过下面的代码读取txt文件的内容：
```java
FileInputStream fis = new FileInputStream(fd);
```
同理，写入数据到txt文件，代码如下：
```java
FileOutputStream out = new FileOutputStream(fd);
                 out.write('写入数据到txt文件中');
                 out.close();
```
获取到输入流或输出流后，剩下的就是关于流的操作了，划分为：*文件字节流*、*文件字符流*、*缓冲流*、*数组流*等
### 3.1 改写上面的例子
`openFile()`方法使用封装好的`decodeFileDescriptor()`，查看`BitmapFactory.decodeFileDescriptor()`相关源码，学习如何读取文件描述符中的内容，这里TeachCourse根据读取流的方式，改写如下：
```java
...
Bitmap bitmap = BitmapFactory.decodeStream(getStreamByFileDescriptor(fileDescriptor));
   ...
/**
 * 通过流的方式读取内容
 *
 * @param fileDescriptor
 * @return
 */
private InputStream getStreamByFileDescriptor(FileDescriptor fileDescriptor) {
    return new FileInputStream(fileDescriptor);
}
```
于是，可以对`FileDescriptor`进行简单的封装成`writeData()`和`readData()`，代码如下：
```java
/**往FileDescriptor中写入数据
 * @param fileDescriptor
 * @param content
 */
private void writeData(FileDescriptor fileDescriptor, String content) {
    FileOutputStream fos = new FileOutputStream(fileDescriptor);
    try {
        fos.write(content.getBytes());
    } catch (IOException e) {
        e.printStackTrace();
    } finally {
        try {
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
/**从FileDescriptor中读取数据
 * @param fileDescriptor
 * @return
 */
private String readData(FileDescriptor fileDescriptor) {
    FileInputStream fis = new FileInputStream(fileDescriptor);
    byte[] b = new byte[1024];
    int read;
    String content=null;
    try {
        while ((read = fis.read(b)) != -1) {
            content = new String(b, 0, read);
        }
    } catch (IOException e) {
        e.printStackTrace();
    } finally {
        try {
            fis.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    return content;
}
```
## 总结：
Android 7.0系统的权限更改，包括三个方面，文章从第二方面开始讲解，着重介绍了`FileProvider`和`DownloadManager`两个类的使用，花了好长时间整理、测试和编辑，如果对你有帮忙，别忘了收藏和分享咯！
- FileProvider源码路径：`nougat/WriteToReadActivity.java`
- DownloadManager源码路径：`download/DownloadActivity.java`
- [Demo源码](https://github.com/TeachCourse/AllDemos)
- 
> 
参考资料：[https://developer.android.google.cn/about/versions/nougat/android-7.0-changes.html](https://developer.android.google.cn/about/versions/nougat/android-7.0-changes.html)

