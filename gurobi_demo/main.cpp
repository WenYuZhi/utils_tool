#include <vector>
#include <string>
#include <gurobi_c++.h>

void getCSRFormat(GRBModel& model, std::vector<int>& rowStart, std::vector<int>& colIdx, std::vector<double>& colCoeff) {
    rowStart.clear();
    colIdx.clear();
    colCoeff.clear();
    GRBConstr* constrs = model.getConstrs();
    int numCols = model.get(GRB_IntAttr_NumVars);
    int numRows = model.get(GRB_IntAttr_NumConstrs);
    int nzCount = 0;
    rowStart.push_back(0); // 初始化第一个元素
    
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            double coeff = model.getCoeff(model.getConstr(i), model.getVar(j));
            if (coeff != 0) {
                colIdx.push_back(j);
                colCoeff.push_back(coeff);
                nzCount++;
            }
        }
        rowStart.push_back(nzCount);
    }
}

int main(int argc, const char ** argv)
{
    GRBEnv env;
    GRBModel model = GRBModel(env);

    // add one variable
    // variable type: GRB_CONTINUOUS, GRB_BINARY, GRB_INTEGER, GRB_SEMICONT, GRB_SEMIINT
    GRBVar x = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x"); // (lb, ub, obj, type, name)
    
    std::vector<GRBVar> y(5);
    for (int i = 0; i < 5; i++)
        y[i] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, "y" + std::to_string(i));

    // set lb and ub
    x.set(GRB_DoubleAttr_LB, 0.0); 
    x.set(GRB_DoubleAttr_UB, 10.0); 
    
    // set variable type
    x.set(GRB_CharAttr_VType, GRB_INTEGER);

    // set variabel name
    x.set(GRB_StringAttr_VarName, "C0000");

    // remove y[0] variable
    model.remove(y[4]);
    
    // added constraint
    GRBLinExpr expr = 2 * x + 3 * y[0];
    GRBConstr c1 = model.addConstr(expr, GRB_LESS_EQUAL, 4, "c_1"); 
    GRBConstr c2 = model.addConstr(expr, GRB_GREATER_EQUAL, 4, "c_2");
    GRBConstr c3 = model.addConstr(expr, GRB_EQUAL, 4, "c_3");
    
    // added constraint
    GRBLinExpr lhs_expr = y[0] + 2 * y[1];
    GRBConstr c4 = model.addConstr(lhs_expr + x <= 10, "c_4");

    // added range constraint
    GRBLinExpr range_expr = 3*y[0] + 10 * y[2] - y[3];
    GRBConstr c5 = model.addRange(range_expr, 2, 6, "c_5");

    // change one coeff in model
    model.chgCoeff(c5, y[0], 2.0); // change y[0] coeff in constraint c5 to 2.0
    
    // remove constraint
    model.remove(c1);
    
    // set objective
    GRBLinExpr objective_expr = x + y[0] + y[1];
    model.setObjective(objective_expr, GRB_MINIMIZE); // GRB_MINIMIZE or GRB_MAXIMIZE 

    // set parameters
    model.set(GRB_DoubleParam_TimeLimit, 100.0);
    model.set("TimeLimit", "20");
    model.set(GRB_IntParam_Threads, 8);
    model.set("Threads", "16");
    model.set(GRB_StringParam_LogFile, "./gurobi.txt");
    model.set("LogFile", "./gurobi.txt");
    model.set(GRB_IntParam_OutputFlag, 0);
    model.set("OutputFlag", "1");
   
    // set start point
    x.set(GRB_DoubleAttr_Start, 0.0);
    model.optimize();
    

    // get solution
    int status = model.get(GRB_IntAttr_Status);

    switch (status) {
        case GRB_OPTIMAL:
            std::cout << "Optimal solution found." << std::endl;
            std::cout << x.get(GRB_StringAttr_VarName) << ":" << x.get(GRB_DoubleAttr_X) << std::endl;
            std::cout << "Objective value: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
            break;
        case GRB_INFEASIBLE:
            std::cout << "Model is infeasible." << std::endl;
            break;
        case GRB_UNBOUNDED:
            std::cout << "Model is unbounded." << std::endl;
            break;
        default:
            std::cout << "Other status: " << status << std::endl;
            break;
    }

    // get model info
    int numCols = model.get(GRB_IntAttr_NumVars);
    int numRows = model.get(GRB_IntAttr_NumConstrs);
    int numNonZeros = model.get(GRB_IntAttr_NumNZs);
    
    std::vector<char> vartypes;
    for (int i = 0; i < model.get(GRB_IntAttr_NumVars); ++i) 
        vartypes.push_back(model.getVar(i).get(GRB_CharAttr_VType));
    
    std::vector<char> sense;
    for (int i = 0; i < model.get(GRB_IntAttr_NumConstrs); ++i) 
        sense.push_back(model.getConstr(i).get(GRB_CharAttr_Sense));
    
    std::vector<double> lb;
    std::vector<double> ub;
    for (int i = 0; i < model.get(GRB_IntAttr_NumVars); ++i) {
        lb.push_back(model.getVar(i).get(GRB_DoubleAttr_LB));
        ub.push_back(model.getVar(i).get(GRB_DoubleAttr_UB));
    }
    
    std::vector<std::string> varname;
    for (int i = 0; i < model.get(GRB_IntAttr_NumVars); ++i) 
        varname.push_back(model.getVar(i).get(GRB_StringAttr_VarName));
    
    std::vector<double> rhs;
    for (int i = 0; i < model.get(GRB_IntAttr_NumConstrs); ++i) 
        rhs.push_back(model.getConstr(i).get(GRB_DoubleAttr_RHS));
    
    std::vector<double> objCoeff;
    for (int i = 0; i < numCols; ++i)
        objCoeff.push_back(model.getVar(i).get(GRB_DoubleAttr_Obj));

    double objConstant = model.get(GRB_DoubleAttr_ObjCon);
        
    std::vector<int> rowStart; 
    std::vector<int> colIdx; 
    std::vector<double> colCoeff; 

    getCSRFormat(model, rowStart, colIdx, colCoeff);

    // rowStart.push_back(0);
    // int numOfIdx = 0;
    // for (int i = 0; i < numRows; ++i) {
    //     for (int j = 0; j < numCols; ++j) {
    //         double coeff = model.getCoeff(model.getConstr(i), model.getVar(j));
    //         if (coeff != 0) {
    //             colIdx.push_back(j);
    //             colCoeff.push_back(coeff);
    //             numOfIdx++;
    //         }
    //     }
    //     rowStart.push_back(numOfIdx);
    // }
    
    
    // read model file
    std::string inputPath = "../data/input_model/test.lp";
    GRBModel model_new = GRBModel(&env, inputPath);
    if (model.get(GRB_IntAttr_Status) == GRB_LOADED) {
        std::cout << "Model Loaded Successfully" << std::endl;
    }
    
    // copy model
    GRBModel model_copy = GRBModel(model_new);

    GRBConstr* constrs_list = model_copy.getConstrs();
    GRBVar* vars_list = model_copy.getVars();

    // automated search for parameter settings
    // model_copy.tune();
    
    // def linexpr
    GRBLinExpr expr1 = GRBLinExpr(0.0);
    expr1.clear();
    expr1 = GRBLinExpr(vars_list[4], 11.0); // GRBLinExpr(GRBVar var, double coeff)

    std::vector<double> exprCoeff(3, 2.0);
    expr1.addTerms(exprCoeff.data(), vars_list, 3);
    int expr_size = expr1.size();

    model_copy.addConstr(expr1 <= 10);

    // doubel val = expr1.getConstant();
    // doube val = expr1.getCoeff(i);
    // doube val = expr1.getValue(); Compute the value of expr1 for the current solution.
    // GRB var = expr1.getVar(i); 

     // write model file
    model_copy.write("../data/output_model/test.lp");

    // model update
    model_copy.update();
}