#include <iostream> // для ввода/вывода через стандартные потоки ввода-вывода (cin и cout)
#include <string> // для работы со строками
#include <vector> // для использования динамических массивов
#include <chrono> // для работы с временем и замера производительности
#include <random> // для генерации псевдослучайных чисел и данных
#include <algorithm> // для использования алгоритмов на структурах данных, таких как сортировка, поиск и т. д.
#include <limits> // для получения информации о пределах различных типов данных
#include <cstdlib> // предоставляет функции для работы с памятью, строками и числами
#include <ctime> // для работы со временем и функциями времени
#include <locale.h>
#include <fstream> // для работы с файлами

#include "TNeUpTable.h"
#include "TNeUpTableList.h"
#include "TUpTable.h"
#include "TBinarySearchTree.h"
#include "THashTable.h"

using namespace std;
using namespace chrono;

// Вспомогательные функции и инструменты

// Вектор
vector<string> GenerateRandomWords(int count) {
    vector<string> randomWords;
    randomWords.reserve(count);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis('A', 'Z');

    for (int i = 0; i < count; ++i) {
        string word;
        for (int j = 0; j < 6; ++j) { // Слова длиной 6 символов
            word.push_back(dis(gen));
        }
        randomWords.push_back(word);
    }

    // Добавляем случайным образом повторяющиеся слова
    uniform_int_distribution<> repeatCount(1, 50); // Случайное количество повторений от n до n

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
    cout << "Список случайных слов для поиска:\n";
    for (int i = 0; i < 10; ++i) {
        cout << i + 1 << ". " << randomWords[i] << endl;
    }
}

// Функция для записи ключа в файл
void WriteToFile(const string& key) {
    ofstream file("C:\\Users\\Ваня\\Desktop\\key.txt", ios::app);
    if (file.is_open()) {
        file << key << endl;
        file.close();
    }
    else {
        cerr << "Не получилось открыть файл" << endl;
    }
}

// Функция для чтения ключей из файла и вывода на экран
void ReadFromFile() {
    ifstream file("C:\\Users\\Ваня\\Desktop\\key.txt");
    if (file.is_open()) {
        cout << "\nСодержимое файла key.txt:" << endl;
        string key;
        while (getline(file, key)) {
            cout << key << endl;
        }
        file.close();
    }
    else {
        cerr << "Не получилось открыть файл" << endl;
    }
}

// Функция для удаления ключа из файла
void RemoveFromFile(const string& key) {
    ifstream inputFile("C:\\Users\\Ваня\\Desktop\\key.txt");
    ofstream tempFile("C:\\Users\\Ваня\\Desktop\\temp.txt");

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

    // Удаляем старый файл
    remove("C:\\Users\\Ваня\\Desktop\\key.txt");
    // Переименовываем временный файл в нужное имя
    rename("C:\\Users\\Ваня\\Desktop\\temp.txt", "C:\\Users\\Ваня\\Desktop\\key.txt");

    if (!found) {
        cerr << "Слово не найдено в файле" << endl;
    }
}

// Функция пользователя системы и тесты производительности таблиц

