def get_number(columns, bias):
    number = 0
    for i in range(0, 8):
        part = columns[i + bias]
        multiplier = 16 ** (2 * i)
        number += (int(part, 16) * multiplier)
    return number


def read_data(filename):
    # Inicjalizacja pustej tablicy qword
    qword_array = []

    # Otwarcie pliku
    with open(filename, 'r') as file:
        for line in file:
            columns = line.split()
            qword_array.append(get_number(columns, 2))
            qword_array.append(get_number(columns, 10))

    return qword_array


def break_password(qword, hash8):
    # Inicjalizacja pustej tablicy password
    password = [0 for i in range(0, 9)]
    id = [0 for i in range(0, 9)]
    hash = [0 for i in range(0, 9)]
    hash[8] = hash8
    for i in range(8, 0, -1):
        for j in range(0, 256):
            if (qword[j] & 0xff00000000000000) == (hash[i] & 0xff00000000000000):
                id[i] = j
                pom = qword[j] ^ hash[i]
                hash[i - 1] = pom << 8
    hash[0] = 0
    for i in range(1, 9):
        password[i] = id[i] ^ (hash[i-1] & 0xFF)
        hash[i] = (hash[i-1] >> 8) ^ qword[id[i]]

    return password


filename = 'dane.txt'
hash8 = 0x3e4c3f5bdf80e7d3
qword = read_data(filename)
password = break_password(qword, hash8)
for i in range(1, 9):
    print(chr(password[i]), end='')
