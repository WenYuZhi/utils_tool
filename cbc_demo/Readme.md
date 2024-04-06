### Introduction

We have already packaged the CBC solver into the third_party directory, so you don't need to install the CBC solver yourself. The code currently only supports Linux operating system and requires Cmake and GCC. It is not compatible with other operating systems at the moment.

### Complie

```
mkdir build
cd build
cmake ..
make 
```

### Run

```
./cbc_demo
```

### 0 CBC 求解器简介

CBC Solver (Coin-or Branch and Cut Solver) is an open-source linear and mixed-integer programming solver. It is based on the Coin-OR project and serves as a powerful mathematical optimization tool. CBC Solver offers a range of efficient algorithms and techniques for handling linear programming (LP) and mixed-integer programming (MIP) problems. It finds applications in diverse fields such as production scheduling, supply chain optimization, resource allocation, and more.

The primary reason for considering the use of CBC Solver over commercial solvers like Gurobi or CPLEX lies in its completely free and open-source nature, making it a cost-effective choice. From a security perspective, CBC Solver is relatively safer (with minimal risk of supply chain disruptions or bans), making it a preferred option for certain organizations. This is one of the reasons why some major companies frequently utilize CBC Solver. However, it's essential to acknowledge the drawbacks of CBC Solver:

Slow Solver Speed: CBC Solver's solving speed is notably slow. It is typically employed as a weak solver, suitable for solving linear programming or small- to medium-sized mixed-integer programming problems. For larger problem instances, its performance may be insufficient.

Documentation Quality: CBC Solver's documentation is considered subpar. Given that it's an open-source project, extensive documentation may not be as comprehensive as in commercial alternatives. Users often find themselves relying on reading the source code for development, leading to lower development efficiency and increased learning curve.

Limited Tutorials and Resources: There is a scarcity of tutorials and resources for CBC Solver compared to mainstream solvers like Gurobi and CPLEX. While these commercial solvers have abundant documentation, tutorials, and real-world application examples available online, CBC Solver lacks similar widespread coverage.

Considering these factors, this article aims to address the gap by providing a basic overview and demonstration of the commonly used CBC Solver API. This will serve as an initial development document, facilitating users in utilizing CBC Solver more effectively.

### 1 Add Variable

#### 1.1 Add and Remove Variable

```C++
// Add variables
  int numVar = 5;
  CoinPackedVector v0;
  for (int i = 0; i < numVar; i++)
      solver1.addCol(v0, 0, 1, 0.0);
```
CoinPackerVector is a data type defined by CBC Solver, primarily designed for storing vectors. Unlike common vectors, CoinPackerVector stores both the indices and values of the vector elements. For example, an array {2, 0, 5, 6, 0, 7} would be represented in CoinPackerVector as {(0, 2), (2, 5), (3, 6), (5, 7)}.

In the context of solver1.addCol(v0, 0, 1, 0.0);, where v0 represents the vector associated with the variable in constraints, 0 and 1 represent the lower and upper bounds of the variable, and 0.0 represents the coefficient of the variable in the objective function. Here, we do not specify in which constraints the variable appears. For all v0, an empty vector is sufficient because we will add constraints later.

Please note that this explanation assumes a certain context related to the usage of CBC Solver and the CoinPackerVector data type.

#### 1.2 Modify the Upper Bound and Lower Bound，Variable Type and Variable Name

```C++
  // Set var upper bound and lower bound
  for (int i = 0; i < numVar; i++){
     solver1.setColUpper(i, 1.0);
     solver1.setColLower(i, 0.0);
     solver1.setInteger(i); // set variable to be integer
     // solver1.setContinuous(i); // set variable to be ocntinuous
     solver1.setColName(i, "x" + std::to_string(i)); // set variable name
  }
```

Setting the upper and lower bounds for decision variables can be done either directly within solver1.addCol(v0, 0, 1, 0.0) or through the code provided above.

#### 1.3 Remove Variable 

```C++
   std::vector<int> colIdx = {0, 1};
   solver1.deleteCols(colIdx.size(), colIdx.data());
```

Delete the decision variables x0 and x1.

### 2 Add and Remove Constraints

#### 2.1 Add Constraints 

```C++
// Add constraint: 11*x0 + 20*x1 + 3*x2 <= 12
  CoinPackedVector v1;
  v1.insert(0, 11.0); // add x0 coefficient 11.0
  v1.insert(1, 20.0); // add x1 coefficient 20.0
  v1.insert(2, 3.0);  // add x2 coefficient 3.0
  char rowsen = 'L'; //  constrants type is equal or less than
  double rowrhs = 12.0; // right hand side of the constraint
  double rowrng = 0.0; // A relaxation degree of 0 indicates that the constraint is strict.
  solver1.addRow(v1, rowsen, rowrhs, rowrng);
```

The constraint types are as follows:

