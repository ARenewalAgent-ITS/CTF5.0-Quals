# 🪟 Sekai Saikou no Ansatsusha, Isekai Kizoku ni Tensei suru 2nd Season

HyggeHalcyon

---

## Flag
```
ARA5{very_kind_of_scuffed_i_barely_learned_dll_injection}
```

## Description
thank you for helping me unlocking the game <3 
but the game closes and does nothing .... or is it?

Flag Format: `ARA5{.*}`  
Note: you might need to run things in Administrator mode to get things working as expected

## Difficulty
medium/hard

## Hints

## Tags
`C++`, `Windows 32 API`, `DLL Injection`, `Shared Memory Region`

## Notes

## Solver
TLDR;
- Flag is encrypted and stored within the Executable
- DLL will then attempt to decrypt it and write it into a Global Shared Memory.
- Open `mspaint.exe` as Administrator. This will be the victim process which the DLL will be injected into.
- Run `LughTuathaDe.exe` as Administrator
- Enter the correct code (i.e. Flag from the first stage)
- While those two process is running, write another program which will read the Shared Memory and output the result (PoC: `src/Solve/Solve.cpp`). 

Executable POV
- Given `LughTuathaDe.exe` it will prompt for an input just like last time. However if the flag was correct, it will do much more in the background (read how to solve the first Stage of the challenge for more context).
- Referring to the same validation function as before, beside `XOR`ing our input, it also does the same to a to another global variable with the same key. Results in `mspaint.exe` and stores it in a READWRITE section of the executable. This will be the `Victim Process`.
- Next, it will try to find the `dll` (i.e. `TotallyNotMalicious.dll`) inside of the same relative directory of the executable.
- Then it will then try to find the if the `Victim Process` (i.e. `mspaint.exe`) is running and try to grab the PID of it. 
- If it found the PID, it will then Allocate a memory in the name of the `Victim Process` and write the dll string to it. 
- With that in mind, it can then `Inject the dll` by creating a `new Thread` in the name of the `Victim Process` specifying the dll name previously written into their memory. If all of this goes without any error, you will notice the executable will output "Injected" to the console. 
- At the same time, the executable will create a new [Shared Memory](https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory). This is used to share memory across procesess and will be used by the dll to decrypt the Flag. What will be allocated to this memory section is the content of `{.*}` from the first flag.

DLL POV:
- Given `TotallyNotMalicious.dll`, it is a Windows Shared Library.
- In Windows, Libaries can also have a Main Entry point which is [DllMain](https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain).
- When this dll is attached to a process, it will a call a function which basically a `Win` function. 
-  It will attempt to decrypt the encrypyed flag which is stored and can be viewed statically within the Executable. 
- The flag itself is encrypted using AES. The Key and the Ciphertext itself can be grabbed by simply reading the raw Executable at a certain offset. 
- The IV However, only exist if the first flag was correct, which also then will be written to A Shared Memory (`Global\\Harem`) previously created by the Executable and read by the dll.
- It will then write the decrypted to another Shared Memory Region (`Global\\Flag`).