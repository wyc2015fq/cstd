# Why objc_msgSend Must be Written in Assembly - game3108的专栏 - CSDN博客
2016年04月29日 13:34:25[game3108](https://me.csdn.net/game3108)阅读数：266
记录一个很不错的blog: http://arigrant.com/
Every message send in Objective-C is compiled into a call to objc_msgSend, a function that is called with the arguments as the message sense but with the receiver and the selector as the first two. So the line
[receiver message:foo beforeDate:bar];
will be compiled into to just call a C function:
objc_msgSend(receiver, @selector(message:beforeDate:), foo, bar);
There are many excellent write-ups on how the method is implemented or how it has evolved over the years, but I'd like to focus on one aspect that I haven't seen given too much attention:
It is not possible to implement objc_msgSend in C, Objective-C, or C++.
### THE RETURN TYPE
Consider the following two lines
NSUInteger n = [array count];
id obj = [array objectAtIndex:6];
which will be compiled as two C function calls.
NSUInteger n = objc_msgSend(array,  @selector(count));
id obj = objc_msgSend(array, @selector(objectAtIndex:), 6);
However there is no possible function that could actually satisfy both calls. Does it return an NSUInteger or an id?
The above two lines won't even compile. Whoops! Let's cast a slightly better lead.
NSUInteger n = (NSUInteger (*)(id, SEL))objc_msgSend(array,  @selector(count));
id obj = (id (*)(id, SEL, NSUInteger))objc_msgSend(array, @selector(objectAtIndex:), 6);
That's a little gross but at least it compiles. objc_msgSend is publicly available and declared in <objc/message.h>. If you do call it directly you will have to cast it in exactly this manner to appease the compiler,  but how on Earth is objc_msgSend implemented
 to support multiple return types?! We'll come back to it.
### THE IMP
The essence of objc_msgSend is quite simple. Given a receiver and a selector it does as follows:
- Get the class of the receiver
- Look for the selector in the method table
- If found, return it
- Else look at the superclass and repeat (until there is no superclass)
It's a pretty simply process that just walks up the inheritance hierarchy looking for the function pointer, the IMP, for the given selector. There's caches at each class level, but hey, that's just an implementation detail!
### THE ARG TYPES AND COUNT
Once objc_msgSend finds the function pointer it just needs to call it with the given args. What remains is to find a way to call an arbitrary function pointer on some numbers of arguments any set of types. The number of arguments is easy to account for. We
 could put them into varargs and then call a function with them. But if we did that, every Objective-C method would have to pull its arguments out of varargs in a hidden prologue and that would just be terrible and unnecessary.
In C, calling a function is compiled into instructions for setting up the arguments (putting them in registers or on the stack) and then a jump/call to the address of the function. If we want to support any possible method signature we would have to write a
 switch statement that contained every possible combination so that we would have the code to properly sets up the arguments for any and every method signature. This simply does not scalable and is not possible.
### UNWINDING THE CALL
The solution that objc_msgSend uses comes from the fact that the arguments were setup correctly for the call into objc_msgSend. Said another way, objc_msgSend began with a stack frame and register combination exactly equal to the one that will be needed by
 the IMP that it finds.
Consider again the line
id obj = objc_msgSend(array, @selector(objectAtIndex:), 6);
which needs to setup 3 arguments: the receiver, the selector, and the index. By insisting that the IMP that we find have the same signature we know that just moments ago the world was as we needed! So, once it finds the IMP it puts everything back the way it
 was (something functions do before returning anyway, called the function epilogue) and then simply jump to the address of that function pointer that was found for the given selector. All the arguments will be setup correctly and the function can go on its
 merry way.
That IMP will end up correctly setting the return value (on x86 and arm it just gets stored in a specific register, eax/rax or r0, respectively) and we don't have to worry about the return value issue discussed above.
### WRAP UP
Taking all of the rambling above and spinning it into a single statement would produce: calling a function in C requires the signature to be known for each call-site at compile-time; doing so at run-time is not possible and so one must drop down into assembly
 and party there instead.
### UPDATE:
It was pointed out that objc_msgSend could likely be implemented using the GCC extensions __builtin_apply_args, __builtin_apply, and __builtin_return. This is definitely true and points out the fact that such builtins were necessary to add since the language
 was not capable of the expressing the behavior on its own. The tricks needed to implement objc_msgSend would be the same tricks needed to implemented those builtins. The point of this writeup was not to get philosophical about what is or is not really C; the
 point was that objc_msgSend resides in a strange territory in the land of function calling and it felt fun to point out. :D
