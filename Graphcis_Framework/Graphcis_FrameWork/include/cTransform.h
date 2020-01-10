#pragma once
#include <vector>
#include "MathTypes.h"

/*class cDevice;
class cDeviceContext;
class cConstBuffer;*/

class cTransform 
{
public:
  cTransform() = default;
  cTransform(const cTransform &other) = default;
  cTransform(cTransform &&other) = default;

  ~cTransform() = default;
public:
  cTransform& operator =(const cTransform& other);
public:

  void
    Init() noexcept;

  /*! this will move the current transform matrix in a direction */
  void
    moveTransform(float x, float y, float z, float w = 1.0f,float deltaTime = 0.1666f);

  /*! will rotate the transform in the X axis */
  void
    rotateInXAxis(float AngleInDegrees);

  /*! will rotate the transform in the Y axis */
  void
    rotateInYAxis(float AngleInDegrees);

  /*! will rotate the transform in the Z axis */
  void
    rotateInZAxis(float AngleInDegrees);
  
  /*!make the current transform equal to the identity matrix*/
  void
    resetToIdentity();

/*! deforms the matrix in the X Axis  */
  void
    shearTransformInXAxis(float shearingAmount);

/*! deforms the matrix in the X Axis  */
  void
    shearTransformInYAxis(float shearingAmount);

/*! deforms the matrix in the X Axis  */
  void
    shearTransformInZAxis(float shearingAmount);

  /*! does a transform that make the model look
  like it's own reflection in the mirror 
  https://www.varsitytutors.com/hotmath/hotmath_help/topics/transformation-of-graphs-using-matrices-reflection */
  void
    reflectTransfromInXAxis(float Amount);

  /*! does a transform that make the model look
  like it's own reflection in the mirror 
  https://www.varsitytutors.com/hotmath/hotmath_help/topics/transformation-of-graphs-using-matrices-reflection */
  void
    reflectTransfromInYAxis(float Amount);

  /*! does a transform that make the model look
  like it's own reflection in the mirror 
  https://www.varsitytutors.com/hotmath/hotmath_help/topics/transformation-of-graphs-using-matrices-reflection  */
  void
    reflectTransfromInZAxis(float Amount);


public: // get and set functions 

  /*!returns the matrix used to keep track of transform */
  enMatrix4x4 
    getMatrix()const;

private:

  //!this is the variable that contains all transform related operation
   enMatrix4x4 m_transformMatrix;
};
