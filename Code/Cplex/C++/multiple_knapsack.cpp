#include "multiple_knapsack.h"

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

void multiple_knapsack::Build() {

	IloEnv env;
	IloModel model(env);
	IloCplex cplex(env);

	int c[] = { 100,100,100,100,100 };
	int w[] = { 48, 30, 42, 36, 36, 48, 42, 42, 36, 24, 30, 30, 42, 36, 36 };
	int v[] = { 10, 30, 25, 50, 35, 30, 25, 40, 30, 35, 45, 10, 20, 30, 25 };
	int items = sizeof(w) / sizeof(int);
	int num_sets = sizeof(c) / sizeof(int);

	// Create variables 
	IloArray<IloBoolVarArray> x(env, items);
	for (int i = 0; i < items; i++) {
		x[i] = IloBoolVarArray(env, num_sets);

		for(int j = 0; j < num_sets; j++)
			x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
	}

	// Objective function
	IloExpr obj(env);
	for (int i = 0; i < items; i++) {
		for (int j = 0; j < num_sets; j++) {
			obj += v[j] * x[i][j];
		}
	}
	model.add(IloMaximize(env, obj));

	// Create constraints
	// Assignment constraints
	for (int i = 0; i < items; i++) {
		model.add(IloSum(x[i]) <= 1);
	}

	// Capacity constraints
	IloExpr cons(env);
	for (int j = 0; j < num_sets; j++) {
		for (int i = 0; i < items; i++) {
			cons += w[i] * x[i][j];
		}
		model.add(cons <= c[j]);
		cons.clear();
	}

	// ExoortModel & solve
	cplex.extract(model);
	cplex.exportModel("multiple_knapsack.lp");

	if (cplex.solve()) {
		double objValue = cplex.getObjValue();
		std::cout << "objective value = " << objValue << std::endl;

		for (int i = 0; i < items; i++) {
			for (int j = 0; j < num_sets; j++) {
				std::cout << x[i][j].getName() << " = " << cplex.getValue(x[i][j]) << std::endl;
			}
		}
	}

}