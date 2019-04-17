# vue.js + element UI的一个注意点 - westbrook1998的博客 - CSDN博客





2018年03月08日 23:16:39[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：187








首先创建一个vue模板项目

```
vue init webpack-simple xxx
cd xxx
npm install  //这里一定要npm不能用cnpm
cnpm run dev
```

这样可以正常启动

但是当引入element ui时就一直出问题

最终发现问题是在webpack.config.js里

```
{
        test: /\.(ttf|woff|png|jpg|gif|svg)$/,
        loader: 'file-loader',
        options: {
          name: '[name].[ext]?[hash]'
        }
      }
```

在正则里添加ttf和woff两个，解决



