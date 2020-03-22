#pragma once
#include "util/Header.h"

  /**
  * @brief : create 1 texture that can be used by both
  * depth-stencil View and Render-target View.
  * @bug : no known bugs.
  */
class enMultiviewTexture
{
public:

  /**
  * @brief : creates a depth stencil.
  * @param[in] width : the width of the texture.
  * @param[in] height: the height of the texture.
  * @bug : no known bugs.
  */
  enErrorCode
  CreateDepthStancil(float width, float height);

public:

  /**
  * @brief : the depth Stencil View.
  */
  enDepthStencilView m_depthView;

  /**
  * @brief : the render-target view.
  */
  enRenderTargetView m_renderView;
private:

  /**
  * @brief : the textured used by both the renderTarget and DepthStencil.
  */
  enTexture2D m_sharedTexture;//shared
};

