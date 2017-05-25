#!/usr/bin/python

# invoke cfhe (continued fractions HE) experiments for different values of n

import subprocess


def parse_eval_time( str_in ):
	str_in_ls=str_in.split(" ")
	eval_time=str_in_ls[2]
	f_eval_time=float(eval_time)
	print("eval_time: ", f_eval_time)
	return f_eval_time

print ("------------------- Invoking CFHE experiments -------------------")

run = 10
avg_eval_time = []

lengthPQ = 5
numPQ = 6
numLevels = 11

for i in range(run):
   avg_eval_time.append(0.0)

print ("*****************************************************************")
print ("invoking cfhe binary ", str(run) , " times: ")
for r in range(0,run):
	valid_run = 0
	while valid_run == 0:
		print ("invoking cfhe run num: ", r)
		crypto_binary_path = "/home/caesar/Desktop/development/cpp/CFHE_codeLite/cfhe/Release/cfhe"
		cmd = crypto_binary_path + " " + str(lengthPQ) + " " + str(numPQ) + " " + str(numLevels)
		print (cmd)

		p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
		p.wait()
		(output, err) = p.communicate()

		#p_status = p.wait()

		#print ("Command output : ", output)
		#print ("Command error : ", err)

		str_output=output.decode("utf-8")
		str_output_ls=str_output.split("\n")
		#print (str_output_ls)

		# iterate over the substrings

		for str_j in str_output_ls:
			if str_j.find("Evaluation time:")!=-1:
				valid_run = 1
				#print ("eval time: ", str_j)
				avg_eval_time[r] = parse_eval_time(str_j)
				continue

		if valid_run == 0:
			print("invalid run, repeating")
		
		

	# compute average:
	#print ("before avg: ")
	#print ("avg_eval_time: ", avg_eval_time)

total_time = 0.0
for i in range(run):
	total_time += avg_eval_time[i]

avg_time = total_time/run;

print ("avg_eval_time: ", avg_time)
print ("*****************************************************************")
print ()
print ()

