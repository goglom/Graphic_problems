#pragma once

#include "renderobjectdecorator.hpp"
#include "material.hpp"

#include <QMatrix4x4>

using MaterialSPrt = std::shared_ptr<Material>;

class MaterialObject : public RenderObjectDecorator
{
    MaterialSPrt material_;

public:

    MaterialObject(
            RenderObjectSPtr parent,
            MaterialSPrt material
            );
    void render() override;
    RenderObjectSPtr clone() const override;
};

