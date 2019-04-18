# Scala语言学习笔记（4） - zwvista - 博客园

## [Scala语言学习笔记（4）](https://www.cnblogs.com/zwvista/p/8078411.html)

### 高阶函数

```
// 高阶函数 map
val salaries = Seq(20000, 70000, 40000)
val doubleSalary = (x: Int) => x * 2
val newSalaries = salaries.map(doubleSalary) // List(40000, 140000, 80000)
val newSalaries = salaries.map(x => x * 2) // List(40000, 140000, 80000)
val newSalaries = salaries.map(_ * 2)
// 将方法强制转换为函数
case class WeeklyWeatherForecast(temperatures: Seq[Double]) {
 private def convertCtoF(temp: Double) = temp * 1.8 + 32
  def forecastInFahrenheit: Double = temperatures.map(convertCtoF) // <-- passing the method convertCtoF
}
// 接收函数参数的函数
object SalaryRaiser {
 private def promotion(salaries: List[Double], promotionFunction: Double => Double): List[Double] =
    salaries.map(promotionFunction)
  def smallPromotion(salaries: List[Double]): List[Double] =
    promotion(salaries, salary => salary * 1.1)
  def bigPromotion(salaries: List[Double]): List[Double] =
    promotion(salaries, salary => salary * math.log(salary))
  def hugePromotion(salaries: List[Double]): List[Double] =
    promotion(salaries, salary => salary * salary)
}
// 返回函数的函数
def urlBuilder(ssl: Boolean, domainName: String): (String, String) => String = {
  val schema = if (ssl) "https://" else "http://"
  (endpoint: String, query: String) => s"$schema$domainName/$endpoint?$query"
}
val domainName = "www.example.com"
def getURL = urlBuilder(ssl=true, domainName)
val endpoint = "users"
val query = "id=1"
val url = getURL(endpoint, query) // "https://www.example.com/users?id=1": String
```

### 嵌套方法

```
def factorial(x: Int): Int = {
    def fact(x: Int, accumulator: Int): Int = {
      if (x <= 1) accumulator
      else fact(x - 1, x * accumulator)
    }  
    fact(x, 1)
}
println("Factorial of 2: " + factorial(2))
println("Factorial of 3: " + factorial(3))
// Factorial of 2: 2
// Factorial of 3: 6
```

### 柯里化

```
object CurryTest extends App {
  def filter(xs: List[Int], p: Int => Boolean): List[Int] =
    if (xs.isEmpty) xs
    else if (p(xs.head)) xs.head :: filter(xs.tail, p)
    else filter(xs.tail, p)
  def modN(n: Int)(x: Int) = ((x % n) == 0)
  val nums = List(1, 2, 3, 4, 5, 6, 7, 8)
  println(filter(nums, modN(2)))
  println(filter(nums, modN(3)))
}
// List(2,4,6,8)
// List(3,6)
```


