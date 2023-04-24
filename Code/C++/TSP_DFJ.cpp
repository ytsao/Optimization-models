#include "TSP_DFJ.h"

#include <vector>
#include "ilcplex/ilocplex.h"
#include "ilconcert/cplexconcertdoc.h"

std::vector<std::vector<int>> TSP_DFJ::Q;

void TSP_DFJ::Build() {

	IloEnv env;
	IloModel model(env);
	IloCplex cplex(env);

	int n = 17;
	int cities[17] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	int distance_matrix[17][17] =
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

	// Create variables
	IloArray<IloBoolVarArray> x(env, n);
	for (int i = 0; i < n; i++) {
		x[i] = IloBoolVarArray(env, n);

		for (int j = 0; j < n; j++) {
			x[i][j].setName(("x(" + std::to_string(i) + "," + std::to_string(j) + ")").c_str());
		}
	}

	// Create objective function
	IloExpr obj(env);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			obj += distance_matrix[i][j] * x[i][j];
		}
	}
	model.add(IloMinimize(env, obj));
	obj.clear();
	obj.end();

	// Assignment constraints 1
	for (int i = 0; i < n; i++)
		model.add(IloSum(x[i]) == 1);

	// Assignment constraint 2
	IloExpr cons(env);
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++) {
			cons += x[i][j];
		}
		model.add(cons == 1);
		cons.clear();
	}

	// Subtour Elimination constraints - DFJ
	for (int i = 2; i < n; i++) {
		printCombination(cities, n, i);
	}

	for (std::vector<int> q : TSP_DFJ::Q) {
		for (int q1 : q) {
			for (int q2 : q) {
				if (q1 != q2) {
					cons += x[q1][q2];
				}
			}
		}

		int rhs = q.size() - 1;
		model.add(cons <= rhs);
		cons.clear();
	}

	// ExoortModel & solve
	cplex.extract(model);
	cplex.exportModel("TSP_DFJ.lp");

	if (cplex.solve()) {
		double objValue = cplex.getObjValue();
		std::cout << "objective value = " << objValue << std::endl;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (cplex.getValue(x[i][j]))
					std::cout << x[i][j].getName() << " = " << cplex.getValue(x[i][j]) << std::endl;
			}
		}
	}
}

/*
	arr[]	-> Input array
	data[]	-> Temporary array to store current combination
	start	-> Starting index in arr[]
	end		-> Ending index in arr[]
	index	-> Current index in data[]
	r		-> Size of a combination to be printed
*/
void TSP_DFJ::combinationUtil(int arr[], int n, int r, int index, std::vector<int> data, int i) {
	// Current combination is ready
	// to be printed, print it
	if (index == r) {
		std::vector<int> q;
		for (int j = 0; j < r; j++)
			q.push_back(data[j]);
		TSP_DFJ::Q.push_back(q);
		return;
	}

	// When no more elements are
	// There to put in data[]
	if (i >= n) return;

	// Current is included, put
	// Next at next location
	data[index] = arr[i];
	combinationUtil(arr, n, r, index + 1, data, i + 1);

	// Current is excluded, replace
	// It with next (Note that
	// i+1 is passed, but index is not changed)
	combinationUtil(arr, n, r, index, data, i + 1);
}

// The main function that prints all combination of size r
// in arr[] of size n.
// This function mainly uses combinationUntil()
void TSP_DFJ::printCombination(int arr[], int n, int r) {
	// A temporary to store all combination one by one
	std::vector<int> data(r, 0);

	// Print all combination using temporary vector 'data'
	combinationUtil(arr, n, r, 0, data, 0);
}