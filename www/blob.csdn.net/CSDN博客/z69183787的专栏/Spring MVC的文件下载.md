# Spring MVC的文件下载 - z69183787的专栏 - CSDN博客
2018年07月04日 12:31:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：111
这篇文章介绍的方法在某些时候会有一些问题，小瑕疵，请大家看[更好的方法](https://my.oschina.net/songxinqiang/blog/898901)
## 读取文件
要下载文件，首先是将文件内容读取进来，使用字节数组存储起来，这里使用spring里面的工具类实现
```java
import org.springframework.util.FileCopyUtils;
    public byte[] downloadFile(String fileName) {
        byte[] res = new byte[0];
        try {
            File file = new File(BACKUP_FILE_PATH, fileName);
            if (file.exists() && !file.isDirectory()) {
                res = FileCopyUtils.copyToByteArray(file);
            }
        } catch (IOException e) {
            logger.error(e.getMessage());
        }
        return res;
    }
```
这个数组就是文件的内容，后面将输出到响应，供浏览器下载
## 下载文件的响应
下载文件的响应头和一般的响应头是有所区别的，而这里面还要根据用户浏览器的不同区别对待
我把生成响应的代码封装成了一个方法，这样所有下载响应都可以调用这个方法了，避免重复代码到处写
```java
protected ResponseEntity<byte[]> downloadResponse(byte[] body, String fileName) {
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
        headers.setContentLength(body.length);
        return new ResponseEntity<byte[]>(body, headers, status);
    }
```
这里需要注意，一般来说下载文件是使用201状态码的，但是IE浏览器不支持，还得我花了很大力气才找出来是那个问题
其中对文件名的处理是为了防止中文以及空格导致文件名乱码
## 控制器方法
在控制器的那里需要对返回值进行处理
```java
@RequestMapping(value = "/download-backup", method = RequestMethod.GET)
    @ResponseBody
    public ResponseEntity<byte[]> downloadBackupFile(@RequestParam String fileName) {
        byte[] body = backupService.downloadFile(fileName);
        return downloadResponse(body, fileName);
    }
```
