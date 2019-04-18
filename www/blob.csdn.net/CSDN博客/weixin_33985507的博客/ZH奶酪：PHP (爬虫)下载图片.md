# ZH奶酪：PHP (爬虫)下载图片 - weixin_33985507的博客 - CSDN博客
2015年10月07日 09:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
原文地址:[http://www.phpfensi.com/php/20140107/1128.html](http://www.phpfensi.com/php/20140107/1128.html)
通过图片地地址把图片保存到本址,这里我们直接通过readfile读取然后通过fopen保存即可,实例代码如下:
```
<?php 
/** 
 * 通过图片的远程url，下载到本地 
 * @param: $url为图片远程链接 
 * @param: $filename为下载图片后保存的文件名 
 */ 
function GrabImage($url,$filename) {  
 if($url==""):return false;endif;  
 ob_start();  
 readfile($url);  
 $img = ob_get_contents();  
 ob_end_clean();  
 $size = strlen($img);  
 //"../../images/books/"为存储目录，$filename为文件名 
 $fp2=@fopen("../../images/books/".$filename, "a");  
 fwrite($fp2,$img);  
 fclose($fp2);  
 return $filename;  
 }  
?>
```
函数返回图片下载完后保存的名字，那么只需在数据库中存储 图片目录+图片名即可。
