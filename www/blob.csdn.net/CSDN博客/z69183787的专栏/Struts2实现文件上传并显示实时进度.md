# Struts2实现文件上传并显示实时进度 - z69183787的专栏 - CSDN博客
2016年09月14日 11:27:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3447
基于浏览器的文件上传，特别是对于通过<input type="file">标签来实现上传的情况， 存在着严重的性能问题，因为用户提交了文件之后，在浏览器把文件上传到服务器的过程中，界面看上去似乎是静止的，如果是小文件还好些，如果不幸需要上传的 是几兆、几十兆甚至上百兆的文件，我相信那是一种非常痛苦的体验，我们中间的很多人应该都有过此种不堪的经历。
      我们为了改善用户界面，通常会在处理量大或者是网络速度较慢的时候，给用户显示一个处理进度，让用户心理有底，增强用户等待结果的耐心，以改善用户体验。
现在我就针对这个问题给出一个解决方案，我们将实现一个具有监控能力的WEB上传的程序——它不仅把文件上传到服务器，而且"实时地"监视文件上传的实际过程。
解决方案的基本思路是这样的：
- 在Form提交上传文件同时，使用AJAX周期性地从Action轮询上传状态信息
- 然后，根据此信息更新进度条和相关文字，及时反映文件传输状态
实现一个文件监听类，实现对文件上传进度的实时监听，并将监听结果存放到session中，公前台界面读取。
```java
/*
 * Copyright (c) 2012-2032 Accounting Center of China Aviation(ACCA).
 * All Rights Reserved.
 */
package com.wallet.myWallet.listener;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import org.apache.commons.fileupload.ProgressListener;
import com.wallet.myWallet.entity.State;
/**
 * 
 * 
 * @author zhouhua, 2014-7-16
 */
public class FileUploadListener implements ProgressListener {
  // 声明一个HttpSession，目的是把State对象放到这个HttpSession中
  private HttpSession session;
  // 此构造函数由MyJakartaMultiPartRequest.java类parseRequest()方法调用
  public FileUploadListener(HttpServletRequest request) {
    super();
    session = request.getSession();
  }
  public void update(long uploadByte, long fileSizeByte, int fileIndex) {
    if (fileSizeByte == -1) {
      // 如果上传的大小为-1则上传已经完成
      System.out.println("上传文件结束！");
    } else {
      if (session.getAttribute("uploadState") == null) {
        // 如果为空就new一个State对象并设置里面的文本内容
        State state = new State();
        state.setState(uploadByte, fileSizeByte, (fileIndex - 1));
        session.setAttribute("uploadState", state);
      } else {
        // 如果session中有uploadState对象就取出来，然后设置里面文本内容
        State state = (State) session.getAttribute("uploadState");
        state.setState(uploadByte, fileSizeByte, (fileIndex - 1));
      }
    }
  }
}
```
接下来是一个文件状态类：
```java
/*
 * Copyright (c) 2012-2032 Accounting Center of China Aviation(ACCA).
 * All Rights Reserved.
 */
package com.wallet.myWallet.entity;
import java.text.NumberFormat;
import java.text.SimpleDateFormat;
/**
 * 
 * 
 * @author zhouhua, 2014-7-16
 */
public class State {
  private long uploadByte; // 已经上传的字节数，单位：字节
  private long fileSizeByte; // 所有文件的总长度，单位：字节
  private int fileIndex; // 正在上传第几个文件
  private long startTime; // 开始上传的时间，用于计算上传速度等
  private int percent; // 上传百分比
  private long speed;
  private long time; 
  private static final SimpleDateFormat SIMPLEFORMAT = new SimpleDateFormat("HH:mm:ss");
  public State() {
    startTime = System.currentTimeMillis();
    percent = 0;
    speed=0L;
  }
  // 从State状态类中取得状态的字符串，用字符串的形式拼成XML文件内容
  public synchronized String getStateString() {
    StringBuilder sb = new StringBuilder("<info>");
    sb.append("<uploadByte>" + NumberFormat.getInstance().format(uploadByte/(1024*1024))
 + "</uploadByte>");
   sb.append("<fileSizeByte>" + NumberFormat.getInstance().format(fileSizeByte/(1024*1024))
        + "</fileSizeByte>");
   sb.append("<speed>" + NumberFormat.getInstance().format((speed/(1024*1024))/time)
  + "</speed>");
    sb.append("<fileIndex>" + fileIndex + "</fileIndex>");
    sb.append("<percent>" + percent + "</percent>");
    sb.append("<startTime>" + SIMPLEFORMAT.format(startTime) + "</startTime>");
    sb.append("</info>");
    return sb.toString();
  }
  public synchronized void setState(long uploadByte, long fileSizeByte, int fileIndex) {
    this.uploadByte = uploadByte;
    this.fileSizeByte = fileSizeByte;
    this.fileIndex = fileIndex;
    this.speed=uploadByte-speed;
    this.time=(System.currentTimeMillis()-startTime)/1000;
    if ((Long.valueOf(uploadByte) * 100 / Long.valueOf(fileSizeByte) <= 100)) {
      // 生成当前上传进度的公式，加入判断条件的含义在于不需要重复计算
      percent = (int) (Long.valueOf(uploadByte) * 100 / Long.valueOf(fileSizeByte));
    }
  }
}
```
如果想通过Struts2监听文件上传的进度，我们需要自己实现Struts2的MultiPartRequest类并将自己的文件上传监听类注入，实现类如下：
```java
/*
 * Copyright (c) 2012-2032 Accounting Center of China Aviation(ACCA).
 * All Rights Reserved.
 */
package com.wallet.myWallet.listener;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import javax.servlet.http.HttpServletRequest;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUploadBase;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.RequestContext;
import org.apache.commons.fileupload.disk.DiskFileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.struts2.StrutsConstants;
import org.apache.struts2.dispatcher.multipart.MultiPartRequest;
import com.opensymphony.xwork2.LocaleProvider;
import com.opensymphony.xwork2.inject.Inject;
import com.opensymphony.xwork2.util.LocalizedTextUtil;
import com.opensymphony.xwork2.util.logging.Logger;
import com.opensymphony.xwork2.util.logging.LoggerFactory;
/**
 * 
 * 
 * @author zhouhua, 2014-7-16
 */
public class MyJakartaMultiPartRequest implements MultiPartRequest {
  static final Logger LOG = LoggerFactory.getLogger(MyJakartaMultiPartRequest.class);
  // maps parameter name -> List of FileItem objects
  protected Map<String, List<FileItem>> files = new HashMap<String, List<FileItem>>();
  // maps parameter name -> List of param values
  protected Map<String, List<String>> params = new HashMap<String, List<String>>();
  // any errors while processing this request
  protected List<String> errors = new ArrayList<String>();
  protected long maxSize;
  private Locale defaultLocale = Locale.ENGLISH;
  @Inject(StrutsConstants.STRUTS_MULTIPART_MAXSIZE)
  public void setMaxSize(String maxSize) {
    this.maxSize = Long.parseLong(maxSize);
  }
  @Inject
  public void setLocaleProvider(LocaleProvider provider) {
    defaultLocale = provider.getLocale();
  }
  /**
   * Creates a new request wrapper to handle multi-part data using methods adapted
   * from Jason
   * Pell's multipart classes (see class description).
   * 
   * @param saveDir the directory to save off the file
   * @param request the request containing the multipart
   * @throws java.io.IOException is thrown if encoding fails.
   */
  public void parse(HttpServletRequest request, String saveDir) throws IOException {
    try {
      setLocale(request);
      processUpload(request, saveDir);
    } catch (FileUploadBase.SizeLimitExceededException e) {
      if (LOG.isWarnEnabled()) {
        LOG.warn("Request exceeded size limit!", e);
      }
      String errorMessage = buildErrorMessage(e,
          new Object[] { e.getPermittedSize(), e.getActualSize() });
      if (!errors.contains(errorMessage)) {
        errors.add(errorMessage);
      }
    } catch (Exception e) {
      if (LOG.isWarnEnabled()) {
        LOG.warn("Unable to parse request", e);
      }
      String errorMessage = buildErrorMessage(e, new Object[] {});
      if (!errors.contains(errorMessage)) {
        errors.add(errorMessage);
      }
    }
  }
  protected void setLocale(HttpServletRequest request) {
    if (defaultLocale == null) {
      defaultLocale = request.getLocale();
    }
  }
  protected String buildErrorMessage(Throwable e, Object[] args) {
    String errorKey = "struts.messages.upload.error." + e.getClass().getSimpleName();
    if (LOG.isDebugEnabled()) {
      LOG.debug("Preparing error message for key: [#0]", errorKey);
    }
    return LocalizedTextUtil.findText(this.getClass(), errorKey, defaultLocale,
     e.getMessage(),args);
  }
  private void processUpload(HttpServletRequest request, String saveDir)
    throws FileUploadException, UnsupportedEncodingException {
    for (FileItem item : parseRequest(request, saveDir)) {
      if (LOG.isDebugEnabled()) {
        LOG.debug("Found item " + item.getFieldName());
      }
      if (item.isFormField()) {
        processNormalFormField(item, request.getCharacterEncoding());
      } else {
        processFileField(item);
      }
    }
  }
  private void processFileField(FileItem item) {
    if (LOG.isDebugEnabled()) {
      LOG.debug("Item is a file upload");
    }
    // Skip file uploads that don't have a file name - meaning that no file was selected.
    if (item.getName() == null || item.getName().trim().length() < 1) {
      LOG.debug("No file has been uploaded for the field: " + item.getFieldName());
      return;
    }
    List<FileItem> values;
    if (files.get(item.getFieldName()) != null) {
      values = files.get(item.getFieldName());
    } else {
      values = new ArrayList<FileItem>();
    }
    values.add(item);
    files.put(item.getFieldName(), values);
  }
  private void processNormalFormField(FileItem item, String charset)
    throws UnsupportedEncodingException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("Item is a normal form field");
    }
    List<String> values;
    if (params.get(item.getFieldName()) != null) {
      values = params.get(item.getFieldName());
    } else {
      values = new ArrayList<String>();
    }
    // note: see http://jira.opensymphony.com/browse/WW-633
    // basically, in some cases the charset may be null, so
    // we're just going to try to "other" method (no idea if this
    // will work)
    if (charset != null) {
      values.add(item.getString(charset));
    } else {
      values.add(item.getString());
    }
    params.put(item.getFieldName(), values);
    item.delete();
  }
private List<FileItem> parseRequest(HttpServletRequest servletRequest, String saveDir)
    throws FileUploadException {
    DiskFileItemFactory fac = createDiskFileItemFactory(saveDir);
    ServletFileUpload upload = new ServletFileUpload(fac);
    // 设置上传进度的监听
    upload.setProgressListener(new FileUploadListener(servletRequest));
    upload.setSizeMax(maxSize);
    return upload.parseRequest(createRequestContext(servletRequest));
  }
  private DiskFileItemFactory createDiskFileItemFactory(String saveDir) {
    DiskFileItemFactory fac = new DiskFileItemFactory();
    // Make sure that the data is written to file
    fac.setSizeThreshold(0);
    if (saveDir != null) {
      fac.setRepository(new File(saveDir));
    }
    return fac;
  }
  /*
   * (non-Javadoc)
   * 
   * @see org.apache.struts2.dispatcher.multipart.MultiPartRequest#getFileParameterNames()
   */
  public Enumeration<String> getFileParameterNames() {
    return Collections.enumeration(files.keySet());
  }
  /*
   * (non-Javadoc)
   * 
   * @see
   * 
   */
  public String[] getContentType(String fieldName) {
    List<FileItem> items = files.get(fieldName);
    if (items == null) {
      return null;
    }
    List<String> contentTypes = new ArrayList<String>(items.size());
    for (FileItem fileItem : items) {
      contentTypes.add(fileItem.getContentType());
    }
    return contentTypes.toArray(new String[contentTypes.size()]);
  }
  /*
   * (non-Javadoc)
   * 
   * 
   */
  public File[] getFile(String fieldName) {
    List<FileItem> items = files.get(fieldName);
    if (items == null) {
      return null;
    }
    List<File> fileList = new ArrayList<File>(items.size());
    for (FileItem fileItem : items) {
      File storeLocation = ((DiskFileItem) fileItem).getStoreLocation();
      if (fileItem.isInMemory() && storeLocation != null && !storeLocation.exists()) {
        try {
          storeLocation.createNewFile();
        } catch (IOException e) {
          if (LOG.isErrorEnabled()) {
            LOG.error(
                "Cannot write uploaded empty file to disk: "
                    + storeLocation.getAbsolutePath(), e);
          }
        }
      }
      fileList.add(storeLocation);
    }
    return fileList.toArray(new File[fileList.size()]);
  }
  public String[] getFileNames(String fieldName) {
    List<FileItem> items = files.get(fieldName);
    if (items == null) {
      return null;
    }
    List<String> fileNames = new ArrayList<String>(items.size());
    for (FileItem fileItem : items) {
      fileNames.add(getCanonicalName(fileItem.getName()));
    }
    return fileNames.toArray(new String[fileNames.size()]);
  }
  public String[] getFilesystemName(String fieldName) {
    List<FileItem> items = files.get(fieldName);
    if (items == null) {
      return null;
    }
    List<String> fileNames = new ArrayList<String>(items.size());
    for (FileItem fileItem : items) {
      fileNames.add(((DiskFileItem) fileItem).getStoreLocation().getName());
    }
    return fileNames.toArray(new String[fileNames.size()]);
  }
  public String getParameter(String name) {
    List<String> v = params.get(name);
    if (v != null && v.size() > 0) {
      return v.get(0);
    }
    return null;
  }
  /*
   * (non-Javadoc)
   * 
   * @see org.apache.struts2.dispatcher.multipart.MultiPartRequest#getParameterNames()
   */
  public Enumeration<String> getParameterNames() {
    return Collections.enumeration(params.keySet());
  }
  public String[] getParameterValues(String name) {
    List<String> v = params.get(name);
    if (v != null && v.size() > 0) {
      return v.toArray(new String[v.size()]);
    }
    return null;
  }
  /*
   * (non-Javadoc)
   * 
   * @see org.apache.struts2.dispatcher.multipart.MultiPartRequest#getErrors()
   */
  public List<String> getErrors() {
    return errors;
  }
  /**
   * Returns the canonical name of the given file.
   * 
   * @param filename the given file
   * @return the canonical name of the given file
   */
  private String getCanonicalName(String filename) {
    int forwardSlash = filename.lastIndexOf("/");
    int backwardSlash = filename.lastIndexOf("\\");
    if (forwardSlash != -1 && forwardSlash > backwardSlash) {
      filename = filename.substring(forwardSlash + 1, filename.length());
    } else if (backwardSlash != -1 && backwardSlash >= forwardSlash) {
      filename = filename.substring(backwardSlash + 1, filename.length());
    }
    return filename;
  }
  /**
   * Creates a RequestContext needed by Jakarta Commons Upload.
   * 
   * @param req the request.
   * @return a new request context.
   */
  private RequestContext createRequestContext(final HttpServletRequest req) {
    return new RequestContext() {
      public String getCharacterEncoding() {
        return req.getCharacterEncoding();
      }
      public String getContentType() {
        return req.getContentType();
      }
      public int getContentLength() {
        return req.getContentLength();
      }
      public InputStream getInputStream() throws IOException {
        InputStream in = req.getInputStream();
        if (in == null) {
          throw new IOException("Missing content in the request");
        }
        return req.getInputStream();
      }
    };
  }
  /*
   * (non-Javadoc)
   * 
   * @see org.apache.struts2.dispatcher.multipart.MultiPartRequest#cleanUp()
   */
  public void cleanUp() {
    Set<String> names = files.keySet();
    for (String name : names) {
      List<FileItem> items = files.get(name);
      for (FileItem item : items) {
        if (LOG.isDebugEnabled()) {
          String msg = LocalizedTextUtil.findText(this.getClass(),
             "struts.messages.removing.file", Locale.ENGLISH, "no.message.found",
              new Object[] { name, item });
          LOG.debug(msg);
        }
        if (!item.isInMemory()) {
          item.delete();
        }
      }
    }
  }
}
```
自己的类实现MultiPartRequest后,需要在Struts.xml文件中进行装配：
<!--demo12，struts2文件上传与下载 --><beantype="org.apache.struts2.dispatcher.multipart.MultiPartRequest"name="parser"class="com.wallet.myWallet.listener.MyJakartaMultiPartRequest"scope="default" /><!--demo12，struts2文件上传与下载 --><constantname="struts.multipart.parser"value="parser" />
注（struts2默认 文件处理）
Struts2本身并没提供上传的组件，我们可以通过调用上传框架来实现文件的上传。
一、配置上传解析器
首先要配置项目的框架，也就是倒导入"struts2-core-2.2.1.jar"库文件，找到org.apache.struts2包下的default.porperties资源文件。如下图；资源文件中给出了不同的strus2的默认配置，我们可看到struts2默认是jakarta作为其文件上传的解析器。
![](http://dl.iteye.com/upload/attachment/361444/1c1799c7-7bcf-38fb-bab3-990437743589.jpg)jakarta是Commo-FileUpload的框架。如果要使用Commo-FileUpload框架来上传文件，只需将"commons-fileupload-1.2.1.jar"和"commons-io-1.3.2.jar"两个jar复制到项目中的WEB-INF/lib目录下就可。
如果想要使用COS框架来上传文件，只需将“cos.jar”复制到项目中就可以，然后在修改struts.multipart.parser常量值。
修改常量值有两种方法，一是在"struts.xml"中修改，代码如下：
<constant name="struts.multipart.paeser" value="cos"></constant>
二是在struts.properties中修改,代码如下：
sruts.multipart.parser=cos
前面的工作做完后就需要写两个Action，一个用于文件上传，另一个用于读取文件上传进度：
文件上传：
```java
/*
 * Copyright (c) 2012-2032 Accounting Center of China Aviation(ACCA).
 * All Rights Reserved.
 */
package com.wallet.myWallet.action;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Controller;
import com.wallet.core.action.BaseAction;
import com.wallet.myWallet.entity.DataImportStatus;
import com.wallet.myWallet.entity.FileUploadTools;
/**
 * 
 * 
 * @author zhouhua, 2014-7-15
 */
@Controller
@Scope("prototype")
public class FileUploadAction extends BaseAction<Object> {
    /**  */
    private static final long serialVersionUID = -2091410772080750644L;
    // 声明封装了File上传的FileUploadTools类的实例
    // FileUploadTools类也封装了上传的属性及get和set方法
    private FileUploadTools fileUploadTools = new FileUploadTools();
    private Map jsonMap = new HashMap();
    public Map getJsonMap() {
        return jsonMap;
    }
    public void setJsonMap(Map jsonMap) {
        this.jsonMap = jsonMap;
    }
    public FileUploadTools getFileUploadTools() {
        return fileUploadTools;
    }
    public void setFileUploadTools(FileUploadTools fileUploadTools) {
        this.fileUploadTools = fileUploadTools;
    }
    /**
     * 处理文件上传
     * 
     * @return
     * @throws IOException
     * @throws InterruptedException
     */
    public String upload() throws IOException, InterruptedException {
        // 文件上传
        fileUploadTools.beginUpload();
        getSession().setAttribute("uploadState", null);
        jsonMap.put("flg", true);
        return "upload";
    }
}
```
```java
/*
 * Copyright (c) 2012-2032 Accounting Center of China Aviation(ACCA).
 * All Rights Reserved.
 */
package com.wallet.myWallet.entity;
import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;
/**
 * 
 * @author zhouhua, 2014-7-16
 */
public class FileUploadTools {
  private String username;
  private File uploadFile[];// 上传的文件是数组类型
  private String uploadFileFileName[];// 文件名是数组类型
  private String uploadFileContentType[];
  public String beginUpload() throws IOException {
    System.out.println("用户名：" + username);
    String targetDirectory = "C:/demo_upload/";
    if (uploadFile != null && uploadFile.length > 0) {
      for (int i = 0; i < uploadFile.length; i++) {
        File target = new File(targetDirectory, uploadFileFileName[i]);
        FileUtils.copyFile(uploadFile[i], target);
      }
    }
    return "success";
  }
  public String getUsername() {
    return username;
  }
  public void setUsername(String username) {
    this.username = username;
  }
  public File[] getUploadFile() {
    return uploadFile;
  }
  public void setUploadFile(File[] uploadFile) {
    this.uploadFile = uploadFile;
  }
  public String[] getUploadFileFileName() {
    return uploadFileFileName;
  }
  public void setUploadFileFileName(String[] uploadFileFileName) {
    this.uploadFileFileName = uploadFileFileName;
  }
  public String[] getUploadFileContentType() {
    return uploadFileContentType;
  }
  public void setUploadFileContentType(String[] uploadFileContentType) {
    this.uploadFileContentType = uploadFileContentType;
  }
}
```
用于读取文件上传进度：
```java
/*
 * Copyright (c) 2012-2032 Accounting Center of China Aviation(ACCA).
 * All Rights Reserved.
 */
package com.wallet.myWallet.action;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import org.apache.struts2.ServletActionContext;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Controller;
import com.wallet.core.action.BaseAction;
import com.wallet.myWallet.entity.State;
/**
 * 
 * 
 * @author zhouhua, 2014-7-16
 */
@Controller
@Scope("prototype")
public class StateAction extends BaseAction<Object> {
    /**  */
    private static final long serialVersionUID = 1L;
    public String execute() throws IOException {
        // 从session中取得名称为uploadState的State对象
        State tempState = (State) getSession().getAttribute("uploadState");
        if (tempState != null) {
            // 设置编码为utf-8
            ServletActionContext.getResponse().setCharacterEncoding("utf-8");
            // 设置响应的格式为XML
            ServletActionContext.getResponse().setContentType("text/xml");
            // 用out对象输出xml代码头
            ServletActionContext.getResponse().getWriter()
                    .print("<?xml version='1.0' encoding='" + "utf-8" + "' ?>");
            // 用out对象输出xml代码体
            ServletActionContext.getResponse().getWriter().print(tempState.getStateString());
        }
        return null;
    }
}
```
```java
<%@ page language="java" contentType="text/html; charset=UTF-8"
  pageEncoding="UTF-8"%>
<%@ page isELIgnored="false"%>
<%@ taglib uri="/struts-tags" prefix="s"%>
<%
    String path = request.getContextPath();
    String basePath = request.getScheme() + "://" + request.getServerName() + ":"
            + request.getServerPort() + path + "/";
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
 "http://www.w3.org/TR/html4/loose.dtd">
<html>
<base href="<%=basePath%>">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>多文件上传，显示进度条实例</title>
<style type="text/css">
<!--
body,td,th {
  font-size: 9pt;
}
-->
</style>
<!--参考：http://api.jqueryui.com/progressbar/-->
<link rel="stylesheet"
  href="./js/jqueryUI/themes/base/jquery.ui.all.css">
<script src="./js/jquery-1.10.2.js"></script>
<script src="./js/jqueryUI/ui/jquery.ui.core.js"></script>
<script src="./js/jqueryUI/ui/jquery.ui.widget.js"></script>
<script src="./js/jqueryUI/ui/jquery.ui.progressbar.js"></script>
<link rel="stylesheet" href="./css/demos.css">
<script src="./js/fileUpload.js"></script>
</head>
<body>
  <br />
  <form action="fileUploadAction!upload.action" method="post"
    enctype="multipart/form-data" onsubmit="return submitForm()">
    <table width="818" border="1">
      <tr>
        <td width="176">
          <div align="center">用户账号</div>
        </td>
        <td width="626"><input type="text"
          name="fileUploadTools.username" /></td>
      </tr>
      <tr>
        <td>
          <div align="center">
                 用户附件 <br /> <a href="javascript:insertFile()">添加附件</a>
          </div>
        </td>
        <td id="fileForm"><br /></td>
      </tr>
    </table>
    <input type="submit" value="开始上传..." />
    <form>
      <br />
      <div id="progressbar" style="width: 500"></div>
      <br />
      <div id="progressDetail" class="demo-description"
        >
      <p>进度详细信息显示于此......</p>
      </div>
</body>
</html>
```
```java
// 下面这三个函数是生成与刷新进度条、进度详细信息的
// 初始化进度条
$(function() {
  $("#progressbar").progressbar({
    value : 0
  });
});
// 调用查询进度信息接口
function refreshProcessBar() {
  
  $.ajax({
    url:'stateAction.action',
    data:{'timestamp': new Date().getTime()},
    type:'get',
    dataType:'xml',
    success:function(data){
      var flg=refreshProcessBarCallBack(data)	
    }
  });
}
// 查询进度信息接口回调函数
function refreshProcessBarCallBack(returnXMLParam) {
  var returnXML = returnXMLParam;
  var percent = $(returnXML).find('percent').text()
  var showText = "完成：" + percent + "%";
  showText = showText + "\n已读取"
      + $(returnXML).find('uploadByte').text()+"MB";
  showText = showText + "\n文件总大小："
      + $(returnXML).find('fileSizeByte').text()+"MB";
  showText = showText + "\n上传速率："
  + $(returnXML).find('speed').text()+"MB/S";
  showText = showText + "\n当前上传文件为第：" + $(returnXML).find('fileIndex').text()
      + "个";
  showText = showText + "\n开始上传时间：" + $(returnXML).find('startTime').text();
  // 刷新进度详细信息
  $('#progressDetail').empty();
  $('#progressDetail').text(showText);
  // 刷新进度条
  $("#progressbar").progressbar("option", "value", parseInt(percent));
  setTimeout("refreshProcessBar()", 1000);
  if(percent==100){
    return true;
  }else{
    return false;
  }
}
// 下面这三个函数是控制添加、删除、修改附件的（允许增加、删除附件，只允许指定后缀的文件被选择等）
var a = 0;
function file_change() {
  // 当文本域中的值改变时触发此方法
  var postfix = this.value.substring(this.value.lastIndexOf(".") + 1)
      .toUpperCase();
  // 判断扩展是否合法
  if (postfix == "JPG" || postfix == "GIF" || postfix == "PNG"
      || postfix == "BMP" || postfix == "RAR" || postfix == "ZIP"
      || postfix == "TXT" || postfix == "GHO" || postfix == "PDF") {
  } else {
    // 如果不合法就删除相应的File表单及br标签
    alert("您上传的文件类型不被支持，本系统只支持JPG,GIF,PNG,BMP,RAR,ZIP,TXT文件！");
    var testtest = $(this).attr('id');
    testtest = '#' + testtest;
    var sub_file = $(testtest);
    var next_a_ele = sub_file.next();// 取得a标记
    var br1_ele = $(next_a_ele).next();// 取得回车
    var br2_ele = $(br1_ele).next();// 取得回车
    $(br2_ele).remove();// 删除回车
    $(br1_ele).remove();// 删除回车
    $(next_a_ele).remove();// 删除a标签
    $(sub_file).remove();
    // 删除文本域，因为上传的文件类型出错，要删除动态创建的File表单
    return;
  }
}
function remove_file() {// 删除File表单域的方法
// 删除表单
  var testtest = $(this).val();
  testtest = '#' + testtest;
  var sub_file = $(testtest);
  var next_a_ele = sub_file.next();// 取得a标记
  var br1_ele = $(next_a_ele).next();// 取得回车
  var br2_ele = $(br1_ele).next();// 取得回车
  $(br2_ele).remove();// 删除回车
  $(br1_ele).remove();// 删除回车
  $(next_a_ele).remove();// 删除a标签
  $(sub_file).remove();// 删除File标记
}
function f() {
  // 方法名为f的主要作用是不允许在File表单域中手动输入文件名，必须单击“浏览”按钮
  return false;
}
function insertFile() {
  // 新建File表单
  var file_array = document.getElementsByTagName("input");
  var is_null = false;
  // 循环遍历判断是否有某一个File表单域的值为空
  for ( var i = 0; i < file_array.length; i++) {
    if (file_array[i].type == "file"
        && file_array[i].name.substring(0, 15) == "fileUploadTools") {
      if (file_array[i].value == "") {
        alert("某一附件为空不能继续添加");
        is_null = true;
        break;
      }
    }
  }
  if (is_null) {
    return;
  }
  a++;
  // 新建file表单的基本信息
  var new_File_element = $('<input>');
  new_File_element.attr('type', 'file');
  new_File_element.attr('id', 'uploadFile' + a);
  new_File_element.attr('name', 'fileUploadTools.uploadFile');
  new_File_element.attr('size', 55);
  new_File_element.keydown(f);
  new_File_element.change(file_change);
  $('#fileForm').append(new_File_element);
  // 新建删除附件的a标签的基本信息
  var new_a_element = $('<a>');
  new_a_element.html("删除附件");
  new_a_element.attr('id', "a_" + new_File_element.name);
  new_a_element.attr('name', "a_" + new_File_element.name);
  new_a_element.val($(new_File_element).attr('id'));
  new_a_element.attr('href', "#");
  new_a_element.click(remove_file);
  $('#fileForm').append(new_a_element);
  var new_br_element = $("<br>");
  $('#fileForm').append(new_br_element);
  var new_br_element = $("<br>");
  $('#fileForm').append(new_br_element);
}
// 提交表单，提交时触发刷新进度条函数
function submitForm() {
  setTimeout("refreshProcessBar()", 1000);
  return true;
}
```
   Iteye的排版有问题，可能导致看的不是很清楚，如果此功能正是你需要的话，可以参看本人的CSDN博客：
http://blog.csdn.net/zhouhua0104/article/details/37922429
![](http://img1.tuicool.com/muuEzeB.png!web)
![](http://img0.tuicool.com/nyAjue.png!web)
![](http://img2.tuicool.com/bEnaQbZ.png!web)
最近在做一个资源共享的项目中，采用了Struts2.1.8+Spring2.5.6+hibernate3.32的框架整合方式进行开发。在文件上传这块，因为需要实现文件上传时显示进度条的功能，所以尝试了一下。怕以后忘记，先贴出来分享下。
 要在上传文件时能显示进度条，首先需要实时的获知web服务端接收了多少字节，以及文件总大小，这里我们在页面上使用AJAX技术每一秒向服务器发送一次请求来获得需要的实时上传信息。但是当我们使用struts2后怎么在服务端获得实时的上传大小呢？这里需要用到commons-fileupload中的progressListener接口,实现这个接口，然后再实现一个自己的解析器，并在解析器中添加自己实现的那个progressListener;然后再替换struts2自带的解析器（struts2自带的解析器类没有添加progressListener），然后就可以了。下面看看主要的代码(技术有限，如有不对之处，望不吝点解):
监听器：
**import** javax.servlet.http.HttpServletRequest;
**import** javax.servlet.http.HttpSession;
**import** org.apache.commons.fileupload.ProgressListener;
**public class** ResourceProgressListener**implements** ProgressListener {
**private** HttpSession session;
**public** ResourceProgressListener(HttpServletRequest request) {
 session = request.getSession();
 ResourceFileUploadStatus newUploadStatus = **new** ResourceFileUploadStatus();
 session.setAttribute("currentUploadStatus", newUploadStatus);
 }
**public void** update(**long** readedBytes, **long** totalBytes, **int** currentItem) {
 ResourceFileUploadStatus status = (ResourceFileUploadStatus) session.getAttribute("currentUploadStatus");
 status.setReadedBytes(readedBytes);
 status.setTotalBytes(totalBytes);
 status.setCurrentItem(currentItem);
 }
}
上传状态类：
**public class** ResourceFileUploadStatus {
**private long** readedBytes = 0L;
**private long** totalBytes = 0L;
**private int** currentItem = 0;
**public long** getReadedBytes() {
**return** readedBytes;
 }
**public void** setReadedBytes(**long** bytes) {
 readedBytes = bytes;
 }
**public long** getTotalBytes() {
**return** totalBytes;
 }
**public void** setTotalBytes(**long** bytes) {
 totalBytes = bytes;
 }
**public int** getCurrentItem() {
**return** currentItem;
 }
**public void** setCurrentItem(**int** item) {
 currentItem = item;
 }
}
实现自己的解析器类：方法比较简单，找到struts2实现的解析器类，把代码拷贝过来然后添加上监听器即可。这个类代码较多就不整个文件拷了，主要是在parse方法里添加。Parse方法代码如下：红色标注部分即是需要自己添加的progressListener.
**public void** parse(HttpServletRequest servletRequest, String saveDir)
**throws** IOException {
 System.*out*.println("执行自定义MultiPartRequest");
 DiskFileItemFactory fac = **new** DiskFileItemFactory();
 // Make sure that the data is written to file
 fac.setSizeThreshold(0);
**if** (saveDir != **null**) {
 fac.setRepository(**new** File(saveDir));
 }
 // Parse the request
**try** {
 ServletFileUpload upload = **new** ServletFileUpload(fac);
 upload.setSizeMax(maxSize);
ResourceProgressListener progressListener = **new** ResourceProgressListener(servletRequest);//新建一个监听器
 upload.setProgressListener(progressListener);//添加自己的监听器
List items = upload.parseRequest(createRequestContext(servletRequest));
**for** (Object item1 : items) {
 FileItem item = (FileItem) item1;
**if** (*LOG*.isDebugEnabled()) *LOG*.debug("Found item " + item.getFieldName());
**if** (item.isFormField()) {
*LOG*.debug("Item is a normal form field");
 List<String> values;
**if** (params.get(item.getFieldName()) != **null**) {
 values = params.get(item.getFieldName());
 } **else** {
 values = **new** ArrayList<String>();
 }
 String charset = servletRequest.getCharacterEncoding();
**if** (charset != **null**) {
 values.add(item.getString(charset));
 } **else** {
 values.add(item.getString());
 }
 params.put(item.getFieldName(), values);
 } **else** {
*LOG*.debug("Item is a file upload");
 // Skip file uploads that don't have a file name - meaning that no file was selected.
**if** (item.getName() == **null** || item.getName().trim().length() < 1) {
*LOG*.debug("No file has been uploaded for the field: " + item.getFieldName());
**continue**;
 }
 List<FileItem> values;
**if** (files.get(item.getFieldName()) != **null**) {
 values = files.get(item.getFieldName());
 } **else** {
 values = **new** ArrayList<FileItem>();
 }
 values.add(item);
 files.put(item.getFieldName(), values);
 }
 }
 } **catch** (FileUploadException e) {
*LOG*.warn("Unable to parse request", e);
 errors.add(e.getMessage());
 }
}
上面的类建立完成后，还需要做一项工作：在struts.xml中添加如下内容：
<bean type=*"org.apache.struts2.dispatcher.multipart.MultiPartRequest"* name=*"requestParser"*
 class=*"com.zeige.ResourceMultiPartRequest"* scope=*"default"* optional=*"true"* />
<constant name=*"struts.multipart.handler"* value=*"requestParser"* />
下面就可以正常使用了，建立两个action，一个用来接收上传文件，以及对接收的文件作相应处理，处理完成后，在return SUCCESS之前去除session中currentUploadStatus属性，一个用来为页面读取实时上传进度服务，这个类中只要将session中的currentUploadStatus对象拿出来按照相应格式返回给客户端即可。
