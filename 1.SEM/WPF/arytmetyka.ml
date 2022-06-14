(* Arytmatyka *)
(* autor: Zuzanna Ortenburger *)
(* recezja: Jagoda Bobińska *)

(* przedział trzymam jako para postaci poczatek koniec *)
type przedzial =  float * float

(* przedział przetwarzany w zadaniu trzymam jako sume przedziałów *)
(* jeśli przedział jednokawałkowy to drugi składowy przedział postaci nan nan jako przedział pusty *)
type wartosc = przedzial * przedzial 

(* KONSTRUKTORY *)
let wartosc_od_do (x : float) (y : float)=
      (((x, y), (nan, nan)) : wartosc)

let wartosc_dokladnosc (x : float) (p : float)=
      let a = x -. ((p *. x) /. 100.) and
      b = x +. ((p *. x) /. 100.) in
      ((wartosc_od_do (min a b) (max a b)) : wartosc)

let wartosc_dokladna (x : float) = 
      ((wartosc_od_do x x) : wartosc)

(* POMOCNICZE *)

(* sprawdza czy liczba to nan *)
let is_nan x = compare x nan = 0

(* sprawdza czy przedział jest pusty *)
let is_empty x1 x2 = 
      if (is_nan x1 || is_nan x2) then true else false

(* sprawdza czy przedział jest jednokawałkowy *)
let is_one (((x1, x2), (x3, x4)) : wartosc) = 
      is_empty x3 x4

(*sprawdza czy przedział to dokladne zero*)
let is_zero (x1, x2) =
      if (x1, x2)=(0., 0.) then true else false

(*sprawdza czy liczba jest wewnątrz przedziału jednokawałokwego*)
let in_pom x1 x2 y =
      if is_empty x1 x2 then false else 
            if (x1 <= y && y <= x2) then true else false

(*SELEKTORY*)

let in_wartosc (((x1, x2), (x3, x4)) : wartosc) (y : float) =
      (in_pom x1 x2 y || in_pom x3 x4 y)

let min_wartosc (((x1, x2), (x3, x4)) : wartosc) = x1 

let max_wartosc (((x1, x2), (x3, x4)) : wartosc) = 
      if is_one ((x1, x2), (x3, x4)) then x2 else x4

let sr_wartosc (((x1, x2), (x3, x4)) : wartosc) =
      (max_wartosc ((x1, x2), (x3, x4)) +. min_wartosc ((x1, x2), (x3, x4))) /. 2.

(* funkcje pomocnicze do modyfikatorów *)

(* funkcja zwracająca mniejszą z dwóch liczb, uwzględniająca nan *)
let mini a b =
      if is_nan a then b else
      if a > b then b else a

(* funkcja zwracająca większą z dwóch liczb, uwzględniająca nan *)
let maxi a b =     
      if is_nan a then b else
      if a < b then b else a

(* funkcja zwracająca najmniejszy iloczyn z dwóch liczb spośród czterech liczb *)
let mini_4 x1 x2 y1 y2 =
      mini (mini (x1 *. y1) (x1 *. y2)) (mini (x2 *. y1) (x2 *. y2))

(* funkcja zwracająca największy iloczyn z dwóch liczb spośród czterech liczb *)
let maxi_4 x1 x2 y1 y2 =
      maxi (maxi (x1 *. y1) (x1 *. y2)) (maxi (x2 *. y1) (x2 *. y2))

(* funkcja dzieląca przedział [x1,x2] jeśli zawiera zero na [x1,-0.][0.,x2]*)
let podziel (x1, x2) =
      if is_empty x1 x2 then ((nan, nan), (nan, nan)) else
      if x2 = 0. then ((x1, -0.), (nan, nan)) else
      if x1 = 0. then ((x1, x2), (nan, nan)) else
      if x1*.x2 > 0. then ((x1, x2), (nan, nan)) else
      ((x1, -0.), (0., x2)) 

(* funkcja scalająca trzy przedziały w dwa *)
let scal3to2 ((x1, x2), (y1, y2)) (z1, z2) =
      if is_empty z1 z2 then ((x1, x2),(y1, y2)) else
      if (in_pom x1 x2 z1 && in_pom y1 y2 z2) then ((x1, y2), (nan, nan)) else
      if in_pom x1 x2 z1 then ((x1, max z2 x2), (y1, y2)) else
      if in_pom x1 x2 z2 then ((min z1 x1,x2), (y1, y2)) else   
      if in_pom y1 y2 z1 then ((x1, x2), (y1, max z2 y2)) else
      if in_pom y1 y2 z2 then ((x1, x2), (min z1 y1, y2)) else
      ((min x1 z1, min x2 z2), (max x1 z1, max x2 z2))

