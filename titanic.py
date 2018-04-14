# titanic.py
#
# Jared R. Lieberman
# COMP 135 Machine Learning
# Project 2
# Professor Liping Liu
# 6 April 2018
#

import csv
import numpy as np
import pandas as pd
from sklearn import svm, model_selection
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.model_selection import cross_val_score
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
from sklearn.linear_model import LogisticRegression
from sklearn.preprocessing import normalize
import pickle


class titanicClassifier():
	def __init__(self, final_classifier):
		self.model = final_classifier

	def score(self, X, y, **kwargs):
    	# include all your data pre-processing
		X = np.delete(X, 5, 1)

		predictions = self.model.predict(X)

		test_accuracy = np.sum(y == predictions) * 100 / len(predictions)

		return test_accuracy


#
# read_in_titanic_data 
#
def read_in_titanic_data(file_name):
	with open(file_name) as f:
		reader = csv.reader(f, delimiter=",")

		df = pd.read_csv(f)

		# need to flip some data because the vector has both categorical and 
		# real-valued features
		# for example = survived is a higher score but 1st class (= 1) might have
		# a higher chance of surviving than 3rd class, so that is flipped.
		df.Pclass = df.Pclass.replace(1, 4)
		df.Pclass = df.Pclass.replace(3, 1)
		df.Pclass = df.Pclass.replace(4, 3)

		return df


#
# svm_titanic
#
def svm_titanic(titanic_data):

	# drop the Ticket number because it is irrelevant
	titanic_data.drop(['Ticket'], 1, inplace=True)
	#titanic_data.drop(['Embarked'], 1, inplace=True)
	#print(titanic_data)
	X = np.array(titanic_data.drop(['Survived'], 1))
	X = normalize(X, axis=0)

	y = np.array(titanic_data['Survived'])

	X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.2)

	param_grid = \
		[{'kernel': ['rbf'], 'gamma': [1e-3, 1e-4], 'C': [1, 10, 100, 1000]},\
		 {'kernel': ['linear'], 'C': [1, 10, 100, 1000]}]

	classifier = GridSearchCV(svm.SVC(), param_grid)

	classifier.fit(X_train, y_train)

	print("The best parameters are %s with a score of %0.2f" \
				% (classifier.best_params_, classifier.best_score_))


	confidence = classifier.score(X_test, y_test)

	print(confidence)

	predictions = classifier.predict(X_test)

	test_accuracy = accuracy_score(y_test, predictions)

	print("Train accuracy :: ", accuracy_score(y_train, classifier.predict(X_train)))
	print("Test accuracy :: ", accuracy_score(y_test, predictions))


	return(classifier, test_accuracy)

#
# rfc_titanic - Random Forest Classifier
#
def rfc_titanic(titanic_data):
	
	# drop the Ticket number because it is irrelevant - done svm
	#titanic_data.drop(['Ticket'], 1, inplace=True)

	X = np.array(titanic_data.drop(['Survived'], 1))
	X = normalize(X, axis=0)

	y = np.array(titanic_data['Survived'])

	X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.2)

	# n_estimators - number of trees in the forest. The larger the better, but 
	# also the longer it will take to compute. In addition, results will stop
	# getting significantly better beyond a critical number of trees
	# max_features - the size of the random subsets of features to consider when 
	# splitting a node. the lower the greater the reduction of variance, but 
	# also the greater the increase in bias - empirical good value for 
	# classification tasks - max_features = sqrt(n_features)

	param_grid = \
		[{'n_estimators': [10, 50, 100], 'max_features': ["auto", "log2", None]}]

	classifier = GridSearchCV(RandomForestClassifier(), param_grid)

	classifier.fit(X_train, y_train)
	
	print("The best parameters are %s with a score of %0.2f" \
				% (classifier.best_params_, classifier.best_score_))

	predictions = classifier.predict(X_test)

	test_accuracy = accuracy_score(y_test, predictions)

	print("Train accuracy :: ", accuracy_score(y_train, classifier.predict(X_train)))
	print("Test accuracy :: ", accuracy_score(y_test, predictions))

	return(classifier, test_accuracy)

#
# logit_titanic - Logistic Regression
#
def logit_titanic(titanic_data):
	
	# drop the Ticket number because it is irrelevant - done in svm
	#titanic_data.drop(['Ticket'], 1, inplace=True)
	
	X = np.array(titanic_data.drop(['Survived'], 1))
	X = normalize(X, axis=0)

	y = np.array(titanic_data['Survived'])

	X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.2)

	classifier = LogisticRegression()

	param_grid = [{'penalty': ['l1', 'l2'], 'C': np.logspace(0.0, 4.0, 10.0)}]
	classifier = GridSearchCV(classifier, param_grid, cv=5, verbose=0)


	classifier = classifier.fit(X_train, y_train)

	print("The best parameters are %s with a score of %0.2f" \
				% (classifier.best_params_, classifier.best_score_))

	score = classifier.score(X_test, y_test)
	print(score)

	predictions = classifier.predict(X_test)

	test_accuracy = accuracy_score(y_test, predictions)

	print("Train accuracy :: ", accuracy_score(y_train, classifier.predict(X_train)))
	print("Test accuracy :: ", accuracy_score(y_test, predictions))

	return(classifier, test_accuracy)



def main():
	
	# read in data
	titanic_data = read_in_titanic_data("titanic_filled.csv")
	
	# SVM titanic
	(clf_svm, svm_acc) = svm_titanic(titanic_data)

	# Random Forest Classifier titanic
	(clf_rfc, rfc_acc) = rfc_titanic(titanic_data)

	# Logistic Regression titanic
	(clf_logit, logit_acc) = logit_titanic(titanic_data)

	if (rfc_acc >= svm_acc):
		if (rfc_acc >= logit_acc):
			classifier = clf_rfc
			print("Best classifier is Random Forest Classifier")
		else: 
			classifier = clf_logit
			print("Best classifier is Logistic Regression")
	elif (svm_acc >= rfc_acc): 
		if (svm_acc >= logit_acc):
			classifier = clf_svm
			print("Best classifier is SVM")
		else:
			classifier = clf_logit
			print("Best classifier is Logistic Regression")


	clf = titanicClassifier(classifier)
	classifier = {'nickname': 'Titanic Cappuccino', 'classifier': clf}
	pickle.dump(classifier, open('titanic_classifier.pkl', 'wb'))


main()
