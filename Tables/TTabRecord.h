#pragma once

#include "TData.h"

// ����� ������ ��������������� �������
class TTabRecord {
protected:
    std::string Key; // ����
    TData* pData; // ������
    TTabRecord* pNext; // ��������� �� ��������� ������
public:
    TTabRecord(const std::string& key = "", TData* pVal = nullptr, TTabRecord* pN = nullptr) : Key(key), pData(pVal), pNext(pN) {}

    std::string GetKey() const { return Key; }
    TData* GetDataPtr() const { return pData; }
    void SetDataPtr(TData* pVal) { pData = pVal; }
    TTabRecord* GetNext() const { return pNext; }
    void SetNext(TTabRecord* pN) { pNext = pN; }
};