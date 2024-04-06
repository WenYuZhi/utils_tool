import cplex
import utils

# Create a new LP problem
problem = cplex.Cplex()

# Add variables
names, obj = ["x1", "x2", "x3", "x4", "x5", "x6"], [1.0, 2.0, 30.0, 3, 4, 5]
lb, ub = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0], [cplex.infinity, cplex.infinity, cplex.infinity, cplex.infinity, cplex.infinity, cplex.infinity]
ctype = ["I", "I", "I", "I", "I", "I"]  # C: continuous, I: integer, B: binary 
problem.variables.add(names=names, obj=obj, lb=lb, ub=ub, types=ctype)

# set bounds 
problem.variables.set_lower_bounds("x1", 1.0)
problem.variables.set_lower_bounds([(1, 10), (2, 3)]) # set lower bounds for x2 = 10 and x3 = 2
problem.variables.set_upper_bounds([("x4", 1), ("x5", 1)]) # set upper bounds for x4 = 1 and x5 = 2

problem.variables.set_names(0, "x1_new") # rename x1 to x1_new
problem.variables.set_names([(1, "x2_new"), (2, "x3_new")]) # rename x2 to x2_new and x3 to x3_new
problem.variables.set_names([("x4", "x4_new"), ("x5", "x5_new")]) # rename x4 to x4_new and x5 to x5_new

# set types 
problem.variables.set_types("x1_new", problem.variables.type.continuous)
problem.variables.set_types([(1, problem.variables.type.continuous), (2, problem.variables.type.continuous)])
problem.variables.set_types([("x3_new", problem.variables.type.continuous), ("x4_new", problem.variables.type.continuous)])

# delete variables
problem.variables.add(names=["y1", "y2", "y3"], obj=[0, 0, 0], lb=[0, 0, 0], ub=[2, 2, 2], types=["I", "I", "I"])
problem.variables.delete("y1")
problem.variables.delete(["y2", "y3"]) # problem.variables.delete([1, 2])
problem.variables.delete([1]) # 请注意，删除变量后，剩余变量的索引将会自动更新（gurobi lazy update）

# add a constraint
problem.linear_constraints.add(
    lin_expr=[cplex.SparsePair(ind=["x1_new", "x3_new"], val=[1.0, 2.0])],
    senses=["R"],
    rhs=[10],
    names=["c1"]
)

# add constraints 
problem.linear_constraints.add(
    lin_expr=[
        cplex.SparsePair(ind=["x1_new", "x4_new"], val=[1.0, 1.0]),
        cplex.SparsePair(ind=["x1_new", "x5_new", "x6"], val=[2.0, 1.0, -2.0]),
        cplex.SparsePair(ind=["x3_new", "x5_new"], val=[1.2, -1.0])
    ],
    senses=["L", "G", "L"],
    rhs=[20, 5, 6],
    names=["c2", "c3", "c4"]
) # L: less than, G: greater than, E: equal


# 删除名为"c1"的约束
# problem.linear_constraints.delete("c1")

# 删除索引为0的约束
problem.linear_constraints.delete(0)

# set objective function
problem.objective.set_linear([(name, 0.0) for name in problem.variables.get_names()]) # set all coefficients to 0
problem.objective.set_linear([("x1_new", 3), ("x3_new", 4), ("x4_new", 5)]) 

problem.linear_constraints.set_linear_components("c2", [("x1_new", "x3_new"), (0.8, 2)]) # set linear components for c2

# set minimize
# problem.objective.set_sense(model.objective.sense.maximize)
problem.objective.set_sense(problem.objective.sense.minimize)
problem.objective.set_offset(10) # offset = 10

objective_sense = problem.objective.get_sense()
if objective_sense == problem.objective.sense.minimize:
    print("objective sense: min")
elif objective_sense == problem.objective.sense.maximize:
    print("objective sense: max")

# get offset
offset = problem.objective.get_offset()
print("offset:", offset)

# set rhs
problem.linear_constraints.set_rhs("c2", 12)

