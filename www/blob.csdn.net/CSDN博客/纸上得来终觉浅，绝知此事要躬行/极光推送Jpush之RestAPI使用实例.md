# 极光推送Jpush之RestAPI使用实例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年11月13日 18:02:37[boonya](https://me.csdn.net/boonya)阅读数：6881








前言：极光推送是国内比较专业的推送服务企业，听说很多写App的童孩都喜欢用激光的推送，我这里根据JpushAPI写了一个web端向客户端推送的实例程序，以资参考。

#### 1.App管理信息配置文件

jpush.properties




```python
# Jpush android application masterSecret**********************FOR TEST SAMPLE
jpush.masterSecret=9ba713ec2eadbea8d4e32616
# Jpush android application appKey
jpush.appKey=7b6c694575694606de9c5591

# Jpush android application masterSecret**********************FOR CUSTOMER APP
jpush.customer.masterSecret=cf972b8899b698365293dbcc
# Jpush android application appKey
jpush.customer.appKey=a25636c0fd5fc7e9bffaf83d

# Jpush android application masterSecret**********************FOR DRIVER  APP
jpush.driver.masterSecret=46e9c9e746e06a414206ab7c
# Jpush android application appKey
jpush.driver.appKey=abc69df6995402f0d117e8c3

#Jpush rest API address
jpush.jpushApi=https://api.jpush.cn
#Push support client platform  value can be "all" or ["android", "ios"]
jpush.platform=all
# set 0 means does not storage offline messages,default 86400==1 day,max value limit to 10 days.
jpush.options.time_to_live=86400
# apns way is to real environment or not.if not set this value means in real environment.
jpush.options.apns_production=true
```



#### 2.推送接口定义和调用

##### 2.1客户接口



```java
package com.wlyd.jyxt.service.push;

import java.util.List;

import com.wlyd.jyxt.persistence.beans.ylxt.YlPushMessage;
/**
 * 
 * @packge com.wlyd.jyxt.service.push.JPushForCustomerService
 * @date   2015年9月29日  下午3:23:35
 * @author pengjunlin
 * @comment   客户运力需求方App推送接口
 * @update
 */
public interface JPushForCustomerService {
	
	/**
	 * 
	 * @MethodName: pushMsgBroadCast 
	 * @Description: 推送广播消息【后台不记录广播消息】
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgBroadCast(String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgToRegistrationId 
	 * @Description: 【单个推送】推送消息到用户注册ID(deviceToken)
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgToRegistrationId(YlPushMessage pushMessage,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgToRegistrationIds 
	 * @Description: 【批量推送】推送消息到用户注册ID(deviceToken)List
	 * @param pushMessages
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgToRegistrationIds(List<YlPushMessage> pushMessages,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgByTag 
	 * @Description: 【单个推送】推送消息到用户标签(deviceToken)
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByTag(YlPushMessage pushMessage,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgByTags 
	 * @Description: 【批量推送】推送消息到用户标签(deviceToken)List
	 * @param pushMessages
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByTags(List<YlPushMessage> pushMessages,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgByAlias 
	 * @Description: 【单个推送】推送消息到用户手机号别名(deviceToken)
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByAlias(YlPushMessage pushMessage,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgByAlias 
	 * @Description: 【批量推送】推送消息到用户手机号别名(deviceToken)List
	 * @param pushMessages
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByAlias(List<YlPushMessage> pushMessages,String title,String alert);
}
```




##### 2.2客户接口实现



