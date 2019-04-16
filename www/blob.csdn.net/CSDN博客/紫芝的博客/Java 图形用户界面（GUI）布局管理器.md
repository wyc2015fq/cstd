# Java 图形用户界面（GUI）布局管理器 - 紫芝的博客 - CSDN博客





2018年11月11日 11:16:10[紫芝](https://me.csdn.net/qq_40507857)阅读数：262








组件不能单独存在，必须放置于容器中，组件在容器中的位置和尺寸是由布局管理器来决定的
- FlowLayout（流式布局管理器）
- BoderLayout（边界布局管理器）
- GridLayout（网格布局管理器）
- GridBagLayout（网格包布局管理器）
- CardLayout（卡片布局管理器）

1.FlowLayout

容器会将组件按照添加顺序，从左到右放置，到达边界时，自动换行

```java
import java.awt.*;

public class Example {
	public static void main(String[] args) {
		//创建一个窗体
		final Frame f=new Frame("Flowlayout");
		//设置布局管理器为FlowLayout,所有组件左对齐，水平间距20，垂直间距30
		f.setLayout(new FlowLayout(FlowLayout.LEFT,20,30));
		f.setSize(520,400);//设置窗体大小
		f.setLocation(200, 300);
		
		for(int i=1;i<30;i++)
		f.add(new Button("Button"+i));
		f.setVisible(true);
	}
}
```

2.BorderLayout

将容器划分为5个区域，分别是【东】EAST，【南】SOUTH，【西】WEST，【北】NORTH，【中】CENTER；

如果不指定添加区域，默认放到CENTER区，每个区域只能放置一个组件

![](https://img-blog.csdnimg.cn/20181111193523555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

```java
import java.awt.*;

public class Example {
	public static void main(String[] args) {
		//创建一个窗体
		Frame f=new Frame("BorderLayout");
		
		//设置布局管理器为BorderLayout
		f.setLayout(new BorderLayout());
		
		f.setSize(520,400);//设置窗体大小
		f.setLocation(200, 300);//显示位置
		f.setVisible(true);//设置可见
		
		Button[] but=new Button[6];
		//创建5个按钮，分别填充在borderLayout的5个区域
		Button but1 =new Button("EAST");
		Button but2 =new Button("SOUTH");
		Button but3 =new Button("WEST");
		Button but4 =new Button("NORTH");
		Button but5 =new Button("CENTER");
		
		f.add(but1,BorderLayout.EAST);
		f.add(but2,BorderLayout.SOUTH);
		f.add(but3,BorderLayout.WEST);
		f.add(but4,BorderLayout.NORTH);
		f.add(but5,BorderLayout.CENTER);
		
		f.add(new Button("每个区域只能放置一个组件"), BorderLayout.CENTER);
	}
}
```

3.GridLayout

将容器分为N行M列的网格，从左到右每个网格放一个组件，所有组件宽高相同

```java
import java.awt.*;

public class Example {
	public static void main(String[] args) {
		//创建一个窗体
		Frame f=new Frame("GridLayout");
		
		//设置布局管理器为BorderLayout
		f.setLayout(new GridLayout(3,5,10,30));//设置网格：行为3，列为5，,水平间距10，垂直间距30
		
		f.setSize(500,300);
		f.setLocation(400, 300);
		f.setVisible(true);
		
		for(int i=0;i<15;i++) {
			f.add(new Button("Button"+i));
		}
	}
}
```

4.Grid BagLayout

最灵活、最复杂的布局管理器，允许组件大小自定义，允许一个组件跨越一个或多个网格

```java
import java.awt.*;
class Layout extends Frame{
	public Layout(String title) {
		GridBagLayout layout =new GridBagLayout();
		GridBagConstraints c=new GridBagConstraints();//布局约束条件
		this.setLayout(layout);
		c.fill=GridBagConstraints.BOTH;//设置组件横向纵向可以拉伸
		c.weightx=1;//设置横向权重为1
		c.weighty=1;//设置纵向权重为1
		
		this.addComponent("button1", layout, c);
		this.addComponent("button2", layout, c);
		this.addComponent("button3", layout, c);
		
		//添加的组件是本行最后一个组件
		c.gridwidth=GridBagConstraints.REMAINDER;
		this.addComponent("button4", layout, c);
		
		c.weightx=0;
		c.weighty=0;
		this.addComponent("button5", layout, c);
		c.gridwidth=1;//设置组件跨一个网格
		
		this.addComponent("button6", layout, c);
		
		//添加的组件是本行最后一个组件
		c.gridwidth=GridBagConstraints.REMAINDER;
		this.addComponent("button7", layout, c);
		
		c.gridheight=2;
		c.gridwidth=1;
		c.weightx=2;
		c.weighty=2;
		
		this.addComponent("button8", layout, c);
		c.gridwidth=GridBagConstraints.REMAINDER;
		c.gridwidth=1;//设置组件跨一个网格
		this.addComponent("button9", layout, c);
		this.addComponent("button10", layout, c);
		
		this.setTitle(title);
		this.pack();
		this.setVisible(true);
	}
	//增加组件的方法
	private void addComponent(String name,GridBagLayout layout,GridBagConstraints c) {
		Button bt=new Button(name);
		layout.setConstraints(bt, c);//设置约束条件c和按钮关联
		this.add(bt);//增加按钮
	}
}
public class Example {
	public static void main(String[] args) {
		new Layout("GridBagLayout");
	}
}
```

5.Card Layout

在操作程序中，经常会遇到通过选项卡来切换程序中的界面，这些界面就像一张张卡片；可以使多个界面共享一个显示空间，某一时刻只有一个界面可见

6.不使用布局管理器

```java
import java.awt.*;

public class Example {
	public static void main(String[] args) {
		Frame f=new Frame("Frame without Layout");
		f.setLayout(null);//取消frame的布局管理器
		f.setSize(300,150);
		f.setVisible(true);
		
		Button button1=new Button("press");
		Button button2=new Button("pop");
		button1.setBounds(40,60,100,30);//左上角的X和Y坐标、组件的长和宽
		button2.setBounds(150,90,110,30);
		f.add(button1);
		f.add(button2);
	}
}
```





