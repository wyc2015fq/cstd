# Mongodb自增id实现方法 - z69183787的专栏 - CSDN博客
2016年10月27日 07:05:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4921
这篇文章主要介绍了Mongodb自增id实现方法,分析了MongoDB数据库自增ID的原理与实现技巧,具有一定参考借鉴价值,需要的朋友可以参考下
本文实例讲述了Mongodb自增id实现方法。分享给大家供大家参考，具体如下：
首先创建一个自动增长id集合 ids
```
>db.ids.save({name:"user", id:0});
```
可以查看一下是否成功
```
> db.ids.find();
{ "_id" : ObjectId("4c637dbd900f00000000686c"), "name" : "user", "id" : 0 }
```
然后每次添加新用户之前自增一下 ids集合 获得id
然后每次在db.user集合里添加新用户之前 在db.ids集合中的name="user"文档的id值加1返回文档
```
>userid = db.ids .findAndModify({update:{$inc:{'id':1}}, query:{"name":"user"}, new:true});
{ "_id" : ObjectId("4c637dbd900f00000000686c"), "name" : "user", "id" : 1 }
```
注：因为findAndModify是一个方法完成更新查找两个操作，所以具有原子性，多线程不会冲突。
然后保存相应的数据
```
>db.user .save({uid:userid.id, username:"dotcoo", password:"dotcoo", info:"http://www.jb51.net/ "});
// --------- update 2011-03-27 13:11 ------------------------
```
其实上边两行可以写为一步
```
>db.user .save({
  uid: db.ids .findAndModify({
    update:{$inc:{'id':1}},
    query:{"name":"user"},
    new:true}).id, //让db.ids集合中的name="user"文档的id值加1并返回充当自增id
  username: "dotcoo",
  password:"dotcoo",
  info:"http://www.jb51.net/  "});
// --------- update 2011-03-27 13:11 ------------------------
```
查看结果
```
> db.user.find();
{ "_id" : ObjectId("4c637f79900f00000000686d"), "uid" : 1, "username" : "admin", "password" : "admin" }
```
这是mongo的shell，如果用的是服务器端程序Java php Python，可以自己对这些操作封装一下，只用传几个参数就可以返回自增的id，还可以实现像Oracle的跨表的自增id。
自己写了一段php的，拿出来给大家分享。
```php
<?php
function mid($name, $db){
  $update = array('$inc'=>array("id"=>1));
  $query = array('name'=>$name);
  $command = array(
      'findandmodify'=>'ids', 'update'=>$update,
      'query'=>$query, 'new'=>true, 'upsert'=>true
  );
  $id = $db->command($command);
  return $id['value']['id'];
}
$conn = new Mongo();
$db = $conn->idtest;
$id = mid('user', $db);
$db->user->save(array('uid'=>$id, 'username'=>'kekeles', 'password'=>'kekeles', 'info'=>'http://www.jb51.net/ '));
$conn->close();
?>
```
其具体实现方式主要是利用MongoDB中findAndModify命令，只要每次往MongoDB里insert对象前生成ID赋值给_id就OK了，因为它的实现满足原子性，所以不存在并发问题。
另外说明一点，findAndModify本身提供了一个upsert参数，为true的话可以自动insert，但那样就不能自定义初始值了，所以文中示例没有使用upsert。
BTW，数据库“_seq”的名字以下划线开头，这样列表的时候会排在前面，容易分辨。
希望本文所述对大家MongoDB数据库程序设计有所帮助。
