;; Comp105 scheme assignment

;; You will be implementing these functions.
(check-expect (procedure? count) #t)
(check-expect (procedure? countall) #t)
(check-expect (procedure? mirror) #t)
(check-expect (procedure? flatten) #t)
(check-expect (procedure? contig-sublist?) #t)
(check-expect (procedure? sublist?) #t)
(check-expect (procedure? takewhile) #t)
(check-expect (procedure? dropwhile) #t)
(check-expect (procedure? zip) #t)
(check-expect (procedure? unzip) #t)
(check-expect (procedure? arg-max) #t)
(check-expect (procedure? merge) #t)
(check-expect (procedure? permutation?) #t)
(check-expect (procedure? split-list) #t)

;; note to staff: template headers generated by `template-header`


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 2a
;;
;; (count x xs) is given an atom, x, and a list of expressions, 
;; xs, and returns the number of top-level elements of xs that are equal to x.
;; It is assumed that x is not nil.


(define count (x xs)
        (if (null? xs)
                0
                (if (= x (car xs))
                        (+ 1 (count x (cdr xs)))
                        (count x (cdr xs)))))
                 
(check-expect (count 'a '()) 0)          ;; xs is nil
(check-expect (count 'a '(a)) 1)         ;; x is the only element of xs
(check-expect (count 'a '(a b a)) 2)     ;; multiple elements of xs equal to x
(check-expect (count 'a '(a b (a b))) 1) ;; non-top-level xs element equal to x

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 2b
;;
;; (countall x xs) is given an atom, x, and a list of expressions, xs, and 
;; returns the number of times x occurs anywhere in xs. It is assumed 
;; that x is not nil.


(define countall (x xs)
        (if (null? xs)
                0
                (if (atom? (car xs))
                        (if (= x (car xs))
                                (+ 1 (countall x (cdr xs)))
                                (countall x (cdr xs)))
                        (+ (countall x (car xs)) (countall x (cdr xs))))))

(check-expect (countall 'a '()) 0)            ;; xs is nil
(check-expect (countall 'a '(a)) 1)           ;; x is the only element of xs
(check-expect (countall 'a '(a b (a b))) 2)   ;; non-top-level xs element = x
(check-expect (countall 'a '(a b (a b) a)) 3) 
                                ;; top-level xs element equals to x
                                ;; non-top-level xs element = x

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 2c
;;
;; (mirror xs) is given a list of S-expressions, xs, and returns a list in which
;; every list in xs is recursively mirrored, and the resulting lists are in 
;; reverse order.

(define mirror (xs)
        (if (pair? xs)
                (append (mirror (cdr xs)) (list1 (mirror (car xs))))
                xs))

(check-expect (mirror '()) '())       ;; xs is nil
(check-expect (mirror '(a)) '(a))     ;; xs is a list with one element
(check-expect (mirror '(a a (b c) (b (a b)))) '(((b a) b) (c b) a a)) 
                                      ;; xs is list with non-top-level element



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 2d
;;
;; (flatten xs) is given a list of S-expressions and returns the list in the 
;; same order, but without the internal parentheses.

(define flatten (xs)
        (if (pair? xs)
                (append  (flatten (car xs)) (flatten (cdr xs)))
                (if (null? xs)
                        '()
                        (list1 xs))))

(check-expect (flatten '()) '())       ;; xs is the emptylist
(check-expect (flatten '(((a)))) '(a)) ;; xs has one non-top-level element
(check-expect (flatten '(a ((b c) d) (((e))))) '(a b c d e)) 
                                       ;; top-level and non-top-level elements

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 2e
;;
;; (contig-sublist? xs ys) is given two lists in the set LIST(ATOM) and 
;; determines whether the list xs is a contiguous subsequence of the list ys. 
;; It returns #t if so, and #f is not.
;;
;; (sublist-checker? xs ys) is given two lists and checks whether or not
;; the sublist of elements match each other in each list. 


(define sublist-checker? (xs ys)
        (if (null? xs) 
                #t      
                (if (equal? (car xs) (car ys))
                        (sublist-checker? (cdr xs) (cdr ys))
                        #f)))

(define contig-sublist? (xs ys)
        (if (null? xs)
                #t
                (if (null? ys)
                        #f
                        (if (equal? (car xs) (car ys))
                                (if (sublist-checker? (cdr xs) (cdr ys))
                                        #t
                                        (contig-sublist? xs (cdr ys)))
                                (contig-sublist? xs (cdr ys))))))

(check-expect (contig-sublist? '() '()) #t)          ;; xs is null
(check-expect (contig-sublist? '(1) '(3 2 1)) #t)  
                                        ;; one-element xs contig-sublist of ys
(check-expect (contig-sublist? '(1 3 5) '(1 2 3 4 5)) #f)
                                        ;; sublist but not contiguous
(check-expect (sublist-checker? '(1) '(1 2)) #t)     ;; sublist contiguous
(check-expect (contig-sublist? '(1 2 2) '(1 2 3 1 2 2 4)) #t)
                                        ;; long test 


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 2f
;;
;; (sublist xs ys) is given two lists in the set LIST(ATOM) and determines 
;; whether the list xs is a mathematical subsequence of the list ys, returning 
;; true or false statements. The list ys must contain the elements of xs in 
;; the same order to return a true statement. It assumes both xs and ys are 
;; lists of atoms.


(define sublist? (xs ys)
        (if (or (null? xs) (null? ys))
                (null? xs)
                (if (equal? (car xs) (car ys))
                        (sublist? (cdr xs) (cdr ys))
                        (sublist? xs (cdr ys)))))

(check-expect (sublist? '() '()) #t)  ;; xs and ys are emptylist 
(check-expect (sublist? '() '(a)) #t) ;; xs emptylist, ys nonempty
(check-expect (sublist? '(1 3 5) '(1 2 3 4 5)) #t) ;; xs sublist of ys, in order
(check-expect (sublist? '(1 3 5) '(5 4 3 2 1)) #f) 
                                      ;; xs sublist of ys, not in order


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 10
;;
;; (takewhile p? xs) takes a predicate and a list and returns the longest 
;; prefix of the list in which every element satisfies the predicate.
;;
;; (even x) if given an integer, x, and returns #t or #f if x is even.

(define even? (x) (= (mod x 2) 0))

(define takewhile (p? xs)
        (if (null? xs)
                xs
                (if (p? (car xs))
                        (append (list1(car xs)) (takewhile p? (cdr xs)))
                        '())))

(check-expect (takewhile even? '()) '()) ;; xs is null
(check-expect (takewhile even? '(2 3)) '(2)) ;; one-element prefix
(check-expect (takewhile even? '(3 2)) '())  ;; no prefix
(check-expect (even? 1) #f) ;; false case
(check-expect (even? 2) #t) ;; true case

;; (dropwhile p? xs) takes a predicate and a list, removes the longest prefix,
;; and returns whatever is left over.

(define dropwhile (p? xs)
        (if (null? xs)
                xs
                (if (p? (car xs))
                        (dropwhile p? (cdr xs))
                        xs)))
                
(check-expect (dropwhile even? '()) '())     ;; xs is null
(check-expect (dropwhile even? '(2 3)) '(3)) ;; one-element prefix
(check-expect (dropwhile even? '(3 2 4)) '(3 2 4)) ;; no prefix

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 30
;;
;;
;; a) The function vector-length calculates the Pythagorean Theorem. It takes
;;    two integers, squares each number, sums the results, and returns the 
;;    square root of the sum.
;;    
;; b) This function works because, according to the let rule, all the 
;;    bound expressions are evaluated first and then the names are binded
;;    to the resulting values. Next, the main expression is evaluated. Thus,
;;    in vector-length, the addition and multiplication expressions are 
;;    altered first and then the math at the bottom of the function is
;;    executed. 
;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem B (zip)
;;
;; (zip xs ys) is given a pair of lists and converts them to an association
;; list. It is assumed that the lists are of equal length.

(define zip (xs ys)
        (if (null? xs)
                xs
                (append (bind (car xs) (car ys) '()) (zip (cdr xs) (cdr ys)))))
        
(check-expect (zip '() '()) '())                         ;; xs and ys are null
(check-expect (zip '(1 2 3) '(a b c)) '((1 a) (2 b) (3 c))) ;; 3-element lists


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem B (unzip)
;;
;; (unzip xs) is given an association list and converts it to a pair of lists.
;;
;; (distribute front back xs) is given three lists and uses xs to accumulate
;; elements in the front and back lists, returning those.

(define distribute (front back xs)
        (if (null? xs)
                (list2 front back)
                (distribute (append front (list1(caar xs))) 
                                (append back (cdar xs)) (cdr xs))))

(define unzip (xs)
        (distribute '() '() xs))

(check-expect (unzip '()) '(() ())) ;; null xs return unzipped version
(check-expect (unzip '((1 a) (2 b) (3 c))) '((1 2 3) (a b c))) 
                                    ;; unzip association lists
(check-expect (distribute '() '() '((1 a) (2 b) (3 c))) '((1 2 3) (a b c)))
                                    ;;distribute - unzup association lists


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem C
;;
;; (arg-max f as) takes two arguments, a function f that maps a value in a set 
;; to a number, and a nonempty list as of values in the set. It returns an 
;; element a in as for which (f a) is as large as possible.
;;
;; (compare f as max) takes the function f, the nonempty list as, and a value
;; of as signifying the max value. It compares each value of as and returns
;; the greatest value returned from the function f.
;;
;; (square a) is a function used to test arg-max and compare which takes a value
;; and returns its sqaure.
;;
;; (invert a) is a function used to test arg-max and compare which takes a value
;; and divides 1000 by the number.


(define square (a) (* a a))

(define invert (a) (/ 1000 a))

(define compare (f as max)
        (if (null? as)
                max
                (if (> (f (car as)) (f max))
                        (compare f (cdr as) (car as))
                        (compare f (cdr as) max))))

(define arg-max (f as)
        (compare f as (car as)))

(check-expect (arg-max square '(1)) '1) ;; square, as has one element
(check-expect (arg-max invert '(20 15 10 5)) '5) ;; invert, max value at end 
(check-expect (square 2) 4)       
(check-expect (invert 5) 200)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem D
;;
;; (merge xs ys) is given two lists of numbers sorted in increasing order and
;; returns a single list sorted in increasing order containing exactly the same
;; elements as the two argument lists together.

(define merge (xs ys)
        (if (null? xs)
                ys
                (if (null? ys)
                        xs
                        (if (< (car xs) (car ys))
                                (cons (car xs) (merge (cdr xs) ys))
                                (cons (car ys) (merge xs (cdr ys)))))))

(check-expect (merge '() '()) '())      ;; xs and ys nil
(check-expect (merge '(2 4 6) '(1 3 5)) '(1 2 3 4 5 6)) 
                                        ;; xs and ys need to be sorted
(check-expect (merge '(1) '(5)) '(1 5)) ;; xs and ys one element


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem G
;;
;; (permutation? xs ys) is given two lists, xs and ys, and decides if they are
;; permutations, that is they have exactly the same elements, but possibly
;; different orders.
;;
;; (remove x xs) is given an integer, x, and a list, xs, and removes x from xs.
;; If x is not a member of xs, the original list is returned.

(define remove (x xs)
        (if (null? xs)
                xs
                (if (equal? x (car xs))
                        (cdr xs)
                        (cons (car xs) (remove x (cdr xs))))))

(define permutation? (xs ys)
        (if (or (null? xs) (null? ys))
                (and (null? xs) (null? ys))
                (permutation? (cdr xs) (remove (car xs) ys))))

(check-expect (permutation? '() '()) #t)   ;; xs and ys nil
(check-expect (permutation? '(a) '(a)) #t) ;; xs and ys same one element
(check-expect (permutation? '(a b) '(b a)) #t) ;; xs and ys different order
(check-expect (permutation? '(a b c) '(a b)) #f) ;; ys sublist of xs


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem H
;;
;; (split-list xs) takes a list xs and split it into two lists of nearly 
;; equal length. When the original length of xs is odd, the length of the 
;; two lists cannot differ by more than one.
;;
;; (create-lists xs length ys) takes two lists, xs and ys, and the length
;; of the current iteration of xs. It returns two lists of nearly equal length.

(define create-lists (xs length ys)
        (if (= length 0)
                (list2 xs ys)
                (create-lists (cdr xs) (- length 1) 
                                (append ys (list1 (car xs)))))) 

(define split-list (xs)
        (create-lists xs (/ (length xs) 2) '()))

(check-expect (split-list '()) '(() ())) ;; xs is nil
(check-expect (split-list '(1 2)) '((2) (1))) ;; xs has even length
(check-expect (split-list '(1 2 3)) '((2 3) (1))) ;; xs has odd length
