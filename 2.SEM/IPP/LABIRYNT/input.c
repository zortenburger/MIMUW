#include "input.h"

// funkcja wczytująca liczbę z zakresu [1, SIZE_MAX] ze standardowego wejścia
// zwraca true przy poprawnym wczytaniu i false w przeciwnym wypadku 
bool readNumber (size_t *result){
    *result = 0;

    char temp;

    for (temp = getchar(); temp != EOF && !isspace(temp); temp = getchar()){
        if(isdigit(temp)){
            int tempINT = ((int)temp-(int)'0');

            if (((SIZE_MAX - tempINT) / 10) < *result) {
                return false;  // liczba przekracza SIZE_MAX
            }

            *result = *result * 10 + tempINT;
        }
        else {
            return false; // wczytany został znak ktory nie jest cyfra
        }
    }

    ungetc(temp, stdin);

    if(*result > 0){
        return true; // liczba jest mniejsza od zera
    }
    else {
        return false;
    }
}

// funkcja wczytująca liczbę z zakresu [0, UINT32_MAX] ze standardowego wejścia
// zwraca true przy poprawnym wczytaniu i false w przeciwnym wypadku 
bool readNumber32 (uint32_t *result){
    *result = 0;
    char temp;
    for (temp = getchar(); temp != EOF && !isspace(temp); temp = getchar()){
        if(isdigit(temp)){
            int tempINT = ((int)temp-(int)'0');

            if (((UINT32_MAX - tempINT) / 10) < *result) {
                return false; // liczba przekracza UINT32_MAX
            }

            *result = *result * 10 + tempINT;
        }
        else {
            return false; // wczytany zostal znak ktory nie jest cyfra
        }
    }

    ungetc(temp, stdin);

    return true;
}

//funkcja prawdzajaca czy nastepny znak to bialy znak lub /n lub EOF
// zwraca 0 dla \n
// zwraca 1 dla bialych znakow i je pomija
// zwraca 0 dla EOF
// zwraca -1 w przeciwnym wypadku
int next() {
    char temp = getchar();

    if (temp == '\n') {
        return 0;
    } 
    else if (isspace(temp)) {
        return 1;
    }
    else if (temp == EOF) {
        ungetc(temp, stdin);

        return 0;
    }
    else {  
        ungetc(temp, stdin);

        return -1;
    }
} 

// funkcja wczytuje linie zlozona z liczb z zakresu [1; SIZE_MAX] i dodaje je na vectora v
// zwraca true przy poprawnym wczytaniu i false w przeciwnym wypadku 
bool readLine (vector *v){
    int fl;

    for(fl = next(); fl != 0; fl = next()){
        if(fl == -1){
            size_t temp;
            if(readNumber(&temp)){
                pushBack(v, temp);
            }
            else {
                return false; //wczytana liczba nie spelnia warunkow
            }
        }
    }

    setSizeVector(v);

    return true;
}

// fukncja sprawdzajaca czy znak jest cyfra w zapisie 16 liczby
bool is16Digit (char c){
    if(isdigit(c)){
        return true;
    }
    else if ('A' <= c && c <= 'F') {
        return true;
    }
    else {
        return false;
    }
}

