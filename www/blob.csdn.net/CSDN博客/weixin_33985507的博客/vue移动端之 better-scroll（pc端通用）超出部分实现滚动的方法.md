# vue移动端之 better-scroll（pc端通用）超出部分实现滚动的方法 - weixin_33985507的博客 - CSDN博客
2018年10月11日 11:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
废话不多说，用到
[better-scroll](http://ustbhuangyi.github.io/better-scroll/doc/zh-hans/#better-scroll%20%E6%98%AF%E4%BB%80%E4%B9%88) 插件 （点击可进入gitup官方文档）
直接上代码案例
举个例子：
 京东分类页，美团店铺详情页都有用到此需求
<div class="goods">
       <!-- 分类列表 -->
<div class="menu-wrapper" ref="menuScroll"> 
//ref 意思是做一个标记，下面的方法可以找到这个元素 用this.$refs.menuScroll 来找到他
    <ul>
        <li>
        分类1
        </li>
         <li>
        分类2
        </li>
        ....
</div>
//首先npm 装上 [better-scroll](http://ustbhuangyi.github.io/better-scroll/doc/zh-hans/#better-scroll%20%E6%98%AF%E4%BB%80%E4%B9%88) ，命令是  npm install better-scroll --save
//然后在要引用的页面 import BScroll from 'better-scroll'
//在然后在methods 方法中写入
methods:{
    initScroll(){   
      new BScroll(this.$refs.menuScroll)    //实例化    用this.$refs.menuScroll 来找到要滚动的元素
      }
  },
//在然后在ajax获取请求成功的回调函数内 执行滚动方法
created(){
    axios.get('/api/goods')
    .then((res) => {
      this.menu_list = res.data.container_operation_source
      this.goods = res.data.food_spu_tags
      this.initScroll()   //执行滚动方法 。
        或者 
    this.$nextTick(()=>{
     this.initScroll()
)
    }).catch((err) => {
      console.log(err)
    });
  }
搞定！！！！
