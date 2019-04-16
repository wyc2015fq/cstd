# Java 学习笔记15：关于web开发中，全局变量的一点想法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月05日 10:22:39[initphp](https://me.csdn.net/initphp)阅读数：6035
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









> 
在PHP开发中，我们经常习惯于在php类中定义很多变量。这是因为php是解析型的语言，在做web开发的时候，一次解析就释放内存。但是Java可能就有些不同，Java是编译型的语言，web容器运行后，类的对象就会常驻在内存中，这个时候如果类的对象中有定义公用变量，那么就会不同的人访问web程序导致这个变量造成冲突。




可以看一个简单的例子：



```java
public int i = 0; //定义一个初始化变量
	
	/**
	 * 列表
	 * @return
	 */
	@RequestMapping(value="/list")
	public ModelAndView list() {
		ModelAndView modelAndView = new ModelAndView();
		List<GuestbookDo> lists = guestbookService.findGuestbooks();
		modelAndView.setViewName("test/list");
		modelAndView.addObject("userList", lists);
		this.i++; //i自增
		System.out.print(i); //控制台输出i
		return modelAndView;
	}
```


我们定义了一个i的变量。常规模式下PHP中，我们在浏览器中请求一下http://127.0.0.1:8090/test/list/地址，则控制台会输出1，然后再请求一次还是1。



但是在Java中，我们可以看到结果：

![](https://img-my.csdn.net/uploads/201212/05/1354673416_8097.jpg)

所以，对于Java来说，i的变量是在内存中常驻的，如果定义在类中，那么就会变成共享变量，如果你的程序中需要用到这个共享变量，那么可能就会造成另外一个人的访问覆盖你这个人的i变量的结果。

那么如果，这个i在方法内呢？方法内是局部变量，方法调用结束就会释放内存，我们还是看下实例：



```java
/**
	 * 列表
	 * @return
	 */
	@RequestMapping(value="/list")
	public ModelAndView list() {
		ModelAndView modelAndView = new ModelAndView();
		List<GuestbookDo> lists = guestbookService.findGuestbooks();
		modelAndView.setViewName("test/list");
		modelAndView.addObject("userList", lists);
		int i = 0; //局部变量
		i++; //i自增
		System.out.print(i + "\r\n"); //控制台输出i
		return modelAndView;
	}
```


结果：



![](https://img-my.csdn.net/uploads/201212/05/1354673621_5519.jpg)

结果显而易见，每次请求，局部变量都会自动释放内存。

我们再做web开发，都会定义一些数据结构的对象用于存储传递进来的数据或者要经常使用的数据，例如：



```java
/**
 * guest_book 数据结构
 * @author zhuli
 */
public class GuestbookDo {
	
	private String title;
	
	private String content;
	
	private String username;
	
	private int id;
	
	
	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public int getDtime() {
		return dtime;
	}

	public void setDtime(int dtime) {
		this.dtime = dtime;
	}

	private int dtime;
}
```


其实这个类不能做成像Service一样的单例，一旦单例了，数据就变成共享数据，不同的数据进来都会互相覆盖。



所以都是需要的地方new一个数据结构对象，反正也是局部变量，运行完毕就释放了。








