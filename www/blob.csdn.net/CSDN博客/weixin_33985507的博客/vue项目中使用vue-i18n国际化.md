# vue项目中使用vue-i18n国际化 - weixin_33985507的博客 - CSDN博客
2018年06月27日 10:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
### vue-i18n
在src/目录下新建lang文件夹
lang/文件中有en.js,index.js,zh.js
例如en.js
```
export default{
    route:{
        'home':'home',
        'component':'component',
        'permission':'permission',
        'i18n':'i18n',
        'drag':'drag',
        'permission-instruction':'permission instruction',
        'permission-page':'permission page'
    },
    login:{
        'title':'admin-vue'
    }
   
}
```
zh.js
```
export default{
    route:{
        'home':'首页',
        'component':'组件',
        'permission':'权限',
        'i18n':'语言切换',
        'drag':'拖拽',
        'permission-instruction':'权限指令',
        'permission-page':'权限页面'
    },
    login:{
     'title':'后台登录'
    }
}
```
index.js
```
import Vue from 'vue'
import VueI18n from 'vue-i18n'
Vue.use(VueI18n)
import elementEnLocale from 'element-ui/lib/locale/lang/en' // element-ui lang
import elementZhLocale from 'element-ui/lib/locale/lang/zh-CN'// element-ui lang
import enLocale from './en'
import zhLocale from './zh'
const messages={
    en:{ 
        ...enLocale,
        ...elementEnLocale
    },
    zh:{ 
      ...zhLocale,
      ...elementZhLocale
    }
}
const i18n=new VueI18n({
    locale:'zh',
    messages,
})
export default i18n
```
在main.js中引入
```
import i18n from './lang/index.js'
new Vue({
  el: '#app',
  i18n,
  components: { App },
  template: '<App/>'
})
```
在组件中的使用
在模板中使用<p>{{ $t("route.home") }}</p>
在有的项目中需要循环侧边栏导航，所以写了一个工具方法
```
const generateTitle=function(title){
    const hasKey=this.$te('route.'+title)
    const translatedTitle=this.$t('route.'+title)
    if(hasKey){
        return translatedTitle
    }
   
    return title
}
export {generateTitle}
```
在组件中使用时，引入这个工具方法
### 语言的切换
### vue-i18n其他的国际化
请参考
[vue-i18n](https://kazupon.github.io/vue-i18n/guide/formatting.html#list-formatting)
### jquery-i18n-properties
这是不用vue时，js的前端国际化
一般会新建一个文件存放语言信息
i18n/language_en.properties
```
language_text=Hello world!
language_lang=Chinese
```
i18n/language_zh.properties
```
language_text=你好，世界
language_lang=中文
```
html文件
```
<script src='xxx/jquery-i18n-properties.js'></script>
<div>
        <p id="text" data-locale='string_text'></p>
        <button onclick="switchLang()" id="lang" data-locale='string_lang'></button>
    </div>
<script>
  var LANGUAGE_CODE = "zh"; //标识语言
function loadProperties(type) {
    jQuery.i18n.properties({
        name: 'language', // 资源文件名称
        path: 'i18n/', // 资源文件所在目录路径
        mode: 'map', // 模式：变量或 Map
        language: type, // 对应的语言
        cache: false,
        encoding: 'UTF-8',
        callback: function () { // 回调方法
           $("[data-locale]").each(function(){
                    console.log($(this).data("locale"));
                    $(this).html($.i18n.prop($(this).data("locale")));
            });
        }
    });
}
function switchLang() {
    LANGUAGE_CODE = LANGUAGE_CODE == 'zh' ? 'en' : 'zh';
    loadProperties(LANGUAGE_CODE);
}
$(document).ready(function () {
    LANGUAGE_CODE = jQuery.i18n.normaliseLanguageCode({}); //获取浏览器的语言
    loadProperties(LANGUAGE_CODE);
})
</script>
```
参考[jquery-i18n-properties](https://github.com/jquery-i18n-properties/jquery-i18n-properties)
