# 成功抓取csdn阅读量过万博文 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年12月05日 09:31:08[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：536
http://images.cnblogs.com/cnblogs_com/elesos/1120632/o_111.png

var commentscount = 1; 嵌套的评论算一条，这个可能有一点点误差，不过不影响
var username = "miniblog";
阅读数在2w的才保存
先选一个 
[http://blog.csdn.net/leixiaohua1020/article/details/15811977](http://blog.csdn.net/leixiaohua1020/article/details/15811977)
提取链接时，只提取visited_num>2w的
//*[@id="rasss"]
如何不提取其它url,只提取 [http://blog.csdn.net/leixiaohua1020/article/details/15811977](http://blog.csdn.net/leixiaohua1020/article/details/15811977)
呢
必须含有aritcle关键字

xpath提取页面中script里声明的变量值
<script type="text/javascript">
*var username = "miniblog";*
var _blogger = username;
var blog_address = "http://blog.csdn.net/miniblog";
var static_host = "http://csdnimg.cn/release/phoenix/";
var currentUserName = "elesos"; 
var fileName = '50214911';
var commentscount = 0;
var islock = false
window.quickReplyflag = true;
var totalFloor = 0;
var isBole = false;
var isDigg = false;
var isExpert=false;
var isAdm = false;
</script>
response.xpath('/html/head/script[1]').re('username = "(.+)"')
response.xpath('/html/head/script[1]').re("fileName = '(.+)'")
response.xpath('/html/head/script[1]').re("commentscount = (.+);")
注意元素是从1开始的 ,如div[1]表示第一个，不是div[0]
xpath最外层最好用单引号！除非必须使用双引号（比如里面有单引号了）


这种页面目前不能爬
[http://blog.csdn.net/u010164190/article/details/53166334](http://blog.csdn.net/u010164190/article/details/53166334)
不要执迷于技术了，把学一门技术所花的精力用一半到其它领域，如财经等，可能会有更多收获。
