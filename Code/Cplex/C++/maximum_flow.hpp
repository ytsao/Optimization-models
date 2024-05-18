#ifndef __MAX_FLOW__
#define __MAX_FLOW__

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

class MaxFlow {
public:
	void build() {
		IloEnv env;
		IloModel model(env);
		IloCplex cplex(env);

		int num_nodes = 5;

		double distance_matrix[5][5] = { 0 };
		distance_matrix[0][1] = 20;
		distance_matrix[0][2] = 30;
		distance_matrix[0][3] = 10;
		distance_matrix[1][2] = 40;
		distance_matrix[1][4] = 30;
		distance_matrix[2][3] = 10;
		distance_matrix[2][4] = 20;
		distance_matrix[3][2] = 5;
		distance_matrix[3][4] = 20;

		// Create decision variables
		IloArray<IloNumVarArray> x(env, num_nodes);
		for (int i = 0; i < num_nodes; i++) {
			x[i] = IloNumVarArray(env, num_nodes, 0, IloInfinity, ILOFLOAT);

			for (int j = 0; j < num_nodes; j++) {
				x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
				x[i][j].setUB(distance_matrix[i][j]);
			}
		}

		// Create objective function
		IloExpr obj(env);
		for (int i = 0; i < num_nodes; ++i) {
			obj += IloSum(x[i]);
		}
		model.add(IloMaximize(env, obj));

		// Create all of constraints
		IloExpr cons(env);
		for (int i = 0; i < num_nodes; ++i) {
			if (i != num_nodes - 1 && i != 0) {
				for (int j = 0; j < num_nodes; ++j) {
					if (distance_matrix[j][i]) cons += x[j][i];
					if (distance_matrix[i][j]) cons -= x[i][j];
				}


				model.add(cons == 0);

				// reset 
				cons.clear();
			}
		}

		// ExoortModel & solve
		cplex.extract(model);
		cplex.exportModel("maximum_flow.lp");

		if (cplex.solve()) {
			double objValue = cplex.getObjValue();
			std::cout << "objective value = " << objValue << std::endl;

			for (int i = 0; i < num_nodes; i++) {
				for (int j = 0; j < num_nodes; j++) {
					if (cplex.getValue(x[i][j]) != 0)
						std::cout << x[i][j].getName() << " = " << cplex.getValue(x[i][j]) << std::endl;
				}
			}
		}
	}
};

#endif
