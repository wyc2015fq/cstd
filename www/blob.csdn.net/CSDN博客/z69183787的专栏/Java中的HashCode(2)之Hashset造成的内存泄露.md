# Java中的HashCode(2)之Hashset造成的内存泄露 - z69183787的专栏 - CSDN博客
2015年07月10日 17:44:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：760
所谓内存泄露就是一个对象占用的一块内存，当这个对象不在被使用时，该内存还没有被收回。
例子
package cn.xy.test;
public class Point2
{
 private int x;
 private int y;
 public Point2(int x, int y)
 {
  super();
  this.x = x;
  this.y = y;
 }
 @Override
**public int hashCode()** {
  final int prime = 31;
  int result = 1;
  result = prime * result + x;
  result = prime * result + y;
  return result;
 }
 @Override
 public boolean equals(Object obj)
 {
  if (this == obj) return true;
  if (obj == null) return false;
  if (getClass() != obj.getClass()) return false;
  Point2 other = (Point2) obj;
  if (x != other.x) return false;
  if (y != other.y) return false;
  return true;
 }
 public int getX()
 {
  return x;
 }
 public void setX(int x)
 {
  this.x = x;
 }
 public int getY()
 {
  return y;
 }
 public void setY(int y)
 {
  this.y = y;
 }
}
public class HashSetAndHashCode2
{
 public static void main(String[] args)
 {
  HashSet<Point2> hs2 = new HashSet<Point2>();
  Point2 p21 = new Point2(3, 3);
  Point2 p22 = new Point2(3, 5);
  hs2.add(p21);
  hs2.add(p22);
**  p22.setY(7);  hs2.remove(p22);**  System.out.println(hs2.size());
 }
}
很多人认为打印出的结果是1。结果是2。为什么呢？当一个对象被存储在Hashset中后，如果修改参与计算hashcode有关的字段，那么修改后的hashcode的值就与一开始存储进来的hashcode的值不同了，这样contains无法通过hashcode找到该元素，所以无法删除。这就告诉我们，当一个对象被存储在Hashset中后，不要修改与计算hashcode有关的字段。
