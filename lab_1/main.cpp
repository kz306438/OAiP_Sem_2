#include <iostream>
#include <cmath>

using namespace std;

// Функция для очистки ввода
void clearInput()
{
    cin.clear(); // Сбрасываем состояние потока
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорируем оставшиеся символы в потоке
}

// Шаблонная функция для ввода значения с проверкой
template <typename T>
void inputValue(const string& prompt, T& value, bool (*checkFunc)(const T&) = nullptr)
{
    while (true)
    {
        cout << prompt << '\n'; // Выводим приглашение для ввода
        cin >> value; // Считываем значение
        if (cin.good()) { // Проверяем, успешно ли считано значение
            if (!checkFunc || checkFunc(value)) // Если передана функция проверки, вызываем её
            {
                break; // Выходим из цикла, если значение корректно
            }
        }
        else {
            cout << "Некорректный ввод! Попробуйте снова!\n"; // Сообщаем об ошибке ввода
            clearInput(); // Очищаем ввод
        }
    }
}

// Функция для проверки допустимости значения подкорневого выражения
bool checkSqrtDomain(const double& value)
{
    if (value < 0) // Проверяем, что значение не отрицательное
    {
        cout << "Подкорневое выражение не может быть меньше нуля!" << endl; // Сообщаем об ошибке
        return false; // Возвращаем false, если значение некорректно
    }

    return true; // Возвращаем true, если значение корректно
}

// Рекурсивная функция для вычисления квадратного корня
double mySqrtRec(double a, double x_n, double epsilon)
{
    double x_n1 = 0.5 * (x_n + a / x_n); // Вычисляем новое приближение
    if (abs(x_n1 - x_n) < epsilon) // Проверяем условие остановки
    {
        return x_n1; // Возвращаем результат, если достигнута точность
    }
    return mySqrtRec(a, x_n1, epsilon); // Рекурсивно вызываем функцию с новым приближением
}

// Функция для вычисления квадратного корня с заданной точностью
double mySqrt(double a, double epsilon = 1e-7)
{
    return mySqrtRec(a, 0.5 * (1 + a), epsilon); // Вызываем рекурсивную функцию с начальным приближением
}

// Функция для выполнения вычислений
void calculate()
{
    double a;
    inputValue("Введите число a:", a, checkSqrtDomain); // Вводим значение с проверкой

    cout << "Результат рекурсивной реализации функции корня:" << endl;

    if (a)
        cout << mySqrt(a) << endl;
    else
        cout << 0 << endl;

    cout << endl;

    cout << "Результат классической реализации функции корня" << endl;
    cout << sqrt(a) << endl; // Выводим результат стандартной функции sqrt

    cout << endl;
}

int main()
{
    setlocale(LC_ALL, "ru"); // Устанавливаем локаль для корректного отображения русского текста

    while (true)
    {
        calculate(); // Запускаем вычисления в бесконечном цикле
    }

    return 0; // Завершаем программу
}