```java
package com.wlyd.jyxt.service.push.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import cn.jpush.api.common.resp.APIConnectionException;
import cn.jpush.api.common.resp.APIRequestException;
import cn.jpush.api.push.PushClient;
import cn.jpush.api.push.PushResult;
import cn.jpush.api.push.model.Platform;
import cn.jpush.api.push.model.PushPayload;
import cn.jpush.api.push.model.audience.Audience;
import cn.jpush.api.push.model.notification.Notification;

import com.alibaba.fastjson.JSON;
import com.wlyd.jyxt.persistence.beans.ylxt.YlPushMessage;
import com.wlyd.jyxt.service.push.JPushForCustomerService;
import com.wlyd.jyxt.service.push.YlPushMessageService;
/**
 * 
 * @packge com.wlyd.jyxt.service.push.impl.JPushForCustomerServiceImpl
 * @date   2015年9月29日  下午3:22:14
 * @author pengjunlin
 * @comment   客户运力需求方App推送接口实现
 * @update
 */
@Service
public class JPushForCustomerServiceImpl implements JPushForCustomerService{
	
	/**
	 * JPush REST API
	 */
	@Value("${jpush.jpushApi}")
	private  String jpushApi="";

	/**
	 * APP MASTER_SECRET
	 */
	@Value("${jpush.customer.masterSecret}")
	private  String MASTER_SECRET="";

	/**
	 * APP APP_KEY
	 */
	@Value("${jpush.customer.appKey}")
	private  String APP_KEY="";

	/**
	 * PLATFORM:推送消息平台设置
	 */
	@Value("${jpush.platform}")
	public  String PLATFORM="";

	/**
	 * OPTIONS_TIME_TO_LIVE:离线消息保存时间
	 */
	@Value("${jpush.options.time_to_live}")
	public  long OPTIONS_TIME_TO_LIVE=86400;

	/**
	 * OPTIONS_TIME_TO_LIVE:推送测试环境
	 */
	@Value("${jpush.options.apns_production}")
	public  boolean OPTIONS_APNS_PRODUCTION=false;

	private static PushClient pushClient = null;
	
	@Autowired
	YlPushMessageService pushMessageService;

	/**
	 * @MethodName: getInstance
	 * @Description: 获取推送客户端对象实例
	 * @return
	 * @throws
	 */
	public synchronized PushClient getInstance() {
		if (pushClient == null) {
			pushClient = new PushClient(MASTER_SECRET, APP_KEY);
			pushClient.setBaseUrl(jpushApi);
			pushClient.setDefaults(OPTIONS_APNS_PRODUCTION, OPTIONS_TIME_TO_LIVE);
		}
		return pushClient;
	}
	
	@Override
	public String pushMsgBroadCast(String title, String alert) {
		PushResult result = null;
		 try {
			 Platform platform = Platform.all();
			 Audience audience = Audience.all();
			 Notification notifcation = Notification.android(alert, title, null);
			 PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			 result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (APIRequestException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgToRegistrationId(YlPushMessage pushMessage,String title,String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
			Audience audience = Audience.registrationId(pushMessage.getYpmDeviceToken());
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			pushMessage.setYpmContent(alert+":"+ title);
			pushMessage.setYpmSuccessState(pushSuccess);
			pushMessage.setYpmPushTime(pushDate);
			// 接收消息状态：0:成功，1:未接收,2:拒收
			pushMessage.setYpmAcceptState(1);
			pushMessageService.insert(pushMessage);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgToRegistrationIds(List<YlPushMessage> pushMessages,
			String title, String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
	    	// 获取发送设备的标识
	    	List<String> registrationIds=new ArrayList<String>();
	    	for (int i = 0,j=pushMessages.size(); i < j; i++) {
	    		registrationIds.add(pushMessages.get(i).getYpmDeviceToken());
			}
			Audience audience = Audience.registrationId(registrationIds);
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			for (int i = 0,j=pushMessages.size(); i < j; i++) {
				pushMessages.get(i).setYpmContent(alert+":"+ title);
				pushMessages.get(i).setYpmSuccessState(pushSuccess);
				pushMessages.get(i).setYpmPushTime(pushDate);
				// 接收消息状态：0:成功，1:未接收,2:拒收
				pushMessages.get(i).setYpmAcceptState(1);
			}
			pushMessageService.batchInsertPushMsg(pushMessages);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByTag(YlPushMessage pushMessage, String title,
			String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
			Audience audience = Audience.tag(pushMessage.getYpmDeviceToken());
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			pushMessage.setYpmContent(alert+":"+ title);
			pushMessage.setYpmSuccessState(pushSuccess);
			pushMessage.setYpmPushTime(pushDate);
			// 接收消息状态：0:成功，1:未接收,2:拒收
			pushMessage.setYpmAcceptState(1);
			pushMessageService.insert(pushMessage);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByTags(List<YlPushMessage> pushMessages, String title,
			String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
	    	// 获取发送设备的标识
	    	List<String> tags=new ArrayList<String>();
	    	for (int i = 0,j=pushMessages.size(); i < j; i++) {
	    		tags.add(pushMessages.get(i).getYpmDeviceToken());
			}
			Audience audience = Audience.tag(tags);
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			for (int i = 0,j=pushMessages.size(); i < j; i++) {
				pushMessages.get(i).setYpmContent(alert+":"+ title);
				pushMessages.get(i).setYpmSuccessState(pushSuccess);
				pushMessages.get(i).setYpmPushTime(pushDate);
				// 接收消息状态：0:成功，1:未接收,2:拒收
				pushMessages.get(i).setYpmAcceptState(1);
			}
			pushMessageService.batchInsertPushMsg(pushMessages);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByAlias(YlPushMessage pushMessage, String title,
			String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
			Audience audience = Audience.alias(pushMessage.getYpmDeviceToken());
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			pushMessage.setYpmContent(alert+":"+ title);
			pushMessage.setYpmSuccessState(pushSuccess);
			pushMessage.setYpmPushTime(pushDate);
			// 接收消息状态：0:成功，1:未接收,2:拒收
			pushMessage.setYpmAcceptState(1);
			pushMessageService.insert(pushMessage);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByAlias(List<YlPushMessage> pushMessages,
			String title, String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
	    	// 获取发送设备的标识
	    	List<String> alias=new ArrayList<String>();
	    	for (int i = 0,j=pushMessages.size(); i < j; i++) {
	    		alias.add(pushMessages.get(i).getYpmDeviceToken());
			}
			Audience audience = Audience.alias(alias);
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			for (int i = 0,j=pushMessages.size(); i < j; i++) {
				pushMessages.get(i).setYpmContent(alert+":"+ title);
				pushMessages.get(i).setYpmSuccessState(pushSuccess);
				pushMessages.get(i).setYpmPushTime(pushDate);
				// 接收消息状态：0:成功，1:未接收,2:拒收
				pushMessages.get(i).setYpmAcceptState(1);
			}
			pushMessageService.batchInsertPushMsg(pushMessages);
		}
	    return JSON.toJSONString(result);
	}

}
```




