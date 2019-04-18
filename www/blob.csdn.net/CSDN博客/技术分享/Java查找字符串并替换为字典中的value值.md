# Java查找字符串并替换为字典中的value值 - 技术分享 - CSDN博客

2018年12月04日 22:00:01[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：475


## 问题：

根据字典对字符串中的字母进行替换 ，并打印出所有的可能，使用Python会更容易些，直接调用字典就可以了。这里使用Java，并通过Json解析字典。

比如说：字典数据为：{‘x’: [‘A’, ‘B’], ‘y’: [‘Y’]}，其中 ‘x’ 能够被 ‘A’ 或 'B’替换，‘y’ 能被 'Y’替换。对字符串“xabcy”进行处理，替换的所有结果为：AabcY、BabcY

## 代码

TestData.java

```
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import net.sf.json.JSONArray;
import net.sf.json.JSONException;
import net.sf.json.JSONObject;

/**
 * 该算法采用的是递归的方式，数据存储到List中，不过也可以使用循环的方式，把数据存储到数组中，或者把List和数组结合起来使用
 */

public class TestData {
	private static String dict;// 存储字典数据，测试用的,可以通过流的方式读取文件中的字典数据
	private static List<List<String>> handleLists;// 存储待处理的数据
	private static List<String> list;// 存储每个字符对应的字典数据
	private static Scanner in;// 输入数据
	private static String str;// 键入的字符串
	private static String ch;// 保存字符对应的字符串
	private static JSONArray arr;// 获取每个字符对应的字典数据

	@SuppressWarnings("deprecation")
	public static void main(String[] args) {
		dict = "{'a': ['B', 'C', 'M'], 'b': ['X','Y']}";
		handleLists = new ArrayList<List<String>>();
		in = new Scanner(System.in);
		try {
			// Json解析数据
			JSONObject json = new JSONObject(dict);
			// 循环键入数据及处理
			while (true) {
				handleLists.clear();
				System.out.println("输入字符串！");
				str = in.next();
				for (int i = 0; i < str.length(); i++) {
					list = new ArrayList<String>();
					ch = String.valueOf(str.charAt(i));
					if (json.has(ch)) {
						arr = json.getJSONArray(ch);
						for (int j = 0; j < arr.length(); j++) {
							list.add(arr.getString(j));
						}
					} else {
						list.add(String.valueOf(ch));
					}
					handleLists.add(list);
				}
				handleLists = Utils.recursitve(handleLists, new ArrayList<List<String>>(), 0, new ArrayList<String>());// 遍历所有的结果
				System.out.println("共 " + handleLists.size() + " 个结果");
				for (List<String> li : handleLists) {
					for (String s : li) {
						System.out.print(s);
					}
					System.out.println();
				}
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
}
```

**遍历数据工具包**

Utils.java

```
import java.util.ArrayList;
import java.util.List;

public class Utils {
	/**
	 * 递归实现对originalLists中所有层数据的遍历，结果放在resultLists中
	 * 
	 * @param originalLists
	 *            原始数据
	 * @param resultLists
	 *            结果数据
	 * @param layer
	 *            originalLists的层数
	 * @param result
	 *            每次遍历的结果
	 */
	public static List<List<String>> recursitve(List<List<String>> originalLists, List<List<String>> resultLists,
			int layer, List<String> result) {
		if (layer < originalLists.size() - 1) {
			if (originalLists.get(layer).size() == 0) {
				recursitve(originalLists, resultLists, layer + 1, result);
			} else {
				for (int i = 0; i < originalLists.get(layer).size(); i++) {
					List<String> list = new ArrayList<String>(result);
					list.add(originalLists.get(layer).get(i));
					recursitve(originalLists, resultLists, layer + 1, list);
				}
			}
		} else if (layer == originalLists.size() - 1) {
			if (originalLists.get(layer).size() == 0) {
				resultLists.add(result);
			} else {
				for (int i = 0; i < originalLists.get(layer).size(); i++) {
					List<String> list = new ArrayList<String>(result);
					list.add(originalLists.get(layer).get(i));
					resultLists.add(list);
				}
			}
		}
		return resultLists;
	}
}
```

