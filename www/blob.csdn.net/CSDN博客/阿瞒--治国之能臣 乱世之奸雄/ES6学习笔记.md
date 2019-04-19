# ES6学习笔记 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2019年03月04日 17:53:39[baofenny](https://me.csdn.net/jiaminbao)阅读数：46
函数：
1、内部不可Let定义
2、展开数组 `...`
箭头函数：
1、this问题，定义函数所在对象， 不再是运行时所在对象
2、箭头函数是没有arguments 用 `...`
3、箭头函数不能当构造函数
数组：
    /**
         *  数组循环
         */
        let arr = ['apple', 'banana', 'orange', 'tomato']
         // arr.forEach() 代替for循环
        arr.forEach((val, index, arr) => {
            console.log(val, index, arr, this)
        }, window)
         // arr.map() 要有return 重新整理数据结构
        let mapArr = arr.map((item, index, arr) => {
            return item + 1
        })
        console.log(mapArr)
         // arr.filter() 过滤，如果返回true就留下来
         let filterArr = arr.filter((item, index, arr) => {
            return item.includes('o')
         })
         console.log(filterArr)
         // arr.some() 类似查找 数组中某个元素符合条件返回true
         let someArr = arr.some((item, index, arr)=>{
            return item == 'apple'
         })
         console.log(someArr)
         // arr.every() 数组中所有元素都要符合条件 才返回true
         // arr.reduce()  求和、阶乘  从左往右
         let nums = [1,2,3,4,5,6,7,8,9,10]
         let reduce = nums.reduce((prev, cur, index, arr) => {
            return prev + cur
         })
         console.log(reduce)
         // arr.reduceRight() 从右往左
         let nums2 = [1,2,3]
         let rightPow = nums2.reduceRight((prev, cur, index, arr) => {
            // return Math.pow(prev, cur)
            return prev ** cur
         })
         console.log(rightPow)
         //for (... of ...)
         for (let index of arr.keys()){ // 数组下标
             console.log(index)
         }
         for (let item of arr.entries()){ //数组某一项
             console.log(item)
         }
         for (let [key, val] of arr.entries()){
             console.log(key, val)
         }
// 数组相关操作 有length属性可把类数组转数组 Array.from() / [...args]
        let obj = {a: 'aaa', b: 'bbb', c: 'ccc', length:3}
        let arr1 = Array.from(obj)
        console.log(arr1)
        // Array.of() 把一组值转数组
        Array.of('apple', 'banana', 'orange')
        //arr.find() 查找第一个符合条件的成员，没有返回undefined
        let arr4 = ['apple', 'banana', 'orange', 'tomato']
        arr4.find((val, index, arr) => {
            return val == 'apple';
        })
        //arr.findIndex() 查找位置，没有返回-1
        let pos = arr4.findIndex((val, index, arr) => {
            return val == 'banana';
        })
        console.log(pos)
        //arr.fill() 数组填充
        let arr5 = Array(10);
        arr5.fill('默认值', 1, 4);
        console.log(arr5);
        // arr.includes() 包含
        arr4.includes('apple')
对象：
     /**
         *  对象  ==> json
         */
        // 对象简洁语法
        let name = 'zhangsan';
        let age = 18;
        let json = {
            name,   // name: name
            age,    // age: age
            showA() {   // showA: function() {console.log(this.name)}  不要用箭头函数
                console.log(this.name)
            }
        }
        json.showA()
        // 实例
        let x = 10;
        let y = 20;
        let show = ({x, y}) => {
            console.log(x, y)
        }
        show({x, y})
        // Object.is() 用来比较两个值是否相等
        console.log(Object.is(+0, -0))
        // Object.assign() 用来合并对象  也可以赋值新数组
        let [json1, json2, json3] = [{a:1},{b:2},{c:3}];
        let obj = Object.assign({}, json1, json2, json3);
        console.log(obj)
        // Object.keys()  Object.entries  Object.values
        let {keys, values, entries} = Object;
        // keys()  values()  entries()
        //Object  ...
        let json4 = {a:3, b:4};
        let json5 = {...json4}
        delete json5.b
        console.log(json5)
