#pragma once 

#include "TNeUpTable.h" 
#include <vector> 
#include <string> 
#include <functional> 
#include <iostream> 

// Класс хеш-таблицы 
class THashTable : public TNeUpTable {
protected:
    std::vector<TTabRecord*> pList;
    int CurrList;
    std::vector<int> initialIndices;  // To store initial indices
    std::vector<int> resolvedIndices; // To store indices after resolving collisions
public:
    THashTable(int Size) : TNeUpTable(), pList(Size, nullptr) {}
    virtual int FindRecord(const std::string& k);
    virtual void InsRecord(const std::string& k, TData* pVal);
    virtual void DelRecord(const std::string& k);
    void DisplayRecords() const;
};

int THashTable::FindRecord(const std::string& k) {
    comparisonCount = 0;
    int hash = std::hash<std::string>{}(k) % pList.size();
    CurrList = hash;
    TTabRecord* pRec = pList[CurrList];
    int count = 0;
    while (pRec) {
        comparisonCount++;
        if (pRec->GetKey() == k) {
            count += pRec->GetDataPtr()->count; // Учитываем счетчик вхождений текущего узла 
            return count;
        }
        pRec = pRec->GetNext();
    }
    return count;
}

void THashTable::InsRecord(const std::string& k, TData* pVal) {
    int hash = std::hash<std::string>{}(k) % pList.size();
    initialIndices.push_back(hash);  // Сохраняем начальный индекс
    CurrList = hash;
    TTabRecord* pRec = pList[CurrList];
    TTabRecord* pPrev = nullptr;

    // Проверяем, есть ли уже такой ключ в списке по текущему индексу
    while (pRec) {
        if (pRec->GetKey() == k) {
            pRec->GetDataPtr()->count++; // Увеличиваем счетчик вхождений 
            resolvedIndices.push_back(CurrList); // Сохраняем разрешенный индекс
            return;
        }
        pPrev = pRec;
        pRec = pRec->GetNext();
    }

    // Если слово не найдено, создаем новую запись 
    TData* newData = new TData();
    newData->data = k;
    newData->count = 1;
    TTabRecord* newRec = new TTabRecord(k, newData);

    // Если в списке по текущему индексу ничего нет, вставляем новую запись
    if (!pPrev) {
        pList[CurrList] = newRec;
    }
    else { // Если уже есть записи, добавляем новую в конец цепочки
        pPrev->SetNext(newRec);
    }

    // Сохраняем разрешенный индекс
    resolvedIndices.push_back(CurrList);
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

void THashTable::DisplayRecords() const {
    std::cout << "Hash Table Records До Collision Resolution:\n";
    for (size_t i = 0; i < pList.size(); ++i) {
        TTabRecord* pRec = pList[i];
        while (pRec) {
            std::cout << "Index: " << i << " | Key: " << pRec->GetKey() << " | Data: " << pRec->GetDataPtr()->data
                << " | Count: " << pRec->GetDataPtr()->count << "\n";
            pRec = pRec->GetNext();
        }
    }

    std::cout << "\nHash Table Records После Collision Resolution:\n";
    for (size_t i = 0; i < pList.size(); ++i) {
        TTabRecord* pRec = pList[i];
        while (pRec) {
            std::cout << "Index: " << i << " | Key: " << pRec->GetKey() << " | Data: " << pRec->GetDataPtr()->data
                << " | Count: " << pRec->GetDataPtr()->count << "\n";
            pRec = pRec->GetNext();
        }
    }

    std::cout << "\nInitial Indices and Resolved Indices:\n";
    for (size_t i = 0; i < initialIndices.size(); ++i) {
        std::cout << "Initial Index: " << initialIndices[i] << " | Resolved Index: " << resolvedIndices[i] << "\n";
    }
}
