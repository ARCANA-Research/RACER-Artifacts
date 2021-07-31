from src.costTable import CostTable
class MemBus:
    def __init__(self, memBusTab):
        self.packageSize = memBusTab['packageSize']
        self.transferLatency = memBusTab['transferLatency']
        self.energyCost = memBusTab['transferEnergy']
        self.size = memBusTab['busSize']
        self.transferCount = 0

    def reset(self):
        self.transferCount = 0

    def getEnergyCost(self):
        return self.transferCount * self.energyCost

    def getLatencyCost(self):
        return self.transferCount * self.transferLatency

    def executeTransfer(self):
        self.transferCount += 1

class MultiWord:
    def __init__(self, id, costTab):
        self.id = id
        self.costTab = costTab
        self.energy = 0

    def reset(self):
        self.energy = 0

    def executeOp(self, op, multiplicativeFactor):
        self.energy += self.costTab['opTab'][op]['energyCost'] * multiplicativeFactor

    def getEnergyCost(self):
        return self.energy


class Cluster:
    def __init__(self, id, costTab=None):
        self.id = id
        self.costTab = costTab
        self.computeUnits = [MultiWord(i, self.costTab) for i in range(64)]
        self.localMemBus = MemBus(self.costTab['localMemBusTab'])

    def reset(self):
        [self.computeUnits[i].reset() for i in range(64)]

    def executeOp(self, op, multiplicativeFactor, id):
        self.computeUnits[id].executeOp(op, multiplicativeFactor)

    def getEnergyCost(self):
        multiWordEnergy = sum([self.computeUnits[i].getEnergyCost()
                               for i in range(64)])
        busEnergy = self.localMemBus.getEnergyCost()
        return (busEnergy + multiWordEnergy)

class Chip:
    def __init__(self, dimx=64, dimy=64, opCostFileName=None, cmosCostFileName=None):

        # compute related
        self.dimx = dimx
        self.dimy = dimy
        self.latency = 0
        self.energy = 0
        self.outBits = 0
        self.instSize = 0
        self.multithread = 1
        self.costTab = CostTable(opCostFileName, cmosCostFileName).costTab
        self.computeUnitGroups = [Cluster(id, self.costTab) for id in range(self.dimx*self.dimy)]
        self.globalMemBus = MemBus(self.costTab['globalMemBusTab'])

        # config related
        self.instructionMemory = []
        self.pc = 0
        self.mask = []
        self.loops = {}

################ Performance/Energy calculation functions: #####################
    def reset(self):
        self.latency = 0
        self.energy = 0
        self.outBits = 0
        self.instructions = []
        self.pc = 0
        self.instSize = 0
        [self.computeUnitGroups[i].reset() for i in range(self.dimx*self.dimy)]

    def executeOp(self, instruction, id):
        op = instruction[0]
        multiplicativeFactor = int(instruction[-1])
        clusterID = id // 64
        multiWordID = id % (64)
        if ('LOAD' not in op and 'STORE' not in op):
            self.computeUnitGroups[clusterID].executeOp(op, multiplicativeFactor, multiWordID)
        else:
            if ('SLLOAD' == op):
                localMemBusID = int(instruction[1]) // 64
                self.computeUnitGroups[localMemBusID].localMemBus.executeTransfer()
            if ('SGLOAD' == op):
                self.globalMemBus.executeTransfer()

    def getEnergyCost(self):
        clusterEnergy = sum([self.computeUnitGroups[i].getEnergyCost()
                             for i in range(self.dimx*self.dimy)])
        busEnergy = self.globalMemBus.getEnergyCost()
        return (busEnergy + clusterEnergy + self.energy) * float(self.multithread)

    def getLatencyCost(self):
        busLatency = self.globalMemBus.getLatencyCost()
        return self.latency + busLatency

    def getOutputBits(self):
        return self.outBits

    def getSingleInstructionThroughput(self): # Giga per second
        return (self.outBits/(8*self.latency))

