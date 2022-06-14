/** @file
 * Interfejs klasy przechowującej strukture trie.
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
#include "trieRev.h"

/**
 * To jest struktura przechowująca wierzcholek trie.
 */
struct trieNode;
/**
 * To jest struktura przechowująca wierzcholek trie.
 */
typedef struct trieNode trieNode;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowan,
 * ani krawedzi.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
trieNode * trieNew(trieNode *pf);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void trieDelete(trieNode *pf);


/** @brief Dodaje etykiete na sciezce.
 * Dodaje etykiete @p forward do wierzcholka, do ktorego od korzenia prowadzą
 * krawedzie tworzace napis @p num.Jeśli wcześniej została dodana przekierowanie
 * z takim samym parametrem @p num, to jest ono zastępowane.
 * Relacja przekierowania numerów nie jest przechodnia.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą trie
 * @param[in] num   – wskaźnik na napis reprezentujący napis reprezentujacy
 *                     kolejne krawedzie.
 * @param[in] forward   – wskaźnik na napis reprezentujący etykiete.
 * @param[in, out] pfRev - wskaźnik na strukturę przechowującą trieRev
 * @return Wartość @p true, jeśli etykieta została dodana.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się alokować pamięci.
 */
bool trieAdd(trieNode *pf, char const *num, char const *forward, trieNodeReverse *pfRev);

/** @brief Usuwa etykiete.
 * Usuwa poddrzewo, do ktorego prowadzi sciezka,
 * ktorej krawedzie tworza napis @p num. Jeśli nie ma takiej ściezki
 * lub napis nie reprezentuje numeru, nic nie robi.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą drzewo.
 * @param[in] num    – wskaźnik na napis reprezentującykrawedzie na sciezce.
 * @param[in] depth - głebokość od korzenia na ktorej sie znajduje.
 * @param[in, out] pfRev - wskaźnik na strukturę przechowującą trieRev
 */
void trieRemove(trieNode *pf, char const *num, size_t depth, trieNodeReverse *pfRev);

/** @brief Wyznacza etykiete na sciezce.
 * Wyznacza etykiete znajdujaca sie na podanej sciezce. Szuka najdłuższej
 * pasujacej sciezki. Wynikiem jest napis reprezentujacy etykiete. Jesli 
 * prefiks tego napisu nie jest sciezką, zwracany jest NULL.
 * @param[in] pf  – wskaźnik na strukturę przechowującą drzewo
 * @param[in] num – wskaźnik na napis reprezentujący sciezke.
 * @param[in] i - głebokość od korzenia na ktorej sie znajduje.
 * @return Wskaźnik na napis reprezentujacy najlepszą etykiete lub NULL, 
 *          gdy nie udało się alokować pamięcilub gdy napis nie znajduje
 *          na zadnej ściezce.
 */
char * trieGet(trieNode const *pf, char const *num, size_t * i);
