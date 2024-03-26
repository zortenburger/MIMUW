#!/usr/bin/env python3
from pwn import *

import utils


# 438646


def xor(a, b):
    prefix = bytes([ac ^ bc for ac, bc in zip(a, b)])
    return prefix + a[len(prefix):] + b[len(prefix):]


def conn(hostname, port):
    r = remote(hostname, port)
    for _ in range(5):
        r.recvline()
    r.recvn(2)
    return r


def check_a(a, s, m, i):
    xor = (1 << i) - 1
    for j in range(1, 5):
        for k in range(j + 1, 5):
            lewa = s[j] ^ s[k]
            prawa = (s[j - 1] * a) ^ (s[k - 1] * a)
            if lewa & xor != prawa & xor:
                return False
    return True


def find_a(s, m):
    possible_a = [0]
    for i in range(64):
        possible_a2 = []
        for a in possible_a:
            a1 = a + (1 << i)
            a2 = a
            if check_a(a1, s, m, i):
                possible_a2.append(a1)
            if check_a(a2, s, m, i):
                possible_a2.append(a2)

        possible_a = possible_a2

    return possible_a


def find_c(s, m, a):
    c = (s[1] ^ (s[0] * a)) % m
    for i in range(2, 5):
        c1 = (s[i] ^ (s[i - 1] * a)) % m
        if c1 != c:
            return None
    return c


def ncg(hostname, port):
    r = conn(hostname, port)
    r.send(bytes("1\n", 'ascii'))

    m = 2 ** 64
    s = [0 for _ in range(5)]

    for i in range(5):
        s[i] = int(r.recvline().decode().strip())
    r.recvline().decode().strip()

    possible_a = find_a(s, m)

    possible_result = []
    for a in possible_a:
        c = find_c(s, m, a)
        if c is None:
            continue
        res = (s[4] * a ^ c) % m
        if res not in possible_result:
            possible_result.append(res)

    if len(possible_result) == 1:
        r.send(
            bytes(str(possible_result[0]) + "\n", 'ascii')
        )

        FLAGA = r.recvline().decode().strip()
        print(FLAGA)
    else:
        print("Nie znaleziono jednoznacznego wyniku, uruchom jeszcze raz")

    # r.interactive()


def get_xored(base_msg, base_cypher, msg):
    new_msg = xor(xor(base_msg, msg), base_cypher)
    new_msg = new_msg + base_cypher[len(new_msg):]
    return new_msg


def send_xored(base_msg, base_cypher, msg, r):
    new_msg = get_xored(base_msg, base_cypher, msg)
    r.sendline(new_msg.hex().encode())


def send_and_recv_xored(base_msg, base_cypher, msg, r):
    send_xored(base_msg, base_cypher, msg, r)
    return bytes.fromhex(r.recvline().strip().decode())


def block_cipher1(hostname, port):
    r = conn(hostname, port)
    r.send(bytes("2\n", 'ascii'))

    msghello = b"Hello"
    cypherhello = bytes.fromhex(r.recvline().decode().strip())

    r.sendline(cypherhello.hex().encode())
    msgunknown = b"unknown command\x01"
    cypherunknown = bytes.fromhex(r.recvline().strip().decode())
    cypherunknown = cypherunknown[:32]

    msgflag = b"flag?"
    flagcypher = send_and_recv_xored(msghello, cypherhello, msgflag, r)
    flagcypher = flagcypher[:32]

    msghash = b"hash?"
    hashcypher = send_and_recv_xored(msghello, cypherhello, msghash, r)

    flaga = b"flag{" + b"\x00" * 11

    # zgadnij ostatni znak
    for i in range(256):
        guess = b"flag{" + b"\x00" * 10 + bytes([i])
        help = b"hash?" + b"\x00" * 10 + b"\x0b"

        send_xored(guess, flagcypher, help, r)

    for i in range(256):
        guess = b"flag{" + b"\x00" * 10 + bytes([i])
        xored = bytes.fromhex(r.recvline().strip().decode())
        if xored == hashcypher:
            flaga = guess

    for i in range(5, 15):
        for j in range(256):
            guess = flaga[:i] + bytes([j]) + b"\x00" * (14 - i) + flaga[-1:]
            help = b"hash?" + guess[5:i] + b"\x00" * (15 - i) + bytes([15 - i])

            send_xored(guess, flagcypher, help, r)
            send_xored(help, cypherunknown, msgunknown, r)

        for j in range(256):
            xored = bytes.fromhex(r.recvline().strip().decode())
            hashletter = bytes.fromhex(r.recvline().strip().decode())
            guess = flaga[:i] + bytes([j]) + b"\x00" * (14 - i) + flaga[-1:]

            if xored == hashletter:
                flaga = guess

    print(flaga.decode())


