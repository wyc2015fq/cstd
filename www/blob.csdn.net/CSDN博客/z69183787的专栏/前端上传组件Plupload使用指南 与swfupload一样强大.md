# 前端上传组件Plupload使用指南 与swfupload一样强大 - z69183787的专栏 - CSDN博客
2015年06月30日 18:00:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2041
[Plupload](http://www.plupload.com/)是一款由著名的web编辑器[TinyMCE](http://www.tinymce.com/)团队开发的上传组件，简单易用且功能强大，我们完全可以使用Plupload来代替以前的SWFUpload。
**Plupload有以下功能和特点：**
1、拥有多种上传方式：HTML5、flash、silverlight以及传统的<input type=”file” />。Plupload会自动侦测当前的环境，选择最合适的上传方式，并且会优先使用HTML5的方式。所以你完全不用去操心当前的浏览器支持哪些上传方式，Plupload会自动为你选择最合适的方式。
2、支持以拖拽的方式来选取要上传的文件
3、支持在前端压缩图片，即在图片文件还未上传之前就对它进行压缩
4、可以直接读取原生的文件数据，这样的好处就是例如可以在图片文件还未上传之前就能把它显示在页面上预览
5、支持把大文件切割成小片进行上传，因为有些浏览器对很大的文件比如几G的一些文件无法上传。
**Plupload的使用方法也与SWFUpload非常类似，可以分为以下几步：**
1、引入js文件
2、实例化一个plupload对象，传入一个配置参数对象进行各方面的配置。
3、调用plupload实例对象的init()方法进行初始化
4、在plupload实例对象上注册各种你需要的事件。plupload从选取文件到文件上传完成这个过程中，会触发很多事件。我们可以通过这些事件来跟plupload进行交互。
5、实现你自己所注册的那些事件的监听函数，利用这些监听函数来进行更新UI、提示上传进度等工作。
大家可以到[http://chaping.github.io/plupload/demo/](http://chaping.github.io/plupload/demo/)看一下我写的关于plupload的几个上传demo。
下面用一段代码来说明Plupload的使用方法。
首先看下我的目录结构
![目录结构](http://images.cnitblog.com/blog/130623/201408/141813455612197.png)
index.html的代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Plupload使用指南</title>
    <!-- 首先需要引入plupload的源代码 -->
    <script src="js/plupload.full.min.js"></script>
</head>
<body>
    <!-- 这里我们只使用最基本的html结构：一个选择文件的按钮，一个开始上传文件的按钮(甚至该按钮也可以不要) -->
    <p>
        <button id="browse">选择文件</button>
        <button id="start_upload">开始上传</button>
    </p>
    <script>
    //实例化一个plupload上传对象
    var uploader = new plupload.Uploader({
        browse_button : 'browse', //触发文件选择对话框的按钮，为那个元素id
        url : 'upload.php', //服务器端的上传页面地址
        flash_swf_url : 'js/Moxie.swf', //swf文件，当需要使用swf方式进行上传时需要配置该参数
        silverlight_xap_url : 'js/Moxie.xap' //silverlight文件，当需要使用silverlight方式进行上传时需要配置该参数
    });    
    //在实例对象上调用init()方法进行初始化
    uploader.init();
    //绑定各种事件，并在事件监听函数中做你想做的事
    uploader.bind('FilesAdded',function(uploader,files){
        //每个事件监听函数都会传入一些很有用的参数，
        //我们可以利用这些参数提供的信息来做比如更新UI，提示上传进度等操作
    });
    uploader.bind('UploadProgress',function(uploader,file){
        //每个事件监听函数都会传入一些很有用的参数，
        //我们可以利用这些参数提供的信息来做比如更新UI，提示上传进度等操作
    });
    //......
    //......
    //最后给"开始上传"按钮注册事件
    document.getElementById('start_upload').onclick = function(){
        uploader.start(); //调用实例对象的start()方法开始上传文件，当然你也可以在其他地方调用该方法
    }
    </script>
</body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
使用Plupload的关键是了解它众多的配置参数、事件以及属性和方法。我把它官网上的文档翻译成了中文并整理如下。我也把它放到了github上，你可以到[http://chaping.github.io/plupload/doc/](http://chaping.github.io/plupload/doc/)去查阅。
目录：
[一、配置参数](http://www.cnblogs.com/2050/p/3913184.html#plupload_doc1)
[二、各种事件说明](http://www.cnblogs.com/2050/p/3913184.html#plupload_doc2)
[三、Plupload实例的属性](http://www.cnblogs.com/2050/p/3913184.html#plupload_doc3)
[四、Plupload实例的方法](http://www.cnblogs.com/2050/p/3913184.html#plupload_doc4)
[五、文件对象的属性和方法](http://www.cnblogs.com/2050/p/3913184.html#plupload_doc5)
[六、QueueProgress 对象的属性](http://www.cnblogs.com/2050/p/3913184.html#plupload_doc6)
[七、plupload命名空间上的一些属性](http://www.cnblogs.com/2050/p/3913184.html#plupload_doc7)
**一、配置参数**
实例化一个plupload对象时，也就是 `new plupload.Uploader()`，需要传入一个对象作为配置参数。后面内容中出现的plupload实例均是指`new plupload.Uploader()`得到的实例对象
|属性|类型|默认值|描述|
|----|----|----|----|
|String / DOM||触发文件选择对话框的DOM元素，当点击该元素后便后弹出文件选择对话框。该值可以是DOM元素对象本身，也可以是该DOM元素的id| |
|String||服务器端接收和处理上传文件的脚本地址，可以是相对路径(相对于当前调用Plupload的文档)，也可以是绝对路径| |
|Object|{ }|可以使用该参数来限制上传文件的类型，大小等，该参数以对象的形式传入，它包括三个属性：**mime_types：**用来限定上传文件的类型，为一个数组，该数组的每个元素又是一个对象，该对象有title和extensions两个属性，title为该过滤器的名称，extensions为文件扩展名，有多个时用逗号隔开。该属性默认为一个空数组，即不做限制。**max_file_size：**用来限定上传文件的大小，如果文件体积超过了该值，则不能被选取。值可以为一个数字，单位为b,也可以是一个字符串，由数字和单位组成，如'200kb'**prevent_duplicates：**是否允许选取重复的文件，为true时表示不允许，为false时表示允许，默认为false。如果两个文件的文件名和大小都相同，则会被认为是重复的文件filters完整的配置示例如下(当然也可以只配置其中的某一项)：filters: {  mime_types : [ //只允许上传图片和zip文件    { title : "Image files", extensions : "jpg,gif,png" },     { title : "Zip files", extensions : "zip" }  ],  max_file_size : '400kb', //最大只能上传400kb的文件  prevent_duplicates : true //不允许选取重复文件}| |
|Object||设置上传时的自定义头信息，以键/值对的形式传入，键代表头信息属性名，键代表属性值。html4上传方式不支持设置该属性。| |
|Boolean|true|为`true`时将以`multipart/form-data`的形式来上传文件，为false时则以二进制的格式来上传文件。html4上传方式不支持以二进制格式来上传文件，在flash上传方式中，二进制上传也有点问题。并且二进制格式上传还需要在服务器端做特殊的处理。一般我们用multipart/form-data的形式来上传文件就足够了。| |
|Object||上传时的附加参数，以键/值对的形式传入，服务器端可是使用$_POST来获取这些参数(以php为例)。比如：multipart_params: {  one: '1',  two: '2',  three: { //值还可以是一个字面量对象    a: '4',    b: '5'  },  four: ['6', '7', '8']  //也可以是一个数组}| |
|Number|0|当发生`plupload.HTTP_ERROR`错误时的重试次数，为0时表示不重试| |
|Number/String|0|分片上传文件时，每片文件被切割成的大小，为数字时单位为字节。也可以使用一个带单位的字符串，如`"200kb"`。当该值为0时表示不使用分片上传功能| |
|Object||可以使用该参数对将要上传的图片进行压缩，该参数是一个对象，里面包括5个属性：**width：**指定压缩后图片的宽度，如果没有设置该属性则默认为原始图片的宽度**height：**指定压缩后图片的高度，如果没有设置该属性则默认为原始图片的高度**crop：**是否裁剪图片**quality：**压缩后图片的质量，只对jpg格式的图片有效，默认为90。`quality`可以跟`width`和`height`一起使用，但也可以单独使用，单独使用时，压缩后图片的宽高不会变化，但由于质量降低了，所以体积也会变小**preserve_headers：**压缩后是否保留图片的元数据，`true`为保留，`false`为不保留,默认为`true`。删除图片的元数据能使图片的体积减小一点点resize参数的配置示例如下：resize: {  width: 100,  height: 100,  crop: true,  quality: 60,  preserve_headers: false}| |
|DOM/String/Array||指定了使用拖拽方式来选择上传文件时的拖拽区域，即可以把文件拖拽到这个区域的方式来选择文件。该参数的值可以为一个DOM元素的id,也可是DOM元素本身，还可以是一个包括多个DOM元素的数组。如果不设置该参数则拖拽上传功能不可用。目前只有html5上传方式才支持拖拽上传。| |
|Boolean|true|是否可以在文件浏览对话框中选择多个文件，true为可以，false为不可以。默认true，即可以选择多个文件。需要注意的是，在某些不支持多选文件的环境中，默认值是false。比如在ios7的safari浏览器中，由于存在bug，造成不能多选文件。当然，在html4上传方式中，也是无法多选文件的。| |
|Mix||可以使用该参数来设置你必须需要的一些功能特征，Plupload会根据你的设置来选择合适的上传方式。因为，不同的上传方式，支持的功能是不同的，比如拖拽上传只有html5上传方式支持，图片压缩则只有html5,flash,silverlight上传方式支持。该参数的值是一个混合类型，可以是一个以逗号分隔的字符串，| |
|Boolean|false|当值为true时会为每个上传的文件生成一个唯一的文件名，并作为额外的参数post到服务器端，参数明为`name`,值为生成的文件名。| |
|String|html5,flash,silverlight,html4|用来指定上传方式，指定多个上传方式请使用逗号隔开。一般情况下，你不需要配置该参数，因为Plupload默认会根据你的其他的参数配置来选择最合适的上传方式。如果没有特殊要求的话，Plupload会首先选择html5上传方式，如果浏览器不支持html5，则会使用flash或silverlight，如果前面两者也都不支持，则会使用最传统的html4上传方式。如果你想指定使用某个上传方式，或改变上传方式的优先顺序，则你可以配置该参数。| |
|String|file|指定文件上传时文件域的名称，默认为`file`,例如在php中你可以使用`$_FILES['file']`来获取上传的文件信息| |
|DOM/String||用来指定Plupload所创建的html结构的父容器，默认为前面指定的`browse_button`的父元素。该参数的值可以是一个元素的id,也可以是DOM元素本身。| |
|String|js/Moxie.swf|flash上传组件的url地址，如果是相对路径，则相对的是调用Plupload的html文档。当使用flash上传方式会用到该参数。| |
|String|js/Moxie.xap|silverlight上传组件的url地址，如果是相对路径，则相对的是调用Plupload的html文档。当使用silverlight上传方式会用到该参数。| |
**二、各种事件说明**
要了解plupload的运行状况，靠的就是在这些事件了
当调用destroy方法时触发
监听函数参数：**(uploader)**
`uploader`为当前的plupload实例对象
**三、Plupload实例的属性**
|属性|描述|
|----|----|
|Plupload实例的唯一标识id| |
|当前的上传状态，可能的值为`plupload.STARTED`或`plupload.STOPPED`，该值由Plupload实例的`stop()`或`statr()`方法控制。默认为`plupload.STOPPED`| |
|当前使用的上传方式| |
|当前的上传队列，是一个由上传队列中的文件对象组成的数组| |
|当前的配置参数对象| |
|表示总体进度信息的QueueProgress对象| |
**四、Plupload实例的方法**
|方法|描述|
|----|----|
|初始化Plupload实例| |
|设置某个特定的配置参数,option为参数名称，value为要设置的参数值。option也可以为一个由参数名和参数值键/值对组成的对象，这样就可以一次设定多个参数，此时该方法的第二个参数value会被忽略。| |
|获取当前的配置参数，参数option为需要获取的配置参数名称，如果没有指定option，则会获取所有的配置参数| |
|刷新当前的plupload实例，暂时还不明白什么时候需要使用| |
|开始上传队列中的文件| |
|停止队列中的文件上传| |
|禁用或启用plupload的文件浏览按钮,参数`disable`为`true`时为禁用，为`false`时为启用。默认为`true`| |
|通过id来获取文件对象| |
|向上传队列中添加文件，如果成功添加了文件，会触发`FilesAdded`事件。参数file为要添加的文件,可以是一个原生的文件,或者一个plupload文件对象,或者一个`input[type="file"]`元素,还可以是一个包括前面那几种东西的数组；fileName为给该文件指定的名称| |
|从上传队列中移除文件，参数`file`为plupload文件对象或先前指定的文件名称| |
|从上传队列中移除一部分文件，`start`为开始移除文件在队列中的索引，`length`为要移除的文件的数量，该方法的返回值为被移除的文件。该方法会触发`FilesRemoved `和`QueueChanged`事件| |
|触发某个事件。`name`为要触发的事件名称，`Multiple`为传给该事件监听函数的参数，是一个对象| |
|用来判断某个事件是否有监听函数，`name`为事件名称| |
|给某个事件绑定监听函数，`name`为事件名，`func`为监听函数，`scope`为监听函数的作用域，也就是监听函数中this的指向| |
|移除事件的监听函数，`name`为事件名称，`func`为要移除的监听函数| |
|移除所有事件的所有监听函数| |
|销毁plupload实例| |
**五、文件对象的属性和方法**
在很多事件监听函数中，都会提供文件对象给你
|属性/方法|描述|
|----|----|
|文件id| |
|文件名，例如"myfile.gif"| |
|文件类型，例如"image/jpeg"| |
|文件大小，单位为字节，当启用了客户端压缩功能后，该值可能会改变| |
|文件的原始大小，单位为字节| |
|文件已上传部分的大小，单位为字节| |
|文件已上传部分所占的百分比，如50就代表已上传了50%| |
|文件的状态，可能为以下几个值之一：`plupload.QUEUED`, `plupload.UPLOADING`,`plupload.FAILED`, `plupload.DONE`| |
|文件最后修改的时间| |
|获取原生的文件对象| |
|获取mOxie.File 对象，想了解mOxie是什么东西，可以看下https://github.com/moxiecode/moxie/wiki/API| |
|销毁文件对象| |
**六、QueueProgress 对象的属性**
plupload实例的total属性是一个QueueProgress对象
|属性|描述|
|----|----|
|上传队列中所有文件加起来的总大小，单位为字节| |
|队列中当前已上传文件加起来的总大小,单位为字节| |
|已完成上传的文件的数量| |
|上传失败的文件数量| |
|队列中剩下的(也就是除开已经完成上传的文件)需要上传的文件数量| |
|整个队列的已上传百分比，如50就代表50%| |
|上传速率，单位为 byte/s，也就是 字节/秒| |
**七、plupload命名空间上的一些属性**
plupload的命名空间上有一些属性，用来表示一些常量。记住，不是plupload实例的属性，而是plupload的属性
|属性名称|描述|
|----|----|
|当前plupload的版本号| |
|值为1，代表上传队列还未开始上传或者上传队列中的文件已经上传完毕时plupload实例的`state`属性值| |
|值为2，代表队列中的文件正在上传时plupload实例的`state`属性值| |
|值为1，代表某个文件已经被添加进队列等待上传时该文件对象的`status`属性值| |
|值为2，代表某个文件正在上传时该文件对象的`status`属性值| |
|值为4，代表某个文件上传失败后该文件对象的`status`属性值| |
|值为5，代表某个文件上传成功后该文件对象的`status`属性值| |
|值为-100，发生通用错误时的错误代码| |
|值为-200，发生http网络错误时的错误代码，例如服务气端返回的状态码不是200| |
|值为-300，发生磁盘读写错误时的错误代码，例如本地上某个文件不可读| |
|值为-400，发生因为安全问题而产生的错误时的错误代码| |
|值为-500，初始化时发生错误的错误代码| |
|值为-600，当选择的文件太大时的错误代码| |
|值为-601，当选择的文件类型不符合要求时的错误代码| |
|值为-602，当选取了重复的文件而配置中又不允许有重复文件时的错误代码| |
|值为-700，发生图片格式错误时的错误代码| |
|当发生内存错误时的错误代码| |
|值为-702，当文件大小超过了plupload所能处理的最大值时的错误代码| |
