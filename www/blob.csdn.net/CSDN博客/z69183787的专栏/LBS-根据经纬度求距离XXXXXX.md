# LBS-根据经纬度求距离XXXXXX - z69183787的专栏 - CSDN博客
2018年11月20日 12:28:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：101
个人分类：[工具-Lbs排序](https://blog.csdn.net/z69183787/article/category/6469379)
```java
public class DistanceUtil {
    private static final double EARTH_RADIUS = 6378.137;
    private static double rad(double d) {
        return d * Math.PI / 180.0;
    }
    /**
     * @param long1
     * @param lat1
     * @param long2
     * @param lat2
     * @return 两点距离，单位（千米）
     */
    public static double getDistance(Double long1, Double lat1, Double long2, Double lat2) {
        if (long1 == null || lat1 == null || long2 == null || lat2 == null) return 0;
        double a, b, d, sa2, sb2;
        lat1 = rad(lat1);
        lat2 = rad(lat2);
        a = lat1 - lat2;
        b = rad(long1 - long2);
        sa2 = Math.sin(a / 2.0);
        sb2 = Math.sin(b / 2.0);
        d = 2 * EARTH_RADIUS
                * Math.asin(Math.sqrt(sa2 * sa2 + Math.cos(lat1)
                * Math.cos(lat2) * sb2 * sb2));
        return d;
    }
    public static void main(String[] args) {
        List<Worker> workers = new ArrayList<>();
        workers.add(new Worker(1));
        workers.add(new Worker(2));
        workers.add(new Worker(3));
        workers.add(new Worker(4));
        workers.add(new Worker(5));
        workers.parallelStream().filter(Worker::finish).findFirst().ifPresent(first -> System.out.println("id : " + first.id));
    }
    static class Worker {
        int id;
        Worker(int id) {
            this.id = id;
        }
        boolean finish() {
            int t = id * 1000;
            System.out.println(id + " -> " + t);
            try {
                Thread.sleep(t);
            } catch (InterruptedException ignored) {
            }
            System.out.println(id+" finished");
            return id <= 3;
        }
    }
}
```
