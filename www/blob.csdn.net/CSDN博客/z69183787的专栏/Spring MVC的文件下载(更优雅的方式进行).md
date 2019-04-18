# Spring MVC的文件下载(更优雅的方式进行) - z69183787的专栏 - CSDN博客
2018年07月04日 12:32:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4525
个人分类：[SpringMVC-文件处理](https://blog.csdn.net/z69183787/article/category/7136696)
[https://my.oschina.net/songxinqiang/blog/898901](https://my.oschina.net/songxinqiang/blog/898901)
**1. 这里可以采用Charset,而非字符串的UTF-8。jdk7里面用StandardCharsets，低版本的可以自己用Charset.forName。fileName = new String(fileName.getBytes(StandardCharsets.UTF_8), StandardCharsets.ISO_8859_1);2. URLEncoder.encode可以换成spring里面的UriUtils.encode，就不用自己替换+号了。**
这是对[另一篇博客](https://my.oschina.net/songxinqiang/blog/891248)的升级改造
## 背景
前一段时间写了一篇博客介绍springmvc的文件下载实现方法，里面使用的是先读进内存，然后构造响应头实现的下载，那种方法看起来没有问题，但是实际上有一个可能出现的问题————要下载的文件很大的情况，在那种情况下有可能出现OOM，而且使用那种方法进行设计的时候，接口也不是很好————下载文件，但是返回了一个字节数组，应该返回“文件”啊，并且文件名还要单独请求。
所以综合那篇博客评论里面有人提出来的改进方法，对下载文件的实现进行了调整。
## 底层文件信息的提供
新建一个“下载文件信息”的类，用于保存下载文件的相关信息。
```java
import java.io.File;
/**
 * 下载文件的信息模型
 *
 * @author 阿信sxq
 *
 */
public class DownloadFileInfoVO {
    private String fileName;
    private File file;
    public DownloadFileInfoVO() {}
    public DownloadFileInfoVO(String fileName, File file) {
        this.fileName = fileName;
        this.file = file;
    }
    //setter、getter
}
```
可能有人会问为什么“文件名”要单独保存，File里面不是有文件名吗？这是应对文件存储时使用的文件名和下载给用户看的文件名不一致的情况，这种情况非常常见。
接下来就是获取这个“信息”了，根据实际业务进行编写，这里来一个简单的
```java
public DownloadFileInfoVO downloadFile(String fileName) {
        if (isBackupFileExists(fileName)) {
            File file = new File(BACKUP_FILE_PATH, fileName);
            return new DownloadFileInfoVO(fileName, file);
        } else {
            throw new NotFoundException();
        }
    }
```
这里文件的实际文件名和返回的文件名一致，不一致的情景例如：
```java
public DownloadFileInfoVO dowloadFile(Integer fileId) {
        if (voiceFileDao.exists(fileId)) {
            VoiceFile voiceFile = voiceFileDao.findOne(fileId);
            String fileName = voiceFile.getFileName() + "." + voiceFile.getFileFormat();
            File file = new File(VoiceFile.FILE_PATH, fileName);
            String showName = voiceFile.getShowName() + "." + voiceFile.getFileFormat();
            return new DownloadFileInfoVO(showName, file);
        } else {
            throw new NotFoundException();
        }
    }
```
## 异常情况
这两个代码段里面都有抛出一个自定义异常，实际中还有更多的异常，比如需要即时生成文件的请求完全有可能生成出错，关于这些一场的处理可以查看[前一篇博客](https://my.oschina.net/songxinqiang/blog/896261)，里面有介绍怎么处理
## 控制器方法
控制器接收请求之后获取下载文件的信息，然后生成下载文件的响应，@Controller返回要使用@ResponseBody注解，否则会因为找不到视图文件出错，当然，如果是@RestController就不存在这个问题了
```java
@RequestMapping(value = "/download-backup", method = RequestMethod.GET)
    @ResponseBody
    public ResponseEntity<Resource> downloadBackupFile(@RequestParam String fileName) {
        DownloadFileInfoVO fileInfo = backupService.downloadFile(fileName);
        return downloadResponse(fileInfo);
    }
```
## 生成下载响应信息
由于一个奇葩的浏览器————IE的存在，响应的时候需要对它单独处理，同事响应给用户的文件名中有可能包含一些不是英文和数字的字符，如汉语，也需要进行处理
```java
protected ResponseEntity<org.springframework.core.io.Resource> downloadResponse(
            DownloadFileInfoVO fileInfo) {
        File file = fileInfo.getFile();
        String fileName = fileInfo.getFileName();
        org.springframework.core.io.Resource body = new FileSystemResource(file);
        HttpServletRequest request = ((ServletRequestAttributes) RequestContextHolder
                .getRequestAttributes()).getRequest();
        String header = request.getHeader("User-Agent").toUpperCase();
        HttpStatus status = HttpStatus.CREATED;
        try {
            if (header.contains("MSIE") || header.contains("TRIDENT") || header.contains("EDGE")) {
                fileName = URLEncoder.encode(fileName, "UTF-8");
                fileName = fileName.replace("+", "%20");    // IE下载文件名空格变+号问题
                status = HttpStatus.OK;
            } else {
                fileName = new String(fileName.getBytes("UTF-8"), "ISO8859-1");
            }
        } catch (UnsupportedEncodingException e) {}
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
        headers.setContentDispositionFormData("attachment", fileName);
        headers.setContentLength(file.length());
        return new ResponseEntity<org.springframework.core.io.Resource>(body, headers, status);
    }
```
这里使用的Resource是完整限定名，那是因为要和进行对象注入的注解@Resource冲突，这样写才不会冲突。
同时，也有人说使用FileSystemResource似乎在某些情况下也有问题，需要使用UrlResource才不会有问题，这个倒是没有去验证
## 小结
修改为这样的方式实现文件下载之后，各个接口更加便于理解，稳定性增加，是一种更好的解决方法。
写程序就是这样，不断追求更好的解决方法，永远不满足于“能够运行”！
