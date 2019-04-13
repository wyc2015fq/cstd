
# 项目学习类容，日期处理，SQL语句查询，hashtable的运用，R语言包的环境配置 - jerrygaoling的博客 - CSDN博客

2017年06月04日 20:14:45[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：179标签：[hashtable																](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[sql																](https://so.csdn.net/so/search/s.do?q=sql&t=blog)[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=class&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=sql&t=blog)个人分类：[学习记录																](https://blog.csdn.net/jerrygaoling/article/category/6954410)
[
																								](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)

# 1.项目工作
1. 按照开会老师提出的要求，更改之前导表出现的问题。
2. 按照宋老师要求，提取数据库数据，处理后，提供接口给粘冬晓运算。
3. 计算宋老师需要的病例比例数据。
4. 导入R包，安装R语言编译环境，尝试运用R包进行数据计算。
## 2.具体完成内容
1.  实现项目所要求的功能；
由于前期数据库数据的局限性，采用SQL语句进行查询计算后，直接提取数据，由循环提取所需要的每日数据信息。
数据库：
![](https://img-blog.csdn.net/20170605114138013)

SQL：SELECT *,COUNT(Csymptoms) AS num FROM clinic WHERE Cseedate >= '2016-01-05%' ANDCseedate < '2016-01-25%' AND Csymptoms LIKE '%发%'GROUP BY Cseedate

![](https://img-blog.csdn.net/20170605114158826)

SQL：SELECT  COUNT(Csymptoms) AS num FROM clinic WHERECseedate LIKE '2016-01-08%' AND Csymptoms LIKE '%发%'
![](https://img-blog.csdn.net/20170605114202082)
将其中关键位置控制为变量；时间，症状等；
查询完成后，查询时间为14s；查询日期间隔为20天。
2.  项目代码优化
由于查询速度太慢，开始尝试优化项目的用户体验感。主要解决一次查询与多次查询。
想到之前分词的时候，用的哈希表，速度快的飞起。主要聚焦点为不能合并的“就诊日期”时间不规整的问题。最终解决问题。
最终运用的思路为邵老师之前不知道的词频查询任务。现在运行时间，一年的数据，可0.9s完成。
![](https://img-blog.csdn.net/20170605114205561)

3.  代码相互结合
与粘冬晓的算法部分相结合，去运算所得到的数据
实现通过数据库的查询来计算出C1C2C3和EWMA 的值。
根据宋老师的要求需要两个接口来调试λ和K的值。下面代码用来计算EWMA的值
for(j=0;j<listavg.length;j++){
Mt=0;
for(x=0,i=j+7;x<8;x++,i--){
Mt+=(1-d)*arr[i]*Math.pow(d,x);
}
//  System.out.println("Mt"+j+"="+Mt);
Y=listavg[j]+e*listvariance[j]*Math.pow((d/(2-d)),0.5);
//System.out.println("Y"+j+"="+Y);
= Mt>Y?1:0;
// System.out.println("ewma"+j+"="+ewma);
Ewma[j]=ewma; //把计算完的ewma值写到Ewam数组中
}
传回到前台用hightcharts来显示



## 3.病例统计
具体代码，见附件check.jsp。主要是根据宋老师要求，运算其所要的病例比例等数据，数据来源为数据库提取。只是反馈数据，所以没有前台展示。

## 4.R包，R语言的学习
R包的安装教程以及myeclipse对R包的调用
Step1.要准备的东西http://pan.baidu.com/s/1bpfBSej可以去这里面下载所有所需文件
Step2.安装R-3.0.1-win双击R-3.0.1-win.exe即可，路径自己配置，建议默认即可，32位和64位都直接安装，可以避免一些麻烦。
Step3.配置R的环境变量。找到安装的R的路径C:\Program Files\R\R-3.0.1\include;C:\Program Files\R\R-3.0.1\bin;在系统变量的path中配置以上两个变量。（环境变量结束的分号一定为英语输入法）
Step4.用键盘键入win+R打开运行，输入cmd进入doc界面输入r大小写都可以，运行，出现以下图片内容即为环境变量配置成功。
![](https://img-blog.csdn.net/20170605114213639)![](https://img-blog.csdn.net/20170605114213639)

接下来打开R i386 3.0.1输入install.packages("Rserve")选择一个地址进行下载例如选择China(Beijing)即可等待自动安装。
成功之后输入library(Rserve) 回车Rserve(),为运行Rserve服务器
![](https://img-blog.csdn.net/20170605114216770)
![](https://img-blog.csdn.net/20170605114220327)


这样即为成功启动Rserve服务
Step5.打开myeclipse在自己建的工程中导入刚才下好的文件中的REngine.jar和RserveEngine.jar包然后新建一个java文件输入以下代码
import org.rosuda.REngine.REXP;
importorg.rosuda.REngine.Rserve.RConnection;
public class R_test {
/**
* @param args
*/
publicstatic void main(String[] args) {
try{    RConnection c = newRConnection();
REXPx = c.eval("R.version.string");
System.out.println(x.asString());}
catch(Exception e) {
e.printStackTrace();    }
出现以下运行结果即可
}
![](https://img-blog.csdn.net/20170605114223786)
}


