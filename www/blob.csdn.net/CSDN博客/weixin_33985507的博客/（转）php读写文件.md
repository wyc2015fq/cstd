# （转）php读写文件 - weixin_33985507的博客 - CSDN博客
2009年03月10日 23:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
#### php读写文件
//例1
<?php 
if $_POST['submit']=='提交' { 
$handle = fopen("list.txt", "w"); 
fwrite($handle,$_POST['text']); 
fclose($handle); 
} 
?> 
<html> 
<head> 
<title>php读写文本文件</title> 
</head> 
<body> 
<form id="form1" name="form1" method="post" action=""> 
<textarea name="text" cols="100" rows="20" id="text"><?php 
require "list.txt"; 
?> 
</textarea> 
<br> 
<input type="submit" name="Submit" value="提交" /> 
</form> 
</body> 
</html>
<?    
function readfromfile($file_name) { //File Reading 
    if (file_exists($file_name)) { 
        if (PHP_VERSION >= "4.3.0") return file_get_contents($file_name); 
        $filenum=fopen($file_name,"r"); 
        $sizeofit=filesize($file_name); 
        if ($sizeofit<=0) return ''; 
        @flock($filenum,LOCK_EX); 
        $file_data=fread($filenum, $sizeofit); 
        fclose($filenum); 
        return $file_data; 
    } else return ''; 
} 
function writetofile ($filename, $data) { //File Writing 
    $filenum=@fopen($filename,"w"); 
    if (!$filenum) { 
        return false; 
    } 
    flock($filenum,LOCK_EX); 
    $file_data=fwrite($filenum,$data); 
    fclose($filenum); 
    return true; 
} 
?> 

