# python 3 operator函数包 - zj360202的专栏 - CSDN博客





2013年04月14日 18:46:56[zj360202](https://me.csdn.net/zj360202)阅读数：1892








```
The operator module exports a set of efficient functions corresponding to the intrinsic operators of Python. For example, operator.add(x, y) is equivalent to the expression x+y. The function names are those used for special class methods; variants without leading and trailing __ are also provided for convenience.

The functions fall into categories that perform object comparisons, logical operations, mathematical operations and sequence operations.

The object comparison functions are useful for all objects, and are named after the rich comparison operators they support:

operator.lt(a, b) 
operator.le(a, b) 
operator.eq(a, b) 
operator.ne(a, b) 
operator.ge(a, b) 
operator.gt(a, b) 
operator.__lt__(a, b) 
operator.__le__(a, b) 
operator.__eq__(a, b) 
operator.__ne__(a, b) 
operator.__ge__(a, b) 
operator.__gt__(a, b) 
Perform “rich comparisons” between a and b. Specifically, lt(a, b) is equivalent to a < b, le(a, b) is equivalent to a <= b, eq(a, b) is equivalent to a == b, ne(a, b) is equivalent to a != b, gt(a, b) is equivalent to a > b and ge(a, b) is equivalent to a >= b. Note that these functions can return any value, which may or may not be interpretable as a Boolean value. See Comparisons for more information about rich comparisons.

The logical operations are also generally applicable to all objects, and support truth tests, identity tests, and boolean operations:

operator.not_(obj) 
operator.__not__(obj) 
Return the outcome of not obj. (Note that there is no __not__() method for object instances; only the interpreter core defines this operation. The result is affected by the __bool__() and __len__() methods.)

operator.truth(obj) 
Return True if obj is true, and False otherwise. This is equivalent to using the bool constructor.

operator.is_(a, b) 
Return a is b. Tests object identity.

operator.is_not(a, b) 
Return a is not b. Tests object identity.

The mathematical and bitwise operations are the most numerous:

operator.abs(obj) 
operator.__abs__(obj) 
Return the absolute value of obj.

operator.add(a, b) 
operator.__add__(a, b) 
Return a + b, for a and b numbers.

operator.and_(a, b) 
operator.__and__(a, b) 
Return the bitwise and of a and b.

operator.floordiv(a, b) 
operator.__floordiv__(a, b) 
Return a // b.

operator.index(a) 
operator.__index__(a) 
Return a converted to an integer. Equivalent to a.__index__().

operator.inv(obj) 
operator.invert(obj) 
operator.__inv__(obj) 
operator.__invert__(obj) 
Return the bitwise inverse of the number obj. This is equivalent to ~obj.

operator.lshift(a, b) 
operator.__lshift__(a, b) 
Return a shifted left by b.

operator.mod(a, b) 
operator.__mod__(a, b) 
Return a % b.

operator.mul(a, b) 
operator.__mul__(a, b) 
Return a * b, for a and b numbers.

operator.neg(obj) 
operator.__neg__(obj) 
Return obj negated (-obj).

operator.or_(a, b) 
operator.__or__(a, b) 
Return the bitwise or of a and b.

operator.pos(obj) 
operator.__pos__(obj) 
Return obj positive (+obj).

operator.pow(a, b) 
operator.__pow__(a, b) 
Return a ** b, for a and b numbers.

operator.rshift(a, b) 
operator.__rshift__(a, b) 
Return a shifted right by b.

operator.sub(a, b) 
operator.__sub__(a, b) 
Return a - b.

operator.truediv(a, b) 
operator.__truediv__(a, b) 
Return a / b where 2/3 is .66 rather than 0. This is also known as “true” division.

operator.xor(a, b) 
operator.__xor__(a, b) 
Return the bitwise exclusive or of a and b.

Operations which work with sequences (some of them with mappings too) include:

operator.concat(a, b) 
operator.__concat__(a, b) 
Return a + b for a and b sequences.

operator.contains(a, b) 
operator.__contains__(a, b) 
Return the outcome of the test b in a. Note the reversed operands.

operator.countOf(a, b) 
Return the number of occurrences of b in a.

operator.delitem(a, b) 
operator.__delitem__(a, b) 
Remove the value of a at index b.

operator.getitem(a, b) 
operator.__getitem__(a, b) 
Return the value of a at index b.

operator.indexOf(a, b) 
Return the index of the first of occurrence of b in a.

operator.setitem(a, b, c) 
operator.__setitem__(a, b, c) 
Set the value of a at index b to c.

Example: Build a dictionary that maps the ordinals from 0 to 255 to their character equivalents.

>>> d = {}
>>> keys = range(256)
>>> vals = map(chr, keys)
>>> map(operator.setitem, [d]*len(keys), keys, vals)   
The operator module also defines tools for generalized attribute and item lookups. These are useful for making fast field extractors as arguments for map(), sorted(), itertools.groupby(), or other functions that expect a function argument.

operator.attrgetter(attr[, args...]) 
Return a callable object that fetches attr from its operand. If more than one attribute is requested, returns a tuple of attributes. After, f = attrgetter('name'), the call f(b) returns b.name. After, f = attrgetter('name', 'date'), the call f(b) returns (b.name, b.date). Equivalent to:

def attrgetter(*items):
    if any(not isinstance(item, str) for item in items):
        raise TypeError('attribute name must be a string')
    if len(items) == 1:
        attr = items[0]
        def g(obj):
            return resolve_attr(obj, attr)
    else:
        def g(obj):
            return tuple(resolve_att(obj, attr) for attr in items)
    return g

def resolve_attr(obj, attr):
    for name in attr.split("."):
        obj = getattr(obj, name)
    return obj
The attribute names can also contain dots; after f = attrgetter('date.month'), the call f(b) returns b.date.month.

operator.itemgetter(item[, args...]) 
Return a callable object that fetches item from its operand using the operand’s __getitem__() method. If multiple items are specified, returns a tuple of lookup values. Equivalent to:

def itemgetter(*items):
    if len(items) == 1:
        item = items[0]
        def g(obj):
            return obj[item]
    else:
        def g(obj):
            return tuple(obj[item] for item in items)
    return g
The items can be any type accepted by the operand’s __getitem__() method. Dictionaries accept any hashable value. Lists, tuples, and strings accept an index or a slice:

>>> itemgetter(1)('ABCDEFG')
'B'
>>> itemgetter(1,3,5)('ABCDEFG')
('B', 'D', 'F')
>>> itemgetter(slice(2,None))('ABCDEFG')
'CDEFG'
Example of using itemgetter() to retrieve specific fields from a tuple record:

>>> inventory = [('apple', 3), ('banana', 2), ('pear', 5), ('orange', 1)]
>>> getcount = itemgetter(1)
>>> list(map(getcount, inventory))
[3, 2, 5, 1]
>>> sorted(inventory, key=getcount)
[('orange', 1), ('banana', 2), ('apple', 3), ('pear', 5)]
operator.methodcaller(name[, args...]) 
Return a callable object that calls the method name on its operand. If additional arguments and/or keyword arguments are given, they will be given to the method as well. After f = methodcaller('name'), the call f(b) returns b.name(). After f = methodcaller('name', 'foo', bar=1), the call f(b) returns b.name('foo', bar=1). Equivalent to:

def methodcaller(name, *args, **kwargs):
    def caller(obj):
        return getattr(obj, name)(*args, **kwargs)
    return caller
10.3.1. Mapping Operators to Functions
This table shows how abstract operations correspond to operator symbols in the Python syntax and the functions in the operator module.

Operation Syntax Function 
Addition a + b add(a, b) 
Concatenation seq1 + seq2 concat(seq1, seq2) 
Containment Test obj in seq contains(seq, obj) 
Division a / b truediv(a, b) 
Division a // b floordiv(a, b) 
Bitwise And a & b and_(a, b) 
Bitwise Exclusive Or a ^ b xor(a, b) 
Bitwise Inversion ~ a invert(a) 
Bitwise Or a | b or_(a, b) 
Exponentiation a ** b pow(a, b) 
Identity a is b is_(a, b) 
Identity a is not b is_not(a, b) 
Indexed Assignment obj[k] = v setitem(obj, k, v) 
Indexed Deletion del obj[k] delitem(obj, k) 
Indexing obj[k] getitem(obj, k) 
Left Shift a << b lshift(a, b) 
Modulo a % b mod(a, b) 
Multiplication a * b mul(a, b) 
Negation (Arithmetic) - a neg(a) 
Negation (Logical) not a not_(a) 
Positive + a pos(a) 
Right Shift a >> b rshift(a, b) 
Slice Assignment seq[i:j] = values setitem(seq, slice(i, j), values) 
Slice Deletion del seq[i:j] delitem(seq, slice(i, j)) 
Slicing seq[i:j] getitem(seq, slice(i, j)) 
String Formatting s % obj mod(s, obj) 
Subtraction a - b sub(a, b) 
Truth Test obj truth(obj) 
Ordering a < b lt(a, b) 
Ordering a <= b le(a, b) 
Equality a == b eq(a, b) 
Difference a != b ne(a, b) 
Ordering a >= b ge(a, b) 
Ordering a > b gt(a, b)
```





