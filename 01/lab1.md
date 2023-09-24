# Лабораторная работа №1. Основной математический аппарат
Работу выполнил студент гр. БИ-42: Охотников Илья.
## 1. Бинарное представление данных.
~~~
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

vector<unsigned char> read_file(string file_path) {
    ifstream file(file_path, ios::binary);
    if (!file) {
        throw "Ошибка открытия файла";
    }
    vector<unsigned char> data(istreambuf_iterator<char>(file), {});
    return data;
}

void print_bytes(vector<unsigned char>data, string format_type) {
    if (format_type == "hex8") {
        for (const auto& byte : data) {
            cout << setw(2) << setfill('0') << hex << static_cast<int>(byte) << ' ';
        }
    }
    else if (format_type == "dec8") {
        for (const auto& byte : data) {
            cout << setw(3) << setfill('0') << dec << static_cast<int>(byte) << ' ';
        }
    }
    else if (format_type == "hex16") {
        for (const auto& byte : data) {
            cout << setw(4) << setfill('0') << hex << static_cast<int>(byte) << ' ';
        }
    }
    else if (format_type == "dec16") {
        for (const auto& byte : data) {
            cout << setw(5) << setfill('0') << dec << static_cast<int>(byte) << ' ';
        }
    }
    else if (format_type == "hex32") {
        for (const auto& byte : data) {
            cout << setw(8) << setfill('0') << hex << static_cast<int>(byte) << ' ';
        }
    }
    else {
        cout << "Неверный формат файла" << endl;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    string format_type = argv[1];
    string file_path = argv[2];

    vector<unsigned char> data = read_file(file_path);
    print_bytes(data, format_type);

    return 0;
}
~~~
## 2. Битовые операции.
~~~
#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");

	string command = argv[1];
	unsigned long long number1 = stoull(argv[2]);
	unsigned long long number2 = stoull(argv[3]);

	unsigned long long result;

	if (command == "xor") {
		result = number1 ^ number2;
	}
	else if (command == "and") {
		result = number1 & number2;
	}
	else if (command == "or") {
		result = number1 | number2;
	}
	else if (command == "set1") {
		result = number2 | (1ULL << number1);
	}
	else if (command == "set0") {
		result = number2 & ~(1ULL << number1);
	}
	else if (command == "shl") {
		result = number2 << number1;
	}
	else if (command == "shr") {
		result = number2 >> number1;
	}
	else if (command == "rol") {
		result = (number2 << number1) | (number2 >> (64 - number1));
	}
	else if (command == "ror") {
		result = (number2 >> number1) | (number2 << (64 - number1));
	}
	else if (command == "mix") {
		bitset<8> bits(number2);

		vector<int> digits;
		while (number1 > 0) {
			digits.insert(digits.begin(), number1 % 10);
			number1 /= 10;
		}

		bitset<8> mix;
		for (int i = 0; i != 8; ++i) {
			mix[digits[i]] = bits[i];
		}
		result = mix.to_ulong();
	}
	else {
		cout << "Неверная команда" << endl;
	}

	cout << "dec: " << result << endl;
	cout << "hex: " << hex << result << endl;
	cout << "bin: " << bitset<64>(result) << endl;
}
~~~
## 3. Модульная арифметика.
~~~
#include <iostream>

using namespace std;

