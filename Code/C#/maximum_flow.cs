using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;

namespace optimization_model
{
    internal class maximum_flow
    {
        public static void Build()
        {
            List<int> n = Enumerable.Range(0, 5).ToList();
            int last_n = n[n.Count - 1];
            int[][] d = new int[n.Count][];
            for(int i = 0; i < n.Count; i++)
            {
                d[i] = new int[n.Count];
                for (int j = 0; j < n.Count; j++)
                {
                    d[i][j] = 0;
                }
            }
            d[0][1] = 20;
            d[0][2] = 30;
            d[0][3] = 10;
            d[1][2] = 40;
            d[1][4] = 30;
            d[2][3] = 10;
            d[2][4] = 20;
            d[3][2] = 5;
            d[3][4] = 20;

            Cplex model = new Cplex();

            // Create variables
            INumVar[][] x = new INumVar[n.Count][];
            for (int i = 0; i < n.Count; i++)
            {
                x[i] = new INumVar[n.Count];
                for (int j = 0; j < n.Count; j++)
                {
                    x[i][j] = model.NumVar(0, d[i][j], NumVarType.Float, $"x({i}, {j})");
                }
            }

            // Create object function
            INumExpr expr = model.NumExpr();
            for (int i = 0; i < n.Count; i++)
            {
                expr = model.Sum(expr, model.Sum(x[i]));
            }
            model.AddMaximize(expr);

            // Export lp file & solve it
            model.ExportModel("maximum_flow.lp");
            if (model.Solve())
            {
                Console.WriteLine($"Objective Value = {model.GetObjValue()}");
                for (int i = 0; i < n.Count; i++)
                {
                    for (int j = 0; j < n.Count; j++)
                    {
                        Console.WriteLine($"{x[i][j].Name} = {model.GetValue(x[i][j])}");
                    }
                }
            }
        }
    }
}
