#include <stdexcept>
#include <cmath>
#include "complex.h"
Complex::Complex() : re(0.0), im(0.0) {

};

Complex::Complex(double r, double i) : re(r), im(i) {

};

Complex::Complex(const Complex& other) : re(other.re), im(other.im) {

};


double Complex::real() const {
    return re;
};

double Complex::imag() const {
    return im;
};

void Complex::setReal(double real) {
    re = real;
};

void Complex::setImag(double imag) {
    im = imag;
};

double Complex::abs() const {
    return std::sqrt(re * re + im * im);
};

double Complex::norm() const {
    return re * re + im* im;
};

double Complex::arg() const {
    return atan2(im, re);
};

Complex Complex::conjugate() const {
    return Complex(re, -im);
};

Complex Complex::exp() const {
    return Complex(std::exp(re) * cos(im), std::exp(re) * sin(im));
};

Complex operator+(const Complex& z1, const Complex& z2) {
    return Complex(z1.real() + z2.real(), z1.imag() + z2.imag());
};

Complex operator-(const Complex& z1, const Complex& z2) {
    return Complex(z1.real() - z2.real(), z1.imag() - z2.imag());
};

Complex operator*(const Complex& z1, const Complex& z2) {
    return Complex(z1.real() * z2.real() - z1.imag() * z2.imag(), z1.real() * z2.imag() + z2.real() * z1.imag());
};

Complex operator/(const Complex& z1, const Complex& z2) {
    double denom = z2.real() * z2.real() + z2.imag() * z2.imag();
    if(denom == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return Complex((z1.real() * z2.real() + z1.imag() * z2.imag()) / denom, (z2.real() * z1.imag() - z1.real() * z2.imag()) / denom);
};

Complex operator+(const Complex& z, double x) {
    return Complex(x + z.real(), z.imag());
};

Complex operator+(double x, const Complex& z) {
    return Complex(x + z.real(), z.imag());
};

Complex operator-(const Complex& z, double x) {
    return Complex(z.real() - x, z.imag());
};

Complex operator-(double x, const Complex& z) {
    return Complex(x - z.real(), z.imag());
};

Complex operator*(const Complex& z, double x) {
    return Complex(x * z.real(), x * z.imag());
};

Complex operator*(double x, const Complex& z) {
    return Complex(x * z.real(), x * z.imag());
};

Complex operator/(const Complex& z, double x) {
    if(x == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return Complex(z.real() / x, z.imag() / x);
};

Complex operator/(double x, const Complex& z) {
    double denom = z.real() * z.real() + z.imag() * z.imag();
    if(denom == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return Complex(x * z.real() / denom, -x * z.imag() / denom);
};