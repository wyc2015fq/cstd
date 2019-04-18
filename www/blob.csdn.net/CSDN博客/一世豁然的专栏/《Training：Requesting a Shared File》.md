# 《Training：Requesting a Shared File》 - 一世豁然的专栏 - CSDN博客





2017年04月22日 15:06:27[一世豁然](https://me.csdn.net/Explorer_day)阅读数：294标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方博客。







当应用程式想要存取其他应用程式共用的文件时，请求app（客户端）通常会向共享文件（服务器）的app发送一个要求。 在大多数情况下，此请求在服务器应用程序中启动一个活动，显示可以共享的文件。 用户选择一个文件，之后服务器应用程序将文件的内容URI返回给客户端应用程序。




本课程向您展示客户端应用程序如何从服务器应用程序请求文件，从服务器应用程序接收文件的内容URI，并使用内容URI打开文件。








一、发送文件请求

要从服务器应用程序请求文件，客户端应用程序会调用包含服务器app能处理的例如 ACTION_PICK的动作以及一个MIME类型的intent的startActivityForResult 方法。





例如，以下代码段演示了如何将Intent发送到服务器应用程序，以便启动共享文件中描述的活动：




```java
public class MainActivity extends Activity {
    private Intent mRequestFileIntent;
    private ParcelFileDescriptor mInputPFD;
    ...
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mRequestFileIntent = new Intent(Intent.ACTION_PICK);
        mRequestFileIntent.setType("image/jpg");
        ...
    }
    ...
    protected void requestFile() {
        /**
         * When the user requests a file, send an Intent to the
         * server app.
         * files.
         */
            startActivityForResult(mRequestFileIntent, 0);
        ...
    }
    ...
}
```



二、访问请求的文件

服务器应用程序将文件的内容URI发送到Intent中的客户端应用程序。 该意图在其overActivityResult（）的重写方法中传递给客户端应用程序。 一旦客户端应用程序拥有该文件的内容URI，它可以通过获取其FileDescriptor来访问该文件。





文件安全性保留在此过程中，因为内容URI是客户端应用程序接收的唯一数据。 由于此URI不包含目录路径，客户端应用程序无法在服务器应用程序中发现和打开任何其他文件。 只有客户端应用程序可以访问该文件，并且只能由服务器应用程序授予的权限。 权限是临时的，所以一旦客户端应用程序的任务堆栈完成，文件就不能在服务器应用程序之外访问。





下一个代码段演示了客户端应用程序如何处理从服务器应用程序发送的Intent，以及客户端应用程序如何使用内容URI获取FileDescriptor：




```java
/*
     * When the Activity of the app that hosts files sets a result and calls
     * finish(), this method is invoked. The returned Intent contains the
     * content URI of a selected file. The result code indicates if the
     * selection worked or not.
     */
    @Override
    public void onActivityResult(int requestCode, int resultCode,
            Intent returnIntent) {
        // If the selection didn't work
        if (resultCode != RESULT_OK) {
            // Exit without doing anything else
            return;
        } else {
            // Get the file's content URI from the incoming Intent
            Uri returnUri = returnIntent.getData();
            /*
             * Try to open the file for "read" access using the
             * returned URI. If the file isn't found, write to the
             * error log and return.
             */
            try {
                /*
                 * Get the content resolver instance for this context, and use it
                 * to get a ParcelFileDescriptor for the file.
                 */
                mInputPFD = getContentResolver().openFileDescriptor(returnUri, "r");
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                Log.e("MainActivity", "File not found.");
                return;
            }
            // Get a regular file descriptor for the file
            FileDescriptor fd = mInputPFD.getFileDescriptor();
            ...
        }
    }
```


方法openFileDescriptor（）返回该文件的ParcelFileDescriptor。 使用该对象，客户端应用程序获取一个FileDescriptor对象，然后可以使用它来读取该文件。



