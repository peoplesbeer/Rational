#pragma once

#include <iostream>
#include "IntTraits.h"
using namespace std;

/*
 * Class representation of a rational number. This rational number will always 
 * be simplified as far as possible, will always indicate sign on the numerator
 * and will always represent 0 as 0/1. For example 2/-8 will be represented as 
 * -1/4 and 0/-128 will be represented as 0/1. Calculations are performed in
 * a larger type (see IntTraits.h).
 */
template <typename T>
class Rational {
  /* Defines NextType as a bigger integer type than T or as T if no bigger type
     was found */
  typedef typename IntTraits::NextType<T>::Type NextType;

  /* Trying to be nice to our friends: */
  friend Rational<T> operator- <T>(const Rational<T>&);

private:
  T numerator, denominator;

  /* Divide by greatest common divisor and makes sure sign is indicated on 
   * numerator.
   */
  void Simplify();
public:
  /* Constructors. */
  Rational(T numerator = 0, T denominator = 1):
    numerator(numerator), denominator(denominator) { Simplify(); }

  //Rational(int numerator): Rational(numerator) { }

  template <typename U>
  Rational(const Rational<U>& r): 
    numerator(r.Numerator()), denominator(r.Denominator()) { }

  /* Sets the numerator and denominator of this Rational */
  Rational& Set(T numerator, T denominator);

  /* Overloaded compound-assignment operators */
  Rational& operator+=(const Rational& right) {
    return *this = Rational<NextType>(
      right.denominator * numerator + denominator * right.numerator,
      denominator * right.denominator);
  }
  Rational& operator-=(const Rational& right) {
    return *this =  Rational<NextType>(
      right.denominator * numerator - denominator * right.numerator,
      denominator * right.denominator);
  }
  Rational& operator*=(const Rational& right) {
    return *this =  Rational<NextType>(
      numerator * right.numerator, denominator * right.denominator);
  }
  Rational& operator/=(const Rational& right) {
    return *this = Rational<NextType>(
      right.denominator * numerator, right.numerator * denominator);
  }

  /* Note: Arithmetic and relational operators are overloaded as non-members. */

  /* Postfix increment */
  Rational operator++(int a) { 
    Rational<T> rval(*this);
    Set(numerator + denominator, denominator);
    return rval;
  }
  /* Postfix decrement */
  Rational operator--(int a) { 
    Rational<T> rval(*this);
    Set(numerator - denominator, denominator); 
    return rval;
  }
  /* Prefix increment */
  Rational& operator++() { return Set(numerator + denominator, denominator); }
  /* Prefix decrement */
  Rational& operator--() { return Set(numerator - denominator, denominator); }

  T Numerator() const { return numerator; }
  T Denominator() const { return denominator; }

  explicit operator int() const { return numerator / denominator; }
  explicit operator double() const { return ((double) numerator) / denominator; }
};

/*
 * Returns the greatest common devisor (Euclidean algorithm).
 */
template <typename T>
T Gcd(T numerator, T denominator) {
  T temp;
  while (denominator != 0) {
    temp = denominator;
    denominator = numerator % denominator;
    numerator = temp;
  }
  return numerator;
}

template <typename T>
void Rational<T>::Simplify() {
  // Always represent 0 as 0/1
  if (numerator == 0) {
    denominator = 1;
    return;
  }

  // Divide by greatest common divisor
  T gcd = Gcd(numerator, denominator);
  numerator /= gcd;
  denominator /= gcd;

  // Indicate sign on numerator only
  if (denominator < 0) {
    numerator = -numerator;
    denominator = -denominator;
  }
}

template <typename T>
Rational<T>& Rational<T>::Set(T numerator, T denominator) {
  this->numerator = numerator;
  this->denominator = denominator;
  Simplify();
  return *this;
}

// Overloaded arithmetic operators
// ===============================

