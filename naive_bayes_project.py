# project1.py
#
# Jared R. Lieberman
# COMP 135 Machine Learning
# Project 1
# Professor Liping Liu
# 28 February 2018
#

import string
import re
from sklearn.feature_extraction.text import CountVectorizer
import numpy as np
import random
import matplotlib.pyplot as plt

global vocab_size

#
# read_in_data
# Arguments: The name of a file
# Purpose: The function opens the given file and loops through putting to given
# text in two lists, one for the sentences and the other for the numbers for 
# reviews. It also makes all the text lowercase and removes punctuation
#
def read_in_data(file_name):
    with open(file_name) as f:
    
        sentences = f.readlines()
        labels = []

        # remove unnecessary parts of sentences and add to two lists
        for i in range(len(sentences)):
            sentences[i] = sentences[i].lower()        
            regex   = re.compile('[%s]' % re.escape(string.punctuation))
            sentences[i] = regex.sub('', sentences[i])
            sentences[i] = re.sub('\s+',' ', sentences[i])
            labels.append(int(sentences[i][-2]))
            sentences[i] = sentences[i][:-3]

        return (sentences, labels)

#
# split_data
# Arguments: A list for the sentences and a list for the corresponding labels
# Purpose: The function first permutes the order of the sentences and their
# corresponding labels in order to ensure randomization. It then splits the
# lists into training and testing sublists and vectorizes each get the binary
# frequencies of words.
#
def split_data(sentences, labels):
    
    # permute the rows to randomize the order of reviews
    zipped_list         = list(zip(sentences, labels))
    np.random.shuffle(zipped_list)
    (sentences, labels) = zip(*zipped_list)
    sentences           = np.asarray(sentences)
    labels              = np.asarray(labels)

    # split for training and testing sentence data
    sentences_train = sentences[200:1000]   # length of 800
    sentences_test  = sentences[:200]       # length of 200


    # put sentences in 2D vector as shown in spec
    # create the transform
    vectorizer = CountVectorizer(binary = True)
    # tokenize and build vocab
    vectorizer.fit(sentences_train)

    # encode document
    vector = vectorizer.transform(sentences_train)

    words_train = np.array(vector.toarray(), copy = True)

    # encode testing sentences
    vector      = vectorizer.transform(sentences_test)

    words_test  = np.array(vector.toarray(), copy = True)

    global vocab_size 
    # if in experiment 1 then have to find exact vocabulary size
    if (vocab_size == -1):
        vocab_size = words_train.shape[1]
    
    # split for training and testing label data
    labels_train = labels[200:1000] # length of 800

    labels_test  = labels[:200]     # length of 200


    return(words_train, labels_train, words_test, labels_test)


