#!/usr/bin/env python

import os
import subprocess
import sys


# Set up default variables
cwd = os.getcwd()
qemu_bin = '%s/qemu/qemu-system-x86_64' % cwd
# qemu_img = '/home/avadh/workspace/vm/spec2006_2.qcow2'
qemu_img ='point to where MRAM-ubuntu-natty.qcow2 is located'
# qemu_img = '/var/work/vm/splash_4core.qcow2'
vm_memory = '8G'
qemu_cmd = ''
vm_smp = 4
simconfig ='point to where /marss-emram/marss.dramsim/run.simcfg'

def add_to_cmd(opt):
    global qemu_cmd
    qemu_cmd = "%s %s" % (qemu_cmd, opt)

# Generate a common command string
add_to_cmd(qemu_bin)
add_to_cmd('-m %s' % vm_memory)
add_to_cmd('-serial pty')
#add_to_cmd('-smp %d' % vm_smp)
add_to_cmd('-vnc :20')
#add_to_cmd('-cpu core2duo')
add_to_cmd('-simconfig %s' % simconfig)

# Add Image at the end
# add_to_cmd('-hda %s' % qemu_img)
add_to_cmd('-drive file=%s,cache=unsafe' % qemu_img)

# Checkpoint list
check_list = []

# ======================= RACER's Baseline =====================================

bench='brightness'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./brightness.exe 4096"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='rgb2gray'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./rgb2gray.exe 4096"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='mmul'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./mmul.exe 20480 384"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='mvmul'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./mvmul.exe 1344 1048576"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='pagerank'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./pagerank.exe 32768 32768 9"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='dftSparse'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./dft.exe 4096 64 64"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='dftDense'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./dft.exe 1 16384 16384"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='manhattan'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./manhattan.exe 9437184"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='hamming'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./hamming.exe 15204352"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='lenet'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./lenet.exe 85"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='grep'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./grep.exe 16 58720256"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='exactMatch'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./exactMatch.exe 8192 256"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
check_list.append(bench_dict)

bench='fuzzyMatch'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./fuzzyMatch.exe 8192 256"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
check_list.append(bench_dict)

# ======================= RACER's Baseline2G ===================================
bench='2Gbrightness'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./brightness.exe 1024"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Grgb2gray'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./rgb2gray.exe 1024"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Gmmul'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./mmul.exe 5120 384"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Gmvmul'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./mvmul.exe 1344 262144"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Gpagerank'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./pagerank.exe 16384 16384 9"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2GdftSparse'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./dft.exe 1024 64 64"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2GdftDense'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./dft.exe 1 1 9216 9216"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Gmanhattan'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./manhattan.exe 2359296"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Ghamming'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./hamming.exe 3801088"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Glenet'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./lenet.exe 20"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2Ggrep'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./grep.exe 16 14689964"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2GexactMatch'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./exactMatch.exe 2048 256"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

bench='2GfuzzyMatch'
pre_command = "export CHECKPOINT_NAME=\"%s\"\n" %(bench)
cmd = "./fuzzyMatch.exe 2048 256"
bench_dict = {'name' : bench, 'command' : '%s\n%s\n' % (pre_command, cmd)}
#check_list.append(bench_dict)

# ======================= END RACER =====================================



print("Execution command: %s" % qemu_cmd)
print("Number of Chekcpoints to create: %d" % len(check_list))

login_cmds = ["root\n", "root\n"]

def pty_to_stdout(fd, untill_chr):
    chr = '1'
    while chr != untill_chr:
        chr = os.read(fd, 1)
        sys.stdout.write(chr)
    sys.stdout.flush()

def pty_login(fd):
    os.write(fd, login_cmds[0])
    pty_to_stdout(fd, ':')
    os.write(fd, login_cmds[1])

# Start simulation from checkpoints
pty_prefix = 'char device redirected to '
for checkpoint in check_list:

    print("Starting QEMU for checkpoint: %s" % checkpoint['name'])

    p = subprocess.Popen(qemu_cmd.split(), stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT, bufsize=0)

    pty_term = None

    while p.poll() is None:
        line = p.stdout.readline()
        sys.stdout.write(line)
        if line.startswith(pty_prefix):
            dev_name = line[len(pty_prefix):].strip()

            # Open the device terminal and send simulation command
            pty_term = os.open(dev_name, os.O_RDWR)

            break

    if pty_term == None:
        print("ERROR: While connecting with pty terminal")
        continue

    pty_to_stdout(pty_term, ':')

    # Now send the login commands to the termianl and wait
    # untill some response text
    pty_login(pty_term)

    pty_to_stdout(pty_term, '#')

    # At this point we assume that we have successfully logged in
    # Now give the command to create checkpoint
    os.write(pty_term, checkpoint['command'])

    pty_to_stdout(pty_term, '#')

    sys.stdout.write('\n')
    for line in p.stdout:
        sys.stdout.write(line)

    # Wait for simulation to complete
    p.wait()
