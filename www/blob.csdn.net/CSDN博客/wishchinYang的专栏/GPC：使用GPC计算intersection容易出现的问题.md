# GPC：使用GPC计算intersection容易出现的问题 - wishchinYang的专栏 - CSDN博客
2018年05月22日 15:22:24[wishchin](https://me.csdn.net/wishchin)阅读数：570
     在使用GPC计算多边形的交的时候，出现问题
     //1.2. 另一种方法，判断新的多边形是否和老多边形相交
     Poly cross = (PolyDefault) Clip.intersection( filed, polyNig );    
     若filed 为两个分离的多边形，则出现计算问题：
     catch (Exception exception) {
                exception.printStackTrace();
            }
     不知道如何解决！！！
后记：
   没办法，只能修改掉，使用遍历方式
```cpp
// 对一个复合多边形判断交
    public static Poly intersection(Poly poly, Poly p2) {
        Poly p0 = new PolyDefault();
        List<Poly> p1 = new ArrayList<>();
        int num = poly.getNumInnerPoly();
        if (num > 1) {
            for (int i = 0; i < num; ++i) {
                Poly psin = (PolyDefault) poly.getInnerPoly(i);
                p1.add(psin);
            }
            p0 = intersection(p1, p2);
        } else {
            p0 = (PolyDefault) Clip.intersection(poly, p2);
        }
        return p0;
    }
```
