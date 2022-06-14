/** @file
 * Interfejs klasy przechowującej funkcje obsługujące napisy.
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


/** @brief Sprawdza czy znak reprezentuje cyfrę.
 * Sprawdza czy podany znak jest z zakresu: {'0', '1',..., '9', '*', '#'}.
 *  Dla poprawnego znaku zwraca true, dla złego false.
 * @param[in] c  – sprawdzany znak;
 * @return Wartość @p true, gdy znak jest poprawny.
 *         Wartość @p false, jeśli znak  nie jest poprawny.
 */
bool isDigit(char const c);

/** @brief Zamienia cyfrę na znak, ktory go reprezentuje.
 * Zamienia cyfrę na znak, ktory go reprezentuje.
 * Odpowiednio dla {0, 1,..., 9, 10, 11} zwraca {'0','1',...,'9','*','#'}.
 * @param[in] c – zamieniany znak;
 * @return Wartość znaku.
 */
char  * ofDigit(int c);

/** @brief Zamienia znak na cyfre, ktora reprezentuje.
 * Zamienia znak na cyfre, ktora reprezentuje.
 * Odpowiednio dla {'0','1',...,'9','*','#'} zwraca {0, 1,..., 9, 10, 11}.
 * @param[in] c  – zamieniany znak;
 * @return Wartość znaku.
 */
int toDigit(char const c);

/** @brief Sprawdza czy napis reprezentuje numer.
 * Sprawdza czy podany napis jest numerem, tj: czy zawiera same cyfry, 
 * nie jest NULL-em ani nie jest pusty. Dla poprawnego
 * napisu zwraca true, dla złego false.
 * @param[in] in  – wskaźnik na sprawdzany napis;
 * @return Wartość @p true, gdy numer jest poprawny.
 *         Wartość @p false, jeśli numer nie jest poprawny np:
 *         jest NULLem, jest pusty, zawiera znaki, ktore
 *          nie są cyframi.
 */
bool checkString (char const *in);

/** @brief Sprawdza czy dwa napisy reprezentujące numer są takie same.
 * Sprawdza czy podane napisy wskazywane przez @p num1 i @p num2 są takie same,
 * @param[in] num1  – wskaźnik na sprawdzany napis;
 * @param[in] num2  – wskaźnik na sprawdzany napis;
 * @return  Wartość @p true, jeśli napisy są takie same.
 *         Wartość @p false, jeśli napisy są rozne.
 */
bool cmpString (char const *num1, char const *num2);

/** @brief Kopiuje napis.
 * Funkcja kopuje napis wskazywany przez @p in i zwraca na niego wskaźnik.
 * @param[in] in  – wskaźnik na kopiowany napis;
 * @return  Wskaźnik na skopiowany napis.
 */
char * setString (char const *in);

/** @brief Kopiuje napis.
 * Funkcja kopije napis wskazywany przez @p in2 oraz podmienia 
 * jego @p x  poczatkowych znakow na napis @p in2 i zwraca wskaźnik na 
 * nowy napis.
 * @param[in] in1  – wskaźnik na podmieniony napis;
 * @param[in] in2  – wskaźnik na kopiowany napis;
 * @param[in] x  – dlugosc usuwanego prefiksu z napisu @p in2;
 * @return  Wskaźnik na nowoutworzony napis.
 */
char * setString2 (char const *in1, char const *in2, size_t x);