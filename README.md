# Optimization-models

## Environment
* Python & SCIP (Jupyter on Windows 11)
* C# & IBM CPLEX 12.9 (Visual Studio 2022 on Windows 11)
* C++ & IBM CPLEX 12.9 (Visual Studio 2022 on Windows 11)
* practice (C++ & SCIP 8.0.4 on WSL)



## 1. Install WSL
1. Control Panel -> Programs -> Turn Windows features on or off.
2. Check "Virtual Machine Platform", "Windows Hypervisor Platform" (Option), "Windows Subsystem for Linux".
3. Restart laptop.
4. Open Powershell as Administrator, enter the "wsl --install".
5. Open Microsoft Store, install "Unbuntu" then open it.
6. Create the user account and password.
7. sudo apt-get install build-essential

## 2. Install SCIP on WSL
#### 1. Download SCIP from following links:
* [SCIP Download](https://scipopt.org/index.php#download)
* [Auto Installation File Download](https://gcg.or.rwth-aachen.de/doc-3.5.0/installSCIPopt.sh)

#### 2. Enter WSL and create the folder to install SCIP.
```shell
mkdir gcg+scip
chown username gcg+scip
cd gcg+scip/
```

* Put the SCIP installation files into gcg+scip/
    - \\wsl.localhost\Ubuntu-20.04\home\username\gcg+scip\

#### 3. Start install SCIP
```shell
chmod +x installSCIPopt.sh
./installSCIPopt.sh
```

"Please enter path to SCIP Optimization Suite tarball:" -> **scipoptsuite-x.y.z.tgz** (x.y.z refers SCIP version)
<br/>
"Intall the SCIP Optimization Suite using..." -> **Makefile [for advanced testing]**
<br/>
"Enter soft-link target file or directory for "lib/include/bliss" (return if not needed)" -> **Press enter**.
<br/>
"Enter soft-link targe file or directory for "lib/static/libbliss.a" (return if not needed)" -> **Press enter**.
<br/>
"Do you wnat to test the suite? (this should not take long) [Y/n]" -> **y**

```shell
cd scipoptsuite-x.y.z/
```

#### 4. Install each package by cmake
##### 4-1. SOPLEX
```shell
cd soplex/
mkdir installation
cd installation/
cmake ..
make 
sudo make install
cd ~/gcg+scip/scipoptsuite-x.y.z/
```
    
##### 4-2. PAPILO
```shell
mkdir installation
cd installation/
cmake ..
sudo make install
cd ~/gcg+scip/scipoptsuite-x.y.z/
```

##### 4-3. ZIMPL
```shell
cd
sudo apt-get update
sudo apt-get installation flex bison
cd /gcg+scip/scipoptsuite-x.y.z/zimpl/
mkdir installation
cd installation/
cmake ..
make 
sudo make install
cd ~/gcg+scip/scipoptsuite-x.y.z/
```

##### 4-4. IPOPT
```shell
cd 
sudo apt-get install libblas-dev liblapack-dev
sudo apt-get install pkgconf
git clone https://github.com/coin-or/Ipopt.git
cd Ipopt
mkdir build
cd build
../configure
make 
sudo make install
cd ~/gcg+scip/scipoptsuite-x.y.z/
```

##### 4-5. SCIP
```shell
cd scip/application/
cmake ..
make 
sudo make install
mkdir installation
cd installation/
cmake .. -DIPOPT_INCLUDE_DIRS=~/gcg+scip/scipoptsuite-8.0.4/Ipopt/build/
make 
sudo make install
cd ~/gcg+scip/scipoptsuite-x.y.z/
```	

##### 4-6. GCG
```shell
cd gcg/
mkdir installation
cd installation/
cmake .. -DSCIP_DIR=~/gcg+scip/scipoptsuite-8.0.4/scip/installation/
make
sudo make install
```
	
#### 5. Testing first SCIP program.
```shell
cd 
mkdir code
cd code 
touch main.cpp
vim main.cpp
```
```C++
// main.cpp
#include <bits/stdc++.h>
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include <gcg/gcg.h>

int main(){
	std::cout << "Hello, world" << std::endl;
	return 0;
}
```

```shell
touch CMakeLists.txt
vim CMakeLists.txt
```

```CMake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.3)
project(first_scip)
find_package(SCIP REQUIRED)
find_package(GCG REQUIRED)
include_directories(${SCIP_INCLUDE_DIRS})
include_directories(${GCG_INCLUDE_DIRS})
add_executable(firstscip main.cpp)
```

```shell
cmake CMakeLists.txt
make all
./firstscip
```
Finally, console will print "Hello, world".
Then installation successful!