// funkcja wczytujaca 4 linie w wersji z zapisem 16 liczby 
// i zaznaczajaca bity w ktorych sa sciany na bitsecie
// zwraca true przy poprawnym wczytaniu i false w przeciwnym wypadku 
bool read4aLine (bitset *b, size_t *size){
    string s; //string wczytujacy liczbe pojawiajaca sie na wejsciu
    initString(&s);

    if(getchar() != 'x'){
        freeString(&s);
        return false; // po zerze nie ma znaku x
    }

    int fl;
    char temp;

    for (temp = getchar(); !isspace(temp); temp = getchar()){
        if(is16Digit(temp)){
            addChar(&s, temp);
        }
        else {
            freeString(&s);
            return false; // znak nie jest liczba w zapisie 16 liczby
        }

        if(isEOF(0)){
            break;
        }
    }
    for (; isspace(temp) && temp != '\n'; temp = getchar()){
        
    }
    if(!isEOF(0) && temp != '\n'){
        freeString(&s);
        return false; // nie wczytano /n ani EOF, liczba ma spacje w srodku
    }

    if(!isEOF(0)){
        ungetc(temp, stdin);
    }
    
    size_t zeros = 0; //liczba zer wiodacych w zapisie 16 liczby
    size_t bitZeros = 0; // liczba zer w pierwszej niezerowej liczbie w zapisie dwojkowym

    while(zeros + 1 < s.size  && s.elements[zeros + 1] == '0'){
        zeros++;
    }

    if(s.elements[zeros] == '0'){
        zeros++;
    }

    if(zeros < s.size){
        for(int i = 0; i < 4; i++){
            if((s.elements[zeros] & (1 << i)) != 0){
                bitZeros = 3 - i;
            }
        }
    }
    
    if((s.size - zeros) * 4 > *size + bitZeros){
        freeString(&s);
        return false; // liczba bitowa ma za duzy rozmiar w porownaniu z rozmiarem labiryntu
    }

    if(!toBitset(&s, b)){
        freeString(&s);
        return false; // blad przy zapisywaniu do bitseta
    }

    freeString(&s);

    return true;
}

// funkcja wczytujaca 4 linie w wersji z R 
// i zaznaczajaca bity w ktorych sa sciany na bitsecie
// zwraca true przy poprawnym wczytaniu i false w przeciwnym wypadku 
bool read4bLine (uint32_t *a, uint32_t *b, uint32_t *m, uint32_t *r, uint32_t *s0){
    int fl;
    int howMany = 0; // liczba dotychczasowo wczytanych liczb

    for(fl = next(); fl != 0; fl = next()){
        if(fl == -1){
            uint32_t temp;

            if(!readNumber32(&temp)){
                return false; // wczytana liczba nie spelnia warunkow
            }

            if(temp >= 0){
                if(howMany == 0){
                    *a = temp;
                }
                else if(howMany == 1){
                    *b = temp;
                }
                else if(howMany == 2){
                    *m = temp;
                }
                else if(howMany == 3){
                    *r = temp;
                }
                else if(howMany == 4){
                    *s0 = temp;
                }
                howMany++;
            }
            else {
                return false; // liczba mniejsza od 0 
            }
        }
    }


    if (howMany == 5 && *m != 0){
        if(fl == 0){
            ungetc('\n',stdin);
        }
        return true;
    }

    return false; // wczytano za malo/za duzlo liczb albo m = 0
}

//funkcja zamieniajaca reprezentacje labityntu z wersji z R na bitset scian labiryntu
void change4b_4a(uint32_t *a, uint32_t *b, uint32_t *m, uint32_t *r, uint32_t *s0, bitset *v, size_t *size){
    uint64_t si = *s0;
    uint64_t wi = si % (*size);

    for(size_t i = 1; i <= *r; i++){
        si = ((*a) * si + (*b)) % (*m);
        wi = si % (*size);

        if(wi <= (1UL << 32)){
            uint64_t akt = wi;
            addBitPos(v, akt);

            while(akt < *size){
                if(akt <= SIZE_MAX - (1UL << 32)){
                    akt += (1UL << 32);
                    if(akt < *size){
                        addBitPos(v, akt);
                    }
                } 
            }
        }
    }
}

