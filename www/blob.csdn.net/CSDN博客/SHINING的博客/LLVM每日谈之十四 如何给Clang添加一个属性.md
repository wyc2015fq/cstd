# LLVM每日谈之十四 如何给Clang添加一个属性 - SHINING的博客 - CSDN博客
2013年06月04日 15:33:06[snsn1984](https://me.csdn.net/snsn1984)阅读数：3808
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)

每次内容主要出自文档： “Clang” CFE Internals Manual
地址： http://clang.llvm.org/docs/InternalsManual.html
在这个文档中，以简明的的一个例子说明了如何按照这个方式在Clang中添加一个属性。讲的很清楚，我在这里不再累述，只是将例子中的部分代码和文档结合起来，一起展现给大家。
### [How to add an attribute](http://clang.llvm.org/docs/InternalsManual.html#id42)[¶](http://clang.llvm.org/docs/InternalsManual.html#how-to-add-an-attribute)
To add an attribute, you’ll have to add it to the list of attributes, add it tothe parsing phase, and look for it in the AST scan.[r124217](http://llvm.org/viewvc/llvm-project?view=rev&revision=124217) has a good example
 of adding a warning attribute.
(Beware that this hasn’t been reviewed/fixed by the people who designed theattributes system yet.)
#### [include/clang/Basic/Attr.td](http://clang.llvm.org/docs/InternalsManual.html#id43)
First, add your attribute to the [include/clang/Basic/Attr.td file](http://llvm.org/viewvc/llvm-project/cfe/trunk/include/clang/Basic/Attr.td?view=markup).
Each attribute gets a def inheriting fromAttr or one of itssubclasses.InheritableAttr
 means that the attribute also applies tosubsequent declarations of the same name.
Spellings lists the strings that can appear in__attribute__((here)) or[[here]].
 All such strings will be synonymous. If you want to allow the[[]] C++11 syntax, you have to define a list ofNamespaces, which willlet users
 write[[namespace::spelling]]. Using the empty string for anamespace will allow users to write just the spelling with no “::”.Attributes which
 g++-4.8 accepts should also have a CXX11<"gnu","spelling"> spelling.
Subjects restricts what kinds of AST node to which this attribute canappertain (roughly, attach).
Args names the arguments the attribute takes, in order. IfArgs is
[StringArgument<"Arg1">,
IntArgument<"Arg2">] then__attribute__((myattribute("Hello",3))) will be a valid use.
--- cfe/trunk/include/clang/Basic/Attr.td	2011/01/25 19:39:31	124216
+++ cfe/trunk/include/clang/Basic/Attr.td	2011/01/25 20:08:12	124217
@@ -232,6 +232,11 @@
   let Spellings = [];
 }
 
+def ForbidTemporaries : Attr {
+  let Spellings = ["forbid_temporaries"];
+  let Subjects = [CXXRecord];
+}
+
 def Format : InheritableAttr {
   let Spellings = ["format"];
   let Args = [StringArgument<"Type">, IntArgument<"FormatIdx">,
#### [Boilerplate](http://clang.llvm.org/docs/InternalsManual.html#id44)
Write a new HandleYourAttr() function in[lib/Sema/SemaDeclAttr.cpp](http://llvm.org/viewvc/llvm-project/cfe/trunk/lib/Sema/SemaDeclAttr.cpp?view=markup),and
 add a case to the switch in ProcessNonInheritableDeclAttr() or
ProcessInheritableDeclAttr() forwarding to it.
If your attribute causes extra warnings to fire, define a DiagGroup in [include/clang/Basic/DiagnosticGroups.td](http://llvm.org/viewvc/llvm-project/cfe/trunk/include/clang/Basic/DiagnosticGroups.td?view=markup) named after the attribute’s Spelling with “_”s replaced by “-“s. If you’reonly defining one diagnostic, you can skipDiagnosticGroups.td and useInGroup<DiagGroup<"your-attribute">>
 directly in[DiagnosticSemaKinds.td](http://llvm.org/viewvc/llvm-project/cfe/trunk/include/clang/Basic/DiagnosticSemaKinds.td?view=markup)
--- cfe/trunk/lib/Sema/SemaDeclAttr.cpp	2011/01/25 19:39:31	124216
+++ cfe/trunk/lib/Sema/SemaDeclAttr.cpp	2011/01/25 20:08:12	124217
@@ -885,6 +885,24 @@
   d->addAttr(::new (S.Context) VecReturnAttr(Attr.getLoc(), S.Context));
 }
 
+static void HandleForbidTemporariesAttr(Decl *d, const AttributeList &Attr,
+                                        Sema &S) {
+  assert(Attr.isInvalid() == false);
+
+  if (Attr.getNumArgs() != 0) {
+    S.Diag(Attr.getLoc(), diag::err_attribute_wrong_number_arguments) << 0;
+    return;
+  }
+
+  if (!isa<TypeDecl>(d)) {
+    S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type)
+      << Attr.getName() << 9 /*class*/;
+    return;
+  }
+
+  d->addAttr(::new (S.Context) ForbidTemporariesAttr(Attr.getLoc(), S.Context));
+}
+
 static void HandleDependencyAttr(Decl *d, const AttributeList &Attr, Sema &S) {
   if (!isFunctionOrMethod(d) && !isa<ParmVarDecl>(d)) {
     S.Diag(Attr.getLoc(), diag::err_attribute_wrong_decl_type)
@@ -2674,6 +2692,8 @@
   case AttributeList::AT_ext_vector_type:
     HandleExtVectorTypeAttr(scope, D, Attr, S);
     break;
+  case AttributeList::AT_forbid_temporaries:
+    HandleForbidTemporariesAttr(D, Attr, S); break;
   case AttributeList::AT_format:      HandleFormatAttr      (D, Attr, S); break;
   case AttributeList::AT_format_arg:  HandleFormatArgAttr   (D, Attr, S); break;
   case AttributeList::AT_global:      HandleGlobalAttr      (D, Attr, S); break;
#### [The meat of your attribute](http://clang.llvm.org/docs/InternalsManual.html#id45)
Find an appropriate place in Clang to do whatever your attribute needs to do.Check for the attribute’s presence usingDecl::getAttr<YourAttr>().
Update the [*Clang Language Extensions*](http://clang.llvm.org/docs/LanguageExtensions.html) document to describe your new attribute.
