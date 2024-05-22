#pragma once

#include "TNeUpTable.h"

// Класс неупорядоченной таблицы (список)
class TNeUpTableList : public TNeUpTable {
public:
    TNeUpTableList() : TNeUpTable() {}
    ~TNeUpTableList() { Clear(); }

    void Insert(const std::string& k, TData* pData);
    int FindRecord(const std::string& k);
    void Delete(const std::string& k);
    void Clear();
};

void TNeUpTableList::Insert(const std::string& k, TData* pData) {
    TTabRecord* pRec = new TTabRecord(k, pData);
    pRec->SetNext(pFirst);
    pFirst = pRec;
}

int TNeUpTableList::FindRecord(const std::string& k) {
    comparisonCount = 0;
    TTabRecord* pRec = pFirst;
    int count = 0;
    while (pRec) {
        comparisonCount++;
        if (pRec->GetKey() == k)
            count++;
        pRec = pRec->GetNext();
    }
    return count;
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