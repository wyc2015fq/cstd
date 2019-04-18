# Java机器学习库ML之六关于模型迭代训练的思考 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月23日 11:32:34[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2680
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









我遇到的场景是：样本集有5000万条，接近5个G，那么这样的样本集一次导入训练，我放着一天一夜都没跑出结果，机器性能还特别好，是64位linux有128G内存。

针对这样的情况，我想到的是两种思路：

1）将样本集分割然后来迭代训练模型，这个对模型结果理论上是没有影响的，一次导入样本集训练，和多次导入样本多次训练同一个模型，最终模型结果应该是一致的；模型保存的针对训练集所训练出来的参数，如y=ax+b的a和b，样本集分割后，一个子集训练出的a和b经过下一个子集训练就会变化，最终朝向整体样本集要训练的值。

这个方法，参考代码如下：



```java
package com.vip;

import java.io.File;

import be.abeel.util.Pair;
import net.sf.javaml.classification.Classifier;
import net.sf.javaml.classification.KNearestNeighbors;
import net.sf.javaml.core.Dataset;
import net.sf.javaml.core.DefaultDataset;
import net.sf.javaml.core.DenseInstance;
import net.sf.javaml.core.Instance;
import net.sf.javaml.sampling.Sampling;
import net.sf.javaml.tools.data.FileHandler;

public class VIPClassifer {
	 public static void main(String[] args)throws Exception {
		    if (args.length != 2) {
				System.err.println("Usage: 输入训练集和测试集路径");
				System.exit(2);
			}		    
		    Sampling s = Sampling.SubSampling;
		    //Contruct a KNN classifier that uses 5 neighbors to make a decision.
	        Classifier knn = new KNearestNeighbors(5);
	        Dataset ds_validate=new DefaultDataset();
		    for(int i=0;i<12;i++){//5000万提交记录，分成10份各500万条
		    	String filePath=args[0]+"/lim_"+String.format("%02d", i);
		    	/* Load a data set 前面13列是训练特征，最后1列标记*/
		        Dataset ori_data = FileHandler.loadDataset(new File(filePath), 13, "\\s+");
		        //抽样训练集和验证集	        
		        Pair<Dataset, Dataset> sam_data = s.sample(ori_data, (int) (ori_data.size() * 0.9));	        
		        knn.buildClassifier(sam_data.x());//多样本集追加训练
		        for(Instance inst:sam_data.y()){//加入验证集
		        	ds_validate.add(inst);
		        }
		        System.out.println("训练完成第"+String.valueOf(i)+"份样本集");
		    }      
		    //验证集
	        int correct = 0, wrong = 0;
	        /* Classify all instances and check with the correct class values */
	        for (Instance inst : ds_validate) {
	            Object predictedClassValue = knn.classify(inst);
	            Object realClassValue = inst.classValue();
	            if (predictedClassValue.equals(realClassValue))
	                correct++;
	            else
	                wrong++;
	        }
	        System.out.println("Correct predictions  " + correct);
	        System.out.println("Wrong predictions " + wrong);
	        //模型预测
	        /* Load a data set 前面13列是训练特征，最后2列是uid和spuid联合标识*/
	        Dataset pre_data = FileHandler.loadDataset(new File(args[1]),"\\s+");
	        System.out.println(pre_data.instance(0));
	        Dataset out_data = new DefaultDataset();
	        for(Instance inst:pre_data){
	        	double[] values = new double[13]; 
	        	for(int i=0;i<13;i++) values[i]=inst.value(i);
	        	Instance pre_inst = new DenseInstance(values); //无标记，13列特征参与训练
	        	Object pre_classvalue = knn.classify(pre_inst);//预测结果
	        	//pre_inst.setClassValue(pre_classvalue);//标注预测结果
	        	double[] u_spu_id=new double[]{inst.value(13),inst.value(14)};
	        	Instance out_inst = new DenseInstance(u_spu_id,pre_classvalue); //带标记
	        	out_data.add(out_inst);
	        }
	        //输出u_Id+spu_id+action_type
	        FileHandler.exportDataset(out_data, new File("/data1/DataFountain/fangjs/output.txt"));
	 }
}
//java -XX:-UseGCOverheadLimit -Xmx10240m -Xms10240m -jar fangjs/vip.jar fangjs/data test_features_new.txt
//优化方向：样本类别不均衡，特征抽取和特征值处理，模型调参或模型更换
```





通过这个方法是否会和最终结果有差距，到时和python执行结果比较。




2）第二个思路我想的是模型合并，或者可以说是并行计算或分布计算，就是将样本集分布到不同机子上然后用同一个算法训练，最后合并模型。这个和模型集成有差别的，模型集成指的是对同一个训练集采用不同模型训练；而模型合并指的是对不同训练集（同一样本集分割出来）采用同一模型训练。

模型合并的思路 ，我暂时没想到怎么去试验，因为对模型结果如何合并我还没想到怎么处理。还是拿y=ax+b来说，分割出来的样本子集用同一个模型训练，一定是有不同的a和b，那用什么来合并a和b呢？如果不能，就无法分布或并行计算。理论上来说，虽然是同一模型和算法，但训练集不一样，最终结果也是和样本集有关系，没有合并的理论基础。所以这个思路应该是不行的。

当然如果能够从理论上解决模型合并，给出数学证明，还是可以尝试，可能对部分学习方法有效，如决策树分支可以合并。有兴趣的可以一起研究，前期是要对学习算法有理论掌握，才能自己编写分布式或并行模型算法。现有的机器学习库中，Spark平台集群上跑MLib库的程序是否可以达到这种效果呢？Hadoop集群的MapReduce我也没想好怎么用ML库跑出分布式效果？这个课题有兴趣的可以一起探索。




对于Java机器学习库ML下步要实战的三个方向：1）特征抽取和选择以及特征值处理；2）模型参数调优，要求掌握该学习方法理论；3）针对具体应用采用不同的学习方法，还是要求掌握理论，要回答的出为什么这个学习方法适用于这个任务呢？