##### 2.3司机接口



```java
package com.wlyd.jyxt.service.push;

import java.util.List;

import com.wlyd.jyxt.persistence.beans.ylxt.YlPushMessage;
/**
 * 
 * @packge com.wlyd.jyxt.service.push.JPushForDriverService
 * @date   2015年9月29日  下午3:24:27
 * @author pengjunlin
 * @comment   司机运力提供方App推送接口
 * @update
 */
public interface JPushForDriverService {
	
	/**
	 * 
	 * @MethodName: pushMsgBroadCast 
	 * @Description: 推送广播消息【后台不记录广播消息】
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgBroadCast(String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgToRegistrationId 
	 * @Description: 【单个推送】推送消息到用户注册ID(deviceToken)
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgToRegistrationId(YlPushMessage pushMessage,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgToRegistrationIds 
	 * @Description: 【批量推送】推送消息到用户注册ID(deviceToken)List
	 * @param pushMessages
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgToRegistrationIds(List<YlPushMessage> pushMessages,String title,String alert);

	/**
	 * 
	 * @MethodName: pushMsgByTag 
	 * @Description: 【单个推送】推送消息到用户手机号(deviceToken)
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByTag(YlPushMessage pushMessage,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgByTags 
	 * @Description: 【批量推送】推送消息到用户手机号(deviceToken)List
	 * @param pushMessages
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByTags(List<YlPushMessage> pushMessages,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgByAlias 
	 * @Description: 【单个推送】推送消息到用户手机号别名(deviceToken)
	 * @param pushMessage
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByAlias(YlPushMessage pushMessage,String title,String alert);
	
	/**
	 * 
	 * @MethodName: pushMsgByAlias 
	 * @Description: 【批量推送】推送消息到用户手机号别名(deviceToken)List
	 * @param pushMessages
	 * @param title
	 * @param alert
	 * @return
	 * @throws
	 */
	public String pushMsgByAlias(List<YlPushMessage> pushMessages,String title,String alert);
}
```



