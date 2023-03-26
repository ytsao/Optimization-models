using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;

namespace optimization_model
{
    internal class TSP_MTZ
    {
        public static void Build()
        {
            int n = 17;
            List<int> cities = Enumerable.Range(0, n).ToList();
            int[,] distance_matrix = new int[,]
            {
                { 9999, 3, 5, 48, 48, 8, 8, 5, 5, 3, 3, 0, 3, 5, 8, 8, 5},
                { 3, 9999, 3, 48, 48, 8, 8, 5, 5, 0, 0, 3, 0, 3, 8, 8, 5 },
                { 5, 3, 9999, 72, 72, 48, 48, 24, 24, 3, 3, 5, 3, 0, 48 ,48, 24},
                { 48, 48 ,74, 9999, 0, 6, 6, 12, 12, 48, 48, 48,48, 74, 6, 6, 12},
                { 48, 48, 74, 0, 9999, 6, 6, 12, 12, 48, 48, 48, 48, 74, 6, 6, 12 },
                { 8, 8, 50, 6, 6, 9999, 0, 8, 8, 8, 8, 8, 8, 50, 0, 0, 8},
                { 8, 8, 50, 6, 6, 0, 9999, 8, 8, 8, 8, 8, 8, 50, 0, 0, 8},
                { 5, 5, 26, 12, 12, 8, 8, 9999, 0, 5, 5, 5, 5, 26, 8, 8, 0},
                { 5, 5, 26, 12, 12, 8, 8, 0, 9999, 5, 5, 5, 5, 26, 8, 8, 0},
                { 3, 0, 3, 48, 48, 8, 8, 5, 5, 9999, 0, 3, 0, 3, 8, 8, 5},
                { 3, 0, 3, 48, 48, 8, 8, 5, 5, 0, 9999, 3, 0, 3, 8, 8, 5},
                { 0, 3, 5, 48, 48, 8, 8, 5, 5, 3, 3, 9999, 3, 5, 8, 8, 5},
                { 3, 0, 3, 48, 48, 8, 8, 5, 5, 0, 0, 3, 9999, 3, 8, 8, 5},
                { 5, 3, 0, 72, 72, 48, 48, 24, 24, 3, 3, 5, 3, 9999, 48, 48, 24},
                { 8, 8, 50, 6, 6, 0, 0, 8, 8, 8, 8, 8, 8, 50, 9999, 0, 8},
                { 8, 8, 50, 6, 6, 0, 0, 8, 8, 8, 8, 8, 8, 50, 0, 9999, 8},
                { 5, 5, 26, 12, 12, 8, 8, 0, 0, 5, 5, 5, 5, 26, 8, 8, 9999}
            };

            Cplex model = new Cplex();

            // Creare variables
            INumVar[][] x = new INumVar[n][];
            for (int i = 0; i < n; i++)
            {
                x[i] = new INumVar[n];
                for (int j = 0; j < n; j++)
                {
                    if (i == j)
                        x[i][j] = model.NumVar(0, 0, NumVarType.Bool, $"x({i},{j})");
                    else
                        x[i][j] = model.NumVar(0, 1, NumVarType.Bool, $"x({i},{j})");
                }
            }

            INumVar[] u = model.NumVarArray(n, 0, n - 1, NumVarType.Float);

            // Create objective function
            INumExpr expr = model.NumExpr();
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    expr = model.Sum(expr, model.Prod(distance_matrix[i, j], x[i][j]));
                }
            }
            model.AddMinimize(expr);

            // Assigment constraints 1 
            for (int i = 0; i < n; i++)
            {
                model.AddEq(model.Sum(x[i]), 1);
            }

            // Assigment constraints 2
            for (int j = 0; j < n; j++)
            {
                expr = model.NumExpr();
                for (int i = 0; i < n; i++)
                {
                    expr = model.Sum(expr, x[i][j]);
                }
                model.AddEq(expr, 1);
            }

            // Subtour Elimination Constraint - MTZ
            for(int i = 0; i < n; i++)
            {
                for(int j = 1; j < n; j++)
                {
                    if(i != j)
                    {
                        model.AddLe(model.Sum(u[i], model.Prod(-1, u[j]), model.Prod(n, x[i][j])), n - 1);
                    }
                }
            }

            // Export lp file & solve it
            model.ExportModel("TPS.lp");
            if (model.Solve())
            {
                Console.WriteLine($"Objective Value = {model.GetObjValue()}");
                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        Console.WriteLine($"{x[i][j].Name} = {model.GetValue(x[i][j])}");
                    }
                }
            }
        }
    }
}
