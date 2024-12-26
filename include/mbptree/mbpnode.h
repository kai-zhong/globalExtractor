#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <tuple>

#include "../configuration/types.h"
#include "../configuration/config.h"

class MbpNode
{
    private:
        MbpNode* parent;
        MbpNode* next;
        MbpNode* prev;
        bool isLeaf;

    public:
        std::vector<uint> keys;
        std::vector<uint> values;
        std::vector<MbpNode*> children;

        MbpNode(MbpNode* _parent = nullptr, MbpNode* _prev = nullptr, MbpNode* _next = nullptr, bool _isLeaf = false);
        ~MbpNode();

        uint indexofChild(uint key); // 根据关键字查找其对应的子节点的索引
        uint indexofKey(uint key); // 根据关键字查找其在节点中的索引

        MbpNode* getParent(); // 获取父节点
        MbpNode* getPrev(); // 获取前一个节点
        MbpNode* getNext(); // 获取后一个节点
        MbpNode* getChild(uint key); // 获取关键字对应的子节点

        void setParent(MbpNode* _parent); // 设置父节点
        void setPrev(MbpNode* _prev); // 设置前一个节点
        void setNext(MbpNode* _next); // 设置后一个节点

        bool isLeafNode(); // 是否为叶子节点

        void setChild(uint key, std::vector<MbpNode*> value); // 设置关键字对应的子节点

        std::tuple<uint, MbpNode*, MbpNode*> splitInternal(); // 分裂中间节点，返回中间关键字和分裂后的两个节点
        std::tuple<uint, MbpNode*, MbpNode*> splitLeaf(); // 分裂叶子节点，返回中间关键字和分裂后的两个节点

        uint get(uint key) const; // 根据关键字获取对应的值（仅叶子节点）

        void set(uint key, uint value); // 设置关键字及其对应的值（仅叶子节点）

        void printNodeInfo();
};