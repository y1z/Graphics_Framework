#pragma once
#include "enMesh.h"
#include "enTransform.h"
#include "enDevice.h"
#include "enDeviceContext.h"

#include <vector>
#include <string>


/*
 FORWARD DECLARATIONS
*/

struct aiScene;
struct aiNode;
struct aiMesh;


  /**
  * @brief : contains all the meshes related with the model.
  * @bug :
  */
class enModel
{
public:
  enModel() = default;
  enModel(std::string_view modelPath);
  ~enModel() = default;

public:
    //! this function it where we load the file from
  bool
  LoadModelFromFile(std::string_view modelPath);

private:
  /**
  * @brief : visit every node in the tree that holds the information
  *         of the model.
  * @bug :
  */
  void
  TraversTree(const aiScene* scene,
              aiNode* node,
              enDevice& device);

  /**
  * @brief : Read the geometry data from the mesh.
  * @bug :
  */
  void
  ExtractMesh(const aiMesh* assimpMesh,
              enDevice& device,
              const aiScene* scene);


public:

  /**
  * @brief :
  * @bug :
  */
  void
  DrawMeshes(std::vector<enConstBuffer*>& buffers,
             const sColorf& color = { 0.5f,0.5f,0.5f,1.0f });

public:
  std::string m_pathToModelFile;

  std::vector<enMesh> m_meshes;

  enTransform m_transform;
};

