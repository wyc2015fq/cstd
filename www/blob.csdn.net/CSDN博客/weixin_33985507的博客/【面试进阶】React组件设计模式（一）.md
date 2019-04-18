# 【面试进阶】React组件设计模式（一） - weixin_33985507的博客 - CSDN博客
2018年12月14日 02:58:16[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
首先编写一下我们的公共组件
#### 单个商品组件（商品组件：展示价格、购买数量）
`goodsItem.js`
```bash
// 单个商品
import React from 'react';
const GoodsItem = props => {
  const { goods: {name, num, price}, handleSub, handleAdd } = props;
  return <div className="goods-item">
    {name}  
    <button onClick={() => handleSub()}>-</button>
    <span>{num}</span>
    <button onClick={() => handleAdd()}>+</button>
    价格：{price}
  </div>
};
export default GoodsItem;
复制代码
```
#### 商品列表组件（循环展示库中的商品）
`goodList.js`
```bash
// 商品列表
import React from 'react';
import GoodsItem from './goodsItem';
class GoodsList extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      goodsData: []
    }
  }
  componentDidMount() {
    const { goodsData } = this.props;
    this.setState({ goodsData: goodsData});
  }
  handleAddorSub(id, type) {
    let { goodsData } = this.state;
    let newGoods = goodsData.reduce((newData, goods) => {
      if (goods.id === id) {
        goods.num = type === '+' ? goods.num + 1 : goods.num - 1;
      }
      newData.push(goods);
      return newData;
    }, [])
    this.setState({ goodsData: newGoods })
  }
  render() {
    const { goodsData } = this.state;
    return (
      <div className="goods-list">
        {goodsData.map(goods =>
          <GoodsItem 
            key={goods.id} 
            goods={goods}
            handleAdd={() => this.handleAddorSub(goods.id, '+')}
            handleSub={() => this.handleAddorSub(goods.id, '-')}
          />
        )}
      </div>
    );
  }
};
export default GoodsList;
复制代码
```
我们一般编写组件，都会这么去做，list包裹item，循环展示item。数据放在list组件中，item作为一个无状态组件，只做他的展示。
数据交互通过props传递，点击`+-`会改变购物车里的数据。
现在需求来了，双12来了（就在昨日），所有商品8折优惠。
这意味着我们需要修改goodsData中所有商品的价格。
这并不难，我叶良辰有100种方法可以修改商品数据。找个可行的生命周期，比如`componentDidMount`中修改list组件`state.goodsData`就行了。
如果每次修改都直接修改goodList组件，也是可以的，大不了多传几个props来判断需要打折还是修改商品名称等等。
但是有些需求是交叉的，如果一直这样写，久而久之组件会变得越来越臃肿，最后爆炸。
好的解决方案应该是goodsList不去动他，外加一层来进行包装，实现我们的逻辑。
这样既保证了goodsList的`纯`，又能实现逻辑的复用。可谓一箭双雕。
用两种组件设计模式可以帮助到我们。
### 一. renderProps 模式
#### renderProps其实是利用组件的props.children api，将函数当成组件的一种写法。
我们调用公共组件的方法如下：
```bash
<GoodsList goodsData={goodsData} />
复制代码
```
我们用renderProps模式实现打折商品组件：
```bash
<DiscountedGoodsList goodsData={goodsData}>
  {(data) => <GoodsList goodsData={(data)} />}
</DiscountedGoodsList>
复制代码
```
可以看到，DiscountedGoodsList的子组件是一个函数，那么一个函数是怎么渲染成组件的？
再来看看DiscountedGoodsList组件的代码：
```bash
const DiscountedGoodsList = props => {
  // 8折优惠逻辑
  const setRenderPropsData = (data) => {
    let renderPropsData = data.reduce((array, goods) => {
      let obj = {};
      for (let k in goods) {
        obj[k] = k === 'price' ? (goods[k] * .9).toFixed(2) : goods[k];
      }
      array.push(obj);
      return array;
    }, []);
    return renderPropsData;
  }
  let goodsData = setRenderPropsData(props.goodsData);
  return (
    <React.Fragment>
      {props.children(goodsData)}
    </React.Fragment>
  );
}
复制代码
```
`setRenderPropsData`的作用是实现8折优惠逻辑，将所有商品价格调整。
然后调用props.children这个api，得到在上面我们编写的函数。
props.children也就是函数`(data) => <GoodsList goodsData={(data)} />`的引用。
将处理后的数据goodsData作为参数执行，最终返回`<GoodsList />`组件，这就是renderProps模式。
以后我们需要调用价格优惠的商品列表组件，直接调用`DiscountedGoodsList`即可。
renderProps的模式实现了逻辑的共用，且对`GoodsList`组件毫无副作用，从而达到我们的目的。
### 二. HOC（高阶组件）模式
#### 所谓的高阶组件，其实就是一个函数，该接受component为参数，返回一个处理后的component。
编写我们的高阶组件如下：
```bash
const BrandGoodsList = (Component, goodsData) => {
  // 品牌商品逻辑
  const setRenderPropsData = (data) => {
    let renderPropsData = data.reduce((array, goods) => {
      let obj = {};
      for (let k in goods) {
        obj[k] = k === 'name' ? goods[k] + '【品牌】' : goods[k];
      }
      array.push(obj);
      return array;
    }, []);
    return renderPropsData;
  }
  let brandGoodsData = setRenderPropsData(goodsData);
  return <Component goodsData={brandGoodsData} />
}
复制代码
```
`BrandGoodsList`组件的逻辑就是给商品名称加上`【品牌】`的标示，区分商品。
高阶组件的调用比较简单：`{BrandGoodsList(GoodsList, goodsData)}`直接执行返回组件，然后渲染。
实现了两种模式，现在我们将他们一起用，实现一个既打折，又是品牌商品的组件。
```bash
<DiscountedGoodsList goodsData={goodsData}>
  {(data) => BrandGoodsList(GoodsList, data)}
</DiscountedGoodsList>
复制代码
```
挺舒服的吧，随时分离，随时结合。正是高内聚、低耦合本人啊。
最后，完整的调用看一下：
```bash
<div className="App">
    基本商品列表组件：
    <GoodsList goodsData={goodsData} />
    <br />
    打8折商品列表组件（renderProps模式实现）：
    <DiscountedGoodsList goodsData={goodsData}>
      {(data) => <GoodsList goodsData={(data)} />}
    </DiscountedGoodsList>
    <br />
    品牌商品列表组件（高阶组件模式实现）：
    {BrandGoodsList(GoodsList, goodsData)} 
    <br />
    
    既是打折商品，又是品牌商品（两种模式复用）
    <DiscountedGoodsList goodsData={goodsData}>
      {(data) => BrandGoodsList(GoodsList, data)}
    </DiscountedGoodsList>
  </div>
复制代码
```
### 总结：
#### 1、renderProps 模式的核心是props.children的使用。
#### 2、高阶组件的写法看起来更舒服，比较受欢迎。
#### 3、两种模式解决的问题：复用逻辑、不污染底层组件。
*觉得有帮助的点个赞，甚至可以关注一波哦~*
