#ifndef QR_H
#define QR_H

#include "matrix.h"

class QR {
private:
    Matrix Q;
    Matrix R;
public:
    QR(const Matrix& A);

    const Matrix& getQ() const;
    const Matrix& getR() const;

    float dot(const Matrix& a, const Matrix& b) const;
    float norm(const Matrix& a) const;

    Matrix solve(const Matrix& d) const;
};

#endif