# css3使用animation实现消息轮播在iOS中的bug - weixin_33985507的博客 - CSDN博客
2018年09月21日 02:49:43[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：39
### 环境
- iOS 11
- Vue2.x
- 在PC端Chrome调试没问题、Android端没问题
- 刚来公司第二天就安排修bug了
### bug重现
源码
```
<template>
  <div class="carousel">
    <div class="scroll-wrapper">
      <span
        v-for="(item, index) in list"
        :class="['scroll-item']"
        :key="index"
      >
      恭喜{{item.mobile}}  抽中 <em class="prize">{{item.prizeName}}</em>
      </span>
    </div>
  </div>
</template>
<script>
  export default {
    name: 'ScrollMessage',
    props: {
      list: Array
      /* 备注：
        1. 默认下会设置五个写死的数据
        2. 如果登录之后，会向后台拿真实数据，取前十条
      */
    }
  }
</script>
<style lang="less" scoped>
.carousel {
  position: relative;
  margin-top: 0.4rem;
  padding: 0 0.2rem;
  border: 1px dashed #d28c5a;
  height: 0.75rem;
  line-height: 0.75rem;
  background: #6f09d4;
  // overflow: hidden;
}
.scroll-wrapper {
  position: absolute;
  white-space: nowrap;
  animation: scroll 30s linear infinite;
}
.scroll-item {
  display: inline-block;
  vertical-align: middle;
  margin-right: 0.4rem;
  font-size: 0.24rem;
  color: #fff;
  background: yellowgreen;
  .prize {
    color: #feb533;
    font-style: normal;
  }
}
@keyframes scroll {
  0% {
    -webkit-transform: translateX(0);
    -moz-transform: translateX(0);
    -ms-transform: translateX(0);
    -o-transform: translateX(0);
    transform: translateX(0);
  }
  100% {
     -webkit-transform: translateX(-50%);
    -moz-transform: translateX(-50%);
    -ms-transform: translateX(-50%);
    -o-transform: translateX(-50%);
    transform: translateX(-50%);	
  }
}
</style>
复制代码
```
登录前：默认下会设置五个写死的数据。正常(不放图啦)
图2-登录后：向后台拿真实数据，取前十条。轮播卡顿问题
图3-登录后刷新页面：不轮播了，切换tab后回到页面才轮播
### 定位bug之旅
- **经验之谈**。看到动画效果卡顿，一下子就想到了开启css3硬件加速。加了属性之后还是没效果。
- **行为观察**。登录后动画才会卡。登录后向后台拿真实数据，取前十条。
- **大胆猜测**。数据的变化，引起`scroll-wrapper`总长度的变化。总长度的变化需要重新计算动画效果。iOS在这里没有处理好。
- **验证**。登录后，把数据写死成登录前默认的五条，效果正常。把数据写成6条，效果不正常。把数据写死成登录前默认的五条，只改变手机尾号，效果正常。验证成功！
### 我的解决方案
在登录后，让动画重新播放。关于如何实现重新播放，可以参考[这里](https://link.juejin.im?target=https%3A%2F%2Fwww.jianshu.com%2Fp%2F7e0c8e298488)。其实就是再多加一个动画frame和类。
解决方案代码
```
<template>
  <div class="carousel">
    <div class="scroll-wrapper" :class="{'play': list.length === 5, 'play2': list.length > 5}">
      <span
        v-for="(item, index) in list"
        :class="['scroll-item']"
        :key="index"
      >
      恭喜{{item.mobile}}  抽中 <em class="prize">{{item.prizeName}}</em>
      </span>
    </div>
  </div>
</template>
<script>
...
</script>
<style lang="less" scoped>
...
.scroll-wrapper {
  position: absolute;
  white-space: nowrap;
  -webkit-transform: translateX(0);
  -moz-transform: translateX(0);
  -ms-transform: translateX(0);
  -o-transform: translateX(0);
  transform: translateX(0);
  &.play{
    animation: scroll 30s linear infinite;
  }
  &.play2{
    animation: scroll2 30s linear infinite;
  }
}
...
@keyframes scroll {
  0% {
    -webkit-transform: translateX(0);
    -moz-transform: translateX(0);
    -ms-transform: translateX(0);
    -o-transform: translateX(0);
    transform: translateX(0);
  }
  100% {
     -webkit-transform: translateX(-50%);
    -moz-transform: translateX(-50%);
    -ms-transform: translateX(-50%);
    -o-transform: translateX(-50%);
    transform: translateX(-50%);	
  }
}
/* to fix bug: after login, animation problem */
@keyframes scroll2 {
  0% {
    -webkit-transform: translateX(0);
    -moz-transform: translateX(0);
    -ms-transform: translateX(0);
    -o-transform: translateX(0);
    transform: translateX(0);
  }
  100% {
     -webkit-transform: translateX(-50%);
    -moz-transform: translateX(-50%);
    -ms-transform: translateX(-50%);
    -o-transform: translateX(-50%);
    transform: translateX(-50%);	
  }
}
</style>
复制代码
```
有更好的解决方案欢迎补充哟。
