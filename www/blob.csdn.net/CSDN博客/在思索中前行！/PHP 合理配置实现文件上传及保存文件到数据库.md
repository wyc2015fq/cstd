# PHP 合理配置实现文件上传及保存文件到数据库 - 在思索中前行！ - CSDN博客





2014年12月14日 15:39:42[_Tham](https://me.csdn.net/txl16211)阅读数：4174








## 合理配置 php.ini

如何配置php.ini实现PHP文件上传功能。其中涉及到php.ini配置文件中的upload_tmp_dir、upload_max_filesize、post_max_size等选项，这些选项是文件上传成败的关键。我以php.5.3
 的版本为例说明。



打开php.ini 配置文件，查找 file_uploads ，在这个区域有以下3个选项：

file_uploads = On

是否允许HTTP文件上传。默认值为On允许HTTP文件上传，此选项不能设置为Off。

upload_tmp_dir =

文件上传的临时存放目录。如果没指定则PHP会使用系统默认的临时目录。该选项默认为空，此选项在手动配置PHP运行环境时，也容易遗忘，如果不配置这个选项，文件上传功能就无法实现，你必须给这个选项赋值，比如upload_tmp_dir = "/home/tmp" ，代表在home目录下有一个tmp目录，并且给这目录赋予所有用户读写权限。

upload_max_filesize = 2M

上传文件的最大尺寸。这个选项默认值为2M，即文件上传的大小为2M，如果你想上传一个50M的文件，你必须设定 upload_max_filesize = 50M。
但是仅设置upload_max_filesize = 50M 还是无法实现大文件的上传功能，我们还必须修改php.ini文件中的 post_max_size 选项。

max_file_uploads = 20 


设置一次最多允许上传文件的数量

继续在 php.ini 中查找 Data Handling ，在这个区域有1个选项：

post_max_size = 8M

指通过表单POST给PHP的所能接收的最大值，包括表单里的所有值。默认为8M。如果POST数据超出限制，那么$_POST和$_FILES将会为空。

要上传大文件，你必须设定该选项值大于upload_max_filesize选项的值，例如你设置了upload_max_filesize = 50M ，这里可以把post_max_size = 100M。

另外如果启用了内存限制，那么该值应当小于memory_limit 选项的值。

继续在 php.ini 中查找 Resource Limits ，在这个区域有3个选项：

max_execution_time = 30

每个PHP页面运行的最大时间值（单位秒），默认30秒。当我们上传一个较大的文件，例如50M的文件，很可能要几分钟才能上传完，但php默认页面最久执行时间为30秒，超过30秒，该脚本就停止执行，这就导致出现无法打开网页的情况。因此我们可以把值设置的较大些，如 max_execution_time = 600。 如果设置为0，则表示无时间限制。

max_input_time = 60

每个PHP脚本解析请求数据所用的时间（单位秒），默认60秒。当我们上传大文件时，可以将这个值设置的较大些。 如果设置为0，则表示无时间限制。

memory_limit = 128M

这个选项用来设置单个PHP脚本所能申请到的最大内存空间。这有助于防止写得不好的脚本消耗光服务器上的可用内存。如果不需要任何内存上的限制将其设为 -1。

php5.2.0以前的版本默认8M; php.5.2.0版本默认为16M。php 5.2.0之后的版本默认为 128M;

#### php.ini 配置上传文件功能示例

假设要上传一个50M的大文件。配置 php.ini 如下：
file_uploads = On
upload_tmp_dir = "/home/fileuploadtmp"
upload_max_filesize = 50M
post_max_size = 100M
max_execution_time = 600
max_input_time = 600
memory_limit = 128M
提示：需要保持 memory_limit > post_max_size > upload_max_filesize
此例仅供参考，你可以根据实际情况调整。

## PHP文件上传详解
上传文件分为两个部分，HTML显示部分和PHP处理部分，HTML部分主要是让用户来选择所要上传的文件，然后通过PHP中的全局变量$_FILES，我们可以把文件上传到服务器的指定目录。

先来看一下HTML部分。

```php
<form action="upload.php"  method="post"  enctype="multipart/form-data">
上传：<input type="file" name="myfile" />
<input type="submit" name="submit" value="上传" />
</form>
```
说明：

form标答的action="upload.php"是指点击这个form中的submit的时候，这个上传命令会被发送到这个叫 upload.php的页面去处理。method="post"是指以post方式去送，enctype="multipart/form-data"属 性规定了在提交这个表单时要使用哪种内容类型，在表单需要二进制数据时，比如文件内容，请使用"multipart/form-data"，如果要上传文 件，这个属性是必要的。input中的type="file"时，规定了应该把输入作为文件来处理，并且在input后面会有一个浏览的按钮。

再来看一个PHP处理页面 upload.php

```php
<?php
if($_FILES['myfile']['name'] != '') {
　　if($_FILES['myfile']['error'] > 0) {
　　　　echo "错误状态：" . $_FILES['myfile']['error'];
　　} else {
　　　　move_uploaded_file($_FILES['myfile']['tmp_name'] , "uploads/" . $FILES['myfile']['name']);
　　　　echo "<script>alert(上传成功！);</script>";
　　}
} else{
　　      echo "<script>alert(请上传文件！);</script>";
}
?>
```
说明：在解释这篇代码前，我们有必要了解以下知识。
$_FILES['myfile']['name']  是指被上传文件的名称

$_FILES['myfile']['type']  是指被上传文件的类型

$_FILES['myfile']['size']  是指被上传文件的大小，单位为字节(B)

$_FILES['myfile']['tmp_name']  是指被上传文件存在服务器中的临时副本文件名称，文件被移动到指定目录后临文件将被自动消毁。

$_FILES['myfile']["error"]  是指由文件上传中有可能出现的错误的状态码，关于各状态含义后在会说明。

了解了这些，我们再来看一下upload.php的代码。

首先，$_FILES['myfile']['name']中的myfile是指在上面HTML页面中上传文件标签的name值，根据这个我们才能知道我 们正在处理的文件是哪一个input提交过来的，然后再来判断一下 $_FILES['myfile']['name'] 不是否为空，根据这个我们可以知道用户有没有上传文件，从而执行不同的操作。如果上传了文件并且状态是0就说明上传成功，我们就可以用
 move_uploaded_file方法把上传的文件存放到指定目录，上面这个例子是指把上传的文件移动到同目录下的uploads文件夹下，这个路径 是相对于这个PHP文件（既upload.php）的相对目录。比如，我们想把上传的文件移动到upload.php上一层叫user的文件夹中的话，我 们就可以这样写：move_uploaded_file($_FILES['myfile']['tmp_name']
 , "../user/" . $FILES['myfile']['name'])，这种方法使用起来很方便、灵活，这样一个文件就被上传到服务器中了，可以打开服务器中的目录查看该 文件。

允许用户上传文件是一个有巨大的安全风险的行为，因此，通常情况下，我们会对用户上传的文件做一些限制，比如常见的限制文件

类型和文件大小，来看一下。



```php
<?php
if($_FILES['myfile']['name'] != '') {
　　if($_FILES['myfile']['error'] > 0) {
　　　　echo "错误状态：" . $_FILES['myfile']['error'];
　　} else {
　　　　if($_FILES['myfile']['type'] == 'image/jpeg' or $_FILES['myfile']['type'] == 'image/pjpeg' or $_FILES['myfile']['type'] == 'image/gif' && $_FILES['myfile']['size'] < 2048000){
　　　　　　move_uploaded_file($_FILES['myfile']['tmp_name'] , "uploads/" . $FILES['myfile']['name']);
　　　　　　echo "<script>alert(上传成功！);</script>";
　　　　} else {
　　　　　　echo "<script>alert(请上传小于2MB的jpeg或Gif类型的附件);<script>";
　　　　}
　　}
} else {
　　echo "<script>alert(请上传文件！);</script>";
}
?>
```
从上面的代码可以看出，我们规定了上传的文件类型必须是jpeg或者Gif并且必须小于2MB的文件($_FILES['myfile']['size']的默认单位是字节)

这里必须提到的是，对于IE浏览器，它识别jpg文件的类型必须是 pjpeg，而对于 FireFox，则必须是 jpeg，因此，我们必须对jpeg和pjpeg都作判断。

这样一来，我们可以限制用户上传的一些危险的比如木马或者病毒脚本，来保证了服务器的安全运行。

现在，一个上传文件程序就基本成形了。但时在有些时候，考虑到用户体验，我们还可以对用户上传过程中发生的错误作出一些提醒，

让用户明白是哪里出了问题，我们会对$_FILES['myfile']['error']作出一些说明，先来看一下在PHP中对$_FILES['myfile']['error']常见6种状态的定义。

$_FILES['teacher_pic']['error'] = 1        文件大小超过了PHP.ini中的文件限制

$_FILES['teacher_pic']['error'] = 2        文件大小超过了浏览器限制

$_FILES['teacher_pic']['error'] = 3        文件部分被上传

$_FILES['teacher_pic']['error'] = 4        没有找到要上传的文件

$_FILES['teacher_pic']['error'] = 5        服务器临时文件夹丢失

$_FILES['teacher_pic']['error'] = 6       文件写入到临时文件夹出错

错误信息状态为1时说明上传的文件超过了php.ini中的文件大小限制，我们可以打开php.ini这个文件。




找一下; Maximum allowed size for uploaded files.upload_max_filesize = 2M

我这里设置语句定义了PHP中上传文件的最大字节数，默认情况下是2MB，这个设置是PHP全局上传限 制，权限最高，即使$_FILES['myfile']['size']设为10MB，也只能上传2MB以下的文件。比如，在默认情况下，如果规 定$_FILES['myfile']['size'] < 10MB，在用户上传文件大于2MB的情况下，就会现在$_FILES['teacher_pic']['error'] = 1的情况，一般来说，我们须要把$_FILES['myfile']['size']的值设定在upload_max_filesize值之下（设大了也
 没用，呵呵）。当然，你完全可以把php.ini中的upload_max_filesize值调的更大，但实际应用中，我们考虑到服务器的负载能力，不 建议upload_max_filesize的值超过20MB，这样会造成网站附件增大，这在论坛社区上可以很明显的看出来。

了解了这些，我们就可以对错误状态作出定义，我们再来完善一下代码，来看一下。

使用了switch语句来对6种错状态作出定义，这样来下，在发生错误的时间，用户就会明白，是哪里出了问题。但是还有一种情况就是，用户上传的文件在指定的目录中已经存在，这里我们可以使用file_exists方法来判断一下：


```php
<?php
if($_FILES['myfile']['name'] != ''){
　　if($_FILES['myfile']['error'] > 0){
　　　　switch($_FILES['myfile']['error']){
　　　　　　case 1:
　　　　　　　　echo "文件大小超过了PHP.ini中的文件限制！";
　　　　　　　　break;
　　　　　　case 2:
　　　　　　　　echo "文件大小超过了浏览器限制！";
　　　　　　　　break;
　　　　　　case 3:
　　　　　　　　echo "文件部分被上传！";
　　　　　　　　break;
　　　　　　case 4:
　　　　　　　　echo "没有找到要上传的文件！";
　　　　　　　　break;
　　　　　　case 5:
　　　　　　　　echo "服务器临时文件夹丢失，请重新上传！";
　　　　　　　　break;
　　　　　　case 6:
　　　　　　　　echo "文件写入到临时文件夹出错！";
　　　　　　　　break;
　　　　}
　　} else {
　　　　if($_FILES['myfile']['type'] == 'image/jpeg' or $_FILES['myfile']['type'] == 'image/pjpeg' or $_FILES['myfile']['type'] == 'image/gif' && $_FILES['myfile']['size'] < 20480) {
　　　　　　if (!file_exists("uploads/" . $_FILES["myfile"]["name"]))
　　　　　　　　move_uploaded_file($_FILES['myfile']['tmp_name'] , "uploads/" . $FILES['myfile']['name']);
　　　　　　　　echo "<script>alert(上传成功！);</script>";
　　　　　　} else{
　　　　　　　　echo "<script>alert(您上传的文件已经存在！);</script>";
　　　　　　}
　　　　} else {
　　　　　　echo "<script>alert(请上传小于2MB的jpeg或Gif类型的附件);<script>";
　　　　}
　　}
} else {
　　echo "<script>alert(请上传文件！);</script>";
}
?>
```

## 总结一下上传中的逻辑判断顺：
1. 先判断是否上传文件

2. 如果有再来判断上传中是否出错

3. 如果出错，则提示出错信息

4. 如查没出错，再判断文件类型

5. 如果类型符合条件，再判断指定目录中有没有存在该文件

6. 如果没有就把该文件移至指定目录



**上传文件先创建目录 ，再上传到目录里面去并建立数据库的文件地址链接**


html代码



```
<html> 
<head> 
<title> 
my is upfile app!! 
</title> 
<meta http-equiv="content-type" content="text/html;charset=utf-8" />
</head> 
<body> 
<form enctype="multipart/form-data" method="post" action="upfile_add.php"> 
文件名: <input type="text" name="title" /> 
上传文件: <input type="file" name="file" /> 
<input type="submit" vlaue="提交" /> 
</form> 
</body> 
</html>
```
php代码

```php
<?php 
$mkdir_file_dir = mkdir('./img/'.$_POST['title'],0777); //获取到标题，在最终的目录下面建立一个文件夹用来存放分类指定的文件 ，并设定文件夹的权限
$tmp_file_name = $_FILES['file']['tmp_name']; //得到上传后的临时文件 
$file_name = $_FILES['file']['name']; //源文件名 
$file_dir = './img/'.$_POST['title'].'/'; //最终保存目录 
if(is_dir($file_dir)) 
{ 
  move_uploaded_file($tmp_file_name,$file_dir.$file_name); //移动文件到最终保存目录 
  $img_url = $file_dir.$file_name; 
  $link = mysql_connect("localhost","root",""); //将文件上传的地址保存到数据库 
  mysql_select_db("test"); 
  mysql_query("set names utf8"); 
  $insert = "insert into upfiles(title,img_url) values ('{$_POST['title']}','{$img_url}') "; 
  mysql_query($insert); //把文件路径存放到数据库中 
} 
?>
```






