#include <iostream>
#include <vector>
#include <algorithm>

int indexofChild(const int& key, const std::vector<int>& keys)
{
    auto it = std::lower_bound(keys.begin(), keys.end(), key); // 返回指向第一个大于等于 key 的迭代器
    if (it != keys.end() && *it == key) // 等于 key 的情况的child index再加1
    {
        ++it;
    }
    return std::distance(keys.begin(), it);
}

int indexofKey(const uint& key, const std::vector<int>& keys)
{
    auto it = std::lower_bound(keys.begin(), keys.end(), key); // 返回指向第一个大于等于 key 的迭代器
    if(it != keys.end() && *it == key)
    {
        return std::distance(keys.begin(), it);
    }
    return keys.size();
}

int main()
{
    std::vector<int> keys = {1, 3, 5, 7, 9};
    int query = 7;
    std::cout << "indexofChild("<< query << ", keys) = " << indexofChild(query, keys) << std::endl;
    std::cout << "indexofKey("<< query << ", keys) = " << indexofKey(query, keys) << std::endl;
    return 0;
}