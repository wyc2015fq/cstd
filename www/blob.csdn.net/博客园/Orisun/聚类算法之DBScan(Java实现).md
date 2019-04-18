# 聚类算法之DBScan(Java实现) - Orisun - 博客园







# [聚类算法之DBScan(Java实现)](https://www.cnblogs.com/zhangchaoyang/articles/2182748.html)







DBScan是一种基于密度的聚类算法，它有一个核心点的概念：如果一个点，在距它Eps的范围内有不少于MinPts个点，则该点就是核心点。核心和它Eps范围内的邻居形成一个簇。在一个簇内如果出现多个点都是核心点，则以这些核心点为中心的簇要合并。

下图给出DBScan的聚类结果：



![](https://pic002.cnblogs.com/images/2011/103496/2011092018210885.png)



可以看到DBScan可以发现噪声，即它把(3,14)判定为噪声。

到这里你一定有个疑问：为什么(8,3)一个点形成了一个簇，不是一个簇最少应该包含MinPts个点吗，如果只有一个点，那(8,3)应该归为噪声才对呀？

其实你仔细阅读下面的代码就会发现原因。在算法运行的早期，(8,3)、(5,3)、(8,6)、(10,4)被划分为一个簇，并且此时判定(8,3)是核心点—这个决定不会再更改。只是到后来(5,3)、(8,6)、(10,4)又被划分到其他簇中去了。

下面给出DBScan算法的核心代码：



package orisun;

import java.io.File;
import java.util.ArrayList;
import java.util.Vector;
import java.util.Iterator;

public class DBScan {

	double Eps=3;	//区域半径
	int MinPts=4;	//密度
	
	//由于自己到自己的距离是0,所以自己也是自己的neighbor
	public Vector<DataObject> getNeighbors(DataObject p,ArrayList<DataObject> objects){
		Vector<DataObject> neighbors=new Vector<DataObject>();
		Iterator<DataObject> iter=objects.iterator();
		while(iter.hasNext()){
			DataObject q=iter.next();
			double[] arr1=p.getVector();
			double[] arr2=q.getVector();
			int len=arr1.length;
			
			if(Global.calEditDist(arr1,arr2,len)<=Eps){		//使用编辑距离
//			if(Global.calEuraDist(arr1, arr2, len)<=Eps){	//使用欧氏距离	
//			if(Global.calCityBlockDist(arr1, arr2, len)<=Eps){	//使用街区距离
//			if(Global.calSinDist(arr1, arr2, len)<=Eps){	//使用向量夹角的正弦
				neighbors.add(q);
			}
		}
		return neighbors;
	}
	
	public int dbscan(ArrayList<DataObject> objects){
		int clusterID=0;
		boolean AllVisited=false;
		while(!AllVisited){
			Iterator<DataObject> iter=objects.iterator();
			while(iter.hasNext()){
				DataObject p=iter.next();
				if(p.isVisited())
					continue;
				AllVisited=false;
				p.setVisited(true);		//设为visited后就已经确定了它是核心点还是边界点
				Vector<DataObject> neighbors=getNeighbors(p,objects);
				if(neighbors.size()<MinPts){
					if(p.getCid()<=0)
						p.setCid(-1);		//cid初始为0,表示未分类；分类后设置为一个正数；设置为-1表示噪声。
				}else{
					if(p.getCid()<=0){
						clusterID++;
						expandCluster(p,neighbors,clusterID,objects);
					}else{
						int iid=p.getCid();
						expandCluster(p,neighbors,iid,objects);
					}
				}
				AllVisited=true;
			}
		}
		return clusterID;
	}

	private void expandCluster(DataObject p, Vector<DataObject> neighbors,
			int clusterID,ArrayList<DataObject> objects) {
		p.setCid(clusterID);
		Iterator<DataObject> iter=neighbors.iterator();
		while(iter.hasNext()){
			DataObject q=iter.next();
			if(!q.isVisited()){
				q.setVisited(true);
				Vector<DataObject> qneighbors=getNeighbors(q,objects);
				if(qneighbors.size()>=MinPts){
					Iterator<DataObject> it=qneighbors.iterator();
					while(it.hasNext()){
						DataObject no=it.next();
						if(no.getCid()<=0)
							no.setCid(clusterID);
					}
				}
			}
			if(q.getCid()<=0){		//q不是任何簇的成员
				q.setCid(clusterID);
			}
		}
	}

	public static void main(String[] args){
		DataSource datasource=new DataSource();
		//Eps=3,MinPts=4
		datasource.readMatrix(new File("/home/orisun/test/dot.mat"));
		datasource.readRLabel(new File("/home/orisun/test/dot.rlabel"));
		//Eps=2.5,MinPts=4
//		datasource.readMatrix(new File("/home/orisun/text.normalized.mat"));
//		datasource.readRLabel(new File("/home/orisun/text.rlabel"));
		DBScan ds=new DBScan();
		int clunum=ds.dbscan(datasource.objects);
		datasource.printResult(datasource.objects,clunum);
	}
}













