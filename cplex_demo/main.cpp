#include <ilcplex/cplex.h>
#include <ilcplex/cplexx.h>
#include <vector>
#include <string>
#include <numeric> 
#include <iostream>

int main(int argc, const char ** argv)
{
    int status = 0;
    CPXENVptr env = CPXopenCPLEX(&status);
    CPXLPptr lp = CPXcreateprob(env, &status, "test_problem");;

    // add variables
    int numCols = 5;
    std::vector<double> lb(numCols, 0.0);  
    std::vector<double> ub(numCols, CPX_INFBOUND); 
    std::vector<std::string> colname(numCols);
    for (int i = 0; i < numCols; i++)
        colname[i] = "x" + std::to_string(i);
    
    std::vector<char*> colname_str;
    for (const auto& str : colname)
        colname_str.push_back(const_cast<char*>(str.c_str()));
    
    std::vector<double> obj(numCols, 0.0);
    status = CPXnewcols(env, lp, numCols, obj.data(), lb.data(), ub.data(), NULL, colname_str.data());

    // set variable name
    std::vector<int> indices(numCols);
    std::iota(indices.begin(), indices.end(), 0);

    for (int i = 0; i < numCols; i++)
        colname[i] = "y" + std::to_string(i);
    
    for (int i = 0; i < numCols; i++)
        colname_str[i] = const_cast<char*>(colname[i].c_str());

    status = CPXchgcolname(env, lp, numCols, indices.data(), colname_str.data());
    
    // change variable type
    for (int i = 0; i < numCols; i++)
        status = CPXchgctype(env, lp, 1, &i, "I");

    // change variable bound
    lb = std::vector<double>(numCols, 1.0);
    ub = std::vector<double>(numCols, 5.0);
    for (int i = 0; i < numCols; i++) {
        status = CPXchgbds(env, lp, 1, &i, "L", &lb[i]);
        status = CPXchgbds(env, lp, 1, &i, "U", &ub[i]);
        // status = CPXchgbds(env, lp, 1, &i, "B", &ud[i]); both lb and ub
    }

    // remove variable
    status = CPXdelcols(env, lp, 4, 5); // remove cols 4

    // add constraint
    int ccnt = 0; // the number of new cols
    int rcnt = 1; // the nuboer of new rows
    int nzcnt = 3; // the number of nonzeros constraints coefficients
    double rhs = 6;
    std::vector<int> rmatbeg = {0};
    std::vector<int> rmatind = {0, 1, 3};
    std::vector<double> rmatval = {2, 10, 4};

    status = CPXaddrows(env, lp, ccnt, rcnt, nzcnt, &rhs, "L", rmatbeg.data(), rmatind.data(), rmatval.data(), NULL, NULL);
    
    // sense L:<=, E:=, G:>=
    rmatbeg = {0};
    rmatind = {0, 4};
    rmatval = {1, 2};
    nzcnt = 2;
    status = CPXaddrows(env, lp, ccnt, rcnt, nzcnt, &rhs, "G", rmatbeg.data(), rmatind.data(), rmatval.data(), NULL, NULL);
    
    status = CPXwriteprob(env, lp, "test.lp", NULL);
    if (status) 
        std::cout << "Write Model Error" << std::endl;
    
    status = CPXdelrows(env, lp, 1, 2); // del constraints 1

    // set params
    status = CPXsetdblparam(env, CPX_PARAM_TILIM, 7200);
	status = CPXsetintparam (env, CPXPARAM_Threads, 8);
    status = CPXsetlogfilename (env, "cplex_log.txt", "w");
	status = CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_ON);

    // solve
    status = CPXmipopt(env, lp);
    status = CPXsolwrite(env, lp, "soln.sol");

    double objValue = -CPX_INFBOUND;
    if (CPXgetstat(env, lp) != CPX_STAT_INForUNBD) {
		status = CPXgetobjval(env, lp, &objValue);
	}

    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return 0;
}