# js关联数组 - z69183787的专栏 - CSDN博客
2015年07月22日 13:20:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3387
标准javascript 是内含支持hash关联数组，经查找资料并测试，有关标准javascript内含的hash关联数组操作备忘如下
**１。Hash关联数组定义**
// 定义空数组 
myhash = { }
// 直接定义数组 
myhash = {”key1″:”val1″, “key2″:”val2″ }
// 用Array 定义数组 
myhash = new Array();
myhash[”key1″] = “val1″;
myhash[”key2″] = “val2″;
**２。向Hash关联数组添加键值**
// 添加一个新键 newkey ，键值为 newval 
myhash[”newkey”] = “newval”;
**３。删除Hash关联数组已有键值**
// 删除一个键 newkey ，同时，该键值对应的 newval 也就消失了
delete myhash[”newkey”];
**４。遍历Hash关联数组**
// 遍历整个hash 数组 
for (key in myhash) {
val = myhash[key];
}
**５。Hash关联数组简易使用示例**
// 转向脚本 
<script type=”text/javascript”>
urlhash = { “yahoo”:”[www.yahoo.cn](http://www.yahoo.cn/)“,
“baidu”:”[www.baidu.com](http://www.baidu.com/)“,
“google”:”[www.google.cn](http://www.google.cn/)” };
// 交互式使用示例 
userinfo = prompt(”请输入您最想去的搜索引擎：(yahoo|baidu|google)”, “yahoo”);
document.write (”您的选择：” + userinfo + “，<a href=http://” + getURL(userinfo) + ” target=_blank>” + “按此即可进入” + “</a>” + userinfo + “。”);
// getURL 
// 如果参数未定义，默认返回 [www.yahoo.cn](http://www.yahoo.cn/) 网址
// @param choice 选择名称
// @return url 实际的URL
function getURL(choice) {
url = urlhash[choice];
if (typeof(urlhash[choice]) == “undefined”)
url = “[www.yahoo.cn](http://www.yahoo.cn/)“;
return url;
}
// 获得hash列表的所有 keys
// @param hash hash数组
// @return keys 键名数据
function array_keys(hash) {
keys = [];
for (key in hash)
keys.push(key);
return keys;
}
</script>
