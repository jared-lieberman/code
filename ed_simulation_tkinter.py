# ed_simulation_tkinter.py

# Jared Lieberman 
# BIDMC 

# Notes:
#	- The simulation does not account for a patient having the same doctor
#	  throughout their entire experience in the ED. Patients request 
#	  a doctors twice (to be treated and for the disposition), but doctors
#	  are not distinguished by their team in this program. There is simply
#	  a doctor resource with a capacity of three (number of teams).
#	- If preferred, more variables can be added to the Patient class, like 
#	  timestamps. This will work well for collecting and synthesizing 
#	  individual patient data.
#	- The program currently only handles three resources (ie EKG, Lab) each
#	  with arbitrary values for time and capacity
#	- As noted later on, Chief Complaint's are categorized as values between
#	  one and five, in essence from least sick to very sick. These can be
#	  changed by setting values for each complaint. If so, the program does
#	  not account for multiple CC values, thus multiple Chief Complaints.


global NUM_BEDS
global RUNTIME
global TBA
global INIT_PTS
NUM_DOCS = 3

import simpy
from simpy import Environment
from simpy.util import start_delayed
import random

import tkinter
from tkinter import *
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure

import numpy as np
import collections


wr_queue       = []
wr_queue_len   = []
wr_queue_times = []

# dictionary - keys: tgreet, values: total ed time 
avg_time_tgreet 	  = {}
avg_time_tgreet_count = {}

# dictionary - keys: CC, values: total ed time 
avg_time_cc       = {}
avg_time_cc_count = {}

# dictionary - keys: Resource, values: total ed time 
global avg_time_res
global RESOURCE_1
global RESOURCE_2
global RESOURCE_3 
avg_time_res_count = {'Resource 1': 0, 'Resource 2': 0, 'Resource 3': 0}

# ED class
# create and setup the ED
class ED(object):
	def __init__(self, env, num_beds):
		self.env      = env
		self.num_beds = num_beds
		self.num_docs = NUM_DOCS



# Patient class 
#	Each instance contains a patient name and CC
#	The get_prio function creates a priority value based on the CC value
class Patient(object):
	def __init__(self, env, name, cc):
		self.env     	   = env
		self.name     	   = name
		
		# for the CC I'll give a value of 1-5
		#	multiply each when being treated so have different times
		#	5 is the worst - which I can make have other queues (ie EKG)
		#	to be put on. So 1 will have the shortest amount of treat time
		self.cc    		   = cc
		#self.tgreet   	   = tgreet
		#self.tout    	   = tout


	def get_prio(self, patient):
		# to convert cc to priority value for queue
		# 1 <-> 5
		# 2 <-> 4
		# 3 <-> 3
		return ((self.cc * 4) % 5) + 1

	def get_total_time(self, patient):
		return self.tout - self.tgreet


# print_stats
# Purpose: This function prints the number of beds and doctors being used at
#		a given moment. It is utilized throughout the program.
def print_stats(res_bed, res_doc):
	print('Beds:    %d of %d slots are allocated.' % (res_bed.count, res_bed.capacity))
	print('Doctors: %d of %d slots are allocated.' % (res_doc.count, res_doc.capacity))



# res_1_queue
#
# Purpose: This is the function for resource 1. This has a unique capacity
#		and time required needed to complete using the resources.
def res_1_queue(env, patient):
	patient.res_list['Resource 1'] = 1
	
	# example time and capacity for resource
	res_1_cap  = 1
	res_1_time = 6
	res_1      = simpy.Resource(env, res_1_cap)

	with res_1.request() as request_res_1:
		yield request_res_1
		yield env.timeout(res_1_time)

		res_1.release(request_res_1)

# res_2_queue
#
# Purpose: This is the function for resource 2. This has a unique capacity
#		and time required needed to complete using the resources.
def res_2_queue(env, patient):
	patient.res_list['Resource 2'] = 1

	# example time and capacity for resource
	res_2_cap  = 2
	res_2_time = 7
	res_2      = simpy.Resource(env, res_2_cap)

	with res_2.request() as request_res_2:
		yield request_res_2
		yield env.timeout(res_2_time)

		res_2.release(request_res_2)

# res_3_queue
#
# Purpose: This is the function for resource 3. This has a unique capacity
#		and time required needed to complete using the resources.
def res_3_queue(env, patient):
	patient.res_list['Resource 3'] = 1

	# example time and capacity for resource
	res_3_cap  = 3
	res_3_time = 8
	res_3      = simpy.Resource(env, res_3_cap)

	with res_3.request() as request_res_3:
		yield request_res_3
		yield env.timeout(res_3_time)

		res_3.release(request_res_3)


