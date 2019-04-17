# 《Training：Retrieving File Information》 - 一世豁然的专栏 - CSDN博客





2017年04月22日 15:17:54[一世豁然](https://me.csdn.net/Explorer_day)阅读数：329标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。







在客户端应用程序尝试使用具有内容URI的文件之前，应用程序可以从服务器应用程序请求有关文件的信息，包括文件的数据类型和文件大小。 数据类型帮助客户端应用程序确定是否可以处理该文件，并且文件大小有助于客户端应用程序为文件设置缓冲和缓存。




本课程演示如何查询服务器应用程序的FileProvider以检索文件的MIME类型和大小。








一、检索文件的MIME类型

文件的数据类型向客户端应用程序指示如何处理文件的内容。 要获取其内容URI的共享文件的数据类型，客户端应用程序将调用ContentResolver.getType（）。 此方法返回文件的MIME类型。 默认情况下，FileProvider根据文件扩展名确定文件的MIME类型。





以下代码段演示了客户端应用程序如何在服务器应用程序将内容URI返回给客户端后检索文件的MIME类型：

```java
...
    /*
     * Get the file's content URI from the incoming Intent, then
     * get the file's MIME type
     */
    Uri returnUri = returnIntent.getData();
    String mimeType = getContentResolver().getType(returnUri);
    ...
```








二、检索文件的名称和大小

FileProvider类具有query（）方法的默认实现，该方法返回与Cursor中的内容URI相关联的文件的名称和大小。 默认实现返回两列：


[DISPLAY_NAME](https://developer.android.google.cn/reference/android/provider/OpenableColumns.html#DISPLAY_NAME)：该文件的名称，作为一个字符串。
 该值与File.getName（）返回的值相同。


[SIZE](https://developer.android.google.cn/reference/android/provider/OpenableColumns.html#SIZE)：文件的大小以字节为单位，长度与File.length（）返回的值相同。





除了内容URI之外，客户端应用程序可以通过将query（）的所有参数设置为null，来获取文件的DISPLAY_NAME和SIZE。例如，此代码段检索文件的DISPLAY_NAME和SIZE，并将其显示在单独的TextView中：



```java
...
    /*
     * Get the file's content URI from the incoming Intent,
     * then query the server app to get the file's display name
     * and size.
     */
    Uri returnUri = returnIntent.getData();
    Cursor returnCursor =
            getContentResolver().query(returnUri, null, null, null, null);
    /*
     * Get the column indexes of the data in the Cursor,
     * move to the first row in the Cursor, get the data,
     * and display it.
     */
    int nameIndex = returnCursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
    int sizeIndex = returnCursor.getColumnIndex(OpenableColumns.SIZE);
    returnCursor.moveToFirst();
    TextView nameView = (TextView) findViewById(R.id.filename_text);
    TextView sizeView = (TextView) findViewById(R.id.filesize_text);
    nameView.setText(returnCursor.getString(nameIndex));
    sizeView.setText(Long.toString(returnCursor.getLong(sizeIndex)));
    ...
```







