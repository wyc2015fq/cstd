# Django之代码风格 - =朝晖= - 博客园
# [Django之代码风格](https://www.cnblogs.com/dhcn/p/7121283.html)
# 1 代码风格
稍微关注一下下面这些代码标准风格指导规则将会对你大有益处，我们高度建议你通读词章，即便你此时可能正想跳过它。
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#11-让你的代码保持可读性的重要性)1.1 让你的代码保持可读性的重要性
代码在读方面的重要性胜过写。一个代码块几分钟编写，花几个小时调试，然后有可能再也不会有人去碰它。然而当你去阅读那些昨天或者甚至10年以前编写的代码，清晰一致的代码风格就变得非常有用。清晰移动的代码能够让你不用去费劲去理解代码的不一致性，从而节省了自己的脑力，并且使代码易于维护，提高了各种规模项目的整体质量。
下面这些方法让你"多走一公里"使你的代码尽可能清晰易读：
- 避免缩写的变量名称
- 写出你的函数的参数名称
- 对你的类和方法要写注释文档
- 要对你的代码加注释
- 把重复的代码重构为可复用的函数和方法
- 使函数和方法短一点，一个好的经验法则是读函数和方法代码时不需要滚动鼠标(也就是函数或方法的长度不能大于一个屏幕的显示高度)
当你长时间不碰某段代码后，再重新拿起这段代码，你能很容易地找到当初是从哪儿中断编写工作的
缩写变量名是非常令人讨厌的。比如：当你看到一个`balance_sheet decrease`的变量名时，解释它肯定要比解释缩写版的`bsd`或者`bal_s_d`要容易。这种类型的缩写可能会帮你省一点输入时间，但是这种节省是以几小时甚至几天的技术(代码阅读)债为代价，所以这种节省不值当。
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#12-pep8)1.2 PEP8
PEP8是一个官方的[Python](http://lib.csdn.net/base/python)代码风格指南。我们建议详细认真地阅读它并学会遵守PEP8的代码约定:([http://www.python.org/dev/peps/pep-0008/](http://www.python.org/dev/peps/pep-0008/))
PEP8描述了类似下面这样的代码约定:
- “每一级代码缩进使用4个空格”
- “独立的顶级函数或者类定义语句下面必须有2个空行”
- “类中的方法定义之间必须用一个空行隔开”
在Django项目中的所有[python](http://lib.csdn.net/base/python)代码文件必须遵守PEP8。如果你记不住PEP8的代码规则，你可以为你的代码编辑器找一个检查你输入代码是否符合PEP8的插件。
当一个有经验的Python程序员在Django项目中看到对PEP8规范的严重违反，即便他当时什么也不说，也在想这是一件很坏的事情。关于这一点，一定要相信我们。
警告：不要改变现存项目的代码约定
PEP8的代码规范一般仅对新的Django项目产生约束，如果你新参与一个项目，这个项目使用的代码约定和PEP8规范不同，那你就遵守项目目前的代码约定即可。
请阅读PEP8规范的第8章节：愚蠢的一致性是令人讨厌的想法，这个规则正好适用于我们此时不死守PEP8规范的情况：
- [http://2scoops.co/hobgoblin-of-little-minds](http://2scoops.co/hobgoblin-of-little-minds) |
**构建技巧：使用flake8检查代码质量
flake8这个项目由Tarek Ziadé发起，由PyCQA小组维护，这是一个非常有用的检查项目代码风格、质量和逻辑错误的命令行工具。当你在本地开发时，可以把它作为持续集成的一个工具组件使用。
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#121-79字符限制)1.2.1 79字符限制
`这不是开玩笑，我仍旧把命令行控制台控制在80个字符宽度`
--- Barry Morrison：系统工程师兼本书每一个版本的审阅者
根据PEP8规范，每行的文本长度限制是79个字符。这样设计是因为79在文本环绕编辑器里面比较安全，开发团队在无需代码可读性的前提下即能够适应。
然后PEP8也提供一个弹性措施：可以为专属团队项目把这个限制提到99个字符。我们理解这个所谓的专属团队项目也就是非开源项目。
下面是我们的偏好设定：
- 一个开源项目的行字符数必须硬性限制在79个字符以内，我们的经验已经证明：开源项目的代码贡献者和访问者会对代码行长度这个问题有抱怨。
- 在私有项目上，我们把代码行长度限制放宽到99个字符，充分利用现代的显示器。
请阅读：([http://www.python.org/dev/peps/pep-0008/#maximum-line-length](http://www.python.org/dev/peps/pep-0008/#maximum-line-length))
提示：Aymeric Augustin关于代码行长度问题的观点
Django核心开发者Aymeric Augustin曾说:"把代码行长度限制在79个字符这个限制，不是给变量、函数、类起糟糕名字的合理理由。给变量起一个可读性的名字比遵守几十年以前的硬件限制而引起的约束更重要。"
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#13-关于import语句的代码规范)1.3 关于import语句的代码规范
PEP8规范建议import语句必须下面的规则排序分组：
- 标准库导入
- 相关的第三方库导入
- 本地应用或者特定库的导入
打我们开发一个Django项目时，import语句应该像下面例子这样
```
Example1.1
# Stdlib imports
from __future__ import absolute_import
from math import sqrt
from os.path import abspath
# Core Django imports
from django.db import models
from django.utils.translation import ugettext_lazy as _
# Third-party app imports
from django_extensions.db.models import TimeStampedModel
# Imports from your apps
from splits.models import BananaSplit
```
(注意：你在实际项目中不需要像上面这个例子这样注释import语句，此处的注释只是为了说明例子演示的规则)
Django项目中import语句的顺序是：
- 标准库导入
- 导入Django内建资源
- 从第三方App的导入，包括那些与Django没有关系的App
- 从Django项目所建App的导入(你将在第四章读到Django App相关的内容，第四章的主题是App设计基础)
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#14-使用显式相对路径导入)1.4 使用显式相对路径导入
当你写代码时，使用显式相对路径导入这是很重要，因为这样可以让你便于移动、重命名、版本化你的模块包。在Python编程中，使用显式相对路径导入可以减少你的模块包的硬代码量要求，可以把模块包从它紧密依赖的[架构](http://lib.csdn.net/base/architecture)环境中独立出来。因为Django App也是包的形式，所以个编码规则也适用于Django App编程。
为了演示显式相对路径导入方式的益处，让我们看一个例子：
假定下面的代码片段摘自一个冰激凌消费量跟踪Django项目,其中消费的冰激凌品类包括你们曾经吃过的华夫、甜筒、暴风雪。
哦，不，你的cones Django App里面有全路径硬编码导入，这太糟了:
```
Bad Example 1.1 
# cones/views.py
from django.views.generic import CreateView
# DON'T DO THIS!
# Hardcoding of the 'cones' package
# with implicit relative imports
from cones.models import WaffleCone 
from cones.forms import WaffleConeForm 
from core.views import FoodMixin
class WaffleConeCreateView(FoodMixin, CreateView):
    model = WaffleCone
    form_class = WaffleConeForm
```
确实，你的cones App在你的冰激凌消费量跟踪项目里面可以正确执行，但是它里面的那些硬编码导入会使它本身的可移植性和可复用性非常差:
- >如果你想在另外一个跟踪通用甜食的Django项目中复用你的cones App,而同时却因为命名冲突不得不修改App的名字,你会怎样做？
- >如果你有时候只是简单想改App的名字，那怎么做？
如果硬编码绝对路径导入，你在修改时就不能只改App的名字，而是不得不找到所有的此类导入并全部修改。虽然手工修改他们不难，但在你驳回相对路径导入这个办法前，请想想与那些带有各种各样的附加功能模块的App相比，上面这个例子是非常简单的。
现在让我们把上面这个硬编码绝对路径导入的代码片段改造成相对路径导入的好代码，下面是修正后的例子：
```
Example 1.2
# cones/views.py
from __future__ import absolute_import
from django.views.generic import CreateView
# Relative imports of the 'cones' package
from .models import WaffleCone
from .forms import WaffleConeForm
from core.views import FoodMixin
class WaffleConeCreateView(FoodMixin, CreateView):
    model = WaffleCone
    form_class = WaffleConeForm
```
另外一个具体的优势是我们可以立即辨别出本地导入和全局导入，强调Python包是一个代码单元这一点。
提示："from future import absolute_import"语句的使用
Python3以一种更好的方式更新和改进了import语句运行机制。幸运的是：我们可以使用`from __future__ import absolute_import`这条语句做到对Python2.7向后兼容。即使你不计划使用Python3,但是考虑到下面表中的相对路径导入，这也是一个很棒的方法。
总结一下，下面的表格给出了不同的Python导入类型以及在Django项目中何时使用它们。
|导入代码|导入类型|使用场景|
|----|----|----|
|from core.views import FoodMixin|绝对路径导入|从目前App外部导入|
|from .models import WaffleCone|显式相对路径导入|从目前App的另一个模块导入|
|from models import WaffleCone|隐式相对路径导入|从目前App的另一个模块导入，但不推荐这么做|
表格 1.1 导入类型 绝对路径导入 VS 显式相对路径导入 VS 隐式相对路径导入
养成使用显式相对路径导入的习惯，这非常容易做到，而且这也是一个Python程序员开发好习惯。
提示：PEP328规范不和PEP8规范冲突吗?
请看Python创始人吉多·范罗苏姆是怎么说的：
- >([http://2scoops.co/guido-on-pep-8-vs-pep-328](http://2scoops.co/guido-on-pep-8-vs-pep-328))
相关阅读: [http://www.python.org/dev/peps/pep-0328/](http://www.python.org/dev/peps/pep-0328/)
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#15-避免使用import-)1.5 避免使用import *
在我们所有工作中的99%的情况下，我们都应该显式导入每个模块：
```
Example 1.3
from django import forms 
from django.db import models
```
不要写下面这样的代码：
```
Bad Example 1.2
# ANTI-PATTERN: Don't do this! 
from django.forms import * 
from django.db.models import *
```
不这样做的原因是避免银式导入另外一个python模块的内部变量而导致覆盖目前模块命名空间内的变量，那样会导致不可预知的结果，有时甚至是灾难性的结果。
我们将在本书内容中讲解一个此规则的例外情况，这个例子将在第五章(章名以后再译)中出现。
我们再来看一下上面的反面案例,在Django models库模块和Django forms库模块中都有一个叫CharField的类，models库中的类会覆盖掉forms库中的同名类，这种情况也会发现在Python内建库和其他第三方库之间，那样就会导致关键功能特性对象被覆盖。
警告：Python命名冲突 假如你试图导入两个具有相同命名的对象时，也会发生相同的相似的命名覆盖问题
```
Bad Example 1.3
# ANTI-PATTERN: Don't do this! 
from django.forms import CharField 
from django.db.models import CharField
```
使用import *方式，就像一个贪婪的客人进入冰激凌店后试品了所有三十一种口味冰激凌，最后却只买了1到2个球。如果你只使用模块中1到2个对象，就不要试图导入模块中所有的对象引用。
当然如果客人走出冰激凌店的时候捧着一个巨大的冰激凌碗，而里面盛着几乎所有口味的冰激凌球，当然，这是另外一回事了。
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#16-django代码风格)1.6 Django代码风格
这一节内容将会覆盖Django官方代码约定规范以及非官方但是普遍被接受的代码规范
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#161-考虑一下django代码风格指南)1.6.1 考虑一下Django代码风格指南
不用说，熟悉通用Django风格约定是个好做法。实际上，Django内部有它自己的代码风格指南，这个指南基于PEP8扩展而成。
- >[http://2scoops.co/1.8-coding-style](http://2scoops.co/1.8-coding-style)
另外，如果有内容不在官方代码标准里面明确指明，那在Django社区也会有非常常见的此类代码风格约定可以供你在项目中使用。
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#162-在ulr模式名称定义里使用下划线而不是破折号中划线)1.6.2 在ULR模式名称定义里使用下划线而不是破折号(中划线)
我们日常使用下划线(字符"-")而不是破折号(字符"-"),这样做不仅仅是因为更Pythonic,而且对更多的IDE和文本编辑器也更友好。注意我们这里下面例子里说的是url()函数的name参数的值，而不是浏览器里面实际输入的URL片段(译注：正则表达式那段)。
像下面这样在url名称里面用中划线是错误的方式：
```
Bad Example 1.4
patterns = [
    url(regex='^add/$',
        view=views.add_topping,
        name='add-topping'),
    ]
```
正确的方式，是在url名称里面用下划线：
```
Example 1.4
patterns = [
    url(regex='^add/$',
        view=views.add_topping,
        name='add_topping'),
    ]
```
注意：我们这里指的是url函数name参数的值，不是浏览器实际输入的URL片段，在URL片段里面用中划线是可以的，(比如:regex='^add-topping/$')。
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#163-在模板block名称里面用下划线而不是中划线)1.6.3 在模板block名称里面用下划线，而不是中划线
与在URL模式名称里面用下划线一样的原因，我们建议在定义模板block时也使用下环线：这样的话就可以更Pythonic且对编辑器更友好。
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#17-选择javascripthtml和css的代码风格指南)1.7 选择JavaScript、HTML和CSS的代码风格指南
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#171-javascript代码风格指南)1.7.1 JavaScript代码风格指南
不像Python有官方的代码风格指南，[JavaScript](http://lib.csdn.net/base/javascript)不存在官方代码风格指南，相代替的是，有很多个人或公司编纂的的[javascript](http://lib.csdn.net/base/javascript)非官方代码风格指南:
- idiomatic.js: 编写一致通顺的JavaScript代码的建议原则([https://github.com/rwaldron/idiomatic.js/](https://github.com/rwaldron/idiomatic.js/))
- Pragmatic.js 代码风格指南:([https://github.com/madrobby/pragmatic.js](https://github.com/madrobby/pragmatic.js))
- Airbnb代码风格指南([https://github.com/airbnb/javascript](https://github.com/airbnb/javascript))
- Node.js风格指南([https://github.com/felixge/node-style-guide](https://github.com/felixge/node-style-guide))
- JavaScript编程语言代码约定([http://javascript.crockford.com/code.html](http://javascript.crockford.com/code.html))
在Django或JavaScript社区对于上面的代码风格指南都没有一致的意见，你只需要选择你偏好的并坚持它即可。
当然，如果你使用一个自带代码风格约定的[JavaScript框架](http://lib.csdn.net/base/angularjs)，你就应该使用它自带的代码约定。比如ember.[js](http://lib.csdn.net/base/javascript)就有其自带的代码约定。
包提示：JSCS代码风格审查器
JSCS( [http://jscs.info/](http://jscs.info/) ) 是一个JavaScript代码风格审查工具。它预设好几种JavaScipt代码风格指南的编码规则,包括前面列出的代码约定中的一些。有面向好几种代码文本编辑器的JSCS插件，还有面向Gulp和Grunt之类构建工具的JSCS检查任务脚本。
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#172-html和css代码风格指南)1.7.2 HTML和CSS代码风格指南
- 由@mod编写的HTML和CSS代码风格指南：([http://codeguide.co](http://codeguide.co/))
- idomatic-css: 编写一致通顺的CSS代码的编码原则:([https://github.com/necolas/idiomatic-css](https://github.com/necolas/idiomatic-css))
包提示：CSScomb
CSScomb ( [http://csscomb.com/](http://csscomb.com/) )一个CSS代码风格格式化工具。它检查你预配置好的风格的代码一致性和CSS属性的排序规则，就像JSCS一样，CSScomb也有面向文本编辑器的插件和面向任务构建工具的插件，其中包括面向brunch这样的[HTML5](http://lib.csdn.net/base/html5)构建工具的版本。
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#18-不要面向ide或者文本编辑器编程)1.8 不要面向IDE(或者文本编辑器)编程
某些程序员在做出关于项目的文件布局和封装实现的选择时可能是基于IDE(集成开发环境)的特性来考虑,这对于那些选择不同不同开发工具的程序员试图来探索你的项目代码时将变得非常困难。
经常假设你身边的程序员和你采用不同的工具，并且你的项目代码结构足够清晰易懂，以便于他们即使使用NotePad或者Nano都可以探索你的代码。
比如：深思模板标签且查看标签源码对于那些使用功能有限的IDE工具的程序员来说是非常困难切耗时的，因此我们使用约定的标签模块命名方式：_tags.py。
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C1%20%E4%BB%A3%E7%A0%81%E9%A3%8E%E6%A0%BC.md#19-小结)1.9 小结
这一章包括了我们推荐的代码风格指南以及解释我们偏好每个技巧的原因。
即使你不遵守我们推荐的代码约定，也请你遵守任何一个统一的代码风格指南。有多种代码风格的项目代码是不易维护的，并且会拖慢快发速度且提高发生开发失误的可能性。

