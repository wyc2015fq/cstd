# Javascript数组、json对象基本操作 - 零度的博客专栏 - CSDN博客
2016年03月29日 14:36:15[零度anngle](https://me.csdn.net/zmx729618)阅读数：400
<script type="text/javascript">  
    //定义空数组
 var myhash={};
 //直接定义数组
 myhash = {"key1":"value1","key2":"value2"};
 //用Array定义数组
 myhash = new Array();
 myhash["key1"] ="value1";
 myhash["key2"] ="value2";
 //向hash关联数组添加键值
 myhash["newKey"] = "newValue";
 //删除hash关联数组已有键值
 delete myhash["key2"];
 //遍历整个hash数组
 for(key in myhash){
  val = myhash[key];
  alert(val);
 }
 //hash关联数组简易使用示例
 //转向脚本
 var urlhash = { "yahoo":"[www.yahoo.cn","baidu":"www.baidu.com","google":"www.google.cn]()"};
 //交互式使用示例
 userinfo = prompt("请输入您最想去的搜索引擎：(yahoo|baidu|google)", "yahoo");
 document.write ("您的选择：" + userinfo + "，<a href=http://" + getURL(userinfo) + " target=_blank>" + "按此即可进入</a>" + userinfo + "。");
 // getURL 
 // 如果参数未定义，默认返回 [www.yahoo.cn](http://www.yahoo.cn) 网址
 // @param choice 选择名称
 // @return url 实际的URL
 function getURL(choice) {
 url = urlhash[choice];
 if (typeof(urlhash[choice]) == "undefined")
     url = "[www.yahoo.cn](http://www.yahoo.cn)";
     return url;
 }
 // 获得hash列表的所有 keys
 // @param hash hash数组
 // @return keys 键名数据
 function array_keys(hash) {
     keys = [];
     for(key in hash){
     keys.push(key);
  }
  return keys;
 }
 alert(array_keys(myhash));
</script>
