#include "..\include\enModel.h"


#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"

#include "enTypes.h"
#include "helperFucs.h"


enModel::enModel(std::string_view modelPath)
  :m_pathToModelFile((modelPath))
{}

bool
enModel::LoadModelFromFile(std::string_view modelPath)
{
  m_pathToModelFile = modelPath;

  Assimp::Importer importer;

  enDevice& device = enDevice::getInstance();

  const aiScene* Scene = importer.ReadFile(m_pathToModelFile.c_str(),
                                           aiProcessPreset_TargetRealtime_MaxQuality |
                                           aiProcess_ConvertToLeftHanded);


  std::vector<std::string> texturePaths;

  if( Scene == nullptr )
  {
    return false;
  }
  else
  {
    this->TraversTree(Scene, Scene->mRootNode, device);
    return true;
  }

  return false;
}


void
enModel::TraversTree(const aiScene* scene,
                     aiNode* node,
                     enDevice& device)
{
// extract the current mesh
  for( uint32_t i = 0; i < node->mNumMeshes; ++i )
  {
    aiMesh* meshes = scene->mMeshes[node->mMeshes[i]];
    this->ExtractMesh(meshes, device, scene);
  }

  // get the children in order to the rest of the meshes 
  for( uint32_t j = 0; j < node->mNumChildren; j++ )
  {
    this->TraversTree(scene, node->mChildren[j], device);
  }
}

void
enModel::ExtractMesh(const aiMesh* assimpMesh,
                     enDevice& device,
                     const aiScene* scene)
{
  std::unique_ptr<std::vector<uint16>> ptr_indices = std::make_unique<std::vector<uint16>>();
  std::unique_ptr<std::vector<SimpleVertex>> ptr_vertices = std::make_unique<std::vector<SimpleVertex>>();


  ptr_indices->reserve(size_t(assimpMesh->mNumFaces) * 3);
  ptr_vertices->reserve(assimpMesh->mNumVertices);

  // get the indices of a the model 
  for( uint32_t i = 0; i < assimpMesh->mNumFaces; ++i )
  {
    uint16 tri[3];
    for( uint32_t j = 0; j < assimpMesh->mFaces[i].mNumIndices; ++j )
    {
      tri[j] = assimpMesh->mFaces[i].mIndices[j];
    }
    ptr_indices->emplace_back(tri[0]);
    ptr_indices->emplace_back(tri[1]);
    ptr_indices->emplace_back(tri[2]);
  }

  // get the vertices of the model
  for( uint32_t i = 0; i < assimpMesh->mNumVertices; ++i )
  {
    SimpleVertex vertex;
    vertex.Pos.x = assimpMesh->mVertices[i].x;
    vertex.Pos.y = assimpMesh->mVertices[i].y;
    vertex.Pos.z = assimpMesh->mVertices[i].z;
    vertex.Pos.w = 1.0f;

      // get the texture coords 
    if( assimpMesh->HasTextureCoords(0) )
    {
      vertex.Tex.x = static_cast<float>(assimpMesh->mTextureCoords[0][i].x);
      vertex.Tex.y = static_cast<float>(assimpMesh->mTextureCoords[0][i].y);
    }
    else
    {
      vertex.Tex.x = 0.0f;
      vertex.Tex.y = 0.0f;
    }

    ptr_vertices->emplace_back(vertex);
  }

  enMesh result;
  result.initIndexBuffer(std::move(ptr_indices));
  result.initVertexBuffer(std::move(ptr_vertices));

  bool const IndexSuccessful = result.createIndexBuffer();
  bool const VertexSuccessful = result.createVertexBuffer();

  assert((IndexSuccessful && VertexSuccessful, "error with index or vertex buffer creation"));
  m_meshes.emplace_back(std::move(result));
}



void
enModel::DrawMeshes(std::vector<enConstBuffer*>& buffers,
                    const sColorf& color)
{
  enDeviceContext& deviceContext = enDeviceContext::getInstance();

  ConstBufferWorldColor Cb;

  const glm::mat4 identity(1.0f);//identity
  const glm::mat4 Transform(identity * this->m_transform.getMatrix());

  const glm::vec4 resultingColor(color.red,
                                 color.green,
                                 color.blue,
                                 color.alpha);

  for( enMesh& mesh : m_meshes )
  {
    mesh.setMesh();

    deviceContext.DrawIndexed(mesh.getIndicesCount());
  }

  enMatrix4x4 temp;
  temp = Transform;
  helper::arrangeForApi(temp);
  Cb.mWorld = temp;
  Cb.vMeshColor = resultingColor;
  //deviceContext.UpdateSubresource(buffers[0], &Cb);

}


