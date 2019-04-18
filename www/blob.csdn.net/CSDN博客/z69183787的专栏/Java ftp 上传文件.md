# Java ftp 上传文件 - z69183787的专栏 - CSDN博客
2012年11月16日 15:58:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：858
```java
public static void FtpUpload(String fileName, String newFileName, String filePath)
    throws Exception
  {
    FtpClient ftpClient;
    try
    {
      ftpClient = new FtpClient();
      ftpClient.openServer(server, Integer.parseInt(port));
      ftpClient.login(user, password);
      ftpClient.sendServer("");
      if (filePath.length() != 0)
        ftpClient.cd(filePath);
      ftpClient.binary();//上传文件
      TelnetOutputStream os = null;
      FileInputStream is = null;
      try
      {
        int c;
        String newname = "";
        fileName = StringUtils.replace(fileName, "\\", "/");
        if (fileName.indexOf("/") > -1)
          newname = fileName.substring(fileName.lastIndexOf("/") + 1);
        else {
          newname = fileName;
        }
        if ((newFileName != null) && (!(newFileName.equals(""))))
          os = ftpClient.put(newFileName);
        else {
          os = ftpClient.put(newname);
        }
        File file_in = new File(fileName);
        is = new FileInputStream(file_in);
        byte[] bytes = new byte[1024];
        while ((c = is.read(bytes)) != -1)
          os.write(bytes, 0, c);
      }
      finally {
        if (is != null)
          is.close();
        if (os != null)
          os.close();
        if (ftpClient != null)
          ftpClient.closeServer();
      }
    }
    catch (Exception e) {
      e.printStackTrace();
      throw e;
    }
  }
```
