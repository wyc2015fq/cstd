# 使用org.apache.commons.io.FileUtils,IOUtils;工具类操作文件 - z69183787的专栏 - CSDN博客
2012年10月22日 10:56:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：17506
```java
File src = new File("G:/2012/portal/login.jsp");
File tar = new File("G:/2012/portal/loginZs.jsp");
File tarDir = new File("G:/2012/portal/center/");
FileUtils.copyFile(src,tar);//复制文件
FileUtils.copyFileToDirectory(src, tarDir);//复制文件到指定文件夹
```
Commons IO是apache的一个开源的工具包,封装了IO操作的相关类,使用Commons IO可以很方便的读写文件,
FileUtils 中提供了许多设计文件操作的 已封装好的方法。
IOUtils 则是提供了读写文件的方法。
读取文件：
```
// by FileUtils  
List<String> lines = FileUtils.readLines(file, "UTF-8");  
  
// by IOUtils  
List<String> lines = IOUtils.readLines(new FileInputStream(file), "UTF-8");
```
写入文件：
```
// by FileUtils  
FileUtils.writeLines(file, "UTF-8", lines);  
  
// by IOUtils  
IOUtils.writeLines(lines, null, new FileOutputStream(file));
```
```java
public class IoTest {  
  
    /** 
     * @param args 
     */  
    public static void main(String[] args) throws Exception {  
        // 输入流复制到 输出流 
        Writer write = new FileWriter("c:\\kk.dat");  
  
        InputStream ins = new FileInputStream(new File("c:\\text.txt"));  
  
        IOUtils.copy(ins, write);  
        write.close();  
  
        ins.close();  
        
        //文本写入指定文件 
        String name = "my name is panxiuyan";  
          
        File file =  new File("c:\\name.txt");  
          
        FileUtils.writeStringToFile(file, name);  
        
        //将输入流转换成文本 
        
        URL url = new URL("http://www.dimurmill.com");  
          
        InputStream ins = url.openStream();  
          
        String contents = IOUtils.toString(ins,"UTF-8");  
        System.out.println( "Slashdot: " + contents );  
  
  			//关闭相关流
  			 File file = null;  
          
        InputStream ins = null;  
        try{  
            file = new File("C:\\Test.java");  
              
            ins = new FileInputStream(file);  
        }catch(Exception e){  
            e.printStackTrace();  
        }finally{  
            IOUtils.closeQuietly(ins);  
        }  
  
    	
    	}
    	
    	//文件复制
        File srcfile = new File("c:\\Test.java");    
        File destfile = new File("c:\\Test.java.bak");    
        FileUtils.copyFile(srcfile, destfile);    
    
    	//文件复制指定的目录 
    	File srcfile = new File("c:\\Test.java");    
        File destDir = new File("D:\\");  
        FileUtils.copyFileToDirectory(srcfile, destDir);  
    
    	//网络流保存为文件
  	URL url = new URL("http://www.163.com");   
        File file = new File("c:\\163.html");      
        FileUtils.copyURLToFile(url, file);  
        
        //文件目录操作 
        File dir = new File("c:\\test");  
        FileUtils.cleanDirectory(dir);//清空目录下的文件  
        FileUtils.deleteDirectory(dir);//删除目录和目录下的文件 
        
        //目录大小  
        long size = FileUtils.sizeOfDirectory(dir);  
        
        //目录操作
        File testFile = new File( "testFile.txt" );  
        //如果不存在,新建  
        // 如果存在,修改文件修改时间  
    	FileUtils.touch( testFile );  
    	//记录流的读取写入字节数 
    	File test = new File( "test.dat" );  
        //输出流的统计  
	CountingOutputStream countStream = null;  
	//输入流的统计  
	//CountingInputStream countStream = null;  
	try {  
	        FileOutputStream fos = new FileOutputStream( test );  		  
		countStream = new CountingOutputStream( fos );  
		countStream.write( "Hello".getBytes( ) );  
		} catch( IOException ioe ) {  
			System.out.println( "Error writing bytes to file." );  
		} finally {  	  
			IOUtils.closeQuietly( countStream );    
		}  
				  
		if( countStream != null ) {  
			 int bytesWritten = countStream.getCount( );  
			 System.out.println( "Wrote " + bytesWritten + " bytes to test.dat" );  
		}  
				
		//相同的内容写入不同的文本 
		File test1 = new File("split1.txt");  
                File test2 = new File("split2.txt");  
		OutputStream outStream = null;  
		try {  
			FileOutputStream fos1 = new FileOutputStream( test1 );  
			FileOutputStream fos2 = new FileOutputStream( test2 );  
			//包含不同的文本  
			outStream = new TeeOutputStream( fos1, fos2 );  
			outStream.write( "One Two Three, Test".getBytes( ) );  
			outStream.flush( );  	  
		} catch( IOException ioe ) {  	  
			System.out.println( "Error writing to split output stream" );  		  
		} finally {  		  
			IOUtils.closeQuietly( outStream );  		  
			} 
		 }  
  
}
```
