#pragma once

#include "TData.h"
#include <iostream>

// Узел бинарного дерева поиска
class BSTNode {
public:
    std::string Key; // Ключ
    TData* pData; // Данные
    BSTNode* Left; // Левый потомок
    BSTNode* Right; // Правый потомок

    BSTNode(const std::string& key = "", TData* pVal = nullptr, BSTNode* left = nullptr, BSTNode* right = nullptr) :
        Key(key), Left(left), Right(right) {
        pData = new TData();
        pData->count = 1;
        pData->data = key;
    }
};


class TBinarySearchTree {
private:
    BSTNode* Root;
    int comparisonCount;
public:
    TBinarySearchTree() : Root(nullptr), comparisonCount(0) {}
    ~TBinarySearchTree() { Clear(); }

    void Insert(const std::string& k, TData* pData);
    int FindRecord(const std::string& k);
    void Delete(const std::string& k);
    void Clear();
    int GetComparisonCount() const { return comparisonCount; }
    void ResetComparisonCount() { comparisonCount = 0; }

    void DisplayRecords() const;
    BSTNode* GetRoot() const; // Перенесенный метод
private:
    void InsertHelper(BSTNode*& node, const std::string& k, TData* pData);
    int FindRecordHelper(BSTNode* node, const std::string& k);
    void DeleteHelper(BSTNode*& node, const std::string& k);
    void ClearHelper(BSTNode*& node);
    void DisplayRecordsHelper(BSTNode* node) const;
};

void TBinarySearchTree::Insert(const std::string& k, TData* pData) {
    InsertHelper(Root, k, pData);
}

void TBinarySearchTree::InsertHelper(BSTNode*& node, const std::string& k, TData* pData) {
    if (!node) {
        TData* newData = new TData();
        newData->data = k;
        newData->count = 1;
        node = new BSTNode(k, newData);
    }
    else if (k < node->Key) {
        InsertHelper(node->Left, k, pData);
    }
    else if (k > node->Key) {
        InsertHelper(node->Right, k, pData);
    }
    else {
        if (!node->pData) {
            node->pData = pData;
        }
        else {
            node->pData->count++;
        }
        return;
    }
}


int TBinarySearchTree::FindRecord(const std::string& k) {
    comparisonCount = 0;
    return FindRecordHelper(Root, k);
}

int TBinarySearchTree::FindRecordHelper(BSTNode* node, const std::string& k) {
    if (!node)
        return 0;

    comparisonCount++;
    if (node->Key == k)
        return node->pData ? node->pData->count : 0;

    if (k < node->Key)
        return FindRecordHelper(node->Left, k);
    else
        return FindRecordHelper(node->Right, k);
}

void TBinarySearchTree::Delete(const std::string& k) {
    DeleteHelper(Root, k);
}

void TBinarySearchTree::DeleteHelper(BSTNode*& node, const std::string& k) {
    if (!node)
        return;
    if (k < node->Key) {
        DeleteHelper(node->Left, k);
    }
    else if (k > node->Key) {
        DeleteHelper(node->Right, k);
    }
    else {

        if (node->pData && node->pData->count > 1) {
            node->pData->count--;
            return;
        }
        if (!node->Left) {
            BSTNode* temp = node->Right;
            delete node;
            node = temp;
        }
        else if (!node->Right) {
            BSTNode* temp = node->Left;
            delete node;
            node = temp;
        }
        else {
            BSTNode* temp = node->Right;
            while (temp->Left)
                temp = temp->Left;
            node->Key = temp->Key;
            node->pData = temp->pData;
            DeleteHelper(node->Right, temp->Key);
        }
    }
}

void TBinarySearchTree::Clear() {
    ClearHelper(Root);
    Root = nullptr;
}

void TBinarySearchTree::ClearHelper(BSTNode*& node) {
    if (!node)
        return;
    ClearHelper(node->Left);
    ClearHelper(node->Right);
    delete node;
}

void TBinarySearchTree::DisplayRecordsHelper(BSTNode* node) const {
    if (node) {
        DisplayRecordsHelper(node->Left);
        std::cout << "Key: " << node->Key << ", Data: " << node->pData->data << ", Count: " << node->pData->count << std::endl;
        DisplayRecordsHelper(node->Right);
    }
}

void TBinarySearchTree::DisplayRecords() const {
    DisplayRecordsHelper(Root);
}

BSTNode* TBinarySearchTree::GetRoot() const {
    return Root;
}