# set the number of threads 
problem.parameters.threads.set(2)
# set timelimit 
problem.parameters.timelimit.set(60)
# set gap 
problem.parameters.mip.tolerances.mipgap.set(0.0)

# set lp method (if model is lp)
# problem.parameters.lpmethod.set(problem.parameters.lpmethod.auto)
# problem.parameters.lpmethod.set(problem.parameters.lpmethod.primal)
# problem.parameters.lpmethod.set(problem.parameters.lpmethod.dual)
# problem.parameters.lpmethod.set(problem.parameters.lpmethod.barrier)
# problem.parameters.lpmethod.set(problem.parameters.lpmethod.sifting)
# problem.parameters.lpmethod.set(problem.parameters.lpmethod.concurrent)

# close the output
# problem.set_results_stream(None)
problem.set_warning_stream(None)
problem.set_error_stream(None)

# save the log file
# problem.set_log_stream("./log.txt")

# close the log file
problem.set_log_stream(None)

# we have an start point
initial_values = [("x1_new", 1), ("x3_mew", 3), ("x4_new", 0), ("x5_mew", 0), ("x6", 0)]

# add start point
# problem.MIP_starts.add(initial_values, problem.MIP_starts.effort_level.auto)

# 获取模型的基本信息
num_vars = problem.variables.get_num()
num_constraints = problem.linear_constraints.get_num()
num_nonzeros = problem.linear_constraints.get_num_nonzeros()

print(f"Number of variables: {num_vars}")
print(f"Number of constraints: {num_constraints}")
print(f"Number of nonzeros: {num_nonzeros}")


# 获取变量的详细信息
var_lower_bounds = problem.variables.get_lower_bounds()
var_upper_bounds = problem.variables.get_upper_bounds()
var_types = problem.variables.get_types()
var_names = problem.variables.get_names()


# 获取约束的详细信息
constraint_rows = problem.linear_constraints.get_rows()
constraint_rhs = problem.linear_constraints.get_rhs()
constraint_senses = problem.linear_constraints.get_senses()
constraint_names = problem.linear_constraints.get_names()

print(constraint_rows) # 

# get csr matrix
row_ptr, col_ind, col_val = utils.get_csr_matrix(problem)

# get coefficients in objective function
coeff_x1 = problem.objective.get_linear("x1_new")
coeffs_all = problem.objective.get_linear()

problem.variables.get_indices("x6") # get the index of a variable

init_solution = [("x1_new", 1), ("x3_new", 3), ("x4_new", 0), ("x5_new", 0), ("x6", 0)]
# 将初始解的格式调整为CPLEX期望的格式
indices = [problem.variables.get_indices(var_name) for var_name, _ in init_solution]
values = [value for _, value in init_solution]

# 注意：这里我们将初始解分解为两个列表：一个用于索引，另一个用于值
# 并且将它们包装在一个更大的列表中，作为MIP_starts.add()方法的第一个参数
problem.MIP_starts.add([indices, values], problem.MIP_starts.effort_level.auto)

# solve the problem
problem.solve()

# CPX_STAT_OPTIMAL：找到了最优解。
# CPX_STAT_INFEASIBLE：模型不可行（没有解满足所有约束）。
# CPX_STAT_UNBOUNDED：模型无界（目标函数可以无限改进而不违反任何约束）。
# CPX_STAT_FEASIBLE：找到了可行解，但不一定是最优解。
# CPX_STAT_ABORT_TIME_LIM：因达到时间限制而终止求解。
status = problem.solution.get_status()
print("Solution status: ", problem.solution.status[status])

optimal_values = problem.solution.get_values()
print("Optimal values: ", optimal_values)

optimal_obj_value = problem.solution.get_objective_value()
print("Optimal objective value: ", optimal_obj_value)

# write the model file
problem.write("./output_model/model.lp")
problem.write("./output_model/model.mps")
problem.write("./output_model/model.mps.gz")

# write the sol file
problem.solution.write("./output_model/sol.txt")

# Read the model file
problem.read("./output_model/model.lp")