// funkcja wczytujaca 4 linie 
// zwraca true przy poprawnym wczytaniu i false w przeciwnym wypadku 
bool read4Line (bitset *v, size_t *size){
    int fl = next();

    while(fl != -1){ // pominiecie bialych znakow
        if(fl == 0)
        {
            return false;
        }

        fl = next();
    }

    char temp = getchar();

    if(temp == '0'){ // wersja z zapisem 16 liczby 
        if(!read4aLine(v, size)){
            return false;
        }
    } 
    else if(temp == 'R'){ // wersja z R
        uint32_t a, b, m, r, s0;

        if(!read4bLine(&a, &b, &m, &r, &s0)){
            return false;
        }

        change4b_4a(&a, &b, &m, &r, &s0, v, size);
    }
    else {
        return false; // pierwszy niebialy znak to nie jest ani 0 ani R
    }

    return true;
}

//funkcja obliczajaca rozmiar calego labiryntu 
// zwraca 0 gdy rozmiar przekracza SIZE_MAX
size_t calculateSize(vector *v){
    size_t result = 1;

    for(int i = 0; i < v->size; i++){
        if(result >= SIZE_MAX / v->elements[i]){
            return 0; // rozmiar przekracza SIZE_MAX
        }
        else{
            result *= v->elements[i];
        }
    }

    return result;
}

// funkcja sprawdzajaca czy kostka o wspolrzednych act miesci sie w labiryncie o wymiarach sizes
bool isCubeIn (vector *sizes, vector *act)
{
    for(size_t i = 0; i < sizes->size; i++){
        if(sizes->elements[i] < act->elements[i]){
            return false;
        }
    }

    return true;
}

// funkcja sprawdzajaca czy wystapil EOF
// w zaleznosci od wersji pomija lub nie biale znaki
// w wersji 1 sprawdza ilosc \n gdy jest wieksza niz jeden zwraca 0
bool isEOF(int ver){
    char temp = getchar();
    int endll = 0;

    if(ver == 1){
        for (; isspace(temp); temp = getchar()){
            if(temp == '\n')
            {
                endll++;
            }
        }

        if(endll > 1){
            return false; // za duzo \n
        }

        ungetc(temp, stdin);

        if(feof(stdin)){
            return true;
        }
        else {
            return false;
        }
    }

    ungetc(temp, stdin);

    if(feof(stdin)){
        return true;
    }
    else{
        return false;
    }
    
}

//funkcja sprawdzjaca czy wszystkie struktury sa dobrze zaalokowane
bool goodAlloc (vector *beginning, vector *end, bitset *walls, vector *sizes){
    if(beginning->elements == NULL){
        return false;
    }

    if(end->elements == NULL){
        return false;
    }

    if(sizes->elements == NULL){
        return false;
    }

    if(walls->elements == NULL){
        return false;
    }

    return true;
}

//funkcja wczytujaca cale wejscie i zwracajaca:
//0 -  wystapil blad alokacji
//1 - wystapil blad w 1 linii
//2 - wystapil blad w 2 linii
//3 - wystapil blad w 3 linii
//4 - wystapil blad w 4 linii
//5 - za duzo lini
//7 - brak bledu
int readData (vector *beginning, vector *end, bitset *walls, vector *sizes){
    size_t size;
    size_t k;

    if(!readLine(sizes)){
        return 1;
    }
    else{
        size = calculateSize(sizes);

        if(size == 0){
            return 0;
        }

        k = sizes->size;

        if(k == 0){
            return 1;
        }
        
        setSizeBitset(walls, size);

        if(!goodAlloc(beginning, end, walls, sizes)){
            return 0;
        }
    }

    if(!readLine(beginning)){
        return 2;
    }
    else{
        if(beginning->size != k){
            return 2;
        }

        if(!isCubeIn(sizes, beginning)){
            return 2;
        }
    }

    if(!readLine(end)){
        return 3;
    }
    else{
       if(end->size != k){
            return 3;
        }

        if(!isCubeIn(sizes, end)){
            return 3;
        }
    }

    if(!read4Line(walls, &size)){
       return 4;
    }

    if(!goodAlloc(beginning, end, walls, sizes)){
        return 0;
    }

    if(!isEOF(1)){
        return 5;
    }

    return 7;
}