#pragma once

#include "TData.h"

// ���� ��������� ������ ������
class BSTNode {
public:
    std::string Key; // ����
    TData* pData; // ������
    BSTNode* Left; // ����� �������
    BSTNode* Right; // ������ �������

    BSTNode(const std::string& key = "", TData* pVal = nullptr, BSTNode* left = nullptr, BSTNode* right = nullptr) :
        Key(key), Left(left), Right(right) {
        pData = new TData();
        pData->count = 1;
        pData->data = key;
    }
};

// ����� ������������� ������� (�������� ������ ������)
class TBinarySearchTree {
private:
    BSTNode* Root;
public:
    TBinarySearchTree() : Root(nullptr) {}
    ~TBinarySearchTree() { Clear(); }

    void Insert(const std::string& k, TData* pData);
    int FindRecord(const std::string& k);
    void Delete(const std::string& k);
    void Clear();

private:
    void InsertHelper(BSTNode*& node, const std::string& k, TData* pData);
    int FindRecordHelper(BSTNode* node, const std::string& k);
    void DeleteHelper(BSTNode*& node, const std::string& k);
    void ClearHelper(BSTNode*& node);
};

void TBinarySearchTree::Insert(const std::string& k, TData* pData) {
    InsertHelper(Root, k, pData);
}

void TBinarySearchTree::InsertHelper(BSTNode*& node, const std::string& k, TData* pData) {
    if (!node) {
        node = new BSTNode(k, pData);
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
        // ������� �� ������� ����� ��������� ��������� �����
        return;
    }
}

int TBinarySearchTree::FindRecord(const std::string& k) {
    return FindRecordHelper(Root, k);
}

int TBinarySearchTree::FindRecordHelper(BSTNode* node, const std::string& k) {
    if (!node)
        return 0;

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