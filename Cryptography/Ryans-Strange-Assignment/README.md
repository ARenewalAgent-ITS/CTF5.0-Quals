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

```python

import sympy
from sympy import mod_inverse

# Known variable
Public_Key = "[ e, N ]"
e = 114886333760015985036554090542783661670178316083
n = 656667633925034928565265657029754592125612174887
ciphertext = [388470564545595079878104053981025526531939606859, 453176023391532805708302460105667157725589851094, 388470564545595079878104053981025526531939606859, 75802357989074313293245504745464495672586500194, 530636545397020801879048076629625949622834349271, 375102954800183654669573725068164483048779280257, 99671660668837563905250376816639356715569135661, 375102954800183654669573725068164483048779280257, 375102954800183654669573725068164483048779280257, 548590315496515548263582684646962335108239338721, 375102954800183654669573725068164483048779280257, 140887375510816447108962772482031766699016216554, 140212787491282887085498898710330206078088868768, 242179089744385364312781540147541186854680604100, 398044336768077716652000929266760922026198523016, 328163223491055229981745557826815118704798556561, 548590315496515548263582684646962335108239338721, 203670039431684285409927419369078161781353023554, 140887375510816447108962772482031766699016216554, 140212787491282887085498898710330206078088868768, 28246179230356600933428735985618279268854527152, 352317776039632073723207591355488816387781272693, 548590315496515548263582684646962335108239338721, 245693816302915231385429799263018906306181844928, 328163223491055229981745557826815118704798556561, 284701600970156838561135032032260883397153054123, 443620019394148520237590263606896913967512611950]

# Found variable
p = 750654204080680317868433
q = 874793787013089568682039
phi = (p-1)*(q-1)
d = mod_inverse(e, phi)

print("Public Key: [",e, n,"]")
print("Private Key: [",d, n,"]")
print("N is: ",n)
print("Phi is: ",phi)
print("P is: ",p)
print("Q is: ",q)

# (cph ^ d) mod n = txt
encodedtext = [pow(ch, d, n) for ch in ciphertext]
plaintext = "".join(chr(ch) for ch in encodedtext)

print(plaintext)

```

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