(* funkcja scalająca cztery przedziały w dwa *)
let scal ((x1, x2), (x3, x4)) ((y1, y2), (y3, y4)) = 
      scal3to2 (scal3to2 (scal3to2 ((x1, x2), (nan, nan)) (x3, x4)) (y1, y2)) (y3, y4)

(* funkcja pomocnicza dodawania dwóch przedziałów *)
let plus_pom (x1, x2) (y1, y2) =
      ((x1 +. y1, x2 +. y2), (nan, nan))

(* funkcja pomocnicza mnożenia dwóch przedziałów *)
let mnozeni (x1, x2) (y1, y2) =
      (mini_4 x1 x2 y1 y2, maxi_4 x1 x2 y1 y2) 

(* funkcja która rozdziela przedziały wobec zera i wykonuje ich mnożenie *)
let razy_pom (x1, x2) (y1, y2) =
      if (is_zero (x1, x2) || is_zero (y1, y2)) then ((0., 0.), (nan, nan)) else 
            let ((a1, a2), (b1, b2)) = podziel (x1, x2) and ((c1, c2), (d1, d2)) = podziel (y1, y2) in
            scal ((mnozeni (a1, a2) (c1, c2)), (mnozeni (a1, a2) (d1, d2))) 
             ((mnozeni (b1, b2) (c1, c2)), (mnozeni (b1, b2) (d1, d2))) 

(* funkcja która rozdziela działanie na czterech przedziałach na wykonywanie go każdy z każdym *)
let dzialanie f (((x1, x2), (x3, x4)) : wartosc) (((y1, y2), (y3, y4)) : wartosc)=
      scal (scal (f (x1, x2) (y1, y2)) (f (x1,x2) (y3, y4))) 
      (scal (f (x3, x4) (y1, y2)) (f (x3, x4) (y3, y4)))


(*MODYFIKATORY*)

let plus (((x1, x2), (x3, x4)) : wartosc) (((y1, y2), (y3, y4)) : wartosc) = 
      if (is_empty x1 x2 || is_empty y1 y2) then ((nan, nan), (nan, nan)) else
      (dzialanie (plus_pom) ((x1, x2), (x3, x4)) ((y1, y2), (y3, y4)) : wartosc)
     
let minus (((x1, x2), (x3, x4)) : wartosc) (((y1, y2), (y3, y4)) : wartosc) =
      if (is_empty x1 x2 || is_empty y1 y2) then ((nan, nan), (nan, nan)) else
      plus ((x1, x2), (x3, x4)) (((-1.0) *. y2, (-1.0) *. y1), ((-1.0) *. y4, (-1.0) *. y3))

let razy (((x1, x2), (x3, x4)) : wartosc) (((y1, y2), (y3, y4)) : wartosc) = 
      if (is_empty x1 x2 || is_empty y1 y2) then ((nan, nan), (nan, nan)) else
      (dzialanie (razy_pom) ((x1, x2), (x3, x4)) ((y1, y2), (y3, y4)) : wartosc)

(* funkcje pomocnicze dzielenia *)

(* funkcja zamieniajaca dzielenie dwóch przedziałów na mnożenie razy odwrotność drugiego *)
let dziel (x1, x2) (y1, y2) =
      razy_pom (x1, x2) (1. /. y2, 1. /. y1)

(* funkcja która rozdziela przedziały wobec zera i wykonuje ich dzielenie *)
let dziel_pom (x1, x2) (y1, y2) =
      if is_zero (y1, y2) then ((nan, nan), (nan, nan)) else
      if is_zero (x1, x2) then ((0., 0.), (nan, nan)) else
      let ((a1, a2), (b1, b2)) = podziel (x1, x2) and ((c1, c2), (d1, d2)) = podziel (y1, y2) in
      dzialanie (dziel) ((a1, a2), (b1, b2)) ((c1, c2), (d1, d2))

let podzielic (((x1, x2), (x3, x4)) : wartosc) (((y1, y2), (y3, y4)) : wartosc) =
      if (is_empty x1 x2 || is_empty y1 y2) then ((nan, nan), (nan, nan)) else
      if (y1, y2)=(0., 0.) then ((nan, nan), (nan, nan)) else
      (dzialanie (dziel_pom) ((x1, x2), (x3, x4)) ((y1, y2), (y3, y4)) : wartosc)