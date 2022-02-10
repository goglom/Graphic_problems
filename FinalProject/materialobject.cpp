#include "materialobject.hpp"

MaterialObject::MaterialObject(RenderObjectSPtr parent, MaterialSPrt material)
    : RenderObjectDecorator(std::move(parent)),
      material_(std::move(material))
{
}

void MaterialObject::render()
{
    shader().bind();
    shader().setUniformValue("material.ambient",    material_->ambient);
    shader().setUniformValue("material.diffuse",    material_->diffuse);
    shader().setUniformValue("material.specular",   material_->specular);
    shader().setUniformValue("material.shininess",  material_->shininess);

    RenderObjectDecorator::render();
}

RenderObjectSPtr MaterialObject::clone() const
{
    return std::make_shared<MaterialObject>(
                RenderObjectDecorator::clone(),
                material_
                );
}

