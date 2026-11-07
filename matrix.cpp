#include "matrix.h"

Matrix::Matrix() : rows(0), columns(0), elements(0), data(nullptr) {

}

Matrix::Matrix(long rows, long columns) : rows(rows), columns(columns) {
    elements = rows * columns;
    data = new float[elements]();
}

Matrix::~Matrix() {
    delete[] data;
}

Matrix::Matrix(const Matrix& other) {
    rows = other.rows;
    columns = other.columns;
    elements = other.elements;
    data = new float[elements];
    for(long element_index=0; element_index<elements; element_index++) {
        data[element_index] = other.data[element_index];
    }
}

Matrix& Matrix::operator=(const Matrix& other) {
    if(this == &other) {
        return *this;
    }
    delete[] data;

    rows = other.rows;
    columns = other.columns;
    elements = other.elements;
    data = new float[elements];
    for(int element_index=0; element_index<elements; element_index++) {
        data[element_index] = other.data[element_index];
    }
    return *this;
}

float* Matrix::operator[](long row) {
    return &data[row * columns];
}

const float* Matrix::operator[](long row) const {
    return &data[row * columns];
}

Matrix Matrix::operator+(const Matrix& other) const {
    if(rows != other.rows || columns != other.columns) {
        return Matrix();
    }
    Matrix result = *this;
    for(long element_index=0; element_index<elements; element_index++) {
        result.data[element_index] += other.data[element_index];
    }
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if(rows != other.rows || columns != other.columns) {
        return *this;
    }
    for(long element_index=0; element_index<elements; element_index++) {
        data[element_index] += other.data[element_index];
    }
    return *this;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if(rows != other.rows || columns != other.columns) {
        return Matrix();
    }
    Matrix result = *this;
    for(long element_index=0; element_index<elements; element_index++) {
        result.data[element_index] -= other.data[element_index];
    }
    return result;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if(rows != other.rows || columns != other.columns) {
        return *this;
    }
    for(long element_index=0; element_index<elements; element_index++) {
        data[element_index] -= other.data[element_index];
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if(columns != other.rows) {
        return Matrix();
    }
    Matrix result(rows, other.columns);
    for(long row_index=0; row_index<rows; row_index++) {
        for(long column_index=0; column_index<other.columns; column_index++) {
            //result[row_index][column_index] = 0.0f;
            for (long k = 0; k < columns; k++) {
                result[row_index][column_index] += (*this)[row_index][k] * other[k][column_index];
            }
        }
    }
    return result;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if(this == &other) {
        return *this;
    }
    if(columns != other.rows) {
        *this = Matrix();
        return *this;
    }
    Matrix result(rows, other.columns);
    for(long row_index = 0; row_index < rows; row_index++) {
        for(long column_index = 0; column_index < other.columns; column_index++) {
            for(long k = 0; k < columns; k++) {
                result[row_index][column_index] +=
                    (*this)[row_index][k] * other[k][column_index];
            }
        }
    }
    *this = result;
    return *this;
}

Matrix Matrix::operator*(float scalar) const {
    if(scalar == 0) {
        return Matrix(rows, columns);
    }
    Matrix result= *this;
    for(long element_index=0; element_index<elements; element_index++) {
        result.data[element_index] *= scalar;
    }
    return result;
}

Matrix& Matrix::operator*=(float scalar) {
    for(long element_index=0; element_index<elements; element_index++) {
        data[element_index] *= scalar;
    }
    return *this;
}

Matrix Matrix::operator/(float scalar) const {
    if(scalar == 0) {
        return Matrix();
    }
    Matrix result= *this;
    for(long element_index=0; element_index<elements; element_index++) {
        result.data[element_index] /= scalar;
    }
    return result;
}

Matrix& Matrix::operator/=(float scalar) {
    if(scalar == 0) {
        *this = Matrix();
        return *this;
    }
    for(long element_index=0; element_index<elements; element_index++) {
        data[element_index] /= scalar;
    }
    return *this;
}

bool Matrix::isSquare() const {
    return rows == columns;
}

bool Matrix::isEmpty() const {
    return data == nullptr;
}

Matrix Matrix::transpose() const {
    Matrix result(columns, rows);
    for(long row_index=0; row_index<rows; row_index++) {
        for(long column_index=0; column_index<columns; column_index++) {
            result[column_index][row_index] = (*this)[row_index][column_index];
        }
    }
    return result;
}