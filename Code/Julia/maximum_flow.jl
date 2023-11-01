using JuMP, HiGHS


num_nodes = 10

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

m = Model(HiGHS.Optimizer)

# create decision variables
@variable(m, x[1:num_nodes, 1:num_nodes] >= 0)

# create objective function
@objective(m, Max, sum(x[i, j] for i in 1:num_nodes for j in 1:num_nodes))

# create constraints
# capacity constraints
@constraint(m, capacity[i in 1:num_nodes, j in 1:num_nodes], x[i, j] <= adj_matrix[i, j])

# flow balance constraints
@constraint(m, flow_balance[k in 2:num_nodes-1], sum(x[:, k]) == sum(x[k, :]))

# export lp file and solve it
write_to_file(m, "maximum_flow.lp")
JuMP.optimize!(m)

println("Objective value = ", objective_value(m))
println("Optimal Solution:")
for i in 1:num_nodes
    for j in 1:num_nodes
        if JuMP.value(x[i, j]) != 0
            println("x[$i,$j] = ", JuMP.value(x[i, j]))
        end
    end
end