# go语言基础 - Orisun - 博客园







# [go语言基础](https://www.cnblogs.com/zhangchaoyang/articles/7784781.html)





## slice

```
package main

import "fmt"

func main() {
    s := make([]int, 3, 4)
    s[0] = 0
    s[1] = 1
    s[2] = 2
    s1 := s[0:2] //s, s1, s2底层用的是同一个数组
    s2 := s[1:4] //slice index可以超出len，但不能超出capacity，超出len的元素值为0
    fmt.Println(cap(s), len(s), s)
    fmt.Println(s1)
    fmt.Println(s2)
    fmt.Println("*****************")

    s1 = append(s1, 9) //append操作导致修改了底层数组的第3个元素
    fmt.Println(cap(s), len(s), s)
    fmt.Println(s1)
    fmt.Println(s2)
    fmt.Println("*****************")

    s = append(s, 7,6) //append操作导致重新分配了一个capacity相当于原先2倍的内存空间。s指向新的数组，而s1和s2还是指向老的数组
    s[1] = 6
    fmt.Println(cap(s), len(s), s)
    fmt.Println(s1)
    fmt.Println(s2)
    fmt.Println("*****************")

    s=append(s,s...)//可以一次append多个元素，甚至是一个slice
    fmt.Println(cap(s), len(s), s)
    fmt.Println("*****************")
}
```

 输出：

4 3 [0 1 2]
[0 1]
[1 2 0]
*****************
4 3 [0 1 9]
[0 1 9]
[1 9 0]
*****************
8 5 [0 6 9 7 6]
[0 1 9]
[1 9 0]
*****************
16 10 [0 6 9 7 6 0 6 9 7 6]
*****************

## json

```
package main

import (
    "encoding/json"
    "fmt"
)

type Student struct {
    Name  string    //默认情况下，序列化成json后字段名就是"Name"
    Class string `json:"class"`    //显式指定序列化成json后字段名就是"class"
    Score int    `json:"score,omitempty"`//omitempty表示如果该字段是空值，那在json里就不显示该字段了
}

func main() {
    stu := Student{Name: "orisun"}
    bytes, _ := json.Marshal(stu)
    str := string(bytes)
    fmt.Println(str)

    stu2 := Student{}
    err := json.Unmarshal(bytes, &stu2)
    if err != nil {
        panic(err)
    }
    fmt.Printf("%+v", stu2)
}
```

 输出：

{"Name":"orisun","class":""}
{Name:orisun Class: Score:0}

## 格式化

简单的字符串格式化用 fmt.Sprint("format", value)，格式化输出用fmt.Printf("format", value)。Sprintf和Printf背后都是调的Fprintf，os.Stdout和bytes.Buffer都实现了io.Writer接口。

```
package fmt

func Fprintf(w io.Writer, format string, args ...interface{}) (int, error)

func Printf(format string, args ...interface{}) (int, error) {
    return Fprintf(os.Stdout, format, args...)
}

func Sprintf(format string, args ...interface{}) string {
    var buf bytes.Buffer
    Fprintf(&buf, format, args...)
    return buf.String()
}
```



```
%d          十进制整数
%x, %o, %b  十六进制，八进制，二进制整数。
%f, %g, %e  浮点数： 3.141593 3.141592653589793 3.141593e+00
%t          布尔：true或false
%c          字符（rune） (Unicode码点)
%s          字符串
%q          带双引号的字符串"abc"或带单引号的字符'c'
%v          变量的自然形式（natural format）
%T          变量的类型
%%          字面上的百分号标志（无操作数）
```

更复杂更高级的格式化可以用text模板或html模板。

```
package main

import (
    "html/template"
    "os"
)

type Boy struct {
    Name  string
    Id    int
    Score float64
}

type Crowd struct{
    TotalCount int
    Items []Boy
}

const tmplet = `
<h1>{{.TotalCount}} students</h1>
<table>
{{range .Items}}
<tr style='text-align: left'>
<td>{{.Name}}</td>
<td>{{.Id}}</td>
<td>{{.Score | printf "%.2f"}}</td>
<td>{{.Score | grade}}</td>
</tr>
{{end}}
</table>`

func grade(score float64) string {
    if score > 90 {
        return "excellent"
    } else if score > 80 {
        return "good"
    } else if score > 70 {
        return "general"
    } else if score > 60 {
        return "pass"
    } else {
        return "fail"
    }
}

func main() {
    boys := []Boy{
        Boy{"Orisun", 1, 56.4324},
        Boy{"Tom", 2, 95.009},
        Boy{"Jim", 3, 87.0},
    }
    crowds := Crowd{len(boys), boys}

    report, err := template.New("report").Funcs(template.FuncMap{"grade": grade}).Parse(tmplet)
    if err != nil {
        panic(err)
    }

    err = report.Execute(os.Stdout, crowds)
    if err != nil {
        panic(err)
    }
}
```

输出：

<h1>3 students</h1>
<table>

<tr style='text-align: left'>
<td>Orisun</td>
<td>1</td>
<td>56.43</td>
<td>fail</td>
</tr>

