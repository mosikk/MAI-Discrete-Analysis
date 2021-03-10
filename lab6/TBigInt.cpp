#include "TBigInt.hpp"

TBigInt ZERO(0);
TBigInt ONE(1);
TBigInt TWO(2);

TBigInt::TBigInt(int value) {
    if (value == 0) {
        digits.push_back(0);
    } else {
        for (int i = value; i > 0; i /= BASE) {
            digits.push_back(i % BASE);
        }
    }
}

TBigInt::TBigInt(const std::string &value) {
    for (int i = value.length() - 1; i >= 0; i -= DIGIT_LENGTH) {
        int curDigit = 0;
        int curDegree = 1;
        for (int j = 0; j < DIGIT_LENGTH && i >= j; ++j) {
            curDigit += (value[i - j] - '0') * curDegree;
            curDegree *= 10;
        }
        digits.push_back(curDigit);
    }
    RemoveZeroes();
}

void TBigInt::RemoveZeroes() {
    int id = GetNumSize() - 1;
    while (id > 0 && digits[id] == 0) {
        digits.pop_back();
        --id;
    }
}

int &TBigInt::operator[](int id) {
    if (id < 0 || id >= GetNumSize()) {
        throw std::logic_error("Wrong id");
    }
    return digits[id];
}

int TBigInt::GetNumSize() const {
    return digits.size();
}

void TBigInt::Resize(int size) {
    digits.resize(size);
}

int TBigInt::GetDigit(int id) const {
    if (id >= GetNumSize()) {
        return 0;
    }
    if (id < 0) {
        throw std::logic_error("Wrong id");
    }
    return digits[id];
}

TBigInt TBigInt::operator+(const TBigInt &other) const {
    int newNumSize = std::max(GetNumSize(), other.GetNumSize()) + 1;
    TBigInt sum;
    sum.Resize(newNumSize);

    int carry = 0;
    for (int i = 0; i < newNumSize - 1; ++i) {
        sum[i] = GetDigit(i) + other.GetDigit(i) + carry;
        carry = sum[i] / BASE;
        sum[i] %= BASE;
    }
    if (carry > 0) {
        sum[sum.GetNumSize() - 1] = carry;
    }
    sum.RemoveZeroes();
    return sum;
}

TBigInt TBigInt::operator-(const TBigInt &other) const {
    int newNumSize = GetNumSize();
    TBigInt diff;
    diff.Resize(newNumSize);

    int carry = 0;
    for (int i = 0; i < newNumSize; ++i) {
        diff[i] = GetDigit(i) - other.GetDigit(i) - carry;
        carry = (diff[i] < 0);
        if (carry) {
            diff[i] += BASE;
        }
    }
    diff.RemoveZeroes();
    return diff;
}

TBigInt TBigInt::operator*(const TBigInt &other) const {
    int newNumSize = GetNumSize() + other.GetNumSize();
    TBigInt prod;
    prod.Resize(newNumSize);

    for (int j = 0; j < other.GetNumSize(); ++j) {
        if (other.digits[j] == 0) {
            continue;
        }
        int carry = 0;
        for (int i = 0; i < GetNumSize(); ++i) {
            int tmp = GetDigit(i) * other.GetDigit(j) + carry + prod.GetDigit(i + j);
            carry = tmp / BASE;
            prod[i + j] = tmp % BASE;
        }
        if (carry) {
            prod[j + GetNumSize()] = carry;
        }
    }
    prod.RemoveZeroes();
    return prod;
}

TBigInt TBigInt::operator/(const TBigInt &other) const {
    int newNumSize = GetNumSize();
    TBigInt quotient;
    quotient.Resize(newNumSize);
    TBigInt curNumber(0);

    for (int i = GetNumSize() - 1; i >= 0; --i) {
        curNumber.digits.insert(curNumber.digits.begin(), digits[i]);
        curNumber.RemoveZeroes();

        int curValue = 0;
        int left = 0;
        int right = BASE;
        while (left <= right) {
            int mid = (left + right) / 2;
            TBigInt tmp = TBigInt(mid) * other;
            if (!(tmp > curNumber)) {
                curValue = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        quotient[i] = curValue;
        curNumber = curNumber - other * TBigInt(curValue);
    }

    quotient.RemoveZeroes();
    return quotient;
}

TBigInt TBigInt::Pow(TBigInt& deg) {
    TBigInt res(1);
    while (deg > ZERO) {
        if (deg.GetDigit(0) % 2 == 1) {
            res = res * (*this);
            deg = deg - ONE;
        } else {
            (*this) = (*this) * (*this);
            deg = deg / TWO;
        }
    }
    return res;
}

bool TBigInt::operator<(TBigInt &other) const {
    if (GetNumSize() != other.GetNumSize()) {
        return GetNumSize() < other.GetNumSize();
    }

    for (int i = GetNumSize() - 1; i >= 0; --i) {
        if (digits[i] != other[i]) {
            return digits[i] < other[i];
        }
    }
    return false;
}

bool TBigInt::operator>(TBigInt &other) const {
    if (GetNumSize() != other.GetNumSize()) {
        return GetNumSize() > other.GetNumSize();
    }

    for (int i = GetNumSize() - 1; i >= 0; --i) {
        if (digits[i] != other[i]) {
            return digits[i] > other[i];
        }
    }
    return false;
}

bool TBigInt::operator==(TBigInt &other) const {
    if (GetNumSize() != other.GetNumSize()) {
        return false;
    }

    for (int i = 0; i < GetNumSize(); ++i) {
        if (digits[i] != other[i]) {
            return false;
        }
    }
    return true;
}

std::ostream &operator<<(std::ostream &out, TBigInt num) {
    if (num.GetNumSize() == 0) {
        out << 0;
    } else {
        out << num[num.GetNumSize() - 1];
        for (int i = num.GetNumSize() - 2; i >= 0; --i) {
            out << std::setfill('0') << std::setw(DIGIT_LENGTH) << num[i];
        }
    }
    return out;
}
