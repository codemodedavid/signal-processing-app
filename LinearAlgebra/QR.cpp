#include "QR.h"
#include "matrix.h"
#include <cmath>

QR::QR(const Matrix& A) {
    long m = A.getRows();
    long n = A.getColumns();

    Q = Matrix(m, n);
    R = Matrix(n, n);

    for(long column_index=0; column_index<n; column_index++) {
        Matrix v = A.getColumn(column_index);

        for(int row_index=0; row_index<column_index; row_index++) {
            Matrix q = Q.getColumn(row_index);
            R[row_index][column_index] = dot(q, v);
            v -= q * R[row_index][column_index];
        }

        R[column_index][column_index] = norm(v);

        Matrix q = v / R[column_index][column_index];

        Q.setColumn(column_index, q);
    }
}

const Matrix& QR::getQ() const {
    return Q;
}
const Matrix& QR::getR() const {
    return R;
}

float QR::dot(const Matrix& a, const Matrix& b) const {
    if(a.getRows() != b.getRows() || a.getColumns() != 1 || b.getColumns() != 1 ) {
        return 0.0f;
    }
    float result = 0.0f;

    for(long row_index=0;row_index<a.getRows(); row_index++) {
        result += a[row_index][0] * b[row_index][0];
    }
    return result;
}

float QR::norm(const Matrix& a) const {
    if(a.getColumns() != 1) {
        return 0.0f;
    }
    float result = 0.0f;

    for(long row_index=0; row_index<a.getRows(); row_index++) {
        result += a[row_index][0] * a[row_index][0];
    }
    return std::sqrt(result);
}

Matrix QR::solve(const Matrix& sol) const {
    long m = Q.getRows();
    long n = Q.getColumns();

    if(sol.getColumns() != 1 || sol.getRows() != m) {
        return Matrix();
    }
    Matrix b = Q.transpose() * sol;

    Matrix x(n, 1);

    for(long column_index=n-1; column_index>=0; column_index--) {
        float sum = 0.0f;

        for(long row_index = column_index + 1; row_index < n; row_index++)
        {
            sum += R[column_index][row_index] * x[row_index][0];
        }
        if(R[column_index][column_index] == 0.0f) {
            return Matrix();
        }
        x[column_index][0] = (b[column_index][0] - sum) / R[column_index][column_index];
    }
    return x;
}