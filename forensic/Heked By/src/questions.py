def main():
    questions = [
        {
            "question": "File mana yang mengandung suatu malware",
            "format": "/path/to/file.example"
        },
        { 
            "question": "Folder mana yang diencrypt oleh malware",
            "format": "/path/to/folder"
        },
        { 
            "question": "File apa saja yang sudah terkena malware, berikan nama file sebelum terkena encrypt",
            "format": "example.php"
        },
        { 
            "question": "Berikan address BTC yang digunakan threat actor untuk memalak victim",
            "format": "-"
        },
        { 
            "question": "Berikan email yang digunakan threat actore",
            "format": "hola@example.com"
        },
        { 
            "question": "Berikan link yang digunakan threat actor untuk mendownload malware",
            "format": "https://example.com"
        },
        { 
            "question": "Sebutkan key yang digunakan threat actor untuk melakukan encrypt data",
            "format": "[a-zA-Z0-9]"
        },
        { 
            "question": "Apa isi dari file yang telah diencrypt? Masukkan stringnya",
            "format": "-"
        },
    ]

    answers = [
        "/root/flask-api-rest/setup.py",
        "/tmp",
        "whut.txt",
        "1AvnuhyVYDbT8J7iucgvW3A3ANNa9UHYTc",
        "anjirlah332233@gmail.com",
        "https://cdn.discordapp.com/attachments/1152303649159139431/1193228819591606393/output",
        "Awikwok531921",
        "https://daffainfo/#{{ARA2025-1}}"
    ]

    print("Silahkan jawab pertanyaan-pertanyaan yang telah disediakan:")

    correct_answers = 0

    for index, q in enumerate(questions, start=1):
        print(f"\nNo {index}:")
        print("Pertanyaan: " + q["question"])
        print("Format: " + q["format"])
        user_answer = input("Jawaban: ")

        if user_answer.strip().lower() == answers[index - 1].lower():
            correct_answers += 1
            print("Correct")
        else:
            print("Incorrect")
            return
    
    if correct_answers == len(questions):
        print("\nCongrats! Flag: ARA5{g4mP4nG_l4H_y4_r3v3r53_m4Lw4R3}")

if __name__ == "__main__":
    main()