using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ILOG.Concert;
using ILOG.CPLEX;

namespace optimization_model
{
    internal class bin_packing
    {
        public static void Build()
        {
            int B = 100;
            int items = 11;
            int num_sets = 11;
            int[] s = new int[items];
            s[0] = 48;
            s[1] = 30;
            s[2] = 19;
            s[3] = 36;
            s[4] = 36;
            s[5] = 27;
            s[6] = 42;
            s[7] = 42;
            s[8] = 36;
            s[9] = 24;
            s[10] = 30;

            Cplex model = new Cplex();

            // Create variables
            INumVar[][] x = new INumVar[items][];
            for(int i = 0; i < items; i++)
            {
                x[i] = new INumVar[num_sets];
                for (int j = 0; j < num_sets; j++)
                {
                    x[i][j] = model.NumVar(0, 1, NumVarType.Bool, $"x({i},{j})");
                }
            }


            INumVar[] y = new INumVar[num_sets];
            for (int i = 0; i < num_sets; i++)
            {
                y[i] = model.NumVar(0, 1, NumVarType.Bool, $"y{i}");
            }

            // Create objective function
            model.AddMinimize(model.Sum(y));

            // Create Constraints
            // Capacity constraints
            for (int j = 0; j < num_sets; j++)
            {
                INumExpr expr = model.NumExpr();
                for(int i = 0; i < items; i++)
                {
                    expr = model.Sum(expr, model.Prod(s[i], x[i][j]));
                }

                //  add constraints
                model.AddLe(expr, model.Prod(B, y[j]));
            }

            // Assignment Constraints
            for(int i = 0; i < items; i++)
            {
                model.AddEq(model.Sum(x[i]), 1);
            }

            // Export lp file & solve it
            model.ExportModel("BinPacking.lp");
            if (model.Solve())
            {
                for(int i = 0; i < items; i++)
                {
                    for (int j = 0; j < num_sets; j++)
                    {
                        Console.WriteLine($"{x[i][j].Name} = {model.GetValue(x[i][j])}");
                    }
                }

                for (int i = 0; i < num_sets; i++)
                {
                    Console.WriteLine($"{y[i].Name} = {model.GetValue(y[i])}");
                }
            }

            Console.ReadLine();
        }
    }
}
