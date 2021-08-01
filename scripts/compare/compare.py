from scipy.stats.mstats import gmean

#############################CONSTANT VARIABLES#################################
CPUCLKFREQ = 2200000000
TECHSCALE = 1.5
GBS2WATT = 0.175
RACER4096LEAKAGEPOWER = 3.301
RACER1024LEAKAGEPOWER = 0.825
#############################CALCULATE SPEEDUPS##################################
racerlat = []
cpulat = []
speedup = []
with open("./example.csv") as f:
    lines = f.readlines()
    for line in lines[1:]:
        racerlat.append(float(line.split(',')[2])) #milisecond
        cpulat.append(float(line.split(',')[3])/(CPUCLKFREQ)*1000) #milisecond

for i in range(len(racerlat)):
    speedup.append(cpulat[i]/racerlat[i])

print("Geo. Mean Speedup is %dx"%(round(gmean(speedup))))
#############################CALCULATE ENERGY SAVINGS############################
racertotalenergy = []
cputotalpower = []
energysaving = []
with open("./example.csv") as f:
    lines = f.readlines()
    for line in lines[1:]:
        _racerDynEn = float(line.split(',')[1]) # Joules
        _racerLat = float(line.split(',')[2]) / 1000 # seconds

        #Please use the top one for RACER-4096, bottom one for RACER-1024
        racertotalenergy.append(_racerDynEn + _racerLat*RACER4096LEAKAGEPOWER)
        #racertotalenergy.append(_racerDynEn + _racerLat*RACER1024LEAKAGEPOWER)

        _offchipinterconnectpower = float(line.split(',')[5]) * GBS2WATT
        _drampower =  float(line.split(',')[4])
        _corepower =  float(line.split(',')[6]) / TECHSCALE
        _l3power = float(line.split(',')[7])  / TECHSCALE
        _nocpower = float(line.split(',')[8])  / TECHSCALE

        _totalpower = _offchipinterconnectpower+_drampower+(_corepower+_l3power+_nocpower)
        cputotalpower.append(_totalpower)

for i in range(len(racertotalenergy)):
    energysaving.append((cpulat[i]/1000*cputotalpower[i])/racertotalenergy[i])

print("Geo. Mean Energy Saving is %dx"%(round(gmean(energysaving))))
