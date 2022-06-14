(*Sortowanie topologiczne*)
(*autor: Zuzanna Ortenburger*)
(*recenzja: Maciej Bielik*)

exception Cykliczne

open PMap

(* dla listy l zwraca liste wyn z pojedynczymi wystapieniami elementow z listy l
i mape ktora dla elementow z l zwraca ich indeks na liscie wyn*)
let make_nodes l = 
    let mapa = ref empty in (*mapa zamieniająca elementy na identyfikatory*)
    let n = List.length l in
    let lis = Array.of_list l in
    let wyn = ref [] in (*lista odwrotna do mapy zawierajaca elementy na miejscach identyfikatorów*)
    let j = ref 0 in (*zwiększający się licznik do identyfikatorów*)
    for i = 0 to n-1 do
        if not (exists (lis.(i)) !mapa) then (*nowy element który nie ma identyfikatora*)
            begin 
            wyn := (lis.(i))::!wyn;
            mapa := add  (lis.(i)) !j !mapa;
            j := 1 + !j;
            end;
    done;
    (wyn, mapa)

(* dla listy lista oraz mapy mapa tworzy graf reprezentowany za pomoca list sąsiedztwa
z zachowaniem relacji na liscie lista*)
let make_graph lista mapa n =
    let graph = Array.make n ([]) in 
    (* pomocnicza funkcja dodająca krawędzie dla danego wierzchołka *)
    let rec edges lis (i : int) = 
        match lis with
        |[] -> ()
        |h::l -> 
            begin 
                graph.(i) <- (find h !mapa)::(graph.(i));
                edges l i;
            end
    in
    let rec loop lista =
        match lista with
        |[] -> ()
        |h::l -> 
            let akt = find (fst h) !mapa in
            begin
            edges (snd h) akt;
            loop l;
            end
    in loop lista;
    graph

(** Dla danej listy [(a_1,[a_11;...;a_1n]); ...; (a_m,[a_m1;...;a_mk])] 
    zwraca liste, na ktorej kazdy z elementow a_i oraz a_ij wystepuje
    dokladnie raz i ktora jest uporzadkowana w taki sposob, ze kazdy
    element a_i jest przed kazdym z elementow a_i1 ... a_il *)
let topol l =
    let (nodes1,nodes2) = List.split l in
    let (nodes3,mapa) = make_nodes (nodes1 @ (List.flatten nodes2)) in 
    let nodes = Array.of_list (List.rev !nodes3) in
    let n = Array.length nodes in
    let graph = make_graph l mapa n in
    let odw = Array.make n 0 in
    let wyn = ref [] in 
    let rec dfs k =
        let akt = odw.(k) in
        if akt = 0 then (* wierzchołek jeszcze nie był odwiedzony*)
            begin
                odw.(k) <- 1;
                List.iter dfs (graph.(k));
                wyn := (nodes.(k))::(!wyn);
                odw.(k) <- 2;
            end;
        if akt = 1 then (* wierzchołek jest w trakcie przeglądania*)
            raise Cykliczne;
        if akt = 2 then () (* przeglądanie wierzchołka zostało zakończone*)
        in 
    for i = 0 to n-1 do
        if odw.(i) = 0 then dfs i;
    done;
     !wyn;;