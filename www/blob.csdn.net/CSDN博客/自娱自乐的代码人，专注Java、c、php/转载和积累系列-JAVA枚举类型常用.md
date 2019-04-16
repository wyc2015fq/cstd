# 转载和积累系列 - JAVA枚举类型常用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年05月04日 19:15:21[initphp](https://me.csdn.net/initphp)阅读数：811








```java
/*最普通的枚举*/
    public enum ColorSelect {
        red, green, yellow, blue;    
    }

//带构造函数和属性的枚举类型
    public enum TestEnum {
        TEST_0("init", "100"), TEST_2("php", "200");

        private String key;

        private String value;

        TestEnum(String key, String value) {
            this.key = key;
            this.value = value;
        }

        //可以通过key获取value的值
        private final static Map<String, String> pool = new HashMap<String, String>();

        static {
            for (TestEnum each : TestEnum.values()) {
                pool.put(each.key, each.value);
            }
        }

        public static String getValue(String key) {
            return pool.get(key);
        }

    }
```




