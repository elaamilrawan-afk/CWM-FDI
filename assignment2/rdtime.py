import time

#Returns a single timestamp counter read

def get_cpu_time_counter():
	return time.perf_counter_ns()



#Returns the difference between two consecutive reads

def get_cpu_time_diff():
	time1 = time.perf_counter_ns()
	time2 = time.perf_counter_ns()
	return time2-time1
	

#Writes a list of values to a file
#Inputs:
#   values - a list of values
#   filename - output file name

def write_list_to_file(values, filename):
    with open(filename, "w") as f:
        for value in values:
            f.write(f"{value}\n")


#Read the timestamp counter multiple times and return
#the minimum difference between two consecutive reads.
#Inputs:
#   num - number of times to read the timestamp
#   filename - output file name
#Output:
#   min_diff - the minimum difference between two 
#   consecutive reads

def get_min_time_diff(num,filename):
	if num < 2:
        	raise ValueError("num must be at least 2")

	prev = time.perf_counter_ns()
	min_diff = None
	diff=[0]*num
	for i in range(num - 1):
		curr = time.perf_counter_ns()
		diff[i] = curr - prev
		if min_diff is None or diff[i] < min_diff:
			min_diff = diff[i]
		prev = curr
	write_list_to_file(diff,filename)
	return min_diff

#Main
	
if __name__ == "__main__":
	filename = "time_py.txt"
	num = 1000000
	counter = get_cpu_time_counter()
	print(f"CPU time counter: {counter} ns")
	counter2 = get_cpu_time_counter()
	print(f"CPU time diff: {counter2-counter} ns")

## Uncomment the lines below to run multiple measurement iterations
	min_diff=get_min_time_diff(num,filename)
	print(f"CPU min diff time: {min_diff} ns")
