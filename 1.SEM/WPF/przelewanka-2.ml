(*Przelewanka*)
(*autor: Zuzanna Ortenburger *)
(*recenzja: Wojciech Weremczuk gr5*)
let przelewanka l = 

    (* usuniecie (0,x) z tablicy*)
    let wejscie_lst = List.filter (fun (x,_) -> if x > 0 then true else false) (Array.to_list l) in
    let wejscie = Array.of_list wejscie_lst in
    (* inicjalizacja zmiennych i tablic pomocniczych *)
    let odw = Hashtbl.create 100 (* zapamiętywanie odwiedzonych stanów *)
    and q = Queue.create () (* kolejka stanów do przeszukanie potrzebna do bfs-a postaci (x,y) gdzie x to stan a y to ilosc ruchów potrzebna do uzyskania go*)
    and wyn = ref (-1) (* wynik *)
    and n = Array.length wejscie (* ilość szklanek *) in
    let szklanki = Array.make n 0 (* poczatkowy stan *)
    and koncowy = Array.make n 0 (* oczekiwana pojemnosc szklanek *)
    and poj = Array.make n 0 (* pojemności szklanek *) 
    in
    for i=0 to n - 1 do
      koncowy.(i) <- snd wejscie.(i);
      poj.(i) <- fst wejscie.(i);
    done;

    (* zaznacz odwiedzenie *)
    let dodaj akt ile =
      if not (Hashtbl.mem odw akt) 
          then 
          begin
            Hashtbl.add odw akt true;
            Queue.add (akt, ile+1) q;
          end;
      in

    (* kopiowanie tablicy *) 
    let skopiuj stan =
      let akt = Array.make n 0 in
          for j = 0 to n-1 do
            akt.(j) <- stan.(j);
          done;
      akt
    in


    (* napełnianie szklanki z kranu *)
    let op1 stan ile = 
      for i = 0 to n-1 do
        if stan.(i) < poj.(i) then begin
        let akt = skopiuj stan in 
        akt.(i) <- poj.(i);
        dodaj akt ile;
        end;
      done;
    in

    (* wylanie wody ze szklanki *)
    let op2 stan ile = 
      for i = 0 to n-1 do
        if stan.(i) > 0 then begin
        let akt = skopiuj stan in 
        akt.(i) <- 0;
        dodaj akt ile;
        end;
      done;
    in

    (* przelanie ze szklanki i do j*)
    let op3 stan ile = 
      for i = 0 to n-1 do
        if stan.(i) > 0 then begin
        for j = 0 to n-1 do 
          if (i <> j && stan.(j) < poj.(j)) then begin 
            let akt = skopiuj stan in 
            let przelane = min (akt.(i)) (poj.(j) - akt.(j)) in 
            akt.(j) <- (akt.(j) + przelane);
            akt.(i) <- (akt.(i) - przelane);
            dodaj akt ile;
          end;
        done;
        end;
      done;
    in
    
    (* funkcja licząca nwd dwóch liczb *)
    let nwd a b =
      let rec e a b =
        if b = 0 then a else e b (a mod b)
      in
    if a > b then e a b else e b a
    in

    (* funkcje sprawdzające warunki konieczne aby mozna bylo wykonac*)
    let mozliwe l = Array.fold_left (fun acc (x,y) -> if (x = y || y = 0) then true else acc) false l in (*sprawdzenie czy po wszystkich ruchach jakaś szklanka jest pełna lub pusta *)
    let nwd_wszystkich l = Array.fold_left (fun acc x -> nwd acc x) 0 l in (* obliczam nwd wszystkich pojemności szklanek, przy przelewaniu tylko jego wielokrotnosci mozemy uzyskac *)
    let czy_podzielne l x = Array.fold_left (fun acc y -> if (acc && y mod x = 0) then true else false) true l in (* sprawdzam czy kazda oczekiwana ilość jest podzielna przez jak liczbe *)

  if (szklanki = koncowy || wejscie = [||]) then 
    wyn := 0
  else 
  if(mozliwe wejscie && czy_podzielne koncowy (nwd_wszystkich poj)) then 
  begin
    Hashtbl.add odw szklanki true;
    Queue.add (szklanki, 0) q;
    while !wyn = -1 && not (Queue.is_empty q) do
      let (akt, ile) = Queue.pop q in
      if koncowy = akt 
      then 
      begin
        wyn := ile;
      end;
      if koncowy <> akt then
      begin
        op1 akt ile;
        op2 akt ile;
        op3 akt ile;
      end;
    done;
  end;
  !wyn
;;