##### 2.4司机接口实现



```java
package com.wlyd.jyxt.service.push.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import sun.util.logging.resources.logging;
import cn.jpush.api.common.resp.APIConnectionException;
import cn.jpush.api.common.resp.APIRequestException;
import cn.jpush.api.push.PushClient;
import cn.jpush.api.push.PushResult;
import cn.jpush.api.push.model.Platform;
import cn.jpush.api.push.model.PushPayload;
import cn.jpush.api.push.model.audience.Audience;
import cn.jpush.api.push.model.notification.Notification;

import com.alibaba.fastjson.JSON;
import com.wlyd.jyxt.persistence.beans.ylxt.YlPushMessage;
import com.wlyd.jyxt.service.push.JPushForDriverService;
import com.wlyd.jyxt.service.push.YlPushMessageService;
import com.wlyd.jyxt.util.Log;
import com.wlyd.jyxt.util.Tools;
/**
 * 
 * @packge com.wlyd.jyxt.service.push.impl.JPushForDriverServiceImpl
 * @date   2015年9月29日  下午3:23:12
 * @author pengjunlin
 * @comment   司机运力提供方App推送接口实现
 * @update
 */
@Service
public class JPushForDriverServiceImpl implements JPushForDriverService {
	
	/**
	 * JPush REST API
	 */
	@Value("${jpush.jpushApi}")
	private  String jpushApi="";

	/**
	 * APP MASTER_SECRET
	 */
	@Value("${jpush.driver.masterSecret}")
	private  String MASTER_SECRET="";

	/**
	 * APP APP_KEY
	 */
	@Value("${jpush.driver.appKey}")
	private  String APP_KEY="";

	/**
	 * PLATFORM:推送消息平台设置
	 */
	@Value("${jpush.platform}")
	public  String PLATFORM="";

	/**
	 * OPTIONS_TIME_TO_LIVE:离线消息保存时间
	 */
	@Value("${jpush.options.time_to_live}")
	public  long OPTIONS_TIME_TO_LIVE=86400;

	/**
	 * OPTIONS_TIME_TO_LIVE:推送测试环境
	 */
	@Value("${jpush.options.apns_production}")
	public  boolean OPTIONS_APNS_PRODUCTION=false;

	private static PushClient pushClient = null;
	
	@Autowired
	YlPushMessageService pushMessageService;

	/**
	 * @MethodName: getInstance
	 * @Description: 获取推送客户端对象实例
	 * @return
	 * @throws
	 */
	public synchronized PushClient getInstance() {
		if (pushClient == null) {
			pushClient = new PushClient(MASTER_SECRET, APP_KEY);
			pushClient.setBaseUrl(jpushApi);
			pushClient.setDefaults(OPTIONS_APNS_PRODUCTION, OPTIONS_TIME_TO_LIVE);
		}
		return pushClient;
	}
	
	@Override
	public String pushMsgBroadCast(String title, String alert) {
		PushResult result = null;
		 try {
			 Platform platform = Platform.all();
			 Audience audience = Audience.all();
			 Notification notifcation = Notification.android(alert, title, null);
			 PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			 result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (APIRequestException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgToRegistrationId(YlPushMessage pushMessage,String title,String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
			Audience audience = Audience.registrationId(pushMessage.getYpmDeviceToken());
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			pushMessage.setYpmContent(alert+":"+ title);
			pushMessage.setYpmSuccessState(pushSuccess);
			pushMessage.setYpmPushTime(pushDate);
			// 接收消息状态：0:成功，1:未接收,2:拒收
			pushMessage.setYpmAcceptState(1);
			pushMessageService.insert(pushMessage);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgToRegistrationIds(List<YlPushMessage> pushMessages,
			String title, String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
	    	// 获取发送设备的标识
	    	List<String> registrationIds=new ArrayList<String>();
	    	for (int i = 0,j=pushMessages.size(); i < j; i++) {
	    		registrationIds.add(pushMessages.get(i).getYpmDeviceToken());
			}
			Audience audience = Audience.registrationId(registrationIds);
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			for (int i = 0,j=pushMessages.size(); i < j; i++) {
				pushMessages.get(i).setYpmContent(alert+":"+ title);
				pushMessages.get(i).setYpmSuccessState(pushSuccess);
				pushMessages.get(i).setYpmPushTime(pushDate);
				// 接收消息状态：0:成功，1:未接收,2:拒收
				pushMessages.get(i).setYpmAcceptState(1);
			}
			pushMessageService.batchInsertPushMsg(pushMessages);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByTag(YlPushMessage pushMessage, String title,
			String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
			Audience audience = Audience.tag(pushMessage.getYpmDeviceToken());
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			pushMessage.setYpmContent(alert+":"+ title);
			pushMessage.setYpmSuccessState(pushSuccess);
			pushMessage.setYpmPushTime(pushDate);
			// 接收消息状态：0:成功，1:未接收,2:拒收
			pushMessage.setYpmAcceptState(1);
			pushMessageService.insert(pushMessage);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByTags(List<YlPushMessage> pushMessages, String title,
			String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
	    	// 获取发送设备的标识
	    	List<String> tags=new ArrayList<String>();
	    	for (int i = 0,j=pushMessages.size(); i < j; i++) {
	    		tags.add(pushMessages.get(i).getYpmDeviceToken());
			}
			Audience audience = Audience.tag(tags);
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			for (int i = 0,j=pushMessages.size(); i < j; i++) {
				pushMessages.get(i).setYpmContent(alert+":"+ title);
				pushMessages.get(i).setYpmSuccessState(pushSuccess);
				pushMessages.get(i).setYpmPushTime(pushDate);
				// 接收消息状态：0:成功，1:未接收,2:拒收
				pushMessages.get(i).setYpmAcceptState(1);
			}
			pushMessageService.batchInsertPushMsg(pushMessages);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByAlias(YlPushMessage pushMessage, String title,
			String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
			Audience audience = Audience.alias(pushMessage.getYpmDeviceToken());
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			pushMessage.setYpmContent(alert+":"+ title);
			pushMessage.setYpmSuccessState(pushSuccess);
			pushMessage.setYpmPushTime(pushDate);
			// 接收消息状态：0:成功，1:未接收,2:拒收
			pushMessage.setYpmAcceptState(1);
			pushMessageService.insert(pushMessage);
		}
	    return JSON.toJSONString(result);
	}

	@Override
	public String pushMsgByAlias(List<YlPushMessage> pushMessages,
			String title, String alert) {
		PushResult result = null;
	    try {
	    	Platform platform = Platform.all();
	    	// 获取发送设备的标识
	    	List<String> alias=new ArrayList<String>();
	    	for (int i = 0,j=pushMessages.size(); i < j; i++) {
	    		alias.add(pushMessages.get(i).getYpmDeviceToken());
			}
			Audience audience = Audience.alias(alias);
			Notification notifcation = Notification.android(alert, title, null);
	    	PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
			result = getInstance().sendPush(pushPayload);
		} catch (APIConnectionException e) {
			e.printStackTrace();
		} catch (APIRequestException e) {
			e.printStackTrace();
		}finally{
			//保存推送消息
			Date pushDate=new Date();
			int pushSuccess=result.getResponseCode()==200?0:1;
			for (int i = 0,j=pushMessages.size(); i < j; i++) {
				pushMessages.get(i).setYpmContent(alert+":"+ title);
				pushMessages.get(i).setYpmSuccessState(pushSuccess);
				pushMessages.get(i).setYpmPushTime(pushDate);
				// 接收消息状态：0:成功，1:未接收,2:拒收
				pushMessages.get(i).setYpmAcceptState(1);
			}
			pushMessageService.batchInsertPushMsg(pushMessages);
		}
	    return JSON.toJSONString(result);
	}

}
```

