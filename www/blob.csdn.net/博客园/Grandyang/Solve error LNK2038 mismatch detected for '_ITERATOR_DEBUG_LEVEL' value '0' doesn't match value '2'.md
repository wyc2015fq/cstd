# Solve error LNK2038: mismatch detected for '_ITERATOR_DEBUG_LEVEL': value '0' doesn't match value '2' - Grandyang - 博客园







# [Solve error LNK2038: mismatch detected for '_ITERATOR_DEBUG_LEVEL': value '0' doesn't match value '2'](https://www.cnblogs.com/grandyang/p/4449120.html)







This error happens in Release mode of VS2010, solve this problem by do following:

.

Go to Project Pages / Configuration Properties / C,C++ / Preprocessor / Preprocessor Definitions, add the following:

**_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH**












