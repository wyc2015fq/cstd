# dynamic_cast操作数必须是多态类型吗 - 电影旅行敲代码 - CSDN博客
2015年11月03日 21:13:25[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2140
dynamic_cast<>是C++运行时的一个类型转换运算符，通常用于自定义类型层次中的向下转型（downcasts），使用形式如下所示:
- dynamic_cast<type*>(e)
- dynamic_cast<type&>(e)
- dynamic_cast<type&&>(e)
根据Primer中的解释，type必须是一个类类型，并且通常情况下该类型应该含有虚函数，即该类型是多态类型。也就是说有少数情况是不需要type为多态类型，这种情况就是派生类指针向基类指针的类型转换**dynamic_cast<Base*>(Derived*)**，这种转换不需要用到运行时信息RTTI，在编译时期就可以通过操作数的类型来判断转型是否合法。
编译器Clang源码，SemaCast.cpp中CheckDynamicCast()函数对dynamic_cast<>的使用语义进行了很好的体现。
dynamic_cast(srctype)中type类型必须是指针或者引用。
```
// C++ 5.2.7p1: T shall be a pointer or reference to a complete class type,
//   or "pointer to cv void".
   QualType DestPointee;
   const PointerType *DestPointer = DestType->getAs<PointerType>();
   const ReferenceType *DestReference = nullptr;
   if (DestPointer) {
     DestPointee = DestPointer->getPointeeType();
   } else if ((DestReference = DestType->getAs<ReferenceType>())) {
     DestPointee = DestReference->getPointeeType();
   } else {
     Self.Diag(OpRange.getBegin(), diag::err_bad_dynamic_cast_not_ref_or_ptr)
       << this->DestType << DestRange;
     SrcExpr = ExprError();
     return;
   }
```
其中590行和592行要求DestType必须是指针或者引用类型。
另外，dynamic_cast(srctype)中srctype的类型要求。
```php
// C++ 5.2.7p5
   // Upcasts are resolved statically.
   if (DestRecord && Self.IsDerivedFrom(SrcPointee, DestPointee)) {
     if (Self.CheckDerivedToBaseConversion(SrcPointee, DestPointee,
                                            OpRange.getBegin(), OpRange, 
                                            &BasePath)) {
       SrcExpr = ExprError();
       return;
     }
     Kind = CK_DerivedToBase;
     return;
   }
```
如果是向上转型（upcasts）的话，转换就会静态决议，也就是在编译阶段就能确定，不会耗费运行时的信息。
否则就需要操作数类型是多态类型，如下代码所示：
```
// C++ 5.2.7: Otherwise, v shall be [polymorphic].
   const RecordDecl *SrcDecl = SrcRecord->getDecl()->getDefinition();
   assert(SrcDecl && "Definition missing");
   if (!cast<CXXRecordDecl>(SrcDecl)->isPolymorphic()) {
     Self.Diag(OpRange.getBegin(), diag::err_bad_dynamic_cast_not_polymorphic)
       << SrcPointee.getUnqualifiedType() << SrcExpr.get()->getSourceRange();
     SrcExpr = ExprError();
   }
   // dynamic_cast is not available with -fno-rtti.
   // As an exception, dynamic_cast to void* is available because it doesnt
   // use RTTI
   if (!Self.getLangOpts().RTTI && !DestPointee->isVoidType()) {
     Self.Diag(OpRange.getBegin(), diag::err_no_dynamic_cast_with_fno_rtti);
     SrcExpr = ExprError();
     return;
   }
   // Done. Everything else is run-time checks.
   Kind = CK_Dynamic;
```
由于dynamic_cast<>需要RTTI信息，如果编译时关闭了RTTI的情况下使用dynamic_cast<>则会报错，但是一种情况除外，就是向void*的转换，因为不需要运行时信息。
dynamic_cast<>会在运行时执行类型安全的检查，自然会影响运行时的效率，该转换需要通过两次指针解引用获得type_info，然后再判断转换是否合法。
