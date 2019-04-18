# 关于Condition的个人理解 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月27日 00:28:56[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：40



Lock lock = new ReentrantLock();     //建个房子，通过lock()和unlock()锁住一个区域，称为同步代码区间. 可以理解为房子的防盗门

Condition l1 = lock.newCondition();   // 通知1

Condition l2 = lock.newCondition();    // 通知2

public void eat(){

        lock.lock();             ------上锁(只允许一个人进来)

        try{

        ..    

            while(food.count==0){

l1.await();        ----- 独食吃没了，没的吃了，把防盗门一开，让别人进了，然后自己睡觉去了。

            }

            ...                   ----- 进来以后就赶紧吃。 

               l2.signalAll();    ----- 一边吃着一边告诉厨师想着供应饭

        }catch{

        ..

        }finally{

            lock.unlock();       -----解锁（把门开开）

        }  

 }

public void cooking(){

        lock.lock();             ------上锁(只允许一个人进来)

        try{

        ..    

            while(food.count==storageMax){

               l1.await();        ----- 桌子放满了，没地方放了，活也干完了，把门一开休息去了，等着顾客唤醒

            }

            ...                   ----- 进来以后就赶紧做饭。 

               l2.signalAll();    ----- 一边做饭，一边告诉别人可以吃了

        }catch{

        ..

        }finally{

            lock.unlock();       -----解锁（把门开开）

        }  

 }

