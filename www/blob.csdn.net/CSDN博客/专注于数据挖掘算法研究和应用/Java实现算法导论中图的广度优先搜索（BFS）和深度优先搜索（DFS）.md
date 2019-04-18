# Java实现算法导论中图的广度优先搜索（BFS）和深度优先搜索（DFS） - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月12日 11:34:17[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2501
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









对算法导论中图的广度优先搜索（BFS）和深度优先搜索（DFS）用Java实现其中的伪代码算法，案例也采用算法导论中的图。



```java
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;

public class GraphSearch {
	
	final static HashMap<String,LinkedList<String>> DGraph=new HashMap<String,LinkedList<String>>(); //有向图
	final static HashMap<String,LinkedList<String>> UGraph=new HashMap<String,LinkedList<String>>(); //无向图
	public static final int MAX_VALUE=2147483647;
	public static final int MIN_VALUE=-2147483648;
	public static void main(String [] args) {   
		//邻接表表示，初始化有向图和无向图，图用算法导论中的案例，分别是图22-1和图22-2
		GraphSearch.initUGraph();
		GraphSearch.initDGraph();		
		//BFS广度优先搜索
		//GraphSearch.BFSUGraph();
		//GraphSearch.BFSDGraph();
		//DFS深度优先搜索
		GraphSearch gs=new GraphSearch();
		//gs.DFSUGraph();
		gs.DFSDGraph();
	}
	//深度优先搜索
	//初始化六个顶点的颜色、父顶点、发现距离和结束距离
	public  String[] DDColor = new String[]{"white","white","white","white","white","white"};
	public  int[] DDDis=new int[]{MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE};
	public  int[] DDFin=new int[]{MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE};
	public  String[] DDParent=new String[]{"null","null","null","null","null","null"};
	public  int DDTime=0;//计算发现时间和结束时间
	public void DFSDGraph(){//有向图深度优先搜索
		//遍历邻接表
		Iterator<Entry<String, LinkedList<String>>> iter=DGraph.entrySet().iterator();	
		while (iter.hasNext() ) {
			Map.Entry<String, LinkedList<String>> entry = iter.next();
			String key = entry.getKey();
			int iKey=Integer.valueOf(key.substring(1))-1;
			if (DDColor[iKey]=="white") DFSDGraphVisit(key);
		}
	}
	public void DFSDGraphVisit(String key){//无向图深度优先搜索
		int n=Integer.valueOf(key.substring(1))-1;
		DDColor[n]="gray";
		DDTime=DDTime+1;
		DDDis[n]=DDTime;
		LinkedList<String> llV=DGraph.get(key);
		for(int i=0;i<llV.size();i++){
			String strV=llV.get(i);
			int m=Integer.valueOf(strV.substring(1))-1;
			if(DDColor[m]=="white"){
				DDParent[m]=key;
				DFSDGraphVisit(strV);
			}
		}	
		DDColor[n]="black";
		DDTime=DDTime+1;
		DDFin[n]=DDTime;
		System.out.println("顶点："+key+"发现时间："+String.valueOf(DDDis[n])+"结束时间："+String.valueOf(DDFin[n])+"父顶点："+DDParent[n]+"当前颜色："+DDColor[n]);
	}

	//初始化五个顶点的颜色、父顶点、发现距离和结束距离
	public  String[] DUColor = new String[]{"white","white","white","white","white"};
	public  int[] DUDis=new int[]{MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE};
	public  int[] DUFin=new int[]{MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE};
	public  String[] DUParent=new String[]{"null","null","null","null","null"};
	public  int DUTime=0;//计算发现时间和结束时间
	public  void DFSUGraph(){//无向图深度优先搜索	
		//遍历邻接表
		Iterator<Entry<String, LinkedList<String>>> iter=UGraph.entrySet().iterator();	
		while (iter.hasNext() ) {
		    Map.Entry<String, LinkedList<String>> entry = iter.next();
		    String key = entry.getKey();
		    int iKey=Integer.valueOf(key.substring(1))-1;
		    if (DUColor[iKey]=="white") DFSUGraphVisit(key);
		}
	}
	public void DFSUGraphVisit(String key){//无向图深度优先搜索
		int n=Integer.valueOf(key.substring(1))-1;
		DUColor[n]="gray";
		DUTime=DUTime+1;
		DUDis[n]=DUTime;
		LinkedList<String> llV=UGraph.get(key);
		for(int i=0;i<llV.size();i++){
			String strV=llV.get(i);
			int m=Integer.valueOf(strV.substring(1))-1;
			if(DUColor[m]=="white"){
				DUParent[m]=key;
				DFSUGraphVisit(strV);
			}
		}	
		DUColor[n]="black";
		DUTime=DUTime+1;
		DUFin[n]=DUTime;
		System.out.println("顶点："+key+"发现时间："+String.valueOf(DUDis[n])+"结束时间："+String.valueOf(DUFin[n])+"父顶点："+DUParent[n]+"当前颜色："+DUColor[n]);
	}
	
	//广度优先搜索
	public static void BFSDGraph(){//有向图广度优先搜索
		//有向图DFS算法要改良，多个源顶点生成多颗树
		//选初始化六个的颜色、父顶点、最短距离
		String[] color = new String[]{"white","white","white","white","white","white"};
		int[] dist=new int[]{MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE};
		String[] parent=new String[]{"null","null","null","null","null","null"};
		//遍历顶点表DGraph，看是否还存在未搜索的
		ArrayList<String> ALV=new ArrayList<String> ();
		Iterator<Entry<String, LinkedList<String>>> iter=DGraph.entrySet().iterator();	
		while (iter.hasNext() && ALV.size()<6) {
		    Map.Entry<String, LinkedList<String>> entry = iter.next();
			String key = entry.getKey();
			if(ALV.contains(key)) continue;//已搜索，下次循环
			//LinkedList<String> val = entry.getValue();
			int iSV=Integer.valueOf(key.substring(1))-1;
			//初始化源顶点
			color[iSV]="gray";
			dist[iSV]=0;
			parent[iSV]="null";
			ArrayList<String> queue=new ArrayList<String>();
			queue.add(key);
			//开始搜索
			while(queue.size()>0){
				String strV=queue.get(0);
				queue.remove(0);//出列
				int m=Integer.valueOf(strV.substring(1))-1;
				LinkedList<String> listV= DGraph.get(strV);
				for(int i=0;i<listV.size();i++){
					String strVTmp=listV.get(i);
					int n=Integer.valueOf(strVTmp.substring(1))-1;//顶点名称第二个字符是数字
					if(color[n]=="white"){
						color[n]="gray";
						dist[n]=dist[m]+1;
						parent[n]=strV;
						queue.add(strVTmp);
						}
					}
				color[m]="black";
				System.out.println("顶点："+strV+"最短距离："+String.valueOf(dist[m])+"父顶点："+parent[m]+"当前颜色："+color[m]);
				ALV.add(strV);
			}		
		}
	}
	public static void BFSUGraph(){//无向图广度优先搜索
		//选择顶点V1为源点，初始化五个的颜色、父顶点、最短距离
		String[] color = new String[]{"white","white","white","white","white"};
		int[] dist=new int[]{MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE,MAX_VALUE};
		String[] parent=new String[]{"null","null","null","null","null"};
		//初始化源顶点V1
		color[0]="gray";
		dist[0]=0;
		parent[0]="null";
		ArrayList<String> queue=new ArrayList<String>();
		queue.add("V1");
		//开始搜索
		while(queue.size()>0){
			String strV=queue.get(0);
			queue.remove(0);//出列
			int m=Integer.valueOf(strV.substring(1))-1;
			LinkedList<String> listV= UGraph.get(strV);
			for(int i=0;i<listV.size();i++){
				String strVTmp=listV.get(i);
				int n=Integer.valueOf(strVTmp.substring(1))-1;//顶点名称第二个字符是数字
				if(color[n]=="white"){
					color[n]="gray";
					dist[n]=dist[m]+1;
					parent[n]=strV;
					queue.add(strVTmp);
				}
			}
			color[m]="black";
			System.out.println("顶点："+strV+"最短距离："+String.valueOf(dist[m])+"父顶点："+parent[m]+"当前颜色："+color[m]);
		}		
	}
	//初始化图	
	public static void initUGraph(){//无向图初始化
		//顶点V1的邻接表
		String strV1="V1";
		LinkedList<String> ListV1=new LinkedList<String>();
		ListV1.add("V2");
		ListV1.add("V5");
		UGraph.put(strV1, ListV1);
		//顶点V2的邻接表
		String strV2="V2";
		LinkedList<String> ListV2=new LinkedList<String>();
		ListV2.add("V1");
		ListV2.add("V5");
		ListV2.add("V3");
		ListV2.add("V4");
		UGraph.put(strV2, ListV2);
		//顶点V3的邻接表
		String strV3="V3";
		LinkedList<String> ListV3=new LinkedList<String>();
		ListV3.add("V2");
		ListV3.add("V4");
		UGraph.put(strV3, ListV3);
		//顶点V4的邻接表
		String strV4="V4";
		LinkedList<String> ListV4=new LinkedList<String>();
		ListV4.add("V2");
		ListV4.add("V5");
		ListV4.add("V3");
		UGraph.put(strV4, ListV4);
		//顶点V5的邻接表
		String strV5="V5";
		LinkedList<String> ListV5=new LinkedList<String>();
		ListV5.add("V4");
		ListV5.add("V1");
		ListV5.add("V2");
		UGraph.put(strV5, ListV5);
	}
	public static void initDGraph(){//有向图初始化
		//顶点V1的邻接表
		String strV1="V1";
		LinkedList<String> ListV1=new LinkedList<String>();
		ListV1.add("V2");
		ListV1.add("V4");
		DGraph.put(strV1, ListV1);
		//顶点V2的邻接表
		String strV2="V2";
		LinkedList<String> ListV2=new LinkedList<String>();
		ListV2.add("V5");
		DGraph.put(strV2, ListV2);
		//顶点V3的邻接表
		String strV3="V3";
		LinkedList<String> ListV3=new LinkedList<String>();
		ListV3.add("V6");
		ListV3.add("V5");
		DGraph.put(strV3, ListV3);
		//顶点V4的邻接表
		String strV4="V4";
		LinkedList<String> ListV4=new LinkedList<String>();
		ListV4.add("V2");
		DGraph.put(strV4, ListV4);
		//顶点V5的邻接表
		String strV5="V5";
		LinkedList<String> ListV5=new LinkedList<String>();
		ListV5.add("V4");
		DGraph.put(strV5, ListV5);
		//顶点V6的邻接表
		String strV6="V6";
		LinkedList<String> ListV6=new LinkedList<String>();
		ListV6.add("V6");
		DGraph.put(strV6, ListV6);
		
		
		/*List arrayList = new ArrayList(DGraph.entrySet());
		Collections.sort(arrayList, new Comparator() {  
			public int compare(Object arg1, Object arg2) {  
				Map.Entry obj1 = (Map.Entry) arg1; 
				Map.Entry obj2 = (Map.Entry) arg2;  
				return (obj1.getKey()).toString().compareTo((String) obj2.getKey());  
		  }  
		}); 
		//将HASHMAP中的数据排序  
		for (Iterator iter = arrayList.iterator(); iter.hasNext();)  {  
		   Map.Entry entry = (Map.Entry)iter.next();  
		   String  key = (String)entry.getKey();  
		   System.out.println(key);    
		}*/
		/*Iterator<Entry<String, LinkedList<String>>> iter=DGraph.entrySet().iterator();	
		while (iter.hasNext() ) {
		    Map.Entry<String, LinkedList<String>> entry = iter.next();
			System.out.println(entry.getKey());
		}*/
	}
	
	
}
```
结果：