int gcdExtended(int a, int b, int& x, int& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

int modInverse(int a, int m) {
    int x, y;
    int gcd = gcdExtended(a, m, x, y);
    if (gcd != 1) {
        // обратный элемент не существует
        return -1;
    }
    else {
        // находим положительное значение обратного элемента
        return (x % m + m) % m;
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    int M;
    cout << "Введите значения M: ";
    cin >> M;

    int a, b;
    cout << "Введите два целых числа a и b: ";
    cin >> a >> b;

    // Сложение
    int addition = (a + b) % M;
    cout << "(a+b) mod M = " << addition << endl;

    // Вычитание
    int subtraction = (a - b) % M;
    if (subtraction < 0) {
        subtraction += M;
    }
    cout << "(a-b) mod M = " << subtraction << endl;

    // Умножение
    int multiplication = (a * b) % M;
    cout << "(a*b) mod M = " << multiplication << endl;

    // Возведение в степень
    int result = 1;
    for (int i = 0; i < b; i++) {
        result = (result * a) % M;
    }
    cout << "(a^b) mod M = " << result << endl;

    // Поиск обратного элемента a
    int inverse_a = modInverse(a, M);
    if (inverse_a != -1) {
        cout << "a^(-1) mod M = " << inverse_a << endl;
    }
    else {
        cout << "a^(-1) mod M = нет решения" << endl;
    }

    // Поиск обратного элемента b
    int inverse_b = modInverse(b, M);
    if (inverse_b != -1) {
        cout << "b^(-1) mod M = " << inverse_b << endl;
    }
    else {
        cout << "b^(-1) mod M = нет решения" << endl;
    }

    // Деление
    if (inverse_a != -1) {
        cout << "(b/a) mod M = " << (b * inverse_a) % M << endl;
    }
    else {
        cout << "(b^a) mod M = нет решения" << endl;
    }

    if (inverse_b != -1) {
        cout << "(a/b) mod M = " << (a * inverse_b) % M << endl;
    }
    else {
        cout << "(a/b) mod M = нет решения" << endl;
    }
}
~~~
## 3.2 Модульная арифметика на полиномах GF(2,n).
~~~
#include <iostream>

using namespace std;

int mult(int a, int b, int m)
{
    int res = 0;
    while (b > 0)
    {
        if ((b & 1) != 0)
        {
            res ^= a;
            if (res >= m) res ^= m;
        }
        a <<= 1;
        if (a >= m) a ^= m;
        b >>= 1;
    }
    return res;
}

int gcdExtended(int a, int b, int& x, int& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

int modInverse(int a, int m) {
    int x, y;
    int gcd = gcdExtended(a, m, x, y);
    if (gcd != 1) {
        // обратный элемент не существует
        return -1;
    }
    else {
        // находим положительное значение обратного элемента
        return (x % m + m) % m;
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    int a = 365;
    int b = 1514;
    int m = 69665;

    //сложение
    cout << "(a + b) mod M = " << (a ^ b) % m << endl;

    //вычитание
    cout << "(a - b) mod M= " << (a ^ b) % m << endl;

    //умножение
    cout << "(a * b) mod M = " << mult(a, b, m) << endl;

    //обратный элемент
    cout << "a^(-1) mod M = " << modInverse(2, m) << endl;
}
~~~
## 4. Проверка числа на простоту.
~~~
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Введите n:" << endl;
	int n;
	cin >> n;
	vector<bool>A(n + 1, true);
	for (int i = 2; pow(i, 2) <= n; ++i) {
		if (A[i] == true) {
			for (int j = pow(i, 2); j <= n; j = j + i) {
				A[j] = false;
			}
		}
	}
	cout << "N первых простых чисел:" << endl;
	for (int i = 2; i != A.size(); ++i) {
		if (A[i] == true) {
			cout << i << " ";
		}
	}
	cout << endl;
	if (A[n] == true) {
		cout << n << "-простое" << endl;
	}
	else {
		cout << n << "-не простое" << endl;
	}
}
~~~
## 5. Арифметика больших чисел.
~~~
#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <chrono>

using namespace std::chrono;

using namespace std;

class BigInteger {
private:
    string number;

public:
    BigInteger(const string& str) : number(str) {}

    void print() const {
        cout << number << endl;
    }

    bool operator>=(const BigInteger& other) const {
        return number.size() > other.number.size() || (number.size() == other.number.size() && number >= other.number);
    }


    BigInteger operator+(const BigInteger& other) const {
        string result;
        int carry = 0;
        int i = number.size() - 1;
        int j = other.number.size() - 1;

        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) {
                sum += number[i] - '0';
                i--;
            }
            if (j >= 0) {
                sum += other.number[j] - '0';
                j--;
            }

            result.push_back(sum % 10 + '0');
            carry = sum / 10;
        }

        reverse(result.begin(), result.end());
        return BigInteger(result);
    }

    BigInteger operator*(const BigInteger& other) const {
        int n = number.size();
        int m = other.number.size();
        string result(n + m, '0');

        for (int i = n - 1; i >= 0; --i) {
            int carry = 0;
            for (int j = m - 1; j >= 0; --j) {
                int product = (number[i] - '0') * (other.number[j] - '0') +
                    (result[i + j + 1] - '0') + carry;

                carry = product / 10;
                result[i + j + 1] = (product % 10) + '0';
            }
            result[i] += carry;
        }

        // Remove leading zeros
        size_t pos = result.find_first_not_of('0');
        if (pos != string::npos)
            return BigInteger(result.substr(pos));

        return BigInteger("0");
    }

    BigInteger operator-(const BigInteger& other) const {
        string result;
        int borrow = 0;
        int i = number.size() - 1;
        int j = other.number.size() - 1;

        while (i >= 0 || j >= 0) {
            int diff = borrow;
            if (i >= 0) {
                diff += number[i] - '0';
                i--;
            }
            if (j >= 0) {
                diff -= other.number[j] - '0';
                j--;
            }

            if (diff < 0) {
                diff += 10;
                borrow = -1;
            }
            else {
                borrow = 0;
            }

            result.push_back(diff + '0');
        }

        // Remove leading zeros
        while (result.size() > 1 && result.back() == '0')
            result.pop_back();

        reverse(result.begin(), result.end());
        return BigInteger(result);
    }

    BigInteger operator/(const BigInteger& other) const {
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }

        BigInteger quotient("0");
        BigInteger remainder = *this;

        while (remainder >= other) {
            BigInteger subtracted = other;
            int count = 1;
            while (remainder >= (subtracted + subtracted)) {
                subtracted = subtracted + subtracted;
                count = count + count;
            }

            remainder = remainder - subtracted;
            quotient = quotient + BigInteger(to_string(count));
        }

        return quotient;
    }

    BigInteger operator%(const BigInteger& other) const {
        BigInteger quotient = *this / other;
        BigInteger result = *this - (other * quotient);
        return result;
    }

    BigInteger abs() const {
        if (number[0] == '-') {
            // Если число отрицательное, вернуть его модуль
            return BigInteger(number.substr(1));
        }
        else {
            // Если число положительное, вернуть его как есть
            return *this;
        }
    }

    // Добавим оператор вывода, чтобы легко выводить объекты BigInteger
    friend ostream& operator<<(ostream& os, const BigInteger& bi) {
        os << bi.number;
        return os;
    }
};

int main()
{
    auto start_time = chrono::steady_clock::now();
    delete new int(1);

    setlocale(LC_ALL, "rus");
    BigInteger num1("123456789012345");
    BigInteger num2("987654321987");

    cout << "num1: ";
    num1.print();

    cout << "num2: ";
    num2.print();

    BigInteger sum = num1 + num2;
    cout << "Сумма: ";
    sum.print();

    BigInteger product = num1 * num2;
    cout << "Произведение: ";
    product.print();

    BigInteger module = num1 % num2; // Вывод остатка
    cout << "Модуль от числа: ";
    module.print();



    auto end_time = chrono::steady_clock::now();
    auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    cout << "Время работы программы = " << elapsed_ns.count() / 1000000 << " ms\n";

    return 0;
}
~~~
