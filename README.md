# SHA3 / SHAKE HW IP module

2024-03-07 - Eros Camacho-Ruiz (camacho@imse-cnm.csic.es)

This is the repository of the evaluation carried out in the SHA3/SHAKE section presented in the [PhD Dissertation](https://github.com/ErosCamacho/PhD_Dissertation/blob/main/PhD_Thesis_Eros_Camacho_Ruiz_vFinal_rev.pdf) entitled: <b>"Design of a hardware Root-of-Trust on embedded systems"</b>

The main idea of this repository is, following the standard FIPS 202:
- Present the IP Module developed in the context of the dissertation. 
- Present an extensible test functionality in which the NIST Tests Vector have been validated. 
- Define a demo for the modules presented.

*Note: All the content of this repository has been implemented using the Pynq Framework.*

<!-- TABLE OF CONTENTS -->
## Table of Contents
  <ol>
    <li><a href="#dir-struc">Directory structure</a></li>
    <li><a href="#ip-integ">IP Integration</a></li>
    <li><a href="#pre-pynqz2">Prerequisites for the Pynq-Z2 platform</a></li>
    <li><a href="#ins-test">Installation and Use of the Test</a></li>
	<li><a href="#ins-demo">Installation and Use of the Demo</a></li>
	<li><a href="#example">Example of the Demo</a></li>
    <li><a href="#note">Note for version</a></li>
    <li><a href="#contact">Contact</a></li>
	<li><a href="#developers">Developers</a></li>
  </ol>

## Directory structure <a name="dir-struc"></a>

- sha3_xl_1_0: the IP module of the SHA3/SHAKE algorithm.
- SHA3_SHAKE_xl: the folder that contains the software implementation and the HW call drivers.
    - files: this folder contains the files to be parsed in the demo of the SHA3/SHAKE.
	- NIST_TestVector: the NIST Test Vector for SHA3 and SHAKE.
    - sha3: this folder contains all the files to implement and use the SHA3/SHAKE either in HW and SW version.
    - sha3_test.c: main file to sha3 tests
	- shake_test.c: main file to shake tests
	- sha3_demo.c: main file to sha3 demo
	- shake_demo.c: main file to shake demo
    - Makefile: to generate the executables for the library
- README.md: this file 

## IP Integration <a name="ip-integ"></a>

The IP module is delivered in the ```sha3_xl_1_0``` folder. 
The design of the core part of the IP module is depicted in the next figure. 
The Basic version stores the intermediate result of the Keccak operation in the register and the Arithmetic Unit is just the Keccak function. 
Apart from that, two optimized versions were proposed. 
The first one is the concatenation of Keccak units. This reduce the number of operation cycles the more stages are concatenated.
The second one apart from concatenates Keccak units also concatenates intermediate registers. That allows to store intermediate results. 

![](images/schematic_sha3core.png)

The IP integration is finished adding an user interface in which it is possible to modify the next parameters of module:
- ```Version```: selection of the algorithm of SHA3/SHAKE that are compiled in the FIPS 202. 
- ```NUM_STAGES```: the number of concatenation stages. The maximum number is 24.
- ```STAGES_REG```: that allows the implementation of intermediate registers. 

<img src="images/IP_integrator_sha3.png" alt="" width="600"/>

The next figure shows the performance of the two optimizations in terms of throughput and efficiency. 

<img src="images/sha3_performance.png" alt="" width="600"/>

For further information, see Chapter 3 of the [PhD Dissertation](https://github.com/ErosCamacho/PhD_Dissertation/blob/main/PhD_Thesis_Eros_Camacho_Ruiz_vFinal_rev.pdf)

## Prerequisites for the Pynq-Z2 platform <a name="pre-pynqz2"></a>

1. Download the PYNQ C-API from https://github.com/mesham/pynq_api

2. Then, issue ```make```. Once it is built, issue ```sudo make install```. 

## Installation and Use of the Test <a name="ins-test"></a>

The Test section performs the NIST Test Vector of each algorithm. 

1. For compilation of a specific test for sha3:

```bash
make sha3_XXX_test
```

where `XXX` can be: `224, 256, 384, 512`. So, for example if the user wants to compile the case of sha3-512, 
they must issue: `make sha3_512_test`

and for shake:

```bash
make shake_XXX_test
```

where `XXX` can be: `128, 256`. So, for example if the user wants to compile the case of shake-256, 
they must issue: `make shaake_256_test`

2. For the use, the program has different input variables:
	- `-h` : Show the help.
	- `-hh` : Show the extended help.
	- `-v` : Verbose Level:
		- `1`: Show the Acceleration of each NIST run (By default).
		- `2`: Show the Acceleration + Output Data of each NIST run.
		- `3`: Show the Acceleration + Input/Output Data of each NIST run.
	- `-b` : Selection of NIST Bit Test.
		- `1`: Use the NIST Byte Test **(By default)**.
		- `2`: Use the NIST Bit Test. ***WARNING: The reference SW is not prepared for this test.***
	- `-n` : [Number] of test executions.
	- `-s` : Stop in each run.
	- `-i` : Select de initial value for the sample run. **It is not mandatory**
	- `-f` : Select de final value for the sample run. **It is not mandatory**

An example, if it is desired to performance 10 tests on the sha3-512, using the NIST Byte Test and those tests between 10 and 20, it has to be typed: `sha3_512_test -n 10 -b 1 -i 10 -f 20`.

## Installation and Use of the Demo <a name="ins-demo"></a>

The main idea of the Demo is to use the hardware implementations in real use of cases. For that it has been given the option to input either a prompt message or a file in ASCII format or hexadecimal format. 

1. For compilation of a specific demo for sha3:

```bash
make sha3_XXX_demo
```

where `XXX` can be: `224, 256, 384, 512`. So, for example if the user wants to compile the case of sha3-512, 
they must issue: `make sha3_512_demo`

and for shake:

```bash
make shake_XXX_demo
```

where `XXX` can be: `128, 256`. So, for example if the user wants to compile the case of shake-256, 
they must issue: `make shaake_256_demo`

2. For the use, the program has different input variables:
	- `-h` : Show the help.
	- `-v`: Verbose Level:
		- `1`: Show the Acceleration (By default).
		- `2`: Show the Acceleration + Output Data.
		- `3`: Show the Acceleration + Input/Output Data.
	- `-m`: Input message (HEX format).
	- `-t`: Input message (TXT format).
	- `-mf`: Input HEX file [file name].
	- `-tf`: Input TXT file [file name].

The results of the demo can also be checked in: https://emn178.github.io/online-tools/sha3_512.html.

## Example of the Demo <a name="example"></a>

There are several use of case of this demo, but in general it is going to be used to hash an input text in hex format and a file.

1. So, for example if the message `0x0123456789` is going to be hashed using the sha3-512
```bash
./sha3_512_demo -m 0123456789
```

whose result is shown in the next figure:



2. In this case if it is the content of a file (in hexadecimal) what is going to be hashed: 
```bash
./sha3_512_demo -mf input_data.txt
```



## Note for version <a name="note"></a>
### v. 1.0

* Reordered the repository structure.
* Added a Readme file. 

## Contact <a name="contact"></a>

**Eros Camacho-Ruiz** - (camacho@imse-cnm.csic.es)

_Hardware Cryptography Researcher_ 

_Instituto de Microelectrónica de Sevilla (IMSE-CNM), CSIC, Universidad de Sevilla, Seville, Spain_

## Developers <a name="developers"></a>
Eros Camacho-Ruiz

_Instituto de Microelectrónica de Sevilla (IMSE-CNM), CSIC, Universidad de Sevilla, Seville, Spain_


