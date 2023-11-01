#=
  Task 2: Solve the problem using the Edmonds-Karp Algorithm using Julia Graph and GraphsFlows packages
=#
using Graphs, GraphsFlows

num_nodes = 10

# create a weighted graph
adj_matrix = [0 16 13 0 0 0 0 0 0 0
  0 0 0 7 12 0 0 0 0 0
  0 4 0 0 0 14 0 0 0 0
  0 0 0 0 10 0 0 0 0 0
  0 0 9 0 0 0 0 4 20 0
  0 0 0 0 7 0 7 0 4 0
  0 0 0 0 0 0 0 0 15 0
  0 0 0 0 6 0 0 0 9 10
  0 0 0 0 0 0 0 5 0 12
  0 0 0 0 0 0 0 0 0 0]

g = Graphs.DiGraph(num_nodes)

for i in 1:num_nodes
  for j in 1:num_nodes
    add_edge!(g, i, j)
  end
end

f, F = maximum_flow(g, 1, 10, adj_matrix, algorithm=EdmondsKarpAlgorithm())
@show f
@show F