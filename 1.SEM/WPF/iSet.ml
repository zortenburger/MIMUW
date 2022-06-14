(*iSet*)
(*autor:Zuzanna Ortenburger*)
(*recenzja: Emilia Dębicka*)
(*
 * ISet - Interval sets
 * Copyright (C) 1996-2003 Xavier Leroy, Nicolas Cannasse, Markus Mottl, Jacek Chrzaszcz
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version,
 * with the special exception on linking described in file LICENSE.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *)


(*Reprezentacja drzewa w postaci lewe poddrzewo, odpowiadający przedział, prawe poddrzewo, wysosość, liczba liczb w poddrzewie*)
type t = Empty | Node of t * (int * int) * t * int * int

(*Konstruktor pustego drzewa*)
let empty = Empty

(*Sprawdza czy drzewo jest puste*)
let is_empty t1 = 
  t1 = Empty

(*Zwraca wysokość poddrzewa*)
let height = function
  | Node (_, _, _, h, _) -> h
  | Empty -> 0

(*Zwraca ile liczb jest w poddrzewie*)
let width = function
  | Node (_, _, _, _, w) -> w
  | Empty -> 0

(*Zwraca sume liczb, jeśli jest większa niż max_int zwraca max_int*)
let add_d a b = 
    if a+b < 0 then max_int else a + b

(*Zwraca dla przedziału ile liczb zawiera*)
let size a b =
  add_d (add_d b (-a) ) 1

(*Konstruktor drzewa o lewym poddrzewie l, prawym poddrzewie r i odpowadającym przedziałem (a,b)*)
(*Wysokość takiego drzewa to maksimum z wysokosci lewego i prawego poddrzewa powiększonego o jeden (krawędź do korzenia)*)
(*Ilość elementów w tym poddrzewie to suma ilości elementów w prawym i lewym poddrzewie oraz ilości elementów w korzeniu*)
let make l (a,b) r = Node (l, (a, b), r, 1 + max (height l) (height r), add_d (size a b)  (add_d (width l) (width r)))

(*Balansowanie drzewa tak aby miało ono strukture drzewa AVL*)
let bal l k r =
  let hl = height l in
  let hr = height r in
  if hl > hr + 2 then
    match l with
    | Node (ll, lk, lr, _, _) ->
        if height ll >= height lr then make ll lk (make lr k r)
        else
          (match lr with
          | Node (lrl, lrk, lrr, _, _) ->
              make (make ll lk lrl) lrk (make lrr k r)
          | Empty -> assert false)
    | Empty -> assert false
  else if hr > hl + 2 then
    match r with
    | Node (rl, rk, rr, _, _) ->
        if height rr >= height rl then make (make l k rl) rk rr
        else
          (match rl with
          | Node (rll, rlk, rlr, _, _) ->
              make (make l k rll) rlk (make rlr rk rr)
          | Empty -> assert false)
    | Empty -> assert false
  else make l k r

(*Znajdowanie najmniejszego (co do wartości) przedziału na drzewie*)
let rec min_elt = function
  | Node (Empty, k, _, _, _) -> k
  | Node (l, _, _, _, _) -> min_elt l
  | Empty -> raise Not_found

(*Usuwanie najmniejszego (co do wartości) przedziału na drzewie*)
let rec remove_min_elt = function
  | Node (Empty, _, r, _, _) -> r
  | Node (l, k, r, _, _) -> bal (remove_min_elt l) k r
  | Empty -> invalid_arg "ISet.remove_min_elt"

(*Znajdowanie najwiekszego (co do wartości) przedziału na drzewie*)
let rec max_elt = function
  | Node (_, k, Empty, _, _) -> k
  | Node (_, _, r, _, _) -> max_elt r
  | Empty -> raise Not_found

(*Usuwanie największego (co do wartości) przedziału na drzewie*)
let rec remove_max_elt = function
  | Node (l, _, Empty, _, _) -> l
  | Node (l, k, r, _, _) -> bal l k (remove_max_elt r)
  | Empty -> invalid_arg "ISet.remove_max_elt"

(*Porównywanie przedziałów względem ich wartości*)
(*zwraca 0 jeśli x i y nie są rozłaczne, 1 jesli x mniejszy niż y i -1 w przeciwnym wypadku*)
let cmp (x1, x2) (y1,y2) =
  if (x1 > y2) then 1 else 
  if (x2 < y1) then -1 else 0

