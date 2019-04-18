# Java静态代码块与spring注解注入的优先级问题 - z69183787的专栏 - CSDN博客
2012年11月17日 14:45:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13178
遇到这样一个问题，代码如下：
```java
package com.wonders.stpt.organTree.util;
import java.util.HashMap;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;
import com.wonders.stpt.organTree.service.TorganRelationService;
@Component("organTreeUtil")
public class OrganTreeUtil {
	private static HashMap<String,String> receiversMap = null;
	private static HashMap<String,String> configersMap = null;
	private static TorganRelationService torganRelationService;
	public static TorganRelationService getTorganRelationService() {
		return torganRelationService;
	}
	@Autowired(required=false)
	public void setTorganRelationService(
			@Qualifier("torganRelationService")TorganRelationService torganRelationService) {
		OrganTreeUtil.torganRelationService = torganRelationService;
	}
	
	static {
		init();
	}
	
	public static void init(){
		if(receiversMap==null){
			//receiversMap = new HashMap<String, String>();
			//receiversMap = (HashMap<String, String>) torganRelationService.getReceivers();
		}
		if(configersMap==null){
			//configersMap = new HashMap<String, String>();
			//configersMap = (HashMap<String, String>) torganRelationService.getConfigers();
		}
	}
	
	public static HashMap<String,String> getReceiversMap(){
		synchronized(receiversMap){
			if(receiversMap==null){
				receiversMap = (HashMap<String, String>) torganRelationService.getReceivers();
			}
		}
		return receiversMap;
	}
	
	public static HashMap<String,String> getConfigersMap(){
		synchronized(configersMap){
			if(configersMap==null){
				configersMap = (HashMap<String, String>) torganRelationService.getReceivers();
			}
		}
		return configersMap;
	}
	
	public static void refresh(){
		receiversMap = null;
		configersMap = null;
	}
	
}
```
可以看出，本来想在静态代码块中直接初始化map 的值，但运行后发现spring报空指针，导致不能生成bean，
而报空指针的行数就是 调用service的地方。之后觉得可能是 执行到static块时，spring还未注入。。。
修改后，运行成功。
