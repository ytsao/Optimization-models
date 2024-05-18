#include <bits/stdc++.h>
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include "scip_exception.hpp"

void create_distance_matrix(int num_nodes, std::vector<std::vector<int>>* distance_matrix);
void print_distance_matrix(int num_nodes, const std::vector<std::vector<int>> distance_matrix);

int main(int args, char** argv){
    std::cout << "Start" << '\n';
    
    if( args < 2 ){
        std::cerr << "call" << argv[0] << " <number of parameters for maximum flow problem" << std::endl;
        exit(1);
    }

    // use current time as seed for random generator
    std::srand(std::time(0));

    // data & parameters for maximum flow problems
    int num_nodes = std::atoi(argv[1]);                                                                                          /**< the number of nodes in the graph */
    std::vector<std::vector<int>> distance_matrix(num_nodes, std::vector<int>(num_nodes));                      /**< the distance between all of nodes in the graph */
    create_distance_matrix(num_nodes, & distance_matrix);
    print_distance_matrix(num_nodes, distance_matrix);

    try{
        // initialize SCIP object, load default plugins link separators, heuristics, etc.
        SCIP* scip;
        SCIP_CALL_EXC( SCIPcreate(& scip) );
        SCIP_CALL_EXC( SCIPincludeDefaultPlugins(scip) );

        // create problem
        SCIP_CALL_EXC( SCIPcreateProb(scip, "Max Flow", NULL, NULL,
                                        NULL, NULL, NULL, NULL, NULL) );
        SCIP_CALL_EXC( SCIPsetObjsense(scip, SCIP_OBJSENSE_MAXIMIZE) );         /**< default sense is "minimize" */

#pragma region create variables 
        
        std::ostringstream namebuf;
        std::vector<std::vector<SCIP_VAR*>> x(num_nodes, std::vector<SCIP_VAR*>(num_nodes));
        for (int i = 0; i < num_nodes; ++i){
            for (int j = 0; j < num_nodes; ++j){
                SCIP_VAR* var_x;
                namebuf.str("");
                namebuf << "x#" << i << "#" << j;

                SCIP_CALL_EXC( SCIPcreateVar(scip, & var_x, namebuf.str().c_str(), 0, INT_MAX, 1, 
                                             SCIP_VARTYPE_CONTINUOUS, TRUE, FALSE, 
                                             NULL, NULL ,NULL, NULL, NULL) );
                SCIP_CALL_EXC( SCIPaddVar(scip, var_x) );
                x[i][j] = var_x;
            }
        }

#pragma endregion
        
#pragma region create constraints   
        std::vector<SCIP_CONS*> all_constraints;

        // flow balance constraints
        for (int k = 1; k < num_nodes - 1; ++k){
            SCIP_CONS* cons;
            namebuf.str("");
            namebuf << "flow_balance_" << k;

            SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & cons, namebuf.str().c_str(), 0, NULL, NULL, 0, 0, 
                                                TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE) );
            
            for (int i = 0; i < num_nodes; ++i){
                if (distance_matrix[i][k])
                    SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons, x[i][k], 1) );
            }
            for (int j = 0; j < num_nodes; ++j){
                if (distance_matrix[k][j])
                    SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons, x[k][j], -1) );
            }

            SCIP_CALL_EXC( SCIPaddCons(scip, cons) );
            all_constraints.push_back(cons);
        }

        // capacity constraints
        for (int i = 0; i < num_nodes; ++i){
            for (int j = 0; j < num_nodes; ++j){
                SCIP_CONS* cons;
                namebuf.str("");
                namebuf << "capa_cons_" << i << "_" << j;

                SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & cons, namebuf.str().c_str(), 0, NULL, NULL, 0, distance_matrix[i][j],
                                                    TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE) );
                SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons, x[i][j], 1) );
                SCIP_CALL_EXC( SCIPaddCons(scip, cons) );
                all_constraints.push_back(cons);
            }
        }
        
#pragma endregion

        // export lp file
        SCIP_CALL_EXC( SCIPwriteOrigProblem(scip, "MaxFlow.lp", NULL, FALSE) );

        // solve the problem
        SCIP_CALL_EXC( SCIPsolve(scip) );

#pragma region get solution status, each variable solution

    SCIP_STATUS sln_status = SCIPgetStatus(scip);
    SCIP_SOL* solution = SCIPgetBestSol(scip);

    if (solution == NULL){
        std::cout << "no solution found" << "\n";
    }
    else{
        double objective_value = SCIPgetSolOrigObj(scip, solution);
        std::cout << "Objective value = " << objective_value << "\n";
        
        for (int i = 0; i < num_nodes; ++i){
            for (int j = 0; j < num_nodes; ++j){
                if (SCIPgetSolVal(scip, solution, x[i][j]) != 0)
                    std::cout << SCIPvarGetName(x[i][j]) << " = " << SCIPgetSolVal(scip, solution, x[i][j]) << "\n";
            }
        }
    }

#pragma endregion

#pragma region  free the SCIP environment, and close it.
        for (int i = 0; i < num_nodes; ++i){
            for (int j = 0; j < num_nodes; ++j){
                SCIP_CALL_EXC( SCIPreleaseVar(scip, & x[i][j]) );
            }
        }
        x.clear();
        
        for (int i = 0; i < all_constraints.size(); ++i){
            SCIP_CALL_EXC( SCIPreleaseCons(scip, & all_constraints[i]) );
        }
        all_constraints.clear();

        SCIP_CALL_EXC( SCIPfree(& scip) );
#pragma endregion

        std::cout << "Finish!" << "\n";
    }
    catch ( SCIPException & exec ){
        std::cerr << exec.what() << '\n';
		exit(exec.getRetcode());
    }
    catch ( ... ){
        std::abort();
    }

    return 0;
}

void
create_distance_matrix  (int num_nodes, std::vector<std::vector<int>>* distance_matrix){
    for (int i = 0; i < num_nodes - 1; ++i){
        for (int j = i + 1; j < num_nodes; ++j){
            int distance = rand() % (40 - 5 + 1) + 1;
            distance_matrix->at(i)[j] = distance; 
            distance_matrix->at(j)[i] = distance;
        }
    }
    
    /*distance_matrix->at(0)[1] = 20;
    distance_matrix->at(0)[2] = 30;
    distance_matrix->at(0)[3] = 10;
    distance_matrix->at(1)[2] = 40;
    distance_matrix->at(1)[4] = 30;
    distance_matrix->at(2)[3] = 10;
    distance_matrix->at(2)[4] = 20;
    distance_matrix->at(3)[2] = 5;
    distance_matrix->at(3)[4] = 20;*/
}

void print_distance_matrix (int num_nodes,const std::vector<std::vector<int>> distance_matrix){
    for (int i = 0; i < num_nodes; ++i){
        for (int j = 0; j < num_nodes; ++j){
            std::cout << distance_matrix[i][j] << " | ";
        }
        std::cout << "\n";
    }
}