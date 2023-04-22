#include "shortest_path.h"

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

void shortest_path::Build() {
	IloEnv env;
	IloModel model(env);
	IloCplex cplex(env);

	int num_nodes = 5;

	double distance_matrix[5][5] = { 0 };
	distance_matrix[0][1] = 5;
	distance_matrix[0][2] = 4;
	distance_matrix[0][3] = 7;
	distance_matrix[2][3] = 2;
	distance_matrix[1][3] = 3;
	distance_matrix[1][4] = 7;
	distance_matrix[3][4] = 2;

	// Create decision variables
	IloArray<IloNumVarArray> x(env, num_nodes);
	for (int i = 0; i < num_nodes; i++) {
		x[i] = IloNumVarArray(env, num_nodes, 0, IloInfinity, ILOFLOAT);

		for (int j = 0; j < num_nodes; j++)
			x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
	}

	// Create objective function
	IloExpr obj(env);
	for (int i = 0; i < num_nodes; ++i) {
		for (int j = 0; j < num_nodes; ++j) {
			obj += distance_matrix[i][j] * x[i][j];
		}
	}
	model.add(IloMinimize(env, obj));

	// Create all of constraints
	IloExpr cons(env);
	for (int i = 0; i < num_nodes; ++i) {
		for (int j = 0; j < num_nodes; ++j) {
			if (distance_matrix[j][i] != 0) cons += x[j][i];
			if (distance_matrix[i][j] != 0) cons -= x[i][j];
		}

		if (i == 0) {
			model.add(cons == -1);
		}
		else if (i == num_nodes - 1) {
			model.add(cons == 1);
		}
		else {
			model.add(cons == 0);
		}

		// reset 
		cons.clear();
	}

	// ExoortModel & solve
	cplex.extract(model);
	cplex.exportModel("shortest_path.lp");

	if (cplex.solve()) {
		double objValue = cplex.getObjValue();
		std::cout << "objective value = " << objValue << std::endl;

		for (int i = 0; i < num_nodes; i++) {
			for (int j = 0; j < num_nodes; j++) {
				std::cout << x[i][j].getName() << " = " << cplex.getValue(x[i][j]) << std::endl;
			}
		}
	}
}