""" 
File Name: random_forest_classifier.py
Jared R. Lieberman

"""

import pandas as pd
import numpy as np
from sklearn import model_selection
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
import math


"""
Name: preprocess_yellow_data
Purpose: This function preprocesses the yellow data
"""
def preprocess_yellow_data():

	df_yellow = pd.read_csv('df_yellow_with_boros.csv')
	df_yellow = df_yellow.loc[df_yellow['BoroughPU'].notnull() & \
							  df_yellow['BoroughDO'].notnull() & \
							  df_yellow['passenger_count'].notnull()]

	# standardize tpep_pickup_datetime 
	df_yellow['tpep_pickup_datetime'] = \
						pd.to_datetime(df_yellow['tpep_pickup_datetime'])

	df_yellow['tpep_pickup_datetime'] = \
			(df_yellow['tpep_pickup_datetime'] - \
			 df_yellow['tpep_pickup_datetime'].min()) / np.timedelta64(1,'D')
	
	# Set to actual borough names
	df_yellow['BoroughPU'] = \
		df_yellow['BoroughPU'].map({'EWR' : 1, 'Queens' : 2, 'Bronx' : 3, \
			     					'Manhattan' : 4, 'Brooklyn' : 5, \
			     					'Staten Island' : 6})

	df_yellow['BoroughDO'] = \
		df_yellow['BoroughDO'].map({'EWR' : 1, 'Queens' : 2, 'Bronx' : 3, \
			     					'Manhattan' : 4, 'Brooklyn' : 5, \
			     					'Staten Island' : 6})

	return df_yellow


"""
Name: preprocess_green_data
Purpose: This function preprocesses the green data
"""
def preprocess_green_data():

	df_green = pd.read_csv('df_green_with_boros.csv')
	df_green = df_green.loc[df_green['BoroughPU'].notnull() & \
							 df_green['BoroughDO'].notnull() & \
							 df_green['passenger_count'].notnull()]

	# standardize lpep_pickup_datetime 
	df_green['lpep_pickup_datetime'] = \
					pd.to_datetime(df_green['lpep_pickup_datetime'])

	df_green['lpep_pickup_datetime'] = \
				(df_green['lpep_pickup_datetime'] - \
				 df_green['lpep_pickup_datetime'].min()) / np.timedelta64(1,'D')
	
	# Set to actual borough names
	df_green['BoroughPU'] = \
		df_green['BoroughPU'].map({'EWR' : 1, 'Queens' : 2, 'Bronx' : 3, \
			     					'Manhattan' : 4, 'Brooklyn' : 5, \
			     					'Staten Island' : 6})

	df_green['BoroughDO'] = \
		df_green['BoroughDO'].map({'EWR' : 1, 'Queens' : 2, 'Bronx' : 3, \
			     					'Manhattan' : 4, 'Brooklyn' : 5, \
			     					'Staten Island' : 6})

	return df_green


"""
Name: preprocess_fhv_data
Purpose: This function preprocesses the fhv data
"""
def preprocess_fhv_data():

	df_fhv = pd.read_csv('df_fhv_with_boros.csv')
	df_fhv = df_fhv.loc[df_fhv['BoroughPU'].notnull() & \
					 	df_fhv['BoroughDO'].notnull()]

	# standardize Pickup_DateTime
	df_fhv['Pickup_DateTime'] = pd.to_datetime(df_fhv['Pickup_DateTime'])

	df_fhv['Pickup_DateTime'] = \
				(df_fhv['Pickup_DateTime'] - \
				 df_fhv['Pickup_DateTime'].min()) / np.timedelta64(1,'D')
	
	# Set to actual borough names
	df_fhv['BoroughPU'] = \
		df_fhv['BoroughPU'].map({'EWR' : 1, 'Queens' : 2, 'Bronx' : 3, \
			     				 'Manhattan' : 4, 'Brooklyn' : 5, \
			     				 'Staten Island' : 6})

	df_fhv['BoroughDO'] = \
		df_fhv['BoroughDO'].map({'EWR' : 1, 'Queens' : 2, 'Bronx' : 3, \
			     				 'Manhattan' : 4, 'Brooklyn' : 5, \
			     				 'Staten Island' : 6})

	# remove BO from column and cast as signed ints
	df_fhv['Dispatching_base_num'] = \
		pd.to_numeric(df_fhv['Dispatching_base_num'].str[2:], downcast='signed')

	return df_fhv

"""
Name: read_in_data
Purpose: This is the general function processing the three relevant csv files
"""
def read_in_data():

	df_yellow = preprocess_yellow_data()

	df_green = preprocess_green_data()

	df_fhv = preprocess_fhv_data()

	return df_yellow, df_green, df_fhv


"""
Name: perform_rfc
Purpose: This function creates a random forest model on the 
appropriate dataset
"""
def perform_rfc(df, ride_type):

	# Separate into training and testing
	if ride_type == 'yellow':
		X = df[['tpep_pickup_datetime', 'passenger_count', 'BoroughPU']]
	elif ride_type == 'green':
		X = df[['lpep_pickup_datetime', 'passenger_count', 'BoroughPU']]
	else:
		X = df[['Dispatching_base_num', 'Pickup_DateTime', 'BoroughPU']]

	y = df['BoroughDO']
	num_features = 3.0

	X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)

	# Train random forest classifier
	param_grid = \
		[{'n_estimators': [10, 50, 100], \
		  'max_features': ["auto", "sqrt", 'log2']}]

	# Create random forest classifier
	classifier = GridSearchCV(RandomForestClassifier(), param_grid)

	classifier.fit(X_train, y_train)

	print("The best parameters are %s with a score of %0.2f" \
				% (classifier.best_params_, classifier.best_score_))
	
	# Apply classifier To test data
	predictions = classifier.predict(X_test)

	test_accuracy = accuracy_score(y_test, predictions)

	print("Train accuracy :: ", \
					accuracy_score(y_train, classifier.predict(X_train)))
	print("Test accuracy :: ", accuracy_score(y_test, predictions))

	return classifier, test_accuracy


def main():

	df_yellow, df_green, df_fhv = read_in_data()

	print("Yellow Taxi")
	yellow_clf, yellow_test_accuracy = perform_rfc(df_yellow, 'yellow')
	
	print("Green Taxi")
	green_clf, green_test_accuracy = perform_rfc(df_green, 'green')
	
	print("For-Hire Vehicle")
	fhv_clf, fhv_test_accuracy = perform_rfc(df_fhv, 'fhv')


if __name__ == "__main__":
	main()
