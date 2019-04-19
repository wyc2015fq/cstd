# 微信公众平台开发3：订阅事件subscribe处理 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:29:31[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：408
新用户关注微信公众平台，将产生一个订阅事件，即subscribe事件，默认代码中没有对这一事件进行相应处理。
在新用户关注公众平台后，可能想知道该平台提供了哪些功能，以及怎样使用该平台，通俗一点讲就是该平台的“使用说明书”。
微信目前提供了五种消息类型，分别为：
- 文本消息（text）；
- 图片消息（image）；
- 地理位置消息（location）；
- 链接消息（link）；
- 事件推送（event）；
收到消息后，首先需要对消息类型做出判断，然后再针对不同类型的消息做出处理。在事件推送中，事件类型又分为三种，subscribe（订阅）、unsubscribe（取消订阅）、CLICK（自定义菜单点击事件）；判断为subscribe事件后，根据设定好的欢迎消息，回复给用户。
## 判断消息类型
$postObj=simplexml_load_string($postStr,'SimpleXMLElement', LIBXML_NOCDATA);$RX_TYPE=trim($postObj->MsgType);switch($RX_TYPE){case"text":$resultStr=$this->handleText($postObj);break;case"event":$resultStr=$this->handleEvent($postObj);break;default:$resultStr="Unknow msg type: ".$RX_TYPE;break;}
## 再判断事件类型
switch($object->Event){case"subscribe":*//订阅事件*$contentStr="感谢关注**\n**"."【1】 查天气，如输入：北京天气**\n**"."【2】 查公交，如输入：北京公交918**\n**"."更多内容，敬请期待...";break;default:$contentStr="Unknow Event: ".$object->Event;break;}
## 效果：
如何判断用户输入的文字并回复：
根据用户发送的信息进行判断，然后给出对应的回复，具有良好的交互性。
用户发送过来的文本信息，我们可以提取关键字，通过简单的 if...elseif...else... 实现。
关键代码如下：
if($keyword=="你好"){$contentStr="hello";}elseif($keyword=="苏州"){$contentStr="上有天堂，下有苏杭";}else{$contentStr="感谢关注";}
## 艺搜参考
[http://www.cnblogs.com/mchina/p/3120531.html](http://www.cnblogs.com/mchina/p/3120531.html)
[http://www.cnblogs.com/mchina/p/3155072.html](http://www.cnblogs.com/mchina/p/3155072.html)
