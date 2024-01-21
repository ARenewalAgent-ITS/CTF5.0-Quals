# 🪙 Thick Tok

aseng

---

## Flag

```
ARA5{17c189e92b1b0169914b7bb23f75e2cf51ab97955698edfc1cf155493bbc32d8}
```


## Description

This `Thick Tok` application will prompt you a **token** if you access it ONLY from their **homemade device**. 💻

If you're able to retrieve it, wrap the token with a flag format `ARA5{.*}`.

Zip password = maomaocantikmaomaokucingmaomaouwu:3

## Difficulty
easy/medium

## Hints

## Tags
`wails`, `golang`, `anti-dump`, `stripped`

## Notes

## Solver
- Given a Desktop App with a Golang backend framework that is stripped, this can be seen from Go Build ID strings.
- This desktop app is built on top of [Wails](https://wails.io/) framework and not obfuscated (~~because the author is nice~~).
- Decompiling it only shows a small and weird CF, which indicates that this executable is either packed/encrypted.
- It is packed by `UPX` but the flag section names is overwritten by **"ARA"** string, so we can recover it by reverting the original UPX header and unpacks it.
- Since the Golang executable is stripped, we can recover it first easily and find the entry point from main function.
- There's `Greet` function which triggered from Submit button of the token and it checks the user's environment variable.
- Get the original expected ENV variable by XOR-ring (~~again~~) and check from the CF that it performs SHA256 calculation from the expected PATH ENV.
- The token is in the form of SHA256 hash , but alternatively you could just patch the if condition before the calculation begins to automatically get the SHA256 hash.