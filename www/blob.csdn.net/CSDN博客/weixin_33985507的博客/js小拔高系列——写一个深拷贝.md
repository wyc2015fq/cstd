# js小拔高系列——写一个深拷贝 - weixin_33985507的博客 - CSDN博客
2018年11月01日 05:30:16[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
> 最近想全职当码农，面试时候被问到了深拷贝，我噼里啪啦说了一通，感觉很牛逼，问我自己写过没有，我说没有，但是我可以写。其实我很心虚，估计现场写会卡壳。。。。。
#### 思路
- 深拷贝需要迭代拷贝对象的所有属性，如果属性是引用型：Object,则继续递归迭代。
#### 实现
- 
首先可以写一个壳子函数，包裹一个将要反复递归调用自身的函数。
```
function deepCopy(obj) {
    const handleDeepCopy = obj=>{
       const clonedObj = obj instanceof Array? []:{}
      return clonedObj
    }
    return handleDeepCopy(obj)
}
```
clonedObj就是即将克隆返回的新对象
  今后会这么使用它：let newobj = deepCopy(oldobj)
  newobj就是经过深拷贝的新对象，这样在操作newobj的时候就不会对oldobj产生干扰了
- 
接着完善递归拷贝逻辑。
- 用for in迭代数组和对象的属性。（若不用forin迭代，用foreach代码会比较多，数组和对象的处理方式会有所不同）
- 判断属性的类型，是否是引用型。
- 如果是引用型，将clonedObj对应的property赋值为handleDeepCopy（obj[property]）【递归拷贝】。
- 
如果不是引用型，直接将clonedObj对应的property赋值为obj[property]。
```
function deepCopy(obj) {
    const handleDeepCopy = obj=>{
       const clonedObj = obj instanceof Array? []:{}
      // add for in logic here
      for (let keyOrIndex in obj) {
          if(obj[keyOrIndex] instanceof Object) {
              clonedObj[keyOrIndex] = handleDeepCopy(obj[keyOrIndex])
          } else {
              clonedObj[keyOrIndex] = obj[keyOrIndex]
          }
      }
      // for in logic over
      return clonedObj
    }
    return handleDeepCopy(obj)
}
```
- 以上逻辑还有疏忽的地方，没有在开始时判断传进的obj究竟是不是一个引用型，如果不是，就应该直接将obj返回。另外在for in中需要使用hasOwnProperty进行判断，因为我们只想拷贝obj自身的属性。为了节约篇幅，增加以上逻辑后修改的代码在：[stackblitz](https://stackblitz.com/edit/js-clonedeep)
