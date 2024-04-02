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
    double rhs = 20;
    std::vector<int> rmatbeg = {0};
    std::vector<int> rmatind = {0, 1, 3};
    std::vector<double> rmatval = {2, 10, 4};

    status = CPXaddrows(env, lp, ccnt, rcnt, nzcnt, &rhs, "L", rmatbeg.data(), rmatind.data(), rmatval.data(), NULL, NULL);
    
    // sense L:<=, E:=, G:>=
    rmatbeg = {0};
    rmatind = {0, 4};
    rmatval = {1, 2};
    nzcnt = 2;
    rhs = 5;
    status = CPXaddrows(env, lp, ccnt, rcnt, nzcnt, &rhs, "G", rmatbeg.data(), rmatind.data(), rmatval.data(), NULL, NULL);
    
    status = CPXwriteprob(env, lp, "test.lp", NULL);
    if (status) 
        std::cout << "Write Model Error" << std::endl;
    
    status = CPXdelrows(env, lp, 1, 2); // del constraints 1
    
    numCols = CPXgetnumcols(env, lp);
    int numRows = CPXgetnumrows(env, lp);
    int numNonZeros = CPXgetnumnz(env, lp);

    std::vector<int> delstat(numRows, 0);
    status = CPXdelsetrows(env, lp, delstat.data());

    delstat = std::vector<int>(numCols, 0); // An array specifying the columns to be deleted.
    status = CPXdelsetcols(env, lp , delstat.data());  // if delstat[i] = 1 delete column i

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

    // get problem data
    std::vector<char> varTypes(numCols);
    status = CPXgetctype(env, lp, varTypes.data(), 0, numCols - 1);
     
    lb = std::vector<double>(numCols, 0.0);
    status = CPXgetlb(env, lp, lb.data(), 0, numCols - 1);

    ub = std::vector<double>(numCols, 0.0);
    status = CPXgetub(env, lp, ub.data(), 0, numCols - 1);

    int objSense = CPXgetobjsen(env, lp); // -1 means max, 1 means min;
    // status = CPXchgobjsen(env, lp, 1);  change objective sense

    std::vector<double> objCoeffs = std::vector<double>(numCols);
    status = CPXXgetobj(env, lp, objCoeffs.data(), 0, numCols - 1);

    std::vector<char> rowtypes = std::vector<char>(numRows);
    status = CPXgetsense(env, lp, rowtypes.data(), 0, numRows - 1);

    std::vector<double> rhs_get = std::vector<double>(numRows);
    status = CPXgetrhs(env, lp, rhs_get.data(), 0, numRows - 1);

    std::vector<double> rhsrange = std::vector<double>(numRows);
    status = CPXgetrngval(env, lp, rhsrange.data(), 0, numRows - 1);

    double objOffset = 0;
    status = CPXgetobjoffset(env, lp, &objOffset);

    std::vector<int> rowStart = std::vector<int>(numRows);
    std::vector<int> colIdxs = std::vector<int>(numNonZeros);
    std::vector<double> colCoeffs = std::vector<double>(numNonZeros);
    
    int count = 0, surplus_p;
    status = CPXgetrows(env, lp, &count, rowStart.data(), colIdxs.data(), colCoeffs.data(),
    numNonZeros, &surplus_p, 0, numRows - 1);
    
    std::vector<int> cmatbeg = std::vector<int>(numCols);
	std::vector<int> cmatind = std::vector<int>(numNonZeros);
	std::vector<double> cmatval = std::vector<double>(numNonZeros);
    status = CPXgetcols(env, lp, &count, cmatbeg.data(), cmatind.data(), cmatval.data(), 
    numNonZeros, &surplus_p, 0, numCols - 1);


    status = CPXchgprobtype(env, lp, CPXPROB_LP); // problem type: CPXPROB_MILP, CPXPROB_LP
    status = CPXlpopt(env, lp);

    status = CPXreadcopyprob(env, lp, "test.lp", NULL);

    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);
    return 0;
}