##### 2.5Junit测试

里面有无关的测试，看的时候可以忽略掉：




```java
package com.wlyd.jyxt;

import java.util.Date;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import cn.jpush.api.push.model.Platform;
import cn.jpush.api.push.model.PushPayload;
import cn.jpush.api.push.model.audience.Audience;
import cn.jpush.api.push.model.notification.Notification;

import com.wlyd.jyxt.persistence.beans.ylxt.YlPushMessage;
import com.wlyd.jyxt.service.push.JPushForCustomerService;
import com.wlyd.jyxt.service.push.JPushForDriverService;
import com.wlyd.jyxt.service.push.JPushService;
/**
 * @packge com.wlyd.jyxt.JPushServiceTest
 * @date   2015年9月18日  下午3:23:25
 * @author pengjunlin
 * @comment   REST API测试
 * @update
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class JPushServiceTest {
	
	@Autowired
	JPushService jPushService;
	
	@Autowired
	JPushForCustomerService jPushForCustomerService;
	
	@Autowired
	JPushForDriverService jPushForDriverService;
	
	/**
	 * 
	 * @MethodName: testPush 
	 * @Description: 推送alert通知 by jsonString
	 * @throws
	 */
	@Test
	public void testPush(){
		String jsonParams="{\""+"platform"+"\":"+"\""+"all"+"\","+"\""+"audience"+"\":"+"\""+"all"+"\","+"\""+"notification"+"\":{"+"\""+"alert"+"\":"+"\""+"Hi,JPush!"+"\"}"+"}";
		System.out.println(jsonParams); 
		String result=jPushService.pushAndroidMsg(jsonParams);
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * @MethodName: testPushPayload 
	 * @Description:  推送alert通知by PushPayload
	 * @throws
	 */
	@Test
	public void testPushPayload(){
	    Platform platform = Platform.all();
	    Audience audience = Audience.all();
	    Notification notifcation = Notification.alert("alert");
	    PushPayload pushPayload=PushPayload.newBuilder().setPlatform(platform).setAudience(audience).setNotification(notifcation).build();
		String result=jPushService.pushAndroidMsgByPushPayload(pushPayload);
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * 
	 * @MethodName: testPushMessageByRegistrationIdForCustomer 
	 * @Description: 推送消息并保持到JYXT
	 * @throws
	 */
	@Test
	public void testPushMessageByRegistrationIdForCustomer(){
		YlPushMessage pushMessage=new YlPushMessage();
		pushMessage.setYpmReceiveCode("YL019");
		pushMessage.setYpmMasterCode(""+System.currentTimeMillis());
		pushMessage.setYpmContent("尊敬的客户朋友：成都-上海 线路的运力需求，发布成功");
		pushMessage.setYpmPushTime(new Date());
		pushMessage.setYpmDeviceToken("shiwl111");
		pushMessage.setYpmSuccessState(1);
		pushMessage.setYpmAcceptState(1);
		pushMessage.setYpmPushState(1);
		String result=jPushForCustomerService.pushMsgToRegistrationId(pushMessage, "成都-上海", pushMessage.getYpmContent());
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * 
	 * @MethodName: testPushMessageByTagForCustomer 
	 * @Description: 推送消息并保持到JYXT
	 * @throws
	 */
	@Test
	public void testPushMessageByTagForCustomer(){
		YlPushMessage pushMessage=new YlPushMessage();
		pushMessage.setYpmReceiveCode("YL019");
		pushMessage.setYpmMasterCode(""+System.currentTimeMillis());
		pushMessage.setYpmContent("尊敬的客户朋友：成都-上海 线路的运力需求，发布成功");
		pushMessage.setYpmPushTime(new Date());
		pushMessage.setYpmDeviceToken("shiwl111");
		pushMessage.setYpmSuccessState(1);
		pushMessage.setYpmAcceptState(1);
		pushMessage.setYpmPushState(1);
		String result=jPushForCustomerService.pushMsgByTag(pushMessage, "成都-上海", pushMessage.getYpmContent());
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * 
	 * @MethodName: testPushMessageByAliasForCustomer 
	 * @Description: 推送消息并保持到JYXT
	 * @throws
	 */
	@Test
	public void testPushMessageByAliasForCustomer(){
		YlPushMessage pushMessage=new YlPushMessage();
		pushMessage.setYpmReceiveCode("YL019");
		pushMessage.setYpmMasterCode(""+System.currentTimeMillis());
		pushMessage.setYpmContent("尊敬的客户朋友：成都-上海 线路的运力需求，发布成功");
		pushMessage.setYpmPushTime(new Date());
		pushMessage.setYpmDeviceToken("account");
		pushMessage.setYpmSuccessState(1);
		pushMessage.setYpmAcceptState(1);
		pushMessage.setYpmPushState(1);
		String result=jPushForCustomerService.pushMsgByAlias(pushMessage, "成都-上海", pushMessage.getYpmContent());
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * 
	 * @MethodName: testPushBrodcastForCustomer 
	 * @Description: 推送消息并保持到JYXT
	 * @throws
	 */
	@Test
	public void testPushBrodcastForCustomer(){
		String result=jPushForCustomerService.pushMsgBroadCast("成都-上海", "尊敬的客户朋友：成都-上海 线路的运力需求，发布成功");
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * 
	 * @MethodName: testPushMessageByRegistrationIdForDriver 
	 * @Description: 推送消息并保持到JYXT
	 * @throws
	 */
	@Test
	public void testPushMessageByRegistrationIdForDriver(){
		YlPushMessage pushMessage=new YlPushMessage();
		pushMessage.setYpmReceiveCode("YL001YL150916005");
		pushMessage.setYpmMasterCode(""+System.currentTimeMillis()); 
		pushMessage.setYpmContent("尊敬的司机朋友：成都-上海 线路的运力需求，正在抢单");
		pushMessage.setYpmPushTime(new Date());
		pushMessage.setYpmDeviceToken("shiwl111");
		pushMessage.setYpmSuccessState(1);
		pushMessage.setYpmAcceptState(1);
		pushMessage.setYpmPushState(0);
		String result=jPushForDriverService.pushMsgToRegistrationId(pushMessage, "成都-上海", pushMessage.getYpmContent());
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * 
	 * @MethodName: testPushMessageByTagForDriver 
	 * @Description: 推送消息并保持到JYXT
	 * @throws
	 */
	@Test
	public void testPushMessageByTagForDriver(){
		YlPushMessage pushMessage=new YlPushMessage();
		pushMessage.setYpmReceiveCode("YL001YL150916005");
		pushMessage.setYpmMasterCode(""+System.currentTimeMillis()); 
		pushMessage.setYpmContent("尊敬的司机朋友：成都-上海 线路的运力需求，正在抢单");
		pushMessage.setYpmPushTime(new Date());
		pushMessage.setYpmDeviceToken("shiwl111");
		pushMessage.setYpmSuccessState(1);
		pushMessage.setYpmAcceptState(1);
		pushMessage.setYpmPushState(0);
		String result=jPushForDriverService.pushMsgByTag(pushMessage, "成都-上海", pushMessage.getYpmContent());
		System.out.println("推送返回结果：");
		System.out.println(result);
	}
	
	/**
	 * 
	 * @MethodName: testPushMessageByAliasForDriver 
	 * @Description: 推送消息并保持到JYXT
	 * @throws
	 */
	@Test
	public void testPushMessageByAliasForDriver(){
		YlPushMessage pushMessage=new YlPushMessage();
		pushMessage.setYpmReceiveCode("WS001YL151010005");
		pushMessage.setYpmMasterCode(""+System.currentTimeMillis()); 
		pushMessage.setYpmContent("尊敬的司机朋友：成都-上海 线路的运力需求，正在抢单");
		pushMessage.setYpmPushTime(new Date());
		pushMessage.setYpmDeviceToken("13882287884");
		pushMessage.setYpmSuccessState(1);
		pushMessage.setYpmAcceptState(1);
		pushMessage.setYpmPushState(0);
		for (int i = 0; i < 3; i++) {
			String result=jPushForDriverService.pushMsgByAlias(pushMessage, "成都-上海", pushMessage.getYpmContent());
			System.out.println("推送返回结果：");
			System.out.println(result);
		}
		
	}

}
```





