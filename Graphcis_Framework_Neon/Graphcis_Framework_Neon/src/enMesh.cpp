#include "..\include\enMesh.h"
#include "enDevice.h"
#include "enDeviceContext.h"

enMesh::enMesh()
  :m_topology(enTopology::TriList)
{
  mptr_indexBuffer = new enIndexBuffer();
  mptr_vertexBuffer = new enVertexBuffer();
  mptr_resource = std::make_shared<enShaderResourceView>();
  mptr_texture = std::make_shared<enTexture2D>();

  glm::vec4 DefaultPosition(0.1f, 0.1f, 0.1f, 1.0f);

  m_transform *= DefaultPosition;
}

enMesh::enMesh(enMesh&& mesh) noexcept
  :mptr_indexBuffer(mesh.mptr_indexBuffer),
  mptr_vertexBuffer(mesh.mptr_vertexBuffer),
  mptr_resource(mesh.mptr_resource),
  mptr_texture(mesh.mptr_texture),
  m_vertexDataContainer(std::move(mesh.m_vertexDataContainer)),
  m_indexDataContainer(std::move(mesh.m_indexDataContainer)),
  m_topology(mesh.m_topology),
  m_transform(std::move(mesh.m_transform))
{
  mesh.mptr_indexBuffer = nullptr;
  mesh.mptr_vertexBuffer = nullptr;
  mesh.mptr_resource = nullptr;
}

enMesh::~enMesh()
{
  DELETE_PTR(mptr_indexBuffer);
  DELETE_PTR(mptr_vertexBuffer);
}

enMesh&
enMesh::operator=(enMesh&& mesh) noexcept
{
  mptr_indexBuffer = (mesh.mptr_indexBuffer);
  mptr_vertexBuffer = (mesh.mptr_vertexBuffer);
  mptr_texture = (mesh.mptr_texture);
  m_vertexDataContainer = (std::move(mesh.m_vertexDataContainer));
  m_indexDataContainer = (std::move(mesh.m_indexDataContainer));
  m_topology = (mesh.m_topology);
  m_transform = (std::move(mesh.m_transform));

  return *this;
}

void
enMesh::initIndexBuffer(std::unique_ptr<std::vector<uint16>>&& indexes)
{
  m_indexDataContainer = std::move(indexes);

  mptr_indexBuffer->init(sizeof(uint16),
                         m_indexDataContainer->size(),
                         0,
                         m_indexDataContainer->data());
}

void
enMesh::initVertexBuffer(std::unique_ptr<std::vector<SimpleVertex>>&& vertexes)
{
  m_vertexDataContainer = std::move(vertexes);

  mptr_vertexBuffer->init(sizeof(SimpleVertex),
                          m_vertexDataContainer->size(),
                          0,
                          m_vertexDataContainer->data());

}

bool
enMesh::createVertexBuffer()
{
  enDevice& device = enDevice::getInstance();
  bool result = device.CreateVertexBuffer(*mptr_vertexBuffer);
  return result;
}

bool 
enMesh::createIndexBuffer()
{
  enDevice& device = enDevice::getInstance();
  bool result = device.CreateIndexBuffer(*mptr_indexBuffer);
  return result;
}

void
enMesh::setTexture(const std::shared_ptr<enTexture2D> newTexture)
{
  mptr_texture = newTexture;
}

void
enMesh::setTransform(const glm::mat4& newTransform)
{
  m_transform = newTransform;
}

void 
enMesh::setMesh()
{
 enDeviceContext& deviceContext = enDeviceContext::getInstance();

 deviceContext.IASetVertexBuffers(mptr_vertexBuffer,1); 
 deviceContext.IASetIndexBuffer(*mptr_indexBuffer, enFormats::uR16);
 deviceContext.IASetPrimitiveTopology((int)m_topology);
 if( mptr_resource )
 {
   deviceContext.PSSetShaderResources(&*mptr_resource);
 }
}

glm::mat4
enMesh::getTransform() const
{
  return m_transform;
}

std::shared_ptr<enShaderResourceView>
enMesh::getShaderResourceView()
{
  return mptr_resource;
}

void
enMesh::setTopology(enTopology topology)
{
  m_topology = topology;
}

enTopology
enMesh::getTopology() const
{
  return m_topology;
}

enVertexBuffer*
enMesh::getVertexBuffer()
{
  return mptr_vertexBuffer;
}

enVertexBuffer& 
enMesh::getVertexBufferRef()
{
  return *mptr_vertexBuffer;
}


enIndexBuffer*
enMesh::getIndexBuffer()
{
  return mptr_indexBuffer;
}

enIndexBuffer&
enMesh::getIndexBufferRef()
{
return *mptr_indexBuffer;
}

const std::vector<SimpleVertex>*
enMesh::getVertexData() const
{
  return &*m_vertexDataContainer;
}

const std::vector<uint16>*
enMesh::getIndiceData() const
{
  return &*m_indexDataContainer;
}
