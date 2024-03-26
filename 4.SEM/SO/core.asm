section .bss
align 8
values: resq N

section .data
align 8
    waiting times N dq N

section .text
global core
extern get_value, put_value
; Funkcja core - realizuje obliczenie na podstawie napisu p
; Argumenty:
; rdi - numer rdzenia
; rsi - wskaźnik na napis ASCIIZ
; Zwracana wartość:
; rax - wynik obliczenia

core:
        push    rbp                             ; Zachowaj poprzednią wartość rejestrow na stosie, aby pod koniec moc przywrocic ich wartosc.
        push    rbx
        push    r12
        mov     rbp, rsp                        ; Zapisz aktualny wskaźnik stosu do rejestru rbp
        mov     rbx, rsi                        ; Skopiuj wskaźnik na napis do rejestru rbx

.loop:                                          ; Etap odczytu i interpretacji kolejnych znaków napisu
        movzx   eax, byte [rbx]                 ; Wczytaj kolejny znak napisu do rejestrów niskich (8 bitów) rejestru eax
        inc     rbx                             ; Zwiększ wskaźnik na napis o 1
        test    al, al                          ; Sprawdź czy znak jest równy 0 (koniec napisu)
        jne      .b_operation                   ; Jeśli tak, zakończ działanie funkcji


.end:
        pop     rax                             ; Zapisz wynik w rejestrze rax
        mov     rsp, rbp                        ; Przywroc stos oraz rejestry do stanu przed funkcja.
        pop     r12
        pop     rbx
        pop     rbp
        ret


.b_operation:
        cmp     al, 'B'                         ; Sprawdź czy aktualny znak jest operatorem 'B'.
        jne     .c_operation                    ; Jeśli nie, sprawdź kolejny warunek.
        pop     rdx                             ; Pobierz wartość ze stosu do rejestru rdx.
        cmp     qword [rsp], 0                  ; Sprawdź czy stos ma na szczycie 0.
        je      .loop                           ; Jeśli tak, wróć do etapu odczytu i interpretacji kolejnych znaków.
        add     rbx, rdx                        ; Przesuń wskaźnik napisu o wartość z rejestru rdx.

.c_operation:
        cmp     al, 'C'                         ; Sprawdź czy aktualny znak jest operatorem 'C'.
        jne     .d_operation                    ; Jeśli nie, sprawdź kolejny warunek.
        pop     rdx                             ; Pobierz wartość ze stosu i ja porzuc.

.d_operation:
        cmp     al, 'D'                         ; Sprawdź czy aktualny znak jest operatorem 'D'.
        jne     .e_operation                    ; Jesli nie, sprawdz kolejny warunek.
        push    qword [rsp]                     ; Zduplikuj wartosc na czubku stosu.

.e_operation:
        cmp     al, 'E'                         ; Sprawdź czy aktualny znak jest operatorem 'E'.
        jne     .g_operation                    ; Jesli nie, sprawdz kolejny warunek.
        pop     rdx                             ; Zdejmij dwie gorne wartosci ze stosu
        pop     rcx
        push    rdx                             ; A nastepnie wrzuc je spowrotem w odwrotnej kolejnosci
        push    rcx
        
.g_operation:
        cmp     al, 'G'                         ; Sprawdź czy aktualny znak jest operatorem 'G'.
        jne     .p_operation                    ; Jesli nie, sprawdz kolejny warunek.
        push    rdi                             ; Wrzuc na stos rejestr rdi, w razie gdyby funkcja go nadpisala
        mov     r12, rsp                        ; Na r12 zapamietaj czy adres wierzcholka stosu jest podzielny prze 16
        and     rsp, -16                        ; W razie potrzeby, wyrownaj stos                  
        call    get_value
        mov     rsp, r12                        ; Przywroc pierwotna wartosc czubka stosu
        pop     rdi                             ; oraz rdi.
        jmp     .push_operation                 ; Wrzuc na stos wynik funkcji get_value zapisany w rax.
        
