# 添加自定义事件以及对osg事件队列的简单理解 - L_Andy的专栏 - CSDN博客

2016年04月04日 22:01:58[卡哥](https://me.csdn.net/L_Andy)阅读数：1822


**添加自定义事件**

首先osg在其内部通过osgGA::EventQueue类维护了一个事件队列，队列中的每一个事件都是一个osgGA::GUIEventAdapter对象（这个对象是用来存放所系统捕捉到的事件），在仿真循环阶段被系统捕捉到的所有事件都会被添加到这个事件队列中，之后会有一个事件遍历过程，在这个过程中实际上就是遍历这个事件队列，并且对事件队列中的每一个事件，它都会与我们在事件处理类中方法进行响应（如果事件处理类中没有对这个事件的处理则不会响应）。 

（此代码来自于王锐老师的Beginner‘s Guide中240页）

**[cpp]**[view
 plain](http://blog.csdn.net/u010133496/article/details/41155033#)[copy](http://blog.csdn.net/u010133496/article/details/41155033#)

- struct TimerInfo : public osg::Referenced  
- {  
-  TimerInfo( unsigned int c ) : _count(c) {}  
-  unsigned int _count;  
- };  


**[cpp]**[view
 plain](http://blog.csdn.net/u010133496/article/details/41155033#)[copy](http://blog.csdn.net/u010133496/article/details/41155033#)

- bool TimerHandler::handle( const osgGA::GUIEventAdapter& ea,  
- osgGA::GUIActionAdapter& aa )  
- {  
- switch ( ea.getEventType() )  
- {  
- case osgGA::GUIEventAdapter::FRAME:   
- if (_count % 100 == 0 )  
- {  
- osgViewer::Viewer* viewer =  
- dynamic_cast<osgViewer::Viewer*>(&aa);  
- if ( viewer )  
- {  
- //在这里，我们添加了一个用户自定义的时间，我们只需要在下面的时间类型中对USER类型进行判断即可对此事件进行处理，

**[cpp]**[view
 plain](http://blog.csdn.net/u010133496/article/details/41155033#)[copy](http://blog.csdn.net/u010133496/article/details/41155033#)

- //注意其参数是一个继承自Reference类的对象。
- viewer->getEventQueue()->userEvent(  
- new TimerInfo(_count) );  
- }  
- }  
- _count++;  
- break;  
- case osgGA::GUIEventAdapter::USER:  
- if ( _switch.valid() )  
- {  
- //同过getUserData方法，直接获取用户数据（我们在上面传入的继承自Reference类的对象）
- const TimerInfo* ti =  
- dynamic_cast<const TimerInfo*>( ea.getUserData());  
- std::cout << "Timer event at: " <<ti->_count<<  
- std::endl;  
- _switch->setValue( 0, !_switch->getValue(0) );  
- _switch->setValue( 1, !_switch->getValue(1) );  
- }  
- break;  
- default:  
- break;  
- }  
- returnfalse;  
- }  

在上面这个handle函数中，我们定义了对两个事件的处理，一个为FRAME事件，另一个为USER事件，在FRAME事件的处理中，首先判断_count值是否能呗100整除（在每一个FRAME事件中都会使_count值加一），即每过100帧就会执行一遍if语句下面的内容。在if语句中，它通过调用userEvent方法将事件加入到事件队列中。

![](https://img-blog.csdn.net/20141115232610843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDEzMzQ5Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上面的图片是userEvent函数的实现源码，我们可以看到在调用userEvent方法时，其函数内部的实现如上，首先设置时间类型为USER（即我们用此方法添加到事件队列中的事件的类型为USER），其次调用setUserData方法设置用户数据，之后设置时间，最后通过addEvent方法将其添加到事件队列中去。这样我们就能在事件处理的handle函数中对此事件进行响应了。

