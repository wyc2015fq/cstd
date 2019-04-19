# PHP 原生 DOM 对象操作 XML - 文章 - 伯乐在线
原文出处： [梁吉林](http://blog.csdn.net/ljl890705/article/details/49681277)
对于操作XML类型文件，PHP内置有一套DOM对象可以进行处理。对XML的操作，从创建、添加到修改、删除都可以使用DOM对象中的函数来进行。
### 创建
创建一个新的XML文件，并且写入一些数据到这个XML文件中。


```
/*
 * 创建xml文件
 */
$info = array(
    array('obj' => 'power','info' => 'power is shutdown'),
    array('obj' => 'memcache','info' => 'memcache used than 90%'),
    array('obj' => 'cpu','info' => 'cpu used than 95%'),
    array('obj' => 'disk','info' => 'disk is removed')
);//用来写入的数据
$dom = new DOMDocument('1.0');
$dom->formatOutput = true;//格式化
$eventList = $dom->createElement('EventList');//创建根节点EventList
$dom->appendChild($eventList);//添加根节点
for($i = 0; $i < count($info); $i++){
    $event = $dom->createElement('event');//创建节点event
    $text = $dom->createTextNode('PHP'.$i);//创建文本节点，值为PHP0,PHP1...
    $event->appendChild($text);//将文本节点添加到节点event，做为节点event的值
    $attr_obj = $dom->createAttribute('obj');//创建属性obj
    $attr_obj->value = $info[$i]['obj'];//为obj属性赋值
    $event->appendChild($attr_obj);//将obj属性添加到event节点中，做为event节点的属性
    $attr_info = $dom->createAttribute('info');
    $attr_info->value = $info[$i]['info'];
    $event->appendChild($attr_info);
    $eventList->appendChild($event);//将event节点添加到根节点EventList中
}
//echo $dom->saveXML();
$dom->save('./t.xml');//保存信息到当前目录下的t.xml文件中
```
上面的代码段可以创建一个XML文件，并添加一些信息到这个文件中，包括值和属性，最终形成的文件为当前目录下的t.xml，可以看一下它的内容。


```
<?xml version="1.0"?>
<EventList>
  <event obj="power" info="power is shutdown">PHP0</event>
  <event obj="memcache" info="memcache used than 90%">PHP1</event>
  <event obj="cpu" info="cpu used than 95%">PHP2</event>
  <event obj="disk" info="disk is removed">PHP3</event>
</EventList>
```
### 读取XML信息&添加新的属性
以上一节创建的t.xml文件为操作对象，读取出t.xml文件中的信息，并给节点添加一个新的属性count，其值为1。


```
/*
 * 读取xml文件信息，并添加新的属性
 */
$dom = new DOMDocument('1.0');
$dom->load('./t.xml');//加载要操作的文件
$list = $dom->getElementsByTagName('event');//获取event节点列表
foreach($list as $item){
    $attr_obj = $item->getAttribute('obj');//获取属性obj的值
    $attr_info = $item->getAttribute('info');
    echo "<pre>Object:$attr_obj;Info:$attr_info;Value:{$item->nodeValue}</pre>";
    $item->setAttribute('count',1);//添加新的属性count=1
}
$dom->save('./t.xml');//保存修改
```
看一下提取到的值：


```
Object:power;Info:power is shutdown;Value:PHP0
Object:memcache;Info:memcache used than 90%;Value:PHP1
Object:cpu;Info:cpu used than 95%;Value:PHP2
Object:disk;Info:disk is removed;Value:PHP3
```
再看一下现在的t.xml文件的内容，count属性已经添加上。


```
<?xml version="1.0"?>
<EventList>
  <event obj="power" info="power is shutdown" count="1">PHP0</event>
  <event obj="memcache" info="memcache used than 90%" count="1">PHP1</event>
  <event obj="cpu" info="cpu used than 95%" count="1">PHP2</event>
  <event obj="disk" info="disk is removed" count="1">PHP3</event>
</EventList>
```
### 修改节点属性&节点值
以上一节中的t.xml文件为操作对象，修改一下obj属性是cpu的节点的count值，新的值为count+1。


```
/*
 * 修改某一个节点的属性和值
 */
$dom = new DOMDocument('1.0');
$dom->load('./t.xml');
$list = $dom->getElementsByTagName('event');
foreach($list as $item){
    $attr_obj = $item->getAttribute('obj');
    if($attr_obj == 'cpu'){//修改cpu的count属性，使其值+1
        $attr_count = $item->getAttribute('count');//获取count属性的值
        $item->setAttribute('count',$attr_count+1);//重置count属性的值
        $item->nodeValue = 'Hello,Kitty';//重置节点的值
    }
}
$dom->save('./t.xml');
```
操作后的t.xml文件如下，要以看到obj=cpu的节点的count属性已经改变，值也修改成功。


```
<?xml version="1.0"?>
<EventList>
  <event obj="power" info="power is shutdown" count="1">PHP0</event>
  <event obj="memcache" info="memcache used than 90%" count="1">PHP1</event>
  <event obj="cpu" info="cpu used than 95%" count="2">Hello,Kitty</event>
  <event obj="disk" info="disk is removed" count="1">PHP3</event>
</EventList>
```
### 删除节点
要添加就会有删除。以上节的t.xml文件为操作对象，删除obj=disk的节点。


```
/*
 * 删除节点
 */
$dom = new DOMDocument('1.0');
$dom->load('./t.xml');
$list = $dom->getElementsByTagName('event');
foreach($list as $item){
    if($item->getAttribute('obj') == 'disk'){//以obj=disk的节点为操作对象
        $item->parentNode->removeChild($item);//删除节点
    }
}
$dom->save('./t.xml');
```
看一下操作后的t.xml文件内容，obj=disk的节点已被成功删除。


```
<?xml version="1.0"?>
<EventList>
  <event obj="power" info="power is shutdown" count="1">PHP0</event>
  <event obj="memcache" info="memcache used than 90%" count="1">PHP1</event>
  <event obj="cpu" info="cpu used than 95%" count="2">Hello,Kitty</event>
</EventList>
```
### 向根节点中添加新的子节点
以上一节的t.xml为操作对象，向根节点EventList中添加一个新的子节点。


```
/*
 * 向EventList中添加一个子节点
 */
$dom = new DOMDocument('1.0');
$dom->load('./t.xml');
$event_list = $dom->getElementsByTagName('EventList');//获取根节点
$event = $dom->createElement('event','lenovo');//新建节点
$event_list->item(0)->appendChild($event);//将新建节点添加到根节点中
$event_attr_obj = $dom->createAttribute('obj');
$event_attr_obj->value = 'lenovo';
$event->appendChild($event_attr_obj);
$event_attr_info = $dom->createAttribute('info');
$event_attr_info->value = 'thinkpad t430';
$event->appendChild($event_attr_info);
$dom->save('./t.xml');
```
看一下操作后的t.xml文件内容，新的子节点已经被插入到根节点中。


```
<?xml version="1.0"?>
<EventList>
  <event obj="power" info="power is shutdown" count="1">PHP0</event>
  <event obj="memcache" info="memcache used than 90%" count="1">PHP1</event>
  <event obj="cpu" info="cpu used than 95%" count="2">Hello,Kitty</event>
<event obj="lenovo" info="thinkpad t430">lenovo</event></EventList>
```
### 关于item($index)
item(index)是DOMNodeList类中的一个方法，它的做用是返回一个由索引指明的节点。而DOMDocument类中的getElementsByTagName(name）方法返回的正是一个DOMNodeList对象的实例，所以可以直接调用item(index)方法。以上节的t.xml为示例，如果e=dom−>getElementsByTagName(‘EventList′)获取EventList节点的信息，因为EventList节点是根节点，有且仅有一个，所以它调用item(index)时，索引只有index=0可用，因为它只有1个；而如果e=dom−>getElementsByTagName(‘event′)获取event节点的信息，因为event有4个，所以它调用item(index)时，索引$index={0,1,2,3}，有4个值可以选。每个节点都包含多个属性，它可以以一种键值对数组的形式表现出来，如下所示：


```
object(DOMElement)#3 (18) {
  ["tagName"]=>
  string(5) "event"
  ["schemaTypeInfo"]=>
  NULL
  ["nodeName"]=>
  string(5) "event"
  ["nodeValue"]=>
  string(11) "Hello,Kitty"
  ["nodeType"]=>
  int(1)
  ["parentNode"]=>
  string(22) "(object value omitted)"
  ["childNodes"]=>
  string(22) "(object value omitted)"
  ["firstChild"]=>
  string(22) "(object value omitted)"
  ["lastChild"]=>
  string(22) "(object value omitted)"
  ["previousSibling"]=>
  string(22) "(object value omitted)"
  ["nextSibling"]=>
  string(22) "(object value omitted)"
  ["attributes"]=>
  string(22) "(object value omitted)"
  ["ownerDocument"]=>
  string(22) "(object value omitted)"
  ["namespaceURI"]=>
  NULL
  ["prefix"]=>
  string(0) ""
  ["localName"]=>
  string(5) "event"
  ["baseURI"]=>
  string(36) "file:/H:/xampp/htdocs/demo/xml/t.xml"
  ["textContent"]=>
  string(11) "Hello,Kitty"
}
```
也可以当做对象的属性来用，例如获取这个节点的值：


```
/*
 * 关于item()
 */
$dom = new DOMDocument('1.0');
$dom->load('./t.xml');
$e = $dom->getElementsByTagName('event');
echo $e->item(2)->nodeValue;
//var_dump($e->item(2));
// $e = $dom->getElementsByTagName('EventList');
// var_dump($e->item(0));
//var_dump($e->item(0)->baseURI);
// for($i=0;$i<$e->length;$i++){
//     echo $e->item($i)->nodeValue;
// }
```
