# ReactiveX 学习笔记（19）使用 RxSwift + RxCocoa 进行 GUI 编程 - zwvista - 博客园

## [ReactiveX 学习笔记（19）使用 RxSwift + RxCocoa 进行 GUI 编程](https://www.cnblogs.com/zwvista/p/9545087.html)

### 课题
- 程序界面由3个文本编辑框和1个文本标签组成。
- 要求文本标签实时显示3个文本编辑框所输入的数字之和。
- 文本编辑框输入的不是合法数字时，将其值视为0。
- 3个文本编辑框的初值分别为1，2，3。

### 创建工程

打开 Xcode，File / New / Project..

在 New Project 向导的第1页，选 iOS / Single View App

在向导的第2页填上 Product Name: RxExample

在向导的第3页选择任意文件夹点击 Create 按钮创建工程

关闭所创建的工程

### 配置 Pods

在工程所在文件夹下创建 Podfile 文件，内容如下：

```
use_frameworks!

target 'RxExample' do
project 'RxExample'
pod 'RxSwift'
pod 'RxCocoa'
end
```

打开终端在工程所在文件夹下执行 pod install 命令。

```
$ cd RxExample
$ pod install
...
Installing RxCocoa (4.2.0)
Installing RxSwift (4.2.0)
...
```

用 Xcode 打开 RxExample.xcworkspace

### 配置 UI

打开 Main.storyboard 文件，在 View Controller Scene / View Controller / View 下面放置3个 Text Field 和1个 Label。

3个 Text Field 的初值分别设为1，2，3。

### 控件变量

打开 ViewController.swift 文件，在 ViewController 类中添加控件变量

```
@IBOutlet weak var tfNumber1: UITextField!
@IBOutlet weak var tfNumber2: UITextField!
@IBOutlet weak var tfNumber3: UITextField!
@IBOutlet weak var lblResult: UILabel!
```

使用 Assistant Editor 同时打开 Main.storyboard 文件，用 Control 键将控件变量挂到相应的控件上。

### 不使用 RxSwift 的传统解决方案

打开 ViewController.swift 文件，在 ViewController 类中添加事件方法

```
@IBAction func tfNumberChanged(_ sender: Any) {
    let num1 = Int(tfNumber1.text!) ?? 0
    let num2 = Int(tfNumber2.text!) ?? 0
    let num3 = Int(tfNumber3.text!) ?? 0
    lblResult.text = String(num1 + num2 + num3)
}
```

使用 Assistant Editor 同时打开 Main.storyboard 文件，用 Control 键将 tfNumberChanged 方法挂到3个 Text Field 的 Editing Changed 事件上。

在 ViewController 类的 viewDidLoad 方法中添加事件方法的调用
`tfNumberChanged(self)`
### 使用 RxSwift 的解决方案

在 ViewController.swift 文件中添加 RxSwift 和 RxCocoa 的引用，然后在 ViewController 类中添加 DisposeBag 类型的实例。

```
import RxSwift
import RxCocoa

class ViewController {
// ...
let disposeBag = DisposeBag()
// ...
}
```

在 ViewController 类的 viewDidLoad 方法中添加以下代码

```
Observable.combineLatest(tfNumber1.rx.text.orEmpty, tfNumber2.rx.text.orEmpty, tfNumber3.rx.text.orEmpty) { num1, num2, num3 -> Int in
    (Int(num1) ?? 0) + (Int(num2) ?? 0) + (Int(num3) ?? 0) }
    .map { String($0) }
    .bind(to: lblResult.rx.text)
    .disposed(by: disposeBag)
```


