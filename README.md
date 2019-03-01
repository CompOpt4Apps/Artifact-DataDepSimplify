# Overview 

This artifact reproduces the evaluation results for the following paper accepted in PLDI 2019:

"Sparse Computation Data Dependence Simplification for Efficient Compiler-Generated Inspectors"

The artifact uses the CHiLL compiler framework for data dependence extraction, IEGenLib and ISL libraries
for dependence simplification, and Omega+ library for runtime inspector code generation. 

There are two parts to the results with details for each below: 
(1) Data dependency simplification analysis evaluation, presented as Figure 6 in the paper. 
(2) Performance evaluation results presented in Figures 7 and 8 in the paper.

# QuickStart

The build has only been tested on Ubuntu 16.04 machine.  Due to the need for
Boost and ROSE for the CHiLL compiler, building all of the software on other platforms
could be excessively challenging.

To download and build all of the software needed execute the following command.
The last section in this README provides more details about installation if you run
into difficulties.
```
./install.sh
```

The following provides details on how to reproduce the results in the paper.

## (1) Reproducing data dependency simplification analysis results
After building the driver and its dependencies (by running install.sh) 
run the following:

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./boost/lib
./simplification list.txt
```

Figure 6 is generated and stored in the results directory at the end.
The output will be the results of doing data dependence analysis on all of the
C kernels specified in the list.txt file. The shell output summarises some results 
and points to output files with more detailed results.
The list.txt file includes names of some JSON files. 
Each JSON file contains the relative path of an input kernel that we want to extract its 
dependences and analyze them for partial parallelism.  The JSON files also contain index 
array properties and analysis information such as which loops we want to analyze.

The driver will produce Figure 6 using gnuplot. Please note however that the reproduced figure 
is not as polished aesthetically as the one in the paper, nonetheless it is the same 
figure in terms of data.

## (2) Performance evaluation results presented in Figures 7 and 8 in the paper
After building the driver and its dependencies (by running install.sh) 
run the following in the main directory:


```
  ./codegen codeGenlist.txt
```

This will generate inspector codes that are needed for runtime inspection
of the remaining maybe satisfiable dependences.
Then, go to performanceEval directory, and run the following to
install performance evaluation specific requirements, download the input
matricies used in the evaluations, and run evaluation drivers:

```bash
./cd performanceEval
./getDataAndInstall.sh
```

The script will install the Metis library (requirement), get the input matrices
(listed in Table 3 of the paper), and compile and run the performance drivers. 
After the run finishes, the results, Table 4, and Figure 7 and 8 of the paper,
will be in the results directory (performanceEval/results).

To rerun performance evaluations after running above script once 
run the following command:

```bash
./performance execList.txt
```


# Specifying index array properties
The index array properties are specified inside JSON files for each kernel. 
The properties are specified in two ways:
1. Some mathmatical properties about a function are defined for each
index array. For instance if you look at data/ic0_csc.json file which is the input JSON file for 
Incomplete Cholesky code (data/ic0_csc.c), you can see the following:
```
        { 
           "Name" : "colPtr",
           "Domain" : "{[j]:0<=j &&j<n}",
           "Range" : "{[i]:0<=i &&i<nnz}",
           "Bijective" : "false",
           "Monotonicity" : "Monotonic_Increasing"
        }
```
Here we define four properties for colPtr index array: 
(1) Domain, which would be the range of 
    the numbers that can be used to index colPtr itself. 
(2) Range, which is the range values stored in coPtr. 
(3) Whether the uninterpreted function (UF) representing colPtr 
    can be considered bijective.
(4) Whether UF can be considered monotonic and in what manner.

2. More general index array properties such as a relationship between two or more 
index arrays, etc, can be defined as universially quantifed assertions.
For instance if you look back at data/ic0_csc.json file again, you can see the following:
```
      "User Defined" :
      [
        {
          "Type" : "Triangularity",
          "UniQuantVar" : "[e1,e2]",
          "Rule (comment)" : "Forall e1,e2, colPtr(e1) < e2 => e1 < rowIdx(e2)",
          "p" : "colPtr(e1) < e2",
          "q" : "e1 < rowIdx(e2)"
        }
      ]
```
This assertion defines the Triangularity properties that can be expressed 
with a relationship between colPtr and rowIdx index arrays. 
In this syntax, we are trying to define
`Forall e1,e2,  e1 < colPtr(e2) => rowIdx(e1) < e2` assertion. In the components, 
`UniQuantVar` field defines the `Forall e1,e2` part; 
`p` defines the left hand side of the inference, e.g the `colPtr(e1) < e2` part;
`q` defines the left hand side of the inference, e.g the `e1 < rowIdx(e2)` part.

Index array properties defined in the JSON files get stored in the IEGenLib library's
environment as universially quantified assertions. Then, whenever they are needed 
for instance for detecting unsatisfiable relations, they are instantiated as 
described in Section 3.2 of the paper. 

**In IEGenLib, the universially quantified assertions are stored as objects of
UniQuantRule class** that is defined inside IEGenLib/src/set_relation/environment.h.
This class stores the assertions as two iegenlib::Set object, one for left hand side,
and one for right hand side, which makes defining assertions easy since
iegenlib::Set's can represent any set of equalitiy and inequality constraints
that include uninterpreted function symbols with arbitrary parameters.
Assertions can be defined and added to the environment with following syntax:
```
  UniQuantRule *uqRule;
  uqRule = new UniQuantRule("Triangularity", "[e1,e2]", "colPtr(e1) < e2", "e1 < rowIdx(e2)");
  currentEnv.addUniQuantRule( uqRule );
