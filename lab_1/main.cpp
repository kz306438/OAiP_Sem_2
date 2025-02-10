#include <iostream>
#include <cmath>

using namespace std;

// ������� ��� ������� �����
void clearInput()
{
    cin.clear(); // ���������� ��������� ������
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���������� ���������� ������� � ������
}

// ��������� ������� ��� ����� �������� � ���������
template <typename T>
void inputValue(const string& prompt, T& value, bool (*checkFunc)(const T&) = nullptr)
{
    while (true)
    {
        cout << prompt << '\n'; // ������� ����������� ��� �����
        cin >> value; // ��������� ��������
        if (cin.good()) { // ���������, ������� �� ������� ��������
            if (!checkFunc || checkFunc(value)) // ���� �������� ������� ��������, �������� �
            {
                break; // ������� �� �����, ���� �������� ���������
            }
        }
        else {
            cout << "������������ ����! ���������� �����!\n"; // �������� �� ������ �����
            clearInput(); // ������� ����
        }
    }
}

// ������� ��� �������� ������������ �������� ������������ ���������
bool checkSqrtDomain(const double& value)
{
    if (value < 0) // ���������, ��� �������� �� �������������
    {
        cout << "����������� ��������� �� ����� ���� ������ ����!" << endl; // �������� �� ������
        return false; // ���������� false, ���� �������� �����������
    }

    return true; // ���������� true, ���� �������� ���������
}

// ����������� ������� ��� ���������� ����������� �����
double mySqrtRec(double a, double x_n, double epsilon)
{
    double x_n1 = 0.5 * (x_n + a / x_n); // ��������� ����� �����������
    if (abs(x_n1 - x_n) < epsilon) // ��������� ������� ���������
    {
        return x_n1; // ���������� ���������, ���� ���������� ��������
    }
    return mySqrtRec(a, x_n1, epsilon); // ���������� �������� ������� � ����� ������������
}

// ������� ��� ���������� ����������� ����� � �������� ���������
double mySqrt(double a, double epsilon = 1e-7)
{
    return mySqrtRec(a, 0.5 * (1 + a), epsilon); // �������� ����������� ������� � ��������� ������������
}

// ������� ��� ���������� ����������
void calculate()
{
    double a;
    inputValue("������� ����� a:", a, checkSqrtDomain); // ������ �������� � ���������

    cout << "��������� ����������� ���������� ������� �����:" << endl;

    if (a)
        cout << mySqrt(a) << endl;
    else
        cout << 0 << endl;

    cout << endl;

    cout << "��������� ������������ ���������� ������� �����" << endl;
    cout << sqrt(a) << endl; // ������� ��������� ����������� ������� sqrt

    cout << endl;
}

int main()
{
    setlocale(LC_ALL, "ru"); // ������������� ������ ��� ����������� ����������� �������� ������

    while (true)
    {
        calculate(); // ��������� ���������� � ����������� �����
    }

    return 0; // ��������� ���������
}
