
function read_gap_instance(filename::String)::Tuple{Integer,Integer,Array,Array,Array}
    f = open("./$filename", "r")

    # number of agents 
    num_agents::Integer = 0

    # number of jobs
    num_jobs::Integer = 0

    # cost matrix 
    cost = Array{Integer}(undef, 0, 0)

    # weights matrix
    weights = Array{Integer}(undef, 0, 0)

    # capacity vector
    capacity = Array{Integer}(undef, 0)

    current_agent_id::Integer = 1
    current_job_id::Integer = 1
    is_cost::Bool = true
    is_weights::Bool = false
    is_capacity::Bool = false

    for (line_number, line) in enumerate(eachline(f))
        line = strip(line)

        parts = split(line)

        if line_number == 1
            num_agents = parse(Int, parts[1])
            num_jobs = parse(Int, parts[2])

            cost = Array{Integer}(undef, num_agents, num_jobs)
            weights = Array{Integer}(undef, num_agents, num_jobs)
            capacity = Array{Integer}(undef, num_agents)
        else
            for (i, p) in enumerate(parts)
                if is_cost
                    cost[current_agent_id, current_job_id] = parse(Int, p)
                elseif is_weights
                    weights[current_agent_id, current_job_id] = parse(Int, p)
                else
                    capacity[current_agent_id] = parse(Int, p)
                end

                if current_job_id == num_jobs && current_agent_id < num_agents
                    current_job_id = 1
                    current_agent_id += 1
                elseif current_job_id == num_jobs && current_agent_id == num_agents
                    current_job_id = 1
                    current_agent_id = 1

                    if is_cost
                        is_cost = false
                        is_weights = true
                    elseif is_weights
                        is_weights = false
                        is_capacity = true
                    end
                elseif is_capacity
                    current_agent_id += 1
                else
                    current_job_id += 1
                end

            end
        end
    end

    close(f)

    return num_agents, num_jobs, cost, weights, capacity
end
