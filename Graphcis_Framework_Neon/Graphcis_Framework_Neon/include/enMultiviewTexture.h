#pragma once
#include "util/Header.h"
#include "enRenderTargetView.h"
#include "enDepthStencilView.h"
#include "enShaderResourceView.h"
#include <string_view>
#include <memory>


  /**
  * @brief : create 1 texture that can be used by both
  * depth-stencil View and Render-target View.
  * @bug : no known bugs.
  */
class enMultiviewTexture
{
public:

  /**
  * @brief : creates a depth-stencil-view.
  * @param[in] width : the width of the texture.
  * @param[in] height: the height of the texture.
  * @bug : no known bugs.
  */
  enErrorCode
  CreateDepthStencil(float width, float height, int Format);

  /**
  * @brief : creates a render-target-view.
  * @param[in] width : the width of the texture.
  * @param[in] height: the height of the texture.
  * @bug : no known bugs.
  */
  enErrorCode
  CreateRenderTarget(float width, float height,int Format);

  /**
  * @brief : creates a shader-resource-view.
  * @param[in] width : the width of the texture.
  * @param[in] height: the height of the texture.
  * @bug : no known bugs.
  */
  enErrorCode
  CreateShaderResource( float width, float height,int Format);

  /*int texFormat{ 0 };
  int Usage{ 0 };
  int BindFlags{ 0 };
  int CpuAccess{ 0 };*/
  bool 
  CreateAll(float width,
            float height,
            int format,
            enBufferUse usage,
            enMultiViewType typeOfView);


  /**
  * @brief set a render-target
  * @bug no known bugs.
  * @returns 'true' if the multiView is a renderTarget, 'false' otherwise.
  * @param[in] 
  */
  bool 
  setRenderTarget();

  /**
  * @brief set a render-target
  * @bug no known bugs.
  * @returns 'true' if the multiView is a Depth-stencil, 'false' otherwise.
  * @param[in] 
  */
  bool 
  setDepthStencil();

private:

  /**
  * @returns : enBufferBind value, based on which value 'm_type' is.
  * 
  */
  enBufferBind 
  getBindingFlagsBasedOnMyType();

  /**
  * @brief : Create a texture that made with a certain view in mind.
  * @param[in] type: controls which type of texture is going to be made.
  * @param[in] width: the width of the texture.
  * @param[in] height: the height of the texture.
  */
  bool 
  CreateTexture(enMultiViewType type,
                float width,
                float height);


  enMultiViewType
  getType() const;

public:

  /**
  * @brief : the depth Stencil View.
  */
  enDepthStencilView m_depthView;

  /**
  * @brief : the render-target view.
  */
  enRenderTargetView m_renderView;

  /**
  * @brief :
  */
  std::shared_ptr< enShaderResourceView> m_shaderResource = std::make_shared<enShaderResourceView>();
private:

  /**
  * @brief : the textured used by both the renderTarget and DepthStencil.
  */
  enTexture2D m_sharedTexture;//shared

  /**
  * @brief : used to keep track of which of the view is being used.
  */
  enMultiViewType m_type = enMultiViewType::zeroType;
};

