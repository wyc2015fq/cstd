# egg(114)--egg之订单页面切换选中 - weixin_33985507的博客 - CSDN博客
2019年02月21日 01:28:10[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## router
```
router.get('/user/welcome', initMiddleware, userauthMiddleware, controller.default.user.welcome);
    router.get('/user/order', initMiddleware, userauthMiddleware, controller.default.user.order);
    router.get('/user/orderinfo', initMiddleware, userauthMiddleware, controller.default.user.orderinfo);
```
## 中间件
传递当前url
> appmiddlewareuserauth.js
`ctx.state.url = url.parse(ctx.request.url).pathname;`
## controller
> appcontrollerdefaultuser.js
```
'use strict';
const Controller = require('egg').Controller;
class UserController extends Controller {
    async welcome() {
        await this.ctx.render('default/user/welcome.html');
    }
    async order() {
        const uid = this.ctx.service.cookies.get('userinfo')._id;
        const page = this.ctx.request.query.page || 1;
        var json = { "uid": uid }; //查询当前用户下面的所有订单
        const pageSize = 2;
        // 总数量
        const totalNum = await this.ctx.model.Order.find(json).countDocuments();
        //聚合管道要注意顺序
        const result = await this.ctx.model.Order.aggregate([{
                $lookup: {
                    from: 'order_item',
                    localField: '_id',
                    foreignField: 'order_id',
                    as: 'orderItems',
                },
            },
            {
                $sort: { "add_time": -1 }
            },
            {
                $match: { "uid": this.app.mongoose.Types.ObjectId(uid) } //条件
            },
            {
                $skip: (page - 1) * pageSize,
            },
            {
                $limit: pageSize,
            }
        ]);
        await this.ctx.render('default/user/order.html', {
            list: result,
            totalPages: Math.ceil(totalNum / pageSize),
            page,
        });
    }
    async orderinfo() {
        // this.ctx.body = '用户订单';
        await this.ctx.render('default/user/order_info.html');
    }
    async address() {
        this.ctx.body = '收货地址';
    }
}
module.exports = UserController;
```
## view
### 左侧导航
> appviewdefaultuseruser_left.html
```
<div class="ddzx">用户中心</div>
<div class="subddzx">
    <ul>
        <li <%if(url=='/user/welcome' ){%> class="active"
            <%}%>><a href="/user/welcome">欢迎页面</a></li>
        <li <%if(url=='/user/order' || url=='/user/orderinfo' ){%> class="active"
            <%}%>><a href="/user/order">我的订单</a></li>
        <li><a href="#">用户信息</a></li>
        <li><a href="#">我的收藏</a></li>
        <li><a href="#">我的评论</a></li>
    </ul>
</div>
```
### 欢迎页
> appviewdefaultuserwelcome.html
`<% include  ./user_left.html%>`
```
<div class="grzxbj">
            <div class="selfinfo center">
                <div class="lfnav fl">
                    <% include  ./user_left.html%>
                </div>
                <div class="rtcont fr">
                    <div class="portal-content-box">
                        <div class="box-bd">
                            <div class="portal-main clearfix">
                                <div class="user-card">
                                    <h2 class="username">2152135723</h2>
                                    <p class="tip">晚上好</p>
                                    <a class="link" href="https://account.xiaomi.com/pass/userInfo" target="_blank" data-stat-id="30d619c492c43471" onclick="_msq.push(['trackEvent', 'f4f3444fdfa3d27a-30d619c492c43471', 'https://account.xiaomi.com/pass/userInfo', 'pcpid', '']);">修改个人信息 ></a>
                                    <img class="avatar" src="https://account.xiaomi.com/static/img/passport/photo.jpg" width="150" height="150" alt="2152135723">
                                </div>
                                <div class="user-actions">
                                    <ul class="action-list">
                                        <li>账户安全：<span class="level level-2">普通</span></li>
                                        <li>绑定手机：<span class="tel">150********01</span></li>
                                        <li>绑定邮箱：<span class="tel"></span><a class="btn btn-small btn-primary" href="https://account.xiaomi.com/pass/userInfo" target="_blank" data-stat-id="d36648c4ef44cb77" onclick="_msq.push(['trackEvent', 'f4f3444fdfa3d27a-d36648c4ef44cb77', 'https://account.xiaomi.com/pass/userInfo', 'pcpid', '']);">绑定</a></li>
                                    </ul>
                                </div>
                            </div>
                            <div class="portal-sub">
                                <ul class="info-list clearfix">
                                    <li>
                                        <h3>待支付的订单：<span class="num">0</span></h3>
                                        <a href="//static.mi.com/order/?type=7" data-stat-id="ff48b3f50874dae7">查看待支付订单</a>
                                        <img src="//s01.mifile.cn/i/user/portal-icon-1.png" alt="">
                                    </li>
                                    <li>
                                        <h3>待收货的订单：<span class="num">0</span></h3>
                                        <a href="//static.mi.com/order/?type=8" data-stat-id="f7b15ff4b8710895">查看待收货订单</a>
                                        <img src="//s01.mifile.cn/i/user/portal-icon-2.png" alt="">
                                    </li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="clear"></div>
            </div>
        </div>
```
### 订单页
` <% include  ./user_left.html%>`
### 订单详情页
`<% include  ./user_left.html%>`
## 效果
### 欢迎页
![clipboard.png](https://image-static.segmentfault.com/428/135/4281351533-5c6e48de10f92_articlex)
### 订单页
![clipboard.png](https://image-static.segmentfault.com/235/587/2355870522-5c6e48eb5c98d_articlex)
### 订单详情页
![clipboard.png](https://image-static.segmentfault.com/352/334/3523345725-5c6e48f7b8a16_articlex)
