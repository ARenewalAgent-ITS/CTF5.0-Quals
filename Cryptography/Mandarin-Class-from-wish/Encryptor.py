import random
from random import randint

flag = "ARA5{g00d_luck_for_y4}"

encrypted_flag = ""

key = randint(1,50000)

for ch in flag:

    e = chr(ord(ch)+key)
    encrypted_flag += e

print(key)
print(encrypted_flag)

# print(key) = ?????
# print(encrypted_flag) = "虨虹虨虜蚢蚎虗虗蚋蚆蚓蚜蚊蚒蚆蚍蚖蚙蚆蚠虛蚤"