# treat
#
# Purpose: This is the general function for a patient requesting needed
#		resources. I have created placeholder resources based on the 
#		patients CC. The greater the CC, the more resources and time
#		needed before they are done being treated. There is a print statement
#		to view a patient's CC value
def treat(env, patient):
	#print("%s %d" % (patient.name, patient.cc))

	# designated time based on patient's CC
	yield env.timeout(patient.cc * 5)
	
	# These are just example cases for which CC's need which resources

	# patient list for what resources they use - all 0s until they used it
	#	example: three total resourses
	patient.res_list = {'Resource 1': 0, 'Resource 2': 0, 'Resource 3': 0}

	if patient.cc == 1:
		yield env.process(res_1_queue(env, patient))
	if patient.cc == 2:
		yield env.process(res_2_queue(env, patient))
	if patient.cc == 3:
		yield env.process(res_3_queue(env, patient))
	if patient.cc == 4:
		yield env.process(res_1_queue(env, patient))
		yield env.process(res_2_queue(env, patient))
	if patient.cc == 5:
		yield env.process(res_1_queue(env, patient))
		yield env.process(res_2_queue(env, patient))
		yield env.process(res_3_queue(env, patient))

	print(patient.res_list)

# visit
#
# Purpose: This is the general visit function. It includes the entire process
#		of a patient visiting the ED. It includes the flow of being greeted in
#		the waiting room, waiting for a bed, waiting for a doctor, requesting
#		different resources (ie EKG), waiting for a disposition from the
#		doctor, and leaving the ED. There are useful print statements 
#		throughout the function.
def visit(env, patient, res_bed, res_doc):
	print_stats(res_bed, res_doc)

	# patient gets added to WR queue
	wr_queue.append(patient.name)
	wr_queue_len.append(len(wr_queue))
	wr_queue_times.append(env.now)

	# for total times graph
	patient.tgreet = env.now

	print("%s is greeted in the WR at %7.4f" % (patient.name, env.now))

	# request a bed, based on priority
	with res_bed.request(priority = patient.get_prio(patient)) as request_bed:
		print_stats(res_bed, res_doc)
		yield request_bed
		print("%s gets bed and enters ER at %7.4f" % (patient.name, env.now))
		# request a doctor, based on priority
		with res_doc.request(priority = patient.get_prio(patient)) as request_doc:
			yield request_doc
			
			# example time for doc to get to patient 
			DOC_TO_PT = 5
			yield env.timeout(DOC_TO_PT)
			print("%s tfirstdoc: %7.4f" % (patient.name, env.now)) 
			print_stats(res_bed, res_doc)
			
			# process for patient to request needed resources
			yield env.process(treat(env, patient))
			print("%s was treated at %7.4f" % (patient.name, env.now))
			print_stats(res_bed, res_doc)
			
			# release doc once done treating
			res_doc.release(request_doc)

			# request doc for disposition - based on priority
			with res_doc.request(priority = patient.get_prio(patient)) as request_doc_dispo:
				print("%s requesting doc for dispo at %7.4f" % (patient.name, env.now))
				yield request_doc_dispo
				print("%s got doc for dispo at %7.4f" % (patient.name, env.now))
				# example time for doc dispo
				DISPO_TIME = 6
				yield env.timeout(DISPO_TIME)
				print("%s dispo at %7.4f" % (patient.name, env.now))
				res_doc.release(request_doc_dispo)
		
		# release bed once doc is done
		# designated time for time for leaving bed after being treated
		LEAVE_BED_TIME = 5
		yield env.timeout(LEAVE_BED_TIME)

		# patient is removed from waiting room queue
		wr_queue.remove(patient.name)

		res_bed.release(request_bed)
		print("%s leaves ER (admitted or home) at %7.4f" % (patient.name, env.now))
		
		# time pt leaves ED
		patient.tout = env.now

		# update avg_time_tgreet dictionary or add new key
		if patient.tgreet in avg_time_tgreet:
			# if not new total time - update avg time by getting sum and 
			#	multiplying by new count
			avg_time_tgreet[patient.tgreet] *= avg_time_tgreet_count[patient.tgreet] 
			avg_time_tgreet[patient.tgreet] += patient.get_total_time(patient)
			avg_time_tgreet_count[patient.tgreet] += 1
			avg_time_tgreet[patient.tgreet] /= avg_time_tgreet_count[patient.tgreet]
		else:
			# if new total time - add to dictionary and set count to 1
			avg_time_tgreet[patient.tgreet] = patient.get_total_time(patient)
			avg_time_tgreet_count[patient.tgreet] = 1


		# update avg_time_cc dictionary or add new key
		if patient.cc in avg_time_cc:
			# if not new total time - update avg time by getting sum and 
			#	multiplying by new count
			avg_time_cc[patient.cc] *= avg_time_cc_count[patient.cc] 
			avg_time_cc[patient.cc] += patient.get_total_time(patient)
			avg_time_cc_count[patient.cc] += 1
			avg_time_cc[patient.cc] /= avg_time_cc_count[patient.cc]
		else:
			# if new total time - add to dictionary and set count to 1
			avg_time_cc[patient.cc] = patient.get_total_time(patient)
			avg_time_cc_count[patient.cc] = 1

		# update avg_time_res dictionary
		if patient.res_list['Resource 1'] == 1:
			avg_time_res[RESOURCE_1]       *= avg_time_res_count['Resource 1']
			avg_time_res[RESOURCE_1]       += patient.get_total_time(patient)
			avg_time_res_count['Resource 1'] += 1
			avg_time_res[RESOURCE_1]       /= avg_time_res_count['Resource 1']
		if patient.res_list['Resource 2'] == 1:
			avg_time_res[RESOURCE_2]       *= avg_time_res_count['Resource 2']
			avg_time_res[RESOURCE_2]       += patient.get_total_time(patient)
			avg_time_res_count['Resource 2'] += 1
			avg_time_res[RESOURCE_2]       /= avg_time_res_count['Resource 2']
		if patient.res_list['Resource 3'] == 1:
			avg_time_res[RESOURCE_3]       *= avg_time_res_count['Resource 3']
			avg_time_res[RESOURCE_3]       += patient.get_total_time(patient)
			avg_time_res_count['Resource 3'] += 1
			avg_time_res[RESOURCE_3]       /= avg_time_res_count['Resource 3']

		print_stats(res_bed, res_doc)

