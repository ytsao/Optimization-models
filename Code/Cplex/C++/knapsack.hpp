#ifndef __KNAPSACK__
#define __KNAPSACK__

#include <vector>
#include <string>
#include <fstream>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

class Knapsack {
private:
	int capacity;
	int num_items;
	std::vector<int> profit;
	std::vector<int> weights;

	void read_knapsack_instance(const std::string filename) {
		std::ifstream f;

		f.open(filename);
		if (!f.is_open()) {
			std::cout << "failed to open file." << std::endl;
			return;
		}

		int current_line = 1;
		std::string s;
		while (std::getline(f, s)) {
			if (s == "") continue;
			if (current_line == 1) num_items = std::stoi(s);
			else if (current_line == 2) capacity = std::stoi(s);
			else {
				std::istringstream iss(s);
				std::string line;
				bool isFirst = true;
				while (std::getline(iss, line, ' ')) {
					if (isFirst) {
						profit.push_back(std::stoi(line));
						isFirst = false;
					}
					else weights.push_back(std::stoi(line));
				}
			}
			current_line++;
		}
		f.close();

		return;
	}
public:
	void build() {
		IloEnv env;
		IloModel model(env);
		IloCplex cplex(env);

		/*int n = 3;
		int W = 9;
		IloNumArray p(env, n, 1, 2, 3);
		IloNumArray w(env, n, 4, 5, 1);*/
		read_knapsack_instance(".//instances//knapsack//s000.kp");

		// Create variables 
		IloBoolVarArray x(env, num_items);
		for (int i = 0; i < num_items; i++) {
			x[i].setName(("x(" + std::to_string(i) + ")").c_str());
		}

		// Objective function
		IloExpr obj(env);
		for (int i = 0; i < num_items; ++i) obj += profit[i] * x[i];
		model.add(IloMaximize(env, obj));
		obj.clear();
		obj.end();

		// Create constraints
		IloExpr cons(env);
		for (int i = 0; i < num_items; ++i) cons += weights[i] * x[i];
		model.add(cons <= capacity);
		cons.clear();
		cons.end();

		// ExoortModel & solve
		cplex.extract(model);
		cplex.exportModel("knapsack.lp");

		if (cplex.solve()) {
			double objValue = cplex.getObjValue();
			std::cout << "objective value = " << objValue << std::endl;

			for (int i = 0; i < num_items; i++) {
				if (cplex.getValue(x[i]) != 0)
					std::cout << x[i].getName() << " = " << cplex.getValue(x[i]) << std::endl;
			}
		}
	}
};

#endif