```
**The addUniQuantRule( uqRule ) call adds the assertion to the environment.**


# Code generation limitations
We use Omega+ to generate inspector codes. Omega+ has limited support to handle
uninterpreted functions calls. As a consequence, we need to do preprocessing inside our drivers
so that Omega+ can handle our dependences. One of the preprocessing steps include
reodering iterators in a way that Omega+ can utilize equality constraints to
remove loops from inspectors. At this time Omega+ can only take advantage of such equality 
constraints if only the iterator that its loop can be removed is the inner most loop.
For instance consider following made up dependence:
```
{ [i,ip,jp]: i = c(jp) and 0<i,ip<n and r(ip)<jp<r(ip+1)}
```
We do not need a loop for i-iterator in the inspector for above example, since we have
"i = c(jp)".
In oredr to enable Omega+ to utilize the equality, we reorder the dependence iterators
as follow:
```
{ [ip,jp,i]: i = c(jp) and 0<i,ip<n and r(ip)<jp<r(ip+1)}
```
However, at this time, our driver does not check for the correctness of this reordering.
We have verified the correctness for our kernels by hand at this time.
Due to such complications, we also cannot use Omega+ to generate inspectors
for Incomplete Cholesky kernel. Consequently, we have included hand written
inspectors for this kernel, similar to inspectors that Omega+ can produce.
At the moment, we are working to address these limitations.


# How to build the artifact:

We have tested the artifact build and execution on the following platform:

+ OS:  Ubuntu 16.04
+ GCC: gcc 5.4.0 (Default on the OS)

** There is a installation script, install.sh, in the same directory as this README
that can build all the dependencies and the driver for the artifact including 
Rose compiler and boost library. Nonetheless, it is only tested in the above platform.
Please note that the installation script must be executed with sudo access:**

```
    sudo ./install.sh
```

In case users need to use other platforms or in case the script fails for whatever reason,
users can follow these more detailed instructions to build all the dependencies step by step:

These instructions assume that you are starting from the root directory of the artifact.
To build the driver for the artifact, these instructions indicate how to build and 
locally install the following major components:
(1) Boost library; (2) rose compiler; (3) IEGenLib library; (4) CHILL compiler.
Of these components, the rose compiler, which is a dependency for CHILL, is the most demanding
one in terms of building effort.


## Step 0: Installation prerequisites:

These softwares are prerequisites for installing all the subsequent components:

`sudo apt-get install git wget tar unzip make autoconf automake cmake libtool default-jdk default-jre flex bison python-dev texinfo gnuplot-x11 evince`


## Step 1: Installing boost:

Boost library is a prerequisit for rose compiler:

`mkdir boost`

`export BOOSTHOME=$(pwd)/boost`

`wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz`

`tar -xvzf boost_1_60_0.tar.gz`

`cd boost_1_60_0/`

`./bootstrap.sh --prefix=$BOOSTHOME --with-libraries=chrono,date_time,filesystem,iostreams,program_options,random,regex,signals,system,thread,wave`

`./b2  --prefix=$BOOSTHOME -sNO_BZIP2=1 install`

`cd ..`



## Step 2: Installing rose compiler:

Detailed insructions for installing rose compiler including some trouble shooting 
information can be found in the official installation guide of rose compiler webpage:

http://rosecompiler.org/ROSE_HTML_Reference/installation.html


The following is a quick installation guide:


+ Setting up the environment
`mkdir rose_build`

`export ROSE_SRC=$(pwd)/rose`

`export ROSEHOME=$(pwd)/rose_build`

`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${BOOSTHOME}/lib`

`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${ROSEHOME}/lib`

`git clone https://github.com/rose-compiler/rose`

`cd rose`


+ Getting the following version instead of the latest version 
  is highly recommended for installing CHILL.

`git checkout 365bb6857c`

`./build`

`cd $ROSEHOME`

+ Excluding JAVA might make things easier.

`${ROSE_SRC}/configure --prefix=$ROSEHOME --enable-languages=c,c++ --with-boost=${BOOSTHOME} --without-java`

+ You can remove -j6 if yop do not wish to build with multi-threading 
activated, or at least decrease the number of threads by decreaseing 
the j6 to jX (with X being desired number of threads)

`make -j6 install-rose-library FRONTEND_CXX_VENDOR_AND_VERSION2=gnu-5.3`

`cd ..`



## Step 3: Installing the IEGenLib library:

Considering that the artifact already has a copy of the appropriate 
IEGenLib version, you do not need to get this library from its github 
repository.  (https://github.com/CompOpt4Apps/IEGenLib)

`export IEGENHOME=$(pwd)/IEGenLib/iegen`

`cd IEGenLib`

`./configure`

`make`

`make install`

`cd ..`



## Step 4: Installing the CHILL compiler:

Considering that the artifact already has the appropriate CHILL version, 
you do not need to download this library. Please note that 
although CHILL is made publicly available as tar balls that are relased 
every once in a while, it does not have a public github repository. 
Use the following commands to build the CHILL version that is already available 
in the artifacts root directory:

`cd chill`

`mkdir build`

`cd build`

`cmake .. -DROSEHOME=$ROSEHOME -DBOOSTHOME=$BOOSTHOME -DIEGENHOME=$IEGENHOME`

`make -j6`

`cd ../..`




## Step 5: Building the artifact driver:

Use following command to build the driver for reproducing the result:

`g++ -O3 -o simplifyDriver simplification.cc -I IEGenLib/src IEGenLib/build/src/libiegenlib.a -lisl -std=c++11`



