# 面向Vue新人：用Vue写一个分页器 - weixin_33985507的博客 - CSDN博客
2018年04月19日 14:43:15[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
> 
之前一直想要自己试着实现一个分页器，但是一直拖，今天写完，大概照着网易云音乐的样子来完成。这个小例子很简单，通过这个小例子，可以学习到Vue计算属性的使用，并了解到写分页器需要区分的情况。这篇文章会慢慢从头来实现这个小例子，相信你一定会学会，而且看完了我的思路之后说不定会有更棒的思路和想法！
实现的效果是这样子的：
### 一、先简单布局
```bash
<template>
  <div class="pageContainer">
    <ul class="pagesInner">
      <li class="page"><span class="fa fa-chevron-left" aria-hidden="true"></span></li>
      <li class="page" v-for="(item, index) in pages" :key="index">
        <span>{{item}}</span>
      </li>
      <li class="page"><span class="fa fa-chevron-right" aria-hidden="true"></span></li>
    </ul>
  </div>
</template>
<script>
export default {
  computed: {
    pages() {
      return 10;
    }
  }
};
</script>
复制代码
```
效果如下：
有两个地方说下：
- 最前面和最后面两个icon用的font awesome的cdn
- 使用v-for来进行渲染的数据用的是计算属性里的pages，暂时写了一个数据11，所以渲染出了11个标签
### 二、理清思路，在这个例子里最重要
这篇文章的例子是参照了网易云音乐里的分页方法，网易的图：
它有几个特点：
- 首页和尾页一直都有
- 最多时候有11个标签页，包括两个 ... ，但是它们不能点击，所以我也把例子里的页码设为11
- 在当前页变动的时候，页码的形态也会发生变化，总共有**三种情况**，这个下面会细说
分页的三种情况：
#### 第一种：当前页码小于等于5的时候
如图：
**这种情况首页尾页保留，倒数第二个页面为..., 页码从头往后算**
#### 第二种情况：当前页码处于最后5个的时候
如图：
**首页和尾页依然保留，第二个页码为...，页码从最后向前算**
#### 第三种情况：页面处于较中间位置的时候
如图：
**首页和尾页都保留，第二个页码和倒数第二个页码都为 ... ，同时页面从当前页码位置向两侧算**
### 三、用代码来实现上面的三种情况
从上面的思路来看，页码具体如何呈现由**当前页码的位置决定**，所以我们要在data里设置一个currentPage:1，而计算属性里用于渲染页码的pages通过currentPage来控制，同时还需要一个总页数totalPages:50。
先写第一种情况：
```bash
<script>
export default {
  data() {
    return {
      currentPage: 1,
      totalPages: 50
    }
  },
  computed: {
    pages() {
      const c = this.currentPage
      const t = this.totalPages
      if (c <= 5) {
        return [1, 2, 3, 4, 5, 6, 7, 8, 9, '...', t]
      }
    }
  }
};
</script>
复制代码
```
效果如下：
写第二种情况，再加个if：
```bash
<script>
export default {
  data() {
    return {
      currentPage: 47,
      totalPages: 50
    }
  },
  computed: {
    pages() {
      const c = this.currentPage
      const t = this.totalPages
      if (c <= 5) {
        return [1, 2, 3, 4, 5, 6, 7, 8, 9, '...', t]
      } else if (c >= t - 4) {
        return [1, '...', t-8, t-7, t-6, t-5, t-4, t-3, t-2, t-1, t]
      }
    }
  }
};
</script>
复制代码
```
把currentPage的值设为>= 46，效果如下：
把第三种情况加上：
```bash
computed: {
    pages() {
      const c = this.currentPage
      const t = this.totalPages
      if (c <= 5) {
        return [1, 2, 3, 4, 5, 6, 7, 8, 9, '...', t]  //第一种情况
      } else if (c >= t - 4) {
        return [1, '...', t-8, t-7, t-6, t-5, t-4, t-3, t-2, t-1, t] //第二种情况
      } else {
        return [1, '...', c-3, c-2, c-1, c, c+1, c+2, c+3, '...', t]  //第三种情况
      }
    }
  }
复制代码
```
基本就是这样，已经可以通过改变currentPage的值查看分页器的变化了。
接下来实现点击相应的页码来改变currentPage的值，只要写一个点击事件再写一个函数就好了。
```bash
<li class="page" 
    v-for="(item, index) in pages" 
    :key="index"
    :class="{actived: item === currentPage}"   // 给点击到的当前页码添加样式
    @click="select(item)"     // 添加一个点击事件
>
...
methods: {
    select(item) {
        this.currentPage = item
    }
}
...
actived: {
    border-color: #2d8cf0;
    background-color: #2d8cf0;
    color: #fff;
}
复制代码
```
效果如下：
为了让当前页码更清楚，再在页面上加上当前多少页
```bash
<div>当前第{{currentPage}}页</div>
复制代码
```
效果如下：
发现了一bug，就是我们每次点击的时候，都是将item的具体内容传递过去改变currentIPage的，但是当我们点的 ... 的时候就把它也传递过去了，但是它不是我们要的页码的数据，在计算的时候就出错了，所以我们需要做一点处理。同时，还有再点击当前页码的时候也不必再执行select函数了。
简单改写一下select函数：
```bash
select(n) {
    if (n === this.currentPage) return 
    if (typeof n === 'string') return 
    this.currentPage = n
}
复制代码
```
这样就正常了。
再把两侧icon向前一页和向后一页的功能加上，因为一个是加1一个是减1，所以写一个函数传递不同的参数就行了。
```bash
<li class="page" @click="prevOrNext(-1)"><span class="fa fa-chevron-left" aria-hidden="true"></span></li>
...
<li class="page" @click="prevOrNext(1)"><span class="fa fa-chevron-right" aria-hidden="true"></span></li>
...
prevOrNext(n) {
    this.currentPage += n
}
复制代码
```
效果如下：
呃，边界问题，当currentPage为1时就不能再减了，当它为最大时也不能再加了。
改写一下代码：
```bash
prevOrNext (n) {
  this.currentPage += n
  this.currentPage < 1
  ? this.currentPage = 1
  : this.currentPage > this.totalPages
    ? this.currentPage = this.totalPages
    : null
}
复制代码
```
这下就可以了，如图：
### 四、 结语
写到这里，这个分页器基本功能就写完了，当然，我们还可以继续封装，在每次改变currentPage的时候用`this.$emit`通知外面实现通信，还可以通过`props`来向内传递数据，比如传递totalPages等，这些都是可以继续完善的内容。最重要的一点，关于分页器的具体计算方法，我用的是最笨的方法，所以同志们要是知道更好的办法记得留言啊~
这是我在掘金上的第五篇文章，感谢您的观看！
