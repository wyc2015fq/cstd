import os


def find2():
    for a in range(1, 10):
        for b in range(1, 10):
            k = b * 10 + a
            for i in range(2, 10):
                z = i * k
                if z < 99:
                    a0, b0 = z % 10, z // 10
                    # print(a, b, i, a0,b0,z)
                    if set([a, b]) == set([a0, b0]):
                        print(k)


def find3():
    for a in range(1, 10):
        for b in range(1, 10):
            for c in range(1, 10):
                k = c * 100 + b * 10 + a
                for i in range(2, 10):
                    z = i * k
                    if z < 999:
                        a0, b0, c0 = z % 10, (z // 10) % 10, z // 100
                        # print(a, b, i, a0,b0,z)
                        if set([a, b, c]) == set([a0, b0, c0]):
                            print(k)


def find4():
    for a in range(1, 10):
        for b in range(1, 10):
            for c in range(1, 10):
                for d in range(1, 10):
                    k = d * 1000 + c * 100 + b * 10 + a
                    for i in range(2, 10):
                        z = i * k
                        if z < 9999:
                            a0, b0, c0, d0 = z % 10, (z // 10) % 10, (z // 100) % 10, (z // 1000)
                            # print(a, b, i, a0,b0,z)
                            if set([a, b, c, d]) == set([a0, b0, c0, d0]):
                                print(k, i, z)


def find5():
    for a in range(1, 10):
        for b in range(1, 10):
            for c in range(1, 10):
                for d in range(1, 10):
                    k = d * 1000 + c * 100 + b * 10 + a
                    for i in range(2, 10):
                        z = i * k
                        if z < 9999:
                            a0, b0, c0, d0 = z % 10, (z // 10) % 10, (z // 100) % 10, (z // 1000)
                            # print(a, b, i, a0,b0,z)
                            if set([a, b, c, d]) == set([a0, b0, c0, d0]):
                                print(k, i, z)


def getallnum(k):
    nums = []
    while k > 0:
        nums.append(k % 10)
        k = k // 10

    return nums


def find(n):
    ret = {}
    for k in range(1, n):
        k1 = getallnum(k)
        if k1[-1] < 5:
            k1.sort()
            sk1 = set(k1)
            for i in range(2, 10):
                z = k * i
                z1 = getallnum(z)
                if len(k1) == len(z1) and sk1 == set(z1) and len(sk1) == len(k1):
                    # print(k, i, z)
                    ss = str(sk1)
                    if ss not in ret:
                        ret[ss] = []

                    ret[ss].append([k, i, z])

    return ret


rr = find(10000000)

for i in rr:
    print(i, rr[i])