(*Dodaje  przedział do drzewa, pod warunkiem że jest on rozłączny ze wszystkimi przedziałami na drzewie*)
let rec add_one  x = function
  | Node (l, k, r, h, w) ->
      let c = cmp x k in
      if c = 0 then Node (l, x, r, h, w)
      else if c < 0 then
        let nl = add_one  x l in
        bal nl k r
      else
        let nr = add_one  x r in
        bal l k nr
  | Empty -> make empty x empty

(*Rekurencyjna funkcja łącząca dwa drzewa i przedział przy założeniu, przy założeniu, że v jest rozłączne z tymi drzewami*)
let rec join l v r =
  match (l, r) with
    (Empty, _) -> add_one v r
  | (_, Empty) -> add_one v l
  | (Node(ll, lv, lr, lh, _), Node(rl, rv, rr, rh, _)) ->
      if lh > rh + 2 then bal ll lv (join lr v r) else
      if rh > lh + 2 then bal (join l v rl) rv rr else
      make l v r

(*Łączy dwa drzewa przy założeniu ze wszystkie elementy z pierwszego drzewa są mniejsze od wszystkich z drugiego*)
let merge t1 t2 =
  match t1, t2 with
  | Empty, _ -> t2
  | _, Empty -> t1
  | _ ->
      let k = min_elt t2 in
      join t1 k (remove_min_elt t2)

(* Zwraca drzewo AVL o elementach mniejszych od x; prawdę jeżeli x należy do s; drzewo AVL o elementach większych od x*)
let split x set =
  let rec loop x1 set1= 
    match set1 with
    | Empty -> (Empty, false, Empty)
    | Node (l, (a, b), r, _, _) ->
        let c = cmp (x, x) (a, b) in
        if c = 0 then 
          let nl = if x <= a then l else add_one (a, (x-1)) l  
          in let nr = if x >= b then r else add_one ((x+1), b) r
          in (nl, true, nr)
        else if c < 0 then
          let (ll, pres, rl) = loop x l in (ll, pres, join rl (a, b) r)
        else
          let (lr, pres, rr) = loop x r in (join l (a, b) lr, pres, rr)
  in loop x set 

(*Sprawdza czy element jest elementem drzewa*)
let mem x set =
  let rec loop = function
    | Node (l, k, r, _, _) ->
        let c = cmp (x,x) k in
        c = 0 || loop (if c < 0 then l else r)
    | Empty -> false in
  loop set

(*Zwraca drzewo z dodanym przedziałem*)
(*Poprzez dodanie drzewa składającego sie ze wszystkich elementów mniejszych od poczatku przedziału,
drzewa składającego sie ze wszystkich elementów większych od końca przedziału,
oraz tego przedziału*)
let add (x, y) s =
    let (l, _, lr) = split x s in
    let (_, _, r) = split y lr in
    let (x1, l1) =
        if (mem (x - 1) l) then 
            (fst (max_elt l), remove_max_elt l)
        else 
            (x, l)
    and (y1, r1) =
        if (mem (y + 1) r)then 
            (snd (min_elt r), remove_min_elt r)
        else 
            (y, r) in
    join l1 (x1, y1) r1

(*Zwraca drzewo bez danego przedziału*)
let remove (x, y) s =
    let (l, _, lr) = split x s in
    let (_, _, r) = split y lr in
    merge l r

(*Wykonuje funkcje f na wierzcholkach drzewa w kolejności rosnącej jesli chodzi o wartosci przedziałow*)
let iter f set =
  let rec loop = function
    | Empty -> ()
    | Node (l, k, r, _, _) -> loop l; f k; loop r in
  loop set

(*Składa funkcje f na wierzcholkach drzewa w kolejności rosnącej jesli chodzi o wartosci przedziałow*)
let fold f set acc =
  let rec loop acc = function
    | Empty -> acc
    | Node (l, k, r, _, _) ->
          loop (f k (loop acc l)) r in
  loop acc set


(*Zwraca liste przedziałow na drzewie w kolejności rosnącej*)
let elements set = 
  let rec loop acc = function
    | Empty -> acc
    | Node(l, k, r, _, _) -> loop (k :: loop acc r) l in
  loop [] set

(*Zwraca liczbę licz w drzewie mniejszych równych x*)
let rec below x set =
  match set with 
  | Empty -> 0
  | Node (l, (a, b), r, _, w) ->
      if x < a then 
        below x l
      else if x > b then
        add_d (add_d (width l) (size a b)) (below x r)
      else 
        add_d (width l) (size a x)