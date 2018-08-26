;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 14 (b)
;;
;; (max* xs) finds the maximum of a non-empty list of integers.  The function 
;; does not have a left or right identity.
;;
;;

(define max* (xs) (foldl max 0 xs))

(check-expect (max* '(1)) '1)       ;; xs is one element
(check-expect (max* '(1 2 5 5)) '5) ;; xs has recurring elements


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 14 (c)
;;
;; (gcd* xs) finds the greatest common divisor of a non-empty list of integers.
;; The function gcd has left and right identities.
;;
;;

(define gcd* (xs) (foldr gcd 0 xs))

(check-expect (gcd* '(1)) '1)        ;; xs is one element, which is 1
(check-expect (gcd* '(3 6 9)) '3)    ;; gcd is not 1
(check-expect (gcd* '(3 6 7 9)) '1)  ;; gcd is 1
(check-expect (gcd* '(5 5 5 5)) '5)  ;; xs all recurring numbers

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 14 (d)
;;
;; (lcm* xs) finds the least common multiple of a non-empty list of integers.
;; The function lcm has left and right identities. 
;;
;;

(define lcm* (xs) (foldr lcm 1 xs))

(check-expect (lcm* '(1)) '1)            ;; xs is one element, which is 1
(check-expect (lcm* '(3 1 9)) '9)        ;; xs is all factors of 9
(check-expect (lcm* '(7 5 4 3 2)) '420)  ;; longer case
(check-expect (lcm* '(5 5 5 5)) '5)      ;; xs all recurring numbers

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 14 (e)
;;
;; (sum xs) finds the sum of a non-empty list of integers.
;;
;;

(define sum (xs) (foldr + 0 xs))

(check-expect (sum '(1)) '1)            ;; xs is one element, which is 1
(check-expect (sum '(3 2 1)) '6)        ;; sum = 6
(check-expect (sum '(5 5 5 5)) '20)     ;; xs all recurring numbers


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 14 (f)
;;
;; (product xs) finds the product of a non-empty list of integers.
;;
;;

(define product (xs) (foldr * 1 xs))

(check-expect (product '(0)) '0)            ;; xs is one element, which is 0
(check-expect (product '(3 2 1)) '6)        ;; product = 6
(check-expect (product '(5 5 5 5)) '625)    ;; xs all recurring numbers

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 14 (h)
;;
;; (append xs ys) appends two lists, xs and ys
;;


(define append (xs ys) (foldr cons ys xs))

(check-expect (append '() '()) '())     ;; xs and ys are emptylists
(check-expect (append '(a) '(b)) '(a b)) ;; xs and ys have one element
(check-expect (append '(1 2 3) '(4 5 6)) '(1 2 3 4 5 6)) ;; longer case


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 14 (j)
;;
;; (reverse xs) takes one list and reverse it's order
;;

(define reverse (xs) (foldl cons '() xs))

(check-expect (reverse '()) '())                   ;; base case
(check-expect (reverse '(a)) '(a))                 ;; xs is one element
(check-expect (reverse '(1 (2 3) 4)) '(4 (2 3) 1)) ;; list within list


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (length xs) takes a list, xs, and finds the length of xs.
;;

(define length (xs) (foldl (lambda (x a) (+ 1 a)) 0 xs))

(check-expect (length '()) 0)        ;; base case
(check-expect (length '(a)) 1)       ;; xs is one element
(check-expect (length '(1 2 1)) 3)   ;; longer case

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (map f xs) takes a function and a list and returns the list formed by 
;; applying function f to every element of list xs.
;;

(define map (f xs) (foldr (lambda (x a) (cons (f x) a)) '() xs))

(check-expect (map ((curry +) 1) '()) '())           ;; xs is emptylist
(check-expect (map ((curry +) 1) '(1)) '(2))         ;; xs has one element
(check-expect (map ((curry +) 1) '(3 2 1)) '(4 3 2)) ;; xs multiple elements

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (filter p? xs) takes a predicate p? and a list xs and returns a new list 
;; consisting of only those elements in xs that satisfy p? 
;;

(define filter (p? xs) (foldr (lambda (x a) (if (p? x) (cons x a) a)) '() xs))

(define even? (x) (= (mod x 2) 0))

(check-expect (filter even? '()) '())       ;; xs is emptylist
(check-expect (filter even? '(1)) '())      ;; xs has one odd element
(check-expect (filter even? '(2)) '(2))     ;; xs has one even element
(check-expect (filter even? '(1 2 3)) '(2)) ;; xs has even and odd elements

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (exists p? xs) takes a predicate p? and a list xs and decides whether there
;; is an element of the list satisfying the predicate 
;;

(define exists? (p? xs) (foldr (lambda (x a) (or (p? x) a)) #f xs))

(define even? (x) (= (mod x 2) 0))

(check-expect (exists? even? '()) #f)     ;; xs is emptylist
(check-expect (exists? even? '(1)) #f)     ;; xs has one odd element
(check-expect (exists? even? '(2)) #t)    ;; xs has one even element
(check-expect (exists? even? '(1 2 3)) #t) ;; xs has even and odd elements

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (all p? xs) takes a predicate p? and a list xs and decides whether all the
;; elements of the list satisfy the predicate
;;

(define all? (p? xs) (foldr (lambda (x a) (and (p? x) a)) #t xs))

(define even? (x) (= (mod x 2) 0))

(check-expect (all? even? '()) #t)      ;; xs is emptylist
(check-expect (all? even? '(1)) #f)     ;; xs has one odd element
(check-expect (all? even? '(2 4 6)) #t) ;; xs all even elements
(check-expect (all? even? '(2 3 4)) #f) ;; xs has even and odd elements

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 19 (a)
;;
;; The set evens contains all teh even integers.
;;

(val evens (lambda (x) (= (mod x 2) 0)))

(define member? (x s) (s x))
(check-expect (member? '0 evens) #t)   ;; x = 0
(check-expect (member? '1 evens) #f)   ;; x = 1
(check-expect (member? '50 evens) #t) ;; larger case

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 19 (b)
;;
;; The set two-digits contains all two-digit (positive) numbers.
;;

(val two-digits (lambda (x) (and (> x 9) (< x 100))))

(define member? (x s) (s x))
(check-expect (member? '9 two-digits) #f)   ;; x = 9, low test case
(check-expect (member? '10 two-digits) #t)  ;; x = 10, low test case
(check-expect (member? '99 two-digits) #t)  ;; x = 99, high test case
(check-expect (member? '100 two-digits) #f) ;; x = 100, high test case

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 19 (c)
;; 
;; (add-element x s) is the set that contains s and the element x
;;

(define add-element (x s) (lambda (a) (or (s a) (equal? x a))))  

(check-expect (member? '1 (add-element '1 evens)) #t) 
                                            ;; value checked is added
(check-expect (member? '1 (add-element '3 evens)) #f) 
                                            ;; value checked not added

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (union s1 s2) is the set that contains the union of s1 and s2
;;

(define union (s1 s2) (lambda (a) (or (s1 a) (s2 a))))

(check-expect (member? '1 (union evens (o not evens))) #t) ;; value in set
(check-expect (member? '1 (union evens two-digits)) #f)    ;; value not in set

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (inter s1 s2) is the set that contains the intersection of s1 and s2
;;

(define inter (s1 s2) (lambda (a) (and (s1 a) (s2 a))))

(check-expect (member? '11 (inter evens two-digits)) #f) ;; value not in set
(check-expect (member? '12 (inter evens two-digits)) #t) ;; value in set

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (differ s1 s2) is the set that contains every element of s1 
;; that is not also in s2
;;

(define diff (s1 s2) (lambda (a) (and (s1 a) (not (s2 a)))))

(check-expect (member? '2 (diff evens two-digits)) #t)  ;; value only in s1
(check-expect (member? '12 (diff evens two-digits)) #f) ;; value in s1 and s2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 19 (d)
;;
;; (set-ops-from) is given an equality predicate, eq?, and returns a list
;; of six values, which are operations that either construct or interrogate
;; a set made with the given equality predicate. Each operation is implemented
;; using the third approach to polymorphism.
;;



(define set-ops-from (eq?)
        (let ([empty (lambda (s) #f)]
              [member? (lambda (y s) (s y))]
              [add (lambda (x s) (lambda (y) (or (s y) (eq? y x))))]
              [union (lambda (s1 s2) (lambda (y) (or (s1 y) (s2 y))))]
              [inter (lambda (s1 s2) (lambda (y) (and (s1 y) (s2 y))))]
              [diff (lambda (s1 s2) (lambda (y) (and (s1 y) (not (s2 y)))))])
           (list6 empty member? add union inter diff)))

(check-assert (procedure? set-ops-from))
(check-expect (length (set-ops-from =)) 6)

(val atom-set-ops (set-ops-from =))

(val empty2 (car atom-set-ops))
(val member2? (cadr atom-set-ops))
(val add-element2 (caddr atom-set-ops))
(val union2 (car (cdddr atom-set-ops)))
(val inter2 (cadr (cdddr atom-set-ops)))
(val diff2 (caddr (cdddr atom-set-ops)))

(check-expect (empty2 evens) #f)

(check-expect (member2? 10 evens) #t)
(check-expect (member2? 11 evens) #f)

(check-expect (member2? 11 (add-element2 1 evens)) #f)
(check-expect (member2? 11 (add-element2 11 evens)) #t)

(check-expect (member2? 8 (union2 evens two-digits)) #t)
(check-expect (member2? 7 (union2 evens two-digits)) #f)
(check-expect (member2? 55 (union2 evens two-digits)) #t)

(check-expect (member2? 8 (inter2 evens two-digits)) #f)
(check-expect (member2? 12 (inter2 evens two-digits)) #t)
(check-expect (member2? 11 (inter2 evens two-digits)) #f)

(check-expect (member2? 8 (diff2 evens two-digits)) #t)
(check-expect (member2? 10 (diff2 evens two-digits)) #f)
(check-expect (member2? 55 (diff2 evens two-digits)) #f) 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem A
;;
;; (f-functional y) is the equivalent uScheme function of f-imperative.
 
(define f-functional (y)
        (let 
                ([cycle (lambda (x)
                          (if (p? x y)
                                (cycle (g x y))
                                (h x y)))])
          (cycle e)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem G1
;;
;; (=edge-list el1 el2) takes two edge lists, el1 and el2, and returns a
;; Boolean indicating whether they represent the same graph.
;;
;; (list-mem? x el) takes a value and an edge list and returns whether
;; the value is in the list.
;; 

(define list-mem? (x xs)
        (if (null? xs)
                #f
                (if (equal? x (car xs))
                        #t
                        (list-mem? x (cdr xs)))))

(define =edge-list? (xs ys)
        (let ([compare (lambda (xs ys)
                        (foldr (lambda (x a)
                                (and (list-mem? x xs) a )) #t ys))])
                (if (compare xs ys) (compare ys xs) #f)))

;; Graphs
(check-expect (=edge-list? '((A B) (A C) (B C)) '((A B) (B C) (A C))) #t)
(check-expect (=edge-list? '((A B) (A C)) '((A C) (A B))) #t)
(check-expect (=edge-list? '((A B) (A C) (B C) (A A)) 
                                        '((A C) (B C) (A A) (A B))) #t)

;; Graph 1 and 2 check
(check-expect (=edge-list? '((A B) (A C) (B C)) '((A B) (A C))) #f) 

;; Graph 1 and 3 check
(check-expect (=edge-list? '((A B) (A C) (B C)) 
                                '((A B) (A C) (B C) (A A))) #f)

;; Graph 2 and 3 check
(check-expect (=edge-list? '((A B) (A C)) '((A B) (A C) (B C) (A A))) #f)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;; Problem G2
;;
;; (=successors-map? al1 al2) takes two successors maps and returns a 
;; Boolean indicating whether they represent that same graph.
;;

(define =successors-map? (al1 al2)
        (let ([sub-map? (lambda (al1 al2) 
                                (all? (lambda (pair)
                                        (=edge-list? 
                                            (alist-pair-attribute pair)
                                         (find (alist-pair-key pair) al2)))
                                        al1))])
                (if (sub-map? al1 al2) (sub-map? al2 al1) #f)))  

(check-expect (=successors-map? '((A (B C)) (B (C)) (C ())) 
                                        '((A (C B)) (B (C)) (C ()))) #t)
(check-expect (=successors-map? '((A (B C)) (B ()) (C ())) 
                                        '((B ()) (A (C B)) (C ()))) #t)   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;; Problem G3 (a)
;;
;; (successors-map-of-edge-list xs) accepts a graph in edge-list 
;; representation and returns a representation of the same graph in 
;; successors-map representation. The let functions are used to 
;; alter the list in appropriate ways to find the computation.
;;

(define successors-map-of-edge-list (xs)
        (letrec ([convert (lambda (l)
                                (foldr (lambda (x a) 
                                        (if (list_mem? x a)
                                                a 
                                                (cons (list2 x '()) a)))
                                        '() (flatten l)))]
                 [add-successors (lambda (xs l)
                                  (foldr (lambda (x a) 
                                          (bind (car x) 
                                              (cons (cadr x) 
                                                    (find 
                                                        (alist-pair-key x) a))
                                                    a))
                                        l xs))]
                 [flatten (lambda (xs) 
                           (if (pair? xs)
                                (append (flatten (car xs)) (flatten (cdr xs)))
                                (if (null? xs)
                                        '()
                                        (list1 xs))))]
                 [list_mem? (lambda (x xs)       
                             (if (null? xs)
                                #f
                                (if (or (equal? x (caar xs)) 
                                                (equal? x (cdar xs)))
                                        #t
                                        (list_mem? x (cdr xs)))))])        
         (add-successors xs (convert xs))))

(check-expect (successors-map-of-edge-list '((A B) (A C) (B C)))
                                                '((A (B C)) (B (C)) (C ())))
(check-expect (successors-map-of-edge-list '((A B) (A C) (A A)))
                                                '((B ()) (C ()) (A (B C A))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem G3 (b)
;;
;; (edge-list-of-successors-map xs) accepts a graph in successors-map 
;; representation and returns a representation of the same graph in
;; edge-list represntation.
;;

(define edge-list-of-successors-map (xs)
        (letrec 
                ([edge (lambda (x xs ys)
                        (if (null? xs) 
                            ys 
                            (edge x (cdr xs) (cons (list2 x (car xs)) ys))))]
                 [list-maker (lambda (l) (foldr (lambda (l ys) 
                                                 (edge (car l) (cadr l) ys)) 
                                            '() l))])
         (list-maker xs)))

(check-expect (edge-list-of-successors-map '((A (B C)) (B (C)) (C ())))
                                                '((A C) (A B) (B C)))

(check-expect (edge-list-of-successors-map '((B ()) (C ()) (A (B C A))))
                                                '((A A) (A C) (A B)))

