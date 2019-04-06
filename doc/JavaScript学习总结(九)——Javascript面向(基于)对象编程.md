## [JavaScript学习总结(九)——Javascript面向(基于)对象编程](https://www.cnblogs.com/xdp-gacl/p/3698862.html)

## 一、澄清概念

　　1.JS中"基于对象=面向对象"

　　2.JS中没有类(Class)，但是它取了一个新的名字叫“原型对象”，因此"类=原型对象"

## 二、类(原型对象)和对象(实例)的区别与联系

　　1.类(原型对象)是抽象，是概念的，代表一类事物。

　　2.对象是具体的，实际的，代表一个具体的事物。

　　3.类(原型对象)是对象实例的模板，对象实例是类的一个个体。

## 三、抽象的定义

　　在定义一个类时，实际上就是把一类事物的共有属性和行为提取出来，形成一个物理模型(模板)，这种研究问题的方法称为抽象。

## 四、JavaScript面向对象三大特征

### 　　4.1.封装

　　封装就是把抽象出来的属性和对属性的操作封装在一起，属性被保护在内部，程序的其它部分只有通过被授权的操作(函数)，才能对属性进行操作！

**封装的范例：**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <script type="text/javascript">
 2     /*定义一个Person类*/
 3     function Person(_name,_age,_salary){
 4         //Person类的公开属性，类的公开属性的定义方式是：”this.属性名“
 5         this.Name=_name;
 6         //Person类的私有属性，类的私有属性的定义方式是：”var 属性名“
 7         var Age=_age;
 8         var Salary=_salary;
 9 
10         //定义Person类的公开方法(特权方法)，类的公开方法的定义方式是：”this.functionName=function(){.....}“
11         this.Show=function(){
12             alert("Age="+Age+"\t"+"Salary="+Salary);//在公开方法里面访问类的私有属性是允许的
13         }
14         /*
15         定义Person类的私有方法(内部方法)，
16         类的私有方法的定义方式是：”function functionName(){.....}“，
17         或者 var functionName=function(){....}
18         */
19         function privateFn(){
20             alert("我是Person类的私有函数privateFn");
21         }
22 
23         var privateFn2=function(){
24             alert("我是Person类的私有函数privateFn2");
25         }
26     }
27     /*通过prototype给可以类的所有对象添加公共(public)方法，
28     但是这种方式定义的方法不能去访问类的私有属性和私有方法*/
29     Person.prototype.Fn=function(){
30         alert("访问公共属性this.Name="+this.Name);//访问公共属性，OK的
31         //alert("访问私有属性Aag="+Age);//访问私有属性，这里会报错“Age未定义”
32         //privateFn();//调用私有方法，这里会报错“缺少对象”
33 
34     }
35 
36     var p1 = new Person("孤傲苍狼",24,2300);
37     alert("p1.Name="+p1.Name);//访问公有属性，这是可以正常访问的
38     alert("p1.Age="+p1.Age+"\t"+"p1.Salary="+p1.Salary);//不能使用类的对象去直接访问类私有属性，这是访问不了的，结果都是undefined
39     p1.Show();//调用类的公共函数，这次允许的
40     p1.Fn();//调用类的公共函数，这次允许的
41     //alert("p1.privateFn()："+p1.privateFn()+"&nbsp;p1.privateFn2()："+p1.privateFn2());//不能使用类的对象去调用类的私有方法，这里会报错”对象不支持此属性或者方法“
42   </script>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

###  　　4.2.继承

