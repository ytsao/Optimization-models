# Optimization-models

This repository aims to provide the sample code to any beginner to learn how to use different programming languages and optimization solvers to build well-known combinatorial optimization models.

## Environment Setting
Our tutorial is based on Linux Ubuntu except C# is based on Windows 11 and Visual Studio 2022. If you are Windows user, you can use following steps to install WSL (Windows Subsystem for Linux).

1. Control Panel -> Programs -> Turn Windows features on or off.
2. Check "Virtual Machine Platform", "Windows Hypervisor Platform" (Option), "Windows Subsystem for Linux".
3. Restart laptop.
4. Open Powershell as Administrator, enter the "wsl --install".
```powershell
wsl --install
```
5. Open Microsoft Store, install "Unbuntu" then open it. (Choose LTS version)
6. Create the user account and password.
7. Install basic packages 
```cmd
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install build-essential
```


### References
* https://learn.microsoft.com/en-us/windows/wsl/install
* https://bayareanotes.com/wsl-installation/

## Combinatorial Optimization Problems
* bin packing
* multiple knapsack
* set covering
* graph coloring
* matching
* maximum clique
* shortest path
* TSP (DFS, MTZ)
* maximum flow 
* minimum cost flow

keep developing...


## Tutorial in Several Solvers with Different Programming Languages
In each folder, there is a installation guidline for each solver in different programming laugues. 

### CPLEX
* C#
* C++

### Gurobi
* Python

### SCIP
* C++ 
* Python

### JuMP
* Julia

### OR-Tools
* Python 

### Minizinc
* Minizinc IDE
