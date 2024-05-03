#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <random>
#include <thread>

using namespace std;
using namespace chrono;

// Класс данных для таблицы
class TData {
public:
    virtual ~TData() {}
    virtual TData* GetCopy() const = 0; // Получить копию объекта
    virtual string GetKey() const = 0; // Получить ключ данных
};

// Класс записи неупорядоченной таблицы
class TTabRecord {
protected:
    string Key; // Ключ
    TData* pData; // Данные
    TTabRecord* pNext; // Указатель на следующую запись
public:
    TTabRecord(const string& key = "", TData* pVal = nullptr, TTabRecord* pN = nullptr) : Key(key), pData(pVal), pNext(pN) {}
    string GetKey() const { return Key; }
    TData* GetDataPtr() const { return pData; }
    void SetDataPtr(TData* pVal) { pData = pVal; }
    TTabRecord* GetNext() const { return pNext; }
    void SetNext(TTabRecord* pN) { pNext = pN; }
};

// Класс неупорядоченной таблицы
class TNeUpTable {
protected:
    TTabRecord* pFirst;
public:
    TNeUpTable() : pFirst(nullptr) {}
    virtual int FindRecord(const string& k);
    virtual void InsRecord(const string& k, TData* pVal);
    virtual void DelRecord(const string& k);
};

int TNeUpTable::FindRecord(const string& k) {
    TTabRecord* pRec = pFirst;
    int count = 0;
    while (pRec) {
        if (pRec->GetKey() == k)
            count++;
        pRec = pRec->GetNext();
    }
    return count;
}

void TNeUpTable::InsRecord(const string& k, TData* pVal) {
    TTabRecord* pRec = new TTabRecord(k, pVal);
    pRec->SetNext(pFirst);
    pFirst = pRec;
}

void TNeUpTable::DelRecord(const string& k) {
    if (!pFirst)
        return;
    if (pFirst->GetKey() == k) {
        TTabRecord* pTemp = pFirst;
        pFirst = pFirst->GetNext();
        delete pTemp;
        return;
    }
    TTabRecord* pPrev = pFirst;
    TTabRecord* pCurr = pFirst->GetNext();
    while (pCurr && pCurr->GetKey() != k) {
        pPrev = pCurr;
        pCurr = pCurr->GetNext();
    }
    if (pCurr) {
        pPrev->SetNext(pCurr->GetNext());
        delete pCurr;
    }
}

// Класс упорядоченной таблицы
class TUpTable : public TNeUpTable {
protected:
    void SortData();
    void MergeSort(TTabRecord** arr, int l, int r);
    void Merge(TTabRecord** arr, int l, int m, int r);
public:
    TUpTable(int Size) : TNeUpTable(), TabSize(Size), DataCount(0) {}
    virtual int FindRecord(const string& k);
    virtual void InsRecord(const string& k, TData* pVal);
    virtual void DelRecord(const string& k);
private:
    int TabSize;
    int DataCount;
};

void TUpTable::SortData() {
    if (DataCount <= 1)
        return;

    TTabRecord** arr = new TTabRecord * [DataCount];
    int i = 0;
    TTabRecord* pRec = pFirst;
    while (pRec) {
        arr[i++] = pRec;
        pRec = pRec->GetNext();
    }

    MergeSort(arr, 0, DataCount - 1);

    pRec = pFirst;
    for (int j = 0; j < DataCount; ++j) {
        *pRec = *arr[j];
        pRec = pRec->GetNext();
    }

    delete[] arr;
}

void TUpTable::MergeSort(TTabRecord** arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);
        Merge(arr, l, m, r);
    }
}

