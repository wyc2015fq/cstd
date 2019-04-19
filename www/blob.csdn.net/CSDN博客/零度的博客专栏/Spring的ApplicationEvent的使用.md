# Spring的ApplicationEvent的使用 - 零度的博客专栏 - CSDN博客
2016年04月06日 12:16:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：389
Spring 3.0中提供了很多类似*Aware的类，其中ApplicationContextAware接口可以实现我们在初始化bean的时候给bean注入ApplicationConxt（Spring上下文对象）对象。ApplicationContextAware接口提供了publishEvent方法，实现了Observe（观察者）设计模式的传播机制，实现了对bean的传播。通过ApplicationContextAware我们可以把系统中所有ApplicationEvent传播给系统中所有的ApplicationListener。因此，我们只需要构造好我们自己的ApplicationEvent和ApplicationListener，就可以在系统中实现相应的监听器。
   下面以增加学生的示例来演示如何构造Spring的监听器，StudentAddEvent是监听的事件对象，StudentAddListener是事件的监听器（负责处理接收到的监听事件），StudentAddBean负责触发StudentAddEvent事件。具体步骤如下：
**1.  定义StudentAddEvent监听事件**
新建StudentAddEvent类，实现抽象类
org.springframework.context.ApplicationEvent
StudentAddEvent类中需要实现自己的构造函数，具体代码如下:
package com.trs.spring.event;
import org.springframework.context.ApplicationEvent;
/**
 * 增加学生的监听事件
 */
public class StudentAddEvent extends ApplicationEvent {
    /**
     * 
     */
    private static final long serialVersionUID = 20L;
    /**
     * 学生姓名
     */
    private String m_sStudentName;
    /**
     * @param source
     */
    public StudentAddEvent(Object source, String _sStudentName) {
        super(source);
        this.m_sStudentName = _sStudentName;
    }
    /**
     * 获取学生姓名
     * 
     * @return
     */
    public String getStudentName() {
        return m_sStudentName;
    }
}
**2.  定义StudentAddListener监听器**
新建StudentAddListener类，实现接口
org.springframework.context.ApplicationListener
中的onApplicationEvent方法，在该方法中只处理StudentAddEvent类型的ApplicationEvent事件，代码如下：
package com.trs.spring.event;
import org.springframework.context.ApplicationEvent;
import org.springframework.context.ApplicationListener;
public class StudentAddListener implements ApplicationListener {
    /*
     * (non-Javadoc)
     * 
     * @see
     * org.springframework.context.ApplicationListener#onApplicationEvent(org
     * .springframework.context.ApplicationEvent)
     */
    public void onApplicationEvent(ApplicationEvent _event) {
        // 1.判断是否是增加学生对象的事件
        if (!(_event instanceof StudentAddEvent)) {
            return;
        }
        // 2.是增加学生事件的对象，进行逻辑处理，比如记日志、积分等
        StudentAddEvent studentAddEvent = (StudentAddEvent) _event;
        System.out.println("增加了学生:::" + studentAddEvent.getStudentName());
    }
}
**3.  定义StudentAddBean触发StudentAddEvent事件**
新建StudentAddBean类，实现接口
org.springframework.context.ApplicationContextAware
中的setApplicationContext方法，在构造bean的时候注入Spring的上下文对象，以便通过Spring上下文对象的publishEvent方法来触发StudentAddEvent事件，具体代码如下：
package com.trs.spring.event;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.context.support.ClassPathXmlApplicationContext;
public class StudentAddBean implements ApplicationContextAware {
    /**
     * 定义Spring上下文对象
     */
    private ApplicationContext m_applicationContext = null;
    /*
     * (non-Javadoc)
     * 
     * @see
     * org.springframework.context.ApplicationContextAware#setApplicationContext
     * (org.springframework.context.ApplicationContext)
     */
    public void setApplicationContext(ApplicationContext _applicationContext)
            throws BeansException {
        this.m_applicationContext = _applicationContext;
    }
    /**
     * 增加一个学生
     * 
     * @param _sStudentName
     */
    public void addStudent(String _sStudentName) {
        // 1.构造一个增加学生的事件
        StudentAddEvent aStudentEvent = new StudentAddEvent(
                m_applicationContext, _sStudentName);
        // 2.触发增加学生事件
        m_applicationContext.publishEvent(aStudentEvent);
    }
    /**
     * @param args
     */
    public static void main(String[] args) {
        String[] xmlConfig = new String[] { "applicationContext.xml" };
        // 使用ApplicationContext来初始化系统
        ApplicationContext context = new ClassPathXmlApplicationContext(
                xmlConfig);
        StudentAddBean studentBean = (StudentAddBean) context
                .getBean("StudentAddBean");
        studentBean.addStudent("我是第一个学生");
        studentBean.addStudent("第二个学生已经添加");
    }
}
**4.  applicationContext.xml配置文件**
<bean id="StudentAddBean" class="com.trs.spring.event.StudentAddBean"></bean>
<bean id="StudentAddListener" class="com.trs.spring.event.StudentAddListener"></bean>
**5.  说明**
ApplicationContext在运行期会自动检测到所有实现了ApplicationListener的bean对象，并将其作为事件接收对象。当ApplicationContext的publishEvent方法被触发时，每个实现了ApplicationListener接口的bean都会收到ApplicationEvent对象，每个ApplicationListener可根据事件类型只接收处理自己感兴趣的事件，比如上面的StudentAddListener只接收StudentAddEvent事件。
**6.  执行StudentAddBean的main函数，结果如下：**
增加了学生:::我是第一个学生
增加了学生:::第二个学生已经添加
