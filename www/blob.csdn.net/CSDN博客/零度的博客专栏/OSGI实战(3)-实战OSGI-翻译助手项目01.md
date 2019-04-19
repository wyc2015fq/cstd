# OSGI实战(3)-实战OSGI-翻译助手项目01 - 零度的博客专栏 - CSDN博客
2017年01月17日 11:23:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：321
                
我们通过写一个真实的Web项目来深入学习使用OSGI技术。
我们做一个Web版的翻译助手(类似电子英汉词典之类的)
(1)服务接口模块
我们在Eclipse中创建一个pulg-in项目:
![](https://img-blog.csdn.net/20160316174026334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316174038311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们创建一个接口，并编写方法
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50906996#)[copy](http://blog.csdn.net/acmman/article/details/50906996#)[print](http://blog.csdn.net/acmman/article/details/50906996#)[?](http://blog.csdn.net/acmman/article/details/50906996#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612452/fork)
- package com.zyg.osgi.translateasslt.service;  
- 
- publicinterface TranslateService {  
- //翻译方法
- public String translate(String word);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.translateasslt.service;
public interface TranslateService {
    //翻译方法
    public String translate(String word);
}
```
我们回想一下现在的翻译软件，当有网络的时候执行的是网络查询，没有网络的时候进行的本地查找，所以我们这个项目要除了这个TranslateAsslt之外，还要衍生出两个插件，分别是“本地查询”模块和“网络查询”模块。
我们到时候要实现随时换本地端，随时换网络端的服务。
(2)本地查询插件
我们这里开始建立本地查询的插件
![](https://img-blog.csdn.net/20160316174103295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316174120366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们接下来要实现TranslateAsslt的TranslateService接口，重点来了，我们没有将TranslateAsslt项目的任何jar包加入我们的localTranslateAsslt项目，我们如何去调用TranslateAsslt的TranslateService接口呢？
重点就在MANIFEST.MF配置上，我们可以选择对外公布哪些插件，然后引入外部的哪些插件。
为了使用TranslateAsslt的TranslateService接口，我们首先让TranslateService接口“暴露”给其它的插件，也就是在TranslateAsslt的
MANIFEST.MF配置文件中配置需要“暴露”给其他插件的部分:
![](https://img-blog.csdn.net/20160316174129929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在localTranslateAsslt工程中通过MANIFEST.MF配置文件引入TranslateAsslt“暴露”给外面的部分:
![](https://img-blog.csdn.net/20160316174148133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来我们就可以引用TranslateService接口了。
我们创建一个TranslateService接口的实现类TranslateServiceLocalImpl:
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50906996#)[copy](http://blog.csdn.net/acmman/article/details/50906996#)[print](http://blog.csdn.net/acmman/article/details/50906996#)[?](http://blog.csdn.net/acmman/article/details/50906996#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612452/fork)
- package com.zyg.osgi.localtranslateasslt.impls;  
- 
- import java.util.concurrent.ConcurrentHashMap;  
- import com.zyg.osgi.translateasslt.service.TranslateService;  
- 
- publicclass TranslateServiceLocalImpl implements TranslateService {  
- privatestaticfinal ConcurrentHashMap<String, String> dictonary=new ConcurrentHashMap<String, String>();  
- static{  
-         dictonary.put("China", "中国");  
-         dictonary.put("USA", "美国");  
-     }  
- 
- @Override
- public String translate(String word) {  
-         System.out.println("LocalTranslateAsslt Service---------->");  
-         String result=dictonary.get(word);  
- if(null==result){  
-             result="未找到您所查单词意义，请检查单词是否正确!";  
-         }  
- return result;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.localtranslateasslt.impls;
import java.util.concurrent.ConcurrentHashMap;
import com.zyg.osgi.translateasslt.service.TranslateService;
public class TranslateServiceLocalImpl implements TranslateService {
	private static final ConcurrentHashMap<String, String> dictonary=new ConcurrentHashMap<String, String>();
	static{
		dictonary.put("China", "中国");
		dictonary.put("USA", "美国");
	}
	
	@Override
	public String translate(String word) {
		System.out.println("LocalTranslateAsslt Service---------->");
		String result=dictonary.get(word);
		if(null==result){
			result="未找到您所查单词意义，请检查单词是否正确!";
		}
		return result;
	}
}
```
因为我们的程序侧重于解决大规模并发访问的问题，我们用并发包(java.util.concurrent.)中的ConcurrentHashMap<K, V>来储存查询对象。
(3)网络查询插件
接下来我们创建一个网络端的查询插件:
![](https://img-blog.csdn.net/20160316174211727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316174221664?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后我们也引入TranslateAsslt“暴露”的TranslateService接口:
![](https://img-blog.csdn.net/20160316174232280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后创建TranslateService接口的实现类TranslateServiceRemoteImpl：
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50906996#)[copy](http://blog.csdn.net/acmman/article/details/50906996#)[print](http://blog.csdn.net/acmman/article/details/50906996#)[?](http://blog.csdn.net/acmman/article/details/50906996#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1612452/fork)
- package com.zyg.osgi.remotetranslateasslt.impls;  
- 
- import java.util.concurrent.ConcurrentHashMap;  
- import com.zyg.osgi.translateasslt.service.TranslateService;  
- 
- publicclass TranslateServiceRemoteImpl implements TranslateService {  
- privatestaticfinal ConcurrentHashMap<String, String> dictonary=new ConcurrentHashMap<String, String>();  
- static{  
-         dictonary.put("cat", "小猫");  
-         dictonary.put("dog", "小狗");  
-     }  
- 
- @Override
- public String translate(String word) {  
-         System.out.println("RemoteTranslateAsslt Service---------->");  
-         String result=dictonary.get(word);  
- if(null==result){  
-             result="未找到您所查单词意义，请检查单词是否正确!";  
-         }  
- return result;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.zyg.osgi.remotetranslateasslt.impls;
import java.util.concurrent.ConcurrentHashMap;
import com.zyg.osgi.translateasslt.service.TranslateService;
public class TranslateServiceRemoteImpl implements TranslateService {
	private static final ConcurrentHashMap<String, String> dictonary=new ConcurrentHashMap<String, String>();
	static{
		dictonary.put("cat", "小猫");
		dictonary.put("dog", "小狗");
	}
	
	@Override
	public String translate(String word) {
		System.out.println("RemoteTranslateAsslt Service---------->");
		String result=dictonary.get(word);
		if(null==result){
			result="未找到您所查单词意义，请检查单词是否正确!";
		}
		return result;
	}
}
```
(4)客户端入口插件
然后我们还需要一个客户端的pulg-in项目TranslateClient，提供系统运行前台界面和组织各个模块的调用:
![](https://img-blog.csdn.net/20160316174629297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316174640701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们要以Web的方式来访问我们的应用。我们要在插件里面写Web，而不是在TomCat的环境下写，但是最终都是要进行Http访问的。
我们osgi中嵌入的是jetty服务器，还有http访问所需要的一切插件都是Web访问所必须的，所以我们要配置Web的访问环境。所以我们新建一个运行配置(Run Configurations--->OSGI Framework--->New)，给这个配置起名为“TranslateService”，我们要进行必要插件的安装:
![](https://img-blog.csdn.net/20160316174709876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316174732984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(两幅图展示了运行时需要的所有插件jar，大家根据情况可以自己添加，必要时按检测按钮Validate Bundles查看插件是否引入完整)
要注意:我们的Run Configurations配置的是“运行时的环境”，跟我们开发时需要些什么东西，引入什么jar包两边是没有任何关系的。“运行时的环境”就是你编译之后运行时的class程序需要调用什么，就到运行时环境中去寻找，所以这里编译环境和运行环境是不一样的，就像我们的下载的jdk一样，有时候会带有一个jre，当我们自己不写代码，只运行别人的代码的时候，只装一个jre都足够了，但是如果自己开发的话，就要安装jdk(jre都可以不用装)。这里的概念和上面一样。
大家启动的时候可能会遇到这种问题：
![](https://img-blog.csdn.net/20160316174804438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这说明我们osgi的启动端口被占用了，我们要解除占用，有两个方法:
(1)更改osgi的启动端口
在配置选项卡中选择“{x}= Arguments”，然后加入
“-Dorg.osgi.service.http.port=8010”即可(可以自己指定端口号，我这里使用的是8010端口)
![](https://img-blog.csdn.net/20160316174816220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(2)找到占用端口的程序，杀死它
运行cmd控制台，输入netstat -ano指令查看端口使用情况:
![](https://img-blog.csdn.net/20160316174825516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316174833595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
osgi默认端口是80，我们在控制台中找到使用80端口的程序，找到他的PID为4，接下来我们到“任务管理器”(Ctrl+Alt+Delete键呼出)中找到程序详细信息列表，点击PID让其按照PID排序，找到PID为4的程序，杀死它:
![](https://img-blog.csdn.net/20160316174843189?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(很遗憾我的80端口被系统占用了，所以我使用了第一种方法解决了端口占用的问题)
解决上述问题，我们就可以开始启动我们的项目了:
![](https://img-blog.csdn.net/20160316174858705?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看到这个之后，说明我们的运行环境配置正确:
![](https://img-blog.csdn.net/20160316174908626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们接下来要做的就是Web项目的一些内容的编写，包括Servlet的创建，为了让大家看的更明白一些，我将其单独总结到下一篇文章中。
**转载请注明出处:[http://blog.csdn.net/acmman/article/details/50906996](http://blog.csdn.net/acmman/article/details/50906996)**
