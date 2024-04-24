using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Configuration;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;

namespace optimization_model
{
    internal class TSP_DFJ
    {
        private static List<List<int>> Q = new List<List<int>>();
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
            for(int i = 0; i < n; i++)
            {
                x[i] = new INumVar[n];
                for(int j = 0; j < n; j++)
                {
                    if (i == j)
                        x[i][j] = model.NumVar(0, 0, NumVarType.Bool, $"x({i},{j})");
                    else
                        x[i][j] = model.NumVar(0, 1, NumVarType.Bool, $"x({i},{j})");
                }
            }

            // Create objective function
            INumExpr expr = model.NumExpr();
            for(int i = 0; i < n; i++)
            {
                for(int j = 0; j < n; j++)
                {
                    expr = model.Sum(expr, model.Prod(distance_matrix[i, j], x[i][j]));
                }
            }
            model.AddMinimize(expr);

            // Assigment constraints 1 
            for(int i = 0; i < n; i++)
            {
                model.AddEq(model.Sum(x[i]), 1);
            }

            // Assigment constraints 2
            for(int j = 0; j < n; j++)
            {
                expr = model.NumExpr();
                for(int i = 0; i < n; i++)
                {
                    expr = model.Sum(expr, x[i][j]);
                }
                model.AddEq(expr, 1);
            }


            // Subtour Elimination Constraint - DFJ
            for(int i = 2; i < n; i++)
            {
                printCombination(cities.ToArray(), n, i);
            }

            foreach(List<int> q in Q)
            {
                expr = model.NumExpr();
                foreach(int q1 in q)
                {
                    foreach(int q2 in q)
                    {
                        if (q1 != q2)
                        {
                            expr = model.Sum(expr, x[q1][q2]);
                        }
                    }
                }

                model.AddLe(expr, q.Count - 1);
            }

            // Export lp file & solve it
            model.ExportModel("TSP.lp");
            if (model.Solve())
            {
                Console.WriteLine($"Objective Value = {model.GetObjValue()}");
                for(int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                    {
                        Console.WriteLine($"{x[i][j].Name} = {model.GetValue(x[i][j])}");
                    }
                }
            }
        }

        /* arr[] ---> Input Array
        data[] ---> Temporary array to
                    store current combination
        start & end ---> Starting and Ending
                         indexes in arr[]
        index ---> Current index in data[]
        r ---> Size of a combination
           to be printed */
        private static void combinationUtil(int[] arr, int n,
                                    int r, int index,
                                    int[] data, int i)
        {
            // Current combination is ready
            // to be printed, print it
            if (index == r)
            {
                List<int> q = new List<int>();
                for (int j = 0; j < r; j++)
                    q.Add(data[j]);
                Q.Add(q);
                return;
            }

            // When no more elements are
            // there to put in data[]
            if (i >= n)
                return;

            // current is included, put
            // next at next location
            data[index] = arr[i];
            combinationUtil(arr, n, r,
                            index + 1, data, i + 1);

            // current is excluded, replace
            // it with next (Note that
            // i+1 is passed, but index
            // is not changed)
            combinationUtil(arr, n, r, index,
                            data, i + 1);
        }

        // The main function that prints
        // all combinations of size r
        // in arr[] of size n. This
        // function mainly uses combinationUtil()
        private static void printCombination(int[] arr,
                                     int n, int r)
        {
            // A temporary array to store
            // all combination one by one
            int[] data = new int[r];

            // Print all combination
            // using temporary array 'data[]'
            combinationUtil(arr, n, r, 0, data, 0);
        }
    }
}
