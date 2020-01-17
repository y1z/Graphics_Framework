#pragma once
#include "MathTypes.h"

/**
* @brief : this is a base class for any future camera classes.
*/

class BasePerspectiveCamera
{
public:
  BasePerspectiveCamera();

  virtual ~BasePerspectiveCamera() = default;
  /**
  * @returns : the current view matrix of the camera.
  * @bug : no known bugs.
  */
  enMatrix4x4
    getView() const;

  /**
  * @returns : the current projection matrix of the camera.
  * @bug : no known bugs.
  */
  enMatrix4x4
    getProjection() const;

  /**
  * @returns : the current position of the camera (eye).
  * @bug : no known bugs.
  */
  enVector3
    getPosition() const;

  /**
  * @returns : the position where the camera is looking at.
  * @bug : no known bugs.
  */
  enVector3
    getViewPosition() const;

  /**
  * @returns : the 'up' direction relative to the camera( it may be rotated).
  * @bug : no known bugs.
  */
  enVector3
    getUpDir() const;


  /**
  * @returns : the 'front' direction relative to the camera( it may be rotated).
  * @bug : no known bugs.
 */
  enVector3
    getFront() const;

  /**
  * @returns : the 'right' direction relative to the camera( it may be rotated).
  * @bug : no known bugs.
 */
  enVector3
    getRight() const;

  /**
  * @returns : the fov of the camera in radians
  * @bug : no known bugs.
 */
  float
    getFov() const;

  /**
  * @returns : the height of the projection
  * @bug : no known bugs.
 */
  float
    getHeight() const;

  /**
  * @returns : the width of the projection
  * @bug : no known bugs.
 */
  float
    getWidth() const;

  /**
  * @returns : how far the camera can see.
  * @bug : no known bugs.
 */
  float
    getFar() const;

  /**
  * @returns : how close the camera can see.
  * @bug : no known bugs.
 */
  float
    getNear() const;

  /**
  * @returns : the speed of the camera.
  * @bug : no known bugs.
 */
  float
    getSpeed() const;

  /**
  * @brief :set the view matrix
  * @bug : no known bugs.
  * @param matrix [in] a different matrix.
  */
  void
    setViewMatrix(const enMatrix4x4& matrix);

  /**
  * @brief :set the  projection matrix
  * @bug : no known bugs.
  * @param matrix [in] a different matrix.
  */
  void
    setProjectionMatrix(const enMatrix4x4& matrix);


  /**
  * @brief :sets the position of the camera
  * @bug : no known bugs.
  * @param x , y ,z [in] the coordinates of the position .
  */
  void
    setPosition(float x, float y, float z);

  /**
  * @brief :sets the position of the camera
  * @bug : no known bugs.
  * @param newPosition [in] the new position of the camera.
  */
  void
    setPosition(const enVector4& newPosition);

  /**
  * @brief :sets the view position of the camera
  * @bug : no known bugs.
  * @param x , y ,z [in] sets the location where the camera is looking at.
  */
  void
    setViewPosition(float x, float y, float z);

  /**
  * @brief :sets the view position of the camera
  * @bug : no known bugs.
  * @param newPosition [in] sets the location where the camera is looking at.
  */
  void
    setViewPosition(const enVector3& newPosition);


  /**
  * @brief :sets the 'up' direction of the camera
  * @bug : no known bugs.
  * @param x , y ,z [in] sets the location where the camera is looking at.
  */
  void
    setUpDir(float x, float y, float z);

  /**
  * @brief :sets the 'up' direction of the camera
  * @bug : no known bugs.
  * @param newUp[in] is the new up direction for the camera.
  */
  void
    setUpDir(const enVector3& newUp);

  /**
  * @brief :sets the 'front' direction of the camera
  * @bug : no known bugs.
  * @param x , y ,z [in]  set the new Front direction for the camera
  */
  void
    setFront(float x, float y, float z);

  /**
  * @brief :sets the 'front' direction of the camera
  * @bug : no known bugs.
  * @param newUp[in] is the new up direction for the camera.
  */
  void
    setFront(const enVector3& newFront);

  /**
  * @brief :sets the 'front' direction of the camera
  * @bug : no known bugs.
  * @param x , y ,z [in]  set the new Front direction for the camera
  */
  void
    setRight(float x, float y, float z);

  /**
  * @brief :sets the 'front' direction of the camera
  * @bug : no known bugs.
  * @param newUp[in] is the new up direction for the camera.
  */
  void
    setRight(const enVector3& newRight);

  /**
  * @brief :sets the 'height' direction of the camera
  * @bug : no known bugs.
  * @param newHeight [in] is the new up direction for the camera.
  */
  void
    setHeight(float newHeight);

  /**
  * @brief :sets the 'width' direction of the camera
  * @bug : no known bugs.
  * @param newWidth[in] is the new up direction for the camera.
  */
  void
    setWidth(float newWidth);

  /**
  * @brief : the 'm_near' variable
  * @bug : no known bugs.
  * @param newNear[in] is the new value for the near variable
  */
  void
    setFar(float newFar);

  /**
  * @brief : the 'm_near' variable
  * @bug : no known bugs.
  * @param newNear[in] is the new value for the near variable
  */
  void
    setNear(float newNear);

  /**
  * @brief :sets how fast the camera is.
  * @bug : no known bugs.
  * @param newSpeed[in] is the new up direction for the camera.
  */
  void
    setSpeed(float newSpeed);

protected:
  /**
  *  @brief : represent the following , where the camera is, how it's rotated .
  */
  enMatrix4x4 m_view;
  /**
  *  @brief : represent
  */
  enMatrix4x4 m_projection;
  /**
  *  @brief : represent where the camera is in world space(equivalent to eye).
  */
  enVector3 m_position;
  /**
  *  @brief : represent WHERE the camera is looking (equivalent to At).
  */
  enVector3 m_lookAt;
  /**
  *  @brief : it's the direction that represents 'up' relative to the camera
  */
  enVector3 m_up;
  /**
  *  @brief :it's the direction  that the camera is pointing too.
  */
  enVector3 m_front;
  /**
  * @brief :it's the direction that's to the cameras right
  */
  enVector3 m_right;
  /**
  *  @brief : used to control how wide the 'lens' of the camera is.
  */
  float m_fov{ 1.04719f };// 60 degrees in radians
  /**
  * @brief : determines the height of the projection
  */
  float m_height;
  /**
  * @brief : determines the width of the projection
  */
  float m_width;
  /**
    * @brief : determines how far an object can get before it ignores the object.
    */
  float m_far;
  /**
    * @brief : determines how close an object can get before it ignores the object.
    */
  float m_near;
  /**
  *  @brief : used to control how fast the camera moves in world-space.
  */
  float m_speed{ 10.0f };
};

