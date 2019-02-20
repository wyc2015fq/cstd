
from datetime import date
from datetime import timedelta
from identity import *

from gen_utils import *
from allcharset import *

def get_check_digit(id):
    """通过身份证号获取校验码"""
    check_sum = 0
    for i in range(0, 17):
        check_sum += ((1 << (17 - i)) % 11) * int(id[i])
    check_digit = (12 - (check_sum % 11)) % 11
    return check_digit if check_digit < 10 else 'X'


id = '132261198811160681'
c = get_check_digit(id)
print(c)