**继承范例：**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <script type="text/javascript">
 2     /*定义Stu类*/
 3     function Stu(name,age){
 4         this.Name=name;
 5         this.Age=age;
 6         this.Show=function(){
 7             window.alert("我的名字是："+this.Name+"，今年："+this.Age);
 8         }
 9         this.SayHello = function(){
10             window.alert("Hello，"+this.Name);
11         }
12     }
13 
14     /*定义MidStu类*/
15     function MidStu(name,age){
16         this.stu=Stu;//MidStu类继承Stu类
17         this.stu(name,age);//JS中实际上是通过对象冒充来实现继承的，这句话不能少，因为JS是动态语言，如果不执行，则不能实现继承效果
18         /*
19         从父类继承下来的公共方法，可以根据实际情况选择重写
20         */
21         //在子类MidStu中重写父类Stu的Show方法
22         /*this.Show=function(){
23             alert("MidStu.Show()");
24         }*/
25         //在子类MidStu中重写父类Stu的SayHello方法
26         this.SayHello=function(){
27             alert("你好，"+this.Name);
28         }
29 
30     }
31 
32     var midStu = new MidStu("孤傲苍狼",24);//创建一个MidStu类实例对象
33     alert("访问继承下来的属性Name和Age，midStu.Name="+midStu.Name+"，midStu.Name="+midStu.Age);//访问继承下来的属性
34     midStu.Show();//调用从父类Stu继承下来的Show方法
35     midStu.SayHello();//调用从父类Stu继承下来的SayHello方法，SayHello()在子类中进行了重写，这里调用的是重写过后的SayHello()方法
36   </script>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

运行结果：

