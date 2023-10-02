#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <cmath>
#include <cstdint>

class Array {
public:
    using Iterator = double*;

    Array(uint32_t size) : m_size(size) {
        if (size > maxsize()) {
            throw std::range_error("invalid array size");
        }
        m_data = new double[size];
    }

    Array(uint32_t size, double default_value) : Array(size) {
        for (auto& element : *this) {
            element = default_value;
        }
    }

    Array(const Iterator begin, const Iterator end) : Array(end - begin) {
        if (begin >= end) {
            throw invalid_iterators("invalid iterators");
        }
        uint32_t index = 0;
        for (Iterator it = begin; it != end; ++it) {
            m_data[index++] = *it;
        }
    }

    Array(const Array& array) : Array(array.cbegin(), array.cend()) {}

    ~Array() { delete[] m_data; }

    Array& operator=(const Array& array) {
        if (this != &array) {
            if (size() != array.size()) {
                throw different_sizes("arrays have different sizes");
            }
            for (uint32_t n = 0; n < size(); ++n) {
                m_data[n] = array[n];
            }
        }
        return *this;
    }

    double& operator[](uint32_t index) {
        if (index < m_size) {
            return m_data[index];
        }
        else {
            throw std::out_of_range("array out of range");
        }
    }

    const double& operator[](uint32_t index) const {
        if (index < m_size) {
            return m_data[index];
        }
        else {
            throw std::out_of_range("array out of range");
        }
    }


    uint32_t size() const { return m_size; }
    static uint32_t maxsize() { return MAX_SIZE; }

    Iterator       begin() { return m_data; }
    Iterator       end() { return m_data + m_size; }
    const Iterator begin()  const { return m_data; }
    const Iterator end()    const { return m_data + m_size; }
    const Iterator cbegin() const { return m_data; }
    const Iterator cend()   const { return m_data + m_size; }



    std::ostream& print(std::ostream& outstream, const char* sep = " ", const char* end = "") const {
        for (int i = 0; i < size(); ++i) {
            outstream << m_data[i];
            if (i != size() - 1) {
                outstream << sep;
            }
        }
        outstream << end;
        return outstream;
    }

    friend std::istream& operator>>(std::istream& instream, Array& array) {
        for (auto& element : array) {
            instream >> element;
        }
        return instream;
    }

    friend std::ostream& operator<<(std::ostream& outstream, const Array& array) {
        for (auto& element : array) {
            outstream << element << " ";
        }
        return outstream;
    }

    class different_sizes : public std::logic_error {
    public: different_sizes(const char* msg) : logic_error(msg) {}
    };

    class invalid_iterators : public std::invalid_argument {
    public: invalid_iterators(const char* msg) : invalid_argument(msg) {}
    };

private:
    const static uint32_t MAX_SIZE = 10000;
    const uint32_t m_size;
    double* m_data;
};

class Integral {
private:
    int size;
    int a, b;
    Array arguments;
    Array values;
public:

    Integral(int size) : size(size), arguments(size), values(size), a(0), b(size - 1) { }

    void readArguments() {
        std::cin >> arguments;
    }

    void readFunctions() {
        std::cin >> values;
    }

    friend std::ostream& operator<<(std::ostream& outstream, const Integral& integral) {

        outstream << "argument " << integral.arguments << "\n"
            << "function " << integral.values;
        return outstream;
    }

    double levPriam() {
        double result = 0;
        for (int i = 0; i < size - 1; ++i) {
            double step = arguments[i + 1] - arguments[i];
            result += values[i] * step;
        }
        return result;
    }

    double srPriam() {
        double result = 0;
        for (int i = 0; i < size - 1; ++i) {
            double step = arguments[i + 1] - arguments[i];
            result += (values[i] + values[i + 1]) / 2 * step;
        }
        return result;
    }

    double pravPriam() {
        double result = 0;
        for (int i = 0; i < size - 1; ++i) {
            double step = arguments[i + 1] - arguments[i];
            result += values[i + 1] * step;
        }
        return result;
    }

    double trapeciy() {
        double result = 0;
        for (int i = 0; i < size - 1; ++i) {
            double step = arguments[i + 1] - arguments[i];
            result += (values[i] + values[i + 1]) / 2 * step;
        }
        return result;
    }

    double Simpson() {
        double integral = 0;
        int intStep = 2;
        double step = intStep * (arguments[b] - arguments[a]) / (size - 1);

        for (int i = 0; i < size - intStep; i += intStep) {
            double a = values[i];
            double b = 4 * values[i + 1];
            double c = values[i + 2];
            integral += (a + b + c) * step / 6;
        }

        return std::round(integral * 10) / 10;
    }

    double Newton() {
        double integral = 0;
        int intStep = 3;
        double step = intStep * (arguments[b] - arguments[a]) / (size - 1);
        for (int i = 0; i < size - intStep; i += intStep) {
            double a = values[i];
            double b = 3 * values[i + 1];
            double c = 3 * values[i + 2];
            double d = values[i + 3];
            integral += (a + b + c + d) * step / 8;
        }

        return integral;
    }

};

const double eps = 0.1E-8;


int main() {

    int code, size;

    std::cin >> size;

    Integral integral = Integral(size);

    integral.readArguments();
    integral.readFunctions();

    std::cout << "input= " << integral << "\n";
    std::cout << "lev priam= " << integral.levPriam() << "\n";
    std::cout << "sr priam= " << integral.srPriam() << "\n";
    std::cout << "prav priam= " << integral.pravPriam() << "\n";
    std::cout << "trapeciy= " << integral.trapeciy() << "\n";
    std::cout << "Simpson= " << integral.Simpson() << "\n";
    std::cout << "Newton " << integral.Newton() << "\n";
}