# 【Scala】泛函数 - zkq_1986的博客 - CSDN博客





2017年12月20日 12:41:24[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：271







def findFirstA[A](arr: Array[A],target:
A)(equ: (A, A) => Boolean): Int = {

        def loop(idx: Int): Int = idx match {

          case l if (l >= arr.length) => -1    //indicate not found

          case i if (equ(arr(i),target)) => idx

          case _ => loop(idx + 1)

        }

        loop(0)

}                                         



findFirstA[Int](Array(2,4,3,9,0),3)((x,y) => x == y)              //> res57: Int = 2

findFirstA[String](Array("Hello","My","World"),"My")((x,y) => x == y)        //> res58: Int = 1


