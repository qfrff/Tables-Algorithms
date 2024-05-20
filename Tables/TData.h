#pragma once

#include <string>

// Класс данных для таблицы
class TData {
public:
    int count; // Количество вхождений
    std::string data; // Данные

    TData(const std::string& data = "", int count = 1) : data(data), count(count) {}

    // Получить копию объекта
    virtual TData* GetCopy() const {
        return new TData(data, count);
    }

    // Получить ключ данных
    virtual std::string GetKey() const {
        return data;
    }
};