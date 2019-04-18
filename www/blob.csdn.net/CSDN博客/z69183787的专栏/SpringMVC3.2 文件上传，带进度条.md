# SpringMVC3.2 文件上传，带进度条 - z69183787的专栏 - CSDN博客
2014年06月24日 09:52:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5023
一、实现org.apache.commons.fileupload.ProgressListener接口
import javax.servlet.http.HttpSession;
import org.apache.commons.fileupload.ProgressListener;
import org.springframework.stereotype.Component;
import com.chinalife.cip.web.multipart.model.Progress;
@Component
public class FileUploadProgressListener implements ProgressListener {
	private HttpSession session;
	public void setSession(HttpSession session){
		this.session=session;
		Progress status = new Progress();
		session.setAttribute("status", status);
	}
	/*
	 * pBytesRead 到目前为止读取文件的比特数 pContentLength 文件总大小 pItems 目前正在读取第几个文件
	 */
	public void update(long pBytesRead, long pContentLength, int pItems) {
		Progress status = (Progress) session.getAttribute("status");
		status.setpBytesRead(pBytesRead);
		status.setpContentLength(pContentLength);
		status.setpItems(pItems);
	}
}
二、扩展org.springframework.web.multipart.commons.CommonsMultipartResolver类，重写public MultipartParsingResult parseRequest(HttpServletRequest request) throws MultipartException方法
import java.util.List;
import javax.servlet.http.HttpServletRequest;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUpload;
import org.apache.commons.fileupload.FileUploadBase;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.multipart.MaxUploadSizeExceededException;
import org.springframework.web.multipart.MultipartException;
import org.springframework.web.multipart.commons.CommonsMultipartResolver;
import com.chinalife.cip.web.multipart.listener.FileUploadProgressListener;
public class CustomMultipartResolver extends CommonsMultipartResolver {
	@Autowired
	private FileUploadProgressListener progressListener;
	
	public void setFileUploadProgressListener(FileUploadProgressListener progressListener){
		this.progressListener=progressListener;
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public MultipartParsingResult parseRequest(HttpServletRequest request)
			throws MultipartException {
		String encoding = determineEncoding(request);
		FileUpload fileUpload = prepareFileUpload(encoding);
		progressListener.setSession(request.getSession());
		fileUpload.setProgressListener(progressListener);
		try {
			List<FileItem> fileItems = ((ServletFileUpload) fileUpload).parseRequest(request);
			return parseFileItems(fileItems, encoding);
		}
		catch (FileUploadBase.SizeLimitExceededException ex) {
			throw new MaxUploadSizeExceededException(fileUpload.getSizeMax(), ex);
		}
		catch (FileUploadException ex) {
			throw new MultipartException("Could not parse multipart servlet request", ex);
		}
	}
}
三、mvc命名空间声明
<bean id="multipartResolver" class="com.chinalife.cip.web.multipart.CustomMultipartResolver">
	<property name="maxUploadSize" value="50000000" />
</bean>
四、获取文件controller编写
@RequestMapping(...)
public String uploadFile(@RequestParam(value = "file") MultipartFile... files) throws IOException {
	for (MultipartFile f : files) {
		if (f.getSize() > 0) {
			File targetFile = new File("目标文件路径及文件名");
			f.transferTo(targetFile);//写入目标文件
		}
	}
	return "...";
}
五、文件进度controller编写
import java.util.Map;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.SessionAttributes;
import com.chinalife.cip.web.multipart.model.Progress;
@Controller
@SessionAttributes("status")
public class ProgressController {
	@RequestMapping(value = "/upfile/progress", method = RequestMethod.POST )
	@ResponseBody
	public String initCreateInfo(Map<String, Object> model) {
		Progress status = (Progress) model.get("status");
		if(status==null){
			return "{}";
		}
		return status.toString();
	}
}
六、最后
html及js脚本就不在此赘述了，网上有很多例子
