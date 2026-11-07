#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
private:
    long rows;
    long columns;
    long elements;

    float* data;
public:
    Matrix();
    Matrix(long rows, long columns);
    ~Matrix();
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    float* operator[](long row);
    const float* operator[](long row) const;

    Matrix operator+(const Matrix& other) const;
    Matrix& operator+=(const Matrix& other);

    Matrix operator-(const Matrix& other) const;
    Matrix& operator-=(const Matrix& other);

    Matrix operator*(const Matrix& other) const;
    Matrix& operator*=(const Matrix& other);
    Matrix operator*(float scalar) const;
    Matrix& operator*=(float scalar);
    
    Matrix operator/(float scalar) const;
    Matrix& operator/=(float scalar);

    bool isSquare() const;
    bool isEmpty() const;

    long getRows() const;
    long getColumns() const;

    Matrix transpose() const;
};

#endif