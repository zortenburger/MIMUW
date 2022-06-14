(* leftist *)
(* autor:Zuzanna Ortenburger *)

type 'a queue = Leaf | Node of 'a queue * 'a * 'a queue

let empty = Leaf

exception Empty

let is_empty q =
	match q with
	|Leaf -> true
	|_    -> false

let left q =
	match q with
	|Leaf -> raise (Empty)
	|Node (x, _, _) -> x


let right q =
	match q with
	|Leaf -> raise (Empty)
	|Node (_, _, x) -> x


let root q =
	match q with
	|Leaf -> raise (Empty)
	|Node (_, x, _) -> x

let v_root q =
	match q with
	|Leaf -> raise (Empty)
	|Node (_, x, _) -> fst x

let d_root q =
	match q with
	|Leaf -> 0
	|Node (_, x, _) -> snd x

let new_node q1 =
	match q1 with
	|Leaf -> Leaf
	|_ -> Node (left q1, (v_root q1, 1 + d_root q1), right q1)

let rec join q1 q2 =
	match (q1, q2) with
	|(Leaf, Leaf) -> Leaf
	|(Leaf, _) -> q2
	|(_, Leaf) -> q1
	|(_, _) ->
	if (v_root q1 > v_root q2) then join q2 q1 else 
	let d3 = new_node (left q1) in
	let d4 = new_node (join (right q1) q2) in 
	if (d_root d3 > d_root d4) then Node (d3, root q1, d4) else Node (d4, root q1, d3)
	

let add a q =
	join (Node (Leaf,(a,0),Leaf)) q

let delete_min q = 
	match q with
	|Leaf -> raise (Empty)
	|Node (d1, e, d2) -> (fst e, join d1 d2)