void TUpTable::Merge(TTabRecord** arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    TTabRecord** L = new TTabRecord * [n1];
    TTabRecord** R = new TTabRecord * [n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->GetKey() <= R[j]->GetKey()) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

int TUpTable::FindRecord(const string& k) {
    int count = 0;

    TTabRecord* pRec = pFirst;
    while (pRec) {
        if (pRec->GetKey() == k) {
            count++;
            TTabRecord* nextRec = pRec->GetNext();
            while (nextRec && nextRec->GetKey() == k) {
                count++;
                nextRec = nextRec->GetNext();
            }
            return count;
        }
        pRec = pRec->GetNext();
    }

    return count;
}

void TUpTable::InsRecord(const string& k, TData* pVal) {
    if (DataCount < TabSize) {
        TTabRecord* pRec = new TTabRecord(k, pVal);
        if (!pFirst || k < pFirst->GetKey()) {
            pRec->SetNext(pFirst);
            pFirst = pRec;
        }
        else {
            TTabRecord* pPrev = pFirst;
            TTabRecord* pCurr = pFirst->GetNext();
            while (pCurr && pCurr->GetKey() < k) {
                pPrev = pCurr;
                pCurr = pCurr->GetNext();
            }
            pPrev->SetNext(pRec);
            pRec->SetNext(pCurr);
        }
        DataCount++;
    }
    SortData();
}

void TUpTable::DelRecord(const string& k) {
    if (!pFirst)
        return;
    if (pFirst->GetKey() == k) {
        TTabRecord* pTemp = pFirst;
        pFirst = pFirst->GetNext();
        delete pTemp;
        DataCount--;
        return;
    }
    TTabRecord* pPrev = pFirst;
    TTabRecord* pCurr = pFirst->GetNext();
    while (pCurr && pCurr->GetKey() != k) {
        pPrev = pCurr;
        pCurr = pCurr->GetNext();
    }
    if (pCurr) {
        pPrev->SetNext(pCurr->GetNext());
        delete pCurr;
        DataCount--;
    }
}

// Класс хеш-таблицы
class THashTable : public TNeUpTable {
protected:
    std::vector<TTabRecord*> pList;
    int CurrList;
public:
    THashTable(int Size) : TNeUpTable(), pList(Size, nullptr) {}
    virtual int FindRecord(const std::string& k);
    virtual void InsRecord(const std::string& k, TData* pVal);
    virtual void DelRecord(const std::string& k);
};

int THashTable::FindRecord(const std::string& k) {
    int hash = std::hash<std::string>{}(k) % pList.size();
    CurrList = hash;
    TTabRecord* pRec = pList[CurrList];
    int count = 0;
    while (pRec) {
        if (pRec->GetKey() == k)
            count++;
        pRec = pRec->GetNext();
    }
    return count;
}

void THashTable::InsRecord(const std::string& k, TData* pVal) {
    int hash = std::hash<std::string>{}(k) % pList.size();
    CurrList = hash;
    if (!pList[CurrList]) {
        pList[CurrList] = new TTabRecord(k, pVal);
    }
    else {
        TTabRecord* pRec = pList[CurrList];
        while (pRec->GetNext())
            pRec = pRec->GetNext();
        pRec->SetNext(new TTabRecord(k, pVal));
    }
}

void THashTable::DelRecord(const std::string& k) {
    int hash = std::hash<std::string>{}(k) % pList.size();
    CurrList = hash;
    TTabRecord* pRec = pList[CurrList];
    if (!pRec)
        return;
    if (pRec->GetKey() == k) {
        pList[CurrList] = pRec->GetNext();
        delete pRec;
        return;
    }
    while (pRec->GetNext() && pRec->GetNext()->GetKey() != k)
        pRec = pRec->GetNext();
    if (!pRec->GetNext())
        return;
    TTabRecord* pDelRec = pRec->GetNext();
    pRec->SetNext(pDelRec->GetNext());
    delete pDelRec;
}

// Вектор
vector<string> GenerateRandomWords(const vector<string>& words, int count) {
    vector<string> randomWords;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, words.size() - 1);

    for (int i = 0; i < count; ++i) {
        randomWords.push_back(words[dis(gen)]);
    }

    return randomWords;
}

// Функция пользователя системы и тесты производительности таблиц

