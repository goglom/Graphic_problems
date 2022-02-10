#include <iostream>
#include <QDebug>
#include "treebranch.hpp"

using std::cout;


template<class It>
class EnumerateIterator
{
    std::pair<int, It> cur;

public:
    EnumerateIterator(It begin) : cur(0, begin)
    {
    }
    std::pair<int, It>& operator*()
    {
        return cur;
    }
    typename It::pointer operator->()
    {
        return cur.second.operator->();
    }
    EnumerateIterator& operator++()
    {
        ++cur.first;
        ++cur.second;
        return *this;
    }
    EnumerateIterator operator++(int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }
    bool operator==(EnumerateIterator const& other)
    {
        return cur.second == other.cur.second;
    }
    bool operator!=(EnumerateIterator const& other)
    {
        return !(*this == other);
    }
};

template<class T>
struct Enumerator
{
    using Iterator = typename T::iterator;
    EnumerateIterator<Iterator> beginIt;
    EnumerateIterator<Iterator> endIt;

    Enumerator(T& container) : beginIt(container.begin()), endIt(container.end())
    {}
    EnumerateIterator<Iterator> begin(){
        return beginIt;
    }
    EnumerateIterator<Iterator> end(){
        return endIt;
    }
};

int main()
{
    TreeBranch tree(0.6, 0.45, 2.5);
    tree.grow(1);
    tree.grow(1);
    tree.grow(1);
    tree.grow(1);
    tree.grow(1);
    tree.grow(1);
    tree.grow(1);

    auto cond = 1 >= 2;

    for (auto& [i, j]: Enumerator(tree)){
        qDebug() << i << ", " << j->isLeaf() << '\n';
    }

}