################ Instruction-related functions: ################################
    def loadInstruction(self,fileName=None):
        blockComment = False
        i = 0

        with open(fileName, 'r') as file:
            lines = file.readlines()

            while(i < len(lines)):
                line = lines[i].strip()

                if line != '' and line[0:2] != '//':
                    if line[0:2] == '/*':
                        blockComment = True
                    if (not blockComment):
                        if ('IMPORT' in line):
                            subFileName = line.split(" ")[1] # This can be nested
                            self.loadInstruction(fileName=subFileName)
                        else:
                            self.instructionMemory.append(line.split(' '))
                    if line[-2:] == '*/':
                        blockComment = False
                i+=1
        self.instSize = len(self.instructionMemory)

    def configSetup(self):
        #self.mask store the compute unit to be called
        while(True):
            opCode = self.instructionMemory[self.pc][0]

            if ('SET' not in opCode): # regular instruction
                break
            else: # configuration instruction
                # broadcast
                if ('SETALL' == opCode):
                    self.mask = [i for i in range(self.dimx*self.dimy*64)]
                elif ('UNSETALL' == opCode):
                    self.mask = []

                # specific compute unit
                elif ('SET' == opCode):
                    self.mask.append(int(self.instructionMemory[self.pc][1]))
                elif ('UNSET' == opCode):
                    self.mask.remove(int(self.instructionMemory[self.pc][1]))

                # group of compute unit
                elif ('SETBULK' == opCode):
                    start = int(self.instructionMemory[self.pc][1])
                    num = int(self.instructionMemory[self.pc][2])
                    step = int(self.instructionMemory[self.pc][3])
                    stop = start + num * step
                    self.mask.extend([i for i in range(start, stop, step)])
                elif ('UNSETBULK' == opCode):
                    start = int(self.instructionMemory[self.pc][1])
                    stop = int(self.instructionMemory[self.pc][2])
                    step = int(self.instructionMemory[self.pc][3])
                    removeList = [i for i in range(start, stop, step)]
                    for id in removeList:
                        self.mask.remove(id)

                self.pc += 1

    def executeEpoch(self):

        # Sort execution order (no more than 1 multi-word per cluster)
        executionSquences = []
        sequence = []

        #self.mask.sort()
        #print(self.mask)
        overlapList = []
        clusterList = [i//64 for i in self.mask]

        for id in range(len(self.mask)):
            if clusterList[id] not in overlapList:
                overlapList.append(clusterList[id])
                sequence.append(self.mask[id])
            elif clusterList[id] in overlapList:
                executionSquences.append(sequence)
                sequence = [self.mask[id]]
                overlapList = [clusterList[id]]
        executionSquences.append(sequence)

        # Get compute instruction
        computeInstructions = []
        while (True):
            opCode = self.instructionMemory[self.pc][0]
            if ('SET' not in opCode):
                computeInstructions.append(self.instructionMemory[self.pc])
                self.pc += 1
            else:
                break

        # Calculate compute instruction's latency and output bits produced
        epochLatency = 0
        epochEnergy = 0

        for instruction in computeInstructions:
            # print(instruction)
            #Transfer latency is handled by membus
            if 'LOAD' in instruction[0]:
                continue

            if 'SHIFT' in instruction[0]:
                hopLatCost = 482.0 # nanosecond per hop
                hopEnergyCost = 4097 # pJ per hop
                epochLatency += float(instruction[1]) * hopLatCost;
                epochEnergy += float(instruction[2]) * hopEnergyCost;
                continue
            if 'PROP' in instruction[0]:
                propLatCost = 12.0 # nanosecond to shift from one tile to another
                propEnergyCost = 3.2768 #  pJ to shift from one tile to another
                epochLatency += float(instruction[1]) * propLatCost
                epochEnergy += float(instruction[2]) * propEnergyCost
                continue

            warmUpEnergy = self.costTab['opTab'][instruction[0]]['warmupEnergy']
            warmupLatency = self.costTab['opTab'][instruction[0]]['warmupLatency']
            energyCost = self.costTab['opTab'][instruction[0]]['energyCost']
            latencyCost = self.costTab['opTab'][instruction[0]]['latency']
            multiplicativeFactor = int(instruction[-1])
            dynamicPower = 0.00079
            # if pipeline-able
            if (warmUpEnergy != 0 or warmupLatency !=0):
                fleetSize = int(instruction[-2])
                epochLatency += (latencyCost * fleetSize + warmupLatency) * len(executionSquences)
                #print(len(executionSquences))
                epochDynamicEnergy = epochLatency * dynamicPower * 1000 # AE Version
                epochEnergy += ((energyCost * fleetSize + warmUpEnergy) * multiplicativeFactor + epochDynamicEnergy) * sum([len(sequence) for sequence in executionSquences])
                # print(len(executionSquences))
                # print(epochLatency)
                # print(epochDynamicEnergy)
                # print(sum([len(sequence) for sequence in executionSquences]))
            # if not pipeline-able, then just add serially
            else:
                epochLatency += latencyCost * len(executionSquences)
                epochDynamicEnergy = latencyCost * dynamicPower * 1000 # pico Joules
                epochEnergy += epochDynamicEnergy * sum([len(sequence) for sequence in executionSquences]) # Cluster and Multiword handle this case
            # print(sum([len(sequence) for sequence in executionSquences]))
            # print(len(executionSquences))
            # print(len(executionSquences[0]))

        self.latency += epochLatency
        self.energy += epochEnergy

        # Execute sequences serially
        for sequence in executionSquences:
            for id in sequence:
                for instruction in computeInstructions:
                    if (instruction[0] == 'SHIFT' or instruction[0] == 'PROP'):
                        continue

                    warmUpEnergy = self.costTab['opTab'][instruction[0]]['warmupEnergy']
                    warmupLatency = self.costTab['opTab'][instruction[0]]['warmupLatency']
                    # Pipeline energy already handled from above
                    if (warmUpEnergy != 0 and warmupLatency != 0):
                        continue

                    self.executeOp(instruction,id)

    def launch(self,pc,_multithread=1):
        self.pc = pc
        self.multithread = int(_multithread)
        while(self.pc < self.instSize - 1):
            self.configSetup()
            self.executeEpoch()
