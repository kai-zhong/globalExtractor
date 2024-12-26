#include "mbptree/mbpnode.h"

MbpNode::MbpNode(MbpNode* _parent, MbpNode* _prev, MbpNode* _next, bool _isLeaf)
{
    parent = _parent;
    prev = _prev;
    next = _next;
    isLeaf = _isLeaf;

    if(next != nullptr)
    {
        next->prev = this;
    }
    if(prev != nullptr)
    {
        prev->next = this;
    }
}

MbpNode::~MbpNode(){}

uint MbpNode::indexofChild(uint key)
{
    for(int i = 0; i < keys.size(); i++)
    {
        if(key < keys[i])
        {
            return i;
        }
    }
    return keys.size();
}

uint MbpNode::indexofKey(uint key)
{
    for(int i = 0; i < keys.size(); i++)
    {
        if(keys[i] == key)
        {
            return i;
        }
    }
    return -1;
}

MbpNode* MbpNode::getParent()
{
    return parent;
}

MbpNode* MbpNode::getPrev()
{
    return prev;
}

MbpNode* MbpNode::getNext()
{
    return next;
}

MbpNode* MbpNode::getChild(uint key)
{
    return children[indexofChild(key)];
}

void MbpNode::setParent(MbpNode* _parent)
{
    parent = _parent;
}

void MbpNode::setPrev(MbpNode* _prev)
{
    prev = _prev;
}

void MbpNode::setNext(MbpNode* _next)
{
    next = _next;
}

bool MbpNode::isLeafNode()
{
    return isLeaf;
}

void MbpNode::setChild(uint key, std::vector<MbpNode*> value)
{
    uint index = indexofChild(key);
    keys.insert(keys.begin() + index, key);
    children.erase(children.begin() + index);
    children.insert(children.begin() + index, value.begin(), value.end());
}

std::tuple<uint, MbpNode*, MbpNode*> MbpNode::splitInternal()
{
    int mid = keys.size() / 2; // 计算分裂位置
    MbpNode* left = new MbpNode(parent, nullptr, nullptr, false); // 创建左分裂节点

    std::copy(keys.begin(), keys.begin() + mid, std::back_inserter(left->keys));
    std::copy(children.begin(), children.begin() + mid + 1, std::back_inserter(left->children));

    for(MbpNode* child : left->children)
    {
        child->parent = left;
    }

    uint key = keys[mid];
    keys.erase(keys.begin(), keys.begin() + mid + 1);
    children.erase(children.begin(), children.begin() + mid + 1);

    return std::make_tuple(key, left, this);
}

std::tuple<uint, MbpNode*, MbpNode*> MbpNode::splitLeaf()
{
    int mid = keys.size() / 2; // 计算分裂位置
    MbpNode* left = new MbpNode(parent, prev, this, true);

    left->keys = std::vector<uint>(keys.begin(), keys.begin() + mid);
    left->values = std::vector<uint>(values.begin(), values.begin() + mid);

    keys.erase(keys.begin(), keys.begin() + mid);
    values.erase(values.begin(), values.begin() + mid);

    return std::make_tuple(keys[0], left, this);
}

uint MbpNode::get(uint key) const
{
    int index = -1;
    for(int i = 0; i < keys.size(); i++)
    {
        if(keys[i] == key)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        std::cout << key << "not found in node" << std::endl;
    }

    return values[index];
}

void MbpNode::set(uint key, uint value)
{
    int i = indexofChild(key);// 查找插入位置
    if(find(keys.begin(), keys.end(), key) == keys.end())
    {
        keys.insert(keys.begin() + i, key);
        values.insert(values.begin() + i, value);
    }
    else
    {
        values[i - 1] = value;
    }
}

void MbpNode::printNodeInfo()
{
    std::cout<<"Keys: [";
    for(int i = 0; i < keys.size(); i++)
    {
        std::cout<<keys[i]<<" ";
    }
    std::cout<<"]"<<std::endl;
}