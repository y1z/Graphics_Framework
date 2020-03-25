#pragma once
#include "util/Header.h"
#include "enShaderResourceView.h"
#include <memory>
#include <vector>


class enVertexBuffer;
class enIndexBuffer;

  /**
  * @brief : contains all the information
  * @bug :
  */
class enMesh
{
public:
  enMesh();
  // I demand move construction
  enMesh(const enMesh& mesh) = delete;
  enMesh(enMesh&& mesh) noexcept;
  ~enMesh();

public:// operators 
  enMesh& operator =(enMesh&& mesh) noexcept;

public:// functions 
  //! set up the index buffer for creation
  void
  initIndexBuffer(std::unique_ptr<std::vector<uint16>>&& indexes);

  //! set up the vertex buffer for creation
  void
  initVertexBuffer(std::unique_ptr<std::vector<SimpleVertex>>&& vertexes);

  //! creates the vertex buffer 
  bool
  createVertexBuffer();

  //! creates the index buffer 
  bool
  createIndexBuffer();

  //! sets a texture to the current mesh
  void
  setTexture(const std::shared_ptr<enTexture2D> newTexture);

  //! give a mesh a different transform
  void
  setTransform(const glm::mat4& newTransform);

  //! sets the topology for the individual mesh 
  void
  setTopology(enTopology topology);

  //! returns the type of topology the mesh contians 
  enTopology
  getTopology() const;

public: // functions 
  /**
  * @brief :
  * @bug :
  */
  enVertexBuffer*
  getVertexBuffer();

  /**
  * @returns a reference to the vertex buffer.
  * @bug :
  */
  enVertexBuffer&
  getVertexBufferRef();

  /**
  * @return: a pointer to the index buffer. 
  * @bug :
  */
  enIndexBuffer*
  getIndexBuffer();

  /**
  * @returns a reference to the index buffer.
  * @bug :
  */
  enIndexBuffer&
  getIndexBufferRef();

/*! returns a vector that contains all data related with vertexes of the mesh */
  const std::vector< SimpleVertex>*
  getVertexData() const;
/*! returns a vector that contains all data related with indices of the mesh */
  const std::vector<uint16>*
  getIndiceData() const;

  void 
  setTransform(glm::mat4& transform);

  void 
  setMesh();

  glm::mat4 
  getTransform()const;


  std::shared_ptr<enShaderResourceView>
  getShaderResourceView();

#if DIRECTX
#endif // DIRECTX

public:


  std::shared_ptr<enShaderResourceView>mptr_resource = nullptr;
private:
  enVertexBuffer* mptr_vertexBuffer = nullptr;

  enIndexBuffer* mptr_indexBuffer= nullptr;
  //! this is so a mesh can share a texture with a another mesh 
  std::shared_ptr<enTexture2D> mptr_texture;
  /*! this contains the values that consist of the vertex data */
  std::unique_ptr<std::vector< SimpleVertex >> m_vertexDataContainer;
  /*! this contains the values that represent the indices */
  std::unique_ptr<std::vector<uint16>> m_indexDataContainer;
public:
  //! controls which topology it used be each mesh.
  enTopology m_topology;
  glm::mat4 m_transform;
};

