# SpringMVC实现文件下载的两种方法 - z69183787的专栏 - CSDN博客
2018年06月30日 20:53:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：980
https://blog.csdn.net/wuzuodingfeng/article/details/53489089
### 基于ResponseEntity实现
```
@RequestMapping("/testHttpMessageDown")
public ResponseEntity<byte[]> download(HttpServletRequest request) throws IOException {
    File file = new File("E://123.jpg");
    byte[] body = null;
    InputStream is = new FileInputStream(file);
    body = new byte[is.available()];
    is.read(body);
    HttpHeaders headers = new HttpHeaders();
    headers.add("Content-Disposition", "attchement;filename=" + file.getName());
    HttpStatus statusCode = HttpStatus.OK;
    ResponseEntity<byte[]> entity = new ResponseEntity<byte[]>(body, headers, statusCode);
    return entity;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
### Java通用下载实现
下载过程的实现，在java中调用reponse.getOutputStream()方法会自动激活下载操作
```java
public static void download(String fileName, String filePath,
HttpServletRequest request, HttpServletResponse response) 
throws Exception {
    //声明本次下载状态的记录对象
    DownloadRecord downloadRecord = new DownloadRecord(fileName, filePath, request);
    //设置响应头和客户端保存文件名
    response.setCharacterEncoding("utf-8");
    response.setContentType("multipart/form-data");
    response.setHeader("Content-Disposition", "attachment;fileName=" + fileName);
    //用于记录以完成的下载的数据量，单位是byte
    long downloadedLength = 0l;
    try {
        //打开本地文件流
        InputStream inputStream = new FileInputStream(filePath);
        //激活下载操作
        OutputStream os = response.getOutputStream();
        //循环写入输出流
        byte[] b = new byte[2048];
        int length;
        while ((length = inputStream.read(b)) > 0) {
            os.write(b, 0, length);
            downloadedLength += b.length;
        }
        // 这里主要关闭。
        os.close();
        inputStream.close();
    } catch (Exception e){
        downloadRecord.setStatus(DownloadRecord.STATUS_ERROR);
        throw e;
    }
    downloadRecord.setStatus(DownloadRecord.STATUS_SUCCESS);
    downloadRecord.setEndTime(new Timestamp(System.currentTimeMillis()));
    downloadRecord.setLength(downloadedLength);
    //存储记录
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
下载记录信息实体
```java
public class DownloadRecord {
public static final int STATUS_ERROR = 0;
    public static final int STATUS_SUCCESS = 1;
    private String uid;
    private String ip;
    private int port;
    private String ua;
    private String fileName;
    private String filePath;
    private long length;
    private int status;
    private Timestamp startTime;
    private Timestamp endTime;
    public DownloadRecord() {
    }
    public DownloadRecord(String fileName, String filePath, 
    HttpServletRequest request) {
        this.uid = UUID.randomUUID().toString().replace("-","");
        this.fileName = fileName;
        this.filePath = filePath;
        this.ip = request.getRemoteAddr();
        this.port = request.getRemotePort();
        this.ua = this.ua = request.getHeader("user-agent");
        this.startTime = new Timestamp(System.currentTimeMillis());
    }
    /** getter and setter **/
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
### 比较
- 基于ResponseEntity的实现的局限性还是很大，从代码中可以看出这种下载方式是一种一次性读取的下载方式，在文件较大的时候会直接抛出内存溢出（我自己亲测一个1.8G的文件在执行下载操作的时候直接抛出了内存溢出）。还有就是这种方式在进行下载统计的时候也存在局限性，无法统计在下载失败的情况已完成下载量，因此限制了对下载的功能扩展。虽然这种实现方式有局限性，但是也有着优点——简洁。在很多时候我们并不需要那么复杂的下载功能时，这种实现就应该是首选了。
- 然而下载java通用实现在功能上比第一种实现更加丰富，对下载的文件大小无限制（循环读取一定量的字节写入到输出流中，因此不会造成内存溢出，但是在下载人数过多的时候应该还是出现一些异常，不过下载量较大的文件一般都会使用ftp服务器来做吧），另外因为是这种实现方式是基于循环写入的方式进行下载，在每次将字节块写入到输出流中的时都会进行输出流的合法性检测，在因为用户取消或者网络原因造成socket断开的时候，系统会抛出SocketWriteException，系统可以捕捉这个过程中抛出的异常，当捕捉到异常的时候我们可以记录当前已经传输的数据量，这样就可以完成下载状态和对应状态下载量和速度之类的数据记录。另外这种方式实现方式还可以实现一种断点续载的功能。
