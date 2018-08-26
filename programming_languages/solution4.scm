;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                 
;; Exercise Q
;; 
;; (qsort lt?) is a polymorphic, higher-order function that takes a binary
;; comparison function and returns a Quicksort function.
;;
;; (sort xs) takes a list and performs Quicksort on the list. It sorts the list
;; according to the binary comparison function that qsort is passed. 
;;
;; qsort terminates when (if (null? xs) is satisfied. This occurs after each
;; of the lists have recursed correctly so that the pivots, and thus each 
;; element, are in the correct positions in the list. Thus, no new elements
;; must be sorted and each element is ready to be placed correctly in the list.
;;

(define qsort (lt?)
    (letrec
        ([sort (lambda (xs) 
            (if (null? xs)
                xs
                (append (sort (filter (o not ((curry lt?) (car xs))) (cdr xs)))
                    (cons (car xs)
                        (sort (filter ((curry lt?) (car xs)) (cdr xs)))))))])
        sort))


(check-expect ((qsort <) '()) '())
(check-expect ((qsort >) '()) '())
(check-expect ((qsort <) '(5)) '(5))
(check-expect ((qsort >) '(1 1)) '(1 1))
(check-expect ((qsort <) '(1 2 3 4 5 4 3 4 5 3 2 3 4 5 6 5))
                            '(1 2 2 3 3 3 3 4 4 4 4 5 5 5 5 6))
(check-expect ((qsort >) '(1 2 3 4 5 4 3 4 5 3 2 3 4 5 6 5))
                            '(6 5 5 5 5 4 4 4 4 3 3 3 3 2 2 1))



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem F
;;
;; (formula? sexp) takes an arbitrary S-expression and returns true i the
;; S-expression represents a Boolean formula and false otherwise. It is a 
;; mutually recursive function.
;;
;; (form? sexp) is an internal recursive function that takes a sexp and 
;; determines the structure of the S-expression. It calls other internal 
;; functions for specific cases like not, and, and or. 

(define formula? (sexp)
    (letrec 
        ([form? (lambda (sexp)
            (if (atom? sexp)
                (symbol? sexp) 
                (if (equal? (car sexp) 'not)
                    (not? sexp)
                    (if (or (equal? (car sexp) 'and) (equal? (car sexp) 'or))
                        (and-or? sexp)
                        (form? (car sexp))))))]

        [not? (lambda (sexp)
                (if (= (length sexp) 2)
                    (form? (cdr sexp))
                    #f))]

        [and-or? (lambda (sexp)
                    (all? form? (cdr sexp)))])

        (form? sexp)))


(check-expect (formula? 'a) #t)
(check-expect (formula? '()) #f)
(check-expect (formula? '(and)) #t)
(check-expect (formula? '(or)) #t)
(check-expect (formula? '(not)) #f)
(check-expect (formula? '(and a (or b) c)) #t)
(check-expect (formula? '(or a (not a))) #t)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Problem 21
;;
;; (find-formula-true-asst f fail succ) takes a formula, a failure 
;; continuation, and a success continuation and finds a satisfying assignment,
;; by mapping variables to Booleans, for the formula. The function calls
;; the success continuation if a satisfying assignment is found and the 
;; failure continuation if a satisfying assignment is not found.
;;
;; (find-formula-asst formula bool cur fail succeed) extends the assignment
;; of cur to find an assignment that makes the signle formula equal to bool.
;;
;; (find-all-asst formulas bool cur fail succeed) extends cur to find an 
;; assignment that makes every formula in the list formulas equal to bool.
;;
;; (find-any-asst formulas bool cur fail succeed) extends cur to find an 
;; assignment that makes any one of the formulas equal to bool.
;;


(define find-formula-true-asst (f fail succ)
    (letrec 
        ([find-formula-asst 
            (lambda (formula bool cur fail succ)
                (if (symbol? formula)
                    (find-literal-true-assignment formula bool cur fail succ)
                    (if (equal? (car formula) 'and)
                        (find-all-asst (cdr formula) bool cur fail succ)
                        (if (equal? (car formula) 'or)
                            (find-any-asst (cdr formula) bool cur fail succ)
                                (if (equal? (car formula) 'not) 
                                    (if (symbol? (cadr formula))
                                        (find-formula-asst (cadr formula) 
                                            (not bool) cur fail succ)
                                        (if (equal? (caadr formula) 'or)
                                            (find-all-asst (cdadr formula) 
                                                (not bool) cur fail succ)
                                            (find-formula-asst (cadr formula) 
                                                (not bool) cur fail succ)))
                                    (fail))))))]

        [find-all-asst (lambda (formulas bool cur fail succ)
                        (if (null? formulas)
                            (succ cur fail)
                            (find-formula-asst (car formulas) bool cur fail 
                                (lambda (cur resume) 
                                    (find-all-asst (cdr formulas) 
                                        bool cur resume succ)))))]

        [find-any-asst (lambda (formulas bool cur fail succ)
                        (if (null? formulas)
                            (fail)
                            (find-formula-asst (car formulas) bool cur 
                                (lambda () 
                                    (find-any-asst (cdr formulas) 
                                        bool cur fail succ))
                                succ)))]

        [find-literal-true-assignment (lambda (lit bool cur fail succ)
                                        (if (or (equal? (find lit cur) bool)
                                                (equal? (find lit cur) '()))
                                            (succ (bind lit bool cur) fail)
                                            (fail)))])

    (find-formula-asst f #t '() fail succ)))

(check-expect (find-formula-true-asst 
                    '(and (or (not a) (not b)) (not (and a b))) 
                    (lambda () 'fail) (lambda (cur resume) cur)) 
                                                '((a #f) (b #f)))

(check-expect (find-formula-true-asst '(not (or a b c)) 
                    (lambda () 'fail) (lambda (cur resume) cur)) 
                                                '((a #f) (b #f) (c #f)))

(check-expect (find-formula-true-asst '(and (not (or a)) (and a))
                    (lambda () 'fail) (lambda (cur resume) cur)) 
                                                'fail)

(check-expect (procedure? find-formula-true-asst) #t) ; correct name
(check-error (find-formula-true-asst))                ; not 0 arguments
(check-error (find-formula-true-asst 'x))             ; not 1 argument
(check-error (find-formula-true-asst 'x (lambda () 'fail)))   ; not 2 args
(check-error
       (find-formula-true-asst 'x (lambda () 'fail) (lambda (c r) 'succeed) z))
                                                                 ; not 4 args

(check-error (find-formula-true-asst 'x 
                        (lambda () 'fail) (lambda () 'succeed)))
        ; success continuation expects 2 arguments, not 0
(check-error (find-formula-true-asst 'x (lambda () 'fail) 
                        (lambda (_) 'succeed)))
        ; success continuation expects 2 arguments, not 1
(check-error (find-formula-true-asst '(and x (not x)) (lambda (_) 'fail) 
                        (lambda (_) 'succeed)))
        ; failure continuation expects 0 arguments, not 1

(check-expect   ; x can be solved
       (find-formula-true-asst 'x
                               (lambda () 'fail)
                               (lambda (cur resume) 'succeed))
       'succeed)

(check-expect   ; x is solved by '((x #t))
       (find-formula-true-asst 'x
                               (lambda () 'fail)
                               (lambda (cur resume) (find 'x cur)))
       '#t)

(check-expect   ; (not x) can be solved
       (find-formula-true-asst '(not x)
                               (lambda () 'fail)
                               (lambda (cur resume) 'succeed))
       'succeed)

(check-expect   ; (not x) is solved by '((x #f))
       (find-formula-true-asst '(not x)
                               (lambda () 'fail)
                               (lambda (cur resume) (find 'x cur)))
       '#f)

(check-expect   ; (and x (not x)) cannot be solved
       (find-formula-true-asst '(and x (not x))
                               (lambda () 'fail)
                               (lambda (cur resume) 'succeed))
       'fail)
