# php遍历所有文件 - weixin_33985507的博客 - CSDN博客
2013年09月24日 22:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
<?php
 
function getdir($dir){
    static $str = '';
    if(is_file($dir)){
        $str.=$dir.'<br>';
    }else{
        //若不是正常文件,判断是否为目录
        if(is_dir($dir)){
            //打开目录
            $openDir = opendir($dir);
            while(($file = readdir($openDir)) !==false ){
                //echo $file.'<br>';
                if($file != '.' && $file != '..'){
                    if(is_file($dir.'/'.$file)){
                        $str.=$file.'<br>';
                    }elseif(is_dir($dir.'/'.$file)){
                        getdir($dir.'/'.$file);
                    }
                }
            }
            closedir($openDir);
        }
    }
    return $str;
}
echo getdir('crm');
?>
```
