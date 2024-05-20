#pragma once

#include <string>

// ����� ������ ��� �������
class TData {
public:
    int count; // ���������� ���������
    std::string data; // ������

    TData(const std::string& data = "", int count = 1) : data(data), count(count) {}

    // �������� ����� �������
    virtual TData* GetCopy() const {
        return new TData(data, count);
    }

    // �������� ���� ������
    virtual std::string GetKey() const {
        return data;
    }
};