def helper1(flagcypher, cypherunknown, msgunknown, dlugosc, flaga, r):
    for i in range(5, 15):
        flagcypher = flagcypher[:-17] + bytes([dlugosc ^ flagcypher[-17] ^ (dlugosc - 1)]) + flagcypher[-16:]
        dlugosc -= 1
        for j in range(256):
            guess = flaga + bytes([j])
            help = b"hash?" + guess[5:i+1]

            help2 = b"hash?" + guess[5:i+1] + b"\x00" * (14 - i) + bytes([15 - i])

            send_xored(guess, flagcypher, help, r)
            send_xored(help2, cypherunknown, msgunknown, r)

        for j in range(256):
            recv = bytes.fromhex(r.recvline().strip().decode())
            hashletter = bytes.fromhex(r.recvline().strip().decode())

            if recv == hashletter:
                flaga = flaga + bytes([j])

    return flaga, dlugosc, flagcypher


def helper2(flagcypher, cypherunknown, msgunknown, dlugosc, flaga, r):
    wanted = bytes(" ", "ascii") * 10 + b"hash?"
    print("dupa")
    for i in range(6):
        flagcypher = flagcypher[:-17] + bytes([dlugosc ^ flagcypher[-17] ^ (dlugosc - 1)]) + flagcypher[-16:]
        dlugosc -= 1
        print(dlugosc)
        for j in range(256):
            guess = flaga + bytes([j])
            help = wanted + guess[-(i + 1):]
            # print(len(help), len(guess))
            # print(help, guess, xor(help, guess))

            help2 = b"hash?" + guess[-(i + 1):] + b"\x00" * (9 - i) + bytes([10 - i])
            print(len(help2))

            assert len(help) == len(guess)
            send_xored(guess, flagcypher, help, r)
            send_xored(help2, cypherunknown, msgunknown, r)

        for j in range(256):
            recv = bytes.fromhex(r.recvline().strip().decode())
            hashletter = bytes.fromhex(r.recvline().strip().decode())

            if recv == hashletter:
                flaga = flaga + bytes([j])
    return flaga, dlugosc, flagcypher


def block_cipher2(hostname, port):
    r = conn(hostname, port)
    r.send(bytes("2\n", 'ascii'))

    msghello = b"Hello"
    cypherhello = bytes.fromhex(r.recvline().decode().strip())

    r.sendline(cypherhello.hex().encode())
    msgunknown = b"unknown command\x01"
    cypherunknown = bytes.fromhex(r.recvline().strip().decode())

    msgflag = b"FLAG!"
    flagcypher = send_and_recv_xored(msghello, cypherhello, msgflag, r)

    msghash = b"hash?"
    hashcypher = send_and_recv_xored(msghello, cypherhello, msghash, r)

    dlugosc = len(flagcypher) - 16 -5
    flagcypher = flagcypher + b"\x00" * 32

    flaga = b"flag{"
    help = b"hash?"

    xored = get_xored(flaga, flagcypher, help)
    # zgadnij dlugosc
    for i in range(256):
        xored = xored[:-17] + bytes([i]) + xored[-16:]
        r.sendline(xored.hex().encode())

    for i in range(256):
        recv = bytes.fromhex(r.recvline().strip().decode())
        if recv == hashcypher:
            flagcypher = flagcypher[:-17] + bytes([i]) + flagcypher[-16:]

    flaga, dlugosc, flagcypher = helper1(flagcypher, cypherunknown, msgunknown, dlugosc, flaga, r)
    flaga, dlugosc, flagcypher = helper2(flagcypher, cypherunknown, msgunknown, dlugosc, flaga, r)

    flagcypher = flagcypher[16:-32]
    flaga_main = flaga
    flaga = flaga_main[16:]

    dlugosc = len(flagcypher) - 16 - 5
    flagcypher = flagcypher + b"\x00" * 32

    xored = get_xored(flaga, flagcypher, help)
    for i in range(256):
        xored = xored[:-17] + bytes([i]) + xored[-16:]
        r.sendline(xored.hex().encode())

    for i in range(256):
        recv = bytes.fromhex(r.recvline().strip().decode())
        if recv == hashcypher:
            flagcypher = flagcypher[:-17] + bytes([i]) + flagcypher[-16:]

    flaga, dlugosc, flagcypher = helper1(flagcypher, cypherunknown, msgunknown, dlugosc, flaga, r)
    flaga, dlugosc, flagcypher = helper2(flagcypher, cypherunknown, msgunknown, dlugosc, flaga, r)

    flaga_main = flaga_main + flaga[5:]



    print(flaga_main.decode())



    r.interactive()


def main(hostname, port):
    # ncg(hostname, port)
    # block_cipher1(hostname, port)
    block_cipher2(hostname, port)


if __name__ == "__main__":
    hostname = sys.argv[1]
    port = int(sys.argv[2])

    main(hostname, port)
