#include "knapsack.h"

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

void knapsack::Build() {
	
	IloEnv env;
	IloModel model(env);
	IloCplex cplex(env);

	int n = 3;
	int W = 9;
	IloNumArray p(env, n, 1, 2, 3);
	IloNumArray w(env, n, 4, 5, 1);

	// Create variables 
	IloBoolVarArray x(env, n);
	for (int i = 0; i < n; i++) {
		x[i].setName(("x(" + std::to_string(i) + ")").c_str());
	}

	// Objective function
	model.add(IloMaximize(env, IloScalProd(p, x)));

	// Create constraints
	IloExpr cons(env);
	cons = IloScalProd(w, x);
	model.add(cons <= W);
	cons.clear();
	cons.end();

	// ExoortModel & solve
	cplex.extract(model);
	cplex.exportModel("knapsack.lp");

	if (cplex.solve()) {
		double objValue = cplex.getObjValue();
		std::cout << "objective value = " << objValue << std::endl;

		for (int i = 0; i < n; i++) {
			std::cout << x[i].getName() << " = " << cplex.getValue(x[i]) << std::endl;
		}
	}

}