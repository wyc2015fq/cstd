# irrlicht引擎消息系统 - gauss的专栏 - CSDN博客
2011年12月16日 00:11:07[gauss](https://me.csdn.net/mathlmx)阅读数：488标签：[class																[Class																[destructor																[events																[processing																[引擎](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)](https://so.csdn.net/so/search/s.do?q=processing&t=blog)](https://so.csdn.net/so/search/s.do?q=events&t=blog)](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)](https://so.csdn.net/so/search/s.do?q=Class&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
说说irrlicht的消息系统，除了irrlicht绘制主窗体，默认的消息处理过程，外部的消息监听是通过IrrlichtDevice设备类中一个接口
virtual void setEventReceiver(IEventReceiver* receiver) = 0;
这个接口是把receiver监听类作物除默认窗体过程外，消息传递对象，
先看看看IEventReceiver接口里面有什么东东，
class IEventReceiver
{
public:
    //! Destructor
    virtual ~IEventReceiver() {}
    //! Called if an event happened.
    /** Please take care that you should only return 'true' when you want to _prevent_ Irrlicht
    * from processing the event any further. So 'true' does mean that an event is completely done.
    * Therefore your return value for all unprocessed events should be 'false'.
    \return True if the event was processed.
    */
    virtual bool OnEvent(const SEvent& event) = 0;
};
这是一个抽象类，用户可以写一个事件监听类来实现这个接口，处理用户关心的事件
例如
class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver()  { }
    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == EET_GUI_EVENT)
        {
        }
      return false
    }
};
具体消息流程如下图![](http://hi.csdn.net/attachment/201112/15/0_1323965458iZiN.gif)
要注意的是在MyEventReceiver类中，没有处理过的消息一定要返回，不然下面的GUI和场景系统会收不到消息.到时候你会感到很奇怪，为什么我GUI收不到点击的消息.
