# Optimization-models

This repository aims to provide the sample code to any beginner to learn how to use different programming languages and optimization solvers to build well-known combinatorial optimization models.

In addition, this repository provides a framework to make different solvers (SCIP, gurobi) have same modeling interface in python.

## Environment Setting
Our tutorial is based on Linux Ubuntu except Cplex is based on Windows 11 and Visual Studio 2022. If you are Windows user, you can use following steps to install WSL (Windows Subsystem for Linux).

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
* [Bin Packing](https://en.wikipedia.org/wiki/Bin_packing_problem)
* [Multiple Knapsack](https://en.wikipedia.org/wiki/Knapsack_problem)
* [Set Covering](https://en.wikipedia.org/wiki/Set_cover_problem)
* [Graph Coloring](https://en.wikipedia.org/wiki/Graph_coloring)
* [Matching](https://en.wikipedia.org/wiki/Matching_(graph_theory))
* [Maximum Clique](https://en.wikipedia.org/wiki/Clique_problem)
* [Shortest Path](https://en.wikipedia.org/wiki/Shortest_path_problem)
* [TSP (DFS, MTZ)](https://en.wikipedia.org/wiki/Travelling_salesman_problem)
* [Maximum Flow ](https://en.wikipedia.org/wiki/Maximum_flow_problem)
* [Minimum Cost Flow](https://en.wikipedia.org/wiki/Minimum-cost_flow_problem)

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

Still developing... and welcome to contribute this repository!