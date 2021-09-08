# RACER-Artifacts

Source code for artifacts used to evaluate the RACER architecture in the following paper:
>M. S. Q. Truong, E. Chen, D. Su, L. Shen, A. Glass, L. R. Carley, J. A. Bain, S. Ghose.
>"**RACER: Bit-Pipelined Processing Using Resistive Memory**".
>To appear in _Proceedings of the International Symposium on Microarchitecture_ (MICRO), October 2021.

## Overview
This repository includes a set of artifacts that capture the performance and energy consumption of RACER, our processing-using-memory (PUM) architecture.

The first artifact, **RACER-Sim**, is a custom simulator that calculates the latency, throughput, and energy of applications executing using a tiled crossbar memory,
using the same architecture proposed for RACER, but not limited to ReRAM-only memory devices.
For inputs, RACER-Sim takes in
(1) device-level latency and energy costs for the different arithmetic operations supported by the RACER architecture (with these costs derived from accurate device-level modeling), and
(2) a program written in the RACER ISA (for which we currently hand-compile microbenchmarks).
We use RACER-Sim to model the performance and energy consumption of the two RACER configurations evaluated in the paper **RACER-4096** and **RACER-1024** in Section 6 of the paper).

The second artifact is **MARSS-CPU**, an *unmodified* version of the MARSSx86 architectural simulator with custom configuration files. The simulator is bundled with DRAMSim2 and McPAT, to model the DRAM subsystem and power/energy, respectively. We use MARSS-CPU evaluate the performance and energy of the two conventional CPU configurations evaluated in the paper (**Baseline** and **Baseline-2GB** in Section 6 of the paper).

The third artifact is **MARSS-eMRAM**, a *modified* version of MARSSx86, which models an embedded main memory stacked on top of the CPU. We provide configuration files to model state-of-the-art embedded MRAM, though the configuration can be modified to model other embedded memory devices. We use MARSS-eMRAM to evaluate the performance and energy of the embedded MRAM configuration in the paper (**eMRAM** in Section 6 of the paper).

## Tested Environment & Dependencies
- Ubuntu 14.04.5
- Python 3.6
- Bash 4.3
- GNU Make 3.4
- Linux kernel version 3.13

## Directory Structure
The repository includes the following directories:
- `./marss-cpu/`: MARSSx86 with an unmodified version of DRAMSim2
- `./marss-emram/`: MARSSx86 with modified version of DRAMSim2 to simulate embedded MRAM.
- `./mcpat`: McPAT source code used for Baseline, Basleline--2GB, and eMRAM
- `./racer-sim/`: RACER-Sim files.
- `racer-sim/src`: Contains the simulation source code.
- `racer-sim/cost/`: Contains spreadsheets detailing the device- and circuit-level costs of RACER.
- `./programs/`:  Contains microbenchmarks written in C++ and in the RACER ISA, used as inputs for Baseline/Baseline-2GB/eMRAM and RACER-4096/RACER-1024 respectively.
- `./scripts/`: Contains scripts to setup and process simulations' outputs.

## Summary of Artifact Executions
There are 4 tasks required to collect the speedups and power savings reported in
the paper:
1. Run RACER-4096 and RACER-1024
2. Run Baseline, Baseline2G, and eMRAM
3. Post-simulation processig for Baseline, Baseline2G, and eMRAM
4. Compare RACER to baselines.

## 1. Running RACER-4096 and RACER-1024
 Our scripts can automatically run RACER-Sim and collect all microbenchmark latency and dynamic energy costs by running the following commands inside `./scripts/racer/`
 ```
 bash runracer1024.sh
 bash runracer4096.sh
 ```

## 2. Running Baseline, Baseline2G and eMRAM
These comparison points are run with MARSSx86, DRAMSim2, and McPAT. We provide
two options for user to run the simulations. We recommend running with option A
using the provided VM for first-time setups.

