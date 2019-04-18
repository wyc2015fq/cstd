# 通过 PDFRenderer 实现预览PDF 功能 - z69183787的专栏 - CSDN博客
2013年10月11日 14:03:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14652
需要一个jar PDFRenderer-0.9.0.jar 
[http://java.net/downloads/pdf-renderer/PDFRenderer-0.9.0.jar](http://java.net/downloads/pdf-renderer/PDFRenderer-0.9.0.jar)
```java
package com.wonders.stpt.attach.action;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Arrays;
import java.util.Comparator;
import javax.imageio.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.Namespace;
import org.apache.struts2.convention.annotation.ParentPackage;
import org.apache.struts2.convention.annotation.Result;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;
import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGImageEncoder;
import com.sun.pdfview.PDFFile;
import com.sun.pdfview.PDFPage;
import com.wonders.stpt.attach.model.vo.UploadFile;
import com.wonders.stpt.attach.service.FjshService;
import com.wonders.stpt.userMsg.action.AbstractParamAction;
@SuppressWarnings("serial")
@ParentPackage("struts-default")
@Namespace(value="/attach")
@Component("attachViewerAction")
@Scope("prototype")
public class AttachViewerAction extends AbstractParamAction{
	
	private FjshService fjshService;
	private final int maxPage = 30; 
	
	public FjshService getFjshService() {
		return fjshService;
	}
	
	@Autowired(required=false)
	public void setFjshService(@Qualifier("fjshService")FjshService fjshService) {
		this.fjshService = fjshService;
	}
	
	/**
	 * PDF文档在线以图片格式预览.
	 * 
	 */
	@Action(value="/pdfPreview",results={@Result(name="pdf",location="/attachPreview/pdfViewer.jsp")})
	public String pdfPreview() {
		//按fileId查找出该文件的路径以及文件名.
		//该部分代码copy自附件上传组件
		HttpServletRequest request = servletRequest;
		HttpServletResponse response = servletResponse;		
		String fileId = request.getParameter("fileId");
		if("".equals(fileId) || null == fileId) {
			servletRequest.setAttribute("state", "f");
			return "pdf";
		}
		
		UploadFile upFile = this.fjshService.loadFileById(fileId);
		if(upFile == null) {
			servletRequest.setAttribute("state", "f");
			return "pdf";
		}
		String path = upFile.getPath();					// 文件所在磁盘路径.
		String fileName = upFile.getFileAllName();		// 真实文件名.
		String saveFileName = upFile.getSaveFileName();	// 磁盘上的文件名.
		String version = upFile.getVersion();
		if ("old".equals(request.getParameter("ver"))){
			if (version != null){
				saveFileName = saveFileName.replace(".dat","_v"+version+".dat");
			}
		}
	
		//当前应用绝对路径
		String appPath =  request.getSession().getServletContext().getRealPath ("");
	    String imageSavePath = appPath + "\\preview_images\\";
	    
		//按照文件路径读取PDF文档，并将其按页转换为图片
		String filePath = path + saveFileName ;		
		if(filePath == null || "".equals(filePath)) {
			servletRequest.setAttribute("state", "f");
			return "pdf";
		}else {
			PDFFile pdfFile = this.getPdfFile(filePath);
			if(this.pdf2Images(pdfFile,imageSavePath,String.valueOf(upFile.getId()))) { //如果转换成功
				return "pdf";
			}else {
				servletRequest.setAttribute("state", "f");
				return "pdf";
			}					
		}				
	}
	
	/**
	 * 图片文件在线预览
	 * 
	 */
	@Action(value="/imagePreview",results={@Result(name="image",location="/attachPreview/imageViewer.jsp")})
	public String imagePreview() {
		//按fileId查找出该文件的路径以及文件名.
		//该部分代码copy自附件上传组件
		HttpServletRequest request = servletRequest;
		HttpServletResponse response = servletResponse;		
		String fileId = request.getParameter("fileId");
		if("".equals(fileId) || null == fileId) {
			servletRequest.setAttribute("state", "f");
			return "image";
		}
		
		UploadFile upFile = this.fjshService.loadFileById(fileId);
		if(upFile == null) {
			servletRequest.setAttribute("state", "f");
			return "image";
		}
		String path = upFile.getPath();					// 文件所在磁盘路径.
		String fileName = upFile.getFileAllName();		// 真实文件名.
		String saveFileName = upFile.getSaveFileName();	// 磁盘上的文件名.
		String version = upFile.getVersion();
		if ("old".equals(request.getParameter("ver"))){
			if (version != null){
				saveFileName = saveFileName.replace(".dat","_v"+version+".dat");
			}
		}
	
		//当前应用绝对路径
		String appPath =  request.getSession().getServletContext().getRealPath ("");
	    String imageSavePath = appPath + "\\preview_images\\";
	    
		//按照文件路径读取文件
		String filePath = path + saveFileName ;
		if(filePath == null || "".equals(filePath)) {
			servletRequest.setAttribute("state", "f");
			return "image";
		}else {
			//如果成功读取文件
			String imageName = String.valueOf(upFile.getId());
			String extName = upFile.getFileExtName();
			if(getImageFile(filePath,imageSavePath,imageName,extName)) {
				return "image";
			}else {
				servletRequest.setAttribute("state", "f");
				return "image";
			}	
		}	
	}
	
	/**
	 * image文件读取. 
	 * @param filePath -- 待读取文件的路径.
	 * @param imageSavePath -- 图片保存路径.
	 * @param imageName -- 图片文件保存后的文件名称（包括后缀）.
	 * @return boolean instance.
	 */
	private boolean getImageFile(String filePath,String imageSavePath,String dirName,String extName) {		
		String path = imageSavePath + dirName + "\\";
		File file = new File(path);
		if(!file.exists()){	//判断以文件名命名的文件夹是否存在.
			file.mkdirs();
		}
		
		try {
			InputStream is = new FileInputStream(filePath);			
			String imagePath = path + dirName + "." + extName;
			FileOutputStream os = new FileOutputStream(imagePath);  // 输出到文件流.
			byte[] buffer = new byte[1024];
			int n = 0;
			while ((n = is.read(buffer, 0, 1024)) > 0) {
				os.write(buffer, 0, n);
			}
			os.close();
			is.close();			
		} catch (Exception ex) {
			ex.printStackTrace();
			return false;
		}	
		
		servletRequest.setAttribute("state", "s");
		servletRequest.setAttribute("dirName", dirName);
		servletRequest.setAttribute("imageName", dirName + "." + extName);
		return true;
	}
	
	/**
	 * PDF文档读取. 
	 * @param filePath -- 待读取PDF文件的路径.
	 * @return null 或者 PDFFile instance.
	 */
	private PDFFile getPdfFile(String filePath) {
		try {
			//load a pdf file from byte buffer.
			File file = new File(filePath);
			RandomAccessFile raf = new RandomAccessFile(file, "r");
			FileChannel channel = raf.getChannel();
			ByteBuffer buf = channel.map(FileChannel.MapMode.READ_ONLY, 0,
					channel.size());
			PDFFile pdfFile = new PDFFile(buf);
			return pdfFile;
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		return null;
	}
	
	/**
	 * PDF文档按页转换为图片.
	 * @param pdfFile -- PDFFile instance
	 * @param imageSavePath -- 图片保存路径.
	 * @param fileName -- 保存图片文件夹名称.
	 */	
	private boolean pdf2Images(PDFFile pdfFile,String imageSavePath,String fileName) {
		if(pdfFile == null ) { //待转换文档不存在，返回false.
			return false;
		}
		
		//将转换后图片存放于path路径下
		String path = imageSavePath + fileName + "\\";
		File filePath = new File(path);
		if(!filePath.exists()){	//判断以文件名命名的文件夹是否存在.
			filePath.mkdirs();
		}
		
		//取得当前文件夹下的所有jpg格式的文件名.
		String[] imageNames = filePath.list(new ImageFilter());		
		if(imageNames.length == 0) {  //当前文件夹下没有文件.
			//将pdf文档按页转为图片.
			String imagePath = "";
			try {
				//对转换页数进行限制,最多只转换前maxPage页.
				int pages = pdfFile.getNumPages();
				if(pages > maxPage){
					pages = maxPage;
				}
				
				for (int i = 1; i <= pages; i++) {
					// draw the page to an image
					PDFPage page = pdfFile.getPage(i);
					// get the width and height for the doc at the default zoom
					Rectangle rect = new Rectangle(0, 
												   0, 
												   (int) page.getBBox().getWidth(), 
												   (int) page.getBBox().getHeight());
					// generate the image
					Image img = page.getImage(rect.width, rect.height, // width & height
											  rect, // clip rect
											  null, // null for the ImageObserver
											  true, // fill background with white
											  true // block until drawing is done
											  );
												
					BufferedImage tag = new BufferedImage(rect.width, 
														  rect.height,
														  BufferedImage.TYPE_INT_RGB);
					
					tag.getGraphics().drawImage(img, 
												0,
												0,
												rect.width,
												rect.height,
												null);
					
					
					imagePath = path + i + ".jpg";
					FileOutputStream out = new FileOutputStream(imagePath);  // 输出到文件流.
					JPEGImageEncoder encoder = JPEGCodec.createJPEGEncoder(out);
					encoder.encode(tag); 		// JPEG编码.
					out.close();
				}				
			}catch (Exception ex) {
				ex.printStackTrace();
				return false;
			}
		}
		
		//取得当前文件夹下的所有jpg格式的文件名.
		imageNames = filePath.list(new ImageFilter());
		//对文件名排序.
		Arrays.sort(imageNames,new FileNameComparator());
		
		servletRequest.setAttribute("state", "s");
		servletRequest.setAttribute("fileName", fileName);
		servletRequest.setAttribute("imageNames", imageNames);
		
		return true;
	}
	
	//图片后缀名过滤类
	
	//图片jpg过滤器类
	class ImageFilter implements FilenameFilter	{
		 public boolean isImageFile(String fileName){
			  if(fileName.toLowerCase().endsWith("jpg")) {
				  return true;
			  }else {
				  return false;
			  }		  
		 }
		 
		 public ImageFilter() {}
		 
		 public boolean accept(File dir,String name){
			 return isImageFile(name);
		  }
	}
	
	//文件名称比较类
	class FileNameComparator implements Comparator {
		public final int compare(Object first, Object second) {
		   String[] fir = ((String)first).split("\\.");
		   String[] sec = ((String)second).split("\\.");
		   
		   int firstPage = Integer.parseInt(fir[0]);
		   int secondPage = Integer.parseInt(sec[0]);
		   int diff = firstPage - secondPage;
		   if (diff > 0)
		    return 1;
		   if (diff < 0)
		    return -1;
		   else
		    return 0;
		}
	}
}
```
