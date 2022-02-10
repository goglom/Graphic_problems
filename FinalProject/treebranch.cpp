#include "treebranch.hpp"


#include <cmath>

namespace {
template<class T>
T mix(T const& x, T const& y, float factor){
    return x * (1.f - factor) + y * factor;
}

}


TreeBranch::TreeBranch(float ratio, float spread, float splisize)
    : _ratio(ratio), _spread(spread), _splisize(splisize)
{
}

bool TreeBranch::isLeaf() const
{
    return !_lChild && !_rChild;
}

void TreeBranch::grow(float feed)
{
    _radius = std::sqrt(_area / M_PI);

    if (isLeaf()){
        _length += std::cbrt(feed);
        feed -= std::cbrt(feed) * _area;
        _area += feed / _length;

        if (_length > _splisize * std::exp(-splitDecay * _depth))
            split();

        return;
    }

    float pass = passRatio;

    if (conserveArea){
        pass = (_lChild->_area + _rChild->_area) /
                                    (_lChild->_area + _rChild->_area + _area);
    }
    _area += pass * feed / _length;
    feed *= 1.f - pass;

    if (feed < feedMin){
        return;
    }
    _lChild->grow(feed * _ratio);
    _rChild->grow(feed* (1.f - _ratio));
}

float TreeBranch::radius() const
{
    return _radius;
}

float TreeBranch::getEndRadius() const
{
    float radius1(0), radius2(0);
    if (_lChild)
        radius1 = _lChild->radius();
    if (_rChild)
        radius2 = _rChild->radius();

    return std::max(radius1, radius2);
}

TreeBranch::iterator TreeBranch::begin()
{
    return iterator(this);
}

TreeBranch::iterator TreeBranch::end()
{
    return iterator();
}

TreeBranch::TreeBranch(const TreeBranch& parent)
    : _parent(&parent),
      _ratio(parent._ratio),
      _spread(parent._spread),
      _splisize(parent._splisize),
      _depth(parent._depth + 1)
{
}

void TreeBranch::split()
{
    _lChild = TreeBranchUPtr(new TreeBranch(*this));
    _rChild = TreeBranchUPtr(new TreeBranch(*this));
    // TODO: There is maybe add ID for new branches

    auto leafDir = leafDensity(localDepth);
    auto norm = QVector3D::crossProduct(_direction, leafDir).normalized();

    float flip = rand() % 2 ? 1.f : 0.f;
    _lChild->_direction =
            mix(flip * _spread * norm, _direction, _ratio).normalized();

    _rChild->_direction =
            mix(flip * _spread * norm, _direction, 1.f - _ratio).normalized();
}

QVector3D TreeBranch::leafDensity(int searchDepth)
{
    QVector3D randVec{_onesDistrib(_randomGen), _onesDistrib(_randomGen),
                                                    _onesDistrib(_randomGen)};
    if (_depth == 0) {
        return randVec;
    }

    auto const* curBranch = this;
    QVector3D relative;

    while(curBranch->_depth > 0 && searchDepth >= 0) {
        --searchDepth;
        relative += curBranch->_length * curBranch->_direction;
        curBranch = curBranch->_parent;
    }

    return directedness * (leafAverage(*curBranch) - relative)
            + (1.f - directedness) * randVec;
}

QVector3D TreeBranch::leafAverage(const TreeBranch& branch)
{
    if (branch.isLeaf()){
        return branch._length * branch._direction;
    }
    return branch._length * branch._direction
            + _ratio * leafAverage(*branch._lChild)
            + (1 - _ratio) * leafAverage(*branch._rChild);
}

TreeBranch::iterator::iterator(TreeBranch* root)
{
    _stack.push(root);
}

TreeBranch::iterator::iterator()
{

}

TreeBranch& TreeBranch::iterator::operator*()
{
    return *_stack.top();
}

TreeBranch* TreeBranch::iterator::operator->()
{
    return _stack.top();
}

TreeBranch::iterator& TreeBranch::iterator::operator++()
{
    auto* cur = _stack.top()->_rChild.get();
    _stack.pop();

    while (cur != nullptr){
        _stack.push(cur);
        cur = cur->_lChild.get();
    }
    return *this;
}

bool TreeBranch::iterator::operator==(const TreeBranch::iterator& other)
{
    return _stack == other._stack;
}

bool TreeBranch::iterator::operator!=(const TreeBranch::iterator& other)
{
    return !(*this == other);
}

TreeBranch::iterator TreeBranch::iterator::operator++(int)
{
    auto temp = *this;
    ++*this;
    return temp;
}