### Option A: Running with Virtual Machine
We have provided a VM with compiled version of MARSSx86 and DRAMSim2. The OVA file
of the VM machine can be found
[here](https://cmu.app.box.com/folder/145178824400?s=sx5kxcazaeiuvtmyq5pcdrbj4ajnjxjm).
We have also uploaded and created
the checkpoints onto two QEMU disk images: `ubuntu-natty.qcow2` used to run Baseline
and Baseline2G, and `MRAM-ubuntu-natty.qcow2` used to run eMRAM. Both disk images
can be found in the same link. The sudo password for the VM is: `arteval`. Users just
need to import the OVA into a VM program (e.g., VirtualBox) file and upload the
two disk images before running the simulations.

#### Artifact location in VM
The RACER-Artifacts repository is located at `/home/arteval/RACER-Artifacts`.We have installed the necessary dependencies to make marss.dramsim andDRAMSim2 compiled. Evaluators can run simulations for Baseline/Baseline2G and eMRAM at `/home/arteval/RACER-Artifacts/marss-cpu` and `/home/arteval/RACER-Artifacts/marss-emram` respectively.

#### Running simulations in VM
Before running Baseline/Baseline2G: In `/home/arteval/RACER-Artifacts/marss-cpu/marss.dramsim/util/util.cfg`, set the `img_dir` variable to where ubuntu-natty.qcow2 image is located in the VM.

To run Baseline, in `/home/arteval/RACER-Artifacts/marss-cpu` do:
```
bash runbench.sh
```
To run Baseline2G, in `/home/arteval/RACER-Artifacts/marss-cpu` do:
```
bash runbench2G.sh
```
Before running eMRAM: In `/home/arteval/RACER-Artifacts/marss-emram/marss.dramsim/util/util.cfg`, set the `img_dir` variable to where MRAM-ubuntu-natty.qcow2 image is located in the VM.

To run Baseline, in `/home/arteval/RACER-Artifacts/marss-emram` do:
```
bash runbench.sh
```
The resulting YML and dramsim.log files will be placed in  `/home/arteval/RACER-Artifacts/marss-cpu/marss.dramsim` and  `/home/arteval/RACER-Artifacts/marss-emram.dramsim`

### Option B: Native Execution
#### Step 1: Download the artifact repository
```
git clone https://github.com/ARCANA-Research/RACER-Artifacts.git
```
#### Step 2: Compile DRAMSim2
Installation guide can be found at the [DRAMSim2 repository](https://github.com/dramninjasUMD/DRAMSim2). We have installed the correct versions of DRAMSim2 in `./marss-cpu/` and `./marss-emram/`. Compile the DRAMSim2 shared library object with the following command from inside the DRAMSim2 directory (you will need to repeat this for each MARSSx86 directory):
```
make libdramsim.so
```
#### Step 3: Compile MARSSx86
Installation guide can be found at the [MARSSx86 repository](https://github.com/dramninjasUMD/marss.dramsim). Once completed, do the following:
- Download  and  unzip  the  disk  images  required  for  the  simulators  from [here](https://cmu.app.box.com/folder/145178824400?s=sx5kxcazaeiuvtmyq5pcdrbj4ajnjxjm)
- Compile marss.dramsim with the following command

```
scons -Q c=16 dramsim=<libdramsim.so location>
```
Noted that the provided disk images already have the necessary checkpoints for
all microbenchmarks presented in the paper. To view the list of checkpoints,
users can optionally install `qemu-utils` and do:
```
qemu-img info <disk image name>
```
We provided 2 disk images: `ubuntu-natty.qcow2` used to run Baseline
and Baseline2G, and `MRAM-ubuntu-natty.qcow2` used to run eMRAM.

#### Step 4: Compile McPAT
Installation guide can be found at the [McPAT repository](https://github.com/HewlettPackard/mcpat). Compile with `make`.

#### Step 5 (optional): Compile microbenchmarks binaries for MARSSx86
User can skip this step if they decide to use the provided disk images. C++ versions of the microbenchmarks are available at `./programs/marss/src/`. Run `make` in that folder, which will create the microbenchmarks' binaries in `./programs/marss/bin/`. Note that the microbenchmarks' source files require `#include ptlcalls.h`, which can be found in  `marss.dramsim/ptlsim/tools/`, to perform checkpointing (users will need to change the include path appropriately to point to the correct file).
#### Step 6 (optional): Upload binaries to disk images
The binaries can be uploaded to the disk image with the by running the following command in `./scripts/upload2image`:
```
bash upload.sh
```
Please specify the disk image and mounting locations in `./scripts/upload2image/upload.sh`
#### Step 7 (optional): Create checkpoints from the uploaded binaries
Users can optionally delete a given checkpoint from the provided image with:
```
qemu-img snapshot -d <checkpoint name> <disk image name>
```
Then users can create their own set of checkpoints by running the following command
inside the respective `marss.dramsim` folders:
```
./util/create_checkpoint.py
```
User need to points the `qemu_img` to where the disk image is located and `simconfig`
to where the `marss.dramsim/run.simcfg` file is located inside `./util/create_checkpoint.py`. To
selectively uploaded a subset of checkpoints (corresponding to a subset of microbenchmarks), add
the checkpoint to the `check_list` variable in `./util/create_checkpoint.py`. For example, to add the `mmul` checkpoint, do:
```python
bench='mmul'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./mmul.exe 20480 384"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
check_list.append(bench_dict) # This line should be uncommented
```
#### Step 8: Run simulations
Before running Baseline/Baseline2G: In `/home/arteval/RACER-Artifacts/marss-cpu/marss.dramsim/util/util.cfg`, set the `img_dir` variable to where ubuntu-natty.qcow2 image is located in the VM.

To run Baseline, in `/home/arteval/RACER-Artifacts/marss-cpu` do:
```
bash runbench.sh
```
To run Baseline2G, in `/home/arteval/RACER-Artifacts/marss-cpu` do:
```
bash runbench2G.sh
```
Before running eMRAM: In `/home/arteval/RACER-Artifacts/marss-emram/marss.dramsim/util/util.cfg`, set the `img_dir` variable to where MRAM-ubuntu-natty.qcow2 image is located in the VM.

To run Baseline, in `/home/arteval/RACER-Artifacts/marss-emram` do:
```
bash runbench.sh
```
The resulting YML and dramsim.log files will be placed in  `/home/arteval/RACER-Artifacts/marss-cpu/marss.dramsim` and  `/home/arteval/RACER-Artifacts/marss-emram.dramsim`

## 3. Post-Simulation Processing
#### Post-Process DRAM Numbers
 To obtain the DRAM’s interconnect  throughput  and  average  power  necessary  to  calculate  the baselines’energy cost, execute the following command:
 ```
 python3 ./scripts/postprocessing/grabPower.py <DRAM log file>
 ```
 The log file is generated as a part of running marss.dramsim

 #### Post-Process CPU Numbers
 The total cycle time of baselines are available from the YML file created when
 marss.dramsim was run. To extract times, run:
 ```
 python3 ./scripts/postprocessing/grabCycle.py <YML file>
 ```
 To obtain the CPU average power, we need to convert the YML output file to
 McPAT's XML format. Insde the installed `marss.dramsim` directory, do:
 ```
./util/marss2mcpat.py --marss <YML file> --xml_in Xeon.xml --cpu_mode user -o <unique XML name>
 ```
 Then, power numbers can be obtained by running the following command in the installed `./mcpat`
 directory:
 ```
 ./mcpat -infile <XML file produced previously> -print_level 5
 ```
 ## 4. Comparison
 There are eight attributes we obtain from running the simulations:
 - From `./scripts/racer/runracer<1024/4096>.sh`:

 (1) RACER-1024/4096 dynamic energy,

 (2) RACER-1024/RACER-4096 total latency
 - From `./scripts/postprocessing/grabCycle.py`:

 (3) Baseline/Baseline2G/eMRAM cycle time
 - From `./scripts/postprocessing/grabPower.py`:

 (4) Baseline/Baseline2G/eMRAM DRAM average power

 (5) Baseline/Baseline2G/eMRAM DRAM off-chip interconnect throughput
 - From McPAT:

 (6) Baseline/Baselin2G/eMRAM core power (including L1/L2 cache power)

 (7) Baseline/Baseline2G/eMRAM L3 cache power

 (8) Baseline/Baseline2G/eMRAM interconnect power
 We provide a comparison script that quickly calculates the speedups and energy
 savings based on the eight attributes above:
 ```
 python3 ./scripts/compare/compare.py <attribute CSV file>
 ```

 Note that user has to set the `compare.py` script whether the comparison is with
 RACER-1024 or RACER-4096 by uncomment out the correct line:
 ```python
 #Please use the top one for RACER-4096, bottom one for RACER-1024
 racertotalenergy.append(_racerDynEn + _racerLat*RACER4096LEAKAGEPOWER)
 #racertotalenergy.append(_racerDynEn + _racerLat*RACER1024LEAKAGEPOWER)
 ```
 Each row of the CSV contains the attributes for each microbenchmark. Run with `./scripts/compare/exxample.csv` should generate the reported speedups and energy savings for RACER-4096 compared to Baseline.
