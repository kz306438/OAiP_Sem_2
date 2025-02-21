#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int n = 8;
const int M = 10;

struct Flight {
    string destination;
    int flightNumber;
    string departureTime;
};

class HashTable {
private:
    vector<Flight*> table;
    vector<bool> occupied;

    int hash1(int key) {
        return key % M;
    }

    int hash2(int key) {
        return 1 + (key % (M - 2));
    }

public:
    HashTable() : table(M, nullptr), occupied(M, false) {}

    void insert(Flight* flight) {
        int key = flight->flightNumber;
        int index = hash1(key);

        if (!occupied[index]) {
            table[index] = flight;
            occupied[index] = true;
            return;
        }

        int step = hash2(key);
        for (int i = 0; i < M; i++) {
            index = index - step;
            if (index < 0) index += M;
            if (!occupied[index]) {
                table[index] = flight;
                occupied[index] = true;
                return;
            }
        }
        cout << "���-������� ���������, ���������� ��������: " << key << endl;
    }

    Flight* search(int key) {
        int index = hash1(key);
        int step = hash2(key);

        for (int i = 0; i < M; i++) {
            if (!occupied[index]) return nullptr;
            if (table[index] && table[index]->flightNumber == key) return table[index];
            index = index - step;
            if (index < 0) index += M;
        }
        return nullptr;
    }

    void display() {
        cout << "���-�������:" << endl;
        for (int i = 0; i < M; i++) {
            if (occupied[i] && table[i]) {
                cout << i << ": " << table[i]->flightNumber << " -> " << table[i]->destination << " (" << table[i]->departureTime << ")" << endl;
            }
            else {
                cout << i << ": �����" << endl;
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");

    Flight flights[n] = {
        {"������", 101, "08:00"},
        {"����", 202, "09:30"},
        {"������", 303, "11:15"},
        {"�����������", 404, "12:45"},
        {"������������", 505, "14:10"},
        {"�����-���������", 606, "15:55"},
        {"�����������", 707, "18:30"},
        {"���������", 808, "20:00"}
    };

    cout << "�������� ������ ������:" << endl;
    for (const auto& flight : flights) {
        cout << flight.flightNumber << " -> " << flight.destination << " (" << flight.departureTime << ")" << endl;
    }

    HashTable hashTable;
    for (int i = 0; i < n; i++) {
        hashTable.insert(&flights[i]);
    }

    hashTable.display();

    int searchKey;
    cout << "������� ����� ����� ��� ������: ";
    cin >> searchKey;
    Flight* foundFlight = hashTable.search(searchKey);
    if (foundFlight) {
        cout << "������ ����: " << foundFlight->flightNumber << " -> " << foundFlight->destination << " (" << foundFlight->departureTime << ")" << endl;
    }
    else {
        cout << "���� �� ������." << endl;
    }

    return 0;
}
