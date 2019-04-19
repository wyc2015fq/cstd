# QML之StackView - LC900730的博客 - CSDN博客
2017年09月01日 14:35:21[lc900730](https://me.csdn.net/LC900730)阅读数：1394
### QML中SatckView
StackView管理着view页面的生命周期，提供了页面的栈式导航。这些view页面可能有业务需要，根据业务需要，可以一级一级向深处跳转，根据当前view的状态与设定的情况，可能产生一个新view或者返回之前view
比如：注册账号分步骤，输入用户名，密码，点击下一步之后，出现新页面，输入兴趣爱好等
//定义一个StackView
```
ApplicationWindow{
    title:"StckViewDemo"
    visible:true
    height:300
    width:530
    StackView {
        id:stack;
        anchors.fill:parent
        width:600
        height:300
        property var home :null;
        Text {
            text:"Cloxk to create first page"
            font.pointSize: 14
            font.bold: true
            color: "blue"
            anchors.centerIn: parent
            MouseArea{
                anchors.fill: parent
                onClicked: 
                    if(stack.depth==0)
                       stack.push(page);
            }
        }
}
Component {
            id: page
            Rectangle {
                color: Qt.rgba(stack.depth*0.1,stack.depth*0.2,stack.depth*0.3);
                Text {
                    anchors.centerIn: parent;
                    text: "depth-"+stack.depth;
                    font.pointSize: 24;
                    font.bold: true
                    color: stack.depth<=4?Qt.lighter(parent.color):Qt.darker(parent.color);
                    }
                Button{
                    id:next;
                    anchors.right: parent.right;
                    anchors.bottom: parent.bottom;
                    anchors.margins: 8;
                    text:"Next"
                    width:70
                    height: 30;
                    onClicked: {
                        if(stack.depth<8)
                            stack.push(page);
                    }
                }
            Button{
                id:back;
                anchors.right: next.left;
                anchors.top: next.top;
                anchors.rightMargin: 8;
                text:"Back"
                width:70
                height: 30;
                onClicked: {
                    if(stack.depth>0)
                        stack.pop();
                }
            }
            Button{
                id:home;
                anchors.right: back.left;
                anchors.top: next.top;
                anchors.rightMargin: 8;
                text:"Home"
                width:70
                height: 30;
                onClicked: {
                    if(stack.depth>0)
                        stack.pop(stack.initialItem);
                }
            }
            Button{
                id:clear;
                anchors.right: home.left;
                anchors.top: next.top;
                anchors.rightMargin: 8;
                text:"Clear"
                width:70
                height: 30;
                onClicked: {
                    if(stack.depth>0)
                        stack.clear();
                }
            }
            }
        }
}
```
### StackView的方法
|clear():|干掉StackView的所有页面|
|----|----|
|pop():|出栈操作。无参调用pop，弹出栈顶页面，带参数则将参数指定额页面之后的所有页面弹出。如[A,B,C,D,E],pop()调用后，就变为[A,B,C,D]。再次调用pop(B),会变成[A,B]|
|push():|压入页面。一般是动态创建的。如栈是[A,B,C,D],push(E,replace),就会用E替换栈顶的D，栈会变成[A,B,C,E]|
|find(func,onlySearchLoadedItems):|查找StackView管理的某个页面。find将对栈内每个页面应用func方法，当func返回true，表示找到，停止查找。然后find返回找到的那个item|
|completeTransition():|立即结束过渡动画|
### StackView的属性
|busy|指示SatckView是否正在应用过渡动画。为true时表示正在应用动画，可以通过onBusyChanged来响应busy属性变化，如在动画期间禁止点击操作|
|----|----|
|delegate|可以指定页面切换的过渡动画，不使用delegate，默认情况下就使用StackView的切换动画|
|depth|栈深度，栈为空则为0，有子页面则从1开始|
|initialItem|初始化View(Item)。可以通过这个属性来指定StackView管理的第一个页面(View)，如果你在初始化时给InitialItem赋值，效果就相当于在Component.onCompleted信号处理器中调用push(yourItem)。如果不显示给initialItem赋值，那么第一个页面被push进StackView时候，这个属性会被自动赋|
|currentItem|指向栈顶的View，可能为空|
