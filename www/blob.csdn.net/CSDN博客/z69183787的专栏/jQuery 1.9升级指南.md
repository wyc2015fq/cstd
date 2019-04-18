# jQuery 1.9升级指南 - z69183787的专栏 - CSDN博客
2013年07月24日 15:41:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：770
## [jQuery 1.9升级指南](http://www.css88.com/archives/5086)
### 时间：2013年02月18日作者：愚人码头查看次数:12,443 views评论次数：[9条评论](http://www.css88.com/archives/5086#comments)
原文地址：[http://jquery.com/upgrade-guide/1.9/](http://jquery.com/upgrade-guide/1.9/) 翻译的不对或者不通顺的地方欢迎拍砖留言。
## 概述
jQuery 1.9删除或修改了几个过去行为不一致或效率低下的几个API。他们在以前的jQuery版本中已经[标注过过时(deprecated)](http://www.css88.com/jqapi-1.9/category/deprecated/)的API，特别是1.7和1.8。
在做这些变化中，团队的目标是修复jQuery的行为不一致或比较难使用的地方，在这个过程中提高整体性能，减小了文件的体积。
此列表是看似给你一些不祥的预感。因为jQuery 1.9删除和修改一些以前做特殊需求的API，比如jQuery.browser。如何升级？首先，看看你的代码，最好的方法是尝试用jQuery 1.9和Migrate（迁移）插件（如下文所述）。
本指南作为标准的[jQuery API文档](http://www.css88.com/jqapi-1.9/)的附录，作为快速上手jQuery 1.9的引导。
## jQuery Migrate（迁移）插件
我们认识到，现有的网站和插件可能会受到这些变化的影响，所以提供一个过渡性的升级路径—— [jQuery Migrate（迁移）插件](https://github.com/jquery/jquery-migrate)。下面的说明中，在1.9中变化或删除的API，大部分可以使用jQuery Migrate（迁移）插件恢复。请注意，jQuery
 1.9中所有的变化也将应用到jQuery 2.0中，jQuery Migrate（迁移）插件在jQuery2.0中也是可用的。
未压缩，开发版本的jQuery Migrate（迁移）插件使用时会在控制台中显示警告信息，详细的指出不兼容或删除等信息及解决方法。这使得它在现有的jQuery代码和插件上查找和修复问题时非常有用。jQuery Migrate（迁移）插件包含了1.6.4以来存在但1.9已不支持所有API。
压缩版本的的jQuery Migrate（迁移）插件，不会在浏览器控制台中产生任何不兼容或删除等信息，并且可以在jQuery 1.9或更高版本，或者旧的不兼容的jQuery代码或插件中使用。
理想情况下，这将只能作为一个短期的解决方案，但是这要你自己做出决定。
更多的信息请参见：[jQuery Migrate plugin](https://github.com/jquery/jquery-migrate)。
译者注：哥过一句嘴，尽早的修改原来不兼容的代码，毕竟使用jQuery Migrate（迁移）插件不是一个长久之计啊。
## jQuery1.9中改变的地方
下面列表中说的并不代表jQuery 1.9中所有改变，只是我们预期的改变。完整的，详细的更改列表，请参阅[jQuery的博客](http://blog.jquery.com/)或访问[bugs.jquery.com](http://bugs.jquery.com/newticket?redirectedfrom=)中发布的公告。
#### **.toggle(function, function, … ) 方法删除**
这个方法绑定两个或多个处理程序到匹配的元素，用来执行交替的点击事件。它不应该被混同于显示或隐藏匹配元素[.toggle( )方法](http://www.css88.com/jqapi-1.9/toggle/)，因为它没有过时。前者被删除，以减少混乱和提高模块化程度。jQuery Migrate（迁移）插件可以恢复此功能。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/toggle-event/](http://www.css88.com/jqapi-1.9/toggle-event/)
#### **jQuery.browser() 方法删除**
jQuery.browser()方法从jQuery 1.3开始已经过时了，在1.9中被删除。 如果需要的话，jQuery Migrate（迁移）插件可以恢复此功能。我们建议如特征检测，请使用[Modernizr库](http://modernizr.com/)。
译者注：具体查看API  [http://www.css88.com/jqapi-1.9/jQuery.browser/](http://www.css88.com/jqapi-1.9/jQuery.browser/)
#### **.live() 方法移除**
.live()方法从jQuery 1.7开始已经过时了，在1.9中被删除。 我们建议使用.on()方法来替换升级你的代码。要完全匹配$(“a.foo”).live(“click”, fn)，例如，你可以写$(document).on(“click”, “a.foo”, fn)（译者注：因为原来的live事件其实是绑定在文档document上的）。有关详细信息，请参阅[.on()文档](http://www.css88.com/jqapi-1.9/on/)。在此期间，你可以使用jQuery
 Migrate（迁移）插件来恢复.live() 功能。
译者注：具体查看API  [http://www.css88.com/jqapi-1.9/on/](http://www.css88.com/jqapi-1.9/on/) 和  [http://www.css88.com/jqapi-1.9/live/](http://www.css88.com/jqapi-1.9/live/)
#### **.die() 方法移除**
.die()方法从jQuery 1.7开始已经过时了，在1.9中被删除。 我们建议使用. off()方法来替换升级你的代码。要完全匹配$(“a.foo”).die(“click”)，例如，你可以写$(document).off(“click”, “a.foo”)。有关详细信息，请参阅[.off()文档](http://www.css88.com/jqapi-1.9/off/)。在此期间，你可以使用jQuery
 Migrate（迁移）插件来恢复. die() 功能。
译者注：具体查看API  [http://www.css88.com/jqapi-1.9/off/](http://www.css88.com/jqapi-1.9/off/) 和 [http://www.css88.com/jqapi-1.9/die/](http://www.css88.com/jqapi-1.9/die/)
#### **jQuery.sub() 方法移除**
jQuery.sub()方法被移到jQuery Migrate（迁移）插件中。使用的频率证明它没有必要再保留在核心代码库中，jQuery Migrate（迁移）插件添加了此功能。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/jQuery.sub/](http://www.css88.com/jqapi-1.9/jQuery.sub/)
#### **.add() 方法修改**
.add()方法返回的结果总是按照节点在document（文档）中的顺序排列。在1.9之前，如果上下文或输入的集合中任何一个以脱离文档的节点（未在文档中）开始，使用.add()方法节点不会按照document（文档）中的顺序排序。现在，返回的节点按照文档中的顺序排序，并且脱离文档的节点被放置在集合的末尾。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/add/](http://www.css88.com/jqapi-1.9/add/)
#### **.addBack(selector) 替换 .andSelf()**
从jQuery1.8开始，.andSelf()方法已经被标注过时，在jQuery1.8和更高版本中应使用.addBack()。
我们认为对于“添加并返回”以前的结果集合这是一个更好的名字。新方法可以接受一个可选的选择器，该选择器可以用来过滤之前集合，将它添加到当前集合并返回。$(“section, aside”).children(“ul”).addBack(“aside”) 会根据他们在文档中的顺序，得到section与aside下所有ul子元素，外加所有aside元素。虽然.addSelf()在1.9中仍然可以使用，我们建议您尽快修改名称。如果使用.addSelf()，jQuery Migrate插件会提出警告。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/andSelf/](http://www.css88.com/jqapi-1.9/andSelf/) 和 [http://www.css88.com/jqapi-1.9/addBack/](http://www.css88.com/jqapi-1.9/addBack/)
#### **.after(), .before(), 和 .replaceWith()使用脱离文档的节点**
1.9以前，.after(), .before(), 和 .replaceWith()将尝试在当前的jQuery集合中添加或改变节点，如果在当前的jQuery集的节点未连接到文档（注：即脱离文档的节点），在这种情况下，返回一个新的jQuery集合，而不是原来的那个集合。这将产生一些前后矛盾和彻底的错误 – 该方法可能会，也可能不会返回一个新的结果，这取决于它的参数！从1.9开始，这些方法总是返回原始未修改集并且试图在一个没有父节点的节点上使用.after(), .before(), or .replaceWith()
 有没有任何效果 – 即这个集或它包含的节点两者都不被改变。
#### **.appendTo, .insertBefore, .insertAfter, 和 .replaceAll**
在1.9中，这些方法总是返回一个新的集合，使他们可以使用的链式调用和.end()方法。1.9之前，只有当他们是一个单独的目标元素时，他们将返回旧的集合。需要注意的是这些方法总是返回所有元素附加到目标元素的聚合集合。如果没有元素被目标选择器选中（例如，$(elements).appendTo(“#not_found”)）那么返回的集合是空的。
#### **AJAX 事件需要绑定到document**
在jQuery 1.9中， 全局的AJAX事件(ajaxStart, ajaxStop, ajaxSend, ajaxComplete, ajaxError, and ajaxSuccess) 只能在document元素上触发。修改AJAX事件监听程序到document元素上。例如，如果目前的代码看起来像这样：
|`1`|`$(``"#status"``).ajaxStart(``function````(){ $(````this``).text(````"Ajax started"``````); });```|
修改成：
|`1`|`$(document).ajaxStart(``function````(){ $(````"#status"``).text(````"Ajax started"``````); });```|
译者注：具体查看API [http://www.css88.com/jqapi-1.9/category/ajax/global-ajax-event-handlers/](http://www.css88.com/jqapi-1.9/category/ajax/global-ajax-event-handlers/)
#### **.trigger() “click” 事件时Checkbox/radio 的状态**
当用户点击一个复选框或单选按钮时，如果节点上没调用event.preventDefault(),事件处理程序中会根据复选框或单选按的当前状态判断并且得到它的新状态。因此，例如，如果用户点击一个未选中的checkbox，事件处理程序将选中（checked）这个checkbox。1.9之前，.trigger(“click”) 或 .click()任何一个将触发一个合成事件，根据用户点击行为，我们可以看到checkbox与实际checked属性相反的状态。在1.9中修复了这个bug，用户行为会得到相应的状态。
#### **focus事件触发顺序**
当用户再表单元素上点击或者按tab键，使元素获取焦点，浏览器首先在焦点元素上触发一个blur(失去焦点)事件，然后在新元素上触发一个focus(获取焦点)事件。在1.9之前，使用.trigger(“focus”) 或 .focus() 绑定一个focus事件，新元素将触发一个focus事件，然后触发先前焦点元素的blur事件，1.9已修正此问题。
如果目标元素没有获取焦点并且可以成功的获取焦点（译者注：比如disabled被禁用的表单元素获取不到焦点），那么使用DOM原生的focus事件，浏览器只访问focus事件处理程序。jQuery总是调用.trigger(“focus”) 或 .focus()绑定的处理程序，无论元素是否获取焦点。在jQuery 1.9中还是这样处理的。和DOM的.focus()方法不同之处在于，在许多情况下，元素已经获取焦点或者元素被禁用，DOM的.focus()方法不会调用事件处理程序。
不幸的是，所有版本的Internet Explorer（6-10）触发焦点事件是异步的。当你在IE中使用.trigger(“focus”)，jQuery无法“预知”异步focus事件以后会发生什么。所以它总是会触发一个自己的focus事件，以确保功能正常。这可能会造成focus事件重覆执行，建议改用DOM内建的focus()较单纯，例如: $(“#boo”).get(0).focus()。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/focus/](http://www.css88.com/jqapi-1.9/focus/)
#### **jQuery(htmlString)与jQuery(selectorString)**
在1.9以前，如果一个字符串中有任何HTML标签，那么这个字符串将被认为是一个HTML字符串。这有可能造成意外的代码执行和拒绝有效的选择器字符串。 1.9开始，以一个小于号（“<”）字符开头的字符串才被认为是HTML字符串。Migrate（延迟）插件可以恢复到1.9以前的行为。
如果一个字符串被认为是HTML，但可能会以不是一个HTML标签的任意文本开始，将它传递给jQuery.parseHTML（）将返回一个DOM节点数组表示的标记。我们可以通过它来创建一个jQuery集合，例如：$($.parseHTML(htmlString))。例如，在处理HTML模板方面这被认为是最佳实践。简单使用文字字符串，如$(“<p>Testing</p>”).appendTo(“body”)不会受此影响。
总之：HTML字符串传递给jQuery()，除了以一个小于号（“<”）字符开始以外的其他字符串都将被解释为一个选择器。因为字符串通常不能被解释为一个选择器，最有可能的结果是Sizzle选择器引擎错误抛出的“无效的选择器语法”。使用jQuery.parseHTML（）来解析任意的HTML。
使用jQuery Migrate（迁移）插件，如果该字符串传递给$()，“看起来像HTML”，它会使用旧的规则来确定。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/jQuery/](http://www.css88.com/jqapi-1.9/jQuery/)
#### **.data()中名称包含点(“.”)改变**
.data()有一个未公开并且令人难以置信的非高性能监控值的设置和获取，1.9中被移除。这已经影响到了包含点的数据名称的解析。从1.9开始，调用 .data(“abc.def”)只能通过名称为“abc.def”检索数据，原本还可以通过“abc”取得的技巧已被取消。需要注意的是较低级别的jQuery.data()方法不支持事件，所以它并没有改变。即使使用jQuery Migrate（迁移）插件也恢复不到原来的行为。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/data/](http://www.css88.com/jqapi-1.9/data/)
#### **脱离文档节点在jQuery集合中的顺序**
对于许多版本，几乎所有的jQuery的方法，返回一组新的节点集合，这个集合是一个使用他们在文档中顺序排序的结果集。（有几个方法，如.parents()返回的结果是他们在文档反向顺序排序，但在1.9中这些例外情况已经记录并没有改变。）
在1.9之前，若jQuery集合中混杂DOM的节点及未放进DOM的脱离文档节点，则可能出现不可预期的随机排序。从1.9开始，在文档中的连接节点都总是按文档顺序放置在集合的开头，脱离文档节点被放置在他们的后面。即使使用jQuery Migrate（迁移）插件也恢复不到原来的行为。
#### **加载并且执行HTML内容中的scripts**
在1.9之前，任何接受HTML字符串的方法（例如，$()、.append()、.wrap()）会执行HTML字符串中所包含的Script，并且将它们从文档中移除，以防止他们再次被执行。在特殊情况下，使用这些方法一个脚本可能会被移除并重新插入到文档中，比如.wrap()。从1.9开始，插入到文档的脚本会执行，但仍然保留在文档中并且标记为已经被执行过的，这样它们就不会被再次执行，即使它们被删除并重新插入。
尽管这种变化，在HTML标记中混合可执行的JavaScript是非常不好的习惯；它对设计，安全性，可靠性和性能有影响。例如，外部脚本标签包含在HTML中同步地取出，然后评估执行，这可能需要大量的时间。没有任何接口通知这些脚本何时何地加载，或者当有错误产生的时候获得纠正提示。
试图通过克隆一个现有的脚本标签加载和注入脚本，克隆到文档将不再起作用，因为克隆的脚本标记已经被标记为已执行。要加载一个新的脚本，建议使用jQuery.getScript()代替。
#### **.attr() 和 .prop()对比**
译者注：[@caikan](http://weibo.com/u/1769654722)的建议，在这里将property翻译成了“对象属性”，以区别attributes（标签属性），后面括号标注了原文。
jQuery 1.6 介绍了.prop()方法设置或获取节点上的对象属性（property），并且不建议使用.attr()方法设置对象属性（property）。然而版本一直到1.9，在某些特殊情况下继续支持使用.attr()方法。当选择器是用来区分标签属性（attributes）和对象属性（properties）时，这种行为在向后兼容的命名方面会引起混乱。
例如，一个复选框的布尔标签属性（attributes），如checked和disabled受到这种变化的影响。”input[checked]“的正确行为是选择有checked属性的复选框，不管是它的字符串值，还是它当前的状态。与此相反， “input:checked” 选择当前checked属性的布尔值（true或false）为true的复选框，例如当用户单击复选框时，会受到影响。1.9之前版本这些选择器有时不选择正确的节点。
这里有一些例子，当在复选框上设置一个checked属性时正确的和不正确的使用方法；同样的规则也适用于disabled属性。请注意只有对象属性（property）在所有的浏览器始终反映和更新的复选框的当前状态；你很少会需要设置的属性（attribute）。
|`01`|```// Correct if changing the attribute is desired```|
|`02`||
|`03`|`$(elem).attr(``"checked"``, ``"checked"``);`|
|`04`||
|`05`|```// Correct for checking the checkbox```|
|`06`||
|`07`|`$(elem).prop(``"checked"``, ``true``);`|
|`08`||
|`09`|```// Correct if removing the attribute is desired```|
|`10`||
|`11`|`$(elem).removeAttr(``"checked"``);`|
|`12`||
|`13`|```// Correct for clearing the checkbox```|
|`14`||
|`15`|`$(elem).prop(``"checked"``, ``false``);`|
input元素的特性（property）和标签属性（attribute）是这个歧义的另一个例子。标签属性（attribute）一般体现读取HTML标记的值；属性反映了当前值。由于.val()方法是jQuery推荐的用来获取或设置表单元素的值的方式，这种混淆通常不会影响用户。
然而，当一个选择器，如”input[value=abc]” 被使用，它应该始终选择的value标签属性（attribute）并且用户对于对象属性（property）没有任何改变，例如，从这些选择器中键入到一个文本输入框。从jQuery 1.9开始，此行为正确并且一贯的。早期jQuery版本当应该使用标签属性（attribute），有时会使用他们的对象属性（property）。
jQuery Migrate（迁移）插件可以恢复原来的对象属性（property）和标签属性（attribute）规则。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/attr/](http://www.css88.com/jqapi-1.9/attr/) 和 [http://www.css88.com/jqapi-1.9/prop/](http://www.css88.com/jqapi-1.9/prop/)
#### **老IE中的$(“input”).attr(“type”, newValue)**
在1.9版之前， 在所有的浏览器中，任何企图设置一个input或者button元素的类型（type属性），jQuery都将抛出一个异常。这样做时为了符合最低标准的兼容；因为如果你试图改变input元素的类型，IE6/7/8抛出一个错误。从 jQuery 1.9开始，如果浏览器允许的话，我们允许您设置元素的类型。但是，你需要知道自己的代码，在老IE（IE6/7/8）下试图做到这一点还是会抛出一个错误。当你试图设置类型属性时，jQuery Migrate（迁移）插件会发出警告，但不会抛出一个JavaScript错误。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/attr/](http://www.css88.com/jqapi-1.9/attr/)
#### **“hover”伪事件**
从1.9开始，事件名称字符串“hover”不再支持为“mouseenter mouseleave”的代名词缩写。允许应用程序绑定和触发自定义的”hover”事件。修改现有的代码是一个简单的查找/替换，并且jQuery Migrate（延迟）插件可以恢复”hover”伪事件。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/hover/](http://www.css88.com/jqapi-1.9/hover/)
#### **jQuery对象上的.selector属性**
jQuery对象上过时的selector属性保留的目的是为了支持过时的.live()事件。在1.9中，jQuery不再试图在链方法上保留这个属性，因为1.9已经移除了.live()事件。不要使用jQuery对象的.selector属性。jQuery Migrate（迁移）插件也没支持这个属性。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/selector/](http://www.css88.com/jqapi-1.9/selector/)
#### **jQuery.attr()**
1.9版移除了jQuery.attr(elem, name, value, pass)方法，用jQuery Migrate（迁移）插件可恢复这个方法。
#### jQuery.ajax返回一个空字符串的JSON结果
1.9之前，一个AJAX调用预期返回JSON或JSONP的数据类型，当返回值是一个空字符串时会被认为是成功的状态，但返回一个null给success处理程序或承诺（promise）。从1.9开始，JSON数据返回一个空字符串被认为是畸形的JSON（因为它本来就是）;这将抛出一个错误。这种情况下，使用error（错误）处理程序捕获。
#### **jQuery.proxy()**
1.9版前，$.proxy(null, fn)、$.proxy(undefined, fn)的this会指向window，而$.proxy(false, fn)的this则指向new Boolean(false) ；1.9起若context传入null/undefined/false，函数的this会维持原先context，不被改变。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/jQuery.proxy/](http://www.css88.com/jqapi-1.9/jQuery.proxy/)
#### **.data(“events”)**
1.9以前,如果没有其他的代码定义一个名称为“events”的数据元素，.data(“events”) 可以用来检索一个元素上，jQuery未公开的内部事件数据结构。这种特殊的情况，在1.9中已被删除。没有公共的接口来获取这个内部数据结构，
它是不公开的。jQuery Migrate（迁移）插件可以恢复原来的行为。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/data/](http://www.css88.com/jqapi-1.9/data/)
#### **移除Event对象的部分属性**
Event对象的attrChange、attrName、realtedNote和srcElement属性自1.7版因无法跨浏览器已被宣告过时；从jQuery 1.9开始，它们不再被复制到Event对象传递给事件处理程序。在jQuery所有版本中，这些属性依然可以在支持他们的浏览器上通过event.orginalEvent存取，以取代event。jQuery Migrate（迁移）插件在Event对象有加回了这些属性。
译者注：具体查看API [http://www.css88.com/jqapi-1.9/category/events/event-object/](http://www.css88.com/jqapi-1.9/category/events/event-object/)
#### **API方法未公开的参数**
1.9之前，几个API方法未公开改变了他们的行为的参数，并存在潜在的意外误用。这些参数已经被删除。受影响的方法包括jQuery.data()，jQuery.removeData()，和jQuery.attr()。jQuery Migrate（迁移）插件也不支持的代码。
#### **其他未公开的属性和方法**
下面的内部属性和方法从未被收入到文档，并已在1.9中删除。
- jQuery.deletedIds
- jQuery.uuid
- jQuery.attrFn
- jQuery.clean()
- jQuery.event.handle()
- jQuery.offset.bodyOffset()