'L': <= constraint
'E': = constraint
'G': >= constraint
'R': ranged constraint
'N': free constraint

#### 2.2 Add Range Constraints

```C++
// Add constraint: 22 <= 0.5*x1 + x2 + 21.8*x3 <= DBL_MAX
  CoinPackedVector v2;
  double rowLowerBound = 22;  
  double rowUpperBound = DBL_MAX;
  v2.insert(1, 0.5);
  v2.insert(2, 1.0);
  v2.insert(3, 21.8);
  solver1.addRow(v2, rowLowerBound, rowUpperBound);
```


The difference with the above method is that this approach allows adding constraints in the form:
22 <= 0.5*x1 + x2 + 21.8*x3 <= DBL_MAX

#### 2.3 Remove Constraints

```C++
  std::vector<int> rowIdx = {0, 1};
  solver1.deleteRows(rowIdx.size(), rowIdx.data());
```

Delete the first and second constraints.

### 3 Set Objective Function

```C++
// Set objective function: x0+x1+x2+x3+x4
  for (int i = 0; i < numVar; i++)
      solver1.setObjCoeff(i, 1.0); // set objective function coefficient
```

```C++
 // Set objective function sense (1 for min (default), -1 for max,)
  solver1.setObjSense(1);
```

With this, the modeling part has been completed. Next, we will discuss solving and obtaining data-related aspects.

### 4 Set Parameter

```C++
  CbcModel model(solver1);
// Set the number of threads to use in parallel
  model.setNumberThreads(16);
  model.setLogLevel(1); // log level range from 0-3
  model.setMaximumSeconds(60.0); // set time limit
```

We first need to load solver1 into the model. Then, set the number of solving threads, log level, and the maximum solving time individually. The log level ranges from 0 to 3, where 0 indicates no log printing, 1 indicates printing regular logs, and 2 and 3 indicate printing the most logs. Users can choose the log level according to their needs.

```C++
 // set initial solution
  std::vector<std::pair< std::string, double > > initSol;
  for (int i = 0; i < numVar; i++)
      initSol.push_back(std::make_pair(data.colName[i], 0.0));
  model.setMIPStart(initSol);
```

If solving a mixed-integer linear programming problem, we can accelerate the solver's solution process by providing it with an initial solution. The function model.setMIPStart(initSol) is used to import the initial solution into the solver's API. initSol represents the initial solution, which is a dictionary where the keys are variable names, and the values are the initial values of the variables. We will explain how to obtain variable names (data.colName) later.

```C++
 // start branch and bound tree search
  model.branchAndBound();
  int status = model.status(); 
  /** Final status of problem
        -1 before branchAndBound
        0 finished - check isProvenOptimal or isProvenInfeasible to see if solution found
        (or check value of best solution)
        1 stopped - on maxnodes, maxsols, maxtime
        2 difficulties so run was abandoned
        (5 event user programmed event occurred)
    */
```

Enable the Branch and Bound search. After solving, you can obtain the solution status. We have provided detailed explanations of the status meanings in the comments.

### 5 Get Data

#### 5.1 Get Optimal Solution

```C++
if (model.isProvenOptimal())
  {
      const double *solution = model.bestSolution();
      for (int i = 0; i < numVar; i++) {
          double value = solution[i];
          std::cout << data.colName[i] << ":" << value << ", ";
      }
      std::cout << std::endl;
      std::cout << "Obj value: " << model.getObjValue() << std::endl; // get objecitve value
  }
```


After completing the solution, the most common operation is to obtain the optimal solution. First, use model.isProvenOptimal() to determine if the optimal solution has been found. If an optimal solution has been found, you can directly use model.bestSolution() to obtain the optimal solution.

#### 5.2 Get Model Info

```C++
    data.numRows = model.solver()->getNumRows(); // get the number of constraints
    data.numCols = model.solver()->getNumCols(); // get the number of vars
    data.numNonZeros = model.solver()->getNumElements(); // get the number of nonzeros elements
    data.objSense = model.getObjSense(); // get the objective sense (1 for min (default), -1 for max,)
```

Retrieve information about the dimensionality of decision variables, the number of constraints, the count of non-zero elements, and whether the objective function is maximized or minimized.

#### 5.3 Get Variable

```C++
/* col types:
       - 0 - continuous
       - 1 - binary
       - 2 - general integer
       - 3 - if supported - semi-continuous
       - 4 - if supported - semi-continuous integer
 */
data.varTypes = std::vector<char>(data.numCols);
for (int i = 0; i < data.numCols; i++)
    {
      if (model.solver()->isContinuous(i)) {
            data.varTypes[i] = 'C';
        } else if (model.solver()->isInteger(i)) {
            data.varTypes[i] = 'I';
        } else {
            std::cout << "Var " << i << " is neither continuous nor integer." << std::endl;
        }
        // model.solver()->isBinary(i)
        // model.solver()->isIntegerNonBinary(i)
    }

    const double *lb = model.solver()->getColLower();
    const double *ub = model.solver()->getColUpper();
    data.lb = std::vector<double>(data.numCols);
    data.ub = std::vector<double>(data.numCols);
    for(int i = 0; i < data.numCols; i++) {
        data.lb[i] = lb[i];
        data.ub[i] = ub[i];
    }
```

