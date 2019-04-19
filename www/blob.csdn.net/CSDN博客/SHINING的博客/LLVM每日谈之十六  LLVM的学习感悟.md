# LLVM每日谈之十六   LLVM的学习感悟 - SHINING的博客 - CSDN博客
2013年07月18日 10:30:57[snsn1984](https://me.csdn.net/snsn1984)阅读数：5116
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
这些总结并非我自己写的，而是摘自LLVM的版本比较老的文档中。因为老版本的文档已经鲜有人关注了，而这篇总结的非常好，到现在也很有用处，所以就把这部分内容贴出来了。这只是原文档的一部分。
原文档地址：http://llvm.org/releases/1.1/docs/Stacker.html
正文内容：
Lessons I Learned About LLVM
Everything's a Value!
Although I knew that LLVM uses a Single Static Assignment (SSA) format, it wasn't obvious to me how prevalent this idea was in LLVM until I reallystarted using it. Reading the
[Programmer's Manual](http://llvm.org/releases/1.1/docs/ProgrammersManual.html) and [Language Reference](http://llvm.org/releases/1.1/docs/LangRef.html),I noted that most of the important LLVM IR (Intermediate Representation) C++ classes were derived from the Value class. The full power of that simpledesign only became fully understood once I started constructing executableexpressions
 for Stacker.
This really makes your programming go faster. Think about compiling codefor the following C/C++ expression:
```
(a|b)*((x+1)/(y+1))
```
. Assumingthe values are on the stack in the order a, b, x, y, this could beexpressed in stacker as:
```
1 + SWAP 1 + / ROT2 OR *
```
.You could write a function using LLVM that computes this expression like this:
```
Value* expression(BasicBlock*bb, Value* a, Value* b, Value* x, Value* y ){    
 Instruction* tail = bb-<getTerminator();    
 ConstantSInt* one = ConstantSInt::get( Type::IntTy, 1);    
 BinaryOperator* or1 = 	BinaryOperator::create( Instruction::Or, a, b, "", tail );
 BinaryOperator* add1 = 	BinaryOperator::create( Instruction::Add, x, one, "", tail );
 BinaryOperator* add2 =	BinaryOperator::create( Instruction::Add, y, one, "", tail ); 
 BinaryOperator* div1 = 	BinaryOperator::create( Instruction::Div, add1, add2, "", tail);
 BinaryOperator* mult1 = 	BinaryOperator::create( Instruction::Mul, or1, div1, "", tail );    return mult1;}
```
"Okay, big deal," you say. It is a big deal. Here's why. Note that I didn'thave to tell this function which kinds of Values are being passed in. They could be`Instruction`s,
```
Constant
```
s, 
```
GlobalVariable
```
s, etc. Furthermore, if you specify Values that are incorrect for this sequence of operations, LLVM will either notice right away (at compilation time) or the LLVM Verifier will pick up the inconsistency when the compiler runs. In no case
 will you make a type error that gets passed through to the generated program. This
*really* helps you write a compiler that always generates correct code!
The second point is that we don't have to worry about branching, registers,stack variables, saving partial results, etc. The instructions we create
*are* the values we use. Note that all that was created in the abovecode is a Constant value and five operators.Each of the instructions
*is* the resulting value of that instruction. This saves a lot of time.
The lesson is this:
*SSA form is very powerful: there is no differencebetween a value and the instruction that created it.* This is fullyenforced by the LLVM IR. Use it to your best advantage.
Terminate Those Blocks!
I had to learn about terminating blocks the hard way: using the debugger to figure out what the LLVM verifier was trying to tell me and begging forhelp on the LLVMdev mailing list. I hope you
 avoid this experience.
Emblazon this rule in your mind:
- *All*`BasicBlock`s in your compiler
**must** be terminated with a terminating instruction (branch, return, etc.).
Terminating instructions are a semantic requirement of the LLVM IR. Thereis no facility for implicitly chaining together blocks placed into a functionin the order they occur. Indeed, in the general
 case, blocks will not beadded to the function in the order of execution because of the recursiveway compilers are written.
Furthermore, if you don't terminate your blocks, your compiler code will compile just fine. You won't find out about the problem until you're running the compiler and the module you just created
 fails on the LLVM Verifier.
Concrete Blocks
After a little initial fumbling around, I quickly caught on to how blocksshould be constructed. In general, here's what I learned:
- *Create your blocks early.* While writing your compiler, you will encounter several situations where you know apriori that you will need several
 blocks. For example, if-then-else, switch, while, and for statements in C/C++ all need multiple blocks for expression in LVVM. The rule is, create them early.
- *Terminate your blocks early.* This just reduces the chances that you forget to terminate your blocks which is required (go
here for more).
- *Use getTerminator() for instruction insertion.* I noticed early on that many of the constructors for the Instruction classes take an optional
```
insert_before
```
 argument. At first, I thought this was a mistake because clearly the normal mode of inserting instructions would be one at a time
*after* some other instruction, not
*before*. However, if you hold on to your terminating instruction (or use the handy dandy
```
getTerminator()
```
 method on a 
```
BasicBlock
```
), it can always be used as the 
```
insert_before
```
 argument to your instruction constructors. This causes the instruction to automatically be inserted in the RightPlace™ place, just before the terminating instruction. The nice thing about this design is that you can pass blocks around and
 insert new instructions into them without ever knowing what instructions came before. This makes for some very clean compiler design.
The foregoing is such an important principal, its worth making an idiom:
```
BasicBlock* bb = new BasicBlock();
bb-<getInstList().push_back( new Branch( ... ) );
new Instruction(..., bb-<getTerminator() );
```
To make this clear, consider the typical if-then-else statement(see StackerCompiler::handle_if() method). We can set this upin a single function using LLVM in the following way:
using namespace llvm;
BasicBlock*MyCompiler::handle_if( BasicBlock* bb, SetCondInst* condition ){
 // Create the blocks to contain code in the structure of if/then/else    
 BasicBlock* then = new BasicBlock();
 BasicBlock* else = new BasicBlock();
 BasicBlock* exit = new BasicBlock();    // Insert the branch instruction for the "if"    
bb-<getInstList().push_back( new BranchInst( then, else, condition ) );    // Set up the terminating instructions    then-<getInstList().push_back( new BranchInst( exit ) );    else-<getInstList().push_back( new BranchInst( exit ) );    // Fill in the then part .. details excised for brevity    this-<fill_in( then );    // Fill in the else part .. details excised for brevity    this-<fill_in( else );    // Return a block to the caller that can be filled in with the code    // that follows the if/then/else construct.    return exit;}
Presumably in the foregoing, the calls to the "fill_in" method would add the instructions for the "then" and "else" parts. They would use the third partof the idiom almost exclusively (inserting
 new instructions before the terminator). Furthermore, they could even recurse back to
```
handle_if
```
 should they encounter another if/then/else statement, and it will just work.
Note how cleanly this all works out. In particular, the push_back methods onthe
```
BasicBlock
```
's instruction list. These are lists of type 
```
Instruction
```
 which also happen to be 
```
Value
```
s. To create the "if" branch, we merely instantiate a 
```
BranchInst
```
 that takes as arguments the blocks to branch to and the condition to branch on. The blocksact like branch labels! This new
```
BranchInst
```
 terminatesthe 
```
BasicBlock
```
 provided as an argument. To give the caller a wayto keep inserting after calling
```
handle_if
```
, we create an "exit" blockwhich is returned to the caller. Note that the "exit" block is used as the terminator for both the "then" and the "else" blocks. This guarantees that nomatter what else "handle_if" or "fill_in" does, they end up at
 the "exit" block.
push_back Is Your Friend
One of the first things I noticed is the frequent use of the "push_back"method on the various lists. This is so common that it is worth mentioning.The "push_back" inserts a value into an STL list,
 vector, array, etc. at theend. The method might have also been named "insert_tail" or "append".Although I've used STL quite frequently, my use of push_back wasn't veryhigh in other programs. In LLVM, you'll use it all the time.
The Wily GetElementPtrInst
It took a little getting used to and several rounds of postings to the LLVMmailing list to wrap my head around this instruction correctly. Even though I hadread the Language Reference and Programmer's
 Manual a couple times each, I stillmissed a few *very* key points:
- GetElementPtrInst gives you back a Value for the last thing indexed.
- All global variables in LLVM are *pointers*.
- Pointers must also be dereferenced with the GetElementPtrInst instruction.
This means that when you look up an element in the global variable (assumingit's a struct or array), you
*must* deference the pointer first! For manythings, this leads to the idiom:
```
std::vector index_vector;
index_vector.push_back( ConstantSInt::get( Type::LongTy, 0 );// ... push other indices ...
GetElementPtrInst* gep = new GetElementPtrInst( ptr, index_vector );
```
For example, suppose we have a global variable whose type is [24 x int]. Thevariable itself represents a
*pointer* to that array. To subscript thearray, we need two indices, not just one. The first index (0) dereferences thepointer. The second index subscripts the array. If you're a "C" programmer,
 thiswill run against your grain because you'll naturally think of the global arrayvariable and the address of its first element as the same. That tripped me upfor a while until I realized that they really do differ .. by
*type*.Remember that LLVM is a strongly typed language itself. Everythinghas a type. The "type" of the global variable is [24 x int]*. That is, it'sa pointer to an array of 24 ints. When you
 dereference that global variable witha single (0) index, you now have a "[24 x int]" type. Althoughthe pointer value of the dereferenced global and the address of the zero'th elementin the array will be the same, they differ in their type. The zero'th element
 hastype "int" while the pointer value has type "[24 x int]".
Get this one aspect of LLVM right in your head, and you'll save yourselfa lot of compiler writing headaches down the road.
Getting Linkage Types Right
Linkage types in LLVM can be a little confusing, especially if your compilerwriting mind has affixed very hard concepts to particular words like "weak","external", "global", "linkonce", etc. LLVM
 does *not* use the precisedefinitions of, say, ELF or GCC, even though they share common terms. To be fair,the concepts are related and similar but not precisely the same. This can leadyou
 to think you know what a linkage type represents but in fact it is slightlydifferent. I recommend you read the
[Language Reference on this topic](http://llvm.org/releases/1.1/docs/LangRef.html#linkage) very carefully. Then, read it again.
Here are some handy tips that I discovered along the way:
- Uninitialized means external. That is, the symbol is declared in the current module and can be used by that module but it is not defined by that module.
- Setting an initializer changes a global's linkage type from whatever it was to a normal, defined global (not external). You'll need to call the setLinkage() method to reset it if you specify the initializer after the
 GlobalValue has been constructed. This is important for LinkOnce and Weak linkage types.
- Appending linkage can be used to keep track of compilation information at runtime. It could be used, for example, to build a full table of all the C++ virtual tables or hold the C++ RTTI data, or whatever. Appending
 linkage can only be applied to arrays. The arrays are concatenated together at link time.
Constants Are Easier Than That!
Constants in LLVM took a little getting used to until I discovered a few utilityfunctions in the LLVM IR that make things easier. Here's what I learned:
- Constants are Values like anything else and can be operands of instructions
- Integer constants, frequently needed, can be created using the static "get" methods of the ConstantInt, ConstantSInt, and ConstantUInt classes. The nice thing about these is that you can "get" any kind of integer quickly.
- There's a special method on Constant class which allows you to get the null constant for
*any* type. This is really handy for initializing large arrays or structures, etc.
这几条都让我感触非常深。但是给我感觉收获最大的还是第一条。等于让我对LLVM的整体认识一下子清晰了很多，对LLVM的理解跟原来马上就不在一个档次之上了。一切都是值！
强烈推荐认真读一读这篇感悟。目前以我对LLVM的理解，写不出这个层次的感悟，但是也会尝试去写一篇，这样可以加深自己的理解的同时，发现自己的不足之处。
