#include "meshfactory.hpp"
#include <QMatrix4x4>
#include <QtMath>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

std::shared_ptr<Mesh> MeshFactory::makeCube(const QVector3D& size, std::size_t grid_steps)
{
    auto pCube = std::make_shared<Mesh>();
    pCube->vertices.reserve(6 * (grid_steps + 1) * (grid_steps + 1));
    pCube->indices.reserve(6 * 6 * grid_steps * grid_steps);
    float step = 1.f / grid_steps;
    QVector3D const baseNormal{0.f, 0.f, 1.f};

    for (size_t j = 0; j < grid_steps + 1; ++j){
        for (size_t i = 0; i < grid_steps + 1; ++i){
            pCube->vertices.emplace_back(
                        QVector3D(-0.5f + step * i, -0.5f + step * j, 0.5f),
                        baseNormal
                        );
        }
    }
    for (size_t j = 0; j < grid_steps; ++j){
        for (size_t i = 0; i < grid_steps; ++i){
            auto k = i + (j + 1) * (grid_steps + 1);
            auto p = i + j * (grid_steps + 1);
            pCube->indices.push_back(k);
            pCube->indices.push_back(p);
            pCube->indices.push_back(p + 1);

            pCube->indices.push_back(p + 1);
            pCube->indices.push_back(k + 1);
            pCube->indices.push_back(k);
        }
    }
    auto vertexCount = pCube->vertices.size();
    auto indexCount = pCube->indices.size();
    size_t maxIndex = 0;

    QMatrix4x4 rotation;
    for (size_t n = 0; n < 3; ++n){
        rotation.rotate(90.f, 0.f, 1.f);
        auto normal = rotation * baseNormal;
        maxIndex = pCube->vertices.size();

        for (size_t i = 0; i < vertexCount; ++i){
            pCube->vertices.emplace_back(rotation * pCube->vertices[i].position, normal);
        }
        for (size_t i = 0; i < indexCount; ++i){
            pCube->indices.push_back(pCube->indices[i] + maxIndex);
        }
    }
    rotation.setToIdentity();
    rotation.rotate(90.f, 1.f, 0.f);
    auto normal = rotation * baseNormal;
    maxIndex = pCube->vertices.size();

    for (size_t i = 0; i < vertexCount; ++i){
        pCube->vertices.emplace_back(rotation * pCube->vertices[i].position, normal);
    }
    for (size_t i = 0; i < indexCount; ++i){
        pCube->indices.push_back(pCube->indices[i] + maxIndex);
    }

    rotation.rotate(180.f, 1.f, 0.f);
    normal = rotation * baseNormal;
    maxIndex = pCube->vertices.size();

    for (size_t i = 0; i < vertexCount; ++i){
        pCube->vertices.emplace_back(rotation * pCube->vertices[i].position, normal);
    }
    for (size_t i = 0; i < indexCount; ++i){
        pCube->indices.push_back(pCube->indices[i] + maxIndex);
    }

    // Resize unit cube to given sized
    for (auto& vertex : pCube->vertices){
        vertex.position *= size;
    }

    return pCube;
}

std::shared_ptr<Mesh> MeshFactory::makeSphere(size_t stackCount, size_t sectorCount, float radius)
{
    auto sphere = std::make_shared<Mesh>();

    float x, y, z, xy;                  // vertex position
    float lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                         // vertex texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (size_t i = 0; i <= stackCount; ++i){
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * qCos(stackAngle);             // r * cos(u)
        z = radius * qSin(stackAngle);              // r * sin(u)

      // add (sectorCount+1) vertices per stack
      // the first and last vertices have same position and normal, but different tex coords
        for (size_t j = 0; j <= sectorCount; ++j){
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi
            Vertex vertex;
            // vertex position (x, y, z)
            x = xy * qCos(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * qSin(sectorAngle);             // r * cos(u) * sin(v)
            vertex.position = QVector3D{x, z, y};
            vertex.normal = QVector3D{x, z, y} * lengthInv;
            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertex.textCoord = QVector2D{s, t};

            sphere->vertices.push_back(vertex);
      }
    }
    // MAYBE NEED THIS VARS MAKE INTEGER***********************************************************************************!!!!!!!!
    size_t k1, k2;
    for(size_t i = 0; i < stackCount; ++i){
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(size_t j = 0; j < sectorCount; ++j, ++k1, ++k2){
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0){
                sphere->indices.push_back(k2);
                sphere->indices.push_back(k1);
                sphere->indices.push_back(k1 + 1);
            }
            // k1+1 => k2 => k2+1
            if(i != (stackCount - 1)){
                sphere->indices.push_back(k2);
                sphere->indices.push_back(k1 + 1);
                sphere->indices.push_back(k2 + 1);
            }
        }
    }
    return sphere;
}

MeshSPtr MeshFactory::makeRectangle(float width, float length)
{
    auto pRect = std::make_shared<Mesh>();
    QVector3D norm = {0, 1, 0};

    pRect->vertices.push_back(Vertex(QVector3D(0    , 0, 0     ), norm, {0    , 0}));
    pRect->vertices.push_back(Vertex(QVector3D(width, 0, 0     ), norm, {1, 0}));
    pRect->vertices.push_back(Vertex(QVector3D(width, 0, length), norm, {1, length}));
    pRect->vertices.push_back(Vertex(QVector3D(0    , 0, length), norm, {0, length}));

    pRect->indices = std::vector<Mesh::indexType>{0, 1, 2, 0, 2, 3};

    return pRect;
}


namespace  {

MeshSPtr processMesh(aiMesh* mesh){
    std::vector<Mesh::vertexType> vertices;
    std::vector<Mesh::indexType> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.position.setX(mesh->mVertices[i].x);
        vertex.position.setY(mesh->mVertices[i].y);
        vertex.position.setZ(mesh->mVertices[i].z);

        vertex.normal.setX(mesh->mNormals[i].x);
        vertex.normal.setY(mesh->mNormals[i].y);
        vertex.normal.setZ(mesh->mNormals[i].z);

        vertex.tangent.setX(mesh->mTangents[i].x);
        vertex.tangent.setY(mesh->mTangents[i].y);
        vertex.tangent.setZ(mesh->mTangents[i].z);

        vertex.biTangent.setX(mesh->mBitangents[i].x);
        vertex.biTangent.setY(mesh->mBitangents[i].y);
        vertex.biTangent.setZ(mesh->mBitangents[i].z);


        vertex.textCoord.setX(mesh->mTextureCoords[0][i].x);
        vertex.textCoord.setY(mesh->mTextureCoords[0][i].y);

        vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; ++i){
        auto face = mesh->mFaces[i];

        indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);

//      indices.reserve(indices.size() + face.mNumIndices); //        for (size_t j = 0; j < face.mNumIndices; ++j){//            indices.push_back(face.mIndices[j]);//        }
    }
    return std::make_shared<Mesh>(std::move(vertices), std::move(indices));
}

void processNode(aiNode* node, aiScene const* scene, std::vector<MeshSPtr>& meshes){
    for (size_t i = 0; i < node->mNumMeshes; ++i){
        auto* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh));
    }
    for (size_t i = 0; i < node->mNumChildren; ++i){
        processNode(node->mChildren[i], scene, meshes);
    }
}

}

std::vector<MeshSPtr> MeshFactory::loadModel3D(const std::string& path)
{
    Assimp::Importer importer;

    auto const* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_GenUVCoords);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        qDebug() << "loadModel3D error: " << importer.GetErrorString();
        return {};
    }
    std::vector<MeshSPtr> meshes;
    processNode(scene->mRootNode, scene, meshes);

    return meshes;
}


