#pragma once

#include "TNeUpTable.h"
#include <iostream>

// Класс упорядоченной таблицы (массив, сортировка слиянием, бинарный поиск)
class TUpTable : public TNeUpTable {
protected:
    void SortData();
    void MergeSort(TTabRecord** arr, int l, int r);
    void Merge(TTabRecord** arr, int l, int m, int r);
    std::string GetKeyAtIndex(int index);
    int comparisonCount;
public:
    TUpTable(int Size) : TNeUpTable(), TabSize(Size), DataCount(0), comparisonCount(0) {}
    virtual int FindRecord(const std::string& k);
    virtual void InsRecord(const std::string& k, TData* pVal);
    virtual void DelRecord(const std::string& k);
    int GetComparisonCount() const { return comparisonCount; }
    void ResetComparisonCount() { comparisonCount = 0; }

    // Метод для отображения хранения данных
    void DisplayRecords() const;
private:
    int TabSize;
    int DataCount;
};

std::string TUpTable::GetKeyAtIndex(int index) {
    TTabRecord* current = pFirst;
    int i = 0;

    while (current && i < index) {
        current = current->GetNext();
        i++;
    }

    if (current)
        return current->GetKey();
    else
        return "";
}

int TUpTable::FindRecord(const std::string& k) {
    comparisonCount = 0;
    int count = 0;
    int left = 0;
    int right = DataCount - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        comparisonCount++;
        const std::string& midKey = GetKeyAtIndex(mid);

        if (midKey == k) {
            count++;

            int idx = mid - 1;
            while (idx >= 0 && GetKeyAtIndex(idx) == k) {
                count++;
                idx--;
            }

            idx = mid + 1;
            while (idx < DataCount && GetKeyAtIndex(idx) == k) {
                count++;
                idx++;
            }

            return count;
        }
        else if (midKey < k) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return count;
}

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

void TUpTable::InsRecord(const std::string& k, TData* pVal) {
    TTabRecord* pRec = pFirst;
    TTabRecord* pPrev = nullptr;
    while (pRec && pRec->GetKey() < k) {
        pPrev = pRec;
        pRec = pRec->GetNext();
    }

    if (pRec && pRec->GetKey() == k) {
        pRec->GetDataPtr()->count++; // Увеличиваем счетчик вхождений
        return;
    }

    // Если слово не найдено, создаем новую запись
    TData* newData = new TData();
    newData->data = k;
    newData->count = 1;
    pRec = new TTabRecord(k, newData);
    if (!pPrev) {
        pRec->SetNext(pFirst);
        pFirst = pRec;
    }
    else {
        pRec->SetNext(pPrev->GetNext());
        pPrev->SetNext(pRec);
    }
    DataCount++;
    SortData();
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

void TUpTable::DisplayRecords() const {
    TTabRecord* pRec = pFirst;
    while (pRec) {
        std::cout << "Key: " << pRec->GetKey() << ", Data: " << pRec->GetDataPtr()->data << ", Count: " << pRec->GetDataPtr()->count << std::endl;
        pRec = pRec->GetNext();
    }
}