int main() {
    setlocale(LC_ALL, "Russian");

    TNeUpTable neUpTable;
    TUpTable upTable(25); // Размер упорядоченной таблицы
    THashTable hashTable(25);   // Размер хеш-таблицы

    string key;
    int choice;

    int neUpInserts = 0, upInserts = 0, hashInserts = 0;

    do {
        cout << "\nМеню:" << endl;
        cout << "1. Вставить слово" << endl;
        cout << "2. Найти слово" << endl;
        cout << "3. Удалить слово" << endl;
        cout << "4. Провести тест" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            // Вставка слова
            cout << "Введите слово: ";
            cin >> key;
            neUpTable.InsRecord(key, nullptr);
            upTable.InsRecord(key, nullptr);
            hashTable.InsRecord(key, nullptr);
            break;
        }

        case 2: {
            // Найти слово
            cout << "Введите слово для поиска: ";
            cin >> key;

            // Поиск в неупорядоченной таблице
            int neUpCount = neUpTable.FindRecord(key);
            cout << "Неупорядоченная таблица: Найдено " << neUpCount << " вхождений" << endl;

            // Поиск в упорядоченной таблице
            int upCount = upTable.FindRecord(key);
            cout << "Упорядоченная таблица: Найдено " << upCount << " вхождений" << endl;

            // Поиск в хеш-таблице
            int hashCount = hashTable.FindRecord(key);
            cout << "Хеш-таблица: Найдено " << hashCount << " вхождений" << endl;
            break;
        }

        case 3: {
            // Удалить слово
            cout << "Введите слово для удаления: ";
            cin >> key;
            neUpTable.DelRecord(key);
            upTable.DelRecord(key);
            hashTable.DelRecord(key);
            break;
        }

        case 4: {

            int tableSize;
            cout << "Введите размер таблиц: ";
            cin >> tableSize;

            // Очистка таблиц перед тестом
            neUpTable = TNeUpTable();
            upTable = TUpTable(tableSize);
            hashTable = THashTable(tableSize);
            cout << "\nПрограмма создает и заполняет неупорядоченные, \n";
            cout << "упорядоченные и хеш-таблицы большим количеством слов.\n";
            cout << "В качестве слов используются названия стран мира, такие как: \n";
            cout << "France, Spain, Mexico, USA, Turkey, Italy, Greece, UAE, Germany, Austria\n";
            cout << "Ожидание...\n";

            vector<string> words = { "France", "Spain", "Mexico", "USA", "Turkey", "Italy", "Greece", "UAE", "Germany", "Austria" };

            vector<string> randomWords = GenerateRandomWords(words, tableSize);

            auto startTables = high_resolution_clock::now();

            for (int i = 0; i < tableSize; ++i) {
                string randomWord = randomWords[i];
                neUpTable.InsRecord(randomWord, nullptr);
                upTable.InsRecord(randomWord, nullptr);
                hashTable.InsRecord(randomWord, nullptr);
            }

            auto endTables = high_resolution_clock::now();
            duration<double> tablesTime = endTables - startTables;

            string testWord;
            cout << "\nВведите слово для поиска: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, testWord);
            // cout << "Нужное нам слово: " << testWord << endl;

            auto startSearch1 = high_resolution_clock::now();
            int neUpCount = neUpTable.FindRecord(testWord);
            auto endSearch1 = high_resolution_clock::now();
            duration<double> neUpTime = endSearch1 - startSearch1;
            cout << "Неупорядоченная таблица: Найдено " << neUpCount << " вхождений за " << neUpTime.count() << " секунд" << endl;

            auto startSearch2 = high_resolution_clock::now();
            int upCount = upTable.FindRecord(testWord);
            auto endSearch2 = high_resolution_clock::now();
            duration<double> upTime = endSearch2 - startSearch2;
            cout << "Упорядоченная таблица: Найдено " << upCount << " вхождений за " << upTime.count() << " секунд" << endl;

            auto startSearch3 = high_resolution_clock::now();
            int hashCount = hashTable.FindRecord(testWord);
            auto endSearch3 = high_resolution_clock::now();
            duration<double> hashTime = endSearch3 - startSearch3;
            cout << "Хеш-таблица: Найдено " << hashCount << " вхождений за " << hashTime.count() << " секунд" << endl;

            break;
        }

        }
    } while (choice != 0);

    return 0;
}
