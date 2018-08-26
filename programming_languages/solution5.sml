(*  Name: Jared R. Lieberman
    Date due: 25 October 2017
    Comp105 Assignment 6: Core ML
    File: warmup.sml
    Purpose: Contains solutions to many problems.
    Hours spent on project: 25 hours
    Collaborators: Jordan Stone, Hazen Breen, Madeline Payne, Eric Maghalaes,
    Ean Mao. I received TA help from Kell, Lily, Alex, and Lexi. *)

(***** Problem A *****)

(* null, when applied to a list, tells whether the list is empty.*)

fun null [] = true
    | null (x::xs) = false


(***** Problem C *****)

(* firstVowel takes a list of lower-case letters and returns true if the 
   first character is a vowel (aeiou) and false if the first character is not
   a vowel or if the list is empty.*)


fun firstVowel (#"a"::xs) = true
    | firstVowel (#"e"::xs) = true
    | firstVowel (#"i"::xs) = true
    | firstVowel (#"o"::xs) = true
    | firstVowel (#"u"::xs) = true
    | firstVowel _ = false


(***** Problem E *****)

(* rev takes a list and returns a list in the reverse order of the 
   original list. It raises a fail exception if it is given an empty list.*)

exception Match

fun rev (x::xs) = foldl (fn (y, ys) => y::ys) [] (x::xs)
    | rev [] = raise Match


(* minlist returns the smallest element of a non-empty list of integers.
   It raises a fail exception if it is given an empty list.*)

fun minlist (x::xs) = foldl (fn (x, y) => Int.min (x, y)) x (x::xs)
    | minlist [] = raise Match


(***** Problem F *****)

(* foldr associates the computation of a function to the right. This is
    implemented using recursion without if. *)

fun foldr f zero (x::xs) = f (x, (foldr f zero xs))
    | foldr f zero [] = zero


(***** Problem G *****)

(* zip takes a pair of lists (of equal length) and returns the equivalent
   list of pairs. If the lengths don't match, raise the exception Mismatch *)

exception Mismatch

fun zip ([],    [])    = []
   | zip (x::xs, y::ys) = (x,y) :: zip (xs,ys)
   | zip _              = raise Mismatch


(***** Problem H *****)

(*  pairfoldr applies a three-argument function to a pair of lists of equal
    length, using the same order as foldr. *)

fun pairfoldr f zero ((x::xs), (y::ys)) = f (x, y, (pairfoldr f zero (xs, ys)))
    | pairfoldr f zero ([], []) = zero
    | pairfoldr f zero _ = raise Mismatch

(* zip2 is a version of zip that uses pairfoldr for its implementation *)

fun zip2 (xs, ys) = pairfoldr (fn (x, y, zero) => (x,y)::zero) [] (xs, ys)


(***** Problem I *****)

(* unzip turns a list of pairs into a pair of lists. *)

fun unzip pairs =
    let fun fst (x, y) = x
        fun snd (x, y) = y
    in  (map fst pairs, map snd pairs)
    end


(***** Problem K *****)

(*  compound takes three arguments. The first is a function which takes two
    arguments of the same type and returns a result also of that type. 
    compound also takes an integer n, which determines how many times to apply
    the function to x, the value first being applied to. *)

(* algebraic laws for compound *)

(*  (compound (op +) 0 1)  = 1
    (compound (op +) 20 1) = 21
    (compound (op * ) 5 2) = 64 *)

fun compound f 0 x = x
    | compound f n x = f (x, compound f (n - 1) x)

(*  exp is a curried function for integer exponentiation that takes base and
    exponent and exponent arguments *)

fun exp b e = compound (op * ) (e - 1) b


(***** Problem L *****)

(*  nth takes an integer n and a list and returns the nth element of the list.
    If the given arguments are not appropriate, it raises the 
    Empty exception. *)

exception Empty

fun nth 0 (x::xs)   = x
    | nth n (x::xs) = nth (n - 1) xs
    | nth _ _       = raise Empty 


(***** Problem N *****)

datatype 'a tree = NODE of 'a tree * 'a * 'a tree 
                 | LEAF

fun insert cmp =
    let fun ins (x, LEAF) = NODE (LEAF, x, LEAF)
          | ins (x, NODE (left, y, right)) = 
              (case cmp (x, y)
                of LESS    => NODE (ins (x, left), y, right)
                 | GREATER => NODE (left, y, ins (x, right))
                 | EQUAL   => NODE (left, x, right))
    in  ins
    end

datatype 'a set = SET of ('a * 'a -> order) * 'a tree
fun nullset cmp = SET (cmp, LEAF)

(* addelt takes an element and a set and adds the element to that set *)

fun addelt (elem, (SET (cmp, tree))) = SET(cmp, (insert (cmp)) (elem, tree))

val addelt : 'a * 'a set -> 'a set = addelt

(* treeFoldr folds a function over every element of a tree, rightmost
   element first. *)

fun treeFoldr f zero LEAF = zero
    | treeFoldr f zero (NODE (left, root, right)) = 
        treeFoldr f (f (root, (treeFoldr f zero right))) left

val treeFoldr : ('a * 'b -> 'b) -> 'b -> 'a tree -> 'b = treeFoldr

(* setFold visits every element of the set exactly once *)

fun setFold f zero (SET (cmp, LEAF)) = zero
    | setFold f zero (SET (cmp, NODE (left, root, right))) =
         treeFoldr f (f (root, (treeFoldr f zero right))) left

val setFold : ('a * 'b -> 'b) -> 'b -> 'a set -> 'b = setFold


(***** Problem P *****)

type 'a flist = 'a list * 'a * 'a list

(* singletonOf takes value of type 'a and returns a 'a flist with one*)

val singletonOf : 'a -> 'a flist = (fn value => ([], value, []))

(* atFinger returns the value that the finger points at *)

val atFinger : 'a flist -> 'a = (fn (_, finger, _) => finger)

exception Size

(* fingerLeft creates a new list in which the finger is moved one position 
   to the left. If the finger already points to the leftmost position,
   the Size exception is raised. *)

fun fingerLeft (l::lefts, finger, rights) = (lefts, l, finger::rights)
    | fingerLeft ([], finger, rights) = raise Size

val fingerLeft  : 'a flist -> 'a flist = fingerLeft

(* fingerRight creates a new list in which the finger is moved one position 
   to the right. If the finger already points to the rightmost position,
   the Size exception is raised. *)

fun fingerRight (lefts, finger, r::rights) = (finger::lefts, r, rights)
    | fingerRight (lefts, finger, []) = raise Size

val fingerRight : 'a flist -> 'a flist = fingerRight

(* deleteLeft creates a new list in which the value to the left of the finger
   has been removed. If the finger already points to the leftmost position,
   the Size exception is raised. *)

fun deleteLeft (l::lefts, finger, rights) = (lefts, finger, rights)
    | deleteLeft ([], finger, rights) = raise Size

val deleteLeft : 'a flist -> 'a flist = deleteLeft

(* deleteRight creates a new list in which the value to the right of the finger
   has been removed. If the finger already points to the rightmost position,
   the Size exception is raised. *)

fun deleteRight (lefts, finger, r::rights) = (lefts, r, rights)
    | deleteRight (lefts, finger, []) = raise Size

val deleteRight : 'a flist -> 'a flist = deleteRight

(* insertLeft creates a new list in which the value is inserted to the left 
   of the finger.*)

fun insertLeft (x, (lefts, finger, rights)) = (x::lefts, finger, rights)

val insertLeft  : 'a * 'a flist -> 'a flist = insertLeft

(* insertRight creates a new list in which the value is inserted to the right 
   of the finger.*)

fun insertRight (x, (lefts, finger, rights)) = (lefts, finger, x::rights)

val insertRight : 'a * 'a flist -> 'a flist = insertRight

(* ffoldl does the same thing as foldl but ignores the position of the 
   finger *)

fun ffoldl f zero (lefts, finger, rights) = 
            foldl f (foldr f zero lefts) (finger::rights)

val ffoldl : ('a * 'b -> 'b) -> 'b -> 'a flist -> 'b = ffoldl

(* ffoldr does the same thing as foldl but ignores the position of the 
   finger *)

fun ffoldr f zero (lefts, finger, rights) = 
            foldl f (foldr f zero rights) (finger::lefts)

val ffoldr : ('a * 'b -> 'b) -> 'b -> 'a flist -> 'b = ffoldr


