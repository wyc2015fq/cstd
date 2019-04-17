# react-native 之导入、导出及使用 - DEVELOPER - CSDN博客





2018年06月03日 17:23:37[学术袁](https://me.csdn.net/u012827205)阅读数：1609
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









在学习使用React-Native过程中，对于组件、类、方法和变量的导入、导出并涉及到使用，这一整套过程，我们要熟知。此乃React-Native开发之必备基础。接下来，我就用一个小demo来详尽的阐述一下他们的使用方式； 
**当前的demo基于React-Native version0.41.0**
**先看下效果图**
![这里写图片描述](https://img-blog.csdn.net/20180603172745356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**demo的项目结构**
![这里写图片描述](https://img-blog.csdn.net/20180603172906451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**组件、变量代码是这样的**
```
/**
 * Created by YJH on 2018/5/24.
 */
import React, {Component} from 'react';
import {
    StyleSheet,
    Text,
    Image,
    View,
} from 'react-native';

/**
 * 功能：定义一个组件、变量，通过该组件、变量来展示其导出和导入以及使用
 */

export var FLAG_NAME={flag_game_gun:'全民出击', flag_game_king:'王者荣耀'};
export default class ExportComponent extends Component<Props> {

    constructor(props) {
        super(props);
    }

    render() {
        return (
            <View style={styles.container}>
                <Text style={styles.export_des}>ExportComponent.js</Text>
                <Image style={{width:100,height:100}} source={require('../images/timg.jpg')}/>
            </View>
        )
    }
}

const styles = StyleSheet.create(
    {
        container: {
            flex: 1,
            alignItems:'center'
        },
        export_des: {
            marginTop: 20,
            fontSize: 30,
            color: 'orange',
            alignSelf: 'center',
        }
    }
);
```

**类的代码是这样的**

```
/**
 * 功能：定义一个类，通过该类来展示其导出和导入以及使用
 */
export default class ExportClass{
    isEqualed(newWord, oldWord){
        if(!newWord || !oldWord)return false;
        if(newWord !== oldWord)return false;

        return true;
    }
}
```

**方法的代码是这样的**

```java
/**
 * 功能：定义一个方法，通过该方法来展示其导出和导入以及使用
 * @param name
 * @param color
 */
export default function ExportFunction(name, color) {
    this.name=name;
    this.color=color;
}
```

**导入使用的方式是这样的**

```
/**
 * Created by YJH on 2018/5/24.
 */
import React, {Component} from 'react';
import {
    StyleSheet,
    Text,
    View,
} from 'react-native';

import ExportComponent, {FLAG_NAME} from './js/component/ExportComponent';
import ExportFunction from './js/function/ExportFunction';
import ExportClass from './js/class/ExportClass';

export default class WelcomePage extends Component<Props> {

    constructor(props) {
        super(props);
        this.exportClass=new ExportClass();
        this.exportFunction = new ExportFunction(FLAG_NAME.flag_game_gun, 'green');
        this.state = {
            disPlayNme: this.exportFunction.name,
            disPlayColor: this.exportFunction.color,
            isEqualed:false,
        }
    }

    //当UI已render调用
    componentDidMount(){
        //通过类 ExportClass 的方法比较来动态改变isEqualed的属性值
        const equaled=this.exportClass.isEqualed('hello world', 'hello react-native');
        this.setState({
            isEqualed:equaled,
        })
    }

    render() {
        return (
            <View style={styles.container}>
                <Text style={{
                    marginTop: 20,
                    fontSize: 30,
                    color: this.state.disPlayColor,
                    alignSelf: 'center',
                }}>欢迎来到{this.state.disPlayNme}</Text>
                <ExportComponent {...this.props}/>
            </View>
        )
    }
}

const styles = StyleSheet.create(
    {
        container: {
            flex: 1,
        },

    }
);
```

**OK！！通过上面的demo代码，对组件、类、方法、变量的导入、导出和使用进行分解详述！！**

**导出组件变量**

从上面代码中，我们可以看到这么2行代码 
![这里写图片描述](https://img-blog.csdn.net/20180603174228388?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于变量的导出使用了关键字`export`

对于组件的导出使用了关键字`export default`
`export default xx ;`
`export default xx extends Component {...}`
**导出类**

从上面代码中，我们可以看到这么1行代码 
![这里写图片描述](https://img-blog.csdn.net/20180603174459551?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于类的导出使用了关键字`export default`
`export default xxclass {...}`
**导出方法**

从上面代码中，我们可以看到这么1行代码 
![这里写图片描述](https://img-blog.csdn.net/20180603174545142?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于方法的导出使用了关键字`export default`； 
`export default function xx() {...}`

而且在一整个js文件中就只有这一个方法并以文件名命名的方法名。然后我也做了下测试，什么测试呢？就是方法名不使用文件名进行命名会怎么样？！！答案是，当你导入并进行使用时因找不到该方法而报错！！
**导入组件、类、方法和变量**
`import xx,{变量} from '地址';`

从上面代码中，我们可以看到这么3行代码 
![这里写图片描述](https://img-blog.csdn.net/20180603174911844?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于组件、类、方法和变量的导入都使用了关键字`import`；当然对于变量的导入比较特殊，可多关注下。就第一行的那个`{}`中的变量哦。
**当然，我们之所以要导入，目的必定是要使用的。一起来看下使用方式**
**组件的使用**

组件的使用方式，看上面`WelcomePage .js`代码，我切下一部分分析 
![这里写图片描述](https://img-blog.csdn.net/20180603175438263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个是`WelcomePage .js`代码的`render` 方法下的代码，可以看到作为我们自己定义的组件。她的使用方式和React-Native自己的组件如，`Image Text View等` 的使用方式一个样，直接作为一个标签使用。
**类和方法的初步使用**

类和方法的使用方式，看上面`WelcomePage .js`代码，我切下一部分分析 
![这里写图片描述](https://img-blog.csdn.net/20180603175913659?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个构造方法，在构造方法中可以看到分别以`new` 的方式，在19、20行代码上看，就像Java语言一样创建了”对象”并将该”对象”赋给了`this.exportClass`和`this.exportFunction`变量。我们知道Java中的class是有这种使用方式，但是这里的方法为什么跟class的使用方式这么类似呢？答案就是这么的类似！！
**类和方法的具体使用**

从“类和方法的初步使用”中创建出各自的“对象”，然后具体到实处还是要通过创建的“对象”进行调用的。下面是在组件生命周期的方法`componentDidMount`，执行在方法`render` 之后。在这里使用类`ExoprtClass` 的方法来动态的改变在构造方法中`state`中的变量值。也就是说其实类导入后的具体使用其实和Java中使用基本是一致的。不过是要先需要对类进行导入，然后创建对象，然后才能进行具体的使用。 
![这里写图片描述](https://img-blog.csdn.net/20180603175840724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再看方法的使用 
![这里写图片描述](https://img-blog.csdn.net/20180603181308313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里我使用在了构造方法中，在构造方法中第20行代码对导入的方法进行创建对象，并使该对象带有两个字符串的值。一个是导入的类中的一个变量值`FLAG_NAME.flag_game_gun`，另一个是`green`。这里也就顺便告诉我们对`导入的变量`该如何使用！然后继续聊对导入的方法的使用方式。其实就是在第22、23行代码上。然后如果我们之后也就可以使用state来调用、使用。当然也可以不这样，可以直接点不用再绕state这一层的赋值过程。而在编程中导入方法的使用一般不是这么去使用的，而是把导入的方法作为一个module来使用。就像Java中我们的Bean一样。
使用方式上就这么多，完结！[demo地址](https://download.csdn.net/download/u012827205/10456087)

##### 此处增加一个   `Style`   导入导出的使用

这个使用在全局的style控制上，简单写写代码相信大家就能看得懂，叙述一下步骤 

首先创建一个js文件，比如这里我创建了`GlobalStyle.js`

```java
/**
 * Created by YJH on 2018/6/4.
 */
//半透明的黑色的隔断线
module.exports={
  line:{
    flex:1,
      height:1,
      backgroundColor:'gray',
      opacity:0.5,
  }  ,
};
```

然后在要使用到的组件类中导入并使用<地址因人而异> 
`import GlobalStyles from './GlobalStyles';`

使用方式：
```xml
<View style={GlobalStyles.line}/>
```













