using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;

namespace optimization_model
{
    internal class maximum_cliques
    {
        public static void Build()
        {
            List<int> n = Enumerable.Range(0, 10).ToList();
            List<string> x_name = n.Select(value => "x" + value.ToString()).ToList();

            int[,] E = new int[n.Count,n.Count];
            for(int i = 0; i < n.Count; i++)
            {
                for(int j = 0; j < n.Count; j++)
                {
                    if (i > j && i % 2 == 0)
                    {
                        E[i, j] = 1;
                        E[j, i] = 1;
                    }
                    else
                    {
                        E[i, j] = 0;
                        E[j, i] = 0;
                    }
                }
            }

            Cplex model = new Cplex();

            // Create variables
            INumVar[] x = model.NumVarArray(n.Count, 0, 1, NumVarType.Bool, x_name.ToArray());

            // Objective function
            model.AddMaximize(model.Sum(x));

            // Constraint 1
            for(int i = 0; i < n.Count; i++)
            {
                for (int j = 0; j < n.Count; j++)
                {
                    if (E[i,j] == 0 && i != j)
                    {
                        model.AddLe(model.Sum(x[i], x[j]), 1);
                    }
                }
            }

            // Export lp file & solve it
            model.ExportModel("maximum_clique.lp");
            if (model.Solve())
            {
                for(int i = 0; i < n.Count; i++)
                {
                    Console.WriteLine($"{x[i].Name} = {model.GetValue(x[i])}");
                }
            }


            Console.ReadLine();
        }
    }
}
