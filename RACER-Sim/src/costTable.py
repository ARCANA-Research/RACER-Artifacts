import csv
class CostTable:
    def __init__(self, opName='', cmosName=''):

        self.opTab={}
        self.cmosTab=[]
        self.localMemBusTab = {}
        self.globalMemBusTab = {}

        self.costTab = {
            'opTab': self.opTab,
            'cmosTab':self.cmosTab,
            'localMemBusTab':self.localMemBusTab,
            'globalMemBusTab':self.globalMemBusTab}

        with open(opName, newline='') as csvfile:
            reader = csv.reader(csvfile, delimiter=',', quotechar='|')
            count = 0
            for row in reader:
                if count == 0:
                    count +=1
                    continue
                self.opTab[row[0]] = {
                        'energyCost': float(row[1]),
                        'outputBits' : int(row[2]),
                        'latency' : float(row[3]),
                        'activeClusters': int(row[4]),
                        'warmupEnergy': float(row[5]),
                        'warmupLatency': float(row[6])
                    }

        with open(cmosName, newline='') as csvfile:
           reader = csv.reader(csvfile, delimiter=',', quotechar='|')
           for row in reader:

               #cluster wiring
               if 'localMemBus' in row[0]:
                   if 'PackageSize' in row[0]:
                       self.localMemBusTab['packageSize'] = int(row[1])
                   if 'Latency' in row[0]:
                       self.localMemBusTab['transferLatency'] = int(row[1])
                   if 'Energy' in row[0]:
                       self.localMemBusTab['transferEnergy'] = int(row[1])
                   if 'BusSize' in row[0]:
                       self.localMemBusTab['busSize'] = int(row[1])

               #chip wiring
               elif 'globalMemBus' in row[0]:
                   if 'PackageSize' in row[0]:
                       self.globalMemBusTab['packageSize'] = int(row[1])
                   if 'Latency' in row[0]:
                       self.globalMemBusTab['transferLatency'] = int(row[1])
                   if 'Energy' in row[0]:
                       self.globalMemBusTab['transferEnergy'] = int(row[1])
                   if 'BusSize' in row[0]:
                       self.globalMemBusTab['busSize'] = int(row[1])

               #CMOS peripherals
               else:
                   pass
