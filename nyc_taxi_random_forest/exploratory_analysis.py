""" 
File Name: exploratory_analysis.py
Jared R. Lieberman
"""

import pandas as pd


"""
Name: read_in_boro_data
Purpose: This function reads in the borough zoning csv
"""
def read_in_boro_data():

	df_boro = pd.read_csv('taxi_zone_lookup.csv')

	#print(df_boro.head((50)))

	return df_boro


"""
Name: get_boro_count
Purpose: This function matches pickup/dropoff locations and the zones
"""
def get_boro_count(df, df_pu, df_do, df_boro):

	print('in get_boro_count')

	boro_dict = {'EWR' : [0, 0], 'Queens' : [0, 0], 'Bronx' : [0, 0], \
			     'Manhattan' : [0, 0], 'Brooklyn' : [0, 0], \
			     'Staten Island' : [0, 0]}
	
	# pick ups
	for index, boro_id in df_pu.items():
		# get only in borough range
		if boro_id < 264 and boro_id > 0:
			boro = df_boro.loc[df_boro['LocationID'] == boro_id, 'Borough'].item()
			boro_dict[boro][0] += 1

			# Add for borough column
			df.loc[index, 'BoroughPU'] = boro

	# drop offs
	for index, boro_id in df_do.items():
		# get only in borough range
		if boro_id < 264 and boro_id > 0:
			boro = df_boro.loc[df_boro['LocationID'] == boro_id, 'Borough'].item()
			boro_dict[boro][1] += 1
			
			# Add for borough column
			df.loc[index, 'BoroughDO'] = boro

	print(boro_dict)
	
	return boro_dict, df


"""
Name: get_pu_do_data
Purpose: This function processes the matching of pickup/dropoff locations and
zones
"""
def get_pu_do_data(df, df_boro, ride_type):

	df['BoroughPU'] = ""
	df['BoroughDO'] = ""
	boro_counts, df = get_boro_count(df, df['PULocationID'], \
									 df['DOLocationID'], df_boro)

	df.to_csv('df_{}_with_boros.csv'.format(ride_type), encoding='utf-8', \
			  index=False)
	
	return boro_counts


"""
Name: plot_table
Purpose: This function creates a table that displays the count of start and
finish rides for each type of vehicle for each borough
"""
def plot_table(yellow_boro_counts, green_boro_counts, fhv_boro_counts):

	print('in plot_table')
	import matplotlib.pyplot as plt

	fig, ax = plt.subplots()
	ax.axis('off')
	ax.axis('tight')

	columns = list(yellow_boro_counts.keys())
	print(columns)
	print(type(columns))

	rows = ["Yellow Taxi", "Green Taxi", "For-Hire Vehicle"]


	yellow_boro_counts_in = ["{} | {}".format(item[0], item[1]) for item in yellow_boro_counts.values()]
	green_boro_counts_in = ["{} | {}".format(item[0], item[1]) for item in green_boro_counts.values()]
	fhv_boro_counts_in = ["{} | {}".format(item[0], item[1]) for item in fhv_boro_counts.values()]

	cell_text = [yellow_boro_counts_in] + [green_boro_counts_in] + [fhv_boro_counts_in]
	print(cell_text)
	plt.title('Pick Up and Drop Off Data for each NYC Borough')
	ax.table(cellText = cell_text, rowLabels = rows, colLabels = columns, loc='center', cellLoc='center')


	plt.show()


"""
Name: plot_counts
Purpose: This is the general function for creating plots based on borough count
data
"""
def plot_counts(yellow_boro_counts, green_boro_counts, fhv_boro_counts):

	plot_table(yellow_boro_counts, green_boro_counts, fhv_boro_counts)



def main():


	#from read_in_data import df_yellow, df_green, df_fhv
	import pandas as pd
	
	print('yellow')
	df_yellow = pd.read_pickle('./df_yellow.pkl')

	print(df_yellow.shape[0])
	print(df_yellow.head())


	df_green = pd.read_pickle('./df_green.pkl')

	print(df_green.shape[0])
	print(df_green.head())

	df_fhv = pd.read_pickle('./df_fhv.pkl')
	#print(df_fhv.columns)
	# Change case of two FHV dataframe columns
	df_fhv.rename(columns={'PUlocationID':'PULocationID', 'DOlocationID': 'DOLocationID'}, inplace=True)
	#print(df_fhv.columns)
	# Change from float to int
	df_fhv['PULocationID'] = pd.to_numeric(df_fhv['PULocationID'], downcast='signed')
	df_fhv['DOLocationID'] = pd.to_numeric(df_fhv['DOLocationID'], downcast='signed')

	print(df_fhv.shape[0])
	print(df_fhv.head())
	
	df_boro = read_in_boro_data()

	# should have three dicts with structure: dict {boro : [pu_num, do_num], ...}
	yellow_boro_counts = get_pu_do_data(df_yellow, df_boro, 'yellow')
	green_boro_counts = get_pu_do_data(df_green, df_boro, 'green')
	fhv_boro_counts = get_pu_do_data(df_fhv, df_boro, 'fhv')

	#plot_counts(yellow_boro_counts, green_boro_counts, fhv_boro_counts)



if __name__ == "__main__":
	main()


