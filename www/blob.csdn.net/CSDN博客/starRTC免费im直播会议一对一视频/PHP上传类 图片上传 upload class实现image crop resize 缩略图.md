# PHP上传类 图片上传 upload class实现image crop resize 缩略图 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:20:38[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：526
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

manage uploaded files, and manipulate images in many ways
through an HTML form, a Flash uploader, XMLHttpRequest or on local files. 
Supported formats
 are PNG, JPG, GIF and BMP.
可以加边框，
示例：
一，操作本地文件
if(file_exists($localfile)){//本地文件
        $handle = new upload($localfile);//操作本地文件
        if ($handle->uploaded) {            
              $handle->file_name_body_pre   = 'thumb_';  //目标文件在原文件基础上加的前缀
              $handle->image_resize         = true;
              $handle->image_x              = 100;
              $handle->image_ratio_y        = true;
              $handle->process($localpath);     //目的文件保存路径         
              if($handle->processed) {                 
                 //$handle->clean();//delete the original uploaded file
                echo_1('ok');return;          
              }else{
                  echo_0($handle->error);return;                
              }
        }else{
            echo_0($handle->error);return;        
        }        
    }else{        
        echo_0('upload failed');return;    
    }

参考：
[http://www.verot.net/php_class_upload.htm](http://www.verot.net/php_class_upload.htm)
使用方法与下载：[https://github.com/verot/class.upload.php](https://github.com/verot/class.upload.php)
处理后的效果：[http://www.verot.net/php_class_upload_samples.htm](http://www.verot.net/php_class_upload_samples.htm)
