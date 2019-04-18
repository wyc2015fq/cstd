# java远程共享文件访问jcifs解决密码中特殊符号问题 - z69183787的专栏 - CSDN博客
2013年12月18日 17:05:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4960
**注意，先通过NtlmPasswordAuthentication进行验证，验证通过后用SmbFile 加入即可解决jcifs密码中的特殊符号问题**
public void smbGet(String remoteUrl,String localDir) { 
NtlmPasswordAuthentication auth = null ;
        try {  
            String userName = "administrator";  
            String password = "admin?1";  
            String domainIP = "192.168.0.5"; 
            InetAddress ip = InetAddress.getByName("192.168.0.5"); 
            UniAddress myDomain = new UniAddress(ip);
auth = new NtlmPasswordAuthentication(domainIP, userName, password);
  //先登录验证
            System.out.println("auth:"+auth.getDomain());  
            System.out.println("username:"+auth.getUsername());  
            System.out.println("password:"+auth.getPassword());  
            SmbSession.logon(myDomain,auth);  
        } catch (UnknownHostException e) {  
            e.printStackTrace();  
            System.out.println("111!!!");  
        } catch (SmbException e) {  
            e.printStackTrace();  
            System.out.println("222!!!");  
        } 
InputStream in = null; 
OutputStream out = null; 
try { 
SmbFile remoteFile = new SmbFile(remoteUrl,auth);
 //注意，这句是关键，SmbFile 不支持特殊字符的密码，因此在这里放置
//remoteFile.connect();
if(remoteFile==null){ 
log.error("共享文件不存在！000"); 
return; 
} 
SmbFile localFile = new SmbFile(localDir,auth); 
in = new BufferedInputStream(new SmbFileInputStream(remoteFile)); 
out = new BufferedOutputStream(new SmbFileOutputStream(localFile)); 
byte[] buffer = new byte[1024]; 
while(in.read(buffer)!=-1){ 
out.write(buffer); 
buffer = new byte[1024]; 
} 
} 
catch (Exception e) { 
e.printStackTrace(); 
} finally { 
try { 
out.close(); 
in.close(); 
} catch (IOException e) { 
e.printStackTrace(); 
} 
} 
}
调用：
       String st1="smb://192.168.0.5/test/123.jpg";
       String st2="smb://192.168.0.5/test/456.jpg";
       System.out.println(st1);
       System.out.println(st2);
smbGet(st1,st2);
