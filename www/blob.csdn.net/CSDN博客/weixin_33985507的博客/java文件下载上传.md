# java文件下载上传 - weixin_33985507的博客 - CSDN博客
2018年02月02日 11:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
## 文件上传
### App图片上传
```
/**
 * app端上传一个文件
 * @param request
 * @param filetype      文件类型
 * @param token
 * @return
 */
@RequestMapping("/appUploadOneFile")    
@ResponseBody
public void appUploadFile( HttpServletRequest request,String filetype){
        // 将字符串转成输入流
    InputStream inputStream = BaseToInputStream(imgStr);
}
/**
 * 将字符串装成 InputStream
 * 
 * @param base64string
 * @return InputStream
 */
//import Decoder.BASE64Decoder;
private static InputStream BaseToInputStream(String base64string) {
    ByteArrayInputStream stream = null;
    try {
        BASE64Decoder decoder = new BASE64Decoder();
        byte[] bytes1 = decoder.decodeBuffer(base64string);
        stream = new ByteArrayInputStream(bytes1);
    } catch (Exception e) {
        return null;
    }
    return stream;
}
```
### html文件上传
```
@Autowired
FileService fileService;
@RequestMapping("/upload")
@ResponseBody
public String UploadOneFile(@RequestParam("file") CommonsMultipartFile file,Integer filetype){
    Result result =new Result();
    InputStream inputStream = null;
    try {
        inputStream = file.getInputStream();
    } catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
    String totalurl = null;
    try {
        totalurl=fileService.UploadOneFile(filetype, inputStream, totalurl);
    } catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
    result.setFileLocation(totalurl);
    result.setResult(Code.Success);
    return JSON.toJSONString(result);
}
```
## 文件下载
### 文件下载
```
/**
 * 下载apk文件
 * @param request
 * @param response
 * @param version
 */
@RequestMapping(value = "/downloadApk{version}",produces = "application/json;charset=utf-8")    
public void downloadApk(HttpServletRequest request,HttpServletResponse response,@PathVariable String version){
    version=CodeUtils.iso8858ToUtf8(version);
        fileService.downloadApk(request,response,version);
}
/**
 * 获取下载apk
 * 
 * @param response
 * @param version
 * @throws UnsupportedEncodingException
 */
public void downloadApk(HttpServletRequest request,
        HttpServletResponse response, String version) {
    // 如果版本不是最新就默认返回最新的
    if (version == null || "".equals(version)) {
        ApkVersion newVersion = setUpMapper.getNewVersion();
        version = newVersion.getVersionName();
    } else {
        // 判断有没有这个版本
        ApkVersion VersionByVersionName = setUpMapper
                .getVersionByVersionName(version);
        if (VersionByVersionName == null) {
            ApkVersion newVersion = setUpMapper.getNewVersion();
            version = newVersion.getVersionName();
        }
    }
    response.setStatus(HttpServletResponse.SC_OK);
    String name = version + ".apk";// 返回的文件名
    // 获取文件在本地的存储路径
    String path = request.getSession().getServletContext().getRealPath("");
    int lastIndexOf = path.lastIndexOf("\\");
    path = path.substring(0, lastIndexOf);
    // 本地的文件地址
    String localpath = path + "\\apkDownload\\" + name;
    try {
        downfile(request, response, name, localpath);
    } catch (UnsupportedEncodingException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
    // 增加下载次数
    Map<String, String> map = new HashMap<>();
    map.put("versionName", version);
    setUpMapper.updateDownloadCount(map);
}
/**
 * 下载文件
 * 
 * @param request
 * @param response
 * @param filename
 *            下载的文件给客户端返回的名称
 * @param localpath
 *            文件在服务器中的地址
 * @throws UnsupportedEncodingException
 */
private void downfile(HttpServletRequest request,
        HttpServletResponse response, String filename, String localpath)
        throws UnsupportedEncodingException {
    ServletOutputStream outputStream = null;
    // 添加文件名称
    // String name=version+".apk";//返回的文件名
    String name = filename;// 返回的文件名
    if (request.getHeader("User-Agent").toUpperCase().indexOf("MSIE") > 0) {
        name = URLEncoder.encode(name, "UTF-8");// IE浏览器
    } else {
        name = new String(name.getBytes("UTF-8"), "ISO8859-1");// firefox浏览器||gool
    }
    response.reset();// 如果有换行，对于文本文件没有什么问题，但是对于其它格
    response.setContentType("application/x-msdownload");
    response.setHeader("Content-Disposition", "attachment;filename="
            + "dtgy.apk");
    try {
        outputStream = response.getOutputStream();
    } catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
    try {
        getFile(outputStream, localpath);
    } catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
}
/**
 * 将文件写到outputStream中
 * 
 * @param outputStream
 *            写入的outstream
 * @param localpath
 *            本地地址
 * @throws IOException
 */
private void getFile(OutputStream outputStream, String localpath)
        throws IOException {
    File file = new File(localpath);
    FileInputStream fileInputStream = null;
    fileInputStream = new FileInputStream(file);
    byte[] cotent = new byte[1024 * 1024];
    int readsize;
    boolean err = false;
    while ((readsize = fileInputStream.read(cotent)) != -1) {
        try {
            outputStream.write(cotent, 0, readsize);
        } catch (Exception e) {
            err = true;
            break;
        }
    }
    // 防止出现Connection reset by peer: socket write error
    // 出现这个原因是客户端的请求突然关闭引起的
    if (err) {
        // System.out.println("下载出错了哦");
        // outputStream.flush();
        // outputStream.close();
        fileInputStream.close();
        System.gc();
    } else {
        // System.out.println("下载没有错误");
        outputStream.flush();
        outputStream.close();
        fileInputStream.close();
    }
}
```
### 版本控制
#### xml版本控制
```
/**
 * Xml中的版本
 * @return
 */
@SuppressWarnings("unused")
private Map<String, String> getVersion() {
    URL resource = this.getClass().getResource("/");
    System.out.println(resource.getFile());
    SAXReader reader = new SAXReader();  
    //独处dom对象
    Document document;
    try {
        document = reader.read(new File(resource.getFile()+"ApkVersion/Version.xml"));
    } catch (DocumentException e) {
        // TODO Auto-generated catch block
        return new HashMap<>();
    } 
    @SuppressWarnings("unchecked")
    List<Element> versionElementsList= document.selectNodes("//update/version");
  int version=0;
  for (int i = 0; i < versionElementsList.size(); i++) {
      Element versionElement = versionElementsList.get(i);
        String text = versionElement.getText();
        int readVersion=Integer.valueOf(text);
        if (readVersion>version) {
            version=readVersion;
        }        
  }        
    Node versionNode = document.selectSingleNode("//update/version[text()='"+version+"']");//版本号
    Element versionparent = versionNode.getParent();
    Node nameNode = versionparent.selectSingleNode("name");//版本名
    Node contentNode = versionparent.selectSingleNode("content");//版本描述
    Node urlNode = versionparent.selectSingleNode("url");//下载地址
    Map<String, String>map=new HashMap<>();
    map.put("version", versionNode.getText());
    map.put("name", nameNode.getText());
    map.put("content", contentNode.getText());
    map.put("url", urlNode.getText());
    return map;
}
//xml
<?xml version="1.0" encoding="UTF-8"?>
<ApkVersion>
    <update>
        <version>1</version><!--版本号Integer -->
        <name>大唐果园1.0版 </name><!--版本名称1.1 String -->
        <content>悠果聚的第一个版本</content><!--版本描述 String -->
        <url>http://47.92.67.169:8080/apkDownload/dtgy1.0.apk</url><!--下载地址String -->
    </update>
</ApkVersion>
```
