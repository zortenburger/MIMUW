# Bezpieczeństwo Sieci Komputerowych
## Inżynieria wsteczna - sprawozdanie z zadania
### Autor: Zuzanna Ortenburger (438646)

1. **Odmaskowanie hasła** \
**Zadanie:** Gra rozpoczyna się próbą uruchomienia komputera. Zmodyfikuj grę, aby
przy próbie wpisania hasła do dysku zamiast gwiazdek wyświetlały się znaki hasła.
Opisz w sprawozdaniu w jaki sposób dokonano modyfikacji. \
**Rozwiązanie:** Żeby odmaskować hasło, sprawdziłam kolejno funkcje: `pc_keypress`, `pc_putch` co 
doprowadziło mnie do miejsca w kodzie gdzie znalazłam znak `*` wypisywany na ekranie. Aby zastąpić
go znakiem hasła, musiałam znaleźć miejsce w pamięci, gdzie jest przechowywany znak hasła.
Abby to zrobić ustawiłam breakpoin w miejscu podmieniania na gwiazdkę i przejrzałam zawartość rejestrów.
W rejestrze r9 znalazłam odpowiadajacy znak. Następmnie podmieniłam znak gwiazdki na 
pasującą rozmiarem cześć rejestru r9 zawierającą znak hasła. 

