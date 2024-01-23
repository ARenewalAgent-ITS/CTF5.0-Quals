# 🪟 Sekai Saikou no Ansatsusha, Isekai Kizoku ni Tensei suru

HyggeHalcyon

---

## Flag
```
ARA5{TarteMahaViekoneDia}
```

## Description
my friend gave me this cool isekai assasin game, but it's currently locked behind some **passcode**...

Flag Format: `ARA5{.*}`

## Difficulty
baby/easy

## Hints

## Tags
`C++`, `XOR`

## Notes

## Solver
- Given `LughTuathaDe.exe`, it asks for an input through the cmdline.
- reversing statically will reveal a validation function.
- it will check if our input starts with `ARA5{` and has the length of 25 which corresponds to the flag.
- it will then the string inside of `{.*}` and simply **xor** it known fixed key exist within the executable.
