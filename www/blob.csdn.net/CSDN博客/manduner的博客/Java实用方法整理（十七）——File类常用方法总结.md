
# Java实用方法整理（十七）——File类常用方法总结 - manduner的博客 - CSDN博客


2018年09月12日 20:46:37[Manduner_TJU](https://me.csdn.net/manduner)阅读数：237


# 一，创建功能
1，public boolean**createNewFile()**throws IOException     创建新文件
2，public boolean**mkdirs()**创建新的目录，若父目录不存在，会自动创建
3，public boolean**renameTo(File dest)**重命名文件
# 二，判断功能
1，public boolean**isFile()**判断是否是文件
2，public boolean**isDirectory()**判断是否是目录
3，public boolean**exists()**判断文件或者目录是否存在
4，public boolean**canRead()**判断文件是否可读
5，public boolean**canWrite()**判断文件是否可写
6，public boolean**isHidden()**判断文件是否隐藏
# 三，获取功能
1，public String**getAbsolutePath()**获取绝对路径
2，public String**getPath()**获取相对路径
3，public String**getName()**获取文件或目录名
4，public long**length()**获取文件大小（应用例如：用于限制上传文件大小）
5，public long**lastModified()**获取文件最后一次修改的时间（单位，毫秒）
# 四，高级获取功能
1，public String[]**list()**获取路径表示目录下的所有文件和目录名称
2，public String[]**list(FilenameFilter filter)**获取满足过滤器FilenameFilter条件的所有目录或文件
3，public File[]**listFiles()**获取路径表示目录下的所有文件和目录对象（文件类型）
4，public File[]**listFiles(FilenameFilter filter)**获取满足过滤器FilenameFilter条件的所有目录或文件对象（文件类型）
# 五，综合实例
需求：把G:\WeeklyTP下的".png"文件重命名为“LabJob(**).png”
实现源码如下：
```python
package fileTest;
import java.io.File;
import java.io.FilenameFilter;
/**
* @author Manduner_TJU
* @version 创建时间：2018年9月11日下午10:25:22
*/
//需求：把G:\WeeklyTP下的.png文件重命名为“LabJob(**).png”
public class Test2 {
	public static void main(String[] args) {
		File file = new File("G:\\WeeklyTP");
		
		File[] fileArr = file.listFiles(new FilenameFilter() {
			@Override
			public boolean accept(File dir, String name) {
				// TODO Auto-generated method stub
				return new File(dir,name).isFile() && name.endsWith(".png");
			}
		});
		
		if(fileArr == null) {
			System.out.println("无指定类型的文件");
		}else {
			System.out.println(fileArr[0]);
		}
		
		for(File f : fileArr) {
			String name = f.getName();
			int beginIndex = name.lastIndexOf("（");
			name = name.substring(beginIndex);
			
			String newName = "LabJob".concat(name);
			File newFile = new File(file,newName);
			
			f.renameTo(newFile);
			
		}
		
		
		System.out.println("end");
		
	}
}
```
**程序运行前"G:\WeeklyTP"下文件列表如下：**
![](https://img-blog.csdn.net/20180912204249100?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**程序运行后"G:\WeeklyTP"下文件列表如下：**
![](https://img-blog.csdn.net/20180912204428552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


