import sys
power = []
band = []
with open (str(sys.argv[1])) as file:
	lines = file.readlines()
	for line in lines:
		if "Average Power" in line:
			power.append(float(line.split()[4]))
		if "bandwidth" in line:
			band.append(float(line.split()[10][:-4]))

print(sys.argv[1])
print(sum(power)/len(power))
print(sum(band)/len(band))
