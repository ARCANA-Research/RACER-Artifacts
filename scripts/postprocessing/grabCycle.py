import sys

cycles = []
with open (str(sys.argv[1])) as file:
	lines = file.readlines()
	cur_line = 0
	i = 0
	for line in lines:
		if "xeon_plat_" in line:
			cycles.append(int(lines[i+1].split()[1]))
		i += 1
print(max(cycles[16:32]))

