using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;

namespace optimization_model
{
    internal class multiple_knapsack
    {
        public static void Build()
        {
            int[] c = new int[] { 100, 100, 100, 100, 100 };
            int[] w = new int[] { 48, 30, 42, 36, 36, 48, 42, 42, 36, 24, 30, 30, 42, 36, 36 };
            int[] v = new int[] { 10, 30, 25, 50, 35, 30, 25, 40, 30, 35, 45, 10, 20, 30, 25 };
            int items = w.Length;
            int num_sets = c.Length;

            Cplex model = new Cplex();

            // Create variables 
            INumVar[][] x = new INumVar[items][];
            for (int i = 0; i < items; i++)
            {
                x[i] = new INumVar[num_sets];
                for(int j = 0; j < num_sets; j++)
                {
                    x[i][j] = model.NumVar(0, 1, NumVarType.Bool, $"x({i},{j})");
                }
            }

            // Create objective function
            INumExpr expr = model.NumExpr();
            for(int i = 0; i < items; i++)
            {
                expr = model.Sum(expr, model.ScalProd(v, x[i]));
            }
            model.AddMaximize(expr);

            // Create constraints
            // Assigment constraints
            for(int i = 0; i < items; i++)
            {
                model.AddLe(model.Sum(x[i]), 1);
            }

            // Capacity constraints
            for(int j = 0; j < num_sets; j++)
            {
                expr = model.NumExpr();
                for(int i = 0; i < items; i++)
                {
                    expr = model.Sum(expr, model.Prod(w[i], x[i][j]));
                }

                model.AddLe(expr, c[j]);
            }

            // Export lp file & solve it
            model.ExportModel("Multiple Knapsack.lp");
            if (model.Solve())
            {
                Console.WriteLine($"Objective Value = {model.GetObjValue()}");

                for(int i = 0; i < items; i++)
                {
                    for(int j = 0; j < num_sets; j++)
                    {
                        Console.WriteLine($"{x[i][j].Name} = {model.GetValue(x[i][j])}");
                    }
                }
            }
        }
    }
}
