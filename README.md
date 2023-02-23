# Optimization-models

## Environment
* Python & SCIP (Jupyter)
* C# & IBM CPLEX 12.9 (Visual Studio 2022)
* C++ & IBM CPLEX 12.9 (Visual Studio 2022)

## Model Formulations

### Bin Packing Problem

***Sets and Parameters***

__items__ := the set of item have to be packed <br />
__num_sets__ := the number of available sets can be used to pack item <br />
__B__ := the capacity for each avaiable set <br />

s<sub>i</sub> := the lenth of item ***i*** <br />

***Decision Variables***

y<sub>j</sub> := {0,1} Whether the set ***j*** be used or not <br />
x<sub>i,j</sub> := {0,1} Whether the item ***i*** be packed in set ***j***

***Model***

Objective Function: <br />
$Minimize:  \sum_{j} y_j $

s.t.<br />
Capacity Constraints: 
$\sum_{i} s_i x_{ij} \leq B y_j $ 
∀ j
<br />
Assignment Constraints: 
$\sum_{j} x_{ij} = 1 $
∀ i
***
