using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;

namespace optimization_model
{
    public class knapsack
    {
        public static void Build()
        {
            // Testing data
            int num_items = 50;
            int Capacity = 850;
            List<int> profits = new List<int>() { 360, 83, 59, 130, 431, 67,  230, 52,  93,  125, 670, 892, 600, 38,  48,  147, 78,
                  256, 63, 17, 120, 164, 432, 35,  92,  110, 22,  42,  50,  323, 514, 28,  87,  73,
                  78,  15, 26, 78,  210, 36,  85,  189, 274, 43,  33,  10,  19,  389, 276, 312 };
            List<int> weights = new List<int>() { 7,  0,  30, 22, 80, 94, 11, 81, 70, 64, 59, 18, 0,  36, 3,  8,  15,
                      42, 9,  0,  42, 47, 52, 32, 26, 48, 55, 6,  29, 84, 2,  4,  18, 56,
                      7,  29, 93, 44, 71, 3,  86, 66, 31, 65, 0,  79, 20, 65, 52, 13 };
            List<string> var_names = new List<string>();

            // Create data sets and decision variables
            Cplex model = new Cplex();

            for (int i = 0; i < num_items; ++i) var_names.Add($"x{i}");
            INumVar[] x = model.NumVarArray(num_items, 0, 1, NumVarType.Bool, var_names.ToArray());

            // Create objective function
            model.AddMaximize(model.ScalProd(profits.ToArray(), x));

            // Create all of constraints
            // Capacity constraints
            model.AddLe(model.ScalProd(weights.ToArray(), x), Capacity);

            // Export lp file
            model.ExportModel("knapsack.lp");

            // Solve 
            if (model.Solve())
            {
                // Print results
                Console.WriteLine($"Solution status: {model.GetStatus()}");
                Console.WriteLine($"Objective value = {model.GetObjValue()}");
                for (int i = 0; i < num_items; ++i)
                {
                    double value = model.GetValue(x[i]);
                    if (value > 0)
                    {
                        Console.WriteLine($"{x[i].Name} = {value}");
                    }
                }
            }

            Console.WriteLine("Finish");
        }
    }
}
