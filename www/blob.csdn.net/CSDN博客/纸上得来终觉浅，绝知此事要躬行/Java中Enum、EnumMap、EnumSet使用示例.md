# Java中Enum、EnumMap、EnumSet使用示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月09日 22:36:26[boonya](https://me.csdn.net/boonya)阅读数：2936








原文地址：[https://examples.javacodegeeks.com/java-basics/java-enumeration-example/](https://examples.javacodegeeks.com/java-basics/java-enumeration-example/)

=========原文讲解也是比较细致，大略观之，以快图之。==========

## 1.Enum示例

Day.java


```java
public enum Day {
    SUNDAY(1),
    MONDAY(2),
    TUESDAY(3),
    WEDNESDAY(4),
    THURSDAY(5),
    FRIDAY(6),
    SATURDAY(7)

     

     private int value;
    

     // 私有构造
    private Day(int value) {this.value = value; }

    // 重写toString方法
   @Override
   public String toString() {
      switch(this) {
        case FRIDAY:
            return "Friday: " + value;
        case MONDAY:
            return "Monday: " + value;
        case SATURDAY:
            return "Saturday: " + value;
        case SUNDAY:
            return "Sunday: " + value;
        case THURSDAY:
            return "Thursday: " + value;
        case TUESDAY:
            return "Tuesday: " + value;
        case WEDNESDAY:
            return "Wednesday: " + value;
        default:
            return null;
   }
 }

 }
```


Car.java


```java
public enum Car {
    AUDI {
        @Override
        public int getPrice() {
            return 25000;
        }
    },
    MERCEDES {
        @Override
        public int getPrice() {
            return 30000;
        }
    },
    BMW {
        @Override
        public int getPrice() {
            return 20000;
        }
    };
    
    public abstract int getPrice();
}
```


## 2.EnumMap示例


```java
import java.util.EnumMap;

public class EnumMapExample {
    public static void main(String[] args) {
        // Create an EnumMap that contains all constants of the Car enum.
        EnumMap cars = new EnumMap(Car.class);
        
        // Put some values in the EnumMap.
        cars.put(Car.BMW, Car.BMW.getPrice());
        cars.put(Car.AUDI, Car.AUDI.getPrice());
        cars.put(Car.MERCEDES, Car.MERCEDES.getPrice());
        
        // Print the values of an EnumMap.
        for(Car c: cars.keySet())
            System.out.println(c.name());
        
        System.out.println(cars.size());
        
        // Remove a Day object.
        cars.remove(Car.BMW);
        System.out.println("After removing Car.BMW, size: " + cars.size());
        
        // Insert a Day object.
        cars.put(Car.valueOf("BMW"), Car.BMW.getPrice());
        System.out.println("Size is now: " + cars.size());
    }
}
```


## 3.EnumSet示例


```java
import java.util.EnumSet;

public class EnumSetExample {
    public static void main(String[] args) {
        // Create an EnumSet that contains all days of the week.
        EnumSet week = EnumSet.allOf(Day.class);
        
        // Print the values of an EnumSet.
        for(Day d: week)
            System.out.println(d.name());
        
        System.out.println(week.size());
        
        // Remove a Day object.
        week.remove(Day.FRIDAY);
        System.out.println("After removing Day.FRIDAY, size: " + week.size());
        
        // Insert a Day object.
        week.add(Day.valueOf("FRIDAY"));
        System.out.println("Size is now: " + week.size());
    }
}
```








