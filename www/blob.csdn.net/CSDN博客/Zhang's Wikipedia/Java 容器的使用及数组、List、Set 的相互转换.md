
# Java 容器的使用及数组、List、Set 的相互转换 - Zhang's Wikipedia - CSDN博客


2018年06月10日 19:21:35[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：888



## 0. Utils
字符串数组的排序：
Set<String> set = new TreeSet<String>();
Collections.addAll(set, args);	// args 为 String[] 类型
数组复制：
system.arraycopy：必须明确自行建立一个数组对象，
int[] arr1 = {1, 2, 3, 4, 5}; 
int[] arr2 = new int[5];
System.arraycopy(arr1, 0, arr2, 0, arr1.length);Arrays.copyOf：可直接传回一个数组对象：
int[] arr1 = {1, 2, 3, 4, 5}; 
int[] arr2 = Arrays.copyOf(arr1, arr1.length);
// 如果新数组的长度超过原数组的长度，则保留数组默认值
int[] arr3 = Arrays.copyOf(arr1, 10);
		// 1 2 3 4 5 0 0 0 0 0
## 1. List <–> Array
list 实例转换为 String[]，使用 list 的实例方法：toArray
List<String> list = new ArrayList<String>();  
list.add("China");  
list.add("Switzerland");  
list.add("Italy");  
list.add("France");  
String [] countries = list.toArray(new String[list.size()]);
String[] => list，使用 Arrays 的静态方法 asList
String[] countries = {"China", "Switzerland", "Italy", "France"};  
List list = Arrays.asList(countries);
## 2. Array => list => Set
其实是先将 array 转换为 list 再转换为 set：
String [] countries = {"India", "Switzerland", "Italy"};        
Set<String> set = new HashSet<String>(Arrays.asList(countries));
## 3. Map => Set/List
Map => List
List<Value> list = new ArrayList<Value>(map.values());
Map => Set
Set<String> targetSet = new HashSet<>(sourceMap.values());
## 4. Collections
shuffle：置乱一个数组（list）
public static void shuffle(List<?> list, Random rnd) {
        int size = list.size();
        if (size < SHUFFLE_THRESHOLD || list instanceof RandomAccess) {
            for (int i=size; i>1; i--)
                swap(list, i-1, rnd.nextInt(i));
        } else {
            Object arr[] = list.toArray();
            // Shuffle array
            for (int i=size; i>1; i--)
                swap(arr, i-1, rnd.nextInt(i));
            ListIterator it = list.listIterator();
            for (int i=0; i<arr.length; i++) {
                it.next();
                it.set(arr[i]);
            }
        }
    }

