# IEnumerable 我的一个很费事的实现－》画蛇添足 - weixin_33985507的博客 - CSDN博客
2007年05月08日 17:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)myClassCollection
 1![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)publicclass myClassCollection : IEnumerable<MyClass>
 2![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
 3![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[Filed]#region [Filed]
 4![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 5![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)private IList<MyClass> myClassItems =new List<MyClass>();
 6![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 7![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
 8![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 9![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[Constructor]#region [Constructor]
10![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
11![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public myClassCollection()
12![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
13![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for (int i =0; i <=10; i++)
14![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
15![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                MyClass my =new MyClass();
16![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                my.Name ="Nmae"+new Random().Next(100).ToString();
17![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                my.Age =new Random().Next(100);
18![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                myClassItems.Add(my);
19![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
20![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
21![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
22![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
23![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
24![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[property]#region [property]
25![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
26![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public IList<MyClass> MyCollection
27![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
28![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
29![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
30![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return myClassItems;
31![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
32![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)set
33![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
34![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                myClassItems = value;
35![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
36![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
37![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
38![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
39![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
40![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[index]#region [index]
41![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
42![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public MyClass this[int index]
43![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
44![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
45![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
46![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return (MyClass)myClassItems[index];
47![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
48![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
49![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
50![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
51![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
52![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IEnumerable 成员#region IEnumerable<MyClass> 成员
53![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
54![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public IEnumerator<MyClass> GetEnumerator()
55![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
56![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnnew myEnumerator(this);
57![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
58![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
59![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
60![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
61![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IEnumerable 成员#region IEnumerable 成员
62![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
63![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
64![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
65![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnnew myEnumerator(this);
66![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
67![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
68![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
69![ExpandedBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)myEnumerator
 1![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)publicclass myEnumerator : IEnumerator<MyClass>
 2![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
 3![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[Filed]#region [Filed]
 4![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 5![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)private myClassCollection myCollection =new myClassCollection();
 6![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateint currentIndex =-1;
 7![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
 8![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 9![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[Constructor]#region [Constructor]
10![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
11![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public myEnumerator(myClassCollection coll)
12![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
13![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            myCollection = coll;
14![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
15![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
16![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
17![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
18![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[property]#region [property]
19![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
20![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public myClassCollection MyCollection
21![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
22![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
23![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
24![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return myCollection;
25![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
26![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)set
27![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
28![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                myCollection = value;
29![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
30![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
31![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
32![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicint CurrentIndex
33![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
34![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
35![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
36![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return currentIndex;
37![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
38![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)set
39![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
40![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                currentIndex = value;
41![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
42![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
43![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
44![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
45![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
46![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IEnumerator 成员#region IEnumerator<MyClass> 成员
47![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
48![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public MyClass Current
49![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
50![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
51![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
52![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return (MyClass)MyCollection[currentIndex];
53![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
54![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
55![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
56![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
57![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
58![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IDisposable 成员#region IDisposable 成员
59![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
60![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicvoid Dispose()
61![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
62![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
63![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
64![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
65![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
66![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)IEnumerator 成员#region IEnumerator 成员
67![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
68![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)object System.Collections.IEnumerator.Current
69![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
70![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)get![dot.gif](https://www.cnblogs.com/Images/dot.gif){ return MyCollection[currentIndex]; }
71![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
72![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
73![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicbool MoveNext()
74![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
75![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if (currentIndex < MyCollection.MyCollection.Count-1)
76![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
77![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                currentIndex++;
78![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returntrue;
79![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
80![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
81![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returnfalse;
82![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
83![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
84![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicvoid Reset()
85![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
86![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            currentIndex =0;
87![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
88![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
89![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
90![ExpandedBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)MyClass
 1![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)publicclass MyClass
 2![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
 3![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[Constructor]#region [Constructor]
 4![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
 5![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 6![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[Filed]#region [Filed]
 7![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 8![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatestring _name;
 9![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
10![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privateint _age;
11![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
12![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
13![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
14![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)[property]#region [property]
15![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
16![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicstring Name
17![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
18![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
19![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
20![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return _name;
21![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
22![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)set
23![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
24![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                _name = value;
25![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
26![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
27![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
28![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicint Age
29![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
30![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)get
31![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
32![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return _age;
33![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
34![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)set
35![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
36![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                _age = value;
37![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
38![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
39![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
40![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)#endregion
41![ExpandedBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)Client
 1![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)public partial class Form1 : Form
 2![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
 3![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)public Form1()
 4![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
 5![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            InitializeComponent();
 6![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 7![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            Thread t =new Thread(new ThreadStart(ChangeLabel));
 8![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            t.Start();
 9![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
10![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
11![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoid ChangeLabel()
12![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
13![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)foreach (MyClass c innew myClassCollection())
14![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
15![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                SetLabelText(c);
16![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                Thread.Sleep(1000);
17![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
18![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
19![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
20![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatevoid SetLabelText(MyClass myClass)
21![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
22![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// label.Text = number.ToString();
23![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// Do NOT do this, as we are on a different thread.
24![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
25![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// Check if we need to call BeginInvoke.
26![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if (this.InvokeRequired)
27![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
28![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// Pass the same function to BeginInvoke,
29![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// but the call would come on the correct
30![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)// thread and InvokeRequired will be false.
31![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.BeginInvoke(new SetLabelTextDelegate(SetLabelText),
32![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)newobject[] ![dot.gif](https://www.cnblogs.com/Images/dot.gif){ myClass });
33![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
34![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
35![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
36![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
37![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            label1.Text = myClass.Name;
38![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
39![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
40![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)privatedelegatevoid SetLabelTextDelegate(MyClass myClass);
41![ExpandedBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
