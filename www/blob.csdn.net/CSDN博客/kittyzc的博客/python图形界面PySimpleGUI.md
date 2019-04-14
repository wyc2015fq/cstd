# python图形界面PySimpleGUI - kittyzc的博客 - CSDN博客
2018年12月24日 09:34:57[IE06](https://me.csdn.net/kittyzc)阅读数：969所属专栏：[python小tips](https://blog.csdn.net/column/details/31935.html)
# 1. 简介

使用pip安装即可。注意如果是python2.7版本，安装的是pySimpleGUI27。

pySimpleGUI可以绘制的界面元素可以从官方case中看出：

```python
import PySimpleGUI
pySimpleGUI.main()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213093310847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

内部代码如下：

```python
import PySimpleGUI as sg      

sg.ChangeLookAndFeel('GreenTan')      

# ------ Menu Definition ------ #      
menu_def = [['File', ['Open', 'Save', 'Exit', 'Properties']],      
            ['Edit', ['Paste', ['Special', 'Normal', ], 'Undo'], ],      
            ['Help', 'About...'], ]      

# ------ Column Definition ------ #      
column1 = [[sg.Text('Column 1', background_color='#F7F3EC', justification='center', size=(10, 1))],      
           [sg.Spin(values=('Spin Box 1', '2', '3'), initial_value='Spin Box 1')],      
           [sg.Spin(values=('Spin Box 1', '2', '3'), initial_value='Spin Box 2')],      
           [sg.Spin(values=('Spin Box 1', '2', '3'), initial_value='Spin Box 3')]]      

layout = [      
    [sg.Menu(menu_def, tearoff=True)],      
    [sg.Text('All graphic widgets in one window!', size=(30, 1), justification='center', font=("Helvetica", 25), relief=sg.RELIEF_RIDGE)],      
    [sg.Text('Here is some text.... and a place to enter text')],      
    [sg.InputText('This is my text')],      
    [sg.Frame(layout=[      
    [sg.Checkbox('Checkbox', size=(10,1)),  sg.Checkbox('My second checkbox!', default=True)],      
    [sg.Radio('My first Radio!     ', "RADIO1", default=True, size=(10,1)), sg.Radio('My second Radio!', "RADIO1")]], title='Options',title_color='red', relief=sg.RELIEF_SUNKEN, tooltip='Use these to set flags')],      
    [sg.Multiline(default_text='This is the default Text should you decide not to type anything', size=(35, 3)),      
     sg.Multiline(default_text='A second multi-line', size=(35, 3))],      
    [sg.InputCombo(('Combobox 1', 'Combobox 2'), size=(20, 1)),      
     sg.Slider(range=(1, 100), orientation='h', size=(34, 20), default_value=85)],      
    [sg.InputOptionMenu(('Menu Option 1', 'Menu Option 2', 'Menu Option 3'))],      
    [sg.Listbox(values=('Listbox 1', 'Listbox 2', 'Listbox 3'), size=(30, 3)),      
     sg.Frame('Labelled Group',[[      
     sg.Slider(range=(1, 100), orientation='v', size=(5, 20), default_value=25),      
     sg.Slider(range=(1, 100), orientation='v', size=(5, 20), default_value=75),      
     sg.Slider(range=(1, 100), orientation='v', size=(5, 20), default_value=10),      
     sg.Column(column1, background_color='#F7F3EC')]])],      
    [sg.Text('_'  * 80)],      
    [sg.Text('Choose A Folder', size=(35, 1))],      
    [sg.Text('Your Folder', size=(15, 1), auto_size_text=False, justification='right'),      
     sg.InputText('Default Folder'), sg.FolderBrowse()],      
    [sg.Submit(tooltip='Click to submit this window'), sg.Cancel()]      
]      


window = sg.Window('Everything bagel', default_element_size=(40, 1), grab_anywhere=False).Layout(layout)      

event, values = window.Read()      

sg.Popup('Title',      
         'The results of the window.',      
         'The button clicked was "{}"'.format(event),      
         'The values are', values)
```

要导出exe的话，安装PyInstaller，然后执行pyinstaller -wF 程序.py即可。

# 2. 全局设置

可以对window/row/element使用setOptions方法，可配参数如下：

```
icon=None      
button_color=(None,None)      
element_size=(None,None),      
margins=(None,None),      
element_padding=(None,None)      
auto_size_text=None      
auto_size_buttons=None      
font=None      
border_width=None      
slider_border_width=None      
slider_relief=None      
slider_orientation=None      
autoclose_time=None      
message_box_line_width=None      
progress_meter_border_depth=None      
progress_meter_style=None      
progress_meter_relief=None      
progress_meter_color=None      
progress_meter_size=None      
text_justification=None      
text_color=None      
background_color=None      
element_background_color=None      
text_element_background_color=None      
input_elements_background_color=None      
element_text_color=None      
input_text_color=None      
scrollbar_color=None, text_color=None      
debug_win_size=(None,None)      
window_location=(None,None)      
tooltip_time = None
```

# 3. 事件处理

PySimpleGUI的事件处理使用while循环实现，下面是一个例子：

```python
import PySimpleGUI as sg      

layout = [[sg.Text('Persistent window')],      
          [sg.Input(key = 'a')],
          [sg.Input(key = 'b')],
          [sg.Button('Read'), sg.Exit()]]      

window = sg.Window('Window that stays open').Layout(layout)      

while True:      
    event, values = window.Read()      
    if event is None or event == 'Exit':      
        break      
    print(event, values)    

window.Close()
```

# 3.1 获取部件

使用window.Element(部件名称).update()可以获取到部件并更改值（部分部件可更改属性，参考API文档），例如：

```python
import PySimpleGUI as sg  

layout = [ [sg.Text('My layout', key='_TEXT_')],  
           [sg.Button('Read')]]  

window = sg.Window('My new window').Layout(layout)  
while True:             # Event Loop  
    event, values = window.Read()  
    if event is None:  
        break  
    window.Element('_TEXT_').Update('My new text value')
```

# 3.2 blocking事件响应

Layout的read方法会等待用户进行响应，称为blocking event，分为三种情况：

第一种响应是按钮点击响应。每次用户点击按钮后，会返回两个值，一个是按钮的名称，一个是返回值。返回值是字典的形式，key是部件name，value是对应的值。

第二种响应是Enter响应。在属性中设置change_submits = True即可，这样按下Enter键就会触发响应。

第三种响应是鼠标/键盘事件。在属性中设置return_keyboard_events = True即可。
```python
import PySimpleGUI as sg  

with sg.Window("Keyboard Test", return_keyboard_events=True, use_default_focus=False) as window:  
    text_elem = sg.Text("", size=(18, 1))  
    layout = [[sg.Text("Press a key or scroll mouse")],  
              [text_elem]]

    window.Layout(layout)  
    # ---===--- Loop taking in user input --- #  
    while True:  
        event, value = window.Read()
        if event is None:   
            break  
        text_elem.Update(event)
window.Close()
```

# 3.3 non-blocking事件响应

如果想定时响应，比如动画每1/24秒刷新一次页面，可以使用non-blocking event。设置read方法的timeout属性即可（时间单位为毫秒），此时返回的event == sg.TIMEOUT_KEY。

```python
import PySimpleGUI as sg  

layout = [ [sg.Text('non-blocking example', key='_TEXT_')],  
           [sg.Button('Read')]]  

window = sg.Window('My new window').Layout(layout)  
while True:             # Event Loop  
    event, values = window.Read(timeout=3000)  
    if event is None:  
        break # the use has closed the window  
    if event == sg.TIMEOUT_KEY:  
        print("Nothing happened") 
    else:
        print(event)
window.Close()
```

