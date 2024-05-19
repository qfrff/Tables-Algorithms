#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>

#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <locale.h>

#include <fstream>

using namespace std;
using namespace chrono;

// Класс данных для таблицы
class TData {
public:
    int count; // Количество вхождений
    string data; // Данные

    TData(const string& data = "", int count = 1) : data(data), count(count) {}

    // Получить копию объекта
    virtual TData* GetCopy() const {
        return new TData(data, count);
    }

    // Получить ключ данных
    virtual string GetKey() const {
        return data;
    }
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

// Узел бинарного дерева поиска
class BSTNode {
public:
    string Key; // Ключ
    TData* pData; // Данные
    BSTNode* Left; // Левый потомок
    BSTNode* Right; // Правый потомок

    BSTNode(const string& key = "", TData* pVal = nullptr, BSTNode* left = nullptr, BSTNode* right = nullptr) :
        Key(key), Left(left), Right(right) {
        pData = new TData();
        pData->count = 1;
        pData->data = key;
    }
};

// Класс неупорядоченной таблицы (массив)
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

// Класс неупорядоченной таблицы (список)
class UnorderedListTable : public TNeUpTable {
public:
    UnorderedListTable() : TNeUpTable() {}
    ~UnorderedListTable() { Clear(); }

    void Insert(const string& k, TData* pData);
    int FindRecord(const string& k);
    void Delete(const string& k);
    void Clear();
};

void UnorderedListTable::Insert(const string& k, TData* pData) {
    TTabRecord* pRec = new TTabRecord(k, pData);
    pRec->SetNext(pFirst);
    pFirst = pRec;
}

int UnorderedListTable::FindRecord(const string& k) {
    TTabRecord* pRec = pFirst;
    int count = 0;
    while (pRec) {
        if (pRec->GetKey() == k)
            count++;
        pRec = pRec->GetNext();
    }
    return count;
}

void UnorderedListTable::Delete(const string& k) {
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

void UnorderedListTable::Clear() {
    TTabRecord* pCurr = pFirst;
    while (pCurr) {
        TTabRecord* pTemp = pCurr;
        pCurr = pCurr->GetNext();
        delete pTemp;
    }
    pFirst = nullptr;
}

// Класс упорядоченной таблицы (массив, сортировка слиянием, бинарный поиск)
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
    if (!pFirst)
        return 0;

    int left = 0, right = DataCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        TTabRecord* midRec = pFirst;
        for (int i = 0; i < mid; ++i)
            midRec = midRec->GetNext();

        if (midRec->GetKey() == k) {
            int count = 1;
            TTabRecord* prevRec = midRec;
            while (midRec->GetNext() && midRec->GetNext()->GetKey() == k) {
                count++;
                midRec = midRec->GetNext();
            }
            return count;
        }
        else if (midRec->GetKey() < k)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return 0;
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

// Класс упорядоченной таблицы (бинарное дерево поиска)
class OrderedBSTTable {
private:
    BSTNode* Root;
public:
    OrderedBSTTable() : Root(nullptr) {}
    ~OrderedBSTTable() { Clear(); }

    void Insert(const string& k, TData* pData);
    int FindRecord(const string& k);
    void Delete(const string& k);
    void Clear();

private:
    void InsertHelper(BSTNode*& node, const string& k, TData* pData);
    int FindRecordHelper(BSTNode* node, const string& k);
    void DeleteHelper(BSTNode*& node, const string& k);
    void ClearHelper(BSTNode*& node);
};

void OrderedBSTTable::Insert(const string& k, TData* pData) {
    InsertHelper(Root, k, pData);
}

void OrderedBSTTable::InsertHelper(BSTNode*& node, const string& k, TData* pData) {
    if (!node) {
        node = new BSTNode(k, pData);
    }
    else if (k < node->Key) {
        InsertHelper(node->Left, k, pData);
    }
    else if (k > node->Key) {
        InsertHelper(node->Right, k, pData);
    }
    else {
        if (!node->pData) {
            node->pData = pData;
        }
        else {
            node->pData->count++;
        }
    }
}

int OrderedBSTTable::FindRecord(const string& k) {
    return FindRecordHelper(Root, k);
}

int OrderedBSTTable::FindRecordHelper(BSTNode* node, const string& k) {
    if (!node)
        return 0;

    if (node->Key == k)
        return node->pData ? node->pData->count : 0;

    if (k < node->Key)
        return FindRecordHelper(node->Left, k);
    else
        return FindRecordHelper(node->Right, k);
}


void OrderedBSTTable::Delete(const string& k) {
    DeleteHelper(Root, k);
}

void OrderedBSTTable::DeleteHelper(BSTNode*& node, const string& k) {
    if (!node)
        return;
    if (k < node->Key) {
        DeleteHelper(node->Left, k);
    }
    else if (k > node->Key) {
        DeleteHelper(node->Right, k);
    }
    else {

        if (node->pData && node->pData->count > 1) {
            node->pData->count--;
            return;
        }
        if (!node->Left) {
            BSTNode* temp = node->Right;
            delete node;
            node = temp;
        }
        else if (!node->Right) {
            BSTNode* temp = node->Left;
            delete node;
            node = temp;
        }
        else {
            BSTNode* temp = node->Right;
            while (temp->Left)
                temp = temp->Left;
            node->Key = temp->Key;
            node->pData = temp->pData;
            DeleteHelper(node->Right, temp->Key);
        }
    }
}

void OrderedBSTTable::Clear() {
    ClearHelper(Root);
    Root = nullptr;
}

void OrderedBSTTable::ClearHelper(BSTNode*& node) {
    if (!node)
        return;
    ClearHelper(node->Left);
    ClearHelper(node->Right);
    delete node;
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
    UnorderedListTable listTable;
    TUpTable upTable(25); // можно изменить
    OrderedBSTTable bstTable;
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

            int neUpCount = neUpTable.FindRecord(key);
            cout << "Неупорядоченная таблица на основе массива: Найдено " << neUpCount << " вхождений" << endl;

            int listCount = listTable.FindRecord(key);
            cout << "Неупорядоченная таблица на основе списка: Найдено " << listCount << " вхождений" << endl;

            int upCount = upTable.FindRecord(key);
            cout << "Упорядоченная таблица (сортировка слиянием, бинарный поиск): Найдено " << upCount << " вхождений" << endl;

            int bstCount = bstTable.FindRecord(key);
            cout << "Упорядоченная таблица (бинарное дерево поиска): Найдено " << bstCount << " вхождений" << endl;

            int hashCount = hashTable.FindRecord(key);
            cout << "Хеш-таблица: Найдено " << hashCount << " вхождений" << endl;
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
                hashTable = THashTable(tableSize);
                bstTable = OrderedBSTTable();
                listTable = UnorderedListTable();

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