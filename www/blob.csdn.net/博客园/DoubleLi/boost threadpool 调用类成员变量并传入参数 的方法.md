# boost::threadpool 调用类成员变量并传入参数 的方法 - DoubleLi - 博客园






1. 首先到官网下载



[http://threadpool.sourceforge.net/](http://threadpool.sourceforge.net/)



2. 包含头文件



#include"../boost/threadpool.hpp"







3. 声明threadpool对象， 




boost::threadpool::fifo_poolm_poolCmdProcess;



上面声明了一个FIFO线程池， 即先进先出



4. 声明一个Runnable适配类 来包装你的类及成员函数




classRunnable

                {

typedefboost::function<void(/*CCommunicationMap*, */ICommandContextEx*)> function;



private:

                                function_f;

CCommunicationMap*_target;

ICommandContextEx*_data;



public:

template<classF>

Runnable(CCommunicationMap*target,Ff,ICommandContextEx*data)

                                {

_f=f;

_target=target;

_data=data;

                                }



                                ~Runnable(){}



voidrun()

                                {

//_target->TestCommand(_data);

_f(/*_target, */_data);

                                }

                };





上面 function 声明了一个函数模板， 此模板应该和你要关联的类成员函数类型一致。



5. 调用threadpool的schedule方法 启动线程







boost::function<void (/*CCommunicationMap*, */ICommandContextEx*)>fun;


fun= boost::bind(&CCommunicationMap::TestCommand, &m_communication,_1);





Runnable*run=newRunnable(&m_communication, fun,pContext);



boost::threadpool::schedule(m_poolCmdProcess,boost::shared_ptr<Runnable>(run));





注意： 

threadpool库中的pool_adaptors.hpp头文件有错误， 需要我们改动源码（注释的为原来代码，下面的为改动后的代码）， 具体为：


template<typenamePool,typenameRunnable>

boolschedule(Pool&pool,shared_ptr<Runnable>const&obj)

    {          

//return pool->schedule(bind(&Runnable::run, obj));

returnpool.schedule(bind(&Runnable::run,obj));

    }          













