#include "BinPacking.h"

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

void BinPacking::Build() {

	int B = 100;
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
    s[10] = 30;

	IloEnv env;
	IloModel model(env);
	IloCplex cplex(env);

    // Create variables
    IloBoolVarArray y(env, num_sets);
    for (int i = 0; i < num_sets; i++) {
        y[i].setName(("y(" + std::to_string(i) + ")").c_str());
    }

    IloArray<IloNumVarArray> x(env, items);
    for (int i = 0; i < items; i++) {
        x[i] = IloNumVarArray(env, num_sets, 0, IloInfinity, ILOFLOAT);

        for(int j = 0; j < num_sets; j++)
            x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
    }
    

    // Objective function
    IloExpr objective_function(env);
    for (int i = 0; i < num_sets; i++) {
        objective_function += y[i];
    }
    model.add(IloMinimize(env, objective_function));
    objective_function.clear();
    objective_function.end();

    // Create constraints
    // Capacity constraints
    IloExpr cons(env);
    for (int j = 0; j < num_sets; j++) {
        for (int i = 0; i < items; i++) {
            cons += s[i] * x[i][j];
        }

        model.add(cons <= B * y[j]);
        cons.clear();
    }

    // Assigment constraints
    for (int i = 0; i < items; i++) {
        for (int j = 0; j < num_sets; j++) {
            cons += x[i][j];
        }
        model.add(cons == 1);
        cons.clear();
    }

    // ExportModel & solve
    cplex.extract(model);
    cplex.exportModel("bin_packing.lp");

    if (cplex.solve()) {
        double objValue = cplex.getObjValue();
        std::cout << "objective value = " << objValue << std::endl;

        for (int i = 0; i < items; i++) {
            for (int j = 0; j < num_sets; j++) {
                std::cout << x[i][j].getName() <<  " = " << cplex.getValue(x[i][j]) << std::endl;
            }
        }

        for (int i = 0; i < num_sets; i++) {
            std::cout << y[i].getName() << " = " << cplex.getValue(y[i]) << std::endl;
        }
    }
}
