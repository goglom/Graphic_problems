#include "texture.hpp"

Texture::Texture(const QImage& image, Type _type)
    : QOpenGLTexture(image),
      type(_type)
{
}

