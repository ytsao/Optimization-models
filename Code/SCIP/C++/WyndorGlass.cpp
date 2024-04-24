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
	try
	{
		// Initializing the SCIP environment
		SCIP* scip;
		SCIP_CALL_EXC( SCIPcreate(& scip) );

   		// Load default plugins link separators, heuristics, etc.
   		SCIP_CALL_EXC( SCIPincludeDefaultPlugins(scip) );

		// Don't show the optimization process in terminal
		SCIPmessagehdlrSetQuiet(SCIPgetMessagehdlr(scip), TRUE);

		// Creating a problem
		SCIP_CALL_EXC( SCIPcreateProb(scip, "WyndorGlass", NULL, NULL,
									  NULL, NULL, NULL, NULL, NULL) );
		SCIP_CALL_EXC( SCIPsetObjsense(scip, SCIP_OBJSENSE_MAXIMIZE) );

		// Creating variables
		SCIP_VAR* var1;
		SCIP_CALL_EXC( SCIPcreateVar(scip, & var1, "x1", 0.0, INT_MAX, 3,
									 SCIP_VARTYPE_CONTINUOUS, TRUE, FALSE,
									 NULL, NULL, NULL, NULL, NULL) );
		SCIP_CALL_EXC( SCIPaddVar(scip, var1) );

		SCIP_VAR* var2;
		SCIP_CALL_EXC( SCIPcreateVar(scip, & var2, "x2", 0.0, INT_MAX, 5,
									SCIP_VARTYPE_CONTINUOUS, TRUE, FALSE,
									NULL, NULL, NULL, NULL, NULL) );
		SCIP_CALL_EXC( SCIPaddVar(scip, var2) );

		// Creating constraints
		SCIP_CONS* cons1;
		SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & cons1, "con1",
											0, NULL, NULL, 0, 4.0, TRUE,
											TRUE, TRUE, TRUE, TRUE, FALSE,
											FALSE, FALSE, FALSE, FALSE) );
		SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons1, var1, 1.0) );
		SCIP_CALL_EXC( SCIPaddCons(scip, cons1) );

		SCIP_CONS* cons2;
		SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & cons2, "con2",
											0, NULL, NULL, 0, 12.0, TRUE,
											TRUE, TRUE, TRUE, TRUE, FALSE,
											FALSE, FALSE, FALSE, FALSE) );
		SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons2, var2, 2.0) );
		SCIP_CALL_EXC( SCIPaddCons(scip, cons2) );

		SCIP_CONS* cons3;
		SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & cons3, "con3",
											0, NULL, NULL, 0, 18.0, TRUE,
											TRUE, TRUE, TRUE, TRUE, FALSE,
											FALSE, FALSE, FALSE, FALSE) );
		SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons3, var1, 3.0) );
		SCIP_CALL_EXC( SCIPaddCoefLinear(scip, cons3, var2, 2.0) );
		SCIP_CALL_EXC( SCIPaddCons(scip, cons3) );

		// Export lp file
		SCIP_CALL_EXC( SCIPwriteOrigProblem(scip, "WyndorGlass.lp", NULL, FALSE) );

		// Solving the problem
		SCIP_CALL_EXC( SCIPsolve(scip) );
		
		// Accessing results
		SCIP_STATUS sln_status = SCIPgetStatus(scip);
		if (sln_status == SCIP_STATUS_OPTIMAL)
		{
			SCIP_SOL* sol = SCIPgetBestSol(scip);
			double objVal = SCIPgetSolOrigObj(scip, sol);
			double var1_sln = SCIPgetVarSol(scip, var1);
			double var2_sln = SCIPgetVarSol(scip, var2);
			std::cout << "Solution status: " << sln_status << '\n';
			std::cout << "Objective value = " << objVal << '\n';
			std::cout << "x1 = " << var1_sln << '\n';
			std::cout << "x2 = " << var2_sln << '\n';	
		}

		// Freeing the SCIP environment, and close the SCIP environment
		SCIP_CALL( SCIPreleaseVar(scip, & var1) );
		SCIP_CALL( SCIPreleaseVar(scip, & var2) );
		SCIP_CALL( SCIPreleaseCons(scip, & cons1) );
		SCIP_CALL( SCIPreleaseCons(scip, & cons2) );
		SCIP_CALL( SCIPreleaseCons(scip, & cons3) );
		SCIP_CALL( SCIPfree(& scip) );
	} 
	catch( SCIPException & exec )
	{
		std::cerr << exec.what() << '\n';
		exit(exec.getRetcode());
	} 
	catch( ... )
	{
		std::abort();
	}
	
	return 0;
}
