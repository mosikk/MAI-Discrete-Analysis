#ifndef DA_LAB6_TBIGINT_HPP
#define DA_LAB6_TBIGINT_HPP


#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

const int BASE = 10000;
const int DIGIT_LENGTH = ceil(log10(BASE));

class TBigInt {
private:
    std::vector<int> digits;

    void RemoveZeroes();

public:
    TBigInt(int value = 0);

    TBigInt(const std::string &value);

    int &operator[](int id);

    int GetDigit(int id) const;

    int GetNumSize() const;

    void Resize(int size);

    TBigInt operator+(const TBigInt &other) const;

    TBigInt operator-(const TBigInt &other) const;

    TBigInt operator*(const TBigInt &other) const;

    TBigInt operator/(const TBigInt &other) const;

    TBigInt Pow(TBigInt& other);

    bool operator<(TBigInt &other) const;

    bool operator>(TBigInt &other) const;

    bool operator==(TBigInt &other) const;

    friend std::ostream &operator<<(std::ostream &out, TBigInt num);

};


#endif //DA_LAB6_TBIGINT_HPP
