# The Python IAQ:  Infrequently Answered Questions——Peter Norvig - YZXnuaa的博客 - CSDN博客
2018年07月16日 20:56:04[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：99
**Q: What is an Infrequently Answered Question?**
A question is infrequently answered either because few people know the answer or because it concerns an obscure, subtle point (but a point that may be crucial to you). I thought I had invented the term for my [Java IAQ](http://norvig.com/java-iaq.html), but it also shows up at the very informative [About.com Urban Legends](http://urbanlegends.about.com/library/weekly/aa082497.htm) site. There are lots of Python FAQs around, but this is the only Python IAQ, except for the [**Chinese translation**](http://www.chenwy.com/python-iaq-cn.html) of this page by Weiyang Chen, the [Russian translation](https://docs.google.com/document/pub?id=1dTAzkH8Cv9Kx0i8f2czAJ9ToKU54btObQGW86Xm5JO4) by Alexander Sviridenko, and the [**Japanese translation**](http://newworld.ddo.jp/doc/python-iaq-ja/index_html) by Akihiro Takizawa. (There are a few Infrequently*Asked* Questions lists, including a satirical one on [C](http://www.plethora.net/~seebs/faqs/c-iaq.html).)
This page was first written in 2000, for an ancient version of Python; over the years many of the answers have been incorporated into newer versions of Python.
**Q: The code in a finally clause will never fail to execute, right?**
What never? Well, hardly ever. The code in a finally clause does get executed after the try clause whether or not there is an exception, and even if sys.exit is called. However, the finally clause will not execute if execution never gets to it. This would happen regardless of the value of choice in the following:
```
try:
    if choice:
        while True:
            pass
    else:
        print "Please pull the plug on your computer sometime soon..."
        time.sleep(60 * 60 * 24 * 365 * 10000)
finally:
    print "Finally ..."
```
:
    if choice:
        while True:
            pass
    else:
        print "Please pull the plug on your computer sometime soon..."
        time.sleep(60 * 60 * 24 * 365 * 10000)
finally:
    print "Finally ..."
**Q: Polymorphism is great; I can sort a list of elements of any type, right?**
Wrong. Consider this:
```
>>> x = [1, 1j]
>>> x.sort()
Traceback (most recent call last):
  File "<pyshell#13>", line 1, in ?
    x.sort()
TypeError: cannot compare complex numbers using <, <=, >, >=
```
 x = [1, 1j]
>>> x.sort()
Traceback (most recent call last):
  File "<pyshell#13>", line 1, in ?
    x.sort()
TypeError: cannot compare complex numbers using <, <=, >, >=
(In Python notation 1j is an imaginary number, a square root of -1.) The problem is that the sortmethod compares elements  using the __lt__ method, which refuses to compare complex numbers (because they are not orderable). So the answer is you can sort a sequence of objects that support the __lt__ method.
As for the first part of the question, "Polymorphism is great", I would agree, but Python sometimes makes it difficult because many Python types (such as sequence, and number) are defined informally.
**Q: Can I do ++x and x++ in Python?**
Literally, yes and no; but for practical purposes, no. What do I mean by that?
- **Yes**, ++x is legal Python, but it won't mean what you think it should mean if you're a C++ or Java programmer. The + is a unary prefix operator, so ++x is parsed as +(+(x)), which (at least for numbers) results in just x.
- **No**, x++ by itself is not a legal expression, although there are contexts where it is legal. For example, x++ - y is parsed as x + (+(-(y))), so this is equivalent to x - y for numbers. Now of course you *could* create a class where ++x made (a limited amount of) sense; for example the class could hold a number and have the unary + operator increment it by 0.5 (or increment it by 1 with probability 0.5 if you like [randomized algorithms](http://www.amazon.com/exec/obidos/ASIN/0521474655)), but ...
- **No**, that would be silly. Better to stick with x += 1, which was added in Python 2.0.
The deeper question is: *why* doesn't Python allow x++? I believe it is the same reason why Python does not allow assignments in expressions: Python wants to clearly separate statements and expressions. If you believe they should be distinct, then disallowing ++ is probably the best decision. On the other hand, advocates of functional languages argue that statements should be expressions. I'm with my fellow Dane, Bjarne Stroustrup, on this one. He said in [*The Design and Evolution of C++*](http://www.amazon.com/exec/obidos/ASIN/0201543303/) ``If I were to design a language from scratch, I would follow the Algol68 path and make every statement and declaration an expression that yields a value''.
**Q: Can I use C++'s syntax for ostreams: cout << x << y ... ?**
You can. If you don't like writing "print(x, y)" then you can try this:
```
import sys
class **ostream**:
    def **__init__**(self, file):
        self.file = file
        
    def **__lshift__**(self, obj):
        self.file.write(str(obj));
        return self
**cout** = ostream(sys.stdout)
**cerr** = ostream(sys.stderr)
**nl** = '\n'
```
 sys
class **ostream**:
    def **__init__**(self, file):
        self.file = file
        
    def **__lshift__**(self, obj):
        self.file.write(str(obj));
        return self
**cout** = ostream(sys.stdout)
**cerr** = ostream(sys.stderr)
**nl** = '\n'
			cout << x << " " << y << nl
*(This document shows code that belongs in a file above the horizontal line and example uses of it below the line.)* This gives you a different syntax, but it doesn't give you a new convention for printing--it just packages up the str convention that already exists in Python. This is similar to the toString() convention in Java. C++ has a very different convention: instead of a canonical way to convert an object to a string, there is a canonical way to print an object to a stream (well, semi-canonical---a lot of C++ code still uses printf). The stream approach is more complicated, but it does have the advantage that if you need to print a really huge object you needn't create a really huge temporary string to do it.
**Q: What if I like C++'s printf?**
In Python 2, print is a statement, and it is not a bad idea to implement a print function such as printf. In Python 3, is a function, so the need goes away. You could do this in Python 2:
`def printf(format, *args): print format % args,` printf(format, *args): print format % args,
Even in a one-liner like this, there are a few subtleties. First, I had to decide whether to add the comma at the end or not. To be more like C++, I decided to add it (which means that if you want a newline printed, you have to add it yourself to the end of the format string). Second, this will still print a trailing space. If you don't want that, use sys.stdout.write instead of print. Third, is this good for anything besides being more C-like? Yes; you need a function (not a statement) in places like lambda expressions and as the first argument to map. In fact, such a function is so handy, that in Python 2 you probably want one that does not do formatting:
`def prin(x): print x,` prin(x): print x,
I've seen some careless programmers (well, OK, it was me, but I *knew* I was being careless) think it would be a good idea to fit both these functions into one, as follows:
`def **printf**(format, *args): print str(format) % args,`**printf**(format, *args): print str(format) % args,
Then printf(42), printf('A multi-line\n message') and printf('%4.2f', 42) all work. But the ``good idea'' thought gets changed to ``what was I thinking'' as soon as you do printf('100% guaranteed'), or anything else with a % character that is not meant as a formatting directive.
**Q: How does the comma-equals operator work?**
You might have seen this operator:
```
>>> x ,= range(1)
>>> x
0
```
 x ,= range(1)
>>> x
0
Actually, there is no ",=" operator; really this is the same as:
```
>>> (x,) = range(1)
>>> x
0
```
(x,) = range(1)
>>> x
0
This is just an assignment statement, with a tuple on the left, meaning to assign each element of the tuple to the corresponding element of the right-hand-side; in this case x gets assigned 0
**Q: Do these unit tests show that there is a factorial operator in Python?**
```
assert 0!=1
assert 3!=6
assert 4!=24
assert 5!=120
```
 0!=1
assert 3!=6
assert 4!=24
assert 5!=120
No. The assertions are all true, but they just show that it is a good idea to follow PEP 8 and put spaces around operators:
```
assert 0 != 1
assert 3 != 6
assert 4 != 24
assert 5 != 120
```
 0 != 1
assert 3 != 6
assert 4 != 24
assert 5 != 120
Now we see that the tests involve the "not equals" operator, not a "factorial" followed by an "equals".
**Q: Why is the ski hat emoticon like the dumbell emoticon?**
The ski hat is *=0 and the dumbell is [:]=[].
They both perform the function of removing all the elements in a list:
```
>>> L = [1, 2, 3]
>>> L *=0
>>> L [:]=[]
>>> L
[]
```
 L = [1, 2, 3]
>>> L *=0
>>> L [:]=[]
>>> L
[]
**Q: Is there a better syntax for dictionary literals? All my keys are identifiers.**
As of Python 2.3 you can use the syntax 
    dict(a=1, b=2, c=3, dee=4) 
rather than the tedious 
    {'a': 1, 'b': 2, 'c': 3, 'dee': 4}. 
(Before that, you needed to define a function.)
**Q: Is there a similar shortcut for objects?**
Indeed there is. When all you want to do is create an object that holds data in several fields, the following will do:
```
class **Struct**:
    "A structure that can have any fields defined."
    def **__init__**(self, **entries): self.__dict__.update(entries)
```
**Struct**:
    "A structure that can have any fields defined."
    def **__init__**(self, **entries): self.__dict__.update(entries)
			>>> options = Struct(answer=42, linelen=80, font='courier')
			>>> options.answer
			42
			>>> options.answer = 'plastics'
			>>> vars(options)
			{'answer': 'plastics', 'font': 'courier', 'linelen': 80}
Essentially what we are doing here is creating an *anonymous class*. OK, I know that the class of options is Struct, but because we are adding slots to it, its like creating a new, unnamed class (in much the same way that lambda creates anonymous functions). I hate to mess with Struct because it is so concise the way it is, but if you add the following method then you will get a nice printed version of each structure:
```
def **__repr__**(self):
        args = ['%s=%s' % (k, repr(v)) for (k,v) in vars(self).items()]
        return 'Struct(%s)' % ', '.join(args)
```
def **__repr__**(self):
        args = ['%s=%s' % (k, repr(v)) for (k,v) in vars(self).items()]
        return 'Struct(%s)' % ', '.join(args)
			>>> options
			Struct(answer='plastics', font='courier', linelen=80)
If you want a non-anonymous class, there is collections.namedtuple.
**Q: That's great for *creating* objects; How about for *updating* them?**
Well, dictionaries have an update method, so you could do d.update(dict(a=100, b=200)) when d is a dictionary. There is no corresponding method for objects, so you have to do obj.a = 100; obj.b = 200. Or you could define one function to let you do update(x, a=100, b=200) when x is either a dictionary or an object:
```
def **update**(x, **entries):
    if hasattr(x, 'update'): 
        x.update(entries)
    else: 
        x.__dict__.update(entries)
    return x
```
**update**(x, **entries):
    if hasattr(x, 'update'): 
        x.update(entries)
    else: 
        x.__dict__.update(entries)
    return x
This is especially concise for constructors:
```
def **__init__**(self, a, b, c, d=42, e=None, f=()):
        update(self, a=a, b=b, c=c, d=d, e=e, f=f)
```
def **__init__**(self, a, b, c, d=42, e=None, f=()):
        update(self, a=a, b=b, c=c, d=d, e=e, f=f)
  
Although some people prefer a more explicit, verbose style:
```
def **__init__**(self, a, b, c, d=42, e=None, f=()):
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.e = e
        self.f = f
```
def **__init__**(self, a, b, c, d=42, e=None, f=()):
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.e = e
        self.f = f
**Q: What if I want a method on my namedtuple? Or a docstring?**
collections.namedtuple allows you to define a new class with a fixed set of fields. But you can't define methods on that class, and there is no place to put the docstring. Unless you use this trick of *inheriting* from a namedtuple:
```
class Point(namedtuple('_', 'x y')):
    "A point in 2-dimensional space."
    def __abs__(self):
        "Absolute value of a Point: the length of the vector."
        return (self.x ** 2 + self.y ** 2) ** 0.5
```
 Point(namedtuple('_', 'x y')):
    "A point in 2-dimensional space."
    def __abs__(self):
        "Absolute value of a Point: the length of the vector."
        return (self.x ** 2 + self.y ** 2) ** 0.5
**Q: Is that the best way to define a 2-dimensional point?**
There are many ways. The simplest is this:
`Point = complex` = complex
But then your fields are called real and imag, not x and y, so you might do this instead:
```
class Point(complex):
    x = property(lambda p: p.real)
    y = property(lambda p: p.imag)
    def __repr__(self): return 'Point({}, {})'.format(self.x, self.y)
    def __iter__(self): yield self.x; yield self.y
```
 Point(complex):
    x = property(lambda p: p.real)
    y = property(lambda p: p.imag)
    def __repr__(self): return 'Point({}, {})'.format(self.x, self.y)
    def __iter__(self): yield self.x; yield self.y
Now, you can do things like this:
```
>>> p = Point(3, 4)
Point(3.0, 4.0)
>>> p.x
3.0
>>> px, py = p
>>> px, py
(3.0, 4.0)
>>> abs(p)
5.0
```
 p = Point(3, 4)
Point(3.0, 4.0)
>>> p.x
3.0
>>> px, py = p
>>> px, py
(3.0, 4.0)
>>> abs(p)
5.0
You can also do arithmetic, like 2 * p, but unfortuntely the result is a complex, not a Point. Another possibility is:
`def Point(x, y): return np.array([x, y])` Point(x, y): return np.array([x, y])
Or you can just use a 2-tuple:
`def Point(x, y): return x, y` Point(x, y): return x, y
**Q: Can I have a dict with a default value of 0 or [ ] or something?**
Before Python 2.7, you would need to define a class to do this; now you can use collections.defaultdict or collections.Counter, e.g.:
```
from collections import Counter
words = 'this is a test this is only a test'.split()
```
 collections import Counter
words = 'this is a test this is only a test'.split()
			>>> Counter(words)
			Counter({'this': 2, 'test': 2, 'a': 2, 'is': 2, 'only': 1})
**Q: A Counter counts items; what if I want to *group* items?**
Continuing the example above, we can define a groupby function, which is similar to itertools.groupby, but does not require the input sequence to be sorted:
```
def groupby(iterable, key=lambda x: x):
    "Return a dict of {key(item): [items...]} grouping all the items in the iterable."
    groups = defaultdict(list)
    for item in iterable:
        groups[key(item)].append(item)
    return groups
```
 groupby(iterable, key=lambda x: x):
    "Return a dict of {key(item): [items...]} grouping all the items in the iterable."
    groups = defaultdict(list)
    for item in iterable:
        groups[key(item)].append(item)
    return groups
Here's an example using groupby to create a table of anagrams:
```
def sortedstr(word): return ''.join(sorted(word))
def anagram_table(words): return groupby(words, sortedstr)
```
 sortedstr(word): return ''.join(sorted(word))
def anagram_table(words): return groupby(words, sortedstr)
			>>> anagram_table('earth hater at heart stop post on pots no stop'.split())
			defaultdict(, {'at': ['at'], 'aehrt': ['earth', 'hater', 'heart'],
			                             'no': ['on', 'no'], 'opst': ['stop', 'post', 'pots', 'stop']})
**Q: Hey, can you write code to transpose a matrix in 0.007KB or less?**
I thought you'd never ask. If you represent a matrix as a sequence of sequences, then zip can do the job:
```
>>> m = [(1,2,3), (4,5,6)]
>>> **zip(*m)**
[(1, 4), (2, 5), (3, 6)]
```
 m = [(1,2,3), (4,5,6)]
>>> **zip(*m)**
[(1, 4), (2, 5), (3, 6)]
To understand this, you need to know that f(*m) is like apply(f, m). This is based on an old Lisp question, the answer to which is Python's equivalent of map(None, *m), but the zip version, suggested by Chih-Chung Chang, is even shorter. You might think this is only useful for an appearance on Letterman's Stupid Programmer's Tricks, but just the other day I was faced with this problem: given a list of database rows, where each row is a list of ordered values, find the domains (set of unique values) for each column. So I wrote:
`domains = map(set, zip(*db))`= map(set, zip(*db))
**Q: The f(*m) trick is cool. Does the same syntax work with method calls, like x.f(*y)?**
This question reveals a common misconception. There is no syntax for method calls! There is a syntax for calling a function, and there is a syntax for extracting a field from an object, and there are bound methods. Together these three features conspire to make it look like x.f(y) is a single piece of syntax, when actually it is equivalent to (x.f)(y), which is equivalent to(getattr(x, 'f'))(y). I can see you don't believe me. Look:
```
class **X**:
    def f(self, y): return 2 * y
```
**X**:
    def f(self, y): return 2 * y
			>>> x = X()
			>>> x.f
			<bound method X.f of <__main__.X instance at 0x009C7DB0>>
			>>> y = 21
			>>> x.f(y)
			42
			>>> (x.f)(y)
			42
			>>> (getattr(x, 'f'))(y)
			42
			>>> xf = x.f
			>>> xf(y)
			42
			>>> map(x.f, range(5))
			[0, 2, 4, 6, 8]
So the answer to the question is: you can put *y or **y (or anything else that you would put into a function call) into a method call, because method calls are just function calls.
**Q: Can you implement abstract classes in Python in 0 lines of code? Or 4?**
Java has an abstract keyword so you can define abstract classes that cannot be instantiated, but can be subclassed if you implement all the abstract methods in the class. It is a little known fact that you can use abstract in Python in almost the same way; the difference is that you get an error at runtime when you try to call the unimplemented method, rather than at compile time. Compare:
|```## Pythonclass **MyAbstractClass**:    def **method1**(self): abstractclass **MyClass**(MyAbstractClass):     pass```class **MyAbstractClass**:    def **method1**(self): abstractclass **MyClass**(MyAbstractClass):     pass			>>> MyClass().method1()			Traceback (most recent call last):			    ...			NameError: name 'abstract' is not defined|```/* Java */public abstract class **MyAbstractClass** {    public abstract void **method1**();}class **MyClass** extends MyAbstractClass {}```public abstract class **MyAbstractClass** {    public abstract void **method1**();}class **MyClass** extends MyAbstractClass {}			% javac MyAbstractClass			MyAbstractClass.java:5:			  class MyClass must be declared abstract.			  It does not define void method1() from class MyAbstractClass.|
Don't spend too much time looking for the abstract keyword in the Python Language Reference Manual; it isn't there. I added it to the language, and the great part is, the implementation is zero lines of code! What happens is that if you call method1, you get a NameError because there is no abstract variable. (You might say that's cheating, because it will break if somebody defines a variable called abstract. But then any program will break if someone redefines a variable that the code depends on. The only difference here is that we're depending on the lack of a definition rather than on a definition.)
If you're willing to write abstract() instead of abstract, then you can define a function that raises a NotImplementedError instead of a NameError, which makes more sense. (Also, if someone redefines abstract to be anything but a function of zero arguments, you'll still get an error message.) To make abstract's error message look nice, just peek into the stack frame to see who the offending caller is:
```
def **abstract**():
    import inspect
    caller = inspect.getouterframes(inspect.currentframe())[1][3]
    raise NotImplementedError(caller + ' must be implemented in subclass')
```
**abstract**():
    import inspect
    caller = inspect.getouterframes(inspect.currentframe())[1][3]
    raise NotImplementedError(caller + ' must be implemented in subclass')
			>>> MyDerivedClass().method1()
			Traceback (most recent call last):
			    ...
			NotImplementedError: method1 must be implemented in subclass
**Q: How do I do Enumerated Types (enums) in Python?**
I had some code here, but as of Python 3.4, there is now an [enum](https://docs.python.org/3/library/enum.html) in the standard library.
**Q: Why is there no *Set* data type in Python?**
When this question was first posed there wasn't one, and programmers mostly used dictionaries instead, but now in Python 2.4 there is good native support for the [set type](http://docs.python.org/lib/types-set.html).
**Q: How about a Boolean type?**
When this question was first posed there was no Boolean type, but Python 2.3 added [bool](http://docs.python.org/lib/node31.html).
One annoying thing is that the two values print as True and False, which have different character lengths, and thus do not line up nicely in tables. I "fixed" that problem:
```
class Bool(int):
    """Just like `bool`, except values display as 'T' and 'F' instead of 'True' and 'False'"""
    __str__ = __repr__ = lambda self: 'T' if self else 'F'
T = Bool(True)
F = Bool(False)
```
 Bool(int):
    """Just like `bool`, except values display as 'T' and 'F' instead of 'True' and 'False'"""
    __str__ = __repr__ = lambda self: 'T' if self else 'F'
T = Bool(True)
F = Bool(False)
Note that it is not legal to subclass bool, so I had to subclass int. But now T == True and F == False.
**Q: Can I do the equivalent of (test ? result : alternative) in Python?**
There used to be various tricks for this; since Python 2.5 there are now [conditional expressions](https://docs.python.org/2/reference/expressions.html#conditional-expressions)of the form 
*result* if *test* else *alternative*.
**Q: What other major types are missing from Python?**
One great thing about Python is that you can go a long way with numbers, strings, lists, and dicts (and now sets and bools and defaultdics and Counters and dequeues). But there are a few major types that are still missing. For me, the most important is a mutable string. Doing str += x over and over, is slow, and manipulating lists of characters (or lists of sub-strings) means you give up some of the nice string functions. One possibility is array.array('c'). Another is UserString.MutableString, although its intended use is more educational than practical. A third is the mmap module and a fourth is cStringIO. None of these is perfect, but together they provide enough choices. After that, I find I often want a queue of some sort. There is a standard library[Queue module](http://www.python.org/doc/current/lib/module-Queue.html), but it doesn't do everything I want. I have some Stack and Queue implementations [here](https://github.com/aimacode/aima-python/blob/master/utils.py#L510).
**Q: How do I do the Singleton Pattern in Python?**
I assume you mean that you want a class that can only be instantiated once, and raises an exception if you try to make another instance. There are [many answers](http://stackoverflow.com/questions/6760685/creating-a-singleton-in-python) with different pros and cons. The simplest way I know to do that is to define a function that enforces the idea, and call the function from the constructor in your class:
```
def singleton(object):
    "Raise an exception if an object of this class has been instantiated before."
    cls = object.__class__
    if hasattr(cls, '__instantiated'):
        raise ValueError("%s is a Singleton class but is already instantiated" % cls)
    cls.__instantiated = True
class YourClass:
    "A singleton class to do something ..."
    def __init__(self, args):
        singleton(self)
        ...
```
 singleton(object):
    "Raise an exception if an object of this class has been instantiated before."
    cls = object.__class__
    if hasattr(cls, '__instantiated'):
        raise ValueError("%s is a Singleton class but is already instantiated" % cls)
    cls.__instantiated = True
class YourClass:
    "A singleton class to do something ..."
    def __init__(self, args):
        singleton(self)
        ...
You could mess around with metaclasses or decorators, but why bother? Before the Gang of Four got all academic on us, ``singleton'' (without the formal name) was just a simple idea that deserved a simple line of code, not a whole religion.
**Q: Is no "news" good news?**
I presume you mean is it good that Python has no **new** keyword. It is indeed. In C++, **new** is used to mark allocation on the heap rather than the stack. As such, the keyword is useful. In Java, all objects are heap-allocated, so **new** has no real purpose; it only serves as a reminder of the distinction between a constructor and other static methods. But making this distinction probably does more harm than good in Java, because the distinction is a low-level one that forces implementation decisions that really should be delayed. I think Python made the right choice in keeping the syntax of a constructor call the same as the syntax of a normal function call, including a factory.
**Q: Can I have a history mechanism like in the shell?**
Yes. You can use the powerful [IPython](https://en.wikipedia.org/wiki/IPython) shell, or if you want something like this:
```
h[2] >>> 7*8
56
h[3] >>> 9*9
81
h[4] >>> h[2]
56
h[5] >>> 'hello' + ' world'
'hello world'
h[6] >>> h
[None, 9, 56, 81, 56, 'hello world']
h[7] >>> h[5] * 2
'hello worldhello world'
```
[2] >>> 7*8
56
h[3] >>> 9*9
81
h[4] >>> h[2]
56
h[5] >>> 'hello' + ' world'
'hello world'
h[6] >>> h
[None, 9, 56, 81, 56, 'hello world']
h[7] >>> h[5] * 2
'hello worldhello world'
Then all you need is this code:
```
import sys
h = [None]
class **HistoryPrompt**:
    "Create a prompt that stores results (i.e. _) in the list h."
    def **__init__**(self, str='h[%d] >>> '):
        self.str = str;
        
    def **__str__**(self):
        try:
            if _ not in [h[-1], None, h]: h.append(_);
        except NameError: # _ not defined yet
            pass 
        return self.str % len(h);
    
    def **__radd__**(self, other):
        return str(other) + str(self)
sys.ps1 = HistoryPrompt()
```
 sys
h = [None]
class **HistoryPrompt**:
    "Create a prompt that stores results (i.e. _) in the list h."
    def **__init__**(self, str='h[%d] >>> '):
        self.str = str;
        
    def **__str__**(self):
        try:
            if _ not in [h[-1], None, h]: h.append(_);
        except NameError: # _ not defined yet
            pass 
        return self.str % len(h);
    
    def **__radd__**(self, other):
        return str(other) + str(self)
sys.ps1 = HistoryPrompt()
How does this work? The variable sys.ps1 is the system prompt. By default it is the string '>>> ' but you can set it to anything else. If you set it to a non-string object, the object's __str__method gets called.  So we'll create an object whose __str__ method appends the most recent result (the variable _) to a list called h (for history), and then returns a prompt string that includes the length of the list. Or at least that was the plan. As it turns out (at least on the IDLE 2.2 implementation on Windows), sys.ps1.__str__ gets called *three* times, not just once before the prompt is printed. Don't ask me why. To combat this, I only append _ when it is not already the last element in the history list. And I don't bother inserting None into the history list, because it's not displayed by the Python interactive loop, and I don't insert h itself into h, because the circularity could lead to problems printing or comparing. Another complication was that the Python interpreter actually attempts to print '\n' + sys.ps1, (when it should print the '\n' separately, or print '\n' + str(sys.ps1)) which means that sys.ps1 needs an __radd__ method as well. Finally, my first version would fail if imported as the very first input in a Python session (or in the .python startup file). After some detective work it turns out this is because the variable _ is not bound until after the first expression is evaluated. So I catch the exception if _ is unbound.
**Q: How do I time the execution of my functions?**
See [timeit](https://docs.python.org/2/library/timeit.html). Or %time in IPython. Or:
```
def **timer**(fn, *args):
    "Time the application of fn to args. Return (result, seconds)."
    import time
    start = time.clock()
    return fn(*args), time.clock() - start
```
**timer**(fn, *args):
    "Time the application of fn to args. Return (result, seconds)."
    import time
    start = time.clock()
    return fn(*args), time.clock() - start
			>>> timer(max, range(1e6))
			(999999, 0.4921875)
There's a more complex answer in my [utils module](http://norvig.com/python/utils.html).
**Q: What are some other useful utility functions that are missing in Python?**
Here are a few of my favorites:
```
def removeall(item, seq):
    """Return a copy of seq (or string) with all occurences of item removed."""
    if isinstance(seq, str):
        return seq.replace(item, '')
    else:
        return [x for x in seq if x != item]
def count(seq):
    """Count the number of items in sequence that are interpreted as true."""
    return sum(bool(x) for x in seq)
def first(iterable, default=None):
    "Return the first element of an iterable or the next element of a generator; or default."
    try:
        return iterable[0]
    except IndexError:
        return default
    except TypeError:
        return next(iterable, default)
```
 removeall(item, seq):
    """Return a copy of seq (or string) with all occurences of item removed."""
    if isinstance(seq, str):
        return seq.replace(item, '')
    else:
        return [x for x in seq if x != item]
def count(seq):
    """Count the number of items in sequence that are interpreted as true."""
    return sum(bool(x) for x in seq)
def first(iterable, default=None):
    "Return the first element of an iterable or the next element of a generator; or default."
    try:
        return iterable[0]
    except IndexError:
        return default
    except TypeError:
        return next(iterable, default)
Thanks to Amit J. Patel, Max M, Dan Winkler, Chih-Chung Chang, Bruce Eckel, Kalle Svensson, Mike Orr, Steven Rogers and others who contributed ideas and corrections.
[*Peter Norvig*](mailto:peter@norvig.com)
