from src import hardware
import sys

chip = hardware.Chip(64,64,'../../racer-sim/cost/updatedOperationCosts.csv', '../../racer-sim/cost/cmosExample.csv')
chip.reset()
multithread = 1
if(len(sys.argv) > 2):
    multithread = int(sys.argv[2])

chip.loadInstruction(sys.argv[1])
chip.launch(0,multithread)
#print("*********** Test Program: " + sys.argv[1] + ' **************')
print("Energy Cost: " + str(chip.getEnergyCost()) + " pJ")
print("Latency Cost:" + str(chip.getLatencyCost()) + " ns")
