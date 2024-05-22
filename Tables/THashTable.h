#pragma once

#include "TNeUpTable.h"
#include <vector>
#include <string>
#include <functional>

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
    comparisonCount = 0;
    int hash = std::hash<std::string>{}(k) % pList.size();
    CurrList = hash;
    TTabRecord* pRec = pList[CurrList];
    int count = 0;
    while (pRec) {
        comparisonCount++;
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