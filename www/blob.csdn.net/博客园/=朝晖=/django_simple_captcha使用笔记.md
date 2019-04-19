# django_simple_captcha使用笔记 - =朝晖= - 博客园
# [django_simple_captcha使用笔记](https://www.cnblogs.com/dhcn/p/10235998.html)
一、先来[官方文档](https://django-simple-captcha.readthedocs.io/en/latest/usage.html)的步骤：
- Install `django-simple-captcha` via [pip](http://pypi.python.org/pypi/pip): `pip install  django-simple-captcha`
- 
Add `captcha` to the `INSTALLED_APPS` in your `settings.py`
- 
Run `python manage.py migrate`
- 
Add an entry to your `urls.py`:
```
urlpatterns += [
    url(r'^captcha/', include('captcha.urls')),
]
```
二、开始改代码，给需要验证码认证的form加个captcha字段：
```
from django import forms
from captcha.fields import CaptchaField
class CaptchaTestForm(forms.Form):
    captcha = CaptchaField()
```
三、单独字段的渲染
1、给模板准备form
```
form = CaptchaTestForm()  
return TemplateResponse(request,"to_render_form.html",{"form":form})
```
2、在模板中渲染验证码字段：
```
{{ form.captcha }}
```
四、定制字段的渲染格式
1、整个字段部分的排版，用了bootstrap3的样式 field.html：
```
<div class="form-inline" style="margin-left: 15px">
    <div class="form-group"><label for="captcha">验证码：</label>{{text_field}}{{image}}{{hidden_field}}</div>
</div>
```
2、text input的样式定制 text_field.html：
```
<div class="form-inline" style="margin-left: 15px">
    <div class="form-group"><label for="captcha">验证码：</label>{{text_field}}{{image}}{{hidden_field}}</div>
</div>
```
3、验证码图片点击刷新
```
$('.captcha').click(function () {
        $.getJSON("/captcha/refresh/", function (result) {
        $('.captcha').attr('src', result['image_url']);
        $('#id_captcha_0').val(result['key'])
        });
    });
```
五、因为整个字段的排版文件和outputformat配置会冲突，所以两个都要改的话，必须配一下排版文件配置来覆盖output format配置：
```
# django_simple_captcha settings
CAPTCHA_FIELD_TEMPLATE = "captcha/field.html"
CAPTCHA_TEXT_FIELD_TEMPLATE = "captcha/text_field.html"
# format
#CAPTCHA_OUTPUT_FORMAT = u'%(text_field)s %(hidden_field)s %(image)s'
# 噪点样式
#CAPTCHA_NOISE_FUNCTIONS = (
#    # 'captcha.helpers.noise_null', # 没有样式
#    'captcha.helpers.noise_arcs', # 线
#    'captcha.helpers.noise_dots', # 点
#)
# 图片大小
#CAPTCHA_IMAGE_SIZE = (120, 50)
#CAPTCHA_BACKGROUND_COLOR = '#ffffff'
#CAPTCHA_CHALLENGE_FUNCT = 'captcha.helpers.random_char_challenge' # 图片中的文字为随机英文字母，如 mdsh
#CAPTCHA_LENGTH = 5 # 字符个数
```