Retrieve variable types and obtain variable upper and lower bounds. Variable types generally fall into two categories: integer and continuous (although there can be further subcategories, we won't delve into more details here). The detailed variable types are already annotated in the comments, and users can choose accordingly based on their needs.

#### 5.4 Get Constraints

```C++
const char* rowtypes = model.solver()->getRowSense();
data.rowtypes = std::vector<char>(data.numRows);
for(int i = 0; i < data.numRows; i++) data.rowtypes[i] = rowtypes[i];

const double* rhs = model.solver()->getRightHandSide();
data.rhs = std::vector<double>(data.numRows);
for(int i = 0; i < data.numRows; i++) data.rhs[i] = rhs[i];
```

```C++
// get CSR matrix
    const CoinPackedMatrix * matrixByRow = model.solver()->getMatrixByRow();
    const double *colCoeffs = matrixByRow->getElements();
    const int *colIdxs = matrixByRow->getIndices();
    const int *rowStart = matrixByRow->getVectorStarts();
    // data.colCoeffs[data.rowStart[i]:data.rowStart[i+1]] means the nonzero elements of row i
    // data.colIdxs[data.rowStart[i]:data.rowStart[i+1]] means the column index of the nonzero elements of row i
    data.colCoeffs = std::vector<double>(data.numNonZeros); // coefficient of each nonzero element
    data.rowStart = std::vector<int>(data.numRows + 1); // start index of each row
    data.colIdxs = std::vector<int>(data.numNonZeros); // column index of each nonzero element

    for (int i = 0; i < data.numNonZeros; i++) {
        data.colCoeffs[i] = colCoeffs[i];
        data.colIdxs[i] = colIdxs[i];
    }

    for (int i = 0; i < data.numNonZeros + 1; i++) 
        data.rowStart[i] = rowStart[i];
```

This segment of code is a bit more complex, primarily to obtain the constraint matrix. Due to the size of the constraint matrix being 
$m \times n$, where m is the number of constraints and n is the dimensionality of decision variables, the scale is enormous. If we use the conventional matrix storage method, it would occupy a significant amount of memory, leading to potential memory errors.

Upon observation, it's apparent that the constraint matrix is typically very sparse, with numerous zero elements consuming substantial memory space. Therefore, in solvers, we generally adopt a compressed matrix approach to store the constraint matrix.

那么其中一种常用的压缩矩阵方式就是 Compressed Sparse Row （CSR）也叫做行压缩矩阵。行压缩矩阵用三个 vector 来存储矩阵：

1. colCoeffs，用来存储矩阵中的非零元素的值；
2. colIdxs，第i个元素记录了V[i]元素的列数；
3. rowStart, 第i个元素记录了前i-1行包含的非零元素的数量。

如果我们想要表示这样一个约束矩阵：

$$
\left( \begin{matrix}
	10&		3&		0&		0&		0&		0\\
	0&		30&		0&		40&		0&		0\\
	0&		0&		50&		60&		5&		0\\
	0&		0&		0&		0&		0&		12\\
\end{matrix} \right)
$$

colCoeffs: $\left[ 10,3,30,40,50,60,5,12 \right]$

colIdxs: $\left[ 0,1,1,3,2,3,4,5 \right]$

rowStart: $\left[ 0,2,4,7,8 \right]$

如何访问某一行的元素，我们在代码注释里边添加了，这里就不再赘述了。更多关于行压缩矩阵的内容可以参考：

https://zhuanlan.zhihu.com/p/623366713

#### 5.5 Get Variable Name and Constraints Name

```C++
for (int i = 0; i < data.numCols; i++)
       data.colName.push_back(model.solver()->getColName(i));
for (int i = 0; i < data.numRows; i++)
       data.rowName.push_back(model.solver()->getRowName(i));
```

#### 5.6 Get Objective Coeff

```C++
 const double* objCoeffs = model.solver()->getObjCoefficients();
 data.objCoeffs = std::vector<double>(data.numCols);
 for(int i = 0; i < data.numCols; i++) 
     data.objCoeffs[i] = objCoeffs[i];
```

### 7 Read and Write MPS File

```C++
// Read the mps file
int numMpsReadErrors = solver1.readMps(argv[1], "");
assert(numMpsReadErrors == 0);
// Write MPS file
solver1.writeMps("./new_model.mps", "");
```

#### References：

【1】https://coin-or.github.io/Cbc/Doxygen/annotated.html

【2】https://coin-or.github.io/Cbc/

【3】https://github.com/coin-or/Cbc
