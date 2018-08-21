# -*-coding:utf8-*-

"""
基于字符串数字混合排序的Python脚本
"""


def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass

    try:
        import unicodedata
        unicodedata.numeric(s)
        return True
    except (TypeError, ValueError):
        pass

    return False


def find_continuous_num(astr, c):
    """

    :param astr: a string
    :param c: where to start looking for
    :return: num: int
    """
    num = ''
    try:
        while not is_number(astr[c]) and c < len(astr):
            c += 1
        while is_number(astr[c]) and c < len(astr):
            num += astr[c]
            c += 1
    except:
        pass
    if num != '':
        return int(num)


def comp2filename(file1, file2):
    """
    compare 2 filename:
    if the prev part of 2 strings are the same, compare the next continuous number
    file1 < file2 : return True, otherwise False
    :param file1:
    :param file2:
    :return:
    """
    smaller_length = min(len(file1), len(file2))
    continuous_num = ''
    for c in range(0, smaller_length):
        if not is_number(file1[c]) and not is_number(file2[c]):
            # print('both not number')
            if file1[c] < file2[c]:
                return True
            if file1[c] > file2[c]:
                return False
            if file1[c] == file2[c]:
                if c == smaller_length - 1:
                    # print('the last bit')
                    if len(file1) < len(file2):
                        return True
                    else:
                        return False
                else:
                    continue
        if is_number(file1[c]) and not is_number(file2[c]):
            return True
        if not is_number(file1[c]) and is_number(file2[c]):
            return False
        if is_number(file1[c]) and is_number(file2[c]):
            if find_continuous_num(file1, c) < find_continuous_num(file2, c):
                return True
            else:
                return False
    # if file1 < file2:
    #     return True
    # else:
    #     return False


def sort_insert(lst):
    """
    simple insert sort
    :param lst:
    :return:
    """
    for i in range(1, len(lst)):
        x = lst[i]
        j = i
        while j > 0 and lst[j-1] > x:
        # while j > 0 and comp2filename(x, lst[j-1]):
            lst[j] = lst[j-1]
            j -= 1
        lst[j] = x
    return lst


def sort_insert_filename(lst):
    """
    simple insert sort
    :param lst:
    :return:
    """
    for i in range(1, len(lst)):
        x = lst[i]
        j = i
        # while j > 0 and lst[j-1] > x:
        while j > 0 and comp2filename(x, lst[j-1]):
            lst[j] = lst[j-1]
            j -= 1
        lst[j] = x
    return lst


def file_name_sort(all_file_list):
    """

    :param all_file_list: list
    :return: new_list:list
    """
    new_list = []
    # all_file_list.sort(key=lambda x: int(x.split('.')[0].split('_')[2]))
    # for file in all_file_list:
    #     pass

    return new_list
