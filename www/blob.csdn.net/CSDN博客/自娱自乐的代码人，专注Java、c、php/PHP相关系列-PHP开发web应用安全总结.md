# PHP相关系列 - PHP开发web应用安全总结 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月22日 15:30:59[initphp](https://me.csdn.net/initphp)阅读数：4553








> 
### **XSS跨站脚本**

概念：恶意攻击者往Web页面里插入恶意html代码，当用户浏览该页之时，嵌入其中Web里面的html代码会被执行，从而达到恶意用户的特殊目的。 

危害：
- 盗取用户COOKIE信息。
- 跳转到钓鱼网站。 
- 操作受害者的浏览器，查看受害者网页浏览信息等。 
- 蠕虫攻击。

描述：反射型跨站。GET或POST内容未过滤，可以提交JS以及HTML等恶意代码。 

代码：

```php
<?php echo $_GET['msg']; ?>
//正常URL
user.php?msg=henhao
//带JS的URL
user.php?msg=<script>alert(1)</script>
//恶意跳转URL
user.php?msg=<script>window.history.back(-1);</script>
```



解决方法：

输出过滤，php端输出到view的模板页面上的数据都需要经过过滤：

```php
/**
	 * 安全过滤类-过滤HTML标签
	 *  Controller中使用方法：$this->controller->filter_html($value)
	 * @param  string $value 需要过滤的值
	 * @return string
	 */
	public function filter_html($value) {
		if (function_exists('htmlspecialchars')) return htmlspecialchars($value);
		return str_replace(array("&", '"', "'", "<", ">"), array("&", """, "'", "<", ">"), $value);
	}
```



### **CSRF跨站攻击**


> 
概念：CSRF跨站请求伪造，也被称成为“one click attack”或者session riding，通常缩写为CSRF或者XSRF，是一种对网站的恶意利用。XSS利用站点内的信任用户，而CSRF则通过伪装来自受信任用户的请求来利用受信任的网站。与XSS攻击相比，CSRF攻击往往不大流行（因此对其进行防范的资源也相当稀少）和难以防范，所以被认为比XSS更具危险性。

危害：强迫受害者的浏览器向一个易受攻击的Web应用程序发送请求,最后达到攻击者所需要的操作行为。 

例子：
`<img src=“http://a.com/addfriend.php?id=123”/> `
1. 上面是一个图片的html标签，但是src中是一个添加id为123好友的新增好友链接。 

2. 恶意用户可以将这段代码植入其它网站网页上面，甚至可以img设置为0,0，让用户不知不觉中点击这个链接，达到用户并不像加这个人好友，但是添加的目的。

3. 当很多人都无意加了id为123这个人为好友的时候，id为123的恶意用户就有权限来查看这些人的信息，甚至可以发送很多恶意的信息，达到恶意用户的目的。 

解决方法：

1. http://a.com/addfriend.php?id=123 使用POST方法会相对安全一点。

2. 采用类似随即码或者令牌的形式，让用户操作唯一性。 （每次用户登录网站随机生成一个token，存放在cookie中，用户的所有操作中都需要经过token验证）

![](https://img-blog.csdn.net/20130902173329265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

带上这个init_token，然后每次请求都去验证一下就好了。token在浏览器打开的时候生效，关闭浏览器再打开浏览器的时候会变化


### **flash安全问题**


> 
例子：
`http://images.sohu.com/bill/s2010/liulin/nokia/1602600902.swf?clickthru=javascript:alert(1)`


解决方法：

在网站根目录中，添加crossdomain.xml文件，这个文件主要是控制flash的域访问。 

淘宝的：http://www.taobao.com/crossdomain.xml 

```
<?xml version="1.0" ?>  
<cross-domain-policy>  
<allow-access-from domain="*.taobao.com" />  
<allow-access-from domain="*.taobao.net" />  
<allow-access-from domain="*.taobaocdn.com" />  
<allow-access-from domain="*.tbcdn.cn" />  
<allow-access-from domain="*.allyes.com" />  
</cross-domain-policy>
```


> 
### **sql注入安全问题**

概念：所谓SQL注入，就是通过把SQL命令插入到Web表单递交或输入域名或页面请求的查询字符串，最终达到欺骗服务器执行恶意的SQL命令。

危害：

1. 查询数据库中敏感信息。 

2. 绕过认证。 

3. 添加、删除、修改服务器数据。 

4. 拒绝服务。?id=(BENCHMARK(100000000, MD5(RAND())); 

例子：
`$sql = "SELECT name FROM users WHERE id = '". $_GET['id'] . "'";`
当ID值为：1’ or 1=’1  SQL语句(已测试可以注入)：
`SELECT name FROM users WHERE id = ‘1’ or 1=’1 ‘`
说明：1=1的时候，条件语句WHEREOR之前的不起作用。 ‘的作用是组装SQL语句。 

解决方法：

SQL组装的时候，对外部变量以及所有变量都进行过滤： 

PHPWIND中，可以用sqlEscape、sqlImplode、sqlSingle、sqlMulti等函数过滤组装。过滤主要是一些’单引号这些可以破坏SQL组装的数据。

```php
/**
	 * SQL组装-私有SQL过滤
	 * @param  string $val 过滤的值
	 * @param  int    $iskey 0-过滤value值，1-过滤字段
	 * @return string
	 */
	private function build_escape_single($val, $iskey = 0) {
		if ($iskey === 0) {
			if (is_numeric($val)) {
				return " '" . $val . "' ";
			} else {
				return " '" . addslashes(stripslashes($val)) . "' ";
			}
		} else {
			$val = str_replace(array('`', ' '), '', $val);
			return ' `'.addslashes(stripslashes($val)).'` ';
		}
	}
