# Ryan-s_Strange_Assignment

### Level      : Easy
### Task       : Crack the code

# Solve

So here we'll see what's inside the chall.txt file

![img](https://github.com/lnk7333/Ryan-s_Strange_Assignment/assets/156457106/71e9a4fc-ed9a-42f8-977d-dab624457b7f)

as we see we have the information about N, e, Ciphertext


### STEP 1

so what we need to do first is to find the P and Q
and we can use [factordb](http://factordb.com) just for that

![img_1](https://github.com/lnk7333/Ryan-s_Strange_Assignment/assets/156457106/6e6d1448-ab87-488b-b354-3ce33bae75e9)

here you'd want to put the number you want to factor, in this case its the N
so after we factor it, we got two long number which is the P and the Q


### STEP 2

Now for the next step is that we must find the phi(N) using [Euler's Totient Function](https://www.doc.ic.ac.uk/~mrh/330tutor/ch05s02.html#:~:text=Euler's%20theorem%20generalises%20Fermat's%20theorem,is%20the%20Euler's%20totient%20function.)
so basically because N = p*q
then the totient function works like this aswell

#### phi(N) = phi(p) * phi(q)

and because P and Q are prime numbers
therefore
#### phi(N) = (p-1)*(q-1)
now we got the phi(N) which i'll refer as phi in the next step


### STEP 3

So after we got the phi, we just need to find one more variable which is 'd'
and we can use [Euler's Theorem](https://www.doc.ic.ac.uk/~mrh/330tutor/ch05s02.html#:~:text=Euler's%20theorem%20generalises%20Fermat's%20theorem,is%20the%20Euler's%20totient%20function.) for this
so here i'll need to reverse the exponent and modulus function to get 'd'

and for that i use sympy library for the mod_inverse()

![img_2](https://github.com/lnk7333/Ryan-s_Strange_Assignment/assets/156457106/cda146e4-6646-4954-ab70-e2ee2eacb746)

here as you can see i form this code, i wrote ```d = mod_inverse(e, phi)``` which will return d


### STEP 4

Now after we got all the variables we needed, we just need to decrypt the Ciphertext by reversing the modular exponentiation.
So the flag was decrypted using ```ciphertext = (plaintext ^ e) mod N```. 
*) the symbol ```^``` stands for "to the power of ... "

So how you might reverse it you ask?
Basically to reverse it, you use ```plaintext = (ciphertext ^ d) mod N```.
Notice that we use Ciphertext to the power of d to reveal the encodedtext (because the plaintext was decoded first so we will need to do something before it).

so we use the function ```encodedtext = [pow(ch, d, N) for ch in ciphertext]```
And that's how you reverse the function.

But wait, because we put the encodedtext in lists, we gotta use chr() to decode it and join it with the plaintext to create a string
so we use 

```plaintext = "".(chr(ch) for ch in encodedtext)```


### FLAG

Now we got everything done. we'll run the code and reveal the flag

``` >>> ARA5{y4yy_y0u've_f0uNd_me!} ```


## Note

- Thanks to ARA 5.0 for letting me learn and participate
- Thanks to you who are willing to spend your time reading the instructions down until this note
- Special Thanks for everyone who helped me learn about this topic
- Feel free to give any comments, suggestions, and critics. It means a lot to me
