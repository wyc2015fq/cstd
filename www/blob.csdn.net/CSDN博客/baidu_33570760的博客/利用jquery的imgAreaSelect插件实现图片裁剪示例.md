# 利用jquery的imgAreaSelect插件实现图片裁剪示例 - baidu_33570760的博客 - CSDN博客
2017年08月29日 15:02:35[carman_风](https://me.csdn.net/baidu_33570760)阅读数：328
原文链接：http://www.cnblogs.com/mizzle/archive/2011/10/13/2209891.html
将用户上传的图片进行裁剪再保存是现在web2.0应用中常常处理的工作，现在借助jquery的imgareaselect插件再配合PHP的GD库就可以轻松的实现这个在以前来说非常棘手的功能。**我们来看看它的实现步骤**：
1、包含进CSS文件(imgareaselect-default.css)和 jquery.imgareaselect.js文件
2、html代码(要裁剪的图片元素)
```java
<img id="selectbanner" src="/pic/banner.jpg" />
```
3、imgAreaSelect的数据操作
```java
$('#selectbanner').imgAreaSelect({ selectionColor: 'blue', x1:0, y1:0, x2: 950, 
maxWidth:950,minWidth:950,y2:400,minHeight:400,maxHeight:400, 
selectionOpacity: 0.2 , onSelectEnd: preview });
```
这样即可在该图片元素中使用裁剪功能了，当选框确定之后我们要保存被选择的图片还得自己写代码来操作。下面是当确定好图片区域后点击一个裁剪按钮后的操作：
```java
//裁剪确认操作
$("#sliceButton").click(function() {
 var pic = $('#selectbanner').attr('src');  
 var x,y,w,h;
 $.post(
  "/template/sliceBanner",
  {
   x:$('#selectbanner').data('x'),
   y:$('#selectbanner').data('y'),
   w:$('#selectbanner').data('w'),
   h:$('#selectbanner').data('h'),
   pic:pic
  },
  function(data){
   //把裁剪后图片加载到原处
   if(data){
    $('#selectbanner').attr(pic);
   }
  }
 );
 
});
//设置选取框的选取信息
//利用jquery中的data方法来保存生成的数据
function preview(img, selection) { 
 $('#selectbanner').data('x',selection.x1);
 $('#selectbanner').data('y',selection.y1);
 $('#selectbanner').data('w',selection.width);
 $('#selectbanner').data('h',selection.height);
}
```
4、PHP端的主要代码：
```php
//利用GD库来作裁剪操作
function sliceBanner(){
 $x = (int)$_POST['x'];
 $y = (int)$_POST['y'];
 $w = (int)$_POST['w'];
 $h = (int)$_POST['h'];
 $filename = trim($_POST['pic']);
 if(isset($filename) ){
  $uploadBanner =  '/temp'. $filename;
  $sliceBanner = 'upload/'. $filename;
  $src_pic = getImageHander($uploadBanner);
  if(!$src_pic){
   echo 0;exit;
  }
  $dst_pic = imagecreatetruecolor($w, $h);
  imagecopyresampled($dst_pic, $src_pic, 0, 0, $x, $y, $w, $h, $w, $h);
  imagejpeg($dst_pic, $sliceBanner);
  imagedestroy($src_pic);
  imagedestroy($dst_pic);
  echo 1;exit;
 }
 echo 0 ;exit;
}
function getImageHander ($url) {
 $size=@getimagesize($url);
 switch($size['mime']){
  case 'image/jpeg': $im = imagecreatefromjpeg($url);break;
  case 'image/gif' : $im = imagecreatefromgif($url);break;
  case 'image/png' : $im = imagecreatefrompng($url);break;
  default: $im=false;break;
 }
 return $im;
}
```
**１、介绍**
[ImgAreaSelect](http://www.jb51.net/jiaoben/31986.html)是一jQuery插件，它支持用户通过鼠标拖曳选择图片的一部分，非常的fashion。另外，可以在这个选择图像区域的基础上应用网站的其他一些技术，比如图片拖曳、图片编辑等。
**２、基本用法**
这个插件基于jQuery上通过imgAreaSelect()方法来调用，它操作的对象是HTML中<img>元素内的图像。 
复制代码 代码如下:
```java
<script type="text/javascript">
$(document).ready(function () {
$('img#photo').imgAreaSelect({
handles: true,
onSelectEnd: someFunction
});
});
</script>
```
如果在jQuery对象里有不止有一个img元素，那么这个插件会对里面的元素逐个应用此方法。其实这个方法不仅仅应用于img元素，它支持任何块级元素（比如有图像背景的div元素）。
与其它jQuery插件相似，这个插件可以在$(document).ready() 或者 $(window).load()句柄中初始化。 
**３、选项**
用户在这个插件的很多方面都可以进行定制，用户通过插件选项来达到这个目的（这些选项会在插件初始化的时候起效）。这些选项包括： 
|**选项**|**描述**|
|----|----|
|**aspectRatio**|**长宽比**，以后在选择时候就会维持不变。 e.g. "**4:3**"|
|**autoHide**|如果设为true，那么在选择完后区域会消失。 **Default:false**|
|**classPrefix**|预先给插件元素的前缀（详见下面：５、元素与类） **Default:imgareaselect**|
|**disable**|如果设置成true，这个插件将不起作用（但是图像还是可见的）|
|**enable**|如果设置成true，这个插件又将重新起作用|
|**fadeSpeed**|若设置成大于零的某个数，将"渐隐/渐现"这个插件 Default:true|
|**handles**|若设置成true，在改变大小的时候显示改变框（就是角点有些小"矩形"） **Default:false**|
|**hide**|若设置成true，则隐藏选择框|
|**imageHeight**|图像的真实高度（因为有可能被CSS缩放过了）|
|**imageWidth**|图像的真实宽度（因为有可能被CSS绽放过了）|
|**instance**|若设为true，imgAreaSelect()函数会返回一个对选择区域图像的一个引用，以便能够进一步使用API。（详见８、API方法）|
|**keys**|启用/关闭键盘支持（详见７、键盘支持） **Default:false**|
|**maxHeight**|限制选择框（以像素为单位），设置最大、最小的高度、宽度。|
|**maxWidth**| |
|**minHeight**| |
|**minWidth**| |
|**movable**|设置是否支持选择框移动 **Default:true**|
|**parent**|指定此插件默认所附加到的父元素 **Default:body**|
|**persistent**|若设置成true，点击选择区域外将开始一个新的选项（换言之，是否让用户只能移动/缩放选择区域） **Default:false**|
|**remove**|若设置成true，则该插件将完全移除|
|**resizable**|决定选择区域是否可以改变大小 **Default:true**|
|**resizeMargin**|当选择区域宽度超过多少像素时将启用"可改变大小"模式|
|**show**|如果设置成true，选择区域将可见|
|**x1****y1**|初始化时选择框左上角的坐标|
|**x2****y2**|初始化时选择框右下角的坐标|
|**zIndex**|设置此插件所作用元素的z-index的值，一般情况下，imgAreaSelect 总是可以自动计算出它的值，但是极少数情况下还是有必要设置的。|
|**onInit**|当插件初始化时所调用的函数（详见６、回调函数）|
|**onSelectStart**|当开始选择时所调用的函数（详见６、回调函数）|
|**onSelectChange**|当改变选择区域时所调用的函数（详见６、回调函数）|
|**onSelectEnd**|当选择结束时所调用的函数（详见６、回调函数）|
**４、样式表**
随着插件发放的还有三个样式表： 
imgareaselect-default.css – 这是默认的样式表, 
imgareaselect-animated.css – 这个样式与默认的样式表基本是一样的，只是它能够让选择区域边框变化 
imgareaselect-deprecated.css – 只有你想使用不赞同的选项时才用这个样式表。 
你只需要在html头部将其中的一个样式表添加进来就可以。这css文件夹中还包含４张gif图片，它们是用来显示边框的。 
**５、元素与类**
此插件通过创建几个div元素来表示选择区域，包括边框、可调整手柄以及未选择的区域。这些元素已经指定了特定的类名了，所以你可以通过CSS或者jQuery选择器来获得并操作它们。
|**类名**|**指定给的对象**|
|----|----|
|**imgareaselect-selection**|选择的区域|
|**imgareaselect-border1imgareaselect-border2imgareaselect-border3imgareaselect-border4**|选择区域的四个边框，由四个div组成的|
|**imgareaselect-handle**|可改变大小的调整手柄（四个或者八个divs，如果没有使能的话，则不显示）|
|**imgareaselect-outer**|未选择区域，由四个divs组成|
下面的对象示意图展示了这插件的这些元素如何布置的：
其中的前缀"imgareaselect"是默认的前缀，可以通过"**classPrefix**"选项来修改。这在当有许多选择框需要分别操作（比如修改样式）的时候特别有用。
### ６、回调函数
回调函数（当设置onInit, onSelectStart, onSelectChange或 onSelectEnd选项）接收两个参数，第一个选项是这个插件所应用图像的引用，另外一个则是呈现当前选择的对象，这个对象有六个性质。
|性质|描述|
|----|----|
|x1 y1|选择区域左上角的坐标|
|x2 y2|选择区域右下角的坐标|
|width|选择区域的宽度|
|height|选择区域的高度|
为了便于理解，下面给出当选择完后执行的回调函数的例子：
```java
$('img#photo').imgAreaSelect({
onSelectEnd: function (img, selection) {
alert('width: ' + selection.width + '; height: ' + selection.height);
}
})
```
### ７、键盘支持
如果选项"keys"设置为true,那么我们能够通过键盘上的按键进来选择框的移动。下面的键可以使用，默认的功能如下：
|**按键**|**动作**|
|----|----|
|**方向键**|每次以10像素为单位移动选区|
|**Shift+方向键**|每次以１像素为单位移动选区|
|**Ctrl+方向键**|每次以10像素为单位扩缩选区|
|**Ctrl+Shift+方向键**|每次以1像素为单位扩缩选区|
当然你也可以自己通过设置"keys"这个选项来覆盖默认键设置，对象有下面的属性：
|**属性**|**描述**|
|----|----|
|**方向键**|设置方向键的功能|
|**Shift**|设置Shift键的功能|
|**Ctrl**|设置Ctrl键的功能|
|**Alt**|设置Alt键的功能|
每个属性通过设置数值（以像素为单位，数值要不小于１），表明当按下此键时移动/改变大小多少像素，或者指定其值为"string"来指定它是"resize"模式。例如：
```java
$('img#example').imgAreaSelect({
keys: { arrows: 15, ctrl: 5, shift: 'resize' }
})
```
这个例子里设置成"方向键会移动选区15像素，按住Ctrl键会移动5像素，而按住Shift键时则切换到resize模式"。
如果不止一个图像，且自己修改了按键设置，那么只对一个图像会应用这个自定义设置。一般来说，当"激活"（用鼠标点击）某个图像时，这个图像就会使用自定义的按键设置。
### ８、API方法
这个插件也提供了几个API方法扩展它的应用，并能够通过这些API方法跟其他web应用联合起来。
为了使用这些方法，首先需要一个插件对象，可以调用imgAreaSelect()函数并让其选项"instance"设置为true来实现：
```java
var ias = $('#photo').imgAreaSelect({ instance: true });
```
现在就可以使用此对象来调用公共的方法了。例如，设置一个默认预定义的范围：
```java
ias.setSelection(50, 50, 150, 200, true);
ias.setOptions({ show: true });
ias.update();
```
只要当初始化完成，就可以使用这些API方法了。下面列出这些API方法：
|**方法**|**描述**|
|----|----|
|**getOptions**|**getOptions()**　　－－返回当前选项的配置Returns:一个包含当前选项配置的对象（一般是JSON对象）|
|**setOptions**|**setOptions(newOptions)**－－设置插件选项 **参数:**newOptions– 选项配置对象（一般是JSON对象）|
|**getSelection**|**getSelection([noScale])**　－－获取当前的选区**参数：**noScale (可选)– 如果设置为真，那么对于返回的选区不进行缩放操作。**返回：**选择区域里的对象|
|**setSelection**|**setSelection(x1, y1, x2, y2, [noScale])**　　－－设置当前选择区域**参数：**x1 – 选择区域左上角X坐标y1 – 选择区域左上角Y坐标x2 –选择区域右下角X坐标y2 – 选择区域右下角Y坐标noScale (可选) –若设为真，则不会对图像进行缩放**注意：**此方法只在插件内设置选择区域，并不会马上观察到选区的变化，如果需要立马显示更改后的区域，必须在使用setSelect()函数后就调用update()函数，且让show选项为true.|
|**cancelSelection**|**cancelSelection()**　　－－取消当前的选择**注意**：这个方法会隐藏"选择/未选择"区域，所以不必调用update()函数。|
|**update**|**update([resetKeyPress])**－－更新插件配置**参数:**resetKeyPress (可选)– 如果设成false，此实例的按键将复位（即不可用）|
## 二、简单实例
### １、宽度或者高度限制
minWidth、minHeight、maxWidth以及maxHeight选项允许你设置选区的范围。在这个例子中，图像的最大范围将限制为200x150px。
```java
$(document).ready(function () {
$('#ladybug_ant').imgAreaSelect({ maxWidth: 200, maxHeight: 150, handles: true });
});
```
### ２、固定高宽比
配置aspectRatio选项就可以了，这里将其设置成"４：３"：
```java
$(document).ready(function () {
$('#bee').imgAreaSelect({ aspectRatio: '4:3', handles: true });
});
```
### ３、设置初始选项区域
配置x1, y1, x2与 y2选项就可以了：
```java
$(document).ready(function () {
$('#duck').imgAreaSelect({ x1: 120, y1: 90, x2: 280, y2: 210 });
});
```
## 三、回调函数示例
### １、选区预览
在下面的代码小片断里，onSelectChange()回调函数实现了选择区域预览的效果。
官方源代码：http://odyniec.net/projects/imgareaselect/examples-callback.html
```java
functionpreview(img, selection) {
var scaleX = 100 / (selection.width || 1);
var scaleY = 100 / (selection.height || 1);
$('#ferret + div > img').css({
width: Math.round(scaleX * 400) + 'px',
height: Math.round(scaleY * 300) + 'px',
marginLeft: '-' + Math.round(scaleX * selection.x1) + 'px',
marginTop: '-' + Math.round(scaleY * selection.y1) + 'px'
});
}
//这里通过jQuery语法在原来图片后插入同样的图片
$(document).ready(function () {
$('<div><img src="ferret.jpg" style="position: relative;" /><div>')
.css({
float: 'left',
position: 'relative',
overflow: 'hidden',
width: '100px',
height: '100px'
})
.insertAfter($('#ferret'));
$('#ferret').imgAreaSelect({ aspectRatio: '1:1', onSelectChange:preview });
});
```
由于预览窗口是100x100px的，因此当截图窗口小于100px时，预览图会放大；当截图窗口大于100px时，预览图会缩小。这两种效果图如下：
截图 1　当截图窗口大于100px时 
截图 2　当截图窗口小于100px时 
需要说明的是，这里制作这种效果使用了一种技巧。最关键的一个是**等比率缩放.**它需要两个图片，第一图是原图，第二个图是选择区域后显示的图，用第一个图上的选择坐标+css控制产生第二个图，实际上两个图是一样的，只不过通过css控制了第二张图的显示区域与缩放比率。证据如下：
【证据一】在$(document).ready()函数中通过insertAfter插入"src"也是"ferret.jpg"的图片。再注意一下，这一段里的"overflow: 'hidden',"这一行代码就是让超过100px高宽的图片额外内容隐藏掉。
【证据二】在preview()函数中，首先就定义了scaleX与scaleY，它们的内容就是100/selection.width(或height)，也就是当selection.width小于100时，这个因子起放大作用，反之起缩小作用。
值得注意的是：
**回调函数中实际图的宽高（这里的300,400是实际图的高，要根据实际情况调整！）,回调函数中新图的宽高这些参数必须设置正确、否则会出现 选择偏差**
### ２、提交选区的坐标
如果需要实现真正截图功能必须使用服务器端支持，例如php asp aspx jsp。也就是意味着，在客户端选择只是第一步，如果需进一步的处理，必须要将这选区的坐标提交给服务器。那么如何实现呢？
创建一个表彰，里面有四个隐藏域：
```java
<form action="crop.php" method="post">
<input type="hidden" name="x1" value="" />
<input type="hidden" name="y1" value="" />
<input type="hidden" name="x2" value="" />
<input type="hidden" name="y2" value="" />
<input type="submit" name="submit" value="Submit" />
</form>
然后在初始化imgAreaSelect时，使用onSelectEnd()回调函数将选择后的数据赋给这些隐藏域，正如下面代码那样：
$(document).ready(function () {
$('#ladybug').imgAreaSelect({
onSelectEnd: function (img, selection) {
$('input[name="x1"]').val(selection.x1);
$('input[name="y1"]').val(selection.y1);
$('input[name="x2"]').val(selection.x2);
$('input[name="y2"]').val(selection.y2);
}
});
});
```
这样当点击"submit按钮"时，页面将上载到服务器，如果使用PHP的话，使用$_POST['x1']等就得到相应的坐标数据了
