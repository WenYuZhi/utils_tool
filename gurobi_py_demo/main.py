from gurobipy import GRB
import gurobipy as gp

# 创建模型
m = gp.Model()

# add variable
x = m.addVars(3, 3, lb=0, ub=GRB.INFINITY, vtype=GRB.CONTINUOUS, name="x")
y = m.addVar(lb=0, ub=GRB.INFINITY, obj = 2.0, vtype=GRB.CONTINUOUS, name="y")
m.remove(y)

# set variable lb and ub
# var.setAttr(GRB.Attr.UB, 0.0) 
# var.setAttr("ub", 0.0)
# var.lb = 0
for i, var in enumerate(m.getVars()):
   var.setAttr(GRB.Attr.UB, 10.0) 
   var.setAttr(GRB.Attr.VarName, f"x{i}" )
   var.setAttr(GRB.Attr.VType, GRB.INTEGER)

# add a constraint
c0 = m.addConstr(x[0,0] + x[1,1] <= 2, "c0")

# add constraints
c = m.addConstrs((x[0,0] + 2*x[i+1,0] <= 3 for i in range(2)), name = "c")

# m.remove(c) remove all constraints in c 
m.remove(c0)
m.update()

# set objective function
obj_expr = gp.LinExpr()
for i, var in enumerate(m.getVars()):
    obj_expr += i*var

m.setObjective(obj_expr + 1) # m.setObjective(obj_expr, GRB.MAXIMIZE)

# 获取并打印模型信息
print("Number of variable:", m.NumVars)
print("Number of constraints:", m.NumConstrs)
print("Number of nonzeros:", m.NumNZs)

# 获取并打印变量信息
for v in m.getVars():
    print(f"Var_name:{v.VarName}, Type:{v.VType}, ub:{v.UB}, lb:{v.LB}, obj_coeff:{v.Obj}")

# 获取并打印约束信息
for c in m.getConstrs():
    print(f"Consts_name:{c.ConstrName}, RHS:{c.RHS}, Sense:{c.Sense}")

# 获取并打印目标函数信息
print("obj sense:", "min" if m.ModelSense == 1 else "max")
print("obj offset:", m.ObjCon)

m.setParam("Threads", 1)
m.setParam('TimeLimit', 120) 
m.setParam('MIPGap', 0.0) 
m.setParam('OutputFlag', 1)
m.setParam('LogFile', 'gurobi.log')
m.setParam('Method', 0) # lp method set to 2
# lp method: -1 auto, 0 primal simplex, 1 dual simplex, 2 barrier, 3 concurrent, 4 deterministic concurrent

# set start point
for var in m.getVars():
    var.setAttr(GRB.Attr.Start, 0)

m.optimize()

if m.Status == GRB.OPTIMAL:
    print("Founded Optimal Solution")
elif m.Status == GRB.INFEASIBLE:
    print("Model is infeasible")
elif m.Status == GRB.INF_OR_UNBD:
    print("Model is unbounded or infeasible")
elif m.Status == GRB.TIME_LIMIT:
    print("Time Limit is reached")

# get solution
for v in m.getVars():
    print(f"{v.varName}: {v.X}")

# get dual
for c in m.getConstrs():
    print(f"{c.constrName}: {c.Pi}")

# get reduced costs
for v in m.getVars():
    print(f"{v.varName}: {v.RC}")

# read and write model file
# m.read()

# copy model
m_new = m.copy()

# get variable and constrs by name
m_new.getVarByName("x[0,0]")
m_new.getConstrByName("c1")

m_new.write("./output_model/out_model.lp")
