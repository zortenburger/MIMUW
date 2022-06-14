/** @file
 * Interfejs klasy przechowującej numery telefoniczne
 *
 * @author Zuzanna Ortenburger <zo438646@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */
#pragma once 

#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
typedef struct phones phones;
/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
struct phones{
    char ** numbers; ///<tablica wskaznikow na napisy reprezentujace numery
    size_t size; ///<ilosc przechowywanych numerow telefonow
    size_t actSize; ///<rozmiar zaalokowanej w strukturze pamieci
};

/** @brief zwraca tablicę numerow przechowywanych w strukturze @p phnum.
 * @param[in] phnum – wskaźnik na  strukturę.
 * @return tablica numerow przechowywanych w strukturze @p phnum.
 */
char ** getNumbers(phones* phnum);

/** @brief zwraca rozmiar tablicy numerow przechowywanych w strukturze @p phnum.
 * @param[in] phnum – wskaźnik na  strukturę.
 * @return rozmiar tablicy numerow przechowywanych w strukturze @p phnum.
 */
size_t getSize(phones* phnum);

/** @brief Inicjuje nową strukturę.
 * Inicjuje nową strukturę niezawierającą żadnych przekierowań.
 * @param[in] phone – wskaźnik na inicjowana strukturę.
 */
void phnumInit (phones * phone);

/** @brief Zwieksza strukture.
 * Zwieksza dwukrotnie strukture na ktora wskazuje @p s.
 * Alokuje niezbedną pamieć.
 * @param[in,out] s – wskaźnik na strukturę przechowującą numery;
 */
void phnumResize(phones *s);

/** @brief Dodaje numer.
 * Dodaje napis reprezentujacy numer @p elem do struktury @p s.
 * @param[in,out] s – wskaźnik na strukturę przechowującą numery;
 * @param[in] elem   – wskaźnik na napis reprezentujący dodawany numer;
 */
void phnumAdd(phones *s, const char * elem);


/** @brief Usuwa numer o zadanym indeksie ze struktury.
 *  Usuwa numer o indeksie @p idx ze struktury @p pnum.
 *
 * @param[in,out] pnum – wskaźnik na strukture przechowującą numery.
 * @param[in] idx – indeks numeru w strukturze
 * (idx >= 0 && idx < liczba numerów w strukturze).
 * @return Wskaźnik na strukturę z usuniętym numerem, jeśli usunięto numer.
 *         Wskaźnik na strukturę bez usuniętego numeru, 
 *         jeżel nie udało się zaalokować pamięci.
 */
void phnumRemove(struct phones *pnum, int idx);

/** @brief Komparator numerów.
 * @param[in] a – wskaźnik na numer.
 * @param[in] b – wskaźnik na numer.
 * @return Wartość mniejszą od 0, jeżeli a < b (w leksykograficznym porządku 
 *         numerów pod tymi wskaźnikami).
 *         Wartość 0, jeżeli a = b. Wartość większą od 0, jeżeli a > b.
 */
int compare(const void *a, const void *b);

/** @brief Sortuje numery w strukturze @p pnum.
 *
 *  Sortuje n pierwszych numerów w strukturze @p pnum
 *
 * @param[in] pnum – wskaźnik na strukture przechowującą numery.
 */
 void phnumSort(phones *pnum);


/** @brief Usuwa powtarzające się numery w strukturze @p pnum.
 * @param[in,out] pnum – wskaźnik na strukture przechowującą numery.
 * @return Wskaźnik, która zawiera wszystkie numery ze struktury
 *         @p pnum bez powtórzeń.
 */
void phnumUnique(struct phones *pnum);


/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete1(phones *pnum);


/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg numerów telefonów;
 * @param[in] idx  – indeks numeru telefonu.
 * @return Wskaźnik na napis reprezentujący numer telefonu. Wartość NULL, jeśli
 *         wskaźnik @p pnum ma wartość NULL lub indeks ma za dużą wartość.
 */
char const * phnumGet1(phones const *pnum, size_t idx);

