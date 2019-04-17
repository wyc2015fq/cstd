# Spring jws 快速发布webservice - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年12月26日 10:22:18[boonya](https://me.csdn.net/boonya)阅读数：8375








### 1、编写需要发布的JavaBean



```java
package com.*.wtms.business.service.ws;

import java.util.Date;
import java.util.List;
import javax.annotation.Resource;
import javax.jws.WebMethod;
import javax.jws.WebService;
import javax.jws.soap.SOAPBinding;
import javax.jws.soap.SOAPBinding.ParameterStyle;
import org.springframework.stereotype.Service;
import com.*.wtms.business.dao.DeepLigthologyDao;
import com.*.wtms.business.dao.DrillingCollectionDao;
import com.*.wtms.business.dao.DrillingTaskDao;
import com.*.wtms.business.dao.QualityCollectionDao;
import com.*.wtms.business.dao.QualityTaskDao;
import com.*.wtms.business.entity.DeepLigthology;
import com.*.wtms.business.entity.DrillingCollection;
import com.*.wtms.business.entity.DrillingTask;
import com.*.wtms.business.entity.QualityCollection;
import com.*.wtms.business.entity.QualityTask;
import com.*.wtms.utils.Log;

@Service
@WebService(serviceName = "mobilewebservice")
@SOAPBinding(parameterStyle=ParameterStyle.WRAPPED)
public class Mobilews
{
	@Resource
	private DrillingCollectionDao drillingCollectionDao;

	@Resource
	private DeepLigthologyDao deepLigthologyDao;

	@Resource
	private QualityCollectionDao qualityCollectionDao;

	@Resource
	private DrillingTaskDao drillingTaskDao;

	@Resource
	private QualityTaskDao qualityTaskDao;

	
	@WebMethod 
	public boolean addDrillCollection(long projectId, long taskId, String measureNo, String wellsNo, String deviceCode, String wellsName, String wellsLocation, double RagEndDeep, int lithology, int deviceType, Date selfCheckDate, double selfCheckDeep, String operator, int note)
	{
		DrillingCollection drillingCollection = new DrillingCollection(projectId, taskId, measureNo, wellsNo, deviceCode, wellsName, wellsLocation, RagEndDeep, lithology, selfCheckDeep, selfCheckDate, operator, note);
		try
		{
			drillingCollectionDao.save(drillingCollection);
			return true;
		} catch (Exception e)
		{
			Log.getLogger(getClass()).error("call webservice to save DrillingCollection error:" + e.getMessage());
		}
		return false;
	}

	
	@WebMethod
	public boolean addDeepLithologyItem(long taskId, double deepFrom, double deepTo, int ligthologyType)
	{
		DeepLigthology deepLigthology = new DeepLigthology(taskId, deepFrom, deepTo, ligthologyType);
		try
		{
			deepLigthologyDao.save(deepLigthology);
			return true;
		} catch (Exception e)
		{
			Log.getLogger(getClass()).error("call webservice to save DeepLigthology error:" + e.getMessage());
		}
		return false;
	}

	
	@WebMethod
	public boolean addQualityCollection(long projectId, long taskId, String measureNo, String wellsNo, String deviceCode, int mechineType, int hasmark, double wellsLocation, double wellsDeep, double waterDeep, int lithology, int measureHandle, int saftyEnviroment, int isOk, String checker, String note)
	{
		QualityCollection qualityCollection = new QualityCollection(projectId, taskId, measureNo, wellsNo, deviceCode, mechineType, hasmark, wellsLocation, wellsDeep, waterDeep, lithology, checker, measureHandle, saftyEnviroment, isOk, 0, note);
		try
		{
			qualityCollectionDao.save(qualityCollection);
			return true;
		} catch (Exception e)
		{
			Log.getLogger(getClass()).error("call webservice to save QualityCollection error:" + e.getMessage());
		}
		return false;
	}

	/**
	 * PDA请求的钻井任务数据列表
	 * 
	 * @param measureNo
	 *            测线编号
	 * @param wellsNo
	 *            井位编号
	 * @param processStatus
	 *            任务进度状态
	 * @return
	 */
	@WebMethod
	public List<DrillingTask> getDrillingTasks(String measureNo, String wellsNo, int processStatus)
	{
		return drillingTaskDao.getDrillingTasks(measureNo, wellsNo, processStatus);
	}

	/**
	 * PDA 请求的质检任务数据列表
	 * 
	 * @param measureNo
	 *            测线编号
	 * @param wellsNo
	 *            井位编号
	 * @param processStatus
	 *            任务进度状态
	 * @return
	 */
	@WebMethod
	public List<QualityTask> getQualityTasks(String measureNo, String wellsNo, int processStatus)
	{
		return qualityTaskDao.getQualityTasks(measureNo, wellsNo, processStatus);
	}

	/*********************************** PDA数据本地化方案 **************************************/

	/**
	 * PDA 请求的质检任务数据列表[本地化数据]
	 * 
	 * @param deviceCode
	 *            设备编码
	 * @return
	 */
	@WebMethod
	public List<DrillingTask> getDrillingTasksByLocal(String deviceCode)
	{
		return drillingTaskDao.getDrillingTasksByLocal(deviceCode);
	}

	/**
	 * PDA 请求的质检任务数据列表[本地化数据]
	 * 
	 * @param deviceCode
	 *            设备编码
	 * @return
	 */
	@WebMethod
	public List<QualityTask> getQualityTasksByLocal(String deviceCode)
	{
		return qualityTaskDao.getQualityTasksByLocal(deviceCode);
	}
}
```



### 2、配置Spring applicationContext.xml



```
<!-- webservice start-->
	<bean class="org.springframework.remoting.jaxws.SimpleJaxWsServiceExporter">
		<property name="baseAddress" value="http://localhost:8088/" />
	</bean>
	
	<bean id="mobilewebservice" class="com.scengine.wtms.business.service.ws.Mobilews"/>  
	
	<!-- webservice   end -->
```



### 3、说明及注意

（1）、通过http://localhost:8080/mobilewebservice?wsdl访问webservice部署描述符，还有自动生成的xsd:http://localhost:8080/mobilewebservice?xsd=1
 。
（2）、@SOAPBinding(parameterStyle=ParameterStyle.WRAPPED)必须添加，否则会报错；另外，如果发布的方法只有一个参数可以使用@SOAPBinding(parameterStyle=ParameterStyle.BARE)。


（3）、@WebService(serviceName = "mobilewebservice")
  服务名称与Spring配置的bean一致。

参考地址：http://wodar.iteye.com/blog/230162，http://m.blog.csdn.net/blog/jadyer/9002553

(4)、webservice的端口设置不要与服务器一样，这一点非常重要否则服务器应用与webservice服务冲突会产生HTTP404错误。

### 4、结果截图

服务地址：

![](https://img-blog.csdn.net/20131226172205640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


wsdl地址：

![](https://img-blog.csdn.net/20131226172214000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