Promise:
    /**
         *  Promise 解决异步回调问题
         */
        let a = 11;
        let promise = new Promise(function(resolve, reject) {
            if (a == 10) {
                return resolve('成功咯~');
            } else {
                return reject('失败咯~');
            }
        })
        promise.then(res => {
            console.log(res)
        },err => {
            console.log(err)
        })
        promise.catch(err => {
            console.log(err)
        })
        //new Promise().then(res=>{}).catch(err=>{})
        // Promise.resolve('aa') :将现有的东西转成Promise对象，成功状态
        // new Promise(resolve => {resolve('aaa')}) 与上等价
        // Promise.all() 把多个Promise对象打包 确保所有Promise对象都是成功
        let promise1 = Promise.reject('aaa')
        let promise2 = Promise.reject('bbb')
        let promise3 = Promise.resolve('ccc')
        // Promise.all([promise1, promise2, promise3]).then(res => {
        //     console.log(res)
        //     let [res1, res2, res3] = res
        //     console.log(res1, res2, res3)
        // }).catch(err => {
        //     console.log(err)
        // })
        //Promise.race()  只要有一个成功，就返回
        Promise.race([promise1, promise2, promise3]).then(res => {
            console.log(res, 111)
        }).catch(err => {
            console.log(err, 222)
        })
        // 实例
        new Promise(userLogin).then(res => {
            console.log('用户登录成功')
            return new Promise(getuserInfo);
        }).then(res => {
            console.log('获取用户信息成功')
        }).catch(err => {
            console.log(err)
        })
模块化：
    /**
         *  模块化
         */
        //export 模块内容  模块以.js结尾文件
        //使用模块 <script type='module'><\/script>
        // export default 不需要{}来import, export 导出来的需要import{}
        //import 特点:
        // 1、import可以是相对路径，也可以是绝对路径
        // 2、import模块只会导入一次，无论你引入多少次
        // 3、import './modules/1.js'; 如果这么用，相当于引入文件
        // 4、import有提升效果，会自动提升到顶部首先执行 
        // 5、导出去模块内容，如果里面有定时器更改，外面也会改动，不像Common规范缓存
        // import()类似node里面require，可以动态引入  默认import语法不能写到if之类里面
        // impoprt('./modules/1.js').then(res=>{ console.log(res.a) })
        promise.all([
            import('./modules/1.js'),
            import('./modules/2.js')
        ]).then(([mod1, mod2]) => {
            console.log(mod1)
            console.log(mod2)
        })
        // class、 async、 await
类：
/**
         *  类
         */
         // ES5定义类方式
        function Person(name, age){
            this.name = name;
            this.age = age
        }
        Person.prototype.showName = function () {
            return `我叫${this.name}`;
        }
        Object.assign(Person.prototype,{
            showAge() {
                return `年龄为：${this.age}`
            }
        })
        let p = new Person('wanglele', 18)
        console.log(p.showName(), p.showAge())
        // ES6定义类方式
        let fu = 'zhangsan';
        class Animal{
            constructor(food, voice) {
                this.food = food;
                this.voice = voice;
            }
            startFood(){
                return `我在吃${this.food}`;
            }
            startVoice() {
                return `我的叫声是${this.voice}`
            }
            // [] 中可以是变量或表达式   json中也可以这么声明[aaa + bbb]
            [fu]() {
                return `变量声明方法...`
            }
            static aaa() {
                return `这是静态方法`
            }
        }
        let a1 = new Animal('骨头', '汪汪汪...');
        // console.log(a1.startFood(), a1.startVoice())
        console.log(a1.zhangsan(),a1[fu]())
        // 注意
        // 1、ES5中函数有预解析处理，可提升函数加载， ES6中类没有提升功能
        // 2、ES6里的this比之前轻松多了
        // apply 、 call 、bind 三者都是用来改变函数的this对象的指向的；
        // apply 、 call 、bind 三者第一个参数都是this要指向的对象，也就是想指定的上下文；
        // apply 、 call 、bind 三者都可以利用后续参数传参；
        // bind 是返回对应函数，便于稍后调用；apply 、call 则是立即调用 。
        // getter() setter() 用于监听类中设置、获取值时的方法 eg: set 属性名(val) {}
        //静态方法
        // static 修饰方法为静态方法   调用方式：父类.aaa();
        // 继承
        // ES5继承方式是 Person.call(this, name) / Student.prototype = new Person();
        // ES6 继承方式 extends
        class Dog extends Animal{
            constructor(food, voice) {
                super(food, voice);
            }
            showName() {
                super.showName(); //父级方法
                console.log(`子类的showName方法...`)
            }
        }
        let d1 = new Dog('肉肉...', '喵喵喵...')
        console.log(d1.startFood(), d1.startVoice())
