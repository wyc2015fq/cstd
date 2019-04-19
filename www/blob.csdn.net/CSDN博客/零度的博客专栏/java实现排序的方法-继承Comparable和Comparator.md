# java实现排序的方法-继承Comparable和Comparator - 零度的博客专栏 - CSDN博客
2016年04月08日 16:19:58[零度anngle](https://me.csdn.net/zmx729618)阅读数：5827
**1、java提供的默认List和数组排序方法**
      代码实现：
```
package zmx.sort.test;
import java.text.Collator;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
public class CollectionSort {
	
	public static void main(String[] args) {
		
		//集合排序
		List<String> list = new ArrayList<>();
		list.add("刘媛媛");
		list.add("王硕");
		list.add("李明");
		list.add("张迪");
		list.add("刘布");	
		//升序
		Collections.sort(list,Collator.getInstance(java.util.Locale.CHINA));//注意：是根据的汉字的拼音的字母排序的，而不是根据汉字一般的排序方法
		for(int i=0;i<list.size();i++)
		{
		    System.out.print(list.get(i));
		}
		System.out.println("");
		//降序
		Collections.reverse(list);//不指定排序规则时，也是按照字母的来排序的
		for(int i=0;i<list.size();i++)
		{
		     System.out.print(list.get(i));
		}
		
		
		
		//数组排序
		System.out.println();
		String[] names = {"王林",  "杨宝", "李镇", "刘迪", "刘波"}; 
		Arrays.sort(names, Collator.getInstance(java.util.Locale.CHINA));
		System.out.println(Arrays.toString(names));      
		
		
	}
}
```
控制台输出：
李明刘布刘媛媛王硕张迪
张迪王硕刘媛媛刘布李明
[李镇, 刘波, 刘迪, 王林, 杨宝]
**2、自定义排序实现**
**     2.1、第一种是model类实现Comparable接口，重写int compareTo(Object o)方法**
```
package zmx.sort.test;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
/**
 * 自定义排序
 * 方法一： model类实现Comparable接口，
 * 重写int compareTo(Object o)方法
 * @author zhangwenchao
 *
 */
public class StudentDTO implements Comparable{
	private String name;
	
	private Integer age;
	
	
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public Integer getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	@Override
	public int compareTo(Object o) {
		StudentDTO sdto = (StudentDTO)o;
		int otherAge = sdto.getAge();
		return this.age.compareTo(otherAge);
		
	}
	
	
	public static void main(String[] args) {
		List<StudentDTO> studentList = new ArrayList<>();
		StudentDTO  s = new StudentDTO ();
		s.setName("yuanyuan");
	    s.setAge(22);
		studentList.add(s);
		StudentDTO s1 = new StudentDTO ();
		s1.setName("lily");
		s1.setAge(23);
        studentList.add(s1);
		Collections.sort(studentList); 
		for(StudentDTO sd : studentList){
			System.out.print(sd.getAge()+" ");
		}
        System.out.println();
        
		Collections.reverse(studentList); 
		for(StudentDTO sd : studentList){
			System.out.print(sd.getAge()+" ");
		}
        System.out.println(); 
	}
	
	
	
}
```
` `
输出：
22 23 
23 22
**2.2、 ****第二种是实现一个比较器类，该比较器实现Comparator接口，重写int compare(Object o1, Object o2)方法；**
```
package zmx.sort.test;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
public class MyStudentDTOCompartor implements Comparator<StudentDTO>{
	@Override
	public int compare(StudentDTO o1, StudentDTO o2) {
        return o1.getAge().compareTo(o2.getAge());
	}
	
	
	public static void main(String[] args) {
		
		List<StudentDTO> studentList = new ArrayList<>();
		StudentDTO  s = new StudentDTO ();
		s.setName("yuanyuan");
	    s.setAge(22);
		studentList.add(s);
		StudentDTO s1 = new StudentDTO ();
		s1.setName("lily");
		s1.setAge(23);
        studentList.add(s1);
        MyStudentDTOCompartor msc = new MyStudentDTOCompartor();  //定义比较器
		Collections.sort(studentList, msc);   //使用比较器进行排序
		for(StudentDTO sd : studentList){
			System.out.print(sd.getAge()+" ");
		}
        System.out.println();
        
		Collections.reverse(studentList);
		
		for(StudentDTO sd : studentList){
			System.out.print(sd.getAge()+" ");
		}
        System.out.println(); 
	}
	
	
}
```
输出：
22 23 
23 22

