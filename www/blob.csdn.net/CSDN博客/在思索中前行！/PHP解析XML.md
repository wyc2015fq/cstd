# PHP解析XML - 在思索中前行！ - CSDN博客





2014年06月17日 22:18:24[_Tham](https://me.csdn.net/txl16211)阅读数：517标签：[PHP																[xml](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=PHP&t=blog)
个人分类：[php开发](https://blog.csdn.net/txl16211/article/category/2231073)







<?php
header("content-type:text/html;charset=utf-8"); 
/*
* 通过DOMDocument类来xml文件的读
* 有2种方式：读文件、都字符串
*/
// 首先要建一个DOMDocument对象      
$xml = new DOMDocument();//属性和方法不是静态 所以要实例化      
$xml->load("me.xml"); // 加载Xml文件 
//$xml->loadXML($xml_str);//加载xml字符串

// 获取所有的post标签      
$postDom = $xml->getElementsByTagName("post");      

// 循环遍历post标签      
foreach($postDom as $post){      
// 获取Title标签Node      
$title = $post->getElementsByTagName("title");      

/**     
* 要获取Title标签的Id属性要分两部走     
* 1. 获取title中所有属性的  
列表也就是$title->item(0)->attributes     
* 2. 获取title中id的属性，  
因为其在第一位所以用item(0)     
*     
* 小提示：     
* 若取属性的值可以用item(*)->nodeValue     
* 若取属性的标签可以用item(*)->nodeName     
* 若取属性的类型可以用item(*)->nodeType     
*/      
echo "Id: " . $title->item(0)->attributes->item(0)->nodeValue . "<br />";      
echo "Title: " . $title->item(0)->nodeValue . "<br />";      
echo "Details: " . $post->getElementsByTagName("details")->item(0)->nodeValue . "<br />";   
echo "Details: " . $post->childNodes->item(1)->nodeValue . "  2<br /><br />";  
}      
?>