数据类型 Symbol  &  generator 函数
   /**
     * 数据类型 Symbol  &  generator 函数
     */
    let sym = Symbol('aaa');
    console.log(sym)
    // number string boolean Object undefined function
    //注意
    // Symbol不能new
    // Symbol() 返回是一个唯一值  一般做一个key,定义一些唯一或者私有的一些东西  json--> {[sym]：'aaa'}
    // symbol是一个单独数据类型，就叫symbol 基本类型
    // generator 函数 解决异步，深度嵌套问题   ===>  更好解决方案async 
    // 语法格式： function * show() { yield  }
    function * gen () {
        yield '第一步...';
        yield '第二步...';
        return 'res';
    }
    let g1 = gen();
    // console.log(g1.next(), g1.next(), g1.next());
    // for ... of .. 自动遍历generator  return不会遍历
    for(let val of g1) {
        console.log(val)
    }
    // 解构赋值
    let [a, b] = gen();
    // 扩展运算符 ...
    console.log(...gen());
    // Array.from()
    console.log(Array.from(gen()));
    // axios  需要引入  axios.get('api url...')
async & await
    /**
     * async & await
     */
    function readFile(filename) {
        new Promise((resolve, reject) => {
            fs.readFile(filename, (err, data) => {
                if(err) reject(err)
                resolve(data)
            })
        })
    }
    async function syn() {
        let a = await readFile('./a.txt');
        let b = await readFile('./b.txt');
        let c = await readFile('./c.txt');
    }
    // async特点：
    // 1、await只能放到async函数中
    // 2、相比genrator语义化更强
    // 3、await后面可以是promise对象，也可以数字、字符串、布尔
    // 4、async函数返回是一个promise对象
    // 5、只要await语句后面的promise状态变成reject，那么整个async函数会中断执行
    // 解决async函数中抛出犯错误，影响后续代码 : 1、try {} catch(e) {}   2、本身catch()
数据结构 set & WeekSet  map & WeekMap
    /**
     * 数据结构 set & WeekSet  map & WeekMap
     */
     //set数据结构 类似数组，但是里面不能有重复值  用法：nwe Set(['a', 'b'])
     let setArr = new Set(['a', 'b', 'c']);
     setArr.add('d');   //添加元素
     setArr.delete('a');    //删除元素
     setArr.has('b');   //检查元素是否存在
     setArr.size;   //元素个数
     setArr.clear();    //清除所有元素
     console.log(setArr);
     // 循环
     for (let [key, val] of setArr.entries()) {
        console.log(key, val);
     }
     setArr.forEach((value, index) => {
         console.log(index, value)
     })
     // set数据结构变数组， 可使用数组相关方法
     let newArr = [...setArr];
     newArr.map(val => val * 2)
     // new WeekSet() 存储json对象， new Set() 存储数组
     // map数据结构 类似json, 但是json的键(key) 只能是字符串  map键值可以是任意类型
     let map = new Map();
     map.set('key', 'value');
     let json = {
         a: 1,
         b: 2
     }
     map.set(json, 'aaa');
     map.set('aaa', json);
     map.get('key');    //获取元素
     map.delete('key'); //删除元素
     map.has('key');    //是否存在元素
     map.clear();   //清空元素
     console.log(map);
     // 循环 ...
     // WeekMap()   key只能是对象
     // 总结： Set 里面是数组，不重复，没有key,没有get方法  Map对json功能增强，key可以是任意类型值
