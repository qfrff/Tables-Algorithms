// THashTable.h

#pragma once

#include "TNeUpTable.h"
#include <vector>
#include <string>
#include <functional>
#include <iostream>

class THashTable : public TNeUpTable {
protected:
    std::vector<TTabRecord*> pList;
    int CurrList;
    std::vector<int> initialIndices;
    std::vector<int> resolvedIndices;
protected:
    std::vector<int> actualIndices;

public:
    THashTable(int Size) : TNeUpTable(), pList(Size, nullptr) {}
    virtual int FindRecord(const std::string& k);
    virtual void InsRecord(const std::string& k, TData* pVal);
    virtual void DelRecord(const std::string& k);
    void DisplayRecords();
    void DisplayResolvedIndices(); // ���������� ������ ������
};


int THashTable::FindRecord(const std::string& k) {
    comparisonCount = 0;
    int hash = std::hash<std::string>{}(k) % pList.size();
    CurrList = hash;
    TTabRecord* pRec = pList[CurrList];
    int count = 0;
    bool found = false;
    while (pRec) {
        comparisonCount++;
        if (pRec->GetKey() == k) {
            count += pRec->GetDataPtr()->count; // ��������� ������� ��������� �������� ����
            found = true;
            break;
        }
        pRec = pRec->GetNext();
    }
    if (!found) {
        comparisonCount++; // ��������� ��� ������� ���� �������� ���������
    }
    return count;
}

void THashTable::InsRecord(const std::string& k, TData* pVal) {
    int hash = std::hash<std::string>{}(k) % pList.size();
    initialIndices.push_back(hash);  // ��������� ��������� ������
    CurrList = hash;
    TTabRecord* pRec = pList[CurrList];
    TTabRecord* pPrev = nullptr;

    // ���������, ���� �� ��� ����� ���� � ������ �� �������� �������
    while (pRec) {
        if (pRec->GetKey() == k) {
            pRec->GetDataPtr()->count++; // ����������� ������� ���������
            resolvedIndices.push_back(CurrList); // ��������� ����������� ������
            return;
        }
        pPrev = pRec;
        pRec = pRec->GetNext();
    }

    // ���� ����� �� �������, ������� ����� ������
    TData* newData = new TData();
    newData->data = k;
    newData->count = 1;
    TTabRecord* newRec = new TTabRecord(k, newData);

    // ���� � ������ �� �������� ������� ������ ���, ��������� ����� ������
    if (!pPrev) {
        pList[CurrList] = newRec;
    }
    else { // ���� ��� ���� ������, ��������� ����� � ����� �������
        pPrev->SetNext(newRec);
    }

    // ��������� ����������� ������ ������ ����� ��������� ���������� ��������
    resolvedIndices.push_back(CurrList);
    actualIndices.push_back(CurrList); // ��������� ���������� �������������� ������ ����� ���������� ��������
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

void THashTable::DisplayResolvedIndices() {
    std::cout << "Resolved Indices:\n";
    for (size_t i = 0; i < actualIndices.size(); ++i) {
        std::cout << actualIndices[i] << "\n";
    }
}


void THashTable::DisplayRecords() {
    std::cout << "Hash Table �� Collision Resolution:\n";
    for (size_t i = 0; i < pList.size(); ++i) {
        TTabRecord* pRec = pList[i];
        while (pRec) {
            std::cout << "Index: " << i << " | Key: " << pRec->GetKey() << " | Data: " << pRec->GetDataPtr()->data
                << " | Count: " << pRec->GetDataPtr()->count << "\n";
            pRec = pRec->GetNext();
        }
    }

    /*std::cout << "\nHash Table ����� Collision Resolution:\n";
    for (size_t i = 0; i < pList.size(); ++i) {
        TTabRecord* pRec = pList[i];
        while (pRec) {
            std::cout << "Index: " << i << " | Key: " << pRec->GetKey() << " | Data: " << pRec->GetDataPtr()->data
                << " | Count: " << pRec->GetDataPtr()->count << "\n";
            pRec = pRec->GetNext();
        }
    }

    std::vector<bool> usedIndices(pList.size(), false);
    for (size_t i = 0; i < initialIndices.size(); ++i) {
        usedIndices[initialIndices[i]] = true;
        usedIndices[resolvedIndices[i]] = true;
    }

    std::cout << "\nInitial Indices and Resolved Indices:\n";
    for (size_t i = 0; i < initialIndices.size(); ++i) {
        if (initialIndices[i] == resolvedIndices[i]) {
            for (size_t j = 0; j < usedIndices.size(); ++j) {
                if (!usedIndices[j]) {
                    resolvedIndices[i] = j;
                    usedIndices[j] = true;
                    break;
                }
            }
        }
        std::cout << "Initial Index: " << initialIndices[i] << " | Resolved Index: " << resolvedIndices[i] << "\n";
    }*/
}

