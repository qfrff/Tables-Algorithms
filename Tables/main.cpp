#include <iostream> // ��� �����/������ ����� ����������� ������ �����-������ (cin � cout)
#include <string> // ��� ������ �� ��������
#include <vector> // ��� ������������� ������������ ��������
#include <chrono> // ��� ������ � �������� � ������ ������������������
#include <random> // ��� ��������� ��������������� ����� � ������
#include <algorithm> // ��� ������������� ���������� �� ���������� ������, ����� ��� ����������, ����� � �. �.
#include <limits> // ��� ��������� ���������� � �������� ��������� ����� ������
#include <cstdlib> // ������������� ������� ��� ������ � �������, �������� � �������
#include <ctime> // ��� ������ �� �������� � ��������� �������
#include <locale.h>
#include <fstream> // ��� ������ � �������

#include "TNeUpTable.h"
#include "TNeUpTableList.h"
#include "TUpTable.h"
#include "TBinarySearchTree.h"
#include "THashTable.h"

using namespace std;
using namespace chrono;

// ��������������� ������� � �����������

// ������
vector<string> GenerateRandomWords(int count) {
    vector<string> randomWords;
    randomWords.reserve(count);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis('A', 'Z');

    for (int i = 0; i < count; ++i) {
        string word;
        for (int j = 0; j < 6; ++j) { // ����� ������ 6 ��������
            word.push_back(dis(gen));
        }
        randomWords.push_back(word);
    }

    // ��������� ��������� ������� ������������� �����
    uniform_int_distribution<> repeatCount(1, 50); // ��������� ���������� ���������� �� n �� n

    for (int i = 0; i < count; ++i) {
        int repeats = repeatCount(gen);
        for (int j = 0; j < repeats; ++j) {
            randomWords.push_back(randomWords[i]);
        }
    }

    shuffle(randomWords.begin(), randomWords.end(), gen);

    return randomWords;
}

void DisplayRandomWords(const vector<string>& randomWords) {
    cout << "������ ��������� ���� ��� ������:\n";
    for (int i = 0; i < 10; ++i) {
        cout << i + 1 << ". " << randomWords[i] << endl;
    }
}

// ������� ��� ������ ����� � ����
void WriteToFile(const string& key) {
    ofstream file("C:\\Users\\s22380640\\Desktop\\key.txt", ios::app);
    if (file.is_open()) {
        file << key << endl;
        file.close();
    }
    else {
        cerr << "�� ���������� ������� ����" << endl;
    }
}

// ������� ��� ������ ������ �� ����� � ������ �� �����
vector<string> ReadFromFile() {
    vector<string> keys;
    ifstream file("C:\\Users\\s22380640\\Desktop\\key.txt");
    if (file.is_open()) {
        cout << "\n���������� ����� key.txt:" << endl;
        string key;
        while (getline(file, key)) {
            cout << key << endl;
            keys.push_back(key);
        }
        file.close();
    }
    else {
        cerr << "�� ���������� ������� ����" << endl;
    }
    return keys;
}