#
# smooth
# Arguments: The lists for training words and training labels and the 
# smooothing factor
# Purpose: The function opens the given file and loops through putting to given
# text in two lists, one for the sentences and the other for the numbers for 
# reviews. It also makes all the text lowercase and removes punctuation
# 
def smooth(words_train, labels_train, smoothing_factor):
    pos_labels       = sum(labels_train)

    labels_train_len = len(labels_train)
    neg_labels       = labels_train_len - pos_labels

    num_feature_calcs = 4
    global vocab_size
    # initialize trained model with ones
    trained_model = np.ones((vocab_size, num_feature_calcs))
   
    # find values of each variation of the a positive/negative word and
    # a positive/negative label - add these to the trained model
    for word in range(vocab_size):
        num_pos_word_pos_label = 0
        num_neg_word_neg_label = 0
        for i in range(labels_train_len):
            if ((words_train[i][word] == 1) and (labels_train[i] == 1)):
                num_pos_word_pos_label += 1
            elif ((words_train[i][word] == 0) and (labels_train[i] == 0)):
                num_neg_word_neg_label += 1

        num_pos_word_neg_label = neg_labels - num_neg_word_neg_label
        num_neg_word_pos_label = pos_labels - num_pos_word_pos_label

        # add to trained model
            
        # positive word negative result - if col == 0 - p(w0 = 1|y = 0)
        trained_model[word][0] = np.log(((num_pos_word_neg_label) + \
            smoothing_factor) / (neg_labels + (smoothing_factor * vocab_size)))

        # negative word negative result - if col == 1 - p(w0 = 0|y = 0)
        trained_model[word][1] = np.log(((num_neg_word_neg_label) + \
            smoothing_factor) / (neg_labels + (smoothing_factor * vocab_size)))

        # positive word poitive result - if col == 2 - p(w0 = 1|y = 1)
        trained_model[word][2] = np.log(((num_pos_word_pos_label) + \
            smoothing_factor) / (pos_labels + (smoothing_factor * vocab_size)))

        # negative word positive result - if col == 3 - p(w0 = 0|y = 1)
        trained_model[word][3] = np.log(((num_neg_word_pos_label) + \
            smoothing_factor) / (pos_labels + (smoothing_factor * vocab_size)))
        
    # total positive and negative probabilities
    prob_pos = np.log(pos_labels / labels_train_len)
    prob_neg = np.log(neg_labels / labels_train_len)        
    return (trained_model, prob_pos, prob_neg)

#
# train
# Arguments: The lists for training words and training labels and the 
# smooothing factor
# Purpose: The function creates a trained model based on the training data
# by smoothing the data.
#
def train(words_train, labels_train, smoothing_factor):
    # number of all positive and negative reviews
    labels_train_len = len(labels_train)

    # smooth the values and place into trained model
    (trained_model, prob_pos, prob_neg) = \
            smooth(words_train, labels_train, smoothing_factor)

    return (trained_model, prob_pos, prob_neg)

#
# test
# Arguments: The training model, positive and negative probabilites for labels,
# and the list of testing word data 
# Purpose: The function creates a set of predicted labels by using the trained
# model to find the correct probabilities to categorize the testing word data.
#
def test(trained_model, prob_pos, prob_neg, words_test):
    global vocab_size
    predicted_labels = []

    # loop through testing data to get appropriate probabilities
    for sentence in range(len(words_test)):

        # p(+|sentence) and p(-|sentence)
        # sum the appropriate probabilities for each word
        word_probs_pos = 0
        word_probs_neg = 0

        for word in range(vocab_size):
            if (words_test[sentence][word] == 1):
                word_probs_pos += trained_model[word][2]
                word_probs_neg += trained_model[word][0]
            else:
                word_probs_pos += trained_model[word][3]
                word_probs_neg += trained_model[word][1]

        prob_sentence_pos  = prob_pos + word_probs_pos
        prob_sentence_neg  = prob_neg + word_probs_neg

        # append the greater probability to the predicted labels list
        if (prob_sentence_pos >= prob_sentence_neg):
            predicted_labels.append(1)
        else:
            predicted_labels.append(0)

    
    return predicted_labels

#
# evalueate
# Arguments: Two lists for the testing labels and predicated labels of 
# testing data
# Purpose: This function evaluates the predicted labels by comparing them to
# the actual testing labels. It finds the accuracy and error rate.
#
def evaluate(labels_test, predicted_labels):

    # compare each predicted and test label
    correct_sum = 0
    for i in range(len(labels_test)):
        if (labels_test[i] == predicted_labels[i]):
            correct_sum += 1

    accuracy = correct_sum / len(labels_test)

    error_rate = 1 - accuracy

    return error_rate

