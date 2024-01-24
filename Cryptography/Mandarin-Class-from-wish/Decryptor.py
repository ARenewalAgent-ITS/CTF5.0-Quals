
encrypted_flag = "㭪䫴㭪ひ灮带⯠⯠孨囖抸櫲婾懎囖崼敶栴囖溚⾈牂"

for key in range (1,501):
    flag = ""
    for ch in encrypted_flag:
        e = chr(ord(ch)//key)
        flag += e
    
    print(key)
    print(flag)