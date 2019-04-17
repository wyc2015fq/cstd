# Java 新浪微博开发包weibo4j - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年10月07日 21:04:59[boonya](https://me.csdn.net/boonya)阅读数：2394









From ：[http://www.oschina.net/p/weibo4j](http://www.oschina.net/p/weibo4j)


  weibo4j 又名 sinat4j 是新浪微博的 Java 客户端开发包，支持Basic Auth, OAuth, 上传图片，头像，地理位置等功能。示例代码：



package weibo4j.examples;

import weibo4j.Status;
import weibo4j.Weibo; 

public class UpdateStatus { 

	/**
	 * 发布一条微博信息 
	 * @param args
	 */
	public static void main(String[] args) {
		System.setProperty("weibo4j.oauth.consumerKey", Weibo.CONSUMER_KEY);
    	System.setProperty("weibo4j.oauth.consumerSecret", Weibo.CONSUMER_SECRET);
       try {
       	Weibo weibo = getWeibo(true,args);
       	Status status = weibo.updateStatus("测试发表微博");
       	System.out.println(status.getId() + " : "+ status.getText()+"  "+status.getCreatedAt());
       	
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static Weibo getWeibo(boolean isOauth,String ... args) {
		Weibo weibo = new Weibo();
		if(isOauth) {//oauth验证方式 args[0]:访问的token；args[1]:访问的密匙
			weibo.setToken(args[0], args[1]);
		}else {//用户登录方式
    		weibo.setUserId(args[0]);//用户名/ID
   		weibo.setPassword(args[1]);//密码
		}
		return weibo;
	}
}




下载地址：[http://www.oschina.net/action/project/go?id=16925&p=download](http://www.oschina.net/action/project/go?id=16925&p=download)



