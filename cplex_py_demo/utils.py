def get_csr_matrix(model):
    """
    获取CPLEX模型的约束矩阵的CSR形式。

    参数:
    - model: 一个CPLEX模型对象。

    返回值:
    - row_ptr: 行指针数组，表示每行（每个约束）的起始位置。
    - col_ind: 列索引数组，表示每个非零系数所在的列（变量索引）。
    - val: 非零系数值数组，表示每个非零系数的值。
    """
    # 获取所有线性约束的系数
    rows = model.linear_constraints.get_rows()

    # 初始化CSR格式的组件
    row_ptr = [0]
    col_ind = []
    col_val = []

    # 填充CSR格式的组件
    for row in rows:
        col_ind.extend(row.ind)
        col_val.extend(row.val)
        row_ptr.append(len(col_ind))
    return row_ptr, col_ind, col_val
    