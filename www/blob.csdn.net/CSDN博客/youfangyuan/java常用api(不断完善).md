# java常用api(不断完善) - youfangyuan - CSDN博客
2013年06月16日 12:24:12[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：578

```java
//读取图片到BufferedImage中
String path = "";
BufferedImage image = ImageIO.read(path);
```
```java
//将控件绘制到BufferedImage中
BufferedImage buttonImage = getGraphicsConfiguration().createCompatibleImage(getWidth(), getHeight());
Graphics gButton = buttonImage.getGraphics();
gButton.setClip(g.getClip());
JButton btn = new JButton();
btn.paint(gButton);
```
```java
//java swing使用系统自带的样式
try {
    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
} catch (ClassNotFoundException ex) {
    ex.printStackTrace();
} catch (InstantiationException ex) {
    ex.printStackTrace();
} catch (IllegalAccessException ex) {
    ex.printStackTrace();
} catch (UnsupportedLookAndFeelException ex) {
    ex.printStackTrace();
}
```
```java
java.lang.System
//拷贝数组
static void arraycopy(Object src, int srcPos, Object dest, int destPos, int length)
```
