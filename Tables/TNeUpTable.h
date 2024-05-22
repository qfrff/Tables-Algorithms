#pragma once

#include "TTabRecord.h"

// Класс неупорядоченной таблицы (массив)
class TNeUpTable {
protected:
    TTabRecord* pFirst;
    int comparisonCount;
public:
    TNeUpTable() : pFirst(nullptr), comparisonCount(0) {}
    virtual int FindRecord(const std::string& k);
    virtual void InsRecord(const std::string& k, TData* pVal);
    virtual void DelRecord(const std::string& k);
    int GetComparisonCount() const { return comparisonCount; }
    void ResetComparisonCount() { comparisonCount = 0; }
};

int TNeUpTable::FindRecord(const std::string& k) {
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

void TNeUpTable::InsRecord(const std::string& k, TData* pVal) {
    TTabRecord* pRec = new TTabRecord(k, pVal);
    pRec->SetNext(pFirst);
    pFirst = pRec;
}

void TNeUpTable::DelRecord(const std::string& k) {
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