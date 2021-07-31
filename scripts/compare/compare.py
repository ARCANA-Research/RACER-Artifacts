from scipy.stats.mstats import gmean

racerlat = []
cpulat = []
speedup = []
with open("./example.csv") as f:
    lines = f.readlines()
    for line in lines[1:]:
        racerlat.append(float(line.split(',')[2])) #milisecond
        cpulat.append(float(line.split(',')[3])/(2.2*10e9)*1000) #milisecond

for i in range(len(racerlat)):
    speedup.append(cpulat[i]/racerlat[i])

print(racerlat)
print(cpulat)
print(speedup)
print("Geo. Mean Speedup is", gmean(speedup))