#### 3.题外话mybatis批量操作

批量修改、批量删除、批量插入



```
<!-- unAccepted -->
	<update id="batchUpdateAcceptStateToUnaccepted" parameterType="java.util.List"> 
	   UPDATE yl_push_message SET YPM_ACCEPT_STATE = 1  where YPM_ID in
       <foreach collection="list" item="item" index="index" open="(" separator="," close=")" > 
         #{item}
       </foreach> 
    </update> 
    <!-- refuse to accept -->
	<update id="batchUpdateAcceptStateToRefuseAccept" parameterType="java.util.List"> 
	   UPDATE yl_push_message SET YPM_ACCEPT_STATE = 2  where YPM_ID in
       <foreach collection="list" item="item" index="index" open="(" separator="," close=")" > 
         #{item}
       </foreach> 
    </update> 
    
     <!-- batch delete  push msg -->
    <delete id="batchDeletePushMsg" parameterType="com.wlyd.jyxt.util.CondOrderLimit"> 
       DELETE FROM yl_push_message WHERE ypm_master_code IN (SELECT c.YMC_CODE from yl_master_car c where c.YMC_STATE in (2,3,4,5))
	   and ypm_receive_code=#{conditions.driverCode,jdbcType=VARCHAR} and YPM_ID in
	   <foreach collection="conditions.ypmIds" index="index" item="item" open="(" separator="," close=")">  
           #{item}  
       </foreach> 
    </delete>  
    
    <!-- batch insert push msg -->
    <insert id="batchInsertPushMsg" parameterType="java.util.List"  >
    insert into yl_push_message (YPM_RECEIVE_CODE, YPM_PUSH_STATE, YPM_MASTER_CODE,YPM_CONTENT, YPM_PUSH_TIME, YPM_SUCCESS_STATE,YPM_DEVICE_TOKEN, YPM_ACCEPT_STATE)   values 
     <foreach collection="list" item="item" index="index" separator="," > 
      (
       #{item.ypmReceiveCode},
       #{item.ypmPushState},
       #{item.ypmMasterCode}, 
       #{item.ypmContent}, 
       #{item.ypmPushTime},
       #{item.ypmSuccessState}, 
       #{item.ypmDeviceToken}, 
       #{item.ypmAcceptState}
       )
    </foreach> 
</insert>
```







