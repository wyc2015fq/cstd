# hw6存在的两个问题 - weixin_33985507的博客 - CSDN博客
2017年10月27日 21:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# 致命问题
这是一个有关**状态转换**的问题。
本作业（俄罗斯方块）存在的状态转换。
这是有关暂停的一个函数。涉及到了状态(@running)的转换。
```
def pause
    if @running
      @running = false
      @timer.stop
    else
      @running = true
      self.run_game
    end
  end
```
另外一个状态转换，如下所示
**Part I**
```
def next_cheat_piece
    @current_block = MyPiece.next_cheat_piece(self)
    @current_pos = nil
    @cheat = false
end
```
**Part II**
```
def next_piece
    @current_block = MyPiece.next_piece(self)
    @current_pos = nil
    @cheat = true
end
```
从表面上来看，代码好像没错，甚至有一些对称美。但是！！！，当next_piece并不会让@cheat = true，而是通过c按键(alter_cheat_condition函数来改变的)，所以对于Part II来说，正确的代码是没有@cheat=true这一行的。
# 严重问题
在hw6provided.rb中，也就是class MyPieces中的run方法和drop_all_the_way方法都用到了next_piece方法。如果采用上面的Part I和Part II的写法来写的话，就需要在run和drop_all_the_way里面修改代码，这无疑造成了代码的冗余性（不简洁）。而对于@current_block来说，这里需要修改的地方只有一处，所以可采用新建函数(MyPiece.cheat_next_piece)的方法。
所以我们需要把Part I和II合并在一起，即：
```
def next_piece
    if @cheat == false
        @current_block = MyPiece.next_piece(self)
    else
        @current_block = MyPiece.cheat_next_piece(self)
        @cheat = false
    end
    @current_pos = nil #common same sentence
end
```
## 2017.10.28思考
**状态转移**其实在生活中也很常见。就拿穿衣服来说，通过看衣服的logo，确定是否要把衣服转过来。如果是正确的，就不需要转，直接穿上。反之转一下穿上即可。
