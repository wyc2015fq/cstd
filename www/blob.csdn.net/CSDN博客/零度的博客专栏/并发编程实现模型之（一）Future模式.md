# 并发编程实现模型之（一）Future模式 - 零度的博客专栏 - CSDN博客
2017年11月10日 09:42:17[零度anngle](https://me.csdn.net/zmx729618)阅读数：289
多线程开发可以更好的发挥多核cpu性能，常用的多线程设计模式有：Future、Master-Worker、Guard Susperionsion、不变、生产者-消费者 模式；jdk除了定义了若干并发的数据结构，也内置了多线程框架和各种线程池；    锁（分为内部锁、重入锁、读写锁）、ThreadLocal、信号量等在并发控制中发挥着巨大的作用。本篇介绍第一种并发编程实现模型——Future模型。
**一、什么是Future模型：**
    该模型是将异步请求和代理模式联合的模型产物。类似商品订单模型。见下图：
![](https://img-blog.csdn.net/20140930160301156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG1kY3N6aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    客户端发送一个长时间的请求，服务端不需等待该数据处理完成便立即返回一个伪造的代理数据（相当于商品订单，不是商品本身），用户也无需等待，先去执行其他的若干操作后，再去调用服务器已经完成组装的真实数据。该模型充分利用了等待的时间片段。
** 二、Future模式的核心结构：**
![](https://img-blog.csdn.net/20140930160856140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG1kY3N6aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Main：启动系统，调用Client发出请求；
Client：返回Data对象，理解返回FutureData，并开启ClientThread线程装配RealData；
Data：返回数据的接口；
FutureData：Future数据，构造很快，但是是一个虚拟的数据，需要装配RealData；
RealData：真实数据，构造比较慢。
**三、Future模式的代码实现：**
（1）Main函数：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39696357#)[copy](http://blog.csdn.net/lmdcszh/article/details/39696357#)[print](http://blog.csdn.net/lmdcszh/article/details/39696357#)[?](http://blog.csdn.net/lmdcszh/article/details/39696357#)
- package tgb;  
- 
- publicclass Main {  
- 
- publicstaticvoid main(String[] args){  
-         Client client = new Client();  
- //理解返回一个FutureData
-         Data data = client.request("name");  
-         System.out.println("请求完毕！");  
- 
- try{  
- 
- //处理其他业务
- //这个过程中，真是数据RealData组装完成，重复利用等待时间
-             Thread.sleep(2000);           
- 
-         }catch (Exception e){  
- 
-         }  
- 
- //真实数据
-         System.out.println("数据 = "+ data.getResult());  
- 
- 
-     }  
- 
- }  
- 
（2）Client的实现：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39696357#)[copy](http://blog.csdn.net/lmdcszh/article/details/39696357#)[print](http://blog.csdn.net/lmdcszh/article/details/39696357#)[?](http://blog.csdn.net/lmdcszh/article/details/39696357#)
- package tgb;  
- 
- publicclass Client {  
- 
- public Data request(final String queryStr){  
- final FutureData future = new FutureData();  
- //开启一个新的线程来构造真实数据
- new Thread(){  
- publicvoid run(){  
-                 RealData realData = new RealData(queryStr);  
-                 future.setRealData(realData);           }  
-         }.start();  
- return future;  
-     }  
- }  
- 
（3）Data的实现：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39696357#)[copy](http://blog.csdn.net/lmdcszh/article/details/39696357#)[print](http://blog.csdn.net/lmdcszh/article/details/39696357#)[?](http://blog.csdn.net/lmdcszh/article/details/39696357#)
- package tgb;  
- 
- publicinterface Data {  
- 
- public  String getResult();  
- }  
- 
（4）FutureData：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39696357#)[copy](http://blog.csdn.net/lmdcszh/article/details/39696357#)[print](http://blog.csdn.net/lmdcszh/article/details/39696357#)[?](http://blog.csdn.net/lmdcszh/article/details/39696357#)
- package tgb;  
- 
- /**
-  * 是对RealData的一个包装
-  * @author limin
-  *
-  */
- publicclass FutureData implements Data {  
- 
- protected RealData realData =null;  
- protectedboolean isReady = false;  
- publicsynchronizedvoid setRealData(RealData realData){  
- if(isReady){  
- return;  
-         }  
- this.realData=realData;  
-         isReady=true;  
-         notifyAll();  
- 
-     }  
- 
- @Override
- publicsynchronized  String getResult() {  
- while(!isReady){  
- try{  
-                 wait();  
-             }catch (Exception e){  
- 
-             }  
-         }  
- return realData.result;  
-     }  
- 
- }  
- 
（5）RealData实现：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39696357#)[copy](http://blog.csdn.net/lmdcszh/article/details/39696357#)[print](http://blog.csdn.net/lmdcszh/article/details/39696357#)[?](http://blog.csdn.net/lmdcszh/article/details/39696357#)
- package tgb;  
- 
- publicclass RealData implements Data {  
- protected  String  result;  
- 
- public RealData(String para){  
- //构造比较慢
-         StringBuffer sb= new StringBuffer();  
- for(int i=0;i<10;i++){  
-             sb.append(para);  
- try{  
-                 Thread.sleep(1000);  
-             }catch(Exception e){  
- 
-             }  
-             result= sb.toString();  
-         }  
-     }  
- 
- 
- 
- @Override
- public String getResult() {  
- 
- return result;  
-     }  
- 
- }  
- 
**注意：**
    FutureData是对RealData的包装，是对真实数据的一个代理，封装了获取真实数据的等待过程。它们都实现了共同的接口，所以，针对客户端程序组是没有区别的；
    客户端在调用的方法中，单独启用一个线程来完成真实数据的组织，这对调用客户端的main函数式封闭的；
    因为FutureData中的notifyAll和wait方法，主程序会等待组装完成后再会继续主进程，也就是如果没有组装完成，main函数会一直等待。
