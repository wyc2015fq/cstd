def ExFunc(n):
    sum = n

    def InsFunc(a):
        nonlocal sum
        sum = sum+a
        return sum+a
    return InsFunc


myFunc=ExFunc(10)
print(myFunc(1))

myFunc2=ExFunc(20)
print(myFunc2(2))
print(myFunc(1))
print(myFunc2(2))
print(myFunc(1))