数字变化和Math新增的东西
    /**
     * 数字变化和Math新增的东西
     */
     // 二进制 let a = 0b010101;
     // 八进制 let a 0o666;
     // 十六进制 #ccc
    // Number.isNaN(NaN)
    // Number.isFinite(a)   判断是不是数字
    // Number.isInteger()   判断是否整数
    // 安全整数： -(2^53-1) 到 (2^53-1)  Number.MAX_SAFE_INTEGER   Number.MIN_SAFE_INTEGER
    // Number.isSafeInteger() 判断是否是安全整数
    // Math
    // Math.trunc() 截断 只保留整数
    // Math.sign()  判断一个数是正数、负数、0
    // Math.cbrt()  计算一个数的立方根
ES2018 (ES9):
    /**
     * ES2018 (ES9)
     */
     // 命名捕获  语法： (?<名字>)
     let str = '2019-02-28';
     let reg = /(?<year>\d{4})-(?<mouth>\d{2})-(?<day>\d{2})/;
     let {year, mouth, day} = str.match(reg).groups;
    console.log(year, mouth, day)
    // 反向引用命名捕获   语法： \k<名字>
    // 常规操作： \1  \2  $1   $2
    let reg2 = /^(?<Strive>welcome)-\k<Strive>$/;
    let str1 = 'a-a';
    let str2 = 'Strive-Strive';
    let str3 = 'welcome-welcome';
    console.log(reg2.test(str1))
    console.log(reg2.test(str2))
    console.log(reg2.test(str3))
    // dotAll 模式  `s`
    // 之前'.'在正则里表示匹配任意东西， 但是不包括\n       let reg = /\w+/gims
    // 标签函数
    // 区别在于调用方式： 常规--->fun()  标签函数---> fun`welcome`
Proxy代理:
    /**
     * Proxy 代理
     */
    // 扩展对象一些功能 
    //Proxy作用：比如vue中拦截
    // 处理预警、上报、扩展功能、统计、增强对象
    // proxy是设计模式一种，代理模式
    // 语法: new Proxy(target, handler);    new Proxy(被代理的对象，对代理的对象做什么操作)
    // handler:
    // {
        //  set(){},    设置的时候做的事情
        //  get(){},    获取干的事情
        //  deleteProperty(){}, 删除
        //  has(){},    问你有没有这个东西  'xxx' in obj
        //  apply()     调用函数处理
        // ...
    // }
        let obj = {
            name: 'Strive'
        }; 
        let newObj = new Proxy(obj, {
            get(target, property) {
                // console.log(`您访问了${property}属性`);
                // return target[property];
                if (property in target) {
                    return target[property];
                } else {
                    // throw new ReferenceError(`${property} 属性不在此对象上`);
                    console.warn(`${property} 属性不在此对象上`);
                }
            },
            set(target, prop, value){
                console.log(target, prop, value);
            },
            has() {
            }
        });
        console.log(newObj.name)
        console.log(newObj.age)
        // Reflect 反射
        function sum(a, b) {
            return a + b;
        }
        let newSum = new Proxy(sum, {
            apply(target, context, args){
                return Reflect.apply(...arguments)
            }
        })
        console.log(newSum(2,3))
        // Object.xxx  语言内部方法 放到Reflect对象身上  
        // 通过Reflect对象身上直接拿到语言内部东西
