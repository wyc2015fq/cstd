# __attribute__:Specifying Attributes of Variables - Koma Hub - CSDN博客
2019年01月24日 21:56:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：43
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
[http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Variable-Attributes.html#Variable-Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Variable-Attributes.html#Variable-Attributes)
### 5.31 Specifying Attributes of Variables
The keyword `__attribute__` allows you to specify special attributes of variables or structure fields. This keyword is followed by an attribute specification inside double parentheses. Some attributes are currently defined generically for variables. Other attributes are defined for variables on particular target systems. Other attributes are available for functions (see [Function Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Function-Attributes.html#Function-Attributes)) and for types (see [Type Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Type-Attributes.html#Type-Attributes)). Other front ends might define more attributes (see [Extensions to the C++ Language](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/C_002b_002b-Extensions.html#C_002b_002b-Extensions)).
You may also specify attributes with `__' preceding and following each keyword. This allows you to use them in header files without being concerned about a possible macro of the same name. For example, you may use `__aligned__` instead of `aligned`.
See [Attribute Syntax](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Attribute-Syntax.html#Attribute-Syntax), for details of the exact syntax for using attributes.
`aligned (`alignment`)`
This attribute specifies a minimum alignment for the variable or structure field, measured in bytes. For example, the declaration:
          int x __attribute__ ((aligned (16))) = 0;
     
causes the compiler to allocate the global variable `x` on a 16-byte boundary. On a 68040, this could be used in conjunction with an `asm` expression to access the `move16` instruction which requires 16-byte aligned operands.
You can also specify the alignment of structure fields. For example, to create a double-word aligned `int` pair, you could write:
          struct foo { int x[2] __attribute__ ((aligned (8))); };
     
This is an alternative to creating a union with a `double` member that forces the union to be double-word aligned.
As in the preceding examples, you can explicitly specify the alignment (in bytes) that you wish the compiler to use for a given variable or structure field. Alternatively, you can leave out the alignment factor and just ask the compiler to align a variable or field to the maximum useful alignment for the target machine you are compiling for. For example, you could write:
          short array[3] __attribute__ ((aligned));
     
Whenever you leave out the alignment factor in an `aligned` attribute specification, the compiler automatically sets the alignment for the declared variable or field to the largest alignment which is ever used for any data type on the target machine you are compiling for. Doing this can often make copy operations more efficient, because the compiler can use whatever instructions copy the biggest chunks of memory when performing copies to or from the variables or fields that you have aligned this way.
The `aligned` attribute can only increase the alignment; but you can decrease it by specifying `packed` as well. See below.
Note that the effectiveness of `aligned` attributes may be limited by inherent limitations in your linker. On many systems, the linker is only able to arrange for variables to be aligned up to a certain maximum alignment. (For some linkers, the maximum supported alignment may be very very small.) If your linker is only able to align variables up to a maximum of 8 byte alignment, then specifying `aligned(16)` in an `__attribute__` will still only provide you with 8 byte alignment. See your linker documentation for further information. 
`cleanup (`cleanup_function`)`
The `cleanup` attribute runs a function when the variable goes out of scope. This attribute can only be applied to auto function scope variables; it may not be applied to parameters or variables with static storage duration. The function must take one parameter, a pointer to a type compatible with the variable. The return value of the function (if any) is ignored.
If -fexceptions is enabled, then cleanup_function will be run during the stack unwinding that happens during the processing of the exception. Note that the `cleanup` attribute does not allow the exception to be caught, only to perform an action. It is undefined what happens if cleanup_function does not return normally. 
`common`
`nocommon`
The `common` attribute requests GCC to place a variable in “common” storage. The `nocommon` attribute requests the opposite—to allocate space for it directly.
These attributes override the default chosen by the -fno-common and -fcommon flags respectively. 
`deprecated`
The `deprecated` attribute results in a warning if the variable is used anywhere in the source file. This is useful when identifying variables that are expected to be removed in a future version of a program. The warning also includes the location of the declaration of the deprecated variable, to enable users to easily find further information about why the variable is deprecated, or what they should do instead. Note that the warning only occurs for uses:
          extern int old_var __attribute__ ((deprecated));
          extern int old_var;
          int new_fn () { return old_var; }
     
results in a warning on line 3 but not line 2.
The `deprecated` attribute can also be used for functions and types (see [Function Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Function-Attributes.html#Function-Attributes), see [Type Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Type-Attributes.html#Type-Attributes).) 
`mode (`mode`)`
This attribute specifies the data type for the declaration—whichever type corresponds to the mode mode. This in effect lets you request an integer or floating point type according to its width.
You may also specify a mode of `byte' or `__byte__' to indicate the mode corresponding to a one-byte integer, `word' or `__word__' for the mode of a one-word integer, and `pointer' or `__pointer__' for the mode used to represent pointers. 
`packed`
The `packed` attribute specifies that a variable or structure field should have the smallest possible alignment—one byte for a variable, and one bit for a field, unless you specify a larger value with the `aligned` attribute.
Here is a structure in which the field `x` is packed, so that it immediately follows `a`:
          struct foo
          {
            char a;
            int x[2] __attribute__ ((packed));
          };
     
`section ("`section-name`")`
Normally, the compiler places the objects it generates in sections like `data` and `bss`. Sometimes, however, you need additional sections, or you need certain particular variables to appear in special sections, for example to map to special hardware. The `section` attribute specifies that a variable (or function) lives in a particular section. For example, this small program uses several specific section names:
          struct duart a __attribute__ ((section ("DUART_A"))) = { 0 };
          struct duart b __attribute__ ((section ("DUART_B"))) = { 0 };
          char stack[10000] __attribute__ ((section ("STACK"))) = { 0 };
          int init_data __attribute__ ((section ("INITDATA"))) = 0;
          
          main()
          {
            /* Initialize stack pointer */
            init_sp (stack + sizeof (stack));
          
            /* Initialize initialized data */
            memcpy (&init_data, &data, &edata - &data);
          
            /* Turn on the serial ports */
            init_duart (&a);
            init_duart (&b);
          }
     
Use the `section` attribute with an *initialized* definition of a *global* variable, as shown in the example. GCC issues a warning and otherwise ignores the `section` attribute in uninitialized variable declarations.
You may only use the `section` attribute with a fully initialized global definition because of the way linkers work. The linker requires each object be defined once, with the exception that uninitialized variables tentatively go in the `common` (or `bss`) section and can be multiply “defined”. You can force a variable to be initialized with the -fno-common flag or the `nocommon` attribute.
Some file formats do not support arbitrary sections so the `section` attribute is not available on all platforms. If you need to map the entire contents of a module to a particular section, consider using the facilities of the linker instead. 
`shared`
On Microsoft Windows, in addition to putting variable definitions in a named section, the section can also be shared among all running copies of an executable or DLL. For example, this small program defines shared data by putting it in a named section `shared` and marking the section shareable:
          int foo __attribute__((section ("shared"), shared)) = 0;
          
          int
          main()
          {
            /* Read and write foo.  All running
               copies see the same value.  */
            return 0;
          }
     
You may only use the `shared` attribute along with `section` attribute with a fully initialized global definition because of the way linkers work. See `section` attribute for more information.
The `shared` attribute is only available on Microsoft Windows. 
`tls_model ("`tls_model`")`
The `tls_model` attribute sets thread-local storage model (see [Thread-Local](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Thread_002dLocal.html#Thread_002dLocal)) of a particular `__thread` variable, overriding -ftls-model= command line switch on a per-variable basis. The tls_model argument should be one of `global-dynamic`, `local-dynamic`, `initial-exec` or `local-exec`.
Not all targets support this attribute. 
`transparent_union`
This attribute, attached to a function parameter which is a union, means that the corresponding argument may have the type of any union member, but the argument is passed as if its type were that of the first union member. For more details see See [Type Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Type-Attributes.html#Type-Attributes). You can also use this attribute on a `typedef` for a union data type; then it applies to all function parameters with that type. 
`unused`
This attribute, attached to a variable, means that the variable is meant to be possibly unused. GCC will not produce a warning for this variable. 
`vector_size (`bytes`)`
This attribute specifies the vector size for the variable, measured in bytes. For example, the declaration:
          int foo __attribute__ ((vector_size (16)));
     
causes the compiler to set the mode for `foo`, to be 16 bytes, divided into `int` sized units. Assuming a 32-bit int (a vector of 4 units of 4 bytes), the corresponding mode of `foo` will be V4SI.
This attribute is only applicable to integral and float scalars, although arrays, pointers, and function return values are allowed in conjunction with this construct.
Aggregates with this attribute are invalid, even if they are of the same size as a corresponding scalar. For example, the declaration:
          struct S { int a; };
          struct S  __attribute__ ((vector_size (16))) foo;
     
is invalid even if the size of the structure is the same as the size of the `int`. 
`weak`
The `weak` attribute is described in See [Function Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Function-Attributes.html#Function-Attributes). 
`dllimport`
The `dllimport` attribute is described in See [Function Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Function-Attributes.html#Function-Attributes). 
`dlexport`
The `dllexport` attribute is described in See [Function Attributes](http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Function-Attributes.html#Function-Attributes).
5.31.1 M32R/D Variable Attributes
One attribute is currently defined for the M32R/D.
`model (`model-name`)`
Use this attribute on the M32R/D to set the addressability of an object. The identifier model-name is one of `small`, `medium`, or `large`, representing each of the code models.
Small model objects live in the lower 16MB of memory (so that their addresses can be loaded with the `ld24` instruction).
Medium and large model objects may live anywhere in the 32-bit address space (the compiler will generate `seth/add3` instructions to load their addresses).
5.31.2 i386 Variable Attributes
Two attributes are currently defined for i386 configurations: `ms_struct` and `gcc_struct`
`ms_struct`
`gcc_struct`
If `packed` is used on a structure, or if bit-fields are used it may be that the Microsoft ABI packs them differently than GCC would normally pack them. Particularly when moving packed data between functions compiled with GCC and the native Microsoft compiler (either via function call or as data in a file), it may be necessary to access either format.
Currently -m[no-]ms-bitfields is provided for the Microsoft Windows X86 compilers to match the native Microsoft compiler.
5.31.3 Xstormy16 Variable Attributes
One attribute is currently defined for xstormy16 configurations: `below100`
`below100`
If a variable has the `below100` attribute (`BELOW100` is allowed also), GCC will place the variable in the first 0x100 bytes of memory and use special opcodes to access it. Such variables will be placed in either the `.bss_below100` section or the`.data_below100` section.