1）无向图广度优先搜索：



```
顶点：V1最短距离：0父顶点：null当前颜色：black
顶点：V2最短距离：1父顶点：V1当前颜色：black
顶点：V5最短距离：1父顶点：V1当前颜色：black
顶点：V3最短距离：2父顶点：V2当前颜色：black
顶点：V4最短距离：2父顶点：V2当前颜色：black
```


2）有向图广度优先搜索



```
顶点：V2最短距离：0父顶点：null当前颜色：black
顶点：V5最短距离：1父顶点：V2当前颜色：black
顶点：V4最短距离：2父顶点：V5当前颜色：black
顶点：V3最短距离：0父顶点：null当前颜色：black
顶点：V6最短距离：1父顶点：V3当前颜色：black
顶点：V1最短距离：0父顶点：null当前颜色：black
```


3）无向图深度优先搜索



```
顶点：V3发现时间：5结束时间：6父顶点：V4当前颜色：black
顶点：V4发现时间：4结束时间：7父顶点：V5当前颜色：black
顶点：V5发现时间：3结束时间：8父顶点：V1当前颜色：black
顶点：V1发现时间：2结束时间：9父顶点：V2当前颜色：black
顶点：V2发现时间：1结束时间：10父顶点：null当前颜色：black
```



4）有向图深度优先搜索



```
顶点：V4发现时间：3结束时间：4父顶点：V5当前颜色：black
顶点：V5发现时间：2结束时间：5父顶点：V2当前颜色：black
顶点：V2发现时间：1结束时间：6父顶点：null当前颜色：black
顶点：V6发现时间：8结束时间：9父顶点：V3当前颜色：black
顶点：V3发现时间：7结束时间：10父顶点：null当前颜色：black
顶点：V1发现时间：11结束时间：12父顶点：null当前颜色：black
```


算法还是有改良空间，比如怎么控制生成树是最大子树。