int main() {
    setlocale(LC_ALL, "Russian");

    TNeUpTable neUpTable;
    TNeUpTableList listTable;
    TUpTable upTable(25); // можно изменить
    TBinarySearchTree bstTable;
    THashTable hashTable(25); // можно изменить

    string key;
    int choice;

    do {
        cout << "\nМеню:" << endl;
        cout << "1. Вставить слово" << endl;
        cout << "2. Найти слово" << endl;
        cout << "3. Удалить слово" << endl;
        cout << "4. Показать данные" << endl;
        cout << "5. Провести тест" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Введите слово: ";
            cin >> key;
            neUpTable.InsRecord(key, nullptr);
            upTable.InsRecord(key, nullptr);
            hashTable.InsRecord(key, nullptr);
            bstTable.Insert(key, nullptr);
            listTable.Insert(key, nullptr);
            WriteToFile(key); // добавление ключа в файл
            break;
        }

        case 2: {
            cout << "Введите слово для поиска: ";
            cin >> key;

            // Поиск в неупорядоченной таблице на основе массива
            neUpTable.ResetComparisonCount();
            int neUpCount = neUpTable.FindRecord(key);
            int neUpComparisons = neUpTable.GetComparisonCount();
            cout << "Неупорядоченная таблица на основе массива: Найдено " << neUpCount << " вхождений, " << neUpComparisons << " кол-во сравнений ключей, выполненных при поиске" << endl;

            // Поиск в неупорядоченной таблице на основе списка
            listTable.ResetComparisonCount();
            int listCount = listTable.FindRecord(key);
            int listComparisons = listTable.GetComparisonCount();
            cout << "Неупорядоченная таблица на основе списка: Найдено " << listCount << " вхождений, " << listComparisons << " кол-во сравнений ключей, выполненных при поиске" << endl;

            // Поиск в упорядоченной таблице (сортировка слиянием, бинарный поиск)
            upTable.ResetComparisonCount();
            int upCount = upTable.FindRecord(key);
            int upComparisons = upTable.GetComparisonCount();
            cout << "Упорядоченная таблица (сортировка слиянием, бинарный поиск): Найдено " << upCount << " вхождений, " << upComparisons << " кол-во сравнений ключей, выполненных при поиске" << endl;

            // Поиск в упорядоченной таблице (бинарное дерево поиска)
            bstTable.ResetComparisonCount();
            int bstCount = bstTable.FindRecord(key);
            int bstComparisons = bstTable.GetComparisonCount();
            cout << "Упорядоченная таблица (бинарное дерево поиска): Найдено " << bstCount << " вхождений, " << bstComparisons << " кол-во сравнений ключей, выполненных при поиске" << endl;

            // Поиск в хеш-таблице
            hashTable.ResetComparisonCount();
            int hashCount = hashTable.FindRecord(key);
            int hashComparisons = hashTable.GetComparisonCount();
            cout << "Хеш-таблица: Найдено " << hashCount << " вхождений, " << hashComparisons << " кол-во сравнений ключей, выполненных при поиске" << endl;
            break;
        }

        case 3: {
            cout << "Введите слово для удаления: ";
            cin >> key;
            neUpTable.DelRecord(key);
            upTable.DelRecord(key);
            hashTable.DelRecord(key);
            bstTable.Delete(key);
            listTable.Delete(key);
            RemoveFromFile(key); // удаление ключа из файла
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
                cout << "Введите размер таблиц: ";
                cin >> tableSize;

                neUpTable = TNeUpTable();
                upTable = TUpTable(tableSize);
                listTable = TNeUpTableList();
                bstTable = TBinarySearchTree();
                hashTable = THashTable(tableSize);

                cout << "\nПрограмма создает и заполняет таблицы большим количеством случайных слов.\n";
                cout << "Ожидание...\n";

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

                cout << "\nВремя создания и заполнения таблиц: " << tablesTime.count() << " секунд\n" << endl;

                DisplayRandomWords(randomWords);

                do {
                    int subChoice;
                    cout << "\n1. Найти слово" << endl;
                    cout << "2. Вернуться в основное меню" << endl;
                    cout << "Выберите действие: ";
                    cin >> subChoice;

                    if (subChoice == 2) {
                        returnToMainMenu = true;
                        break;
                    }

                    string testWord;
                    cout << "\nВведите слово для поиска: ";
                    cin >> testWord;

                    auto startSearch1 = high_resolution_clock::now();
                    int neUpCount = neUpTable.FindRecord(testWord);
                    auto endSearch1 = high_resolution_clock::now();
                    duration<double> neUpTime = endSearch1 - startSearch1;
                    cout << "Неупорядоченная таблица на основе массива: Найдено " << neUpCount << " вхождений за " << neUpTime.count() << " секунд" << endl;

                    auto startSearch5 = high_resolution_clock::now();
                    int listCount = listTable.FindRecord(testWord);
                    auto endSearch5 = high_resolution_clock::now();
                    duration<double> listTime = endSearch5 - startSearch5;
                    cout << "Неупорядоченная таблица на основе списка: Найдено " << listCount << " вхождений за " << listTime.count() << " секунд" << endl;

                    auto startSearch2 = high_resolution_clock::now();
                    int upCount = upTable.FindRecord(testWord);
                    auto endSearch2 = high_resolution_clock::now();
                    duration<double> upTime = endSearch2 - startSearch2;
                    cout << "Упорядоченная таблица (сортировка слиянием, бинарный поиск): Найдено " << upCount << " вхождений за " << upTime.count() << " секунд" << endl;

                    auto startSearch4 = high_resolution_clock::now();
                    int bstCount = bstTable.FindRecord(testWord);
                    auto endSearch4 = high_resolution_clock::now();
                    duration<double> bstTime = endSearch4 - startSearch4;
                    cout << "Упорядоченная таблица (бинарное дерево поиска): Найдено " << bstCount << " вхождений за " << bstTime.count() << " секунд" << endl;

                    auto startSearch3 = high_resolution_clock::now();
                    int hashCount = hashTable.FindRecord(testWord);
                    auto endSearch3 = high_resolution_clock::now();
                    duration<double> hashTime = endSearch3 - startSearch3;
                    cout << "Хеш-таблица: Найдено " << hashCount << " вхождений за " << hashTime.count() << " секунд" << endl;

                } while (true);

                if (returnToMainMenu) {
                    break;
                }

            } while (true);

            break;
        }

        }

    } while (choice != 0);

    return 0;
}