# graph
#
# Purpose: Visualize different statistics that are tracked in the program
def graph():
	frame = tkinter.Frame(root)
	fig = Figure(figsize = (15,15), dpi = 100)

	button_q = Button(root, text = 'Quit', command = root.quit)
	button_q.pack(side = BOTTOM)

	# Patients in WR over time graph
	num_pts_graph = fig.add_subplot(221, title = 'Patients in WR over time', \
        xlabel = 'Time', ylabel = 'Number of Patients')
        
	num_pts_graph.plot(wr_queue_times, wr_queue_len)


	# Average Total ED Time by Time of Arrival graph

	# create ordered dictionary for avg_time_tgreet and sorted lists of keys
	oavg_time_tgreet = collections.OrderedDict(sorted(avg_time_tgreet.items()))

	keys   = []
	values = []
	for k, v in oavg_time_tgreet.items():
		keys.append(k)
		values.append(v)
	
	time_by_tgreet_graph = fig.add_subplot(222, title = \
		'Average Total ED Time by Time of Greeting', \
        xlabel = 'Time of Greeting', ylabel = 'Average Total Time in ED')
	time_by_tgreet_graph.plot(keys, values)

	
	# Average Total ED Time by CC graph
	oavg_time_cc = collections.OrderedDict(sorted(avg_time_cc.items()))

	keys   = []
	values = []
	for k, v in oavg_time_cc.items():
		keys.append(k)
		values.append(v)

	cc_by_time_graph = fig.add_subplot(223, title = \
		'Average Total ED Time by Chief Complaint', \
        xlabel = 'Chief Complaint', ylabel = 'Average Total Time in ED')
	y_pos = np.arange(len(keys))
	cc_by_time_graph.bar(y_pos, values, align = 'center')
	cc_by_time_graph.set_xticks(y_pos)
	cc_by_time_graph.set_xticklabels(keys)


	# Average ED Time by Resource used graph
	keys   = []
	values = []
	for k, v in avg_time_res.items():
		keys.append(k)
		values.append(v)
	
	res_by_time_graph = fig.add_subplot(224, title = \
		'Average Total ED Time by Resource Used', \
        xlabel = 'Resource', ylabel = 'Average Total Time in ED')
	y_pos = np.arange(len(keys))
	res_by_time_graph.bar(y_pos, avg_time_res.values(), align = 'center')
	res_by_time_graph.set_xticks(y_pos)
	res_by_time_graph.set_xticklabels(keys)

	canvas = FigureCanvasTkAgg(fig, master = root)
	canvas.show()
	canvas.get_tk_widget().pack(side = 'top', fill = 'both', expand = 5)
	frame.pack()

	canvas.draw()


