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
#include "phone_numbers.h"

/**
 * To jest struktura przechowująca wierzcholek trie.
 */
struct trieNodeReverse;
/**
 * To jest struktura przechowująca wierzcholek trie.
 */
typedef struct trieNodeReverse trieNodeReverse;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowan,
 * ani krawedzi.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
trieNodeReverse * trieNewRev(trieNodeReverse * pf);

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pf – wskaźnik na usuwaną strukturę.
 */
void trieDeleteRev(trieNodeReverse *pf);


/** @brief Dodaje numer w wierzcholku.
 * Dodaje numer @p forward do wierzcholka, do ktorego od korzenia prowadzą
 * krawedzie tworzace napis @p num.
 * Relacja przekierowania numerów nie jest przechodnia.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą trieReverse
 * @param[in] num – wskaźnik na napis reprezentujący napis reprezentujacy
 *                    kolejne krawedzie.
 * @param[in] forward  – wskaźnik na napis reprezentujący etykiete.
 * @return Wartość @p true, jeśli etykieta została dodana.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się alokować pamięci.
 */
bool trieAddRev(trieNodeReverse *pf, char const *num, char const *forward);

/** @brief Usuwa etykiete.
 * Usuwa napis @p revForward, w wierzcholku do ktorego prowadzi sciezka,
 * ktorej krawedzie tworza napis @p forward. Jeśli nie ma takiej ściezki
 * lub napis nie reprezentuje numeru, nic nie robi.
 * @param[in,out] pf – wskaźnik na strukturę przechowującą drzewo.
 * @param[in] forward – wskaźnik na napis reprezentujący krawedzie na sciezce.
 * @param[in] revForward – wskaźnik na napis do usuniecia.
 * @param[in] depth - głebokość od korzenia na ktorej sie znajduje.
 */
void trieRemoveRev(trieNodeReverse *pf, char const *forward, char const *revForward, size_t depth);

/** @brief Wyznacza wszystkie etykiety na sciezce.
 * Wyznacza wszystkie etykiety znajdujace sie na podanej sciezce. 
 * Podmienia ich prafiks na sciezke do nich prowadzaca
 * Wynikiem jest struktura przechowujaca wynikowe napisy. Jesli 
 * prefiks tego napisu nie jest sciezką, zwracany jest NULL.
 * @param[in] pf  – wskaźnik na strukturę przechowującą drzewo
 * @param[in] forward – wskaźnik na napis reprezentujący sciezke.
 * @param[in] i - głebokość od korzenia na ktorej sie znajduje.
 * @return Wskaźnik na struktura przechowujaca wynikowe napisy lub NULL, 
 *          gdy nie udało się alokować pamięcilub gdy napis nie znajduje
 *          na zadnej ściezce.
 */
phones * trieGetRev(trieNodeReverse const *pf, char const *forward, size_t * i);