```




> 
### **XML注入安全问题**


> 
概念：和SQL注入原理一样，XML是存储数据的地方，如果在查询或修改时，如果没有做转义，直接输入或输出数据，都将导致XML注入漏洞。攻击者可以修改XML数据格式，增加新的XML节点，对数据处理流程产生影响。

危害：

1. 攻击者可以新增XML节点 

2. 破坏原来的XML结构，影响业务流程，甚至产生严重的错误。 

例子：

```php
$xml = "<USER role=guest><name>“ . $_GET[‘name’] . "</name><email>“ . $_GET[‘email’] . "</email></USER>";
```

需要得到的XML结构： 

```php
<?xml version="1.0" encoding="UTF-8"?>   
<USER role=guest>  
<name>user1</name>  
<email>user1@a.com</email>  
</USER>
```

恶意代码： 

```php
user1@a.com</email></USER><USER role=admin><name>test</name><email>user2@a.com
```

意外的XML文档： 

```php
<?xml version="1.0" encoding="UTF-8"?>  
<USER role=guest>  
<name>user1</name>  
<email>user1@a.com</email>  
</USER>  
<USER role=admin>  
<name>test</name>  
<email>user2@a.com</email>  
</USER>
```

解决方法：

1. 对php处理XML文档的时候，进行标签过滤 

2. 尽量减少直接被外部访问到xml文档，可以采用文件名用散列方法等。 



### **url跳转漏洞**


> 
概念：Web应用程序接收到用户提交的URL参数后，没有对参数做”可信任URL”的验证，就向用户浏览器返回跳转到该URL的指令。 

危害：钓鱼网站

例子：

http://m.yahoo.cn/log.php?c=web&u=http://www.163.com

解决方法：

对跳转的php函数进行进一步优化，使页面跳转可以在可信任的范围内。 例如可以有跳转域名白名单方法，这个访问各大公司使用比较多



### **文件系统跨越漏洞**


> 
概念：对文件目录参数没有进行过滤，导致恶意用户可以通过在参数中输入一些执行命令，或者跨越访问的行为，来超出用户的访问权限。 

例子：通过一个或多个../跨越目录限制 

```php
$fp = fopen("image/{$_GET['filename']}", 'r');
```

Getfile?filename=../../../../etc/passwd

解决方法：

1. 对文本操作的时候一定要谨慎，不可信任 

2. 严格使用phpwind中安全类库 escapePath函数 



### **系统命令漏洞**


> 
概念：用户提交的参数用于执行系统命令的参数。 

解决：

1. 谨慎使用系统命令，对使用系统命令的地方需要进行安全评审 

2. 对命令语句进行严格过滤 



### **文件上传漏洞**


> 
概念：Web应用程序在处理用户上传的文件时，没有判断文件的扩展名是否在允许的范围内，或者没检测文件内容的合法性，就把文件保存在服务器上，甚至上传脚本木马到web服务器上，直接控制web服务器。

情况：

1. 未限制扩展名 

2. 未检查文件内容 

3. 病毒文件 

解决方法：

1. 使用安全的，可信任的上传组件。 

2. 检查文件扩展名，保证文件的类型正确。 

3. 检查文件内容，保证用户不伪造文件类型。 



### **任意文件下载漏洞 **


> 
解决方法：

1． Apache虚拟目录指向 

2． Java/PHP读取文件 



### **权限控制漏洞 **


> 
概念：属于业务逻辑上的安全管理。

访问控制：

1. 水平访问：Web应用程序接收到用户请求，修改某条数据时，没有判断数据的所属人，或判断数据所属人时，从用户提交的request参数（用户可控数据）中，获取了数据所属人id，导致恶意攻击者可以通过变换数据ID，或变换所属人id，修改不属于自己的数据。

2. 垂直访问：由于web应用程序没有做权限控制，或仅仅在菜单上做了权限控制，导致的恶意用户只要猜测其他管理页面的URL，就可以访问或控制其他角色拥有的数据或页面，达到权限提升目的。

存在情况：

1． URL级别的。（例如论坛需要操作评分的时候，有一个提交的URL地址，该地址提交过去，如果不做权限判断，那么恶意用户就可以随意的拿这个URL地址来进行恶意行为）

2． 菜单级别。（会员中心或者后台管理中心，会有菜单，管理员可以看到多个功能，普通管理员只能看到一部分功能。但是如果你对管理员操作的功能区不做权限判断，那么普通管理员只要猜测或者获取管理区的URL，就可以进行管理员操作了）

危害：

1. 属于业务逻辑的漏洞，这些危害性是巨大的，可以让普通用户就可能获取管理员的权限，对网站进行恶意破坏或者做非法行为。

解决方案：

1. 项目先期，做一份详细的权限规划文档。 

2. 在开发中严格按照权限文档的要求去做权限。 

3. 后期测试需要覆盖权限这一块功能区。 

4. 程序员需要经常注意这些方面的要求。 



### **cookie安全设置**


> 
解决：

cookie httponly flag ： 在用到用户名登陆密码之类的安全性比较高的cookie的时候，可以在cookie中设置httponly属性，该属性只允许php等访问cookie，而不允许js访问。

cookie secure flag ： 在涉及到https这样的情况，需要对cookie加密传输，那么可以设置这个属性



### **session安全**


> 
1. SESSION是保存在服务器端的，具有比COOKIE一定的安全性。 

2. 使用COOKIE的时候，如果长时间没有动作，可以设置一个时间值，来对COOKIE进行过期。 

3. 尽量让用户每次的cookie值都是不同的，这样可以保证cookie被盗取也不能长期使用的问题。



### 网络请求数据都需要判断






### HTTPS



敏感信息，请走HTTPS


















