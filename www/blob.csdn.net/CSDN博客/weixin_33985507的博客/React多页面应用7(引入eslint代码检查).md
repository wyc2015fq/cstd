# React多页面应用7(引入eslint代码检查) - weixin_33985507的博客 - CSDN博客
2018年01月18日 18:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
![9794326-c0b67a75adf1970c](https://upload-images.jianshu.io/upload_images/9794326-c0b67a75adf1970c)
![9794326-1a72548903686ace](https://upload-images.jianshu.io/upload_images/9794326-1a72548903686ace)
本教程总共7篇,每日更新一篇,请关注我们!你可以进入历史消息查看以往文章,也敬请期待我们的新文章!
1.React多页面应用1(webpack开发环境搭建,包括Babel、热更新等) ----2017.12.28
2.React多页面应用2(处理CSS及图片,引入postCSS及图片处理等)----2017.12.29
3.React多页面应用3(webpack性能提升,包括打包性能、提取公共包等)----2017.12.30
4.React多页面应用4(webpack自动化生成多入口页面)----2017.12.31
5.React多页面应用5(webpack生产环境配置,包括压缩js代码,图片转码等)----2018.01.01
6.React多页面应用6(gulp自动化发布到多个环境,生成版本号,打包成zip等)----2018.01.02
7.React多页面应用7(引入eslint代码检查)----2018.01.03
**开发环境:Windows 8,node v8.9.1,npm 5.5.1,WebStorm 2017.2.2**
团队开发,如何保持代码风格统一,引入eslint代码检查是一个不错的选择!
当然现在也有更好的替代方案就是微软的TypeScript,
今天先讲讲eslint
还是先引入依赖(注意安装命令太长, 复制后改成一行)
npmi -D eslint eslint-config-react-app eslint-loader eslint-plugin-flowtype
eslint-plugin-import eslint-plugin-jsx-a11y@5.1.1 eslint-plugin-react
react-dev-utils babel-eslint
修改 package.json
添加如下代码
"eslintConfig": {
"extends": "react-app",
  "rules": {
"import/no-webpack-loader-syntax": 0,
    "no-script-url": 0,
    "jsx-a11y/href-no-hash": 2
  }
}
修改 webpack.dev.conf.js
const webpack =require('webpack');//引入webpack
const opn =require('opn');//打开浏览器
const merge =require('webpack-merge');//webpack配置文件合并
const path =require("path");
const baseWebpackConfig =require("./webpack.base.conf");//基础配置
const webpackFile =require("./webpack.file.conf");//一些路径配置
const eslintFormatter =require('react-dev-utils/eslintFormatter');
let config = merge(baseWebpackConfig, {
output: {
path: path.resolve(webpackFile.devDirectory),
        filename:'js/[name].js',
        chunkFilename:"js/[name]-[id].js",
        publicPath:''
    },
    plugins: [
/*设置开发环境*/
        new webpack.DefinePlugin({
'process.env': {
NODE_ENV: JSON.stringify('development'),
            }
}),
        /*设置热更新*/
        new webpack.HotModuleReplacementPlugin(),
        /* common 业务公共代码，vendor引入第三方 */
        new webpack.optimize.CommonsChunkPlugin({
name: ["common", "vendor"],
        }),
        /* 防止 vendor hash 变化 */
// extract webpack runtime and module manifest to its own file in order to
// prevent vendor hash from being updated whenever app bundle is updated
        new webpack.optimize.CommonsChunkPlugin({
name:'manifest',
            chunks: ['vendor']
}),
    ],
    module: {
rules: [
{
test:/\.(js|jsx)$/,
                use: [
'cache-loader',
                    'babel-loader',
                ],
                include: [
path.resolve(__dirname, "../../app"),
                    path.resolve(__dirname, "../../entryBuild")
],
                exclude: [
path.resolve(__dirname, "../../node_modules")
],
            },
            {
test:/\.(js|jsx)$/,
                enforce:'pre',
                use: [
{
options: {
formatter: eslintFormatter,
                            eslintPath:require.resolve('eslint'),
                            // @remove-on-eject-begin
                            baseConfig: {
extends: [require.resolve('eslint-config-react-app')],
                            },
                            //ignore: false,
                            useEslintrc:false,
                            // @remove-on-eject-end
                        },
                        loader:require.resolve('eslint-loader'),
                    },
                ],
                include: [
path.resolve(__dirname, "../../app")
],
                exclude: [
path.resolve(__dirname, "../../node_modules")
],
            },
            {
test:/\.(css|pcss)$/,
                loader:'style-loader?sourceMap!css-loader?sourceMap!postcss-loader?sourceMap',
                exclude:/node_modules/
            },
            {
test:/\.(png|jpg|gif|ttf|eot|woff|woff2|svg|swf)$/,
                loader:'file-loader?name=[name].[ext]&outputPath=' +webpackFile.resource +'/'
            }
]
},
    /*设置api转发*/
    devServer: {
host:'0.0.0.0',
        port:8080,
        hot:true,
        inline:true,
        contentBase: path.resolve(webpackFile.devDirectory),
        historyApiFallback:true,
        disableHostCheck:true,
        proxy: [
{
context: ['/api/**', '/u/**'],
                target:'http://192.168.12.100:8080/',
                secure:false
            }
],
        /*打开浏览器 并打开本项目网址*/
        after() {
opn('http://localhost:' +this.port);
        }
}
});
module.exports = config;
添加两处代码
引入
const eslintFormatter =require('react-dev-utils/eslintFormatter');
加入
{
test:/\.(js|jsx)$/,
    enforce:'pre',
    use: [
{
options: {
formatter: eslintFormatter,
                eslintPath:require.resolve('eslint'),
                // @remove-on-eject-begin
                baseConfig: {
extends: [require.resolve('eslint-config-react-app')],
                },
                //ignore: false,
                useEslintrc:false,
                // @remove-on-eject-end
            },
            loader:require.resolve('eslint-loader'),
        },
    ],
    include: [
path.resolve(__dirname, "../../app")
],
    exclude: [
path.resolve(__dirname, "../../node_modules")
],
},
至此我们就配置完了
执行
npm run dev 
ok! 一切正常!
我们来测试下!
在 app\component\common\Seconds.jsx 组件中 加一个没有用到的变量!
我们看看浏览器什么反映,
![9794326-6cb1ba81e6a65189](https://upload-images.jianshu.io/upload_images/9794326-6cb1ba81e6a65189)
浏览器控制台,清楚的给了提示!
是不是很爽!!!!
本文完
![9794326-c0b67a75adf1970c](https://upload-images.jianshu.io/upload_images/9794326-c0b67a75adf1970c)
禁止擅自转载,如需转载请在公众号中留言联系我们!
感谢童鞋们支持!
如果你有什么问题,可以在下方留言给我们!
