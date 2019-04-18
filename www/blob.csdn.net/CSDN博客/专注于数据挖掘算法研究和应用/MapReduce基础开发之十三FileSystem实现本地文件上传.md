# MapReduce基础开发之十三FileSystem实现本地文件上传 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年02月24日 18:00:08[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2154
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









场景：从本地目录下上传文件到hdfs。

参考代码：



```java
package ct.gd;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;

public class SGPGWHDFS {
	//hadoop jar /home/hdfs/dx.jar ct.gd.SGPGWHDFS &
	public static void main(String[] args) throws IOException { 
		SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd");
		String hisDate=format.format(System.currentTimeMillis());
		Configuration conf = new Configuration();
		FileSystem fs = FileSystem.get(conf);	
		List<String> hisfilename=new ArrayList<String>();//保存当前日期下已上传的文件名
		while(true){
			//获取当前时间
			String nowDate=format.format(System.currentTimeMillis());
			if(hisDate!=nowDate) {//日期变化
				hisfilename.clear();//清空
				hisDate=nowDate;
			}
			String localpath="/data/4GPGW_Data/SG/"+nowDate.substring(0, 6)+"/"+nowDate;
			String hdfspath="/pgw/sg/"+nowDate.substring(0, 6)+"/"+nowDate;		
			try {
				//从本地目录检索文件
				File f = new File(localpath);
				if (!f.exists()) continue;
				File fa[] = f.listFiles();
				for (int i = 0; i < fa.length; i++) {
					File file = fa[i];
					String filename=file.getName();
					if(!hisfilename.contains(filename)){//未上传
						//上传到hdfs
						String localfile=localpath+"/"+filename;
						InputStream in = new BufferedInputStream(new FileInputStream(localfile));
						String hdfsfile = hdfspath+"/" + filename;//hdfs存放文件路劲和名字
						OutputStream out = fs.create(new Path(hdfsfile),true);//如果已存在，覆盖
						IOUtils.copyBytes(in, out, 1024*1024,true);//迁移
						out.flush();
						if(out!=null) out.close();
						if(in!=null) in.close();
						hisfilename.add(filename);//加入已上传列表
					}
				}
			} catch (IOException e) {
				e.printStackTrace();
				continue;
			}
		}	
	} 
}
```





