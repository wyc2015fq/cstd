# jQuery 异步上传插件 Uploadify 使用 （Java平台） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月12日 15:00:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：23
个人分类：[HTML和Javascript](https://blog.csdn.net/leixiaohua1020/article/category/1678189)










Uploadify是JQuery的一个上传插件，实现的效果非常不错，带进度显示。而且是Ajax的，省去了自己写Ajax上传功能的麻烦。不过官方提供的实例时php版本的，本文将详细介绍Uploadify在J2EE中的使用。



1.引入头文件（注意一定要把jQuery放在前面）：

注意：uploader是上传的Action路径名；formData是传递给Action的参数列表。



```
<script src="lib/jquery-1.8.3.min.js"></script>
<script src="lib/uploadify/jquery.uploadify.min.js" type="text/javascript"></script>
<script type="text/javascript">
		$(function() {
			$('#projectfile').uploadify({
				'swf'      : 'lib/uploadify/uploadify.swf',
				'uploader' : 'ProjectfileAdd.action',
				'formData': {'projectid': '${project.id}'},
				'fileObjName' : 'file',
				'fileSizeLimit' : '0',
				'onQueueComplete' : function(queueData) {
            		alert(queueData.uploadsSuccessful + ' files were successfully uploaded.');
        			}
				
			});
		});
</script>
```


 2.需要上传的地方添加一个<form>，里面有一个<input>（注意其id属性）：





```
<form>
	<input id="projectfile" name="file" type="file" ></input>
</form>
```


 3.看一下<input>效果如下，还挺美观的



![](https://img-blog.csdn.net/20131012145100578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4.后台的Action就是一个普通的上传文件的Action，示例如下：



```java
package dc.action.projectfile;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.sql.Timestamp;
import java.util.Date;
import java.util.Map;

import org.apache.struts2.ServletActionContext;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import dc.bean.Project;
import dc.bean.Projectfile;
import dc.bean.User;
import dc.service.ProjectService;
import dc.service.ProjectfileService;

public class ProjectfileAdd extends ActionSupport {
	private static final int FILE_SIZE=16*1024;
	private int projectid;
	private ProjectService projectService;
	private ProjectfileService projectfileService;
	private File file;
	private String fileFileName;
	private String fileContentType;
	
	public int getProjectid() {
		return projectid;
	}

	public void setProjectid(int projectid) {
		this.projectid = projectid;
	}

	public ProjectService getProjectService() {
		return projectService;
	}

	public void setProjectService(ProjectService projectService) {
		this.projectService = projectService;
	}

	public ProjectfileService getProjectfileService() {
		return projectfileService;
	}

	public void setProjectfileService(ProjectfileService projectfileService) {
		this.projectfileService = projectfileService;
	}

	public File getFile() {
		return file;
	}

	public void setFile(File file) {
		this.file = file;
	}

	public String getFileFileName() {
		return fileFileName;
	}

	public void setFileFileName(String fileFileName) {
		this.fileFileName = fileFileName;
	}

	public String getFileContentType() {
		return fileContentType;
	}

	public void setFileContentType(String fileContentType) {
		this.fileContentType = fileContentType;
	}

	public static int getFileSize() {
		return FILE_SIZE;
	}

	//写入文件时候用到的函数
	 public void upLoadFile(File source,File target){
		  InputStream in=null;
		  OutputStream out=null;
		  try{
		   in=new BufferedInputStream(new FileInputStream(source),FILE_SIZE);
		   out=new BufferedOutputStream(new FileOutputStream(target),FILE_SIZE);
		   //in-->image-->out
		   byte[] image=new byte[FILE_SIZE];
		   while(in.read(image)>0){
		    out.write(image);
		   }
		  }catch(IOException ex){
		   ex.printStackTrace();
		  }finally{
		   try{
		    in.close();
		    out.close();
		   }catch(IOException ex){
			ex.printStackTrace();
		   }
		  }
		 }
	 
		public String execute() {
			try {
				Projectfile projectfile=new Projectfile();
				projectfile.setName(fileFileName);
				Project project=projectService.ReadByID(projectid);
				projectfile.setProject(project);
				projectfile.setModifytime( new Timestamp(new Date().getTime()));
				String filepath="file/"+project.getId()+"/"+fileFileName;
				projectfile.setFilepath(filepath);
				projectfile.setFilesize(file.length());
				//不是连接到外网的文件
				projectfile.setFilelink(0);
				projectfile.setContenttype(fileContentType);
				//--记录编辑人------------------
				ActionContext context = ActionContext.getContext();
				Map sessionMap = context.getSession();
				User currentuser=(User)sessionMap.get("user");
				projectfile.setUser(currentuser);
				//------------------------------
				//projectfile.setFilesize(1);
				projectfileService.save(projectfile);
				//文件上传，存至硬盘
				//ServletActionContext.getServletContext().getRealPath获得webRoot绝对路径
				String realfilePath=ServletActionContext.getServletContext().getRealPath("file")
				  +"/"+project.getId()+"/"+this.fileFileName;
				File targetFile=new File(realfilePath);
				targetFile.getParentFile().mkdirs();
				upLoadFile(file,targetFile);
			} catch (Exception ex) {
				ex.printStackTrace();
				return ERROR;
			}
			return SUCCESS;
		}
}
```


 Uploadify官方网站：[http://www.uploadify.com/](http://www.uploadify.com/)



Uploadify下载：[http://download.csdn.net/detail/leixiaohua1020/6376463](http://download.csdn.net/detail/leixiaohua1020/6376463)