2. **Jak sprawdzane jest hasło do dysku?**\
**Zadanie:** Gra weryfikuje poprawność wprowadzonego hasła do dysku. W sprawozdaniu opisz algorytm, za pomocą którego hasło jest weryfikowane. Na podstawie
zebranych informacji, odnajdź poprawne hasło. \
**Algorytm weryfkacji hasła:** Algorytm weryfikacji hasła składa się z dwóch kroków.
Pierwsza z nich zaimplementowana jest w funkcji `pc_check_luks_password`, gdzie sprawdzana jest najpierw długość, która musi 
wynosić 8. Następnie sprawdzana jest obecnosc liter w haśle, zakodowana na zmiennej. W naszym przypadku litery znajdowaly sie na pozycjach
pierwszej, piątej i ósmej. Po sprawdzeniu tych warunków przechodzimy do drugiego kroku
weryfikacji hasła - funkcji haszującej `hash_pasword`, która haszuje hasło a następnie sprawdza czy hash odpowiada
hashowi prawidłowego hasła. Aby odnaleźć poprawne hasło, musiałam zrozumieć działanie funkcji hashującej, a następnie stworzyć 
skrypt odwracający to działanie.\
**Algorytm hashujący:** Aby łatwiej opisać algorytm pomocniczo zdefiniuje trzy tablice:
`qword`, `hash`, `id`. Tablica `qword` zawiera pewne mapowanie z chara na liczbę 64 bitową. Jej zawartość 
wyciągnęłam z pamieci programu otrzemując plik postaci:
```csv
$==>             00007FF736849730     00 00 00 00 00 00 00 00 79 89 35 30 C8 70 D8 7A       ........y.50ÈpØz
$+10             00007FF736849740     F2 12 6B 60 90 E1 B0 F5 8B 9B 5E 50 58 91 68 8F       ò.k`.á°õ..^PX.h.                                                        
$+20             00007FF736849750     8F B6 41 98 73 E5 38 C0 F6 3F 74 A8 BB 95 E0 BA       .¶A.så8Àö?t¨».àº                                                        
$+30             00007FF736849760     7D A4 2A F8 E3 04 88 35 04 2D 1F C8 2B 74 50 4F       }¤*øã..5.-.È+tPO                                                        
$+40             00007FF736849770     75 FE 14 68 B4 EC 28 AB 0C 77 21 58 7C 9C F0 D1       uþ.h´ì(«.w!X|.ðÑ                                                        
...
```
Dane wczytałam do tablicy `qword` w następujący sposób:
```python
def get_number(columns, bias):
    number = 0
    for i in range(0, 8):
        part = columns[i + bias]
        multiplier = 16 ** (2 * i)
        number += (int(part, 16) * multiplier)
    return number


def read_data(filename):
    qword_array = []
    with open(filename, 'r') as file:
        for line in file:
            columns = line.split()
            qword_array.append(get_number(columns, 2))
            qword_array.append(get_number(columns, 10))

    return qword_array
```
Tablica `hash` zawier hash prefixu hasła, pózniej sprawdzamy czy `hash[8] == 0x3e4c3f5bdf80e7d3` i jeśli tak to znaczy, że hasło jest poprawne.
Tablica `id` to tablica pomocnicza, zawerajaca indeksy w tablicy `qword` potrzebne dla odpowiednich hash-y.\
Algorytm hashujący działa następująco (kod w pseudokodzie):

```pseudo
hash[0] = 0
for i in range(0, 8):
    id[i] = password[i] ^ hash[i]
    hash[i + 1] = (hash[i] >> 8) ^ qword[id[i]]
```
Aby odwrócić działanie tego algorytmu, musiałam znaleźć odwrotność operacji wykonywanych w nim, poniższy skrypt
odkodowuje hasło posiadajac tablice `qword` i `hash[8]`:
```python
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
```
Po uruchomieniu skryptu otrzymałam **hasło**:`p455w04d`

3. **Mame nie teraz, są nowe poszlaki** \
**Zadanie:** Ciągłe zmuszanie do uruchomienia komputera przez mamę za każdym restartem gry jest nieco irytujące, prawda? Zmodyfikuj grę w taki sposób, by jej początkowy stan był taki jak po próbie uruchomienia komputera z mamą. Opisz w jaki
sposób ustalana jest aktualna ”faza” gry. \
**Rozwiązanie:** Aby ustalić aktualną fazę gry, zaczęłam od przejrzenia funkcji `SDL_main`, co mnie zaciekawiło, to po wykonaniu 
ciągu funkcji postaci load... wywoływany jest ciąg funkcji `mark`. Funkcja `mark` zapala odpowiedni bit pod adresem w pamięci. 
Ustawiłam breakpoint na początku funkcji `mark` i obserwowałam zawartość tego miejsca w pamięci przy każdym przerwaniu.
Wywnioskowałam, że to miejsce `ds:[7ff6f9b41ac8]` odpowiada za stan gry. Aby ustawić stan gry na taki jak po próbie uruchomienia komputera z mamą,
przeszłam grę do tego momentu i sprawdziłam wartość tego miejsca w pamięci. Następnie zmodyfikowałam funkcję `SDL_main` w taki sposób, aby po
wykonaniu funkcji `load...` ustawiała stan gry w tym miejscu na wartość, którą odczytałam czyli `0x747`.

4. **Nieuczciwa przewaga**\
**Zadanie:** Zmodyfikuj grę w taki sposób, aby dało się wygrać walkę z Garym. Postaraj się nie używać wyłącznie brutalnej siły, zmniejszenie wartości HP lub zwiększenie
wartości ataku nie będzie oceniane pełną liczbą punktów. Zbadaj na czym polega
nieuczciwa przewaga przeciwnika i wyrównaj szanse, stosując odpowiednią modyfikację.\
**Nieuczciwa przewaga przeciwnika:** Nieczuciwą przewagą przeciwnika jest stosowanie strategii `strategy_endless_healing`,
która przzewiduje przy użyciu funkcji `predict` czy przeciwnik nas zabije w następnej turze i jeśli tak to używa 
mikstury leczącej. Po wykonaniu funkcji `predict` Gary wykonuje skok warunkowy jeśli zagra uczciwie i nie wykonuje go w przypasku użycia eliksiru. 
Aby wyrównać szanse, zmieniłam skok warunkowy na skok bezwarunkowy, co sprawia, że Gary nigdy nie używa eliksiru.


