#!/bin/bash
echo "=====================  FOR RACER-4096 ======================="
echo ""
# brightness:
echo "For the microbenchmark brightness"
python3 test.py './programs-4096/brightness/16bbrightness.rc'
echo ""
# rgb2gray:
echo "For the microbenchmark rgb2gray"
python3 test.py './programs-4096/rgb2gray/rgb2gray.rc'
echo ""
# mmul:
echo "For the microbenchmark mmul"
python3 test.py './programs-4096/mmul/16bmmulLarge.rc'
echo ""
# mvmul:
echo "For the microbenchmark mvmul"
python3 test.py './programs-4096/mvmul/16bmvmulLarge.rc'
echo ""
# pagerank:
echo "For the microbenchmark pagerank"
python3 test.py './programs-4096/pagerank/main.rc'
echo ""
# dftSparse:
echo "For the microbenchmark dftSparse"
python3 test.py './programs-4096/dft/16b64dft.rc'
echo ""
# dftDense:
echo "For the microbenchmark dftDense"
python3 test.py './programs-4096/dft/16b16384dft.rc'
echo ""
# manhattan
echo "For the microbenchmark manhattan"
python3 test.py './programs-4096/distance/16b3dmanhattan.rc'
echo ""
# hamming:
echo "For the microbenchmark hamming"
python3 test.py './programs-4096/distance/16bhamming.rc'
echo ""
# lenet5:
echo "For the microbenchmark lenet"
python3 test.py './programs-4096/cnn/lenet5/main.rc' 85
echo ""
# grep:
echo "For the microbenchmark grep"
python3 test.py './programs-4096/reduce/wordcount/8bwordcountlarge.rc'
echo ""
# exactMatch:
echo "For the microbenchmark exactMatch"
python3 test.py './programs-4096/stringmatch/exact/8bexactmatchlarge.rc'
echo ""
# fuzzyMatch:
echo "For the microbenchmark fuzzyMatch"
python3 test.py './programs-4096/stringmatch/fuzzy/8bfuzzymatchlarge.rc'

echo "=====================  FOR RACER-1024 ======================="
echo ""
# brightness:
echo "For the microbenchmark brightness"
python3 test.py './programs-1024/brightness/16bbrightness.rc'
echo ""
# rgb2gray:
echo "For the microbenchmark rgb2gray"
python3 test.py './programs-1024/rgb2gray/rgb2gray.rc'
echo ""
# mmul:
echo "For the microbenchmark mmul"
python3 test.py './programs-1024/mmul/16bmmulLarge.rc'
echo ""
# mvmul:
echo "For the microbenchmark mvmul"
python3 test.py './programs-1024/mvmul/16bmvmulLarge.rc'
echo ""
# pagerank:
echo "For the microbenchmark pagerank"
python3 test.py './programs-1024/pagerank/main.rc'
echo ""
# dftSparse:
echo "For the microbenchmark dftSparse"
python3 test.py './programs-1024/dft/16b64dft.rc'
echo ""
# dftDense:
echo "For the microbenchmark dftDense"
python3 test.py './programs-1024/dft/16b16384dft.rc'
echo ""
# manhattan
echo "For the microbenchmark manhattan"
python3 test.py './programs-1024/distance/16b3dmanhattan.rc'
echo ""
# hamming:
echo "For the microbenchmark hamming"
python3 test.py './programs-1024/distance/16bhamming.rc'
echo ""
# lenet5:
echo "For the microbenchmark lenet"
python3 test.py './programs-1024/cnn/lenet5/main.rc' 20
echo ""
# grep:
echo "For the microbenchmark grep"
python3 test.py './programs-1024/reduce/wordcount/8bwordcountlarge.rc'
echo ""
# exactMatch:
echo "For the microbenchmark exactMatch"
python3 test.py './programs-1024/stringmatch/exact/8bexactmatchlarge.rc'
echo ""
# fuzzyMatch:
echo "For the microbenchmark fuzzyMatch"
python3 test.py './programs-1024/stringmatch/fuzzy/8bfuzzymatchlarge.rc'
