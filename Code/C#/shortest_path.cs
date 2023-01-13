using ILOG.Concert;
using ILOG.CPLEX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Mime;
using System.Text;

namespace optimization_model
{
    public class shortest_path
    {
        public static void Build()
        {
            // Testing data
            int num_nodes = 5;

            double[,] distance_matrix = new double[num_nodes, num_nodes];
            distance_matrix[0, 1] = 5;
            distance_matrix[0, 2] = 4;
            distance_matrix[0, 3] = 7;
            distance_matrix[2, 3] = 2;
            distance_matrix[1, 3] = 3;
            distance_matrix[1, 4] = 7;
            distance_matrix[3, 4] = 2;

            // Create data sets and decision variables
            Cplex model = new Cplex();
            INumVar[,] x = new INumVar[num_nodes, num_nodes];
            for (int i = 0; i < num_nodes; ++i)
            {
                for (int j = 0; j < num_nodes; ++j) {
                    x[i, j] = model.NumVar(0, double.MaxValue, NumVarType.Float, $"x({i},{j})");
                }
            }

            // Create objective function
            ILinearNumExpr objective_function = model.LinearNumExpr();
            for(int i = 0; i < num_nodes; ++i)
            {
                for(int j = 0; j < num_nodes; ++j)
                {
                    objective_function.AddTerm(distance_matrix[i, j], x[i, j]);
                }
            }
            model.AddMinimize(objective_function);

            // Create all of constraints
            // flow balance constraints
            ILinearNumExpr flow_balance = model.LinearNumExpr();
            for (int i = 0; i < num_nodes; ++i)
            {
                for (int j = 0; j < num_nodes; ++j)
                {
                    if (distance_matrix[j, i] != 0)
                        flow_balance.AddTerm(1, x[j, i]);
                    if (distance_matrix[i, j] != 0)
                        flow_balance.AddTerm(-1, x[i, j]);
                }
                if (i == 0)
                {
                    model.AddEq(flow_balance, -1, "FlowBalance1");
                }
                else if (i == num_nodes - 1)
                {
                    model.AddEq(flow_balance, 1, "FlowBalance3");
                }
                else // i != num_nodes - 1 && i != 0
                {
                    model.AddEq(flow_balance, 0, "FlowBalance2");
                }

                // reset
                flow_balance.Clear();
            }

            // Export lp file & Solve 
            model.ExportModel("shortest_path.lp");
            if (model.Solve())
            {
                // Print results
                Console.WriteLine($"Solution status: {model.GetStatus()}");
                Console.WriteLine($"Objective value = {model.GetObjValue()}");
                
                for (int i = 0; i < num_nodes; ++i) 
                {
                    for(int j = 0; j < num_nodes; ++j)
                    {
                        double value = model.GetValue(x[i, j]);
                        if (value != 0)
                        {
                            Console.WriteLine($"{x[i,j].Name} = {value}");
                        }
                    }
                }
            }

            Console.WriteLine("Finish");
        }
    }
}
