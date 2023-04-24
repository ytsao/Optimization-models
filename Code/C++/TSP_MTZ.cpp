#include "TSP_MTZ.h"

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

void TSP_MTZ::Build() {

	IloEnv env;
	IloModel model(env);
	IloCplex cplex(env);

	int n = 17;
	int distance_matrix[17][17] =
	{
				{ 9999, 3, 5, 48, 48, 8, 8, 5, 5, 3, 3, 0, 3, 5, 8, 8, 5},
				{ 3, 9999, 3, 48, 48, 8, 8, 5, 5, 0, 0, 3, 0, 3, 8, 8, 5 },
				{ 5, 3, 9999, 72, 72, 48, 48, 24, 24, 3, 3, 5, 3, 0, 48 ,48, 24},
				{ 48, 48 ,74, 9999, 0, 6, 6, 12, 12, 48, 48, 48,48, 74, 6, 6, 12},
				{ 48, 48, 74, 0, 9999, 6, 6, 12, 12, 48, 48, 48, 48, 74, 6, 6, 12 },
				{ 8, 8, 50, 6, 6, 9999, 0, 8, 8, 8, 8, 8, 8, 50, 0, 0, 8},
				{ 8, 8, 50, 6, 6, 0, 9999, 8, 8, 8, 8, 8, 8, 50, 0, 0, 8},
				{ 5, 5, 26, 12, 12, 8, 8, 9999, 0, 5, 5, 5, 5, 26, 8, 8, 0},
				{ 5, 5, 26, 12, 12, 8, 8, 0, 9999, 5, 5, 5, 5, 26, 8, 8, 0},
				{ 3, 0, 3, 48, 48, 8, 8, 5, 5, 9999, 0, 3, 0, 3, 8, 8, 5},
				{ 3, 0, 3, 48, 48, 8, 8, 5, 5, 0, 9999, 3, 0, 3, 8, 8, 5},
				{ 0, 3, 5, 48, 48, 8, 8, 5, 5, 3, 3, 9999, 3, 5, 8, 8, 5},
				{ 3, 0, 3, 48, 48, 8, 8, 5, 5, 0, 0, 3, 9999, 3, 8, 8, 5},
				{ 5, 3, 0, 72, 72, 48, 48, 24, 24, 3, 3, 5, 3, 9999, 48, 48, 24},
				{ 8, 8, 50, 6, 6, 0, 0, 8, 8, 8, 8, 8, 8, 50, 9999, 0, 8},
				{ 8, 8, 50, 6, 6, 0, 0, 8, 8, 8, 8, 8, 8, 50, 0, 9999, 8},
				{ 5, 5, 26, 12, 12, 8, 8, 0, 0, 5, 5, 5, 5, 26, 8, 8, 9999}
	};

	// Create variables
	IloArray<IloBoolVarArray> x(env, n);
	for (int i = 0; i < n; i++) {
		x[i] = IloBoolVarArray(env, n);

		for (int j = 0; j < n; j++) {
			x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
		}
	}

	IloNumVarArray u(env, n, 0, n - 1, ILOFLOAT);
	for (int i = 0; i < n; i++)
		u[i].setName(("u(" + std::to_string(i) + ")").c_str());

	// Create objective function
	IloExpr obj(env);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			obj += distance_matrix[i][j] * x[i][j];
		}
	}
	model.add(IloMinimize(env, obj));
	obj.clear();
	obj.end();

	// Assignment constraints 1
	for (int i = 0; i < n; i++)
		model.add(IloSum(x[i]) == 1);

	// Assignment constraint 2
	IloExpr cons(env);
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++) {
			cons += x[i][j];
		}
		model.add(cons == 1);
		cons.clear();
	}

	// Subtour Elimination constraints - MTZ
	for (int i = 0; i < n; i++) {
		for (int j = 1; j < n; j++) {
			if (i != j) {
				cons += u[i];
				cons -= u[j];
				cons += n * x[i][j];
				model.add(cons <= n - 1);
				cons.clear();
			}
		}
	}

	// ExoortModel & solve
	cplex.extract(model);
	cplex.exportModel("TSP_MTZ.lp");

	if (cplex.solve()) {
		double objValue = cplex.getObjValue();
		std::cout << "objective value = " << objValue << std::endl;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cout << x[i][j].getName() << " = " << cplex.getValue(x[i][j]) << std::endl;
			}
		}

		for (int i = 0; i < n; i++) {
			std::cout << u[i].getName() << " = " << cplex.getValue(u[i]) << std::endl;
		}
	}

}