<tr style='text-align: left'>
<td>Tom</td>
<td>2</td>
<td>95.01</td>
<td>excellent</td>
</tr>

<tr style='text-align: left'>
<td>Jim</td>
<td>3</td>
<td>87.00</td>
<td>good</td>
</tr>

</table>

```
package main

import (
    "text/template"
    "os"
)

type Boy struct {
    Name  string
    Id    int
    Score float64
}

type Crowd struct{
    TotalCount int
    Items []Boy
}

const tmpl = `{{.TotalCount}} students:
{{range .Items}}
----------------------
Name:   {{.Name}}
ID:        {{.Id}}
Score:    {{.Score | printf "%.2f"}}
Grade:    {{.Score | grade}}
{{end}}`

func grade(score float64) string {
    if score > 90 {
        return "excellent"
    } else if score > 80 {
        return "good"
    } else if score > 70 {
        return "general"
    } else if score > 60 {
        return "pass"
    } else {
        return "fail"
    }
}

func main() {
    boys := []Boy{
        Boy{"Orisun", 1, 56.4324},
        Boy{"Tom", 2, 95.009},
        Boy{"Jim", 3, 87.0},
    }
    crowds:=Crowd{len(boys),boys}

    report, err := template.New("report").Funcs(template.FuncMap{"grade": grade}).Parse(tmpl)
    if err != nil {
        panic(err)
    }

    err = report.Execute(os.Stdout, crowds)
    if err != nil {
        panic(err)
    }

}
```

输出：

3 students:

----------------------
Name:   Orisun
ID:		1
Score:	56.43
Grade:	fail

----------------------

Name:   Tom
ID:		2
Score:	95.01
Grade:	excellent

----------------------
Name:   Jim
ID:		3
Score:	87.00
Grade:	good

##  方法接收器

```
package main

import "fmt"

type Man struct{
    Age int
}

//method的receiver是非指针类型时，会产生一次拷贝
func (this Man)ChangeAge1()  {
    this.Age*=2
    fmt.Println(this.Age)
}

//method的receiver是指针类型时，传的是内存地址，会修改原始对象的成员属性
func (this *Man)ChangeAge2()  {
    this.Age*=2
    fmt.Println(this.Age)
}

func main()  {
    inst:=Man{18}
    inst.ChangeAge1()
    fmt.Println(inst.Age)
    fmt.Println("**************")
    //不管你的method的receiver是指针类型还是非指针类型，都是可以通过指针/非指针类型进行调用的，编译器会帮你做类型转换
    inst.ChangeAge2()
    fmt.Println(inst.Age)
}
```

输出：

36
18
**************
36
36

## BitSet

```
package main

import "fmt"

type BitSet struct {
    words []uint64
}

// Has reports whether the set contains the non-negative value x.
func (s *BitSet) Has(x int) bool {
    word, bit := x/64, uint(x%64)
    return word < len(s.words) && s.words[word]&(1<<bit) != 0
}

// Add adds the non-negative value x to the set.
func (s *BitSet) Add(x int) {
    word, bit := x/64, uint(x%64)
    for word >= len(s.words) {
        s.words = append(s.words, 0)
    }
    s.words[word] |= 1 << bit
}

func main()  {
    bitSet:=BitSet{}
    bitSet.Add(54334523)
    bitSet.Add(79878577)
    bitSet.Add(47989689)
    fmt.Println(bitSet.Has(6783729038))//false
    fmt.Println(bitSet.Has(47989689))//true
}
```

##  快速排序

```
package main

import (
    "fmt"
    "math/rand"
)

func swap(array []int, i int, j int) {
    tmp := array[j]
    array[j] = array[i]
    array[i] = tmp
}

func partition(array []int, left int, right int) int {
    pivot := array[left]
    i := left + 1
    j := right
    for {
        for ; j > i; j-- {
            if array[j] < pivot {
                break
            }
        }
        for ; i < j; i++ {
            if array[i] > pivot {
                break
            }
        }
        if i == j {
            if array[i] < pivot {
                swap(array, i, left)
                return i

            }else{
                return left
            }
        } else {
            swap(array, i, j)
        }
    }
}

func sort(array []int, left int, right int) {
    if left >= right {
        return
    }
    mid := partition(array, left, right)
    sort(array, left, mid-1)
    sort(array, mid+1, right)
}

func QuickSort(array []int) {
    sort(array, 0, len(array)-1)
}

func main() {
    array := []int{6, 1, 2, 7, 9, 6, 4, 5, 10, 8}
    QuickSort(array)
    for _, ele := range array {
        fmt.Printf("%d\t", ele)
    }
    fmt.Println()

    LEN := 10000
    array = make([]int, LEN)
    for i := 0; i < LEN; i++ {
        array[i] = rand.Intn(LEN)
    }
    QuickSort(array)
    success:=true
    for i := 1; i < LEN; i++ {
        if array[i] < array[i-1] {
            success=false
            break
        }
    }
    fmt.Printf("success %t\n",success)
}
```