# setup
#
# Purpose: This function is the initial setup function for the simulation
#		It creates an instance of the ED and a designated number of 
#		initial patients. Additionally, the function infinitely loops
#		creating new patients. 
def setup(env, res_bed, res_doc):
	# create the ED
	ed = ED(env, NUM_DOCS)

	# create initial patients with individual name IDs and a CC value 1-5
	for i in range(INIT_PTS):
		p = Patient(env, "Patient_%02d" % (i,), (i % 5) + 1)
		env.process(visit(env, p, res_bed, res_doc))

	# Create more pts while the simulation is running
	while True:
		# yield every TBA +- 2 minutes to create new pt - to create variability
		# this is where we'd put avg of how often a pt arrives at ed
		yield env.timeout(random.randint(TBA - 2, TBA + 2))
		i += 1
		p = Patient(env, "Patient_%02d" % (i,), (i % 5) + 1)
		env.process(visit(env, p, res_bed, res_doc))



def run_prog(num_beds, tba, runtime, resource_1, resource_2, resource_3, \
		init_pts):
	env = simpy.Environment()
	
	# assign variables to be global

	global TBA
	TBA = tba  # time between arrivals, minutes

	global NUM_BEDS
	NUM_BEDS  = num_beds     # number of beds in ED

	global RUNTIME
	RUNTIME   = runtime

	global avg_time_res
	global RESOURCE_1
	global RESOURCE_2
	global RESOURCE_3
	RESOURCE_1 = resource_1
	RESOURCE_2 = resource_2
	RESOURCE_3 = resource_3
	avg_time_res = {RESOURCE_1: 0, RESOURCE_2: 0, RESOURCE_3: 0}

	global INIT_PTS
	INIT_PTS = init_pts

	# create a bed resource with NUM_BEDS capacity
	res_bed   = simpy.PriorityResource(env, NUM_BEDS) 
	# create a doctor resource with NUM_DOCS capacity
	res_doc   = simpy.PriorityResource(env, NUM_DOCS)

	env.process(setup(env, res_bed, res_doc))

	env.run(until = RUNTIME)

	graph()


print('SIMULATION OF EMERGENCY DEPARTMENT')

root = tkinter.Tk()


label1 = Label(root, text = "Number of beds")
label1.grid(row = 0)

label1 = Label(root, text = "Time between arrival of patients")
label1.grid(row = 1)

label1 = Label(root, text = "Simulation Runtime")
label1.grid(row = 2)

# Note: Program currently only handles 3 resources
label1 = Label(root, text = "Resource 1")
label1.grid(row = 3)

label1 = Label(root, text = "Resource 2")
label1.grid(row = 4)

label1 = Label(root, text = "Resource 3")
label1.grid(row = 5)

label1 = Label(root, text = "Initial Patients")
label1.grid(row = 6)

# num_beds
num_beds = Entry(root)
num_beds.grid(row = 0, column = 1)

# tba
tba = Entry(root)
tba.grid(row = 1, column = 1)

# runtime
runtime = Entry(root)
runtime.grid(row = 2, column = 1)

# resource 1
resource_1 = Entry(root)
resource_1.grid(row = 3, column = 1)

# resource 2
resource_2 = Entry(root)
resource_2.grid(row = 4, column = 1)

# resource 3
resource_3 = Entry(root)
resource_3.grid(row = 5, column = 1)

# initial patients
init_pts = Entry(root)
init_pts.grid(row = 6, column = 1)

# create Quit button
button_q = Button(root, text = 'Quit', command = root.quit)
button_q.grid(row = 7, column = 0, sticky = W, pady = 4)

# create Run button with numerous fields and command to run_prog
button_run = Button(root, text = 'Run', command = \
	lambda: run_prog(int(num_beds.get()), float(tba.get()), int(runtime.get())\
	, resource_1.get(), resource_2.get(), resource_3.get(), int(init_pts.get())))
button_run.grid(row = 7, column = 1, sticky = W, pady = 4)


root.mainloop()





