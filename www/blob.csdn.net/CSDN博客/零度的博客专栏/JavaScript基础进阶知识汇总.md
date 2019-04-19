# JavaScript基础进阶知识汇总 - 零度的博客专栏 - CSDN博客
2015年09月06日 16:49:38[零度anngle](https://me.csdn.net/zmx729618)阅读数：404标签：[javascript																[JavaWeb](https://so.csdn.net/so/search/s.do?q=JavaWeb&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[前台UI](https://blog.csdn.net/zmx729618/article/category/5767211)
       //一、JS中常见的系统类
       // 1) String类
       var str =  "HelloWorld";
       window.document.write(str.length+"<br />");
       window.document.write(str.indexOf("World")+"<br />");
       window.document.write(str.replace("World", "JavaScript")+"<br />");  
       // 2) Data类
       var date1 = new Date();                           // 创建 Date 对象。
       var ss = (date1.getMonth() + 1) + "/";            // 获取月份。
       ss += date1.getDate() + "/";                   // 获取日。
       ss += date1.getFullYear();  
       window.document.write(""+ss+"<br />");
       // 3) Math类
       window.document.write(Math.ceil(4.2)+"<br />");
       window.document.write(Math.random()+"<br />");
       window.document.write(Math.max(23,24,14,25)+"<br />");
       //二、JS自定义类
       //4) JS自定义类  --对象的属性是动态添加的   
       function Person(){}    //构造函数---定义类
       var p1 = new Person();  //执行构造函数
       p1.name = "zhangsan";
       p1.age = 30;
       p1["marry"] = true;
       window.document.write(p1.name+" " +p1.age+" "+p1.marry +"<br />");
       window.document.write(p1.constructor+"<br />");   //返回对象的构造器
       window.document.write(typeof p1+"<br />");        //对象的类型
       window.document.write(" "+(p1 instanceof Person) +"<br />");  //判断是否为类的实例
       //5) JS自定义类 --不需要动态添加属性自动具有定义      
       function People(p_name,p_age){
           this.name = p_name;
           this.age = p_age;
       }
       var v1 = new People("zhangsan",30);
       window.document.write(v1.name+" " +v1.age+"<br />");
       //参数是一个对象
       function display(obj){       
        window.document.write(obj.name+" " +obj.age+"<br />");  
       }
       //6)  for...in语句  for(var i in obj) ---遍历属性
       function display2(obj){
          for(var i in obj){
             window.document.write(i+":  "+obj[i]+"<br />");
          }
       }
       display2(v1);
       //7)  delete关键词，用于删除自定义对象属性
       delete v1.age;
       display2(v1);
       //8) 自定义类-动态添加成员方法
       function Animal(){}
       var a1 = new Animal();
       a1.name="Tom";
       a1.say=function(){
           window.document.write("我是"+this.name+"<br />");
       }
       a1.say();
       //9) 自动义类-自动具有成员属性和成员方法
       function Animal2(p_name){      
           this.name = p_name;
           this.say = function(){
               window.document.write("我是"+this.name+"<br />");
           }
       }
       var a2 = new Animal2("kite");
       a2.say();
      //三、JS中JSON对象 
       //10) json格式数据[{},{},{}]
       var o1 = {name:"zhangsan", age:18,email:"[zs@163.com](mailto:zs@163.com)"};
       var o2 = {"name":"lisi", "age":36,"email":"[ls@163.com](mailto:ls@163.com)"};
       var o2 = {"name":"lisi", "age":36,"email":"[ls@163.com](mailto:ls@163.com)", "say": function(){window.document.write("我是"+this.name+"<br />");}};
       window.document.write(o1.name+o1.age+o1.email+"<br />");
       window.document.write(o2.name+o2.age+o2.email+"<br />");
       o2.say();
       //四、JS中的prototype使用 
       //11) prototype的使用： 返回对象类型原型的引用    Class.prototype
       function Brid(b_name){
            this.name = b_name
       }
       //使用原型可以为该类的所有对象添加属性和方法
       Brid.prototype.age=30;  
       Brid.prototype.say= function(){
            window.document.write("我是"+this.name+"<br />");
       }  
       var b = new Brid("Poly");
       window.document.write(b.name + b.age+"<br />");   //age属性找不到，会去原型对象中去找
       b.say();      
       alert(Brid.prototype.constructor);          
       //原型解释：程序加载某一个类(或函数)，会为该类的构造器创建原型对象，它们两者是独立存在的，
       //类构造器中存在一个prototype属性指向了原型对象，原型对象中存在一个constructor属性指又向该类的构造器
       //当该类的一个实例在使用一个不存在的属性时，会自动到该类构造器的原型对象中去找。        
       alert(b.constructor); //如该代码,b对象实例中没有constructor属性，会自动到Brid类的原型对象中去找，原型对象中存在constructor找到，指向构造器
       // 12) 原型继承
       //原型也是一个对象，是Object类的一个实例，可以调用Object类下的所有属性和方法。
       window.document.write(b.toString()+"<br />"); 
       // 13) 原型链
       //一个自定义类对继承自Object，该类的原型对象也是对象，当这个类找不到某个属性时，会到Object构造器的原型对象
       function Brid2(b_name){
            this.name = b_name
       }
       //使用原型可以为该类的所有对象添加属性和方法
       Object.prototype.age=30;  
      Object.prototype.say= function(){      
           alert("我是"+this.age+"<br />");
      }  
       var b2 = new Brid2("Poly2");
       alert(b2.name + b2.age+"<br />");   //age属性找不到，会去原型对象中去找
       //14)函数闭包
       //定义：所谓"闭包"，指的是一个拥有许多变量和绑定了这些变量的环境的表达式（通常是一个函数）因而这些变量也是该表达式的一部分。
       //功能：访问局部变量； 使局部变量不被回收
       function fn1(){
           var i = 10
           function fn2(){ 
               alert(i++);
           }
           return fn2;
       }
       var test = fn1();  //访问i的值
       test();
       //15) 私有属性
       function Pet(p_name,p_age){
          this.name = p_name;   //模拟公有属性
          var age= p_name;      //模拟私有属性
          this.setAge = function(a){    //使用闭包实现
               age = a
          }
          this.getAge = function(){     //使用闭包实现
              return age;
          }
       }
       var pet1 = new Pet("Cat",8);
       pet1.setAge(8);
       alert(pet1.getAge());
       // 16) call、apply方法的使用：使用指定对象调用当前方法
       // call(thisObj,arg1,arg2,argN);
       // apply(thisObj,[argArray]);
       //say.call(p);  
       //17)继承实现
       // 方法一:扩展Object
       Object.prototype.ext = function(parObject){
           for(var i in parObject){
               this[i] = parObject[i];
           }
       }       
       function Dog(d_name,d_age){
          this.name = d_name;
          this.age = d_age;
          this.speak = function(){
             alert(this.name+this.age);
          }
       }      
       function PetDog(p_no){
          this.no = p_no;
          this.say = function(){
            alert(this.p_no);
          }
       }       
       var piter = new PetDog(101);
       piter.ext(new Dog("piter",3));   //继承实现
       piter.speak();
       //方法二:使用 call、apply方法
       function Dog(d_name,d_age){
          this.name = d_name;
          this.age = d_age;
          this.speak = function(){
             alert(this.name+this.age);
          }
       }
       function PetDog(p_no,d_name,d_age){
          this.no = p_no;
          this.speak = function(){
             alert(this.name+this.age);
          }
          Dog.call(this,d_name,d_age);    //继承实现
       } 
       //方法三:使用 prototype原型继承    (推荐使用)
       //语法：子类.prototype = new 父类();
       function Dog(d_name,d_age){
          this.name = d_name;
          this.age = d_age;
          this.speak = function(){
             alert(this.name+this.age);
          }
       }
       function PetDog(p_no,d_name,d_age){
          this.no = p_no;
          this.speak = function(){
             alert(this.name+this.age);
          }          
       }
       PetDog.prototype = new Dog("piter",4)   //继承实现
       var pd2 = new PetDog(101,"piter",4);  
       pd2.speak(); 