.p_operation:
        cmp     al, 'P'                         ; Sprawdź czy aktualny znak jest operatorem 'G'.
        jne     .s_operation                    ; Jesli nie, sprawdz kolejny warunek.
        pop     rsi                             ; Odczytaj wartosc czubka stosu, jeden z argumentow funkcji
        push    rdi                             ; Wrzuc na stos rejestr, w razie gdyby funkcja go nadpisala
        mov     r12, rsp                        ; Na r12 zapamietaj czy adres wierzcholka stosu jest podzielny prze 16
        and     rsp, -16                        ; W razie potrzeby, wyrownaj stos
        call    put_value
        mov     rsp, r12                        ; Przywroc pierwotna wartosc czubka stosu
        pop     rdi                             ; oraz rdi.
        jmp     .come_back

.s_operation:
        lea     r8, [rel values]                ; Oblicz adres tablicy "values" i zapisz go w rejestrze r8
        lea     rdx, [rel waiting]              ; Oblicz adres tablicy "waiting" i zapisz go w rejestrze rdx
        cmp     al, 'S'                         ; Sprawdź czy aktualny znak jest operatorem 'S'.
        jne     .add_operation                  ; Jesli nie, sprawdz kolejny warunek.
        pop     rcx                             ; Na rejestrze rcx zapisz wartoc czubka stosu - numer rdzenia z ktorym chce sie zsynchronizowac
        pop     qword [r8 + 8 * rdi]            ; Zapisz w values[n] wartosc, ktora chce oddac
        mov     [rdx + 8 * rdi], rcx            ; Przenies numer rdzenia na ktory czekam do tablicy waiting[n]

.s_loop:
        cmp     [rdx + 8 * rcx], rdi            ; Czekaj dopoki rdzen na ktorego czekam, nie bedzie czekal na mnie.
        jne     .s_loop

        push    qword [r8 + 8 * rcx]            ; Pobierz wartosc od rdzenia z ktorym sie synchronizuje
        mov     qword [rdx + 8 * rcx], N        ; oraz zaznacz ze juz skonczylem wymiane.

.sync:
        cmp     qword [rdx + 8 * rdi], N        ; Czekaj dopoki wymiana sie nie zakonczy/
        jne     .sync

.add_operation:
        cmp     al, '+'                         ; Sprawdź czy aktualny znak jest operatorem '+'.
        jne     .mul_operation                  ; Jesli nie, sprawdz kolejny warunek.
        pop     rcx                             ; Zdejmij ze stosu wartosc i dodaj ja do liczby na czukbku stosu
        add     [rsp], rcx

.mul_operation:
        cmp     al, '*'                         ; Sprawdź czy aktualny znak jest operatorem '*'.
        jne     .neg_operation                  ; Jesli nie, sprawdz kolejny warunek.
        pop     rcx                             ; Zdejmij ze stosu dwie wartosci, pomnoz je i wrzuc spowrotem na stos
        pop     rdx
        imul    rcx, rdx
        push    rcx

.neg_operation:
        cmp     al, '-'                         ; Sprawdź czy aktualny znak jest operatorem '-'.
        jne     .n_operation                    ; Jesli nie, sprawdz kolejny warunek.
        neg     qword [rsp]                     ; Zaneguj wartosc czubka stosu.

.n_operation:
        cmp     al, 'n'                         ; Sprawdź czy aktualny znak jest operatorem 'n'.
        jne     .push_operation_check           ; Jesli nie, sprawdz kolejny warunek.
        mov     rcx, rdi                        ; Wrzuc n na stos.
        push    rcx

.push_operation_check:
        sub     rax, '0'                        ; Zapisz na rax wartosc wlasciwa cyfry.
        cmp     al, 9                           ; Sprawdz czy aktualny znak to cyfra.
        ja      .come_back                      ; Jesli nie, to przejdz do kolejnego znaku.           

.push_operation:
        push    rax

.come_back:
        jmp     .loop