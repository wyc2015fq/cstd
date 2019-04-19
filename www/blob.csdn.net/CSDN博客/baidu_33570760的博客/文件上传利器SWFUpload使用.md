# 文件上传利器SWFUpload使用 - baidu_33570760的博客 - CSDN博客
2017年08月29日 14:44:22[carman_风](https://me.csdn.net/baidu_33570760)阅读数：297
原文链接：http://www.cnblogs.com/2050/archive/2012/08/29/2662932.html
SWFUpload是一个flash和js相结合而成的文件上传插件，其功能非常强大。以前在项目中用过几次，但它的配置参数太多了，用过后就忘记怎么用了，到以后要用时又得到官网上看它的文档，真是太烦了。所以索性就把它的用法记录下来，也方便英语拙计的同学查看，利人利己，一劳永逸。
**SWFUpload的特点：**
1、用flash进行上传，页面无刷新，且可自定义Flash按钮的样式;
2、可以在浏览器端就对要上传的文件进行限制;
3、允许一次上传多个文件，但会有一个上传队列，队列里文件的上传是逐个进行的，服务器端接收文件时跟普通的表单上传文件是一样的;
4、提供了丰富的事件接口供开发者使用;
**SWFUpload的文件上传流程是这样的：**
1、引入相应的js文件
2、实例化SWFUpload对象，传入一个配置参数对象进行各方面的配置。
3、点击SWFUpload提供的Flash按钮，弹出文件选取窗口选择要上传的文件；
4、文件选取完成后符合规定的文件会被添加到上传的队列里；
5、调用startUpload方法让队列里文件开始上传；
6、文件上传过程中会触发相应的事件，开发者利用这些事件来更新ui、处理错误、发出提示等等；
**SWFUpload包括三部分的内容**：SWFUpload.js、swfupload.swf、初始化配置参数及各种事件处理函数。所以首先在页面引入SWFUpload.js
1
```java
<script src='SWFUpload.js'></script>
```
然后实例化一个SWFUpload对象：
```java
var swfu;
     
window.onload = function () {
    var settings_object = {//定义参数配置对象
        upload_url : "http://www.swfupload.org/upload.php",
            flash_url : "http://www.swfupload.org/swfupload.swf",
            file_post_name : "Filedata",
            post_params : {
           "post_param_name_1" : "post_param_value_1",
           "post_param_name_2" : "post_param_value_2",
           "post_param_name_n" : "post_param_value_n"
            },
            use_query_string : false,
            requeue_on_error : false,
            http_success : [201, 202],
            assume_success_timeout : 0,
            file_types : "*.jpg;*.gif",
            file_types_description: "Web Image Files",
            file_size_limit : "1024",
            file_upload_limit : 10,
            file_queue_limit : 2,
 
            debug : false,
     
            prevent_swf_caching : false,
            preserve_relative_urls : false,
     
            button_placeholder_id : "element_id",
            button_image_url : "http://www.swfupload.org/button_sprite.png",
            button_width : 61,
            button_height : 22,
            button_text : "<b>Click</b> <span class="redText">here</span>",
            button_text_style : ".redText { color: #FF0000; }",
            button_text_left_padding : 3,
            button_text_top_padding : 2,
            button_action : SWFUpload.BUTTON_ACTION.SELECT_FILES,
            button_disabled : false,
            button_cursor : SWFUpload.CURSOR.HAND,
            button_window_mode : SWFUpload.WINDOW_MODE.TRANSPARENT,
     
            swfupload_loaded_handler : swfupload_loaded_function,
            file_dialog_start_handler : file_dialog_start_function,
            file_queued_handler : file_queued_function,
            file_queue_error_handler : file_queue_error_function,
            file_dialog_complete_handler : file_dialog_complete_function,
            upload_start_handler : upload_start_function,
            upload_progress_handler : upload_progress_function,
            upload_error_handler : upload_error_function,
            upload_success_handler : upload_success_function,
            upload_complete_handler : upload_complete_function,
            debug_handler : debug_function,
    };
     
    swfu = new SWFUpload(settings_object);//实例化一个SWFUpload，传入参数配置对象
};
/*定义各种事件监听函数*/
function swfupload_loaded_function(){}
function file_dialog_start_function(){}
...等等
```
||||||
我们看到要实现一个swfupload上传功能很简单，就是实例化一个swfupload对象。但繁琐的地方就在于实例化实要用到的参数配置对象，以及各种事件的发生时机以和提供的参数。所以重点来了。下面几个表格对开发中要用到的东西列举了出来，虽然已经蛮多了，但并不是swfupload的全部，我列出来的只是常用的。要查看完整的文档，请到[swfupload](http://demo.swfupload.org/Documentation/)官网上查询。
**一、配置参数对象中的常用属性及说明**
|属性|类型|默认值|描述|
|----|----|----|----|
|String||处理上传文件的服务器端页面的url地址，可以是绝对地址，也可以是相对地址，当为相对地址时相对的是当前代码所在的文档地址| |
|Boolean|false|如果为false则SWFUpload会把swfupload.swf用到的相对地址转换为绝对地址，以达到更好的兼容性| |
|String|Filedata|相当于用普通的文件域上传文件时的name属性，服务器端接收页面通过该名称来获取上传的文件| |
|Object(直接量)||一个对象直接量，里面的键/值对会随着每一个文件一起上传，文件上传要附加一些信息时很有用| |
|Boolean|false|为false时,post_params属性定义的参数会以post方式上传；为true时，则会以get方式上传（即参数会以查询字符串的形式附加到url后面）| |
|String||该属性指定了允许上传的文件类型，当有多个类型时使用分号隔开，比如：*.jpg;*.png ,允许所有类型时请使用 *.*| |
|String||指定在文件选取窗口中显示的文件类型描述，起一个提示和说明的作用吧| |
|String||指定要上传的文件的最大体积，可以带单位，合法的单位有:B、KB、MB、GB，如果省略了单位，则默认为KB。该属性为0时，表示不限制文件的大小。| |
|Number||指定最多能上传多少个文件，当上传成功的文件数量达到了这个最大值后，就不能再上传文件了，也不能往上传队列里添加文件了。把该属性设为0时表示不限制文件的上传数量。| |
|Number||指定文件上传队列里最多能同时存放多少个文件。当超过了这个数目后只有当队列里有文件上传成功、上传出错或被取消上传后，等同数量的其他文件才可以被添加进来。当file_upload_limit的数值或者剩余的能上传的文件数量小于file_queue_limit时，则取那个更小的值| |
|String||swfupload.swf文件的绝对或相对地址，相对地址是指相对于当前的页面地址。实例化swfupload后，就不能再改变该属性的值了。| |
|Boolean||为true时会加一个随机数在swfupload.swf地址的后面，以阻止flash影片被缓存，这是为了防止某些版本的IE浏览器在读取缓存的falsh影片时出现的bug| |
|String||指定一个dom元素的id,该dom元素在swfupload实例化后会被Flash按钮代替，这个dom元素相当于一个占位符| |
|DOMElement||指定一个dom元素,该dom元素在swfupload实例化后会被Flash按钮代替，这个dom元素相当于一个占位符。当button_placeholder_id与button_placeholder都存在时，以button_placeholder_id为优先| |
|String||指定Flash按钮的背景图片，相对地址或绝对地址都可以。该地址会受到preserve_relative_urls属性的影响，遵从与upload_url一样的规则。该背景图片必须是一个sprite图片,从上到下包含了Flash按钮的正常、鼠标悬停、按下、禁用这四种状态。因此该图片的高度应该是Flash按钮高度的四倍| |
|Number||指定Flash按钮的宽度| |
|Number||指定Flash按钮的高度，应该为button_image_url所指定的按钮背景图片高度的1/4| |
|String||指定Flash按钮上的文字，也可以是html代码| |
|String||Flash按钮上的文字的样式，使用方法见示例| |
|Number||指定Flash按钮顶部的内边距，可使用负值| |
|Number||指定Flash按钮左边的内边距，可使用负值| |
|Boolean|false|为true时Flash按钮将变为禁用状态，点击也不会触发任何行为| |
|||指定鼠标悬停在Flash按钮上时的光标样式，可用值为SWFUpload.CURSOR里定义的常量| |
|||指定Flash按钮的WMODE属性，可用值为SWFUpload.WINDOW_MODE里定义的常量| |
|Function||fileDialogStart事件侦听函数| |
|Function||fileQueued事件侦听函数| |
|Function||fileQueueError事件侦听函数| |
|Function||fileDialogComplete事件侦听函数| |
|Function||uploadStart事件侦听函数| |
|Function||uploadProgress事件侦听函数| |
|Function||uploadError事件侦听函数| |
|Function||uploadSuccess事件侦听函数| |
|Function||uploadComplete事件侦听函数| |
**二、各种事件说明**
要实现与用户的交互，靠的就是在这些事件上做文章了
当一次文件上传的流程完成时（不管是成功的还是不成功的）会触发该事件，该事件表明本次上传已经完成，上传队列里的下一个文件可以开始上传了。该事件发生后队列中下一个文件的上传将会开始
**三、swfupload实例的方法**
方法中大多数是动态改变参数配置对象的方法
动态设置配置对象中button_cursor的值
**四、文件信息对象 File Object**
在事件监听函数中，经常要用到文件信息对象来获取文件的信息以供下一步的操作
|属性|类型|描述|
|----|----|----|
|String|SWFUpload定义的文件id,用来控制文件的上传| |
|Number|文件的索引，用在getFile(i)方法中| |
|String|文件的原始名称，不包括路径| |
|String|文件类型| |
|Date|文件的创建日期| |
|Date|文件的最后修改日期| |
|Number|当前文件的状态，详细的请参照SWFUpload.FILE_STATUS中定义的常量| |
**五、队列状态对象 Stats Object**
用来获取当前队列的状况
|属性|类型|描述|
|----|----|----|
|Number|得到的值为1或0，表明当前队列是否有文件正在上传中| |
|Number|目前上传队列中的文件数量| |
|Number|已成功上传(指触发了uploadSuccess事件)的文件数量| |
|Number|上传失败的文件数量(包括被取消上传的文件)| |
|Number|被取消上传的文件数量| |
|Number|触发了fileQueueError事件的文件数量| |
**六、一些常量**
定义的一些常量，便于理解
|常量名|描述|
|----|----|
|该常量是一个对象，代表一个页面上所有的SWFUpload实例的引用的集合，用SWFUpload实例的movieName属性进行索引| |
|页面上存在的SWFUpload实例的数量| |
|| |
|用户选取的文件超过了允许的数量| |
|文件的体积超过了允许的大小| |
|文件是空的| |
|不允许的文件类型| |
|| |
|服务器返回的状态码不是200| |
|没有设置 upload_url| |
|读取或传输文件时发生错误| |
|上传受到了安全方面的限制| |
|上传的文件数量超过了允许的最大值| |
|上传出现错误| |
|给startUpload()方法传入的文件id不存在| |
|uploadStart()方法中返回了false| |
|上传被取消了| |
|上传被终止了| |
|| |
|文件正在队列中等待上传| |
|文件正在上传| |
|文件在添加到队列或是上传的时候出现了错误| |
|文件已上传成功| |
|文件被取消上传| |
|| |
|鼠标以箭头显示| |
|鼠标以手形显示| |
|| |
|Flash按钮会显示在页面的所有dom元素上面| |
|允许其他dom元素覆盖住Flash按钮| |
|允许Flash按钮透明显示| |
内容太多了，感觉有点乱了，如果还不清楚怎么使用，**建议看下官网的使用流程，明白怎么使用后再来看这些属性、事件、方法、常量什么的吧**
