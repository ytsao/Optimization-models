#ifndef __MIN_COST_FLOW__
#define __MIN_COST_FLOW__

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

class MinCostFlow {
public:
	void build() {
		IloEnv env;
		IloModel model(env);
		IloCplex cplex(env);

		int num_nodes = 5;

		double edge[5][5] = { 0 };
		edge[0][1] = 1;
		edge[0][2] = 4;
		edge[1][2] = 2;
		edge[1][3] = 2;
		edge[1][4] = 6;
		edge[2][3] = 1;
		edge[2][4] = 3;
		edge[3][4] = 2;
		edge[4][2] = 3;

		double cost[5][5] = { 0 };
		cost[0][1] = 4;
		cost[0][2] = 4;
		cost[1][2] = 2;
		cost[1][3] = 2;
		cost[1][4] = 6;
		cost[2][3] = 1;
		cost[2][4] = 3;
		cost[3][4] = 2;
		cost[4][2] = 3;

		double demand[5] = { -20, 0, 0, 5, 15 };

		double flow[5][5] = { 0 };
		flow[0][1] = 15;
		flow[0][2] = 8;
		flow[1][2] = 20;
		flow[1][3] = 4;
		flow[1][4] = 10;
		flow[2][3] = 15;
		flow[2][4] = 4;
		flow[3][4] = 20;
		flow[4][2] = 5;

		// Create decision variables
		IloArray<IloNumVarArray> x(env, num_nodes);
		for (int i = 0; i < num_nodes; i++) {
			x[i] = IloNumVarArray(env, num_nodes, 0, IloInfinity);

			for (int j = 0; j < num_nodes; j++) {
				x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
				x[i][j].setUB(flow[i][j]);
			}
		}

		// Create objective function
		IloExpr obj(env);
		for (int i = 0; i < num_nodes; ++i) {
			for (int j = 0; j < num_nodes; ++j) {
				obj += cost[i][j] * x[i][j];
			}
		}
		model.add(IloMinimize(env, obj));

		// Create all of constraints
		IloExpr cons(env);
		for (int k = 0; k < num_nodes; k++) {
			// Flow in
			for (int i = 0; i < num_nodes; i++) {
				if (edge[i][k] != 0) {
					cons += x[i][k];
				}
			}

			// Flow out
			for (int j = 0; j < num_nodes; j++) {
				if (edge[k][j] != 0) {
					cons -= x[k][j];
				}
			}

			model.add(cons == demand[k]);
			cons.clear();
		}

		// ExoortModel & solve
		cplex.extract(model);
		cplex.exportModel("MinCostFlow.lp");

		if (cplex.solve()) {
			double objValue = cplex.getObjValue();
			std::cout << "objective value = " << objValue << std::endl;

			// it should work, but there is a bug..
			/*for (int i = 0; i < num_nodes; i++) {
				for (int j = 0; j < num_nodes; j++) {
					if (cplex.getValue(x[i][j]) != 0)
						std::cout << x[i][j].getName() << " = " << cplex.getValue(x[i][j]) << std::endl;
				}
			}*/
		}
	}
};

#endif