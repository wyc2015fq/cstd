# Selenium+python+API分类总结 - zhusongziye的博客 - CSDN博客





2018年03月27日 20:38:22[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：325








Selenium+python+API分类总结

http://selenium-python.readthedocs.org/index.html

|分类|方法|方法描述|
|----|----|----|
|客户端操作|__init__(self, host, port, browserStartCommand, browserURL)|构造函数。host：selenium server的ip；port：elenium server的port，默认为4444；browserStartCommand：浏览器类型，iexplore，firefox。browserURL：打开的url。|
|start(self)|启动客户端| |
|stop(self)|关闭客户端| |
|鼠标点击|click(self, locator)|点击链接、按钮、单选或者复选框|
|double_click(self, locator)|双击链接、按钮、单选或者复选框| |
|click_at(self, locator, coordString)|点击链接、按钮、单选或者复选框后调用waitForPageToLoad，coordString是鼠标事件相对于locator返回元素的位置| |
|double_click_at(self, locator, coordString)|双击链接、按钮、单选或者复选框后调用waitForPageToLoad，coordString是鼠标事件相对于locator返回元素的位置| |
|鼠标操作|mouse_over(self, locator)|模拟用户将鼠标悬停在指定元素上|
|mouse_out(self, locator)|模拟用户将鼠标从指定元素移走| |
|mouse_down(self, locator)|模拟用户按下鼠标键，未释放| |
|mouse_down_at(self, locator, coordString)|模拟用户按下鼠标键，未释放，coordString是鼠标事件相对于locator返回元素的位置| |
|mouse_up(self, locator)|模拟用户释放鼠标键| |
|mouse_up_at(self, locator, coordString)|模拟用户释放鼠标键，coordString是鼠标事件相对于locator返回元素的位置| |
|mouse_move(self, locator)|鼠标拖动指定元素| |
|mouse_move_at(self, locator, coordString)|鼠标拖动指定元素，coordString是鼠标事件相对于locator返回元素的位置| |
|按键操作|key_press(self, locator, keySequence)|模拟用户按下和释放键，keySequence可以是一个字符串（被按下键的ASCII码值）；也可以是单独的字符，例如”e”。|
|shift_key_down(self)|按下shift键直到调用doShiftUp()或者载入新页面| |
|shift_key_up(self)|释放shift键| |
|alt_key_down(self)|按下alt键直到调用doAltUp()或者载入新页面| |
|alt _key_up(self)|释放alt键| |
|control_key_down(self)|按下control键直到调用doControlUp()或者载入新页面| |
|control _key_up(self)|释放control键| |
|key_down(self, locator, keySequence)|模拟用户按键不释放，keySequence可以是一个字符串（被按下键的ASCII码值）；也可以是单独的字符，例如”e”。| |
|key_up(self, locator, keySequence)|模拟用户释放键，keySequence可以是一个字符串（被按下键的ASCII码值）；也可以是单独的字符，例如”e”。| |
|type(self, locator, value)|设置输入框的值；也可用于设置组合框和复选框的值，所设置的值是勾选的选项，是不可见的文本。| |
|check(self, locator)|选中单选或者复选框| |
|设置输入值|uncheck(self, locator)|取消选中单选或者复选框|
|单选和复选|is_checked(self, locator)|检查单选或者复选按钮是否选中，如果指定元素不存在或者不是切换按钮则失败|
|select(self, selectLocator, optionLocator)|使用option locator选择下拉列表的选项；其中option locator有以下几种形式：1、  label=labelPattern：基于label匹配选项，例如可视化文本，* label=regexp:^[Oo]ther。（注意label前面的星号）2、  value=valuePattern：基于值匹配选项，* value=other。3、  id=id：基于id匹配选项，* id=option1。4、  index=index：基于索引（从零开始）匹配选项，* index=2。如果未指定任何前缀，默认情况下是label。selectLocator是下拉列表定位。| |
|add_selection(self, locator, optionLocator)|对于多选元素，在已选选项集合中增加一个选项，locator是多选框定位。| |
|下拉框|remove_selection(self, locator, optionLocator)|对于多选元素，从已选选项集合中删除一个选项，locator是多选框定位。|
|get_selected_labels(self, selectLocator)|获取下拉列表或者多级下拉列表的多个已选选项的所有label（可视化文本）| |
|get_selected_label(self, selectLocator)|获取下拉列表或者多级下拉列表的某个选项的label（可视化文本）| |
|get_selected_values(self, selectLocator)|获取下拉列表或者多级下拉列表的多个已选选项的所有value（value属性）| |
|get_selected_value(self, selectLocator)|获取下拉列表或者多级下拉列表的某个选项的value（value属性）| |
|get_selected_indexes(self, selectLocator)|获取下拉列表或者多级下拉列表的多个已选选项的所有index（index从零开始）| |
|get_selected_index(self, selectLocator)|获取下拉列表或者多级下拉列表的某个选项的index（index从零开始）| |
|get_selected_ids(self, selectLocator)|获取下拉列表或者多级下拉列表的多个已选选项的所有element ID| |
|get_selected_id(self, selectLocator)|获取下拉列表或者多级下拉列表的某个选项的element ID| |
|is_something_selected(self, selectLocator)|判断下拉列表的某个选项是否选中| |
|get_select_options(self, selectLocator)|获取下拉列表的所有label| |
|set_speed(self, value)|设置操作后的等待时间，以毫秒为单位，设置后作用于每一个selenium操作，默认情况下为0毫秒。| |
|get_speed(self)|获取操作后的等待时间| |
|设置超时或者等待事件|set_timeout(self, timeout)|为action设置超时，默认为30秒。一般用于open以及waitFor*。运行错误返回error。|
|wait_for_page_to_load(self, timeout)|等待装载新页面。可以使用该命令代替”AndWait”命令，例如"clickAndWait", "selectAndWait", "typeAndWait"等（这些命令只在js API中提供）。Selenium装载新页面时通常会将”newPageLoaded” flag置为true，直到flag恢复为false才继续执行命令。超时后返回error。| |
|submit(self, formLocator)|表单提交，在没有提交按钮是非常有用的表单提交接口函数| |
|open(self, url)|打开url，url可以是相对或者绝对url，open按钮一直等待页面载入后再处理，其隐式调用“AndWait”。注意：由于安全限制，url的域需要与runner HTML（Selenium Server）的域保持一致，如果需要使用不同域的url，需要启动新的浏览器会话。| |
|提交|open_window(self, url, windowID)|如果标识为ID的窗口还未打开，则打开弹出窗口。打开窗口后，需要使用selectWindow命令选择。注意：当window.open调用发生在“onLoad”事件之前或者事件过程时，Selenium无法成功调用window.open，在这种情况下，可以使用Selenium的openWindow命令强制打开窗口，注意使用空白页面，例如openWindow（””,”muFunnyWindow”）。注意：url可以为空，’windowID’是js window ID。|
|窗口操作|select_window(self, windowID)|选择弹出的窗口，选中后所有的命令都在选中窗口中执行。如果想再次选择主窗口，windowID使用null。Selenium根据windowID查找窗口对象的几种策略：1、如果windowID是null，选中浏览器初始化时打开的最原始的窗口。2、如果windowID是当前应用窗口的js变量名，则该变量包含js window.open()方法的返回值。3、最后，Selenium查找其哈希表，在该表中维护着字符串与窗口对象的映射表，这里的字符串与js方法window.open(url, windowName, windowFeatures, replaceFlag)中的第二个参数匹配。如果不知道窗口的名字，可以通过selenium日志查看window.open创建窗口时的名字标识。同open_window，无法成功调用window.open时，可强制性打开空白页面。|
|select_frame(self, locator)|在当前窗口选择frame或者iframe，可以多次调用该命令选择嵌套的frame。如果想选择parent frame，定位符使用’relative=parent’；如果想选择top frame，使用’relative=top’。也可以使用DOM表达式直接查找frame，例如dom=frames["main"].frames["subframe"]。| |
|go_back(self)|模拟用户点击浏览器的后退按钮| |
|refresh(self)|模拟用户点击浏览器的刷新按钮| |
|close(self)|模拟用户点击浏览器的关闭按钮| |
|window_focus(self, windowName)|将焦点设置到窗口上，windowName为窗口名| |
|window_maximize(self, windowName)|最大化窗口，windowName为窗口名| |
|get_all_window_ids(self)|返回浏览器打开的所有窗口ID| |
|get_all_window_names(self)|返回浏览器打开的所有窗口名| |
|get_all_window_titles(self)|返回浏览器打开的所有窗口标题| |
|wait_for_pop_up(self, windowID, timeout)|为等待窗口弹出设置超时，windowID为js的窗口ID，超时的单位为毫秒。超过超时时间后返回错误。| |
|choose_cancel_on_next_confirmation(self)|默认情况下，返回true，就像用户手工点击ok；运行完此命令后，下次再调用confirm()则返回false，就像用户点击Cancle。| |
|弹出窗口|answer_on_next_prompt(self, answer)|在下次js窗口弹出时，selenium返回answer中的字符串|
|is_alert_present(self)|判断是否有alert窗口出现，此API不会抛出异常| |
|is_prompt_present(self)|判断是否有prmp窗口出现，此API不会抛出异常| |
|is_confirmation_present(self)|判断是否有confirm窗口出现，此API不会抛出异常| |
|get_alert(self)|获取先前弹出的js alert窗口的内容，如果没有alert窗口则失败。类似于手动点击ok。注意：1、如果alert出现但是没有获取或者验证它，则selenium的下次操作会失败。2、selenium中的alert不会弹出可视化的alert。3、不支持页面onload()时间处理过程中的js alert窗口，在这种情况下，将会弹出可视化对话窗口，直到手动点击ok后selenium才会继续执行。| |
|get_confirmation(self)|获取先前弹出的js confirm窗口的内容，如果没有confirm窗口则失败。类似于手动点击ok，但是如果在之前执行了chooseCancelOnNextConfirmation命令，则类似于手动点击cancel。注意：1、如果confirm出现但是没有获取或者验证它，则selenium的下次操作会失败。2、selenium中的confirm不会弹出可视化的confirm。3、不支持页面onload()时间处理过程中的js confirm窗口，在这种情况下，将会弹出可视化对话窗口，直到手动点击ok后selenium才会继续执行。| |
|get_prompt(self)|获取先前弹出的js prompt窗口的内容，如果没有prompt窗口则失败。注意：1、如果alert出现但是没有获取或者验证它，则selenium的下次操作会失败。2、selenium中的prompt不会弹出可视化的prompt。3、不支持页面onload()时间处理过程中的js prompt窗口，在这种情况下，将会弹出可视化对话窗口，直到手动点击ok后selenium才会继续执行。4、调用此API之前必须调用answerOnNextPromp命令，prompt才能成功处理。| |
|get_all_buttons(self)|返回页面所有button的ID，如果某个button没有ID，返回“”| |
|get_all_links(self)|返回页面所有link的ID，如果某个link没有ID，返回“”| |
|获取页面元素，元素索引|get_all_fields(self)|返回页面所有输入域的ID，如果某个输入域没有ID，返回“”|
|get_attribute_from_all_windows(self, attributeName)|根据属性名获取某个属性在所有已知窗口中的实例| |
|get_element_index(self, locator)|获取元素对于0索引的相对索引，忽略comment命令和空文本节点。| |
|get_location(self)|获取当前页面的绝对url| |
|get_title(self)|获取当前页面的绝对title| |
|获取元素属性|get_body_text(self)|获取页面的内容|
|get_value(self, locator)|获取输入量的值（以空格截断），对于单选和复选框，返回值为on或者off| |
|get_text(self, locator)|获取元素的文本，适用于任何包含文本的元素。此API调用textContent（firefox）或者innerText（IE）。（不同js）| |
|get_table(self, tableCellAddress)|获取表格的单元格，tableCellAddress的格式为tableLocator.row.column，其中行和列的索引从零开始，例如foo.1.4。| |
|get_attribute(self, attributeLocator)|获取元素属性的value| |
|get_element_position_left(self, locator)|获取元素的水平位置| |
|get_element_position_top(self, locator)|获取元素的垂直位置| |
|get_element_width(self, locator)|获取元素的宽度| |
|get_element_height(self, locator)|获取元素的高度| |
|is_text_present(self, pattern)|验证页面上是否有满足pattern的文本| |
|is_element_present(self, locator)|验证页面上是否有满足pattern的文本| |
|验证元素|is_visible(self, locator)|验证元素是否可见，元素不存在时失败。元素不可见通过将css的”visibility”设置为“hidden”或者将“display”设置为“none”。|
|is_editable(self, locator)|验证输入元素是否被disable，元素不存在时失败。| |
|is_ordered(self, locator1, locator2)|验证两个元素是否属于同一个父节点，并且排序。两个元素相同是不排序。| |
|dragdrop(self, locator, movementsString)|将定位到的元素拖曳，movementString是目的地址相对于当前位置的偏移，以像素衡量，例如”+70,-300”。不推荐使用dragAndDrop代替。| |
|drag_and_drop(self, locator, movementsString)|将定位到的元素拖曳，movementString是目的地址相对于当前位置的偏移，以像素衡量，例如”+70,-300”。不推荐使用dragAndDrop代替。| |
|移动元素|drag_and_drop_to_object(self, locatorOfObjectToBeDragged, locatorOfDragDestinationObject)|将元素拖曳到目标对象上，已两个元素的左上角位置重合|
|set_cursor_position(self, locator, position)|在输入框或者多行文本中移动光标，如果元素不是输入框或者多行文本，则失败。position是输入域中光标的数字位置，其中0是开头，-1是结尾。| |
|get_cursor_position(self, locator)|返回输入元素（输入框或者多选框）或者多行文本框的文本光标位置。如果指定元素不是输入元素或者多行文本框，或者指定元素没有光标，该方法调用失败。注意：该函数在某些浏览器上可能会失效（浏览器对js解析的差异性）。特别是当光标或者选择项被js清除时，该命令趋向于返回光标最后定位的位置，即使光标已经离开页面。| |
|光标操作|get_eval(self, script)|执行script中的js脚本代码片段，只返回最后一行的结果。注意：1、默认情况下，脚本代码片段在selenium对象的上下文中执行，因此this是指向selenium对象，window指向最上层的运行测试窗口，而不是应用窗口。2、如果想指向应用窗口，使用this.browserbot.getCurrentWindow()。3、如果想定位到应用页面中的某个元素，使用this.page().findElement(“foo”)定位foo元素。|
|get_expression(self, expression)|用于js的预处理，用于产生类似assertExpression和waitForExpression等。expression是返回值。| |
|js代码|wait_for_condition(self, script, timeout)|不断地运行js代码片段知道返回为true，代码片段可以有多行，但只返回最后一行的结果，timeout的单位为毫秒。注意：默认情况下js代码片段在selenium runner的测试窗口下运行而不是应用窗口，如果需要在应用窗口下运行，先调用selenium.browserbot.getCurrentWindow()。运行错误返回error。|
|get_html_source(self)|返回整个html源码| |
|get_cookie(self)|返回当前测试页面的所有cookies。| |
|获取html源码|create_cookie(self, nameValuePair, optionsString)|以当前测试页面path和domain创建新的cookie，也可以显示指定path。nameValuePair是键值对。optionsString目前支持两种可选项，path=/path/和max_age=60（cookie的有效时间，以秒为单位）。|
|Cookie操作|delete_cookie(self, name, path)|删除指定路径下的name的value|
|fire_event(self, locator, eventName)|显示地模拟事件，以触发相应的”onevent”处理函数，eventName例如是focus或者blur等。| |
|get_whether_this_frame_match_frame_expression(self, currentFrameString, target)|用于代理注入模式，在该模式下，get_whether_this_frame_match_frame_expression在每个frame和window中运行，帮助selenium识别出当前的frame。在这种情况下，当测试脚本调用selectFrame时，每个frame都调用此API计算哪个frame被选中，选中的frame选择true，其它frame返回false。CurrentFrameString是当前frame，target是新frame（可能是相对于当前frame）| |
|事件触发|get_whether_this_window_match_window_expression(self, currentWindowString, target)|原理同上|
|多frame和多窗口|get_log_messages(self)|很少使用，主要用于当frame来自不同域时，获取日志信息，因为在这种情况下不符合js日志集中管理机制。|
|set_context(self, context, logLevelThreshold)|写消息到状态栏或者增加浏览器端的日志，logLevelThreshold用于指定日志级别（debug，info，warn，error）。注意浏览器端的日志不会返回给服务器，并且对于客户驱动器不可见。| |
|辅助功能：日志，状态栏，|||
||| |