// ������� ��� �������� ����� �� �����
void RemoveFromFile(const string& key) {
    ifstream inputFile("C:\\Users\\s22380640\\Desktop\\key.txt");
    ofstream tempFile("C:\\Users\\s22380640\\Desktop\\temp.txt");

    string line;
    bool found = false;
    while (getline(inputFile, line)) {
        if (line == key && !found) {
            found = true;
        }
        else {
            tempFile << line << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    // ������� ������ ����
    remove("C:\\Users\\s22380640\\Desktop\\key.txt");
    // ��������������� ��������� ���� � ������ ���
    rename("C:\\Users\\s22380640\\Desktop\\temp.txt", "C:\\Users\\s22380640\\Desktop\\key.txt");

    if (!found) {
        cerr << "����� �� ������� � �����" << endl;
    }
}

// ������� ��� �������� ���� ������� �� �����
void ClearFile() {
    ofstream file("C:\\Users\\s22380640\\Desktop\\key.txt", ios::trunc);
    if (file.is_open()) {
        file.close();
        cout << "\n���� ������" << endl;
    }
    else {
        cerr << "�� ���������� ������� ����" << endl;
    }
}

// ������� ������������ ������� � ����� ������������������ ������

int main() {
    setlocale(LC_ALL, "Russian");

    TNeUpTable neUpTable;
    TNeUpTableList listTable;
    TUpTable upTable(5); // ����� ��������
    TBinarySearchTree bstTable;
    THashTable hashTable(5); // ����� ��������

    string key;
    int choice;

    do {
        cout << "\n����:" << endl;
        cout << "1. �������� �����" << endl;
        cout << "2. ����� �����" << endl;
        cout << "3. ������� �����" << endl;
        cout << "4. �������� ������" << endl;
        cout << "5. �������� ����" << endl;
        cout << "6. �������� ����" << endl;
        cout << "7. �������� ������ ������" << endl;
        cout << "0. �����" << endl;
        cout << "�������� ��������: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "������� �����: ";
            cin >> key;
            neUpTable.InsRecord(key, nullptr);
            upTable.InsRecord(key, nullptr);
            hashTable.InsRecord(key, nullptr);
            bstTable.Insert(key, nullptr);
            listTable.Insert(key, nullptr);
            WriteToFile(key); // ���������� ����� � ����
            break;
        }

        case 2: {
            cout << "������� ����� ��� ������: ";
            cin >> key;

            // ����� � ��������������� ������� �� ������ �������
            neUpTable.ResetComparisonCount();
            int neUpCount = neUpTable.FindRecord(key);
            int neUpComparisons = neUpTable.GetComparisonCount();
            cout << "��������������� ������� �� ������ �������: ������� " << neUpCount << " ���������, " << neUpComparisons << " ���-�� ��������" << endl;

            // ����� � ��������������� ������� �� ������ ������
            listTable.ResetComparisonCount();
            int listCount = listTable.FindRecord(key);
            int listComparisons = listTable.GetComparisonCount();
            cout << "��������������� ������� �� ������ ������: ������� " << listCount << " ���������, " << listComparisons << " ���-�� ��������" << endl;

            // ����� � ������������� ������� (���������� ��������, �������� �����)
            upTable.ResetComparisonCount();
            int upCount = upTable.FindRecord(key);
            int upComparisons = upTable.GetComparisonCount();
            cout << "������������� ������� (���������� ��������, �������� �����): ������� " << upCount << " ���������, " << upComparisons << " ���-�� ��������" << endl;

            // ����� � ������������� ������� (�������� ������ ������)
            bstTable.ResetComparisonCount();
            int bstCount = bstTable.FindRecord(key);
            int bstComparisons = bstTable.GetComparisonCount();
            cout << "������������� ������� (�������� ������ ������): ������� " << bstCount << " ���������, " << bstComparisons << " ���-�� ��������" << endl;

            // ����� � ���-�������
            hashTable.ResetComparisonCount();
            int hashCount = hashTable.FindRecord(key);
            int hashComparisons = hashTable.GetComparisonCount();
            cout << "���-�������: ������� " << hashCount << " ���������, " << hashComparisons << " ���-�� ��������" << endl;
            break;
        }

        case 3: {
            cout << "������� ����� ��� ��������: ";
            cin >> key;
            neUpTable.DelRecord(key);
            upTable.DelRecord(key);
            hashTable.DelRecord(key);
            bstTable.Delete(key);
            listTable.Delete(key);
            RemoveFromFile(key); // �������� ����� �� �����
            break;
        }

        case 4: {
            ReadFromFile();
            break;
        }

        case 5: {
            bool returnToMainMenu = false;
            do {
                int tableSize;
                cout << "������� ������ ������: ";
                cin >> tableSize;

                neUpTable = TNeUpTable();
                upTable = TUpTable(tableSize);
                listTable = TNeUpTableList();
                bstTable = TBinarySearchTree();
                hashTable = THashTable(tableSize);

                cout << "\n��������� ������� � ��������� ������� ������� ����������� ��������� ����.\n";
                cout << "��������...\n";

                vector<string> randomWords = GenerateRandomWords(tableSize);

                auto startTables = high_resolution_clock::now();

                for (int i = 0; i < tableSize; ++i) {
                    string randomWord = randomWords[i];
                    neUpTable.InsRecord(randomWord, nullptr);
                    upTable.InsRecord(randomWord, nullptr);
                    hashTable.InsRecord(randomWord, nullptr);
                    bstTable.Insert(randomWord, nullptr);
                    listTable.InsRecord(randomWord, nullptr);
                }

                auto endTables = high_resolution_clock::now();
                duration<double> tablesTime = endTables - startTables;

                cout << "\n����� �������� � ���������� ������: " << tablesTime.count() << " ������\n" << endl;

                DisplayRandomWords(randomWords);

                do {
                    int subChoice;
                    cout << "\n1. ����� �����" << endl;
                    cout << "2. ��������� � �������� ����" << endl;
                    cout << "\n�������� ��������: ";
                    cin >> subChoice;

                    if (subChoice == 2) {
                        returnToMainMenu = true;
                        break;
                    }

                    string testWord;
                    cout << "\n������� ����� ��� ������: ";
                    cin >> testWord;

                    auto startSearch1 = high_resolution_clock::now();
                    int neUpCount = neUpTable.FindRecord(testWord);
                    auto endSearch1 = high_resolution_clock::now();
                    duration<double> neUpTime = endSearch1 - startSearch1;
                    int neUpComparisons = neUpTable.GetComparisonCount();
                    cout << "��������������� ������� �� ������ �������: ������� " << neUpCount << " ��������� �� " << neUpTime.count() << " ������, " << neUpComparisons << " ���-�� ��������" << endl;

                    auto startSearch5 = high_resolution_clock::now();
                    int listCount = listTable.FindRecord(testWord);
                    auto endSearch5 = high_resolution_clock::now();
                    duration<double> listTime = endSearch5 - startSearch5;
                    int listComparisons = listTable.GetComparisonCount();
                    cout << "��������������� ������� �� ������ ������: ������� " << listCount << " ��������� �� " << listTime.count() << " ������, " << listComparisons << " ���-�� ��������" << endl;

                    auto startSearch2 = high_resolution_clock::now();
                    int upCount = upTable.FindRecord(testWord);
                    auto endSearch2 = high_resolution_clock::now();
                    duration<double> upTime = endSearch2 - startSearch2;
                    int upComparisons = upTable.GetComparisonCount();
                    cout << "������������� ������� (���������� ��������, �������� �����): ������� " << upCount << " ��������� �� " << upTime.count() << " ������, " << upComparisons << " ���-�� ��������" << endl;

                    auto startSearch4 = high_resolution_clock::now();
                    int bstCount = bstTable.FindRecord(testWord);
                    auto endSearch4 = high_resolution_clock::now();
                    duration<double> bstTime = endSearch4 - startSearch4;
                    int bstComparisons = bstTable.GetComparisonCount();
                    cout << "������������� ������� (�������� ������ ������): ������� " << bstCount << " ��������� �� " << bstTime.count() << " ������, " << bstComparisons << " ���-�� ��������" << endl;

                    auto startSearch3 = high_resolution_clock::now();
                    int hashCount = hashTable.FindRecord(testWord);
                    auto endSearch3 = high_resolution_clock::now();
                    duration<double> hashTime = endSearch3 - startSearch3;
                    int hashComparisons = hashTable.GetComparisonCount();
                    cout << "���-�������: ������� " << hashCount << " ��������� �� " << hashTime.count() << " ������, " << hashComparisons << " ���-�� ��������" << endl;

                } while (true);

                if (returnToMainMenu) {
                    break;
                }

            } while (true);

            break;
        }

        case 6: {
            ClearFile();
            break;
        }

        case 7: {
            cout << "����������� ������ � ��������: \n" << endl;

            cout << "\n��������������� ������� �� ������ �������:" << endl;
            neUpTable.DisplayRecords();

            cout << "\n��������������� ������� �� ������ ������:" << endl;
            listTable.DisplayRecords();

            cout << "\n������������� ������� (���������� ��������, �������� �����):" << endl;
            upTable.DisplayRecords();

            cout << "\n������������� ������� (�������� ������ ������):" << endl;
            bstTable.DisplayRecords();
            cout << "\n������ ������: " << bstTable.GetRoot()->Key << endl;

            cout << "\n���-�������: \n" << endl;
            hashTable.DisplayRecords();
            hashTable.DisplayResolvedIndices();
            break;
        }

        }

    } while (choice != 0);

    return 0;
}