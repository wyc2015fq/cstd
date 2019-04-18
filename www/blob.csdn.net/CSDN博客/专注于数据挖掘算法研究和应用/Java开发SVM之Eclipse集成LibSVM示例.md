# Java开发SVM之Eclipse集成LibSVM示例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月21日 14:47:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9520








                
LIBSVM是台湾大学林智仁(Lin Chih-Jen)教授等开发设计的一个简单、易于使用和快速有效的SVM模式识别与回归的软件包。


1、官网下载LibSVM

   1）官网：http://www.csie.ntu.edu.tw/~cjlin/libsvm/

   2）下载：libsvm-3.21.zip

      解压后取其中的Java文件夹；

   3）下载测试数据集：https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/



2、Eclipse集成LibSVM

   1）在工程中引入libsvm.jar；

      关联libsvm源码：右键工程进入properties页面，选择Java Build Path->Libraris

      点开libsvm.jar=>Source attachment:(None)=>Edit，External location=>External Folder

      选择libsvm-3.21.zip解压后的Java文件夹；

   2）libsvm-3.21.zip解压后的Java文件夹下libsvm子文件夹外的svm_predict.java、svm_scale.java、svm_toy.java、svm_train.java复制到工程src下；

      工程项目结构图如下：
![](https://img-blog.csdn.net/20161221144233499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



3、Java示例：

   1）数据格式要求：

      label1 index1:value1 index2:value2 ...

      label2 index1:value1 index2:value2 ...

      label就是标标记，index是特征，value是特征值。

      从官网直接下载a1a训练数据和测试数据；

      训练数据用写字板打开如下图：
![](https://img-blog.csdn.net/20161221144346329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      测试数据用写字板打开如下图：

![](https://img-blog.csdn.net/20161221144451081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   2）代码如下：


```java
public class SVMDemo {
	public static void main(String[] args) throws IOException {
                String[] arg = { "D:\\tmp\\traindata.txt", //训练集
						"D:\\tmp\\model.txt" }; // 存放SVM训练模型


		String[] parg = { "D:\\tmp\\testdata.txt", //测试数据
						   "D:\\tmp\\model.txt", // 调用训练模型
						   "D:\\tmp\\predict.txt" }; //预测结果
		System.out.println("........SVM运行开始..........");
		long start=System.currentTimeMillis(); 
		svm_train.main(arg); //训练
		System.out.println("用时:"+(System.currentTimeMillis()-start)); 
		//预测
		svm_predict.main(parg); 
	}
}
```



   3）执行结果：

```
........SVM运行开始..........
*
optimization finished, #iter = 537
nu = 0.4602701289755638
obj = -673.0314153588002, rho = 0.6283368113816543
nSV = 754, nBSV = 722
Total nSV = 754
用时:1426
Accuracy = 83.58638066933712% (25875/30956) (classification)
```



  模型输出用写字板打开如下图：

![](https://img-blog.csdn.net/20161221144717161?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



  预测结果用写字板打开如下图：

![](https://img-blog.csdn.net/20161221144743880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




