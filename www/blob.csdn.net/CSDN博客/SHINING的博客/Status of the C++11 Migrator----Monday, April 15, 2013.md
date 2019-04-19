# Status of the C++11 Migrator----Monday, April 15, 2013 - SHINING的博客 - CSDN博客
2013年12月08日 16:34:59[snsn1984](https://me.csdn.net/snsn1984)阅读数：1118标签：[LLVM																[c++11																[clang																[Migrator																[transform](https://so.csdn.net/so/search/s.do?q=transform&t=blog)](https://so.csdn.net/so/search/s.do?q=Migrator&t=blog)](https://so.csdn.net/so/search/s.do?q=clang&t=blog)](https://so.csdn.net/so/search/s.do?q=c++11&t=blog)](https://so.csdn.net/so/search/s.do?q=LLVM&t=blog)
个人分类：[编译技术及LLVM																[C++](https://blog.csdn.net/snsn1984/article/category/488621)](https://blog.csdn.net/snsn1984/article/category/1249418)
Since the [design document](http://blog.csdn.net/snsn1984/article/details/17203591) for cpp11-migrate, the C++11 migrator tool, was first proposed in early
 December 2012 development has been making steady progress. In this article I'll talk about what's been implemented in cpp11-migrate so far, what's coming up, and how you can get involved.
The purpose of the C++11 Migrator is to do source-to-source translation to migrate existing C++ code to use C++11 features to enhance maintainability, readability, runtime performance, and compile-time performance. Development is still early and transforms
 fall mostly into the first two categories. The migrator is based on Clang's [LibTooling](http://clang.llvm.org/docs/LibTooling.html) and the [AST
 Matching library](http://clang.llvm.org/docs/LibASTMatchers.html).
Most of the development so far has been carried out by a small core group at Intel. Our focus so far has been to set up project infrastructure and testing, implement a few basic transforms, and make sure those transforms work well. Our aim is to make this tool
 useful to the community so we're always listening for transform ideas and feedback.
## How to Get cpp11-migrate
cpp11-migrate is located in the Extra Clang Tools repository. To build cpp11-migrate, you will need the LLVM and Clang sources as well. Follow the directions in Clang's [Getting
 Started instructions](http://clang.llvm.org/get_started.html) making sure to perform the optional step of checking out the Extra Clang Tools repository. Once checked out into the correct directory the build system, after a reconfiguration, will automatically include the extra Clang tools as part
 of the next full build. If you're using the CMake build system, you can build just cpp11-migrate with the cpp11-migratetarget. The check-clang-tools target
 provided by CMake will run all regression tests for extra Clang tools, including cpp11-migrate.
## The Transforms So Far
The C++11 Migrator currently supports four features of C++11:
- Range-based for loops
- The nullptrliteral for null pointers
- The autotype specifier
- overridevirtual specifier
The range-based for-loop transform once existed as a stand-alone tool called loop-convert contributed by Sam Panzer. When
 development on more transforms started, the idea became to pull all transforms under the jurisdiction of a single tool and cpp11-migrate was born. The
 range-based for-loop transform replaces for-loops used in one of the following three common situations:
- Loops over containers using iterators
std::vector<**int**> myVec;**for**(std::vector<**int**>::iterator I = myVec.begin(),
                                E = myVec.end();
     I != E;++I)
  llvm::outs()<<*I;
⇒
std::vector<**int**> myVec;**for**(**auto**& elem : myVec)
  llvm::outs()<< elem;
- Loops over statically allocated arrays
**int** arr[]={1,2,3,4,5};**for**(**int** i =0; i <5;++i)
  llvm::outs()<< arr[i];
⇒
**int** arr[]={1,2,3,4,5};**for**(**auto**& elem : arr)
  llvm::outs()<< elem;
- Loops over array-like containers using operator[] or at().
std::vector<**int**> myVec;**for**(**int** i =0; i < myVec.size();++i)
  llvm::outs()<< v[i];
⇒
std::vector<**int**> myVec;**for**(**auto**& elem : myVec)
  llvm::outs()<< elem;
The nullptr transform uses the new nullptr literal where pointers are being initialized with or assigned a null value. In cases where an explicit cast is used,
 the explicit cast is left behind to avoid introducing ambiguities into the code.
**void** foo(**int***arg);**void** foo(**float***arg);**int***IntPtr =0;**float***FloatPtr =NULL;
foo(**static_cast**<**int***>(0));
⇒
**void** foo(**int***arg);**void** foo(**float***arg);**int***IntPtr = nullptr;**float***FloatPtr = nullptr;
foo(**static_cast**<**int***>(nullptr));
The auto type specifier transform replaces the type specifier for variable declarations with the new auto keyword. In general, such a replacement can be done
 whenever the type of the variable declaration matches the type of its initializer. However, the transform targets only a few specific useful situations with readability and maintainability in mind:
- When the variable is an iterator for an STL container.
std::vector<std::pair<**int**,std::string>>::iterator NameAgeI = People.begin();**for**(std::vector<MyType>::iterator I = Container.begin(),
                                   E = Container.end;
     I != E;++I){// ...}
⇒
**auto** NameAgeI = People.begin();**for**(**auto** I = Container.begin(),
                                  E = Container.end;
     I != E;++I){// ...}
- When the initializer is an allocation using the new operator.
MyType *VarPtr =**new** MyType();
MyType ***const** VarCPtr =**new** MyType();
⇒
**auto** VarPtr =**new** MyType();**auto****const** VarCPtr =**new** MyType();
Support for a third situation is in development: creating objects with factory functions.
MyType *FooPtr = makeObject<MyType>(/*...*/);
MyType *BarPtr = MyType::create(/*...*/);
⇒
**auto** FooPtr = makeObject<MyType>(/*...*/);**auto** BarPtr = MyType::create(/*...*/);
In each situation, the deduced type for the declared variable should be obvious to the reader. Iterators for standard containers are created by functions with specific names and are used in specific situations. For factory functions and operator new, the
 type is spelled out in the initializer so repeating it in the variable declaration is not necessary.
The override virtual specifier transform, contributed by Philip Dunstan, is the migrator's fourth transform and the first to be contributed from outside the core group at Intel. This
 transform detects virtual member functions in derived classes that override member functions from parent classes and adds the override virtual specifier to the function.
**class** Parent {**public**:**virtual****int** getNumChildren();};**class** Child {**public**:**virtual****int** getNumChildren();};
⇒
**class** Parent {**public**:**virtual****int** getNumChildren();};**class** Child {**public**:**virtual****int** getNumChildren()**override**;};
More details on these transforms, what they can and can't do, how to adjust their behaviour, and known limitations can be found in the[cpp11-migrate
 User's Manual](http://ma.leiseshoubei.info/browse.php?u=ac0b85664a23001c0Oi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvY3BwMTEtbWlncmF0ZS5odG1s&b=13).
## Testing on Real Projects
What better way to test the C++11 Migrator than to run it on entire real projects? We've set up a continuous integration server to build and run cpp11-migrate on two projects so far and have plans for at least three more. For each project, the goal is
 to build the transformed code and run that project's test suite to ensure semantics haven't changed.
**Implemented: **
- [LLVM 3.1](http://ma.leiseshoubei.info/browse.php?u=ac0b85664a23001c0Oi8vd3d3Lmxsdm0ub3JnLw%3D%3D&b=13)
- [ITK 4.3.1](http://ma.leiseshoubei.info/browse.php?u=ac0b85664a23001c0Oi8vd3d3Lml0ay5vcmcv&b=13)
**Planned: **
- [LLDB](http://ma.leiseshoubei.info/browse.php?u=ac0b85664a23001c0Oi8vbGxkYi5sbHZtLm9yZy8%3D&b=13)
- [OpenCV](http://ma.leiseshoubei.info/browse.php?u=ac0b85664a23001c0Oi8vb3BlbmN2Lm9yZy8%3D&b=13)
- [Poco](http://ma.leiseshoubei.info/browse.php?u=ac0b85664a23001c0Oi8vcG9jb3Byb2plY3Qub3JnLw%3D%3D&b=13)
Running the migrator on real code has been enormously helpful for finding bugs. Real code from varying projects often reveals code expressions not accounted for in the development and unit testing of the transforms. Every time a bug found from transforming
 these projects gets fixed, new test cases are added to the regression test suite and the migrator becomes more robust.
## Future Work
Fixing bugs found by migrating real code is of high priority right now since we want a good user experience for as many people as we can as soon as possible. Adding more transforms is another priority and those transforms with the most interest from the
 community will come first. Currently at the top of the list are:
- Use the standard library instead of TR1
- Replace use of the deprecated auto_ptr class.
In addition to fixing bugs and adding transforms, there are also more general improvements to consider. One such improvement we're making progress on is to remove the restriction that only source files are transformed and not any of the headers they include.
 The restriction has been in place until now because the migrator needs to know which headers are safe to transform. System headers and third-party library headers clearly shouldn't be touched.
## Get Involved!
If you want to get involved, the first thing you can do is try out cpp11-migrate on your code. Bugs can be logged with[LLVM's bug tracker](http://llvm.org/bugs/) under the product clang-tools-extra.
 Send an email to the [Clang Developer's Mailing List](http://lists.cs.uiuc.edu/mailman/listinfo/cfe-dev) if you need help or would like to get more involved. We look
 forward to hearing from you!
Posted by Edwin Vaneat 5:53
 AM![](http://ma.leiseshoubei.info/browse.php?u=ac0b85664a23001c0Oi8vaW1nMi5ibG9nYmxvZy5jb20vaW1nL2ljb24xOF9lZGl0X2FsbGJrZy5naWY%3D&b=13)
Labels: C++, Clang
Location: Waterloo, ON, Canada