// +

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator+(const Rational<T>& left, const Rational<U>& right) {
  Rational<typename IntTraits::LargestType<T, U>::Type> sum = left;
  sum += right;
  return sum;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator+(const Rational<T>& left, const U& right) {
  Rational<IntTraits::LargestType<T, U>::Type> sum = left;
  return sum += right;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator+(const U& left, const Rational<T>& right) {
  return right + left;
}

// -

template <typename T>
Rational<T> operator-(const Rational<T>& in) {
  Rational<T> out = in;
  out.numerator = -out.numerator;
  return out;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator-(const Rational<T>& left, const Rational<U>& right) {
  Rational<typename IntTraits::LargestType<T, U>::Type> sum = left;
  sum -= right;
  return sum;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator-(const Rational<T>& left, const U& right) {
  Rational<IntTraits::LargestType<T, U>::Type> sum = left;
  return sum -= right;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator-(const U& left, const Rational<T>& right) {
  return (-right) + left;
}

// *

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator*(const Rational<T>& left, const Rational<U>& right) {
  Rational<IntTraits::LargestType<T, U>::Type> product = left;
  return product *= right;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator*(const Rational<T>& left, const U& right) {
  Rational<IntTraits::LargestType<T, U>::Type> product = left;
  return product *= right;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator*(const U& left, const Rational<T>& right) {
  return right * left;
}

// /

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator/(const Rational<T>& left, const Rational<U>& right) {
  Rational<IntTraits::LargestType<T, U>::Type> quotient = left;
  return quotient /= right;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator/(const Rational<T>& left, const U& right) {
  Rational<IntTraits::LargestType<T, U>::Type> quotient = left;
  return quotient /= right;
}

template <typename T, typename U>
Rational<typename IntTraits::LargestType<T, U>::Type>
operator/(const U& left, const Rational<T>& right) {
  Rational<IntTraits::LargestType<T, U>::Type> quotient = left;
  return quotient /= right;
}

// Overloaded relational operators
// ===============================

// ==

template <typename T, typename U>
bool operator==(const Rational<T>& left, const Rational<U>& right) {
  return left.Numerator() == right.Numerator() && left.Denominator() == right.Denominator();
}

template <typename T, typename U>
bool operator==(const Rational<T>& left, const U& right) {
  return left.Denominator() == 1 && left.Numerator() == right;
}

template <typename T, typename U>
bool operator==(const U& left, const Rational<T>& right) {
  return right == left;
}

// !=

template <typename T, typename U>
bool operator!=(const Rational<T>& left, const Rational<U>& right) {
  return !(left == right);
}

template <typename T, typename U>
bool operator!=(const Rational<T>& left, const U& right) {
  return !(left == right);
}

template <typename T, typename U>
bool operator!=(const T& left, const Rational<U>& right) {
  return !(right == left);
}

// <

template <typename T, typename U>
bool operator<(const Rational<T>& left, const Rational<U>& right) {
  return (right - left).Numerator() > 0;
}

template <typename T, typename U>
bool operator<(const Rational<T>& left, const U& right) {
  return left < Rational<U>(right);
}

template <typename T, typename U>
bool operator<(const T& left, const Rational<U>& right) {
  return Rational<T>(left) < right;
}

// >

template <typename T, typename U>
bool operator>(const Rational<T>& left, const Rational<U>& right) {
  return (left - right).Numerator() > 0;
}

template <typename T, typename U>
bool operator>(const Rational<T>& left, const U& right) {
  return left > Rational<U>(right);
}

template <typename T, typename U>
bool operator>(const T& left, const Rational<U>& right) {
  return Rational<T>(left) > right;
}

// <=

template <typename T, typename U>
bool operator<=(const Rational<T>& left, const Rational<U>& right) {
  return !(left > right);
}

template <typename T, typename U>
bool operator<=(const Rational<T>& left, const U& right) {
  return left <= Rational<U>(right);
}

template <typename T, typename U>
bool operator<=(const T& left, const Rational<U>& right) {
  return Rational<T>(left) <= right;
}

// >=

template <typename T, typename U>
bool operator>=(const Rational<T>& left, const Rational<U>& right) {
  return !(left < right);
}

template <typename T, typename U>
bool operator>=(const Rational<T>& left, const U& right) {
  return left >= Rational<U>(right);
}

template <typename T, typename U>
bool operator>=(const T& left, const Rational<U>& right) {
  return Rational<T>(left) >= right;
}


// Overloaded stream operator
// ==========================

template <typename T>
ostream& operator<<(ostream& os, const Rational<T>& r) {
  return os << r.Numerator() << "/" << r.Denominator();
}

template <typename T>
istream& operator>>(istream& is, Rational<T>& item) {
  T numerator, denominator;
  is >> numerator;
  is.ignore(1);
  is >> denominator;

  item.Set(numerator, denominator);

  if (!is)
    item = Rational<T>(); // Input failed: give the object the default state
	
  return is;
}