![img](data:image/png;base64,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)![img](data:image/png;base64,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)![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAJ8AAAB5CAIAAACoZGQoAAAYVUlEQVR4nO2de1xUdd7HPzOjoJaWkoYmmHnBe0J5ScBLKJYwCiIKCogXFMl0NbY2N+tpWZ8ydV/Z5la7z27qppslmWab29PTru1mJiqIgIggc78DMwMDgsD3+eMMZw4zZ0YExtFxzuv9Oq/f+Z3vnN/lfX6/czhndEDtF8RcEMQWCGILMP8K5l/BglIrcRIkSLFcgTQ1sxas0iFNi+VqpGiRpkVKG0l691PtXpI17sW+Le1bF8+gRLweMWrEKxGvRIwa8XrE6BGjRowc0QpEKxAjR4waMWrE6BGtw6xCe5tsipHqt77Of2tLwDYa8hsa8hsasYNC9lDobgp9n2bspRl7KXIfRe6jWR84hQlwKy5K7xbufBMi91n7ll07I3S3PSN20IgdFLCN/DY2Y1YhQs/Y2xXEFghTtX7r64QbTf7ZrQHbiBU8fCcN30khe2j8uzZC37fCzfTRQUL2WOFuMpKY4cT0OcuIHe02GS8sjKyAbeS/ucV/cwuSahFeaLOLOecQJ0NCmSBDL8jQY30Vg3CjSbTJItrc5J/dipdaRFtJtJWYtH92K5vJJJhMvy2t/lutkfcugo2tboUpguk9pusYBFlNwoxGUWadKMMkSLdgpRnLLVhpxgqzIN1izUmvtpJiQHo1VpqRqLNO6SkGpBiwRI6oy4gs59iNuYDllTbS1FitZUwLN5oYRJsszhBuNIky62xkmHzcHmsMojUG0Sq9YJVOmKoVpmqxXI0kFZJUSFRaWSK3kiBFghRxEsRJBOJyLCjHglLbfVLUZUzLQ+DnRASEncaCcsRcQEKZFY5g1rGVDWYbbOb6KkGGvt055Xa09zzL1fYkqZCo7LlUvu73WqXhJt3msu4dSc/ZFxB5HtPyEHYaYacxcD+sqTnnBLEFzuyy2BkVZOixWmu9eU7jVJo56Xw4gx2F3EEZJ0OcZN1edZO0XL/rJe2v09VbElVZYuW65xSrnlWsmatYOVOxcqY8JVyeEi5b+pRs6VOyhMkS8ZjyecMKEp6uKS1c91YlQs8g7DTGfYPRRzFwPzDyMJ9dBa9d1ihHqhpp7U9A7mTiJtjecRPM1HcHYIuLkzHTLBEZ9rxc89d3jYffr/7jDv2eX+re2qR7M1P7xjrta6s1r67UvJqm+WWyeutS1aY4ZebzivRZksSwgugRf4+ZrNQ1Wu0+8Q+MPIx+24HAzxF2GpHnhfPs7QpW6awWWcerdLbMtPYzjOOJee9yB7zGSRAna/MqYy+iRKR9I8N45APT0T9W/3mnYe82/a6tuh0vat/M1LyeoX1ttfbXqzSvpGqyk1WbFyuzYhVroqTJ0woXjPnsqYFEhNAzmHCWxy7mX2k3cNO0jEsG6x9Oq3TCVM5U7DAbW2/Eb384dvyD1sj2PriZvAG8+c4iuRqsMd2sVuaABDESxEiISJOdVP3nnTUH9lTte0P/zhZdTpZ2+1rNr9LULyWrNy9RvRivzIxRZi5QrJmrSJ8lT5khWfp0Yey4o1MDiQjT8jDhLB4/y7EbegazCq0PpJbIbY+fOIKZRvYJzGHu6ISpWsexy8b0CcxxIcb1cruRbDBbLrvpugLcSN5z65bldp54ZTu1YjnEctauevNiw95XDb9/Tf/OlobzPxCRemuy+sUEZaZYsTZasXKOfMUMefJ0Iqr931xZYmhlwuTLCyfkzggiIkwttNodfRT9tgNDvsO0PIQXYkE5EqRIVDJ2b9m5thv35eqOOLDTYNezdoaQqGTS3EhHHB3w5nDhHtNZ0czDP8dy2Rzr08Guw3htb1eVJda/tUn/zi90OVls9ZSZYsXa+YqVs+UrZsiWPs3myxZPrFw86bLYwa517A75DlMLMbscC8qtd3HJGmbsupDBZFoFJ2t4Bdj6wvlocLR7u6PH2XHsKuD4QcfzpoNj19nMdHtj17ldZUa09s1MXU6W9vUMzStpbLmKVVGK1Eh50lQ2R7YwRBY3ThI/4bJ4XO4zQ4mxO7ag/dhl7cYr2+wamD9emaOI1hgc+06UamCxNZtzz+zYF7wmXHScXe+7OGO4x3Q2RXMF85bOa9d16V11bFPbzq5mW7rmtdWabema7BXqTUvYGspTZtjUikfKYp6QiUdJ4sYWxY794pnHiAhTSp3YjZMhScUMXGe9b7fY2eVd+gTmIFHHYGeCN8EGO8Zzc5zFsJG8wW3ibbsctNmX7rpFXYIdwTFq1jERyVMjNNnJmldSNS+nqrcmqzYnKF9YaFcHRXyIYtEIeezjcvET0kWjiheMOjZtMDF2meE77hv02w48fhbTSxGtQLzS+sQyxYD0asdzltqfyNwRzLvX1hdt77Zc9JdjMBvv+iRwdiheuy6cOYt0Mna7xa6+PUqI5UQkT56mfjFes3WZ5qVk9ZZlqk2LVVli5fr5bD2VyyYoE0Yr455QioOV4mGyhcNLnh9+bMogIsKMq9bhO/ooAna1t9umFiutj61FGSa7LvBbX+e3vk6UYRKl2+C4sb0ldczk9qOzWxveI/DY7UBMR4IdTkT70m9ZyU6SaLLBcUxEsiVhyg0x6l8sUW9ZqvlFonrzYtVGsV0d1Imj1YuHqxYOVYmHymOHlkQP/fKpAUSEcAmmlGJECcZ9g4BdwLBCRJYjWoHENrsrzYJ0i2hVvWBto113tBO8qp7FWUdY+2KFmYHbp45pu2BuPI8w5zFYYbZ31hbmWCJ/VZNqGZxVlQ3oBmyOjYg3EpF00XjFmrmqFxaqNy9Wb05QbYq3Sc2IZBKtFpNmyXBN3GNqcaBiQeCVZwedCH2IiDBbinAJJpVj3DcY+B4wrBDhEkQrkKhj1GKlWbC2UZjR6NgqNuGX1SjKbBJmNAozGlm7zmZmZhoQpFvscpgw3l0sjias187lFgYeuw67HDftKswbaRXJyXFMdwPtHRORJGaUPC1Suf451caFqhfjbGozn1VnRGhW2+6ZtXGDNbGDFPMDSmc9fHxiHyLCHA3CZRy7I0oQLkG0DvF6q9p0W/OsF9cXbrKbfhub2b2izCbGsa130htZHDN5w1zv4tVARHYB3ArbZbLHdAzg3eXs43bnGW8rOkoKB9ZxYh0S64iocl6wLGmqYvWzyvXPs8WpMuep189Rrw3XrJ6iS5toE7ygv2pu3yvhDxwf70dEiNZZBYed5tiNUSNJz87JwoxGtkP9NjazRvsE5jBfAGD3CrNahVmt5HzpE5gjWNvI4JjDzefusjuCi4Owm87yHXN40467XEzg1vkjvbmzMJqb28lOvoGkBiK6PjtQGj9JnjJDsfpZy+mviUiVEaXKmKNaG6lePV2THqZNGadLHklEDacOaJ/rp5zd+8rUnl+GCIkI82swR4PpOkw4y87MMsTokWS9mRJmNIoym0Qv3BS9cNNvY7P/5ha/La1+W1qZJvlnt/pvteb4bWkVbSLRJhJsbLWblrkwZwCDYw43n7vL2REcD8JuOsvnzXFM220iowUZLY7NYXd1D6zylEakNBJRRcQjkphR0iWh8hXPKNIiFSsjFWnhyrRnlGnTVKlh6hUTNMtGaZcM08YN1i7or5n7gCKyZ8lTwmOjBVa70TpM1yH0DAa+BwzJZ+2yA5ex67e5iVXrv9X6bRsWq132CyubbDjK8+EIr2MiKp3W71rU0IrYMdcXP1mZ+JRk2dOSxFDJ0lDp0kmyxHGKhFGKuGHK2MHK5wKUUX0VM/0l04WXnsRnI0BEiK3F/BpM17XNzO3tMoKZCyozcP03t/hvbXG061qwW/G4GPcJVla1XHh+zPnpAednPpY/d3j+/FH580fnR48smD/iUvTwwnnBxfMeK352UMnsgOLIfiXP9C6e0uPSZMEP4/B5xDB7uwG72tllvp3FDF9mBPtlNdo75kzLvGo93mX3CrxT9LoPaqtLCr6eO+bw+L5HJ/f/PHTAkbBHjkwecDQsIDcs4IvQh49P7ntiUp+TE3sdH+d3fIzoZIjgi5E4GjFMV5S/7vcmXrsS1i6ztj6mWFUvyqyzOmbY2MzAXJXtYCrNjHu30p2XPQ/S7g7Lem/Vc0X9uj+Yee/mXCxKQ/O6d009F5lt1117u2I5lsjbvmJhe2hl+94e91uPmU2CLHuQ0cLcnXb+T4WO0umb1du6p3UbKY3W26jkG9Y/h5bVI6kBSQ1YVo/EOsTXWddMgiXeiNgaxBute2NrrYPVDnu74RLESBB5HlH5grkFwnkFwnkF7D8zEYjLGRAngVhue+3s+DUD7iute5doiXuJkSDGSSlR5ZhdjtnliGxjZpl1c3YbTM7MMsy4ihlXMb3U+nh5eimmlCJc4sRuVD6i8gVR+YK5BaxjXpgAwdwCARvP+aC7YerpPgTux431Z55EMvfM/k+XDcqq7pVYYdCW+vAOEF6G4UcQsNtn1wvx2fVmbHZ7++x6HYgsR9B+n13vxGfXm7HZ7RV61WfXy/DZ9WZ8dr0Zm13/0FKfXS8DkeUI9tn1Utxot0p3wWz8yOMtvJ9xl91qwz9aWx8i8m9qDFdIL3u8nfcnbrJbVFuLGw0gAhHy8kZ8sO+Ex5t6H2Kz26v77JqqE3U6tDSJiCYwguMTFr399lGPt/Z+AzPLEXSge+3m6dTQa9Ha8ihRJrX2JcKZHwcOCNh24dxZjzf4vqL77ZoM6UopzFUgCiQKJRreZAERFixYkrnhjx5v8H2FzW53XXe1cmgVaKzrQdSfqDfRmNYbIMI/vw184KGXiwvPe7zN9w+IrEBw99k16VaqK2HSgKgfUW8iEA0lGkR1oFY8/9ySTZsOerzN9w/daVevKdbJhFoJyCIiCmhtEP37lJCoD9EIuiEgwsljQQMDXy8qPOfxZt8n2Ox2/bpr1GapylCnAlG/vx3uDwgAAL3kshFEAc1G0E3Mnp30y5fdPnwB2CVcBHCX2yqC91O8OXY14S3IRQBvczpSW+5bhK7a1VZAVwEyiogCAD+gdx9MBkJTxJOIgqkW1IQTnz4eMPCN8rL8O2DXRftv2b+d08x7ijjutZPkenFsUcfPyG6za1JvUF2BUcJccR8ABMAjgZgLRMVFjCcaSDSIDKBazJmV/Nr2T92t1lm/O+uaTqvlXfNWxnVBLg7i4ggdt9ulN4D6qw9oStGkEREFEAkf7uMHBA3ETGDOi4lPEImIRlIVqAGH/jQ6+PEcNw1f3vFxy47uitqO2HV2erkQ7Nr9bYzd4C7bNap+pSyE6Rqovh/z+GL5wgeBJ4N7LRginH/odwFEIAqmxoE3lSAznpy8+r/fOuIOtVzBzszdMbuOU6sLhS7q7OKzd8KurriHpgg3lD3oZgAZRUQ4+VcBMHHa8MzIkEXy80JqADU8SDSK1CATPn5/7NCgHXJJYbcL7mCnc3u/40PBsaCOzMy8RXQwzFlxtz12/cM6abdG8YriIqqKQdV9qb4vaUAtKPgOwBNxs9+bG7qQmkBVIC2IgskSQDI0ShEWlr7nd90/fG9pl3e8uugjZz3oOC5dHNlFDq9vrkVnbeEtzsFuRVftai8HqC6i9qqALA+TriepQCaQBiGPjtyw4tu1cWFkAWlBClBdP6LgpuugGuzbOWn0xBxpZfe/HHSt00Vv3pZdZ6eRC0+ufbioT0ccu8WuUfGW4hy0+WhW9iJzX1KAlCANiJA0Nxh47bO9g6gJpALJQeoeRMGtml43roHkGBmSse/9z91hl7en7DLvsF1nAS7OA97J+ZbH4bfbub+Iqor6qfNgLgZVP0y6XiQDyUEKUCt2/zoIyLr4hR81ghQgKUgKahhEjY81XgFp8YedE8dMzNGqSrrdrotN3t5x4c+1+FuKcTbaXNTqlvW8nbHbhfe7JvnbqnMwXECz1J9M/UkqoEpQJUgKMuHM5w/6I8ZwTkg1ICmoElQBUvkTjSSZH1WgthjjJ6z+6MMv3WTXbj500Ym3HAQuxLueZu3CXE+/jgtvJW9j4umK3epLgzU/o7YQVNWf1L2pAlQBug6qBClx44oo6OEoqgRpQNdh3VsOahhMjUFNRSAN3v2v0NBpb2mU3Tx8fdjbvd3rrlGWo/kJVefQWvkgVQ+gCgFdA10DlYMqQFKQAv/8eAApQVJQOegaqAJUCpL5EY0lxQPNJaAKjBm79n/+5Ptezl1mV/9TT/WPaCgQkuERkvemq6CroDLQNdB1kAFUBbKA9CANqLxt11XQFZB5MLWMaikCyfCXnWNDntzR7VdfH4ZOz8w1kt8q/gnDGbRe60Oa/lZnV0CloDKQDj9/6gf0B/xDBj/eXAySgq6CSkFXQMWgchE1j6LKPnQZlouYOHHNu+995vG+8D4ws7wzbwCr8oJU/4LpZ5D8Yarwo2JQEagEVAIqA2kwcnB/AMBgIOzV1OFkBF0BlYCK29A+QtWPNZ4HSfHb7KmTn96hUhR7vDu8jM7YrZZ/qvw/aE+j+ZI/yftSCegyqKiNUpAEQA8gIHTIquAHNm9fnUpVoJK2gMugy6CynmQMai7xa74I808YOHjTwf3HPN4dXgbXbkevu8aSFbJT0P8LVNSDrvagEkE7rgpIDfGMQODRR3uu+f4EnfrzIlIyY7ctplhAJQKSPEjF/pazoCtIWrhwZtQOj3eHl9EZu9qfQq5/DeW3MP1HUP8TLD8JLD8LLD8LmETDOQFdAl1Fzobhr6xa+tUHkVSIxjzU/ww2xnJWUH8WDWdh+g9qToMuY/fLT/k9tOXHH/7t8R7xJjpjt/rymuJclB3H9ZOo/IoH6ddQfoumc2jOg+k0ZH+HxCHy+leQnIT8FGr+BcpH3Pz4/o9u3rv3kMd7xJvojF2DtlT944yyL5F3SHjhE8GFw7jwCYfDuPAJLh5C/mHk/w0XD+HiIVw4DFtYW+LiIZR8hooTwjdfmPrggJeCHt9w4OMvPN4j3kQn7Rq0pV/kHot45oXQJ9PDJvOysj08MaGT06eEpT45aVW/R7YOCcpauuy3SnmRx3vEm+i8XYO29FLBhb/85cTHH584eOCrgwe+Onjwq4MHOYmD7TMd0wesHPj4WO7nX/v+Irq77Pq4y/HZ9WZ8dr0Zn11vxmfXm/HZ9WZ8dr0Zn11vxmfXm/HZ9WYws4L9X1F8dr0Nn11vxmfXm0Gk77rrvdjs+nvUrlZdrKi8KK04Jyn/+Q5QXvKD+2BLkVack13PU1Re1Ko983LzbrGrqLyokhZs2/fpqGU5iMj2DgaJX1//9n6NolCjcMu/Q79n7Mqu523b92mIeMdFNXkTIeIdv9535H4fu7LreaOW5XxXVP9dUX1c5CSvWX9XVD9qWU6VrsxDdivY30XwpF1pxTlEZH972bJ0xqTm5ubm5pvdCIAuBnSOpeETv71cj4jsmqrK+9qu7HoeIrK/Kag7damuqamxK9j9E1i7HGdhjp/qMjdOXbKcKrAgIttYLfHZzT55oTY5ZPyNG/VdB4CzBJPmbjr7bFdoaLAkhYz/+mKth+0G3zV2j+eZj+eZ6+truwgANuG4dP34HcFiMR/PMx8/b/bZtdrNPWucM2lsfb25i3BEmrlru4SzpesVqK83WyymOZPGHP3J6LNrtXvkx5pPzxgtFlNXYAwxCXaTu/B+pIuFOlJXZzzyY82RH2s8aXdm+V1kd//3hthlT9fW1nQRALxrbsJZZndhNleLl0058L3BZ9dq98NTug9Pac3mqi4CgLu2W+zCHGO6XgGzucpkMnx0SvfhKa3PrtXu+yc1v1o+2WSq6iKsJybNrrkJNswus7swGg2vpk7ed1LjUbsVd8uzKkRk7z2u3ntcZTIZuggA3rWzBJvuRoxGPdMcn12r3d8dU62eH2k06rsLANwEI5Kbz43kLl0vuqZGuzp65p5jyrvCrmff7zJ2d+UqduUqjEadF1BTrdmVq9hltSv1mN2gu+DXlRm7O3MVGXMj9HqlQa806FX3Lnq9UquRZsyN2JmrQES2qVp2X9uVXz8/SPz6zlwFI9hr1jtzFYPEr9eaVB6269lfV1ZK89e9tT9EvEO8/fSG31R7DSHiHVm7Dlrq9J6xy3m/60m7WlWRQVu6/aPPvOy7GRt3f1JXp2uor76v7VbpymqqKmtN6npLVUN9zY16440GN9LUVO8+OAWZG2+YbzSYaqque9iu7zuR3kf3/KaJj7uTbvhNEx93LZzf3/XZ9Tp8dr0Z7i+j++x6G767Km/GZ9eb8dn1Zrrt93d93IX47HozNrsYnN8r9OqAxUrGsf8iHQueM/m494gsx8Tv2+wO+gLB+xG8H8EH2hK8HGjDSUCQE3hjbEc70D74AH9BQQes8FepPUFOcDxakJMjuOqKjge47k/HDuSrqotmOq32XxF8AI/9CQG7gUc+QsBuH17J/wNxvfgavzc59AAAAABJRU5ErkJggg==)

###  4.3.多态

　　所谓多态，就是指一个引用在不同情况下的多种状态，在Java中多态是指通过指向父类的引用，来调用不同子类中实现的方法。

　　JS实际上是无态的，是一种动态语言，一个变量的类型是在运行过程中由JS引擎决定的，所以说，JS天然支持多态。

## 五、JavaScript自定义类(原型对象)的方式

### 　　5.1.工厂方法——使用new Object创建对象并添加相关属性

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1     //通过Object直接创建对象
 2     //var p1 = new Object();//创建一个Object对象
 3     var p1 = {};//创建一个Object对象，简写
 4     //动态增加属性、方法
 5     p1.Name = "孤傲苍狼";
 6     p1.Age = 24;
 7     p1.SayHello = function() { alert("hello，"+p1.Name); };
 8     p1.SayHello();  
 9     for(var propertyName in p1) {//对象的成员都是对象的key
10         alert(propertyName);
11     }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 　　5.2.使用构造函数来定义类(原型对象)

```
基本语法：
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 function 类名(){
 2      this.属性名;//公共属性
 3      var 属性名;//私有属性
 4      /*凡是定义类的公共属性和公共方法都要使用this*/
 5      //定义类的公共函数
 6      this.函数名=function(){
 7                 
 8      }
 9      //定义类的私有函数
10      function 函数名(){
11 
12      }
13 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

范例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 /*定义一个猫类，这种就是使用构造函数来定义类(原型对象)*/
 2     function Cat(){
 3         this.Name="catName";//public属性
 4         this.Age;//public属性
 5         this.Color;//public属性
 6         var weight=2;//private属性，只能在Cat类内部使用，Cat类的对象无法访问这个私有属性
 7         //public方法
 8         this.publicFunction = function(){
 9             alert(weight);
10             alert("猫叫...");
11         }
12         //private方法，只能在Cat类内部使用，Cat类的对象无法访问这个私有方法
13         var privateFunction = function(){
14             alert("私有方法");
15         }
16 
17 
18     }
19     //如果这样用，那么就当成函数来使用
20     Cat();
21     //如果这样用，那么就当成类来使用
22     var cat1 = new Cat();
23     //cat1就是一个对象(实例)
24     alert(cat1.Name);//访问公共属性，弹出默认值catName
25     cat1.Name="TomCat";//访问公共属性
26     cat1.Age=3;//访问公共属性
27     cat1.Color="白色";//访问公共属性
28     alert(cat1.weight);//访问私有属性，无法访问，弹出undefined
29     alert(cat1.Name+"\t"+cat1.Age+"\t"+cat1.Color);//访问对象的属性方式1：对象名.属性名
30     alert(cat1["Name"]+"\t"+cat1["Age"]+"\t"+cat1["Color"]);//访问对象的属性方式2：对象名["属性名"]
31     cat1.publicFunction();//调用public方法
32     cat1.privateFunction();//调用private方法，报错：对象不支持此属性或方法
33     for(var property in cat1){
34         document.writeln(property+"\t");
35     }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 JS中一切都是对象，类(原型对象)其实也是对象，它实际上是Function类的一个实例

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 document.write("<pre>");
 2     function Person(){
 3     
 4     }
 5     /*Person.constructor得到的Person类的构造函数如下：
 6         function Function() {
 7             [native code]
 8         }
 9     */
10     document.writeln("Person.constructor："+Person.constructor);//Person类的构造函数
11     document.writeln("Person："+Person);
12     var p1 = new Person();
13     document.writeln("p1.constructor："+p1.constructor);//”p1.constructor“是得到p1实例的构造函数
14     //如何判断某个对象是不是某个类型，采用如下两种方式
15     if(p1 instanceof Person){
16         document.writeln("p1 is Person ok1");
17     }
18     if(p1.constructor==Person){
19         document.writeln("p1 is Person ok2");
20     }
21 
22     var num1=123;
23     document.writeln("num1.constructor："+num1.constructor);
24 
25     var str1="abc";
26     document.writeln("str1.constructor："+str1.constructor);
27     document.write("</pre>")
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

运行结果：

```
Person.constructor：
function Function() {
    [native code]
}

Person：function Person(){
	
	}
p1.constructor：function Person(){
	
	}
p1 is Person ok1
p1 is Person ok2
num1.constructor：
function Number() {
    [native code]
}

str1.constructor：
function String() {
    [native code]
}
```



分类: [JavaScript](https://www.cnblogs.com/xdp-gacl/category/431982.html)