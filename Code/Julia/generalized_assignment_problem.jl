using JuMP, HiGHS
using Coluna, BlockDecomposition

include("read_data.jl")

# Global Parameters
global isDecompose = true

function main()::Nothing

    # only "a05100", "a10100" solvable
    instances::Array{String} = ["a05100", "a05200", "a10100", "a10200", "a20100", "a20200"]

    for filename in instances
        # read GAP instance
        println("=====================================================$filename")
        num_agents, num_jobs, cost, weights, capacity = read_gap_instance(filename)

        # println("num of agents = $num_agents")
        # println("num of jobs = $num_jobs")
        # println("cost matrix")
        # println(cost)
        # println("weights matrix")
        # println(weights)
        # println("capacity vector")
        # println(capacity)
        #=
            build a optimization model
        =#
        if isDecompose
            @axis(A, 1:num_agents)
            J = 1:num_jobs

            coluna = optimizer_with_attributes(
                Coluna.Optimizer,
                "params" => Coluna.Params(
                    solver=Coluna.Algorithm.TreeSearchAlgorithm()
                ),
                "default_optimizer" => HiGHS.Optimizer
            )
            # m = BlockModel(HiGHS.Optimizer)
            m = BlockModel(coluna)

            # create decision variables
            @variable(m, x[a in A, j in J], Bin)

            # create objective function
            @objective(m, Min, sum(cost[a, j] * x[a, j] for a in A for j in J))

            # create constraints
            # knapsack constraints
            @constraint(m, knapsack[a in A], sum(x[a, :]) <= capacity[a])

            # SOS1
            @constraint(m, SOS[j in J], sum(x[:, j]) == 1)

            # export lp file and solve it 
            write_to_file(m, "gap.lp")
            @dantzig_wolfe_decomposition(m, decomposition, A)
            JuMP.optimize!(m)

            # display solution
            println("objective function = ", objective_value(m))
            println("=====================================================")

        else
            A = 1:num_agets
            J = 1:num_jobs

            m = Model(HiGHS.Optimizer)

            # create decision variables
            @variable(m, x[A, J] >= 0, Bin)

            # create objective function
            @objective(m, Min, sum(cost[a, j] * x[a, j] for a in A for j in J))

            # create constraints
            # knapsack constraints
            @constraint(m, knapsack[a in A], sum(x[a, :]) <= capacity[a])

            # SOS1
            @constraint(m, SOS[j in J], sum(x[:, j]) == 1)

            # export lp file and solve it
            write_to_file(m, "gap.lp")
            JuMP.optimize!(m)

            # display solution
            println("objective function = ", objective_value(m))
            println("=====================================================")
        end
    end

    return
end


main()