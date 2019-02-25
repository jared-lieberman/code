""" 
File Name: read_in_data.py
Jared R. Lieberman

"""

import pandas as pd


"""
Name: shuffle_df
Purpose: This function takes in a dataframe and randomizes the order of its rows
"""
def shuffle_df(df):
	
	import sklearn.utils
	df = sklearn.utils.shuffle(df)

	df = df.reset_index(drop=True)

	return df

"""
Name: grab_subset
Purpose: This function takes in a dataframe and return the first 20,000 rows
"""
def grab_subset(df):

	subset_value = 20000
	return df.head(subset_value)


"""
Name: process_data
Purpose: This is the general function for reading in a csv file and returning
it restructured in as a pandas dataframe 
"""
def process_data(filename, fhv_bool):

	df = pd.read_csv(filename)

	# remove null values from location columns according to correct column name
	if fhv_bool:
		df = df.loc[df['PUlocationID'].notnull() & df['DOlocationID'].notnull()] 
	else:
		df = df.loc[df['PULocationID'].notnull() & df['DOLocationID'].notnull()] 

	print(df.head())

	df = shuffle_df(df)
	df = grab_subset(df)


	return df



def main():

	# set to global to allow other modules to access without needing
	# to process fully -- time consuming
	
	print('yellow taxi')
	df_yellow = process_data("./yellow_tripdata_2018-01.csv", 0)
	df_yellow.to_pickle('df_yellow.pkl')


	print('green taxi')
	df_green = process_data("./green_tripdata_2018-01.csv", 0)
	df_green.to_pickle('df_green.pkl')


	print('fhv')
	df_fhv = process_data("./fhv_tripdata_2018-01.csv", 1)
	df_fhv.to_pickle('df_fhv.pkl')



if __name__ == "__main__":
	main()
