import random

en_flags = [8, 167, 8, 118, 243, 40, 84, 118, 208, 133, 241, 141, 136, 170, 225, 118, 201, 117, 121, 218, 208, 218, 201, 40, 70, 133, 68, 133, 208, 214, 113, 189, 12] #dynamic encrypted flag
cycle = 5
block_size = 8


inv_s1 = {
    15: 0, 
    2:1, 
    14:2, 
    0:3, 
    1:4, 
    3:5, 
    10:6, 
    6:7, 
    4:8, 
    11:9, 
    9:10, 
    7:11, 
    13:12, 
    12:13, 
    8:14, 
    5:15
}

inv_s2 = {
    12:0, 
    8:1, 
    13:2, 
    6:3, 
    9:4, 
    1:5, 
    11:6, 
    14:7, 
    5:8, 
    10:9, 
    3:10, 
    4:11, 
    0:12, 
    15:13, 
    7:14, 
    2:15
}

key = [random.randrange(255), random.randrange(255)] * 4
to_bin = lambda x, n=block_size: format(x, "b").zfill(n)
to_int = lambda x: int(x, 2)
to_chr = lambda x: "".join([chr(i) for i in x])
to_ord = lambda x: [ord(i) for i in x]
bin_join = lambda x, n=int(block_size / 2): (str(x[0]).zfill(n) + str(x[1]).zfill(n))
bin_split = lambda x: (x[0 : int(block_size / 2)], x[int(block_size / 2) :])
str_split = lambda x: [x[i : i + block_size] for i in range(0, len(x), block_size)]
xor = lambda x, y: x ^ y

def inv_s(a, b):
    return inv_s1[a], inv_s2[b]

def inv_p(a):
    return a[5] + a[3] + a[1] + a[2] + a[7] + a[0] + a[4] + a[6]

def rnd_keys(k):
    return [
        k[i : i + int(block_size)] + k[0 : (i + block_size) - len(k)]
        for i in range(cycle)
    ]

def xkey(state, k):
    return [xor(state[i], k[i]) for i in range(len(state))]

def de_xkey(val,key):
    return [xor(val,key)]

def de(state,key):
    decrypted = []
    count = 0 
    for i in state:
        pe = inv_p(to_bin(i))
        a, b = bin_split(pe)
        s1, s2 = inv_s(to_int(a), to_int(b))
        step1 = bin_join((to_bin(s1, int(block_size / 2)), to_bin(s2, int(block_size / 2))))
        step2 = to_int(step1)
        if count%2 == 0:
            re = de_xkey(step2,key[0])
        if count%2 == 1:
            re = de_xkey(step2,key[1])
        count+=1
        decrypted.append(re[0])
    return decrypted

def run(en_flags,key):
    keys = rnd_keys(key)
    state = str_split(en_flags)
    for b in range(len(state)):
        for i in range(cycle):
            state[b] = de(state[b],keys[i])
    return [chr(e) for es in state for e in es]

count= 0
print("finding the key pair...please wait")
for i in range(256):
    count+=1
    for j in range(256):
        key = [i,j] * 4
        decrypted = run(en_flags,key)
        decrypted_string = ''.join(decrypted)
        if 'ARA5' in decrypted_string:
            print("Here's your flag:",decrypted_string)
            break
    else:
        continue
    break