#
# process
# Arguments: The name of the file and the value of the smoothing factor
# Purpose: This is the general function for calling other functions to
# process the file, convert the data into training and testing data, create
# trained model, and apply and evaluate the model.
#
def process(file_name, smoothing_factor):

    # send read_in_data different file name
    # return tuple of words and labels
    (sentences, labels) = read_in_data(file_name) 

    # split words and labels into training and testing subsets
    (words_train, labels_train, words_test, labels_test) = \
                split_data(sentences, labels)

    (trained_model, prob_pos, prob_neg) = \
        train(words_train, labels_train, smoothing_factor)

    predicted_labels = test(trained_model, prob_pos, prob_neg, words_test)

    error_rate = evaluate(labels_test, predicted_labels)

    print("File: %s Error Rate: %7.4f" % (file_name, error_rate))

    return error_rate

#
# plot
# Arguments: The list of error rates for the different datasets and 
# smoothing factors
# Purpose: The function plots the graphs of each smoothing factor error rate
# for all the datasets.
#
def plot(error_rates):
    # imdb plot
    plt.subplot(221)
    plt.plot([0.1, 0.5, 2.5, 12.5], [error_rates[0], error_rates[3], \
                        error_rates[6], error_rates[9]], 'bo-')
    plt.axis([0, 14, 0, 1])
    plt.title('IMDB Review Plot')
    plt.xlabel('Smoothing Factor')
    plt.ylabel('Error Rate')

    # amazong plot
    plt.subplot(222)
    plt.plot([0.1, 0.5, 2.5, 12.5], [error_rates[1], error_rates[4], \
                        error_rates[7], error_rates[10]], 'go-')
    plt.axis([0, 14, 0, 1])
    plt.title('Amazon Review Plot')
    plt.xlabel('Smoothing Factor')
    plt.ylabel('Error Rate')

    # Yelp plot
    plt.subplot(223)
    plt.plot([0.1, 0.5, 2.5, 12.5], [error_rates[2], error_rates[5], \
                        error_rates[8], error_rates[11]], 'ro-')
    plt.axis([0, 14, 0, 1])
    plt.title('Yelp Review Plot')
    plt.xlabel('Smoothing Factor')
    plt.ylabel('Error Rate')
    plt.show()

#
# experiment_1
# Arguments: None. 
# Purpose: The function calls the process and plot subfuntions with the 
# appropriate smoothing factors and error rates.
#
def experiment_1():
    print("Experiment 1")    
    global vocab_size

    error_rates = []
    smoothing_factor = 0.1
    for i in range(4):
        print("Smoothing Factor: %7.4f" % smoothing_factor)
        vocab_size = -1
        imdb_er   = process("imdb_labelled.txt", smoothing_factor)
        vocab_size = -1
        amazon_er = process("amazon_cells_labelled.txt", smoothing_factor)
        vocab_size = -1
        yelp_er   = process("yelp_labelled.txt", smoothing_factor)
        smoothing_factor *= 5
        
        error_rates.append(imdb_er)
        error_rates.append(amazon_er)
        error_rates.append(yelp_er)

    return error_rates

#
# experiment_2
# Arguments: None. 
# Purpose: The function calls the process subfuntion with the 
# appropriate vocabulzary size. The chosen smoothing factor is 0.5
#
def experiment_2():
    print('\n'+"Experiment 2")
    # use m = 0.5 for experiment
    smoothing_factor = 0.5
    global vocab_size
    vocab_size = 1000
    
    for i in range(4):
        imdb_er   = process("imdb_labelled.txt", smoothing_factor)
        amazon_er = process("amazon_cells_labelled.txt", smoothing_factor)
        yelp_er   = process("yelp_labelled.txt", smoothing_factor)

        print('\n'+"Vocabulary size: %d" % (vocab_size))
        print("File: imdb_labelled.txt Error Rate: %7.4f" % (imdb_er))
        print("File: amazon_cells_labelled.txt Error Rate: %7.4f" % (amazon_er))
        print("File: yelp_labelled.txt Error Rate: %7.4f" % (yelp_er))
        vocab_size /= 2
        vocab_size = int(vocab_size)
    


def main():
    global vocab_size
    vocab_size = -1
    error_rates = experiment_1()

    # using m = 0.5
    experiment_2()

    plot(error_rates)


main()


      



