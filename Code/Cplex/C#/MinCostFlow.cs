using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;


namespace optimization_model
{
    internal class MinCostFlow
    {
        public static void Build()
        {
            List<int> n = Enumerable.Range(0, 5).ToList();
            int last_n = n[n.Count - 1];
            int[][] e = new int[n.Count][];
            for(int i = 0; i < n.Count; i++)
            {
                e[i] = new int[n.Count];
                for(int j = 0; j < n.Count; j++)
                {
                    e[i][j] = 0;
                }
            }
            e[0][1] = 1;
            e[0][2] = 4;
            e[1][2] = 2;
            e[1][3] = 2;
            e[1][4] = 6;
            e[2][3] = 1;
            e[2][4] = 3;
            e[3][4] = 2;
            e[4][2] = 3;

            int[][] c = new int[n.Count][];
            for(int i = 0; i < n.Count; i++)
            {
                c[i] = new int[n.Count];
                for(int j = 0; j < n.Count; j++)
                {
                    c[i][j] = 0;
                }
            }
            c[0][1] = 4;
            c[0][2] = 4;
            c[1][2] = 2;
            c[1][3] = 2;
            c[1][4] = 6;
            c[2][3] = 1;
            c[2][4] = 3;
            c[3][4] = 2;
            c[4][2] = 3;

            int[] d = new int[n.Count]; ;
            d[0] = -20;
            d[1] = 0;
            d[2] = 0;
            d[3] = 5;
            d[4] = 15;

            int[][] f = new int[n.Count][];
            for(int i = 0; i < n.Count; i++)
            {
                f[i] = new int[n.Count];
                for(int j = 0; j < n.Count; j++)
                {
                    f[i][j] = 0;
                }
            }
            f[0][1] = 15;
            f[0][2] = 8;
            f[1][2] = 20;
            f[1][3] = 4;
            f[1][4] = 10;
            f[2][3] = 15;
            f[2][4] = 4;
            f[3][4] = 20;
            f[4][2] = 5;

            Cplex model = new Cplex();

            // Create variables
            INumVar[][] x = new INumVar[n.Count][];
            for(int i = 0; i < n.Count; i++)
            {
                x[i] = new INumVar[n.Count];
                for(int j = 0; j < n.Count; j++)
                {
                    x[i][j] = model.NumVar(0, f[i][j], NumVarType.Float, $"x({i},{j})");
                }
            }

            // Objective function
            INumExpr expr = model.NumExpr();
            for(int i = 0; i < n.Count; i++)
            {
                expr = model.Sum(expr, model.ScalProd(c[i], x[i]));
            }
            model.AddMinimize(expr);

            // Create constraints
            // Flow balance constraints
            for(int k = 0; k < n.Count; k++)
            {
                expr = model.NumExpr();

                // Flow in
                for (int i = 0; i < n.Count; i++)
                {
                    if (e[i][k] != 0)
                    {
                        expr = model.Sum(expr, x[i][k]);
                    }
                }

                // Flow out
                for(int j = 0; j < n.Count; j++)
                {
                    if (e[k][j] != 0)
                    {
                        expr = model.Sum(expr, model.Prod(-1, x[k][j]));
                    }
                }

                model.AddEq(expr, d[k]);
            }

            // Export lp file & solve it
            model.ExportModel("MinCostFlow.lp");
            if (model.Solve())
            {
                Console.WriteLine($"Objective Value = {model.GetObjValue()}");
                for(int i = 0; i < n.Count; i++)
                {
                    for(int j = 0; j < n.Count; j++)
                    {
                        Console.WriteLine($"{x[i][j].Name} = {model.GetValue(x[i][j])}");
                    }
                }
            }
        }
    }
}
