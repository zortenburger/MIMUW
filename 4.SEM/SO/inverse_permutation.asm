MAX_VAL equ     0x7FFFFFFF                      ; Maksymalna wartość jaką może przyjować n-1.

global inverse_permutation

inverse_permutation:                            ; Parametry funkcji dostajemy odpowiednio w rejestrach:
                                                ;   rdi - size_t n - rozmiar tablicy,
                                                ;   rsi - int *p   - wskaznik na tablice.

.check_n:                                       ; Sekcja sprawdzająca poprawność n - powinno znajdować się w zakresie (0; MAX_VAL+1).
        test    rdi, rdi                        ; Testujemy czy n jest dodatnie,
        jz      .end_false                      ; w przeciwnym wypadku skaczemy do zakończenia funkcji z wynikiem false.

        dec     rdi                             ; Zmniejszamy n o jeden.
        cmp     rdi, MAX_VAL                    ; Testujemy czy n - 1 jest mniejsze od swojej maksymalnej wartosci,
        ja      .end_false                      ; w przeciwnym wypadku skaczemy do zakonczenia funkcji z wynikiem false.

.check_values:                                  ; Sekcja sprawdzająca poprawność danych w tablicy.
                                                ; Wartości powinny być w zakresie [0, n-1].
                                                ; Zaznaczam "odwiedzenia", używając najstarszego, nieużywanego bitu każdej liczby w tablicy.
                                                ; Przechodzę przez tablicę dopóki nie znajdę błędu,
                                                ; błędne dane to liczba większa niż n-1 (po odkodowaniu)
                                                ; lub próba podwójnego odwiedzenia tej samej liczby albo odwiedzenie liczby ujemnej.
                                                ;   ecx - licznik w pętli, indeks w tablicy
                                                ;   eax - aktualnie sprawdzana liczba
                                                ;   edx - wartość modyfikowana w celu zaznaczenia "odwiedzenia"
                                                ;   edi - maksymalna dozwolona wartość - n-1
                                                ;   rsi - wskaźnik na pierwszy element tablicy

        xor     ecx, ecx                        ; Zeruje licznik pętli.
.check_values_loop:
        mov     eax, DWORD [rsi + rcx * 4]      ; Kopiuje kolejne wartości z tablicy do rejestru eax.
        test    eax, eax                        ; Odzyskuje oryginalną wartość z tablicy, ignorując zaznaczone odwiedzenie.
        jns     .check_unsigned
        not     eax

.check_unsigned:                                ; W tym miejscu wartość eax jest na pewno dodatnia.
        cmp     eax, edi                        ; Sprawdzam górne ograniczenie na eax,
        ja      .reverse_changes                ; gdy jest przekroczone skaczę do sekcji cofającej naniesione zmiany.

        mov     edx, DWORD [rsi + rax * 4]      ; Sprawdzam czy aktualna wartość została juz odwiedzona 
        test    edx, edx                        ; (lub wartość pod jej indeksem jest ujemna),       
        js      .reverse_changes                ; jeśli tak skacze do sekcji cofającej zmiany.

        not     DWORD [rsi + rax * 4]           ; W przeciwnym wypadku zaznaczam "odwiedzenie".

        inc     ecx                             ; Przechodzę do kolejnego elementu w tablicy.
        cmp     ecx, edi
        jbe     .check_values_loop

        jmp     .inverse                        ; Jeśli przeszłam całą tablicę bez znalezienia błędu przechodzę 
                                                ; do sekcji odwracania permutacji.

.reverse_changes:                               ; Sekcja cofająca zmiany naniesione przy sprawdzaniu poprawności wejścia,
                                                ; gdy znaleziono błąd przechodzę tablicę od początku do miejsca, w którym znalazłam błąd.
                                                ; Odwracam wszystko co zmodyfikowałam zauważając, że podwójna negacja
                                                ; wraca do stanu początkowego.
                                                ;   ecx - pierwszy niezmodyfikowany indeks w tablicy
                                                ;   edx - licznik w pętli, aktualny indeks w tablicy
                                                ;   eax - aktualnie sprawdzana liczba
                                                ;   rsi - wskaźnik na pierwszy element tablicy

        xor     edx, edx                        ; Zeruje licznik pętli.
.reverse_changes_loop:
        cmp     edx, ecx                        ; Sprawdzam czy doszłam do pierwszego niezmodyfikowanego indeksu,
        je      .end_false                      ; jeśli tak skaczę do zakończenia funkcji z wynikiem false.

        mov     eax, DWORD [rsi + rdx * 4]      ; W przeciwnym wypadku kopiuje aktualnie sprawdzaną wartość do rejstru eax.
        test    eax, eax                        ; Odzyskuje oryginalną wartość z tablicy, ignorując zaznaczone odwiedzenie.
        jns     .reverse_changes_unsigned
        not     eax

.reverse_changes_unsigned:
        not     DWORD [rsi + rax * 4]           ; Cofam modyfikacje którą wykonałam w poprzedniej sekcji.
        inc     edx                             ; Przechodzę do kolejnego elementu tablicy.
        jne     .reverse_changes_loop
                                                ; Po zakończeniu sekcji cofającej modyfikacje zawsze przechodzę 
                                                ; do sekcji kończącej program z wynikiem false.

.end_false:                                     ; Sekcja kończąca program w wynikiem false.
        xor     eax, eax
        ret

.inverse:                                       ; Sekcja odwracająca permutację.
                                                ; Przechodzę "po cyklach" wstawiając elementy na miejsce,
                                                ; w którym powinny się znaleźć po odwróceniu permutacji.
                                                ; Dodatkowo w każdej z liczb zgaszam bit zaznaczony przy sprawdzaniu poprawności wejścia.
                                                ; Liczby dodatnie to liczby na właściwej pozycji.
                                                ;   edx - licznik w pętli, aktualny indeks w tablicy
                                                ;   eax - aktualnie sprawdzana liczba
                                                ;   ecx - aktualny indeks w cyklu
                                                ;   r8d - aktualna wartość w cyklu
                                                ;   rsi - wskaźnik na pierwszy element tablicy

        xor     edx, edx                        ; Zeruje licznik pętli.
.inverse_loop:
        mov     eax, DWORD [rsi + rdx * 4]      ; Kopiuje aktualnie sprawdzaną wartość do rejestru eax,

        test    eax, eax                        ; jeśli jest nieujemna
        jns     .inverse_loop_end               ; to skacze do zakonczenia petli.

        mov     ecx, edx                        ; Zapisuje aktualny indeks.
.inverse_while:
        not     eax                             ; Cofam modyfikacje ze sprawdzania,
        mov     r8d, DWORD [rsi + rax * 4]      ; zapisuje wartosc z miejsca, w które zaraz 
        mov     DWORD [rsi + rax * 4], ecx      ; wstawie poprawna wartosc.

        mov     ecx, eax                        ; Przenosze wartosci do odpowiednich rejestrow.
        mov     eax, r8d
        cmp     edx, ecx                        ; Sprawdzam czy skonczyłam cykl (doszłam do jego początku),
        jne     .inverse_while                  ; jesli nie skacze do kolejnego elementu na cyklu.

.inverse_loop_end:                              ; Przechodze do kolejnego elementu tablicy.
        inc     edx
        cmp     edx, edi
        jbe     .inverse_loop
                                                ; Po zakończeniu sekcji odwracajacej permutacje 
                                                ; zakańczam program z wynikiem true.

.end_true:                                      ; Sekcja kończąca program z wynikiem true
        xor     eax, eax
        inc     eax     
        ret