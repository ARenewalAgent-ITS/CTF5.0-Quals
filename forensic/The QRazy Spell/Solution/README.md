# The QRazy Spell
Author: zalv

Category: Forensic

**Flag: ARA5{t3chn0bl4d3_nev4h_d13s}**

## Description 
Technoblade's immortal spell is now lost in pieces. Help him find it!

## Difficulty 
Easy

## Solution 
From the challenge's image, we can use binwalk to check and extract the hidden file 

![image](https://github.com/ARenewalAgent-ITS/CTF5.0-Quals/blob/main/forensic/The%20QRazy%20Spell/Solution/binwalk.png)

From there, we know there’s a GIF file of QR codes. 

![gif](https://github.com/ARenewalAgent-ITS/CTF5.0-Quals/blob/main/forensic/The%20QRazy%20Spell/Solution/Pieces.gif)

If we try to scan one of them, it will give you a random string. Based on Challenge description, *"the spell is lost in pieces"* So we need to scan all of that QR to know the full string.  
To extract all GIF frames, we can use ```Graphicsmagick```

```gm convert 362E -coalesce +adjoin GIF_Frame%3d.png```

After extracted, we can automatically scan all of them with openCV-python 

```python
import cv2
import glob
import pathlib
from cv2 import *

folder = glob.glob('_TheBookOfMagick.jpg.extracted/*.png')

with open("resultQR.txt", "w") as outputFile:
    for file in sorted(folder):
        img = cv2.imread(file)
        detect =cv2.QRCodeDetector()
        data, pts, st_code = detect.detectAndDecode(img)
        outputFile.write(data)

```
From the output file, we can find a link to Mega
[https://mega.nz/file/TUVxRQpZ#AMmOgOmA86aVmk0wHrWKmMIlgvWKsfvuAleE7BilZBU]

![image](https://github.com/ARenewalAgent-ITS/CTF5.0-Quals/blob/main/forensic/The%20QRazy%20Spell/Solution/catResult.png)

After downloading that file, we’ll see a mostly black image

![image](https://github.com/ARenewalAgent-ITS/CTF5.0-Quals/blob/main/forensic/The%20QRazy%20Spell/Solution/Picture1.png)

If we apply a ```Red Plane``` filter from Stegsolve, we’ll see the hidden text which is the flag

![image](https://github.com/ARenewalAgent-ITS/CTF5.0-Quals/blob/main/forensic/The%20QRazy%20Spell/Solution/flag.png)

```ARA5{t3chn0bl4d3_nev4h_d13s}```
