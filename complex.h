#ifndef COMPLEX_H

#define COMPLEX_H

class Complex {
    double re;
    double im;
public: 
    // constructors
    Complex();
    Complex(double r, double i);
    Complex(const Complex& other);

    // getters
    double real() const;
    double imag() const;

    // setters
    void setReal(double real);
    void setImag(double imag);

    //operations
    double abs() const;        // absolute value of z    // |z|
    double norm() const;       // absolute value squared // |z|^2
    double arg() const;        // argument of z          // arg(z))
    Complex conjugate() const; // conjugate of z         // ~z
    Complex exp() const;       // exponential if z (e^z) // e^z
};

// arithmetic operations (x = real, z = complex)
Complex operator+(const Complex& z1, const Complex& z2); // z + z
Complex operator-(const Complex& z1, const Complex& z2); // z - z
Complex operator*(const Complex& z1, const Complex& z2); // z * z
Complex operator/(const Complex& z1, const Complex& z2); // z / z
Complex operator+(const Complex& z, double x);           // z + x
Complex operator+(double x, const Complex& z);           // x + z
Complex operator-(const Complex& z, double x);           // z - x
Complex operator-(double x, const Complex& z);           // x - z
Complex operator*(const Complex& z, double x);           // z * x
Complex operator*(double x, const Complex& z);           // x * z
Complex operator/(const Complex& z, double x);           // z / x
Complex operator/(double x, const Complex& z);           // x / z

#endif