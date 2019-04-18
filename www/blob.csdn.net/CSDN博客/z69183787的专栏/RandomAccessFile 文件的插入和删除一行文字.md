# RandomAccessFile 文件的插入和删除一行文字 - z69183787的专栏 - CSDN博客
2016年12月16日 15:26:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5166
文件中的插入一行字符目前没找到什么好方法，只能将数据读出来，然后在该插入的地方插入string后再写回去
**[java]**[view
 plain](http://blog.csdn.net/pipisky2006/article/details/6232947#)[copy](http://blog.csdn.net/pipisky2006/article/details/6232947#)
- /**
-  * 在文件里面的指定行插入一行数据
-  * 
-  * @param inFile
-  *            文件
-  * @param lineno
-  *            行号
-  * @param lineToBeInserted
-  *            要插入的数据
-  * @throws Exception
-  *             IO操作引发的异常
-  */
- publicstaticvoid insertStringInFile(File inFile, int lineno,  
-         String lineToBeInserted) throws Exception {  
- // 临时文件
-     File outFile = File.createTempFile("name", ".tmp");  
- 
- // 输入
-     FileInputStream fis = new FileInputStream(inFile);  
-     BufferedReader in = new BufferedReader(new InputStreamReader(fis));  
- 
- // 输出
-     FileOutputStream fos = new FileOutputStream(outFile);  
-     PrintWriter out = new PrintWriter(fos);  
- 
- // 保存一行数据
-     String thisLine;  
- // 行号从1开始
- int i = 1;  
- while ((thisLine = in.readLine()) != null) {  
- // 如果行号等于目标行，则输出要插入的数据
- if (i == lineno) {  
-             out.println(lineToBeInserted);  
-         }  
- // 输出读取到的数据
-         out.println(thisLine);  
- // 行号增加
-         i++;  
-     }  
-     out.flush();  
-     out.close();  
-     in.close();  
- 
- // 删除原始文件
-     inFile.delete();  
- // 把临时文件改名为原文件名
-     outFile.renameTo(inFile);  
- 
- }  
删除当然也可以像上面那样，但是如果文件比较大，那就是个工程了。
先看看下面的关于RandomAccessFile的seek
### seek
public void **seek**(long pos)
          throws [IOException](http://yuanpan1987.iteye.com/java/io/IOException.html)设置到此文件开头测量到的文件指针偏移量，在该位置发生下一个读取或写入操作。偏移量的设置可能会超出文件末尾。偏移量的设置未超出文件末尾不会改变文件的长度。只有在偏移量的设置超出文件末尾的情况下对文件进行写入才会更改其长度。
- **参数：**
`pos` - 从文件开头以字节为单位测量的偏移量位置，在该位置设置文件指针。- **抛出：**
`[IOException](http://yuanpan1987.iteye.com/java/io/IOException.html)` - 如果 `pos` 小于 `0` 或者发生 I/O 错误。
说的很是让人迷糊。
下面就一个小例子说明你：
在项目目录下建一个test文件夹，在该文件夹中新建一个aa.txt的文件。在里面写上abcdefghijklmnopqrstuvwsyz
源文件内容如下（Main方法部分）：
**[java]**[view
 plain](http://blog.csdn.net/pipisky2006/article/details/6232947#)[copy](http://blog.csdn.net/pipisky2006/article/details/6232947#)
- String basePath=System.getProperty("user.dir");  
- String fileName="aa.txt";  
- System.out.println(basePath);  
- File file=new File(basePath+"/test", fileName);  
- RandomAccessFile accessor=new RandomAccessFile(file, "rw");  
- System.out.println("文件长度:"+accessor.length());  
- accessor.seek(accessor.length());  
- accessor.writeBytes("[First Add]");  
- accessor.close();  
程序执行结果为：
**abcdefghijklmnopqrstuvwsyz[First Add]**
**[java]**[view
 plain](http://blog.csdn.net/pipisky2006/article/details/6232947#)[copy](http://blog.csdn.net/pipisky2006/article/details/6232947#)
- abcdefghijklmnopqrstuvwsyz[First Add]  
现在修改一下程序：
**[java]**[view
 plain](http://blog.csdn.net/pipisky2006/article/details/6232947#)[copy](http://blog.csdn.net/pipisky2006/article/details/6232947#)
- String basePath=System.getProperty("user.dir");  
- String fileName="aa.txt";  
- System.out.println(basePath);  
- File file=new File(basePath+"/test", fileName);  
- RandomAccessFile accessor=new RandomAccessFile(file, "rw");  
- System.out.println("文件长度:"+accessor.length());  
- accessor.seek(3);  
- accessor.writeBytes("[Second Add]");  
- accessor.close();  
程序运行结果为：
**abc[Second Add]pqrstuvwsyz[First Add]**
**[java]**[view
 plain](http://blog.csdn.net/pipisky2006/article/details/6232947#)[copy](http://blog.csdn.net/pipisky2006/article/details/6232947#)
- abc[Second Add]pqrstuvwsyz[First Add]  
比较两次结果我们可以得出以下的结论：
seek()方法是移动文件操作的指针的，而其参数就是位移量。
我们第一次将位移量设置为整个文件的长度，则指针指向了文件的末尾，我们向文件写入[First Add]，则会出现在文件的结尾。
第二次，我们将位移量设置为3，则文件指针指向了第三个字符c的后面，我们向我文件写入[Second Add]，则该文字出现在c的后方，同时覆盖掉了其所占位置的原来的字符。
这样如果是中间的文字，可以用空格符（或者其他无意义的字符）来进行一个替换。一个简单在第一行插入一个标识字符串。
**[java]**[view
 plain](http://blog.csdn.net/pipisky2006/article/details/6232947#)[copy](http://blog.csdn.net/pipisky2006/article/details/6232947#)
- publicstaticvoid writeOffset(String name, String str) {    
-     File file = new File(name);    
- 
- if (file == null || !file.exists()) {    
-         Log.d(TAG, name + "file donest exist");    
- return;    
-     }    
- 
- if (str == null || str.length() <= 0) {    
- return;    
-     }    
- 
-     RandomAccessFile rf;    
- try {    
-         rf = new RandomAccessFile(file, "rw");    
- 
-     String line1 = rf.readLine();    
- 
- if(line1 != null && line1.startsWith("PrefixXXXXXXXX")) {    
- 
- //PrefixXXXXXXXX 的长度  
-         rf.seek(13);    
-         rf.write(str.getBytes());    
-         rf.close();    
-     }else{    
- 
-         rf.close();             
-         insertStringInFile(file, 1, "XXXXXXX");    
-     }    
- 
-     } catch (FileNotFoundException e) {    
-         e.printStackTrace();    
-     } catch (IOException e) {    
- // TODO Auto-generated catch block  
-         e.printStackTrace();    
-     } catch (Exception e) {    
- // TODO Auto-generated catch block  
-         e.printStackTrace();    
-     }    
- 
- }   
参考，http://yuanpan1987.iteye.com/blog/668204
