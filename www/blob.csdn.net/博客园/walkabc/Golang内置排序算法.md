# Golang内置排序算法 - walkabc - 博客园
# [Golang内置排序算法](https://www.cnblogs.com/hitfire/articles/6424798.html)
　　Golang官方包中包含一个sort，提供了几种排序算法。
　　比如sort.Ints、Float64、Strings，分别是对整数，浮点数，字符串的排序。
```go;gutter
//数字排序
	ages := []int{2,1,5,66,55,23,78,98,13}
	sort.Ints(ages)
	for _, value := range ages {
		fmt.Println(value)
	}
	//字符串排序
	names := []string{"Hello", "World", "private", "folders", "Users", "workspace"}
	sort.Strings(names)
	for _, value := range names {
		fmt.Println(value)
	}
```
　　除了上面几种基础的数值排序之外，golang还包含了针对复杂对象的排序。
```go;gutter
type SortSample struct {
}
func (this *SortSample) Sort() {
	var planets = []Person{
		{"Jim", 11},
		{"Jack", 23},
		{"Meimei", 43},
		{"Justin", 32},
	}
        //依据名称对人群排序
	name := func(p1, p2 *Person) bool {
		return p1.name < p2.name
	}
        //依据年龄对人群排序
	age := func(p1, p2 *Person) bool {
		return p1.age < p2.age
	}
	By(name).Sort(planets)
	fmt.Println("By name:", planets)
	By(age).Sort(planets)
	fmt.Println("By mass:", planets)
}
type earthMass float64
type au float64
type Person struct {
	name     string
	age int
}
type By func(p1, p2 *Person) bool
func (by By) Sort(planets []Person) {
	ps := &PersonSorter{
		persons: planets,
		by:      by,
	}
	sort.Sort(ps)
}
type PersonSorter struct {
	persons []Person
	by      func(p1, p2 *Person) bool
}
func (s *PersonSorter) Len() int {
	return len(s.persons)
}
func (s *PersonSorter) Swap(i, j int) {
	s.persons[i], s.persons[j] = s.persons[j], s.persons[i]
}
func (s *PersonSorter) Less(i, j int) bool {
	return s.by(&s.persons[i], &s.persons[j])
}
```
　　如果要对复杂对象排序的话，该排序必须包含Len,Swap,Less这三个方法。

