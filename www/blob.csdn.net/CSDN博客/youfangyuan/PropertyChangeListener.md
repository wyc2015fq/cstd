# PropertyChangeListener - youfangyuan - CSDN博客
2013年05月18日 17:12:04[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：572标签：[swing](https://so.csdn.net/so/search/s.do?q=swing&t=blog)
个人分类：[java](https://blog.csdn.net/youfangyuan/article/category/1310255)
[http://vakinge.iteye.com/blog/441908](http://vakinge.iteye.com/blog/441908)
接口 java.beans.PropertyChangeListener 
顾名思义监听属性改变事件，最近做一些东西需要用到数据绑定相关东西，用到了这个接口。 
这里做一个简单DEMO。 
```java
public class DemoBeans {
	private String demoName;
	
	PropertyChangeSupport listeners = new PropertyChangeSupport(this);
	public DemoBeans() {
		demoName = "initValue";
	}
	public String getDemoName() {
		return demoName;
	}
	
	public void setDemoName(String demoName) {
		String oldValue = this.demoName;
		this.demoName = demoName;
		//发布监听事件
		firePropertyChange("demoName", oldValue, demoName);
		
	}
	
	public void addPropertyChangeListener(PropertyChangeListener listener) {
		listeners.addPropertyChangeListener(listener);
	}
	
	public void removePropertyChangeListener(PropertyChangeListener listener){
		listeners.addPropertyChangeListener(listener);
	}
	
	/**
	 * 触发属性改变的事件
	 */
    protected void firePropertyChange(String prop, Object oldValue, Object newValue) {
        listeners.firePropertyChange(prop, oldValue, newValue);
    }
   
}
```
```java
public class Main {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		DemoBeans beans = new DemoBeans();
		beans.addPropertyChangeListener(new PropertyChangeListener(){
			public void propertyChange(PropertyChangeEvent evt) {
				System.out.println("OldValue:"+evt.getOldValue());
				System.out.println("NewValue:"+evt.getNewValue());
				System.out.println("tPropertyName:"+evt.getPropertyName());
			}});
		beans.setDemoName("test");
	}
}
```
