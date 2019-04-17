# 《Training：Setting Up File Sharing》 - 一世豁然的专栏 - CSDN博客





2017年04月22日 11:14:50[一世豁然](https://me.csdn.net/Explorer_day)阅读数：309








注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。







要安全地将应用程序中的文件提供给另一个应用程序，您需要配置应用程序，以内容URI的形式为文件提供安全的句柄。 Android FileProvider组件根据您在XML中提供的规范生成文件的内容URI。 本课程向您展示如何将FileProvider的默认实现添加到您的应用程序，以及如何指定要提供给其他应用程序的文件。





注意：FileProvider类是v4支持库的一部分。 有关在应用程序中包含此库的信息，请参阅支持库设置。








一、指定FileProvider

为您的应用定义FileProvider需要您的清单中的条目。 此条目指定了在生成内容URI时使用的权限，以及指定应用程序可以共享的目录的XML文件的名称。





以下代码段显示了如何向清单添加指定FileProvider类，权限和XML文件名的<provider>元素：




```
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.example.myapp">
    <application
        ...>
        <provider
            android:name="android.support.v4.content.FileProvider"
            android:authorities="com.example.myapp.fileprovider"
            android:grantUriPermissions="true"
            android:exported="false">
            <meta-data
                android:name="android.support.FILE_PROVIDER_PATHS"
                android:resource="@xml/filepaths" />
        </provider>
        ...
    </application>
</manifest>
```


在这个例子中，android：authority属性指定要用于FileProvider生成的内容URI的URI权限。 在该示例中，权限为com.example.myapp.fileprovider。 对于您自己的应用程序，请指定由应用程序的android：package值组成的权限，并附加字符串“fileprovider”。 要了解有关权限值的更多信息，请参阅内容URI和android:authorities属性的文档。




<provider>的<meta-data>子元素指向一个XML文件，用于指定要共享的目录。 android:resource属性是文件的路径和名称，没有.xml扩展名。该文件的内容将在下一节中介绍。








二、指定可分享的目录

将FileProvider添加到应用程序清单后，需要指定包含要共享的文件的目录。 要指定目录，首先在项目的res / xml /子目录中创建filepaths.xml文件。 在此文件中，通过为每个目录添加一个XML元素来指定目录。 以下片段显示了res / xml / filepaths.xml的内容示例。 该片段还演示了如何在内部存储区域中共享 files/ directory的子目录：




```
<paths>
    <files-path path="images/" name="myimages" />
</paths>
```


在此示例中，<files-path>标签在应用程序内部存储的 files/ 目录中共享目录。 path属性共享文件/的images /子目录。 name属性告诉FileProvider将路径碎片myimages添加到files / images /子目录的内容URI文件中。




<paths>元素可以有多个子节点，每个子节点指定一个不同的共享目录。 除了<files-path>元素之外，您可以使用<external-path>元素在外部存储中共享目录，以及<cache-path>元素来共享内部缓存目录中的目录。 要了解有关指定共享目录的子元素的更多信息，请参阅FileProvider参考文档。





注意：XML文件是您可以指定要共享的目录的唯一方法; 您无法以编程方式添加目录。





您现在有一个FileProvider的完整规范，它可以为应用程序内部存储的files/目录中的文件或files/子目录中的文件生成内容URI。当您的应用程序生成文件的内容URI时，它包含在<provider>元素（com.example.myapp.fileprovider）中指定的权限，路径myimages /和文件的名称。





例如，如果您根据本课程中的碎片定义FileProvider，并且为文件default_image.jpg请求内容URI，FileProvider将返回以下URI：



`content://com.example.myapp.fileprovider/myimages/default_image.jpg`




