# J2EE中使用MediaInfo库获取视频信息 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月11日 21:50:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：28
个人分类：[MediaInfo																[J2EE](https://blog.csdn.net/leixiaohua1020/article/category/1362940)](https://blog.csdn.net/leixiaohua1020/article/category/1645587)









MediaInfo 用来分析视频和音频文件的编码和内容信息,是一款是自由软件 (免费使用、免费获得源代码）。

我在J2EE项目软件中集成了它的DLL，很好用。

下面简单记录一下它的使用方法。
 （系统开发平台是MyEclipse）

1.需要加入JNA库。加入项目的 Build Path 后就可以使用Java调用DLL了。

2.需要MediaInfo的Dll。可以放到“C:\Windows\System32”里面。

3.需要一个名为MediaInfo的类（在MediaInfoDLL.JNA.java中）。

4.加入以下代码（这是个片段）试一下：



```python
String To_Display = "\r\n\r\nOpen\r\n";
MediaInfo MI = new MediaInfo();
if (MI.Open("test.flv")>0)
	To_Display+="is OK\r\n";
else
	To_Display+="has a problem\r\n";
To_Display += "\r\n\r\nInform with Complete=false\r\n";
MI.Option("Complete", "");
To_Display += MI.Inform();
MI.Close();

System.out.println(To_Display);
```

如果没问题，可以打印出test.flv的完整信息。





实际Struts2 + Spring + Hibernate项目中可以按照如下方式使用：

（一个实际的Action）



```java
package action.video;


import javax.servlet.ServletContext;

import org.apache.struts2.ServletActionContext;

import service.BaseService;

import bean.Video;
import util.MediaInfo;
import com.opensymphony.xwork2.ActionSupport;
/**
 * @author 雷霄骅
 * Action
 */
public class VideoReadByID extends ActionSupport {
	private int videoid;
	private BaseService baseService;
	private String original_videoinfo;
	private String convert_videoinfo;
	private Video video;
	public int getVideoid() {
		return videoid;
	}
	public void setVideoid(int videoid) {
		this.videoid = videoid;
	}
	public BaseService getBaseService() {
		return baseService;
	}
	public void setBaseService(BaseService baseService) {
		this.baseService = baseService;
	}
	
	public Video getVideo() {
		return video;
	}
	public void setVideo(Video video) {
		this.video = video;
	}

	public String getOriginal_videoinfo() {
		return original_videoinfo;
	}
	public void setOriginal_videoinfo(String original_videoinfo) {
		this.original_videoinfo = original_videoinfo;
	}
	public String getConvert_videoinfo() {
		return convert_videoinfo;
	}
	public void setConvert_videoinfo(String convert_videoinfo) {
		this.convert_videoinfo = convert_videoinfo;
	}
	public String execute(){
		try{
			video=(Video) baseService.ReadByID("Video", videoid);
			ServletContext servletContext = ServletActionContext.getServletContext();
			String realfilePath=servletContext.getRealPath("/")+video.getPath();
			String realfileoriginalPath=servletContext.getRealPath("/")+video.getTemppath();
			//MediaInfo-------------------
			MediaInfo MI = new MediaInfo();
			MI.Option("Output", "HTML");
			original_videoinfo = "";
		    if (MI.Open(realfileoriginalPath)>0){
		    	MI.Option("Complete", "");
				original_videoinfo+= MI.Inform();
		    	}
		    	else{
		    	original_videoinfo+="媒体信息检测失败\r\n";
		    	}
			
			//----------------------------
			convert_videoinfo= "";
			if (MI.Open(realfilePath)>0){
				MI.Option("Complete", "");
				convert_videoinfo+= MI.Inform();
			}
		    else{
		    	convert_videoinfo+="媒体信息检测失败\r\n";
		    }
			
			//----------------------------
			//System.out.println(original_videoinfo);
			//System.out.println(convert_videoinfo);
			return SUCCESS;
		}
		catch(Exception ex){
			ex.printStackTrace();
			return ERROR;
		}
	}
}
```


 最终original_videoinfo 和convert_videoinfo中都得到了相应媒体的信息，事实上分别是转码前和转码后视频文件的信息。



在J2EE项目中应该注意：**MI.Option("Output", "HTML");**这句一定要加，这样才能保证original_videoinfo 和convert_videoinfo中的字符串是HTML格式的（以<table>的形式输出）。













