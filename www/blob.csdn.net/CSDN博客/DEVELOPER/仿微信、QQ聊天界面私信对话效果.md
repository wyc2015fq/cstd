# 仿微信、QQ聊天界面私信对话效果 - DEVELOPER - CSDN博客





2014年09月05日 10:21:48[学术袁](https://me.csdn.net/u012827205)阅读数：9293








我们大家可能都看到过在自己使用社交Android软件的时候，会发现聊天页面的效果；有人说是气泡效果，也很像，是吧？那我们该怎么去实现那种效果呢？？

[点击这里进入她的姊妹篇](http://blog.csdn.net/junhuahouse/article/details/38261919)


[*点击这里下载源码*](http://download.csdn.net/detail/u012827205/7866401)


![](https://img-blog.csdn.net/20140905094045406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这个就是效果：有人物头像、聊天的当时时间、聊天内容、发送信息等；其实呢，她的思路很简单，我也弄了几个小时！恩，那我们就简单详细的聊聊吧！先看源代码：项目入口MainActivity.java：

```java
public class MainActivity extends Activity {

	private LinkedList<PeopleStudentBean> sList = null;
	private LinkedList<PeopleTeacherBean> tList = null;
	private LinkedList<Bean> beans = null;


	/** 聊天message 格式 */
	private ListView listView;
	/** 信息编辑框 */
	private EditText edt;
	/** 信息发送按钮 */
	private Button btnEnter;

	private CustomAdapter adapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		sList = new LinkedList<PeopleStudentBean>();
		tList = new LinkedList<PeopleTeacherBean>();
		beans = new LinkedList<Bean>();
		String[] msg = new String[] { "你好！", "你也在金象工作吗？", "我在天安门扫大街呢，这里可舒服了！",
				"原来你也细化这个工作啊，我这里还招人呢，你来不？来的话，我一句话的事儿！", "呵呵，你好！", "是的，你在哪里呢？",
				"吼吼，这么便宜的事儿？！，我怎么没有遇到呢。", "恩，好啊 好啊。那等着我。。。" };

		// 0 是教师； 1 是学生
		for (int i = 0; i < 4; i++) {
			sList.add(new PeopleStudentBean(msg[i], R.drawable.you,"", 1));
			tList.add(new PeopleTeacherBean(msg[i + 4], R.drawable.me,"", 0));
		}

		// 归放到 同一个 类集合Bean中
		for (int j = 0; j < sList.size(); j++) {

			beans.add(sList.get(j));
			beans.add(tList.get(j));
		}

		setContentView(R.layout.activity_main);
		initViewsMethod();
		onHandleMethod();
	}
	
	/** 处理listView 的 item方法  */
	private void initViewsMethod(){
		listView = (ListView) findViewById(R.id.lvMessages);
		edt = (EditText) findViewById(R.id.edt);
		btnEnter = (Button) findViewById(R.id.enter);

		listView.setOnCreateContextMenuListener(new OnCreateContextMenuListener() {
			
			@Override
			public void onCreateContextMenu(ContextMenu menu, View v,
					ContextMenuInfo menuInfo) {
				// TODO Auto-generated method stub
				
				menu.setHeaderTitle("提示：");
				menu.setHeaderIcon(android.R.drawable.stat_notify_error);
				menu.add(0, 0, 1, "删除");
				menu.add(1, 1, 0, "取消");
				
			}
		});
		
	}
	
	
	/** 处理发送信息的方法  */
	public void onHandleMethod(){
		adapter = new CustomAdapter(this, beans);
		listView.setAdapter(adapter);
		btnEnter.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String txt = edt.getText().toString();
				if(null == txt)
					Toast.makeText(getApplicationContext(), "发送内容不能为空 !", Toast.LENGTH_SHORT).show();
				adapter.addItemNotifiChange(new Bean(txt, R.drawable.me, new Date()+"", 0));
				edt.setText("");
		        listView.setSelection(beans.size()-1);
			}
		});
	}

	@Override
	public boolean onContextItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		switch (item.getItemId()) {
		case 0:
			Toast.makeText(getApplicationContext(), "删除成功", Toast.LENGTH_SHORT).show();
			AdapterContextMenuInfo info = (AdapterContextMenuInfo) item.getMenuInfo();
			Bean bean = (Bean) adapter.getItem(info.position);
			beans.remove(bean);
			adapter.notifyDataSetChanged();
			break;
		}
		return super.onContextItemSelected(item);

	}
```
填充效果内容对应的自定义适配器CustomAdapter.java：
```java
public class CustomAdapter extends BaseAdapter {

	private LinkedList<Bean> beans = null;
	private LayoutInflater li;
	private final int ITEM_TYPES = 2, TYPE_0 = 0, TYPE_1 = 1;

	public CustomAdapter(Context context, LinkedList<Bean> beans) {
		// TODO Auto-generated constructor stub
		this.beans = beans;
		li = LayoutInflater.from(context);

	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return beans.size();
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return beans.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public int getViewTypeCount() {
		// TODO Auto-generated method stub
		return ITEM_TYPES;
	}

	@Override
	public int getItemViewType(int position) {
		// TODO Auto-generated method stub
		int tp = beans.get(position).getId();
		if (TYPE_0 == tp)
			return TYPE_0;
		else if (TYPE_1 == tp)
			return TYPE_1;
		return TYPE_0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub

		PeopleView s = null;
		Bean bean = beans.get(position);
		int type = getItemViewType(position);

		if (null == convertView) {
			s = new PeopleView();

			switch (type) {
			case TYPE_0:
				convertView = li.inflate(R.layout.listview_item_teacher, null);
				break;
			case TYPE_1:
				convertView = li.inflate(R.layout.listview_item_student, null);
				break;
			}

			s.time = (TextView) convertView.findViewById(R.id.Time);
			s.message = (TextView) convertView.findViewById(R.id.Msg);
			s.portrait = (ImageView) convertView.findViewById(R.id.Img);
			convertView.setTag(s);
		} else
			s = (PeopleView) convertView.getTag();
		s.time.setText(DateFomats.getCurrentTime(new Date().getTime()));
		s.message.setText(bean.gettMessage());
		s.portrait.setImageResource(R.drawable.you);

		return convertView;

	}

	class PeopleView {
		TextView time;
		TextView message;
		ImageView portrait;
	}

	/** 添加发表私信内容，更新列表 */
	public void addItemNotifiChange(Bean bean) {
		beans.add(bean);
		notifyDataSetChanged();

	}

}
```


我想大家从代码中也已经看出端倪了；她的实现思路是使用ListView，在ListView中对其item布局展示做出变动。也就是说，这个Listview中item要用到两种布局，然后通过用户的   Id 进行判断 ，不同 id 对应不同的聊天布局。然后按照时间的顺序展示在界面上(当然这个是后台提供的，你不需要进行什么作为，这里只是自己添加排布的)；为了实现的高效率，在创建item布局时候控件的两类布局的id使用同一套。这是关键的一点，这样的话在进行Layout布局初始化的时候可以使ListView得到优化。还要注意，要在适配器中重写方法：
```java
getViewTypeCount()
```
和重写方法：
```java
getItemViewType(int position)
```
上面第一个方法表返回值多少，对应的该ListView就会有有多少种的item布局类型，即在加载适配的时候每加载一次对话信息会走getView(int
 position ,View convertView ,ViewGroup parent);方法多少遍。如果按照原来的普通ListView适配器填充的话，现在的这个有两种item布局展示，为展示其对话效果在每走两次getView()方法 就相当于普通的ListView适配方法走一次，这样才能展示出我们想要的不同布局item的展示效果。第二个方法通过参数position对用户的id进行处理判断，并返回我们自定义布局类型或者说用户 id 所对应的类型数值值(这个是我们自己设置的),然后在getView()方法中通过方法获取不同布局类型对应的我们所设置的数值来进行判断此时我们应该展示那种itme布局，并来初始化该布局；

另外，ListView对应的item；如果想长按删除要实现其监听

```java
listView.setOnCreateContextMenuListener(new OnCreateContextMenuListener()
```
和监听必须实现的方法；以及重写回调的方法，对被点击的item做出处理
```java
@Override
public boolean onContextItemSelected(MenuItem item)
```
顺便提一下：
```java
menu.add(0, 0, 1, "删除");
```
       第一个int类型的group ID参数，代表的是组概念，你可以将几个菜单项归为一组，以便更好的以组的方式管理你的菜单按钮。
       第二个int类型的item ID参数，代表的是项目编号。这个参数非常重要，一个item ID对应一个menu中的选项。在后面使用菜单的时候，就靠这个item ID来判断你使用的是哪个选项。
       第三个int类型的order ID参数，代表的是菜单项的显示顺序。默认是0，表示菜单的显示顺序就是按照add的显示顺序来显示。
       第四个String类型的title参数，表示选项中显示的文字。











