#pragma once
#include <QVector3D>
#include <memory>
#include <random>
#include <stack>

class TreeBranch;
using TreeBranchUPtr = std::unique_ptr<TreeBranch>;


class TreeBranch
{
public:
    TreeBranch(float ratio, float spread, float splisize);
    bool isLeaf() const;
    void grow(float feed);
    float radius() const;
    float getEndRadius() const;

    float passRatio = 0.3;
    float splitDecay = 1e-2;
    bool conserveArea = true;
    float feedMin = 1e-5;
    int localDepth = 2;
    float directedness = 0.5;

    class iterator;

    iterator begin();
    iterator end();

private:
    TreeBranch(TreeBranch const& parent);

    void split();
    QVector3D leafDensity(int searchDepth);
    QVector3D leafAverage(TreeBranch const& branch);

    TreeBranchUPtr _lChild;
    TreeBranchUPtr _rChild;
    TreeBranch const* _parent;

    QVector3D _direction;
    float _ratio, _spread, _splisize;
    int _depth = 0;
    float _length = 0;
    float _radius = 0;
    float _area = 0.1
                  ;
    std::mt19937 _randomGen{};
    std::uniform_real_distribution<float> _onesDistrib{-1.f, 1.f};
};

class TreeBranch::iterator
{
    std::stack<TreeBranch*> _stack{};
public:

    using reference = TreeBranch&;
    using pointer = TreeBranch*;

    iterator(TreeBranch* root);
    iterator();
    TreeBranch& operator*();
    TreeBranch* operator->();
    iterator& operator++();
    iterator operator++(int);
    bool operator==(iterator const& other);
    bool operator!=(iterator const& other);
};
