# WPF 学习笔记 - zwvista - 博客园

## [WPF 学习笔记](https://www.cnblogs.com/zwvista/p/9692575.html)

### 依赖属性（Dependency Property）

相比较于普通属性，依赖属性有以下特点：
- 变化通知（change notification）
- 节省内存
- 使用某种带优先级决定策略（resolution strategy）来读取属性的值。

属性值可以依赖于其他对象，适用于数据绑定，属性值继承等场景。

#### 依赖属性的声明
- 宿主必须是 DependencyObject 的子类。
- 使用 DependencyProperty 类型声明依赖属性。（public static readonly）
- 使用 DependencyProperty.Register 方法注册依赖属性。
- 使用属性包装器包装依赖属性（调用 GetValue, SetValue）。

#### 附加属性（Attached Property）

附加属性是所处环境（上级元素）所附加的属性，是一种特殊的依赖属性。
- 宿主必须是 DependencyObject 的子类。
- 使用 DependencyProperty 类型声明依赖属性。（public static readonly）
- 使用 DependencyProperty.RegisterAttached 方法注册附加属性。
- 使用静态方法 GetXXX, SetXXX 存取附加属性（调用 GetValue, SetValue）。

### 路由事件（Routed Events）

路由事件被用来实现事件的路由功能。

#### 路由事件的声明
- 宿主必须是 FrameworkElement 的子类。
- 使用 RoutedEvent 类型声明路由事件。（public static readonly）
- 使用 EventManager.RegisterRoutedEvent 方法注册路由事件。
- 使用标准 .NET 事件声明机制包装路由事件（add, remove）。

#### 路由策略
- 直接：事件由事件源直接触发。
- 冒泡：由事件源逐级向上传递直至根元素。
- 隧道：由根元素逐级向下传递直至事件源。

隧道事件名通常以 Preview 打头。

#### 附加事件（Attached Event）

由下级元素的冒泡事件附加的事件。

### 数据绑定（Data Binding）

通过在目标对象的目标属性中设定绑定源及其值路径（属性名 Path=）来实现数据绑定。

#### 绑定其他界面元素
- 指定界面元素名作为绑定源。（ElementName=）
- 绑定模式（方向 Mode=）
- OneWay：单向，源属性到目标属性
- TwoWay：双向，源属性到目标属性，目标属性到源属性
- OneTime：单向，源属性到目标属性，仅限一次
- OneWayToSource：单向，目标属性到源属性
- Default：由目标对象元数据决定

- 方向为目标属性到源属性时触发源属性更新的因素（UpdateSourceTrigger=）
- PropertyChanged：目标属性变更时
- LostFocus：目标属性变更后且失去焦点时
- Explicit：明确调用 UpdateSource 方法时
- Default：由源属性元数据决定

#### 绑定数据源

指定资源或对象作为绑定源。（Source=）

#### 绑定相对数据源

寻找某个与自身存在某种相对位置关系的元素，把它作为绑定源。（RelativeSource=）
- 绑定模式（目标 Mode=）
- Self：自身
- FindAncestor：上级元素
- PreviousData
- TemplatedParent

- 绑定模式为上级元素时需要满足的条件
- 指定上级元素的种类（AncestorType=）
- 找到第n个指定种类的上级元素（AncestorLevel=）

#### 绑定数据上下文

没有指定数据源时，逐级向上寻找上级元素的 DataContext 属性，该属性非空时把它作为绑定源。

### 命令（Command）
- ICommand 接口
- RoutedCommand 类
- RoutedUICommand 类


