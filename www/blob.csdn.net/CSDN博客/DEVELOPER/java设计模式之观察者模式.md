# java设计模式之观察者模式 - DEVELOPER - CSDN博客





2014年06月19日 22:03:14[学术袁](https://me.csdn.net/u012827205)阅读数：646标签：[android																[java设计模式																[观察者模式](https://so.csdn.net/so/search/s.do?q=观察者模式&t=blog)
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)

所属专栏：[JAVA设计模式](https://blog.csdn.net/column/details/15234.html)](https://so.csdn.net/so/search/s.do?q=java设计模式&t=blog)



最近在学习下设计模式，而加深学习的不错的方法就是把心得写出来吧。



        观察者<Observer>模式是软件设计模式的一种。在此种模式中，一个目标物件管理所有相依于它的观察者物件，并且在它本身的状态改变时主动发出通知。这通常透过呼叫各观察者所提供的方法来实现。此种模式通常被用来实作事件处理系统。




        看下结构图：

![](https://img-blog.csdn.net/20140619220418140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


```java
开始一个小例子

TheObserver<观察者>接口：

    public interface TheObserver {  
        void getNewPaper();  
    }  


实现TheObserver接口的观察者：

个人订阅者：
    public class PersonalSubscriber implements TheObserver{  
        private String strName;  
          
        public void setNewsPaperName(String strName){  
            this.strName = strName;  
        }  
          
        public String getNewsPaperName(){  
            return strName;  
        }  
      
        @Override  
        public void getNewPaper() {  
            // TODO Auto-generated method stub  
            System.out.println("我是个人用户，我得到了我的报纸："+getNewsPaperName());  
        }  
      
    }  


企业订阅者：
    public class EnterpriseSubscriber implements TheObserver{  
        private String strName;  
          
        public void setNewsPaperName(String strName){  
            this.strName = strName;  
        }  
          
        public String getNewsPaperName(){  
            return strName;  
        }  
        @Override  
        public void getNewPaper() {  
            // TODO Auto-generated method stub  
            System.out.println("我是企业用户，我得到了我的报纸："+getNewsPaperName());  
        }  
      
    }  


被观察者：TheObserved
    public abstract class TheObserved{   
        public List<TheObserved> list;  
        public Publish(){  
            list = new ArrayList();  
        }  
          
        public void registered(TheObserver theObserver){  
            list.add(theObserver);  
        }  
          
        public void unregistered(TheObserver theObserver){  
            list.remove(theObserver);  
        }  
          
        public abstract void sendNewsPaper();  
    }  


这里使用的是抽象类

下面是实现：
    public class PostOffice extends Publish{  
      
        @Override  
        public void sendNewsPaper() {  
            // TODO Auto-generated method stub  
            Iterator iterator = list.iterator();  
            while(iterator.hasNext()){  
                ((ISubscribe) iterator.next()).getNewPaper();  
            }  
        }  
          
    }  


测试：
    public static void main(String[] args) {  
            // TODO Auto-generated method stub  
            PostOffice postOffice = new PostOffice();  
              
            //得到个人用户  
            PersonalSubscriber person = new PersonalSubscriber();  
            person.setNewsPaperName("《南方周末》");  
            //得到企业用户  
            EnterpriseSubscriber enterprise = new EnterpriseSubscriber();  
            enterprise.setNewsPaperName("《企业报》");  
              
            //注册观察者  
            postOffice.registered(person);  
            postOffice.registered(enterprise);  
              
            //发放报纸  
            postOffice.sendNewsPaper();  
        }  


测试结果：

    我是个人用户，我得到了我的报纸：《南方周末》  
    我是企业用户，我得到了我的报纸：《企业报》
```](https://so.csdn.net/so/search/s.do?q=android&t=blog)




