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