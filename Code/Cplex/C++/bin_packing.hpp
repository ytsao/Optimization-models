#ifndef __BIN_PACKING__
#define __BIN_PACKING__

#include <vector>
#include <string>
#include <fstream>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

class BinPacking {
private: 
    int capacity = 0;
    int num_items = 0;
    std::vector<int> weights;

    void read_bin_packing_instance(const std::string filename) {
        std::ifstream f;

        f.open(filename);
        if (!f.is_open()) {
            std::cout << "failed to open file." << std::endl;
            return;
        }

        int current_line = 1;
        std::string s;
        while (std::getline(f, s)) {
            if (current_line == 1) num_items = std::stoi(s);
            else if (current_line == 2) capacity = std::stoi(s);
            else weights.push_back(std::stoi(s));
            current_line++;
        }
        f.close();

        return;
    }
public:
	void build() {
        /*int B = 100;
        int items = 11;
        int num_sets = 11;
        std::vector<int> s(items, 0);
        s[0] = 48;
        s[1] = 30;
        s[2] = 19;
        s[3] = 36;
        s[4] = 36;
        s[5] = 27;
        s[6] = 42;
        s[7] = 42;
        s[8] = 36;
        s[9] = 24;
        s[10] = 30;*/

        read_bin_packing_instance(".\\instances\\bin_packing\\BPP_100_100_0.1_0.7_0.txt");

        IloEnv env;
        IloModel model(env);
        IloCplex cplex(env);

        // Create variables
        IloBoolVarArray y(env, num_items);
        for (int i = 0; i < num_items; i++) {
            y[i].setName(("y(" + std::to_string(i) + ")").c_str());
        }

        IloArray<IloNumVarArray> x(env, num_items);
        for (int i = 0; i < num_items; i++) {
            x[i] = IloNumVarArray(env, num_items, 0, IloInfinity, ILOFLOAT);

            for (int j = 0; j < num_items; j++)
                x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
        }


        // Objective function
        IloExpr objective_function(env);
        for (int i = 0; i < num_items; i++) {
            objective_function += y[i];
        }
        model.add(IloMinimize(env, objective_function));
        objective_function.clear();
        objective_function.end();

        // Create constraints
        // Capacity constraints
        IloExpr cons(env);
        for (int j = 0; j < num_items; j++) {
            for (int i = 0; i < num_items; i++) {
                cons += weights[i] * x[i][j];
            }

            model.add(cons <= capacity * y[j]);
            cons.clear();
        }

        // Assigment constraints
        for (int i = 0; i < num_items; i++) {
            cons = IloSum(x[i]);
            model.add(cons == 1);
            cons.clear();
        }

        // ExportModel & solve
        cplex.extract(model);
        cplex.exportModel("bin_packing.lp");

        if (cplex.solve()) {
            double objValue = cplex.getObjValue();
            std::cout << "objective value = " << objValue << std::endl;

            for (int i = 0; i < num_items; i++) {
                for (int j = 0; j < num_items; j++) {
                    if (cplex.getValue(x[i][j]) != 0)
                        std::cout << x[i][j].getName() << " = " << cplex.getValue(x[i][j]) << std::endl;
                }
            }

            for (int i = 0; i < num_items; i++) {
                if (cplex.getValue(y[i]) != 0)
                    std::cout << y[i].getName() << " = " << cplex.getValue(y[i]) << std::endl;
            }
        }
	}
};


#endif
