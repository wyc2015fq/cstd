# CSS三栏布局5种解决方案 - weixin_33985507的博客 - CSDN博客
2017年08月13日 11:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
### 浮动布局
```
<!-- 浮动布局 -->
    <section class="layout float">
        <style>
            .layout.float .left {
                width: 100px;
                float: left;
                background: red;
            }
            .layout.float .right {
                width: 100px;
                float: right;
                background: blue;
            }
            .layout.float .center {
                background: yellow;
            }
        </style>
        <article class="container">
            <div class="left">左边</div>
            <div class="right">右边</div>
            <div class="center">
                <h2>浮动布局</h2>
                浮动布局浮动布局 浮动布局浮动布局 
                浮动布局浮动布局 浮动布局浮动布局
                浮动布局浮动布局 浮动布局浮动布局 
            </div>
        </article>
    </section>
```
缺点: 需要清除浮动.
优点: 兼容性好
**注: center 盒子必须放在最后**
### 绝对定位布局
```
<!-- 绝对定位布局 -->
    <section class="layout absolute">
        <style>
            .layout.absolute .left {
                position: absolute;
                left: 0;
                width: 100px;
                background: red;
            }
            .layout.absolute .right {
                width: 100px;
                right: 0;
                position: absolute;
                background: blue;
            }
            .layout.absolute .center {
                position: absolute;
                left: 100px;
                right: 100px;
                background: yellow;
            }
        </style>
        <article class="container">
            <div class="left">左边</div>
            <div class="center">
                    <h2>绝对定位布局</h2>
                    绝对定位布局 绝对定位布局 
                    绝对定位布局 绝对定位布局
                    绝对定位布局 绝对定位布局 
                    绝对定位布局 绝对定位布局
                </div>
            <div class="right">右边</div>
        </article>
    </section>
```
**注: 这种办法会让整个布局脱离文档流.**
### flexbox布局
```
<!-- flexbox布局 -->
    <section class="layout flexbox">
        <style>
            .layout.flexbox .container {
                display: flex;
            }
            .layout.flexbox .left {
                width: 100px;
                background: red;
            }
            .layout.flexbox .right {
                width: 100px;
                background: blue;
            }
            .layout.flexbox .center {
                flex: 1;
                background: yellow;
            }
        </style>
        <article class="container">
            <div class="left">左边</div>
            <div class="center">
                <h2>flexbox布局</h2>
                flexbox布局 flexbox布局 
                flexbox布局 flexbox布局 
                flexbox布局 flexbox布局 
                flexbox布局 flexbox布局
            </div>
            <div class="right">右边</div>
        </article>
    </section>
```
缺点: 兼容性问题
优点: 解决以上两种布局存在的问题
**注:目前比较主流的移动端布局方式**
### 表格布局
```
<!-- 表格布局 -->
    <section class="layout table">
        <style>
            .layout.table .container {
                display: table;
                width: 100%;
                height: 100px;
            }
            .layout.table .container>div {
                display: table-cell;
            }
            .layout.table .left {
                width: 100px;
                background: red;
            }
            .layout.table .right {
                width: 100px;
                background: blue;
            }
            .layout.table .center {
                background: yellow;
            }
        </style>
        <article class="container">
            <div class="left">左边</div>
            <div class="center">
                <h2>表格布局</h2>
                表格布局表格布局表格布局
                表格布局表格布局表格布局
                表格布局表格布局表格布局
            </div>
            <div class="right">右边</div>
        </article>
    </section>
```
**注:该方式在实际项目开发中已经淘汰.**
### 网格布局
```
<!-- 网格布局 -->
    <section class="layout grid">
        <style>
            .layout.grid .container {
                display: grid;
                width: 100%;
                /* 设置高度 */
                grid-template-rows: 100px;
                /* 设置行数 */
                grid-template-columns: 100px auto 100px;
            }
            .layout.grid .left {
                background: red;
            }
            .layout.grid .right {
                background: blue;
            }
            .layout.grid .center {
                background: yellow;
            }
        </style>
        <article class="container">
            <div class="left">左边</div>
            <div class="center">
                <h2>网格布局</h2>
                网格布局网格布局网格布局
                网格布局网格布局网格布局
                网格布局网格布局网格布局
                网格布局网格布局网格布局
            </div>
            <div class="right">右边</div>
        </article>
    </section>
```
**注:黑科技**
