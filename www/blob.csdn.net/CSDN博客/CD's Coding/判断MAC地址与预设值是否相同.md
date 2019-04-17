# 判断MAC地址与预设值是否相同 - CD's Coding - CSDN博客





2014年10月03日 15:57:38[糖果天王](https://me.csdn.net/okcd00)阅读数：716











一直特别蠢的在寻找如何确定此时获得的ｍａｃ地址是否和事先预定的相同的方法……

原来是不可以用＂＝＝＂，应该用 equals……

==是地址相同…… equals是值相同……

哭……






```java
/* 显示列表 */
	protected void showDevices()
	{
		List<String> list = new ArrayList<String>();
		int flag=0;
		for (int i = 0, size = _devices.size(); i < size; ++i)
		{
			StringBuilder a = new StringBuilder();
			StringBuilder b = new StringBuilder();
			BluetoothDevice d = _devices.get(i);
			a.append(d.getName());
			String s = a.toString();
			b.append(d.getAddress());
			String mactmp=b.toString();
			//b.append('\n');
			//b.append(d.getName());
			String cat01mac = "40:98:4E:20:4B:E3";
			String basemac="00:18:31:3E:35:3C";
			if(mactmp.equals(cat01mac)){s = s + " is INEYE now"; list.add(s);flag=1;}
			else if(mactmp.equals(basemac)){s = s + " is INEYE now";  list.add(s);flag=1;}
		}
		if(flag==0)list.add("No cats in eye now");
		
		final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, list);
		_handler.post(new Runnable() {
			public void run()
			{

				setListAdapter(adapter);
			}
		});
	}
```











