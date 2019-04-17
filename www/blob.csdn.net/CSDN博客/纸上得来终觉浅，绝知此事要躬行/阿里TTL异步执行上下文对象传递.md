# 阿里TTL异步执行上下文对象传递 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 11:45:31[boonya](https://me.csdn.net/boonya)阅读数：2754










Github地址：[https://github.com/alibaba/transmittable-thread-local](https://github.com/alibaba/transmittable-thread-local)


![](https://img-blog.csdn.net/20170214123736542)


**验证对象异步传递示例：**



```java
package com.alibaba.ttl.threadpool.agent.demo;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

import com.alibaba.ttl.TransmittableThreadLocal;

public final class AgentDemo {

    static TransmittableThreadLocal<String> stringTransmittableThreadLocal = new TransmittableThreadLocal<String>();

    static TransmittableThreadLocal<Person> personReferenceTransmittableThreadLocal = new TransmittableThreadLocal<Person>() {
        @Override
        protected Person initialValue() {
            return new Person("unnamed", -1);
        }
    };

    static TransmittableThreadLocal<Person> personCopyTransmittableThreadLocal = new TransmittableThreadLocal<Person>() {
        @Override
        protected Person copy(Person parentValue) {
            // copy value to child thread
            return new Person(parentValue.getName(), parentValue.getAge());
        }

        @Override
        protected Person initialValue() {
            return new Person("unnamed", -1);
        }
    };
    
    private AgentDemo() {
    	throw new InstantiationError( "Must not instantiate this class" );
    }

    public static void main(String[] args) throws Exception {
        ExecutorService executorService = Executors.newFixedThreadPool(3);
        expandThreadPool(executorService);

        stringTransmittableThreadLocal.set("foo - main");
        personReferenceTransmittableThreadLocal.set(new Person("jerry - reference", 1));
        personCopyTransmittableThreadLocal.set(new Person("Tom - value", 2));

        printTtlInstancesInfo("Main - Before execution of thread pool");

        Future<?> submit = executorService.submit(new Runnable() {
            @Override
            public void run() {
                printTtlInstancesInfo("Thread Pool - enter");
                stringTransmittableThreadLocal.set("foo - modified in thread pool");
                personReferenceTransmittableThreadLocal.get().setName("jerry - reference - modified in thread pool");
                personCopyTransmittableThreadLocal.get().setName("Tom - value - modified in thread pool");
                printTtlInstancesInfo("Thread Pool - leave");
            }
        });
        submit.get();

        printTtlInstancesInfo("Main - After execution of thread pool");

        executorService.shutdown();
        if (!executorService.awaitTermination(3, TimeUnit.SECONDS)) {
            System.exit(1);
        }
    }

    public static void expandThreadPool(ExecutorService executor) throws Exception {
        List<Future<?>> ret = new ArrayList<Future<?>>();
        for (int i = 0; i < 3; ++i) {
            Future<?> submit = executor.submit(new Runnable() {
                @Override
                public void run() {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        throw new RuntimeException(e);
                    }
                }
            });
            ret.add(submit);
        }
        for (Future<?> future : ret) {
            future.get();
        }
    }

    static void printTtlInstancesInfo(String msg) {
        System.out.println("====================================================");
        System.out.println(msg);
        System.out.println("====================================================");
        System.out.println("stringTransmittableThreadLocal: " + stringTransmittableThreadLocal.get());
        System.out.println("personReferenceTransmittableThreadLocal: " + personReferenceTransmittableThreadLocal.get());
        System.out.println("personCopyTransmittableThreadLocal: " + personCopyTransmittableThreadLocal.get());
    }

    public static class Person {
        String name;
        int age;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public int getAge() {
            return age;
        }

        public void setAge(int age) {
            this.age = age;
        }

        public Person(String name, int age) {
            this.name = name;
            this.age = age;
        }

        @Override
        public String toString() {
            return "Person{" +
                    "name='" + name + '\'' +
                    ", age=" + age +
                    '}';
        }
    }
}
```

**测试结果：**




```
====================================================
Main - Before execution of thread pool
====================================================
stringTransmittableThreadLocal: foo - main
personReferenceTransmittableThreadLocal: Person{name='jerry - reference', age=1}
personCopyTransmittableThreadLocal: Person{name='Tom - value', age=2}
====================================================
Thread Pool - enter
====================================================
stringTransmittableThreadLocal: null
personReferenceTransmittableThreadLocal: Person{name='unnamed', age=-1}
personCopyTransmittableThreadLocal: Person{name='unnamed', age=-1}
====================================================
Thread Pool - leave
====================================================
stringTransmittableThreadLocal: foo - modified in thread pool
personReferenceTransmittableThreadLocal: Person{name='jerry - reference - modified in thread pool', age=-1}
personCopyTransmittableThreadLocal: Person{name='Tom - value - modified in thread pool', age=-1}
====================================================
Main - After execution of thread pool
====================================================
stringTransmittableThreadLocal: foo - main
personReferenceTransmittableThreadLocal: Person{name='jerry - reference', age=1}
personCopyTransmittableThreadLocal: Person{name='Tom - value', age=2}
```
**分析：**


-Before 初始化线程输出

-enter  线程重新设置传递对象的值，但并未提交，所以对象是一个默认的实例输出

-leave submit提交对象修改，对象值被修改传递赋值

-after 输出最终结果







