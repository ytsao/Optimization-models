#include <vector>

class TSP_DFJ {
public:
	static void Build();

private:
	static std::vector<std::vector<int>> Q;
	static void combinationUtil(int arr[], int n, int r, int index, std::vector<int> data, int i);
	static void printCombination(int arr[], int n, int r);
};