#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
using namespace std;
static int charToInt(char c)  {
    switch (c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default:
            string message("[BigNumber] It's not a number. Received: ");
            message.push_back(c);
            throw invalid_argument(message);
    }
}

class BigNumber {

    public:

        BigNumber(string string)  {
            if (string.length() > 0) {
                if (string.at(0) == '-' || string.at(0) == '+') {
                    this->m_positive = string.at(0) == '+';
                    string = string.substr(1);
                }
                this->m_values.reserve(string.length());
                for (int i = string.size() - 1; i >= 0; i--) {
                    this->m_values.push_back(charToInt(string[i]));
                }
            } else {
                this->m_values.push_back(0);
            }
            this->removeLeftZeros();
        }

        BigNumber() : BigNumber("0") {}

        BigNumber(long long value) : BigNumber(to_string(value)) {}


        BigNumber& operator=(const BigNumber& number)  {
            this->m_values = number.m_values;
            this->m_positive = number.m_positive;
            return *this;
        }

        BigNumber operator-() const  {
            BigNumber number = *this;
            if (!number.isZero()) {
                number.m_positive = !number.isPositive();
            }
            return number;
        }

        BigNumber operator+(const BigNumber& number) const  {
            if (this->isPositive() && !number.isPositive()) {
                return *this - number.absoluteValue();
            } else if (!this->isPositive() && number.isPositive()) {
                return (number - this->absoluteValue());
            }

            if (number.isZero()) {
                return *this;
            }
            if (this->isZero()) {
                return number;
            }

            BigNumber result = *this;

            for (int i = 0; i < number.lenght(); i++) {
                int digit = number.m_values[i];
                if (i < result.lenght()) {
                    result.m_values[i] += digit;
                } else {
                    result.m_values.push_back(digit);
                }
            }
            result.doCarryOver();
            result.afterOperation();
            return result;
        }


        BigNumber operator-(const BigNumber& number) const  {
            if (this->isPositive() && !number.isPositive()) {
                return *this - number.absoluteValue();
            } else if (!this->isPositive() && number.isPositive()) {
                return -(number + this->absoluteValue());
            } else if(!this->isPositive() && !number.isPositive()){
                return -(this->absoluteValue() + number.absoluteValue());
            }

            if (number.isZero()) {
                return *this;
            }
            if (this->isZero()) {
                return number;
            }

            BigNumber result;
            const BigNumber* smaller;
            if (this->lenght() >= number.lenght()) {
                result = *this;
                smaller = &number;
            } else {
                result = number;
                smaller = this;
            }

            for (int i = 0; i < smaller->lenght(); i++) {
                int dif = result.m_values[i] - smaller->m_values[i];
                if (dif < 0) {
                    for (int j = i + 1; j < result.lenght(); j++) {
                        if (result.m_values[j] == 0) {
                            result.m_values[j] = 9;
                        } else {
                            dif += 10;
                            result.m_values[j]--;
                            break;
                        }
                    }
                }

                result.m_values[i] = dif;
            }
            result.m_positive = *this >= number;
            result.afterOperation();
            return result;
        }


        BigNumber operator*(const BigNumber& number) const  {
            if (this->isZero() || number.isZero()) {
                return BigNumber(0);
            }
            if (this->isOne()) {
                return number;
            }
            if(number.isOne()) {
                return *this;
            }

            if (this->lenght() < 10 && number.lenght() < 10) {
                return BigNumber(this->asLongLong() * number.asLongLong());
            }

            int maxLenght = max(this->lenght(), number.lenght());
            int splitPoint = maxLenght / 2;

            pair<BigNumber, BigNumber> splitThis = this->splitAt(splitPoint);
            pair<BigNumber, BigNumber> splitNumber = number.splitAt(splitPoint);

            BigNumber secondProduct = splitThis.second * splitNumber.second;
            BigNumber firstProduct = splitThis.first * splitNumber.first;
            BigNumber sumProduct = (splitThis.second + splitThis.first) * (splitNumber.second + splitNumber.first);

            BigNumber firstPadded = firstProduct.times10(splitPoint * 2);
            BigNumber deltaPadded = (sumProduct - firstProduct - secondProduct).times10(splitPoint);

            return firstPadded + deltaPadded + secondProduct;
        }

        BigNumber operator/(const BigNumber& number) const {
            return this->divide(number).first;
        }

        BigNumber operator%(const BigNumber& number) const {
            return this->divide(number).second;
        }

        BigNumber& operator+=(const BigNumber& number)  {
            *this = *this + number;
            return *this;
        }

        BigNumber& operator-=(const BigNumber& number)  {
            *this = *this - number;
            return *this;
        }

        BigNumber& operator*=(const BigNumber& number)  {
            *this = *this * number;
            return *this;
        }

        BigNumber& operator/=(const BigNumber& number)  {
            *this = *this / number;
            return *this;
        }

        BigNumber& operator++()  {
            *this += 1;
            return *this;
        }

        BigNumber& operator--()  {
            *this -= 1;
            return *this;
        }

        BigNumber operator++(int)  {
            BigNumber copy = *this;
            *this += 1;
            return copy;
        }

        BigNumber operator--(int)  {
            BigNumber copy = *this;
            *this -= 1;
            return copy;
        }

        bool operator<(const BigNumber& number) const  {
            if (this->m_positive != number.m_positive) {
                return !this->m_positive;
            }
            if (this->lenght() != number.lenght()) {
                return this->lenght() < number.lenght();
            }
            // esli oba ravni po dline
            if (this->m_positive) {
                return this->asString() < number.asString();
            }
            return -(*this) > -number;
        }

        bool operator>(const BigNumber& number) const  {
            return *this >= number && !(*this == number);
        }

        bool operator<=(const BigNumber& number) const  {
            return *this == number || *this < number;
        }

        bool operator>=(const BigNumber& number) const  {
            return !(*this < number);
        }

        bool operator==(const BigNumber& number) const  {
            return this->m_positive == number.m_positive && this->m_values == number.m_values;
        }

        bool operator!=(const BigNumber& number) const  {
            return !(*this == number);
        }

        friend ostream& operator<<(ostream& stream, BigNumber const& number) {
            stream << number.asString();
            return stream;
        }

        friend istream& operator>>(istream& istream, BigNumber& number) {
            string in;
            istream >> in;
            number = BigNumber(in);
            return istream;
        }

        string asString() const  {
            stringstream ss;
            if (!this->isPositive()) {
                ss << '-';
            }
            for (int i = this->lenght() - 1; i >= 0; i--) {
                ss << this->m_values[i];
            }
            return ss.str();
        }

        BigNumber absoluteValue() const  {
            BigNumber temp = *this;
            temp.m_positive = true;
            return temp;
        }

        BigNumber times10(int times = 1) const  {
            BigNumber temp = *this;
            for (int i = 0; i < times; i++) {
                temp.m_values.insert(temp.m_values.begin(), 0);
            }
            temp.afterOperation();
            return temp;
        }

        pair<BigNumber, BigNumber> splitAt(long long splitPos) const  {
            splitPos = min(max(splitPos, (long long) 0), (long long) this->lenght());

            vector<int> firstHalf;
            firstHalf.reserve(splitPos);

            vector<int> secondHalf;
            secondHalf.reserve(this->lenght() - splitPos);

            for (size_t i = 0; i < this->lenght(); i++) {
                int digit = this->m_values[i];
                if (i > splitPos - 1) {
                    firstHalf.push_back(digit);
                } else {
                    secondHalf.push_back(digit);
                }
            }

            return make_pair(BigNumber(firstHalf), BigNumber(secondHalf));
        }

        long long asLongLong() const  {
            return stoll(this->asString());
        }

        size_t lenght() const  {
            return this->m_values.size();
        }

        bool isZero() const  {
            return this->lenght() == 1 && this->m_values[0] == 0;
        }

        bool isOne() const  {
            return this->m_positive && this->lenght() == 1 && this->m_values[0] == 1;
        }

        bool isPositive() const  {
            return this->m_positive;
        }

    private:

        bool m_positive = true;
        vector<int> m_values; // Вектор для хранения чисел

        void removeLeftZeros()  {
            for (int i = this->lenght() - 1; i >= 1; i--) {
                if (this->m_values[i] == 0) {
                    this->m_values.pop_back();
                } else {
                    break;
                }
            }
        }

        void doCarryOver(int start = 0)  {
            for (int i = start; i < this->lenght(); i++) {
                if (this->m_values[i] > 9) {
                    this->m_values[i] -= 10;
                    if (i + 1 < this->lenght()) {
                        this->m_values[i + 1]++;
                    } else {
                        this->m_values.push_back(1);
                        return doCarryOver(i);
                    }
                }
            }
        }

        void afterOperation()  {
            this->removeLeftZeros();
            if (this->isZero()) {
                this->m_positive = true;
            }
        }

        pair<BigNumber, BigNumber> divide(const BigNumber& number) const  {
            if (number.isZero()) {
                throw invalid_argument("[BigNumber] Can't divide a zero.");
            }
            if (number.isOne()) {
                return make_pair(*this, BigNumber(0));
            }
            if (number == *this) {
                return make_pair(BigNumber(1), BigNumber(0));
            }
            if (number > this->absoluteValue()) {
                return make_pair(BigNumber(0), *this);
            }

            BigNumber rest = this->absoluteValue();
            const BigNumber absoluteNumber = number.absoluteValue();

            BigNumber quotient;

            int lenghDifference = rest.lenght() - absoluteNumber.lenght();
            while (lenghDifference-- >= 0) {
                BigNumber toSubtract = absoluteNumber.times10(lenghDifference);
                while (rest >= toSubtract) {
                    quotient += BigNumber(1).times10(lenghDifference);
                    rest -= toSubtract;
                }
            }
            quotient.m_positive = this->m_positive == number.m_positive;
            quotient.afterOperation();
            rest.afterOperation();
            return make_pair(quotient, rest);
        }

        BigNumber(const vector<int> &vector, bool reversed=false)  {
            if (vector.size() > 0) {
                this->m_values = vector;
                if (reversed) {
                    reverse(this->m_values.begin(), this->m_values.end());
                }
            } else {
                this->m_values.push_back(0);
            }
            this->afterOperation();
        }

    };