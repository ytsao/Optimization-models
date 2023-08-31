#include <bits/stdc++.h>
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include "scip_exception.hpp"

int 
main(
    int args,
    char** argv
    )
{
    std::cout << "Start" << '\n';
    
    if( args < 2 )
    {
        std::cerr << "call" << argv[0] << " <number of parameters for bin packing problem" << std::endl;
        exit(1);
    }

    // use current time as seed for random generator
    std::srand(std::time(0));

    // data & Parameters for Bin Packing Problems
    int max_capacity            = std::atoi(argv[1]);                   /**< maximum capacity for each set */
    int num_sets                = std::atoi(argv[2]);                   /**< number of sets can be used */
    int num_items               = std::atoi(argv[3]);                   /**< number of items needed to be packed */
    int item_size[num_items]    = { 0 };                                /**< the size for each item */
    for( int i = 0; i < num_items; ++i )
    {
        /* generate random number between lb and ub
         * lb = 1, ub = max_capacity
         * std::rand() & (ub - lb + 1)) + 1
         */
        item_size[i] = (std::rand() % (max_capacity - 1 + 1)) + 1;
    }

    try
    {
        // initialize SCIP object, load default plugins link separators, heuristics, etc.
        SCIP* scip;
        SCIP_CALL_EXC( SCIPcreate(& scip) );
        SCIP_CALL_EXC( SCIPincludeDefaultPlugins(scip) );

        // create problem
        SCIP_CALL_EXC( SCIPcreateProb(scip, "Bin Packing", NULL, NULL,
                                        NULL, NULL, NULL, NULL, NULL) );

#pragma region create variables 
        std::vector<std::vector<SCIP_VAR* >> x(num_items, std::vector<SCIP_VAR* >(num_sets));
        std::vector<SCIP_VAR* > y(num_sets);
        std::ostringstream namebuf;
        for( int i = 0; i < num_items; ++i )
        {
            for( int j = 0; j < num_sets; ++j )
            {
                SCIP_VAR* var_x;
                namebuf.str("");
                namebuf << "x#" << i << "#" << j;

                // create teh SCIP_VAR object
                SCIP_CALL_EXC( SCIPcreateVar(scip, & var_x, namebuf.str().c_str(), 0.0, INT_MAX, 0, 
                                            SCIP_VARTYPE_CONTINUOUS, TRUE, FALSE,
                                            NULL, NULL, NULL, NULL, NULL) );

                // add the SCIP_VAR object to the scip problem
                SCIP_CALL_EXC( SCIPaddVar(scip, var_x) );

                // store the SCIP_VAR pointer for later access
                x[i][j] = var_x;
            }
        }

        for( int i = 0; i < num_sets; ++i )
        {
            SCIP_VAR* var_y;
            namebuf.str("");
            namebuf << "y#" << i;

            // create the SCIP_VAR object
            SCIP_CALL_EXC( SCIPcreateVar(scip, & var_y, namebuf.str().c_str(), 0.0, 1, 1,
                                        SCIP_VARTYPE_BINARY, TRUE, FALSE,
                                        NULL, NULL, NULL, NULL, NULL) );
            
            // add SCIP_VAR object to the scip problem
            SCIP_CALL_EXC( SCIPaddVar(scip, var_y) );

            // store the SCIP_VAR pointer for later access
            y[i] = var_y;
        }
#pragma endregion
        
#pragma region create constraints   
        std::vector<SCIP_CONS* > all_constraints;
        /* capacity constraints */
        for( int i = 0; i < num_sets; ++i )
        {
            SCIP_CONS* cons;
            namebuf.str("");
            namebuf << "capacity_con_" << i;

            // create SCIP_CON object
            SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & cons, namebuf.str().c_str(), 0, NULL, NULL, -max_capacity, 0,
                                                TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE) );
            
            // add variables to SCIP_CON object
            for( int j = 0; j < num_items; ++j )
                SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons, x[j][i], item_size[j]) );
            SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons, y[i], -max_capacity) );

            // add SCIP_CON object to SCIP problem
            SCIP_CALL_EXC( SCIPaddCons(scip, cons) );
            all_constraints.push_back(cons);
        }

        /* assignment constraints */
        for( int i = 0; i < num_items; ++i )
        {
            SCIP_CONS* cons;
            namebuf.str("");
            namebuf << "assignment_con_" << i;

            // create SCIP_CON object
            SCIP_CALL_EXC (SCIPcreateConsLinear(scip, & cons, namebuf.str().c_str(), 0, NULL, NULL, 1, 1,
                                                TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE) );
            
            // add variables to SCIP_CON object
            for( int j = 0; j < num_sets; ++j )
                SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons, x[i][j], 1) );
            
            // add SCIP_CON object to SCIP problem
            SCIP_CALL_EXC( SCIPaddCons(scip, cons) );
            all_constraints.push_back(cons);
        }
#pragma endregion

        // export lp file
        SCIP_CALL_EXC( SCIPwriteOrigProblem(scip, "BinPacking.lp", NULL, FALSE) );

        // solve the problem
        SCIP_CALL_EXC( SCIPsolve(scip) );

#pragma region get solution status, each variable solution

    SCIP_STATUS sln_status = SCIPgetStatus(scip);
    SCIP_SOL* solution = SCIPgetBestSol(scip);

    if (solution == NULL)
    {
        std::cout << "no solution found" << "\n";
    }
    else
    {
        double objective_value = SCIPgetSolOrigObj(scip, solution);
        std::cout << "Objective value = " << objective_value << "\n";
        
        for (int i = 0; i < num_items; ++i)
        {
            for (int j = 0; j < num_sets; ++j)
            {
                if (SCIPgetSolVal(scip, solution, x[i][j]) != 0)
                    std::cout << SCIPvarGetName(x[i][j]) << " = " << SCIPgetSolVal(scip, solution, x[i][j]) << "\n";
            }
        }

        for (int i = 0; i < num_sets; ++i)
        {
            if (SCIPgetSolVal(scip, solution, y[i]) != 0)
                std::cout << SCIPvarGetName(y[i]) << " = " << SCIPgetSolVal(scip, solution, y[i]) << "\n";
        }
    }

#pragma endregion

#pragma region  free the SCIP environment, and close it.
        for (int i = 0; i < num_items; ++i)
        {
            for (int j = 0; j < num_sets; ++j)
            {
                SCIP_CALL_EXC( SCIPreleaseVar(scip, & x[i][j]) );
            }
        }
        x.clear();
        for (int i = 0; i < num_sets; ++i)
        {
            SCIP_CALL_EXC( SCIPreleaseVar(scip, & y[i]) );
        }
        y.clear();

        for (int i = 0; i < all_constraints.size(); ++i)
        {
            SCIP_CALL_EXC( SCIPreleaseCons(scip, & all_constraints[i]) );
        }
        all_constraints.clear();

        SCIP_CALL_EXC( SCIPfree(& scip) );
#pragma endregion

        std::cout << "Finish!" << "\n";
    }
    catch ( SCIPException & exec )
    {
        std::cerr << exec.what() << '\n';
		exit(exec.getRetcode());
    }
    catch ( ... )
    {
        std::abort();
    }

    return 0;
}