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

// ����� ������ ��� �������
class TData {
public:
    int count; // ���������� ���������
    string data; // ������

    TData(const string& data = "", int count = 1) : data(data), count(count) {}

    // �������� ����� �������
    virtual TData* GetCopy() const {
        return new TData(data, count);
    }

    // �������� ���� ������
    virtual string GetKey() const {
        return data;
    }
};

// ����� ������ ��������������� �������
class TTabRecord {
protected:
    string Key; // ����
    TData* pData; // ������
    TTabRecord* pNext; // ��������� �� ��������� ������
public:
    TTabRecord(const string& key = "", TData* pVal = nullptr, TTabRecord* pN = nullptr) : Key(key), pData(pVal), pNext(pN) {}
    string GetKey() const { return Key; }
    TData* GetDataPtr() const { return pData; }
    void SetDataPtr(TData* pVal) { pData = pVal; }
    TTabRecord* GetNext() const { return pNext; }
    void SetNext(TTabRecord* pN) { pNext = pN; }
};

// ���� ��������� ������ ������
class BSTNode {
public:
    string Key; // ����
    TData* pData; // ������
    BSTNode* Left; // ����� �������
    BSTNode* Right; // ������ �������

    BSTNode(const string& key = "", TData* pVal = nullptr, BSTNode* left = nullptr, BSTNode* right = nullptr) :
        Key(key), Left(left), Right(right) {
        pData = new TData();
        pData->count = 1;
        pData->data = key;
    }
};

// ����� ��������������� ������� (������)
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

// ����� ��������������� ������� (������)
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

// ����� ������������� ������� (������, ���������� ��������, �������� �����)
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

// ����� ������������� ������� (�������� ������ ������)
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

// ����� ���-�������
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
    ofstream file("C:\\Users\\����\\Desktop\\key.txt", ios::app);
    if (file.is_open()) {
        file << key << endl;
        file.close();
    }
    else {
        cerr << "�� ���������� ������� ����" << endl;
    }
}

// ������� ��� ������ ������ �� ����� � ������ �� �����
void ReadFromFile() {
    ifstream file("C:\\Users\\����\\Desktop\\key.txt");
    if (file.is_open()) {
        cout << "\n���������� ����� key.txt:" << endl;
        string key;
        while (getline(file, key)) {
            cout << key << endl;
        }
        file.close();
    }
    else {
        cerr << "�� ���������� ������� ����" << endl;
    }
}

// ������� ��� �������� ����� �� �����
void RemoveFromFile(const string& key) {
    ifstream inputFile("C:\\Users\\����\\Desktop\\key.txt");
    ofstream tempFile("C:\\Users\\����\\Desktop\\temp.txt");

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
    remove("C:\\Users\\����\\Desktop\\key.txt");
    // ��������������� ��������� ���� � ������ ���
    rename("C:\\Users\\����\\Desktop\\temp.txt", "C:\\Users\\����\\Desktop\\key.txt");

    if (!found) {
        cerr << "����� �� ������� � �����" << endl;
    }
}

// ������� ������������ ������� � ����� ������������������ ������
int main() {
    setlocale(LC_ALL, "Russian");

    TNeUpTable neUpTable;
    UnorderedListTable listTable;
    TUpTable upTable(25); // ����� ��������
    OrderedBSTTable bstTable;
    THashTable hashTable(25); // ����� ��������

    string key;
    int choice;

    do {
        cout << "\n����:" << endl;
        cout << "1. �������� �����" << endl;
        cout << "2. ����� �����" << endl;
        cout << "3. ������� �����" << endl;
        cout << "4. �������� ������" << endl;
        cout << "5. �������� ����" << endl;
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

            int neUpCount = neUpTable.FindRecord(key);
            cout << "��������������� ������� �� ������ �������: ������� " << neUpCount << " ���������" << endl;

            int listCount = listTable.FindRecord(key);
            cout << "��������������� ������� �� ������ ������: ������� " << listCount << " ���������" << endl;

            int upCount = upTable.FindRecord(key);
            cout << "������������� ������� (���������� ��������, �������� �����): ������� " << upCount << " ���������" << endl;

            int bstCount = bstTable.FindRecord(key);
            cout << "������������� ������� (�������� ������ ������): ������� " << bstCount << " ���������" << endl;

            int hashCount = hashTable.FindRecord(key);
            cout << "���-�������: ������� " << hashCount << " ���������" << endl;
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
                hashTable = THashTable(tableSize);
                bstTable = OrderedBSTTable();
                listTable = UnorderedListTable();

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
                    cout << "�������� ��������: ";
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
                    cout << "��������������� ������� �� ������ �������: ������� " << neUpCount << " ��������� �� " << neUpTime.count() << " ������" << endl;

                    auto startSearch5 = high_resolution_clock::now();
                    int listCount = listTable.FindRecord(testWord);
                    auto endSearch5 = high_resolution_clock::now();
                    duration<double> listTime = endSearch5 - startSearch5;
                    cout << "��������������� ������� �� ������ ������: ������� " << listCount << " ��������� �� " << listTime.count() << " ������" << endl;

                    auto startSearch2 = high_resolution_clock::now();
                    int upCount = upTable.FindRecord(testWord);
                    auto endSearch2 = high_resolution_clock::now();
                    duration<double> upTime = endSearch2 - startSearch2;
                    cout << "������������� ������� (���������� ��������, �������� �����): ������� " << upCount << " ��������� �� " << upTime.count() << " ������" << endl;

                    auto startSearch4 = high_resolution_clock::now();
                    int bstCount = bstTable.FindRecord(testWord);
                    auto endSearch4 = high_resolution_clock::now();
                    duration<double> bstTime = endSearch4 - startSearch4;
                    cout << "������������� ������� (�������� ������ ������): ������� " << bstCount << " ��������� �� " << bstTime.count() << " ������" << endl;

                    auto startSearch3 = high_resolution_clock::now();
                    int hashCount = hashTable.FindRecord(testWord);
                    auto endSearch3 = high_resolution_clock::now();
                    duration<double> hashTime = endSearch3 - startSearch3;
                    cout << "���-�������: ������� " << hashCount << " ��������� �� " << hashTime.count() << " ������" << endl;

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