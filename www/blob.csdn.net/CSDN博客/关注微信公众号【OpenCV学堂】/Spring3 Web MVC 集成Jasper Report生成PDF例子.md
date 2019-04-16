# Spring3 Web MVC 集成Jasper Report生成PDF例子 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年01月05日 23:21:55[gloomyfish](https://me.csdn.net/jia20003)阅读数：16916








Spring3 Web MVC 集成JasperReport生成PDF例子

一：环境搭建与配置

1.      安装JDK6以上版本

2.      安装STS, 下载地址如下：[http://www.springsource.org/downloads/sts-ggts](http://www.springsource.org/downloads/sts-ggts)

3.      下载并安装Tomcat7

4.      创建一个Dynamic Web Project项目，然后选择创建好的项目右键选择

Configuration->convert to Manve Project.

5.      添加web.xml文件，在WEB-INF目录下新建reports, pages, classes三个子目录

6.      新建index.jsp文件在webapp目录下。

7.      最终的项目目录结构如下：

![](https://img-my.csdn.net/uploads/201301/05/1357399082_9667.png)




二：Spring配置详解

在web.xml中配置Spring的DispatchServlet与Context Listener，配置如下：

![](https://img-my.csdn.net/uploads/201301/05/1357399152_1171.png)


在express-servlet.xml中配置spring view解析器

![](https://img-my.csdn.net/uploads/201301/05/1357399179_3161.png)


三：Jasper Report配置详解

在jasper-views.xml添加如下配置

![](https://img-my.csdn.net/uploads/201301/05/1357399193_4642.png)


四：Report内容与数据源

两个报表，演示了子报表的用法，同时还演示了如何想子报表传递数据源，以及参

数传递在报表中显示图像等技巧。需要特别说明的是如果要在报表中使用图像路径

图像必须位于WEB-INF/classes下面，因为JasperReport解析是按找类路径寻找。关

于报表的详细内容建议查看下载以后的源文件，此处不再细说。

五：Controller与注解

Spring3 Controller支持注解(annotation)方式，使用非常方便，生成PDF报表的

Controller代码如下：



```java
package com.gloomyfish.express.controllers;

import java.util.HashMap;
import java.util.Map;

import net.sf.jasperreports.engine.JRDataSource;
import net.sf.jasperreports.engine.JREmptyDataSource;

import org.apache.log4j.Logger;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import com.gloomyfish.report.dao.MockDataFactory;

@Controller
public class JasperReportController {
	
	protected static Logger logger = Logger.getLogger("controller");
	
    /**
     * Retrieves the PDF report file
     * 
     * @return
     */
    @RequestMapping(value = "/getpdfReport", method = RequestMethod.GET)
    public ModelAndView doSalesReportPDF(ModelAndView modelAndView) 
		 {
		logger.debug("Received request to download PDF report");
		
		// Retrieve our data from a mock data provider
		MockDataFactory dataprovider = new MockDataFactory();
		
		// Assign the datasource to an instance of JRDataSource
		// JRDataSource is the datasource that Jasper understands
		// This is basically a wrapper to Java's collection classes
		JRDataSource categoryData  = dataprovider.getCategoriesData();

		// parameterMap is the Model of our application
		Map<String,Object> parameterMap = new HashMap<String,Object>();
		
		// must have the empty data source!!!
		JREmptyDataSource emptyData = new JREmptyDataSource();
		parameterMap.put("datasource", emptyData);
		parameterMap.put("JasperfishSubReportDatasource", categoryData);
		parameterMap.put("JasperfishSummaryInfo", dataprovider.getSummaryInfo());
		
		// pdfReport is the View of our application
		// This is declared inside the /WEB-INF/jasper-views.xml
		modelAndView = new ModelAndView("pdfReport", parameterMap);
		
		// Return the View and the Model combined
		return modelAndView;
	}
}
```
Mock数据工厂代码如下：




```java
package com.gloomyfish.report.dao;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import net.sf.jasperreports.engine.JRDataSource;
import net.sf.jasperreports.engine.data.JRBeanCollectionDataSource;

public class MockDataFactory {
	
	public MockDataFactory()
	{
		System.out.println("create mock up data");
	}
	
	public GloomyFishSummaryInfoBean getSummaryInfo()
	{
		GloomyFishSummaryInfoBean summaryBean = new GloomyFishSummaryInfoBean();
		summaryBean.setBlogURL("http://blog.csdn.net/jia20003");
		summaryBean.setMajorDomain("J2SE, J2EE, WEB developer");
		summaryBean.setName("jia20003");
		summaryBean.setNickName("gloomyfish");
		summaryBean.setRegDate("2003-03-02");
		summaryBean.setWorkYears(8);
		return summaryBean;
	}
	
	public JRDataSource getCategoriesData()
	{
		List<ArticlesCategory> listData = new ArrayList<ArticlesCategory>();
		SimpleDateFormat sdf = new SimpleDateFormat("dd MMM yyyy, hh:mm:ss");
		Date createDte = new Date();
		ArticlesCategory category1 = new ArticlesCategory();
		ArticlesCategory category2 = new ArticlesCategory();
		ArticlesCategory category3 = new ArticlesCategory();
		ArticlesCategory category4 = new ArticlesCategory();
		ArticlesCategory category5 = new ArticlesCategory();
		ArticlesCategory category6 = new ArticlesCategory();
//		ArticlesCategory category7 = new ArticlesCategory();
//		ArticlesCategory category8 = new ArticlesCategory();
//		ArticlesCategory category9 = new ArticlesCategory();
//		ArticlesCategory categoryTen = new ArticlesCategory();
		category1.setCategoryName("Android Development");
		category1.setCount(6);
		category1.setCreateDate(sdf.format(createDte));
		category2.setCategoryName("Swing Desktop Development");
		category2.setCount(21);
		category2.setCreateDate(sdf.format(createDte));
		category3.setCategoryName("JAVA 2D Image Process");
		category3.setCount(56);
		category3.setCreateDate(sdf.format(createDte));
		category4.setCategoryName("J2EE");
		category4.setCount(8);
		category4.setCreateDate(sdf.format(createDte));
		category5.setCategoryName("HTML5");
		category5.setCount(4);
		category5.setCreateDate(sdf.format(createDte));
		category6.setCategoryName("Network Protocols Research");
		category6.setCount(4);
		category6.setCreateDate(sdf.format(createDte));
		category6.setCategoryName("ExtJS Learning");
		category6.setCount(2);
		category6.setCreateDate(sdf.format(createDte));
		listData.add(category1);
		listData.add(category2);
		listData.add(category3);
		listData.add(category4);
		listData.add(category5);
		listData.add(category6);
		JRBeanCollectionDataSource data = new JRBeanCollectionDataSource(listData);
		return data;
	}

}
```
启动运行在浏览器中访问地址为：http://localhost:8080/express/hello.html


六：Deploy与运行

全部代码完成以后，从IDE中运行Maven的clean与install命令，得到war包

将war拷贝到tomcat的webapps下面即可启动tomcat然后从浏览器访问。

点击[Get PDF Report]会自动在新窗口中打开生成的PDF报表文件。

程序运行打开主页面结果如下：

![](https://img-my.csdn.net/uploads/201301/05/1357399535_2802.png)


获取PDF报表在浏览器中打开以后效果如下：

![](https://img-my.csdn.net/uploads/201301/05/1357399598_4462.png)


七：常见问题

1.      必须在applicationContext.xml中导入jasper-views.xml资源否则报表不会被编译

          为jasper文件

2.      Web.xml的servlet名必须与spring的xxx-servlet.xml中的xxx一致

3.      Jasper-views.xml中声明的子报表路径参数与数据参数必须与报表文件jrxml中保

         持一致

4.      报表中field变量名必须与Java Class中的field变量名一一对应。



八：项目文件打包下载，解压缩作为Maven项目导入以后运行clean与 install命令。

下载地址：http://download.csdn.net/detail/jia20003/4963552

**转载请注明-2013-01-05**




