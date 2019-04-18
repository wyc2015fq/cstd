# 聚类算法之CHAMELEON(Java实现) - Orisun - 博客园







# [聚类算法之CHAMELEON(Java实现)](https://www.cnblogs.com/zhangchaoyang/articles/2182752.html)





CHAMELEON是一种两阶段聚类法。第一阶段把点分成很多小的簇；第二阶段根据相近程度合并这些小的簇。第一阶段采用K最邻近法，即把一个点和它最邻近的K个点连接起来。第二阶段计算任意两个簇的互连性RI和紧密性RC，当两个指标都比较大时才合并这两个簇。

相对互连度

$$RI(C_i,C_j)=\frac{2*|EC(C_i,C_j)|}{|EC(C_i)|+|EC(C_j)|}$$

相对紧密度

\begin{equation}RC(C_i,C_j)=\frac{(|C_i|+|C_j|)EC(C_i,C_j)}{|C_j|EC(C_i)+|C_i|EC(C_j)}\end{equation}

|Ci|表示簇i内数据点的个数；EC(Ci)表示簇i内所有边的权重和；EC(Ci,Cj)表示跨越两个簇的所有边的权重和。

下图是第一阶段后形成的几个小的子簇：

![](https://pic002.cnblogs.com/images/2011/103496/2011092018225981.png)



把子簇合并后形成的最终簇划分：



![](https://pic002.cnblogs.com/images/2011/103496/2011092018232562.png)

 CHAMELEON具有两个特点：（1）适合于高维数据的聚类，在文本分类中，每个文本都被表示为一个数千维的向量。（2）采用k-邻近图可以动态地捕捉邻域概念，在稠密区域邻域比较窄，在稀疏区域邻域比较宽，这相比于DBSCAN中的全局邻域密度来说容易获得更自然的邻域。

下面给出CHAMELEON算法的核心代码：

/**
 * Author: Orisun
 * Date: Sep 13, 2011
 * FileName: chameleon.java
 * Function: 
 */
package orisun;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.zip.DeflaterOutputStream;
import java.util.zip.InflaterInputStream;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Map.Entry;
import java.util.Queue;
import java.util.Vector;

public class Chameleon {
	float[][] W; // weight矩阵(方阵)
	byte[][] Conn; // 连接矩阵(方阵)
	Vector<Vector<Integer>> clusters;
	double MI; // 综合指数

	// 构造函数，初始化变量
	public Chameleon(int datanum, double mi) {
		W = new float[datanum][];
		for (int i = 0; i < datanum; i++) {
			W[i] = new float[datanum];
		}
		Conn = new byte[datanum][];
		for (int i = 0; i < datanum; i++) {
			// 由于是无向图，连接矩阵是对称的，所以我们只用矩阵的下三角以节约空间
			Conn[i] = new byte[i + 1];
		}
		clusters = new Vector<Vector<Integer>>();
		MI = mi;
	}

	// 构造weight矩阵。根据两点间距离的倒数计算两点的相似度，作为连接权重
	public void buildWeightMatrix(ArrayList<DataObject> objects) {
		for (int i = 0; i < W.length; i++) {
			W[i][i] = 1.0f;
			for (int j = i + 1; j < W[i].length; j++) {
				float dist = (float) Global.calEuraDist(objects.get(i)
						.getVector(), objects.get(j).getVector(), objects
						.get(j).getVector().length);
				W[i][j] = W[j][i] = 1 / (1 + dist);
			}
		}
	}

	// 把weight矩阵写入文件，下次计算相同实例时免得重新计算
	public void writeWeightToFile(File file) {
		DataOutputStream fout;
		try {
			fout = new DataOutputStream(
					new DeflaterOutputStream(new FileOutputStream(file)));
			fout.writeInt(W.length);	//先把方阵的边长写入文件
			for(int i=0;i<W.length;i++)
				for(int j=0;j<W.length;j++)
					fout.writeFloat(W[i][j]);
			fout.close();
		} catch (FileNotFoundException e) {
			System.err.println("File Not Found!");
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	// 从文件读入weight矩阵
	public void readWeightFromFile(File file){
		DataInputStream fin;
		try {
			fin = new DataInputStream(
					new InflaterInputStream(new FileInputStream(file)));
			fin.readInt();	//第一个数字是方阵的边长，略过
			for(int i=0;i<W.length;i++)
				for(int j=0;j<W.length;j++)
					W[i][j]=fin.readFloat();
			fin.close();
		} catch (FileNotFoundException e) {
			System.err.println("File Not Found!");
		}catch(IOException e){
			e.printStackTrace();
		}
	}

	// CHAMELEON第一阶段，按照K最邻近建立较小的子簇
	public void buildSmallCluster() {
		PriorityQueue<Entry<Integer, Float>> pq = new PriorityQueue<Entry<Integer, Float>>(
				Global.k, new Comparator<Map.Entry<Integer, Float>>() {
					public int compare(Entry<Integer, Float> arg0,
							Entry<Integer, Float> arg1) {
						return arg0.getValue().compareTo(arg1.getValue());
					}
				});
		for (int i = 0; i < W.length; i++) {
			pq.clear();
			int j = 0;
			HashMap<Integer, Float> map = new HashMap<Integer, Float>();
			// 找到与object距离最小（亦即相似度最大）的K个点
			for (; j < Global.k; j++) {
				map.clear();
				map.put(j, W[i][j]);
				pq.add(map.entrySet().iterator().next());
			}
			for (; j < W[i].length; j++) {
				if (W[i][j] > pq.peek().getValue()) {
					pq.poll();
					map.clear();
					map.put(j, W[i][j]);
					pq.add(map.entrySet().iterator().next());
				}
			}
			for (j = 0; j < Global.k; j++) {
				Entry<Integer, Float> entry = pq.poll();
				if (i > entry.getKey())
					Conn[i][entry.getKey()] = 1;
				else if (i < entry.getKey())
					Conn[entry.getKey()][i] = 1;
				// 对角线上的设为0
			}
		}
		// 根据连接矩阵构造连通子图
		boolean[] visited = new boolean[W.length];
		boolean allvisited = false;
		while (!allvisited) {
			allvisited = true;
			L1: for (int i = 0; i < W.length; i++) {
				if (visited[i])
					continue;
				allvisited = false;
				// 搜寻第i列，以图发现新子图的第一个点
				for (int j = i + 1; j < W.length; j++) {
					// 发现了新子图的第一个点
					if (Conn[j][i] == 1) {
						Vector<Integer> cluster = new Vector<Integer>();
						Queue<Integer> queue = new LinkedList<Integer>();
						queue.add(i);
						queue.add(j);
						while (!queue.isEmpty()) {
							int ele = queue.poll();
							cluster.add(ele);
							visited[ele] = true;
							// 遍历第ele列
							for (int k = ele + 1; k < W.length; k++) {
								if (visited[k])
									continue;
								if (Conn[k][ele] == 1 && !queue.contains(k)) {
									queue.add(k);
								}
							}
							// 遍历第ele行
							for (int k = 0; k < ele; k++) {
								if (visited[k])
									continue;
								if (Conn[ele][k] == 1 && !queue.contains(k))
									queue.add(k);
							}
						}
						clusters.add(cluster);
						break L1;
					}
				}
			}
		}
	}

	// 打印子簇
	public void printClusters() {
		for (int i = 0; i < clusters.size(); i++) {
			System.out.print("以下数据点属于第" + i + "簇：");
			Iterator<Integer> iter = clusters.get(i).iterator();
			while (iter.hasNext()) {
				System.out.print(iter.next() + ",");
			}
			System.out.println();
		}
	}

	// CHAMELEON第二阶段，合并相对互联度RI和相对紧密度RC都较高的簇
	public void cluster() {
		int len = clusters.size();
		float[] EC1 = new float[len];
		for (int i = 0; i < len; i++) {
			Vector<Integer> vec = clusters.get(i);
			for (int j = 0; j < vec.size(); j++) {
				for (int k = 0; k < vec.size(); k++) {
					EC1[i] += W[vec.get(j)][vec.get(k)];
				}
			}
		}
		boolean end = true;
		for (int i = 0; i < clusters.size(); i++) {
			for (int j = i + 1; j < clusters.size(); j++) {
				Vector<Integer> vec1 = clusters.get(i);
				Vector<Integer> vec2 = clusters.get(j);
				float EC = 0.0f;
				float RI = 0.0f;
				float SEC = 0.0f;
				float RC = 0.0f;
				for (int k = 0; k < vec1.size(); k++) {
					for (int m = 0; m < vec2.size(); m++) {
						EC += W[vec1.get(k)][vec2.get(m)];
					}
				}
				RI = 2 * EC / (EC1[i] + EC1[j]);
				RC = (vec1.size() + vec2.size()) * EC
						/ (vec2.size() * EC1[i] + vec1.size() * EC1[j]);
				// 以RI*RC作为综合指数
				if (RI * RC > MI) {
					mergeClusters(i, j);
					end = false;
					break;
				}
			}
		}
		// 递归合并子簇
		if (!end)
			cluster();
	}

	// 把簇b合并到簇a里面去
	public void mergeClusters(int a, int b) {
		Iterator<Integer> iter = clusters.get(b).iterator();
		while (iter.hasNext()) {
			clusters.get(a).add(iter.next());
		}
		clusters.remove(b);
	}

	public static void main(String[] args) {
		Global.setK(2); // 2最邻近，这里面包括它自己
		DataSource datasource = new DataSource();
		datasource.readMatrix(new File("/home/orisun/test/dot.mat"));
		datasource.readRLabel(new File("/home/orisun/test/dot.rlabel"));
//		datasource.readMatrix(new File("/home/orisun/test/text.normalized.mat"));
//		datasource.readRLabel(new File("/home/orisun/test/text.rlabel"));
//		File wfile=new File("/home/orisun/test/test_weight");
//		if(!wfile.exists()){
//			try {
//				wfile.createNewFile();
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
//		}
		//综合指数0.1
		Chameleon cham = new Chameleon(datasource.row, 0.1);
//		cham.readWeightFromFile(wfile);
		cham.buildWeightMatrix(datasource.objects);
//		cham.writeWeightToFile(wfile);
		cham.buildSmallCluster();
		System.out.println("==============第一阶段后的分类结果==============");
		cham.printClusters();
		cham.cluster();
		System.out.println("==============第二阶段后的分类结果==============");
		cham.printClusters();
	}
}












