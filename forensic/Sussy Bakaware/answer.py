def main():

    # Soal 1

    soal1 = input(
        "1. What the IP and DNS that host the malware? (ip_domain)\n")
    if soal1 == "46.4.205.200_mimsmehediclub.com":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 2

    soal2 = input("2. IP Address that has been infected?\n")
    if soal2 == "10.1.12.101":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 3

    soal3 = input(
        "3. What is the request token when the malware initiated the connection to the CnC?\n")
    if soal3 == "f960cc969e79d7b100652712b439978f789705156b5a554db3acca13cb298050efa268fb":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 4

    soal4 = input("4. The filename of malware? (xxx.xxxx.redacted_xxxx.ext)\n")
    if soal4 == "att.file.downloaded_1914.zip":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 5

    soal5 = input(
        "5. Arrival or timestamp of malware? (UTC Format, YYYY-MM-DD HH:MM:SS UTC)\n")
    if soal5 == "2024-01-12 20:34:43 UTC":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 6

    soal6 = input("6. Malware family labels? (format: lowercase, fam1_fam2)\n")
    if soal6 == "calisto_sload":
        print("Correct")
    elif soal6 == "sload_calisto":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 7

    soal7 = input("7. SHA-256 of malware?\n")
    if soal7 == "7acaa1011452c0d1a72dd162a8d78e07fbe0cce56276a937eacff119aa39da83":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 8

    soal8 = input("8. What the computer name of victim? (xxxxxxx-xxxxxPC)\n")
    if soal8 == "DESKTOP-WIN11PC":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 9

    soal9 = input(
        "9. What the frame number of the stealer capture the desktop victim?\n")
    if soal9 == "5824":
        print("Correct")
    else:
        print("Try Again")
        return

    # Soal 10

    soal10 = input(
        "10. What the function name that has loaded command for the malware\n")
    if soal10 == "_0x3cef":
        print("ARA5{1t5_4ll_4b0ut_4tt3nt10n_th3_M4lW4r3_1nv3st1g4t0r_0x69a221}")
    else:
        print("Try Again")
        return


if __name__ == "__main__":
    main()
