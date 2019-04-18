# JavaFx中Controller获取Stage并自定义窗口移动与缩放逻辑 - weixin_33985507的博客 - CSDN博客
2019年02月05日 13:25:42[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# JavaFx中Controller获取Stage并自定义窗口移动与缩放逻辑
由于去掉了平台自带了标题栏，窗口就无法移动和缩放了，需要我们自己来实现。
## 去除窗口标题栏
```java
public class Main extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        Parent root = FXMLLoader.load(getClass().getResource("main.fxml"));
        primaryStage.setTitle("main");
        // 主要是这一句
        primaryStage.initStyle(StageStyle.TRANSPARENT);
        primaryStage.setScene(new Scene(root));
        primaryStage.show();
    }
    public static void main(String[] args) {
        launch(args);
    }
}
```
## 在Controller中获取Stage
首先确定在Controller中获取到你的跟布局对象，如下：
```java
public class Controller implements Initializable {
    public BorderPane root;
    private Stage stage;
}
```
```xml
<BorderPane fx:id="root" maxHeight="-Infinity" maxWidth="-Infinity" minHeight="-Infinity" minWidth="-Infinity" prefHeight="400.0" prefWidth="600.0" stylesheets="@../css/main.css" xmlns="http://javafx.com/javafx/8.0.172-ea" xmlns:fx="http://javafx.com/fxml/1" fx:controller="line.main.Controller">
      <!--其他布局元素-->
</BorderPane>
```
然后可以在`public void initialize(URL url, ResourceBundle resourceBundle) `方法内获取Stage
```java
public class Controller implements Initializable {
    public BorderPane root;
    private Stage stage;
    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
       var stage = getStage();
    }
    private Stage getStage() {
        if (stage == null) {
            stage = (Stage) root.getScene().getWindow();
        }
        return stage;
    }
}
```
## 自定义窗体移动与缩放逻辑
- 
记录鼠标坐标
```java
root.setOnMousePressed(event -> {
            event.consume();
            xOffset = event.getSceneX();
            yOffset = event.getSceneY();
        });
```
- 
自定义移动控制区域
```java
dragArea.setOnMouseDragged(event -> {
            event.consume();
            var stage = getStage();
            stage.setX(event.getScreenX() - xOffset);
            stage.setY(event.getScreenY() - yOffset);
        });
```
- 
判断鼠标坐标位置，并处理光标变换
```java
root.setOnMouseMoved(this::mouseMoveHandle);
 private void mouseMoveHandle(MouseEvent event) {
        event.consume();
        double x = event.getSceneX();
        double y = event.getSceneY();
        var stage = getStage();
        double width = stage.getWidth();
        double height = stage.getHeight();
        Cursor cursorType = Cursor.DEFAULT;
        bit = 0;
        if (y >= height - RESIZE_WIDTH) {
            if (x <= RESIZE_WIDTH) {
                bit |= 1 << 3;
            } else if (x >= width - RESIZE_WIDTH) {
                bit |= 1;
                bit |= 1 << 2;
                cursorType = Cursor.SE_RESIZE;
            } else {
                bit |= 1;
                cursorType = Cursor.S_RESIZE;
            }
        } else if (x >= width - RESIZE_WIDTH) {
            bit |= 1 << 2;
            cursorType = Cursor.E_RESIZE;
        }
        root.setCursor(cursorType);
    }
```
- 
处理窗口缩放
```java
root.setOnMouseDragged(this::mouseDraggedHandle);
private void mouseDraggedHandle(MouseEvent event) {
    event.consume();
    var primaryStage = getStage();
    double x = event.getSceneX();
    double y = event.getSceneY();
    double nextX = primaryStage.getX();
    double nextY = primaryStage.getY();
    double nextWidth = primaryStage.getWidth();
    double nextHeight = primaryStage.getHeight();
    if ((bit & 1 << 2) != 0) {
        nextWidth = x;
    }
    if ((bit & 1) != 0) {
        nextHeight = y;
    }
    if (nextWidth <= MIN_WIDTH) {
        nextWidth = MIN_WIDTH;
    }
    if (nextHeight <= MIN_HEIGHT) {
        nextHeight = MIN_HEIGHT;
    }
    primaryStage.setX(nextX);
    primaryStage.setY(nextY);
    primaryStage.setWidth(nextWidth);
    primaryStage.setHeight(nextHeight);
}
```
## 完整代码
```java
package line.main;
import javafx.fxml.Initializable;
import javafx.scene.Cursor;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import java.net.URL;
import java.util.ResourceBundle;
public class Controller implements Initializable {
    public BorderPane root;
    public Pane dragArea;
    private Stage stage;
    private double xOffset = 0;
    private double yOffset = 0;
    private int bit = 0;//left,right,top,bottom
    private static final double RESIZE_WIDTH = 5.00;
    private static final double MIN_WIDTH = 600.00;
    private static final double MIN_HEIGHT = 400.00;
    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        dragArea.setOnMouseDragged(event -> {
            event.consume();
            var stage = getStage();
            stage.setX(event.getScreenX() - xOffset);
            stage.setY(event.getScreenY() - yOffset);
        });
        root.setOnMousePressed(event -> {
            event.consume();
            xOffset = event.getSceneX();
            yOffset = event.getSceneY();
        });
        root.setOnMouseMoved(this::mouseMoveHandle);
        root.setOnMouseDragged(this::mouseDraggedHandle);
    }
    private Stage getStage() {
        if (stage == null) {
            stage = (Stage) root.getScene().getWindow();
        }
        return stage;
    }
    private void mouseMoveHandle(MouseEvent event) {
        event.consume();
        double x = event.getSceneX();
        double y = event.getSceneY();
        var stage = getStage();
        double width = stage.getWidth();
        double height = stage.getHeight();
        Cursor cursorType = Cursor.DEFAULT;
        bit = 0;
        if (y >= height - RESIZE_WIDTH) {
            if (x <= RESIZE_WIDTH) {
                bit |= 1 << 3;
            } else if (x >= width - RESIZE_WIDTH) {
                bit |= 1;
                bit |= 1 << 2;
                cursorType = Cursor.SE_RESIZE;
            } else {
                bit |= 1;
                cursorType = Cursor.S_RESIZE;
            }
        } else if (x >= width - RESIZE_WIDTH) {
            bit |= 1 << 2;
            cursorType = Cursor.E_RESIZE;
        }
        root.setCursor(cursorType);
    }
    private void mouseDraggedHandle(MouseEvent event) {
        event.consume();
        var primaryStage = getStage();
        double x = event.getSceneX();
        double y = event.getSceneY();
        double nextX = primaryStage.getX();
        double nextY = primaryStage.getY();
        double nextWidth = primaryStage.getWidth();
        double nextHeight = primaryStage.getHeight();
        if ((bit & 1 << 2) != 0) {
            nextWidth = x;
        }
        if ((bit & 1) != 0) {
            nextHeight = y;
        }
        if (nextWidth <= MIN_WIDTH) {
            nextWidth = MIN_WIDTH;
        }
        if (nextHeight <= MIN_HEIGHT) {
            nextHeight = MIN_HEIGHT;
        }
        primaryStage.setX(nextX);
        primaryStage.setY(nextY);
        primaryStage.setWidth(nextWidth);
        primaryStage.setHeight(nextHeight);
    }
}
```
