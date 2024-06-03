#pragma once

#include "TNeUpTable.h"
#include <iostream>

// ����� ��������������� ������� (������)
class TNeUpTableList : public TNeUpTable {
public:
    TNeUpTableList() : TNeUpTable() {}
    ~TNeUpTableList() { Clear(); }

    void Insert(const std::string& k, TData* pData);
    int FindRecord(const std::string& k);
    void Delete(const std::string& k);
    void Clear();

    // ����� ��� ����������� �������� ������
    void DisplayRecords() const;
};

void TNeUpTableList::Insert(const std::string& k, TData* pData) {
    TTabRecord* pRec = pFirst;
    TTabRecord* pPrev = nullptr;

    // ���� ��������� ������� ������
    while (pRec) {
        if (pRec->GetKey() == k) {
            pRec->GetDataPtr()->count++; // ����������� ������� ���������
            return;
        }
        pPrev = pRec;
        pRec = pRec->GetNext();
    }

    // ���� ����� �� �������, ������� ����� ������
    TData* newData = new TData();
    newData->data = k;
    newData->count = 1;
    pRec = new TTabRecord(k, newData);

    // ���� ������ ����, ����� ������ ���������� ������ ���������
    if (!pFirst) {
        pFirst = pRec;
    }
    else { // � ��������� ������, ��������� ����� ������ � ����� ������
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