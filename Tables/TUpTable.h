#pragma once

#include "TNeUpTable.h"

// Класс упорядоченной таблицы (массив, сортировка слиянием, бинарный поиск)
class TUpTable : public TNeUpTable {
protected:
    void SortData();
    void MergeSort(TTabRecord** arr, int l, int r);
    void Merge(TTabRecord** arr, int l, int m, int r);
public:
    TUpTable(int Size) : TNeUpTable(), TabSize(Size), DataCount(0) {}
    virtual int FindRecord(const std::string& k);
    virtual void InsRecord(const std::string& k, TData* pVal);
    virtual void DelRecord(const std::string& k);
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

    pFirst = arr[0];
    pRec = pFirst;
    for (int j = 1; j < DataCount; ++j) {
        pRec->SetNext(arr[j]);
        pRec = pRec->GetNext();
    }
    pRec->SetNext(nullptr);

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

int TUpTable::FindRecord(const std::string& k) {
    if (!pFirst)
        return 0;

    int count = 0;
    TTabRecord* pRec = pFirst;
    while (pRec) {
        if (pRec->GetKey() == k)
            count++;
        pRec = pRec->GetNext();
    }
    return count;
}

void TUpTable::InsRecord(const std::string& k, TData* pVal) {
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
}

void TUpTable::DelRecord(const std::string& k) {
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