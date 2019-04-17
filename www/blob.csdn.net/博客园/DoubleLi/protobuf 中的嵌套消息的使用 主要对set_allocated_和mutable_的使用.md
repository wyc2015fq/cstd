# protobuf 中的嵌套消息的使用 主要对set_allocated_和mutable_的使用 - DoubleLi - 博客园






protobuf的简单的使用，不过还留下了一个问题，那就是之前主要介绍的都是对简单数据的赋值，简单数据直接采用set_xx()即可，但是如果不是简单变量而是自定义的复合类型变量，就没有简单的set函数调用了，下面看一个简单的例子。

在网络游戏中，游戏玩家之间的同步是一个最基本的功能，而同步是通过对坐标的广播进行的，因此我们假设一个简单的模型，当一个玩家的位置发生变化时，将玩家的新位置发给地图内所有玩家，根据这个情况写出以下proto文件。





**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- message PlayerPos  
- {     
-     required  uint32  playerID = 1;   
-     required  float   posX = 2 ;      
-     required  float   posY = 3 ;  
- };  
- 
- file  vector.protomessage  vector3D  
- {  
-     required float x = 1;  
-     required float y = 2;  
-     required float z = 3;  
- };  



这样就有一个问题，现在的游戏都是3D游戏，因此需要xyz来表示位置，还需要另一组xyz来表示朝向，如果用简单变量的话就会显的很乱，而且无论是位置还是朝向其实都是一组xyz，因此可以将xyz抽出来成为一个复合数据类型，单独放在一个文件中。这样就构成以下文件。







**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- file  Player.protoimport "vector.proto";  
- message PlayerPos   
- {  
-     required uint32 playerID = 1;  
-     required vector3D  pos = 2;  
- };  



编译的时候先编译vector文件，采用import时需要注意路径，本例中两文件在同一目录下。







**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- protoc --cpp_out=.  vector.proto  Player.proto  




proto对应的文件已经生成了，但是该怎么赋值呢，查API查了半天有点不知所以，干脆来看生成的类文件的源代码吧





**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- // required uint32 playerID = 1;    
- inline bool has_playerid() const;    
- inline void clear_playerid();    
- static const int kPlayerIDFieldNumber = 1;   
- inline ::google::protobuf::uint32 playerid() const;    
- inline void set_playerid(::google::protobuf::uint32 value);    
- // required .vector3D pos = 2;    
- inline bool has_pos() const;    
- inline void clear_pos();    
- static const int kPosFieldNumber = 2;    
- inline const ::vector3D& pos() const;    
- inline ::vector3D* mutable_pos();    
- inline ::vector3D* release_pos();    
- inline void set_allocated_pos(::vector3D* pos);  




上面列出了生成的部分源代码，主要是PlayerPos的操作变量的函数，第一个playID很简单，可以看到直接使用set_playerid ( ) 即可，但是对于嵌套的pos 发现没有对应的set_pos方法，不过发现了一个set_allocated_pos() 函数，这个函数也是set开头的，看看这个函数是干嘛的。



**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- inline void PlayerPos::set_allocated_pos(::vector3D* pos)   
- {    
- delete pos_;    
-     pos_ = pos;    
- if (pos)   
-     {      
-         set_has_pos();    
-     }   
- else {     
-          clear_has_pos();    
-     }  
- }  



看上去可以赋值，直接调用set_allocated_pos() 进行赋值看一看



**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- PlayerPos player;  
- vector3D  tmp;  
- tmp.x = 1;  
- tmp.y = 2;  
- tmp.z = 3;  
- player.set_allocated_pos(&tmp)  


编译没问题，但是运行时出现错误，而且是很奇怪的错误，仔细了查看一下PlayerPos的源码，发现一个问题





**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- ::vector3D* pos_;  ::google::protobuf::uint32 playerid_;  



上面是PlayerPos中变量的保存形式，发现pos是作为一个指针存储的，如果按照之前的赋值 tmp 是一个局部变量，函数返回时局部变量自动销毁，而pos_保存的仍然是已被销毁的tmp的位置，因此会出错，如果采用new的话就可以解决这个问题，即赋值方法如下:







**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- PlayerPos player;vector3D  *tmp = new Vector3D;  
- tmp->x = 1;  
- tmp->y = 2;  
- tmp->z = 3;  
- player.set_allocated_pos(tmp)  




这样即可，编译运行都没有问题。 
如此之外，还有一种赋值方法，就是调用mutable_pos()





**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- inline ::vector3D* PlayerPos::mutable_pos()   
- {    
-     set_has_pos();    
- if (pos_ == NULL)   
-         pos_ = new ::vector3D;    
- return pos_;  
- }  



mutable_pos () 中自己new出了一个vector3D 对象，而vector3D中又实现了赋值的重载，因此可以这样解决:







**[cpp]**[view plain](http://blog.csdn.net/xiaxiazls/article/details/50118161)[copy](http://blog.csdn.net/xiaxiazls/article/details/50118161)



- PlayerPos player;  
- vector3D  *tmp = player.mutable_pos();  
- tmp->x = 1;  
- tmp->y = 2;  
- tmp->z = 3;  










