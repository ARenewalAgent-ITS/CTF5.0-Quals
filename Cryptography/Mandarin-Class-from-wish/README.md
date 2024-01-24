# Mandarin-Class-from-wish

### Level  : Easy
### Task   : Reverse the function

# Solve

So here we'll see what's inside the chall.py file

![img](https://github.com/lnk7333/Mandarin-Class-from-wish/assets/156457106/71177d55-fa8a-4664-8e20-52ef12179d85)

as we see, we got information about the encrypted_flag and how it was made.
But we didn't know what the key is


### STEP 1

So what we need to do first is to examine how the code works.
From the chall.py file, we know that the key is a random number ranging from 1 to 500
```key = randint(1,500)```
and for that, we can make a code to iterate from 1 to 500

![img_2](https://github.com/lnk7333/Mandarin-Class-from-wish/assets/156457106/18ff0161-9921-4a33-9d9c-85b90613430d)


### STEP 2

The next step is to examine again what the code does
```
for ch in flag:
    e = chr(ord(ch)*key)
    encrypted_flag += e
```
So, what this code does is that it separates every character in the flag string, then ```ord(ch)``` turns the character into its base10 (decimal) representation.
and ```ord(ch)*key``` using the character earlier means that their base10 value is multiplied by any number in the key. Then we have ```chr()``` which turns the result back into its character representation and the character is joined together using ```+=```

Okay so we need to reverse those code

- First, is the ```+=``` so what i did to reverse is ```for ch in ...``` using the encrypted flag. so how this works is that we separate every character.
- Second, is the ```chr()``` so what i did to reverse is ```ord(ch)``` and what this does is we turn the encrypted characters back into its base10 (decimal) representation in order to use mathematical operation on it
- Third is the multiplication, so because it was **multiplied**, we can try to divide it back. But because chr() doesn't accept float (0,...) type value, we can use ```//``` to round down the result of the division
- Fourth, we need to return those results back to their character representation using ```chr()```.
- Now we need to combine those characters back in which I'll be using ```+=```

Here is the code i wrote to reverse

![img_3](https://github.com/lnk7333/Mandarin-Class-from-wish/assets/156457106/02d723f0-0764-41ad-92f1-59615c497658)  

Now everything is all set, you just need to ```print()``` those flags and then find the original flag
*) The flag format ```ARA5{}``` might helps you find it


### FLAG

So here's the flag you'd find

```>>> ARA5{g00d_luck_for_y4}```

*) Because we used floor division ```//``` there might be more than one flag, but both are similar

## Note

- Thanks to ARA 5.0 for letting me learn and participate
- Thanks to you who are willing to spend your time reading the instructions down until this note
- Special Thanks for everyone who helped me learn ctf and python. You guys are the best1
- Feel free to give any comments, suggestions, and critics. Thank You
