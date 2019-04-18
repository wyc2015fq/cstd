# Java fx 坐标揭秘 - weixin_33985507的博客 - CSDN博客
2016年11月22日 11:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
scene graph
一张呈现为树结构的数据结构，java fx 的渲染系统 是通过该数据结构来完成图形的渲染。
- 所有在树结构的每一个节点称为一个node
- 根节点(root)是唯一一个没有父母节点的node
- 没有子节点的node的称为leaf node（叶子节点）
1 . 在scene graph中的每一个node，都有自己的笛卡尔坐标系
![2053209-08c9dfc884259f07.png](https://upload-images.jianshu.io/upload_images/2053209-08c9dfc884259f07.png)
Paste_Image.png
2 . 每个node 的视图“呈现”除了几何图形外，其实还包括effect(特效),clip（裁剪）,transformation（变换）等特性
3 . Fx 针对每个node 对所包含的特性，提供了不同bound框来囊括这些特性，不同bound的范围
- layoutBounds : node geometry
- boundsInLocal: node geometry + effects + clip
- boundsInParent: node geometry + effects + clip + transformations
![2053209-952e5525459db5f5.png](https://upload-images.jianshu.io/upload_images/2053209-952e5525459db5f5.png)
Paste_Image.png
```java
public class BoundTest extends Application {
    public static void main(String[] args) {
        Application.launch(args);
    }
    @Override
    public void start(Stage primaryStage) throws Exception {
        VBox root  = new VBox();
        Scene scene = new Scene(root);
        primaryStage.setScene(scene);
        
        // 1. only a simple button
        System.out.println("only a simple button");
        Button button1 = new Button();
        System.out.println("button1=" + button1.getLayoutBounds());
        System.out.println("button1=" + button1.getBoundsInLocal());
        System.out.println("button1=" + button1.getBoundsInParent());
        
        System.out.println();
        System.out.println("add shadow");
        // 2. add shadow
        Button button2 = new Button();
        button2.setEffect(new BoxBlur());
        System.out.println("button2=" + button2.getLayoutBounds());
        System.out.println("button2=" + button2.getBoundsInLocal());
        System.out.println("button2=" + button2.getBoundsInParent());
        
        System.out.println();
        System.out.println("add transformation");
        // 3. add transformation
        Button button3 = new Button();
        //button3.setRotate(60);
        button3.getTransforms().add(new Translate(150, 75));
        System.out.println("button3=" + button3.getLayoutBounds());
        System.out.println("button3=" + button3.getBoundsInLocal());
        System.out.println("button3=" + button3.getBoundsInParent());
        
        root.getChildren().addAll(button1, new Group(button2), button3);
        //primaryStage.show();
    }
}
```
4 . 别名
- Layout Bounds: 逻辑bound(logic bound)
- BoundsInLocal: 现实上的bound(physical bound) （希望当前node包含Effect，Clip时候用的，另一种情况检测两个node有无碰撞）
- BoundsInParent: 虚幻的bound（visual bound) (较少在code中使用到）
5 .这些Bound 处于不同的坐标系中
Layout Bounds: 在该node 的坐标系
BoundsInLocas:在该node 的坐标系
BoundsInParent:在该node 的parent的坐标系
了解这些不同的Bounds
6 . 每一个node 都有一对LayoutX和LayoutY以及一对translateX和translateY 来描述他们在坐标系中的位置
LayoutX 和LayoutY : 固定（stable) layout
translateX和translateY: 动态（dynamic) layout  (animaiton中)
finalTranslationX = layoutX + translateX
finalTranslationY = layoutY + translateY
但是layoutX and layoutY 并不是就是直接确定了node的位置，它通常需要转为node的坐标系中的位置，通常需要补偿一个layoutBound.minX和layoutBound.minY
```
layoutX = finalX - node.getLayoutBounds().getMinX();
layoutY = finalY - node.getLayoutBounds().getMinY();
```
7 . 由于Bound 的存在，当你想设置坐标时候，记得加上边框左上角的位置的横纵坐标，推荐使用relocate()  ，它会帮你补偿minX 和minY of layoutBound，自动完成6中提到的转换
8 .  当Parenty 为region 类时候，父节点有自己的位置策略，这时候忽略node layoutX,layoutY. 事实上只有当你的parent节点（layout manager）为Group或着Pane的时候，通过修改layoutX以及layoutY才能修改Node的位置
### Node 的Size
- 
当一个node可以随着父节点layout的调整自己的大小，该节点就是可以resize的.Node 的分为resizable和non-resize node.
- re-sizable node: Regions, Controls, andWebView
- non-resizable node : Group, Text, and Shapes
- 
三个“大小”
- Preferred size 最合适大小，当layout 足够满足的时候呈现的size
- Minimum size 最小的大小，当resize的时候的最小大小
- Maximum size 最大的大小， 当resize的时候的最大大小
- 
对着三个size可以设置两个常量，
USE_COMPUTED_SIZE ：依据的node的内容和属性计算
USE_PREF_SIZE：通常用来设置Minimum size和Maximum size保持和Preferred size一致
- 
每个node也有6个属性来对应这个三个特性,这些属性的默认值为：USE_COMPUTED_SIZE
prefWidth,prefHeight
minWidth,minHeight
maxWidth,maxHeight
- 
不要相信getXXX(getMaxWidth,getMinWidth,getPre...),通常这些方法返回的值并没有真实反映node的大小
- 
使用以下方法：
```
double prefWidth(double height) 
double prefHeight(double width)
double minWidth(double height)
double minHeight(double width)
double maxWidth(double height)
double maxHeight(double width)
```
通过传入-1来获得content bias 边的长度，再根据该长度算得相应的宽或者高
![2053209-f013ea5555906746.png](https://upload-images.jianshu.io/upload_images/2053209-f013ea5555906746.png)
Paste_Image.png
```
public class NodeSizes extends Application {
    public static void main(String[] args) {
        Application.launch(args);
    }
    @Override
    public void start(Stage stage) {
        Button btn = new Button("Hello JavaFX!");
        HBox root = new HBox();
        root.getChildren().addAll(btn);
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.setTitle("Sizes of a Node");
        stage.show();
        // Print button's sizes
        System.out.println("Before changing button properties:");
        printSizes(btn);
        // Change button's properties
        btn.setWrapText(true);
        btn.setPrefWidth(80);
        stage.sizeToScene();
        // Print button's sizes
        System.out.println("\nAfter changing button properties:");
        printSizes(btn);
    }
    public void printSizes(Button btn) {
        System.out.println("btn.getContentBias() = " + btn.getContentBias());
        System.out.println("btn.getPrefWidth() = " + btn.getPrefWidth() + 
                           ", btn.getPrefHeight() = " + btn.getPrefHeight());
        System.out.println("btn.getMinWidth() = " + btn.getMinWidth() + 
                           ", btn.getMinHeight() = " + btn.getMinHeight());
        System.out.println("btn.getMaxWidth() = " + btn.getMaxWidth() + 
                           ", btn.getMaxHeight() = " + btn.getMaxHeight());
        double prefWidth = btn.prefWidth(-1);
        System.out.println("btn.prefWidth(-1) = " + prefWidth + 
               ", btn.prefHeight(prefWidth) = " + btn.prefHeight(prefWidth));
        double minWidth = btn.minWidth(-1);
        System.out.println("btn.minWidth(-1) = " + minWidth + 
               ", btn.minHeight(minWidth) = " + btn.minHeight(minWidth));
        double maxWidth = btn.maxWidth(-1);
        System.out.println("btn.maxWidth(-1) = " + maxWidth + 
               ", btn.maxHeight(maxWidth) = " + btn.maxHeight(maxWidth));
        System.out.println("btn.getWidth() = " + btn.getWidth() + 
                           ", btn.getHeight() = " + btn.getHeight());
    }
}
```
7 对于non-resizable node
prefWidth(double h), minWidth(double h),  maxWidth(double h) 返回layoutBound的width
prefHeight(double w), minHeight(double w), maxHeight(double w)  返回layoutBound的height
8 关于managed node
当子节点的managedProperty设置为false的时候，父节点在layout布局的时候，就不会帮把计算如layoutboundbox. 这与visibleProperty的区别，尽管它不可见，父节点仍考虑它的布局。
```
public class SlidingLeftNodeTest extends Application {
    public static void main(String[] args) {
        Application.launch(args);
    }
    @Override
    public void start(Stage stage) {
        Button b1 = new Button("B1");
        Button b2 = new Button("B2");
        Button b3 = new Button("B3");
        Button visibleBtn = new Button("Make Invisible");
        // Add an action listener to the button to make b2 visible
        // if it is invisible and invisible if it is visible
        visibleBtn.setOnAction(e -> b2.setVisible(!b2.isVisible()));
        // Bind the text property of the button to the visible
        // property of the b2 button
        visibleBtn.textProperty().bind(new When(b2.visibleProperty()).then("Make Invisible").otherwise("Make Visible"));
        b2.managedProperty().bind(b2.visibleProperty());
        HBox root = new HBox();
        root.getChildren().addAll(visibleBtn, b1, b2, b3);
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.setTitle("Sliding to the Left");
        stage.show();
    }
}
```
