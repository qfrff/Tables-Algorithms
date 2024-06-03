#pragma once

#include "TNeUpTable.h"
#include <iostream>

// Класс неупорядоченной таблицы (список)
class TNeUpTableList : public TNeUpTable {
public:
    TNeUpTableList() : TNeUpTable() {}
    ~TNeUpTableList() { Clear(); }

    void Insert(const std::string& k, TData* pData);
    int FindRecord(const std::string& k);
    void Delete(const std::string& k);
    void Clear();

    // Метод для отображения хранения данных
    void DisplayRecords() const;
};

void TNeUpTableList::Insert(const std::string& k, TData* pData) {
    TTabRecord* pRec = pFirst;
    TTabRecord* pPrev = nullptr;

    // Ищем последний элемент списка
    while (pRec) {
        if (pRec->GetKey() == k) {
            pRec->GetDataPtr()->count++; // Увеличиваем счетчик вхождений
            return;
        }
        pPrev = pRec;
        pRec = pRec->GetNext();
    }

    // Если слово не найдено, создаем новую запись
    TData* newData = new TData();
    newData->data = k;
    newData->count = 1;
    pRec = new TTabRecord(k, newData);

    // Если список пуст, новая запись становится первым элементом
    if (!pFirst) {
        pFirst = pRec;
    }
    else { // В противном случае, добавляем новую запись в конец списка
        pPrev->SetNext(pRec);
    }
}

int TNeUpTableList::FindRecord(const std::string& k) {
    comparisonCount = 0;
    TTabRecord* pRec = pFirst;
    while (pRec) {
        comparisonCount++;
        if (pRec->GetKey() == k)
            return pRec->GetDataPtr()->count;
        pRec = pRec->GetNext();
    }
    return 0; 
}

void TNeUpTableList::Delete(const std::string& k) {
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

void TNeUpTableList::Clear() {
    TTabRecord* pCurr = pFirst;
    while (pCurr) {
        TTabRecord* pTemp = pCurr;
        pCurr = pCurr->GetNext();
        delete pTemp;
    }
    pFirst = nullptr;
}

void TNeUpTableList::DisplayRecords() const {
    TTabRecord* pRec = pFirst;
    while (pRec) {
        std::cout << "Key: " << pRec->GetKey() << ", Data: " << pRec->GetDataPtr()->data << ", Count: " << pRec->GetDataPtr()->count << std::endl;
        pRec = pRec->GetNext();
    }
}