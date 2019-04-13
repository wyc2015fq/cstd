
# js实现手机短信验证码倒计时 - wangzhiguo9261的博客 - CSDN博客


2018年10月18日 20:12:18[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：32个人分类：[js](https://blog.csdn.net/wangzhiguo9261/article/category/7508784)


|<div class=|"user-form"|>
|<form action=|"{{ path('zm_member_register') }}"|method=|"post"|>
|<div class=|"form-list"|>
|<label class=|"register-label"|>手机号码</label>
|<input class=|"regphone input-register"|type=|"text"|name=|"phone"|placeholder=|"请输入手机号码"|/>
|</div>
|<div class=|"form-list"|>
|<label class=|"register-label"|>验证码</label>
|<input class=|"input-short"|type=|"text"|name=|"sms_salt"|placeholder=|"请输短信验证码"|/>
|<input class=|"input-code"|id=|"btn"|type=|"button"|value=|"发送验证码"|/>
|</div>
|<input style=|"margin-top: 60px;"|type=|"submit"|class=|"registerSubmit form-sumbit"|value=|"提交"|/>
|</form>
|</div>
|
这里的验证码是通过向后台这个url({{ path(‘zm_member_get_salt') }}))里面，传手机号码和类型（type=1为注册）这两个值，后台接收值成功则返回成功状态值。
基于这里，实现验证码倒计时，即可以在判断成功后。调用封装了的倒计时函数 time()，注意验证码应使用type为button的input，此时可以方便地更改其value值，来展示倒计时的时间。
[?](https://www.jb51.net/article/95827.htm#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|<script type=|"text/javascript"|>
|//倒计时60秒
|var|wait=60;
|function|time(o) {
|if|(wait == 0) {
|o.removeAttribute(|"disabled"|);
|o.value=|"获取动态码"|;
|wait = 60;
|}|else|{
|o.setAttribute(|"disabled"|,|true|);
|o.value=|"重新发送("|+ wait +|")"|;
|wait--;
|setTimeout(|function|() {
|time(o)
|}, 1000)
|}
|}
|$(|'.input-code'|).click(|function|() {
|var|phone = $(|'.regphone'|).val();
|$.ajax({
|type:|'post'|,
|url:|"{{ path('zm_member_get_salt') }}"|,
|data: {
|phone: phone,
|type: 1
|},
|dataType:|'json'|,
|success:|function|(result) {
|if|(result.flag == 1) {
|// alert('成功');
|time(btn);
|}|else|{
|alert(result.content);
|}
|}
|});
|});
|</script>
|
以上所述是小编给大家介绍的JS实现用户注册时获取短信验证码和倒计时功能，希望对大家有所帮助，如果大家有任何疑问请给我留言，小编会及时回复大家的。在此也非常感谢大家对脚本之家网站的支持

