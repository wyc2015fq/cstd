# ListView的不同item加载不同Layout - DEVELOPER - CSDN博客





2014年07月29日 11:21:40[学术袁](https://me.csdn.net/u012827205)阅读数：9804








在重写ListView的BaseAdapter时，我们常常在getView()方法中复用convertView，优化ListView以提高性能。convertView在Item为单一的同种类型布局时，能够回收并重用，但是多个Item布局类型不同时，convertView的回收和重用会出现问题。比如有些行为纯文本，有些行则是图文混排，这里纯文本行为一类布局，图文混排的行为第二类布局。单一类型的ListView很简单，下面着重介绍一下ListView包含多种类型视图布局的情形，首先看图效果展示：

[***点击下载该内容源码***](http://download.csdn.net/detail/u012827205/7690201)

![](https://img-blog.csdn.net/20140729111514156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140729111501395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



































在编写代码之前呢，我们需要做这些工作并了解其作用:


　　1）重写 getViewTypeCount() – 该方法返回多少个不同的布局


　　2）重写 getItemViewType(int) – 根据position返回相应的Item


　　3）根据view item的类型，在getView中创建正确的convertView

下面让我们看一下代码怎么实现：

MainActivity.class



```java
/**
 * 
 *ListViewDifferentType
 * @author Yuanjunhua
 *
 * 2014-7-28下午6:23:39
 */
public class MainActivity extends Activity {

	private ListView listView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		listView=(ListView) findViewById(R.id.lsitview);
		String[] name1=new String[]{"北京","上海","南京","郑州","轻工业学院","姜寨村"};
		String[] name2=new String[]{"北京","上海","南京","郑州","轻工业学院","姜寨村"};
		Integer[] id2=new Integer[]{
				R.drawable.jx_left_listitem_1,
				R.drawable.jx_left_listitem_2,
				R.drawable.jx_left_listitem_3,
				R.drawable.jx_left_listitem_4,
				R.drawable.jx_left_listitem_5,
				R.drawable.jx_left_listitem_6};
		
		List<String> list1=new ArrayList<String>();
		fillListMethod(list1,name1);
		
		List<Map<String,Integer>> list2=new ArrayList<Map<String,Integer>>();
		fillMapMethod(list2,name2,id2);
		
		Log.d("mapList", "mapList=="+list1);
		Log.d("strList", "strList=="+list2);
		listView.setAdapter(new MyAdapter(this, list1, list2));
		
	}

	private void fillMapMethod(List<Map<String, Integer>> list,String[] name,Integer[] id) {
		// TODO Auto-generated method stub
		for (int i = 0; i < name.length; i++) {
			Map<String,Integer> map=new HashMap<String, Integer>();
			map.put(name[i], id[i]);
			list.add(map);
		}
	}

	private void fillListMethod(List<String> list,String[] name) {
		// TODO Auto-generated method stub
		for (int i = 0; i < name.length; i++) {
			list.add(name[i]);
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
```



继承BaseAdapter



```java
/**
 * 
 *ListViewDifferentType
 * @author Yuanjunhua
 *
 * 2014-7-28下午6:23:31
 */
public class MyAdapter extends BaseAdapter {

	private LayoutInflater li;
	private List<String> list;
	private List<Map<String,Integer>> map;
	private final int TYPE_ONE=0,TYPE_TWO=1,TYPE_COUNT=2;
	public MyAdapter(Context context,List<String> list,List<Map<String,Integer>> map) {
		// TODO Auto-generated constructor stub
		this.list =list;
		this.map =map;
		li=LayoutInflater.from(context);
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return list.size()+map.size();
	}
	
	/** 该方法返回多少个不同的布局*/
	@Override
	public int getViewTypeCount() {
		// TODO Auto-generated method stub
		return TYPE_COUNT;
	}
	/** 根据position返回相应的Item*/
	@Override
	public int getItemViewType(int position) {
		// TODO Auto-generated method stub
		int po = position % 2;
		if (po == TYPE_ONE)
			return TYPE_ONE;
		else
			return TYPE_TWO;
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return list.get(position%6);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position%6;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder1 vh1=null;
		ViewHolder2 vh2=null;
		int type=getItemViewType(position);
		if(convertView==null){
			switch (type) {
			case TYPE_ONE:
				vh1=new ViewHolder1();
				convertView=li.inflate(R.layout.item_one, null);
				vh1.tv1=(TextView) convertView.findViewById(R.id.tv1);
				
				convertView.setTag(vh1);
				
				break;
			case TYPE_TWO:
				vh2=new ViewHolder2();
				convertView=li.inflate(R.layout.item_two, null);
				vh2.tv2=(TextView) convertView.findViewById(R.id.tv2);
				vh2.img2=(ImageView) convertView.findViewById(R.id.img2);
				convertView.setTag(vh2);
				break;
			}
		}
		else{
			switch (type) {
			case TYPE_ONE:
				vh1=(ViewHolder1) convertView.getTag();
				break;
			case TYPE_TWO:
				vh2=(ViewHolder2) convertView.getTag();
				break;
			}
		}
		
		switch (type) {
		case TYPE_ONE:
			if(position<5)
			vh1.tv1.setText(list.get((position%6)-(position%6)/2));
			if(position>5)
				vh1.tv1.setText(list.get((position%6)-(position%6)/2+3));
			break;
		case TYPE_TWO:
			int i=0;
			String txt=null;
			Map<String,Integer> mapSI=null;
			if(position<6){
				i=(position%6)-(position%6+1)/2;
				mapSI=map.get(i);
				Iterator<String> it=mapSI.keySet().iterator();
				if(it.hasNext()) txt=it.next();
			}
				
			if(position>6){
				i=(position%6)-(position%6+1)/2+3;
				mapSI=map.get(i);
				Iterator<String> it=mapSI.keySet().iterator();
				if(it.hasNext()) txt=it.next();
			}
			vh2.tv2.setText(txt);
			vh2.img2.setBackgroundResource(mapSI.get(txt));
			Log.d("txt", "txt=="+txt);
			break;
		}
		return convertView;
	}
	
	static class ViewHolder1{
		TextView tv1;
	}
	static class ViewHolder2{
		TextView tv2;
		ImageView img2;
	}

}
```
实现上图的效果就用这么几句代码！！





