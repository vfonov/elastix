/*======================================================================

This file is part of the elastix software.

Copyright (c) University Medical Center Utrecht. All rights reserved.
See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __itkCyclicBSplineDeformableTransform_h
#define __itkCyclicBSplineDeformableTransform_h

#include "itkAdvancedTransform.h"
#include "itkImage.h"
#include "itkImageRegion.h"
#include "itkBSplineInterpolationWeightFunction2.h"
#include "itkBSplineInterpolationDerivativeWeightFunction.h"
#include "itkBSplineInterpolationSecondOrderDerivativeWeightFunction.h"
#include "itkAdvancedBSplineDeformableTransform.h"
#include "itkCyclicBSplineDeformableTransform.h"

namespace itk
{

/** \class CyclicBSplineDeformableTransform
 * \brief Deformable transform using a BSpline representation in which the
 *   B-spline grid is formulated in a cyclic way
 * \ingroup Transforms
 */
template <
    class TScalarType = double,          // Data type for scalars
    unsigned int NDimensions = 3,        // Number of dimensions
    unsigned int VSplineOrder = 3 >      // Spline order
class ITK_EXPORT CyclicBSplineDeformableTransform
  : public AdvancedBSplineDeformableTransform< TScalarType, NDimensions, NDimensions >
{
public:
  /** Standard class typedefs. */
  typedef CyclicBSplineDeformableTransform          Self;
  typedef AdvancedBSplineDeformableTransform<
    TScalarType, NDimensions, NDimensions >         Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  /** New macro for creation of through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( CyclicBSplineDeformableTransform, AdvancedBSplineDeformableTransform );

  /** Dimension of the domain space. */
  itkStaticConstMacro( SpaceDimension, unsigned int, NDimensions );

  /** The BSpline order. */
  itkStaticConstMacro( SplineOrder, unsigned int, VSplineOrder );

  typedef typename Superclass
    ::NonZeroJacobianIndicesType                    NonZeroJacobianIndicesType;
  typedef typename Superclass::SpatialJacobianType  SpatialJacobianType;
  typedef typename Superclass
    ::JacobianOfSpatialJacobianType                 JacobianOfSpatialJacobianType;
  typedef typename Superclass::SpatialHessianType   SpatialHessianType;
  typedef typename Superclass
    ::JacobianOfSpatialHessianType                  JacobianOfSpatialHessianType;
  typedef typename Superclass::InternalMatrixType   InternalMatrixType;
  
  /** Parameters as SpaceDimension number of images. */
  typedef typename ParametersType::ValueType            PixelType;
  typedef Image< PixelType,
    itkGetStaticConstMacro( SpaceDimension )>           ImageType;
  typedef typename ImageType::Pointer                   ImagePointer;

  /** Typedefs for specifying the extend to the grid. */
  typedef ImageRegion< itkGetStaticConstMacro( SpaceDimension ) > RegionType;
  typedef ImageRegion< itkGetStaticConstMacro( SpaceDimension - 1 ) > RegionRedDimType;
  
  typedef typename RegionType::IndexType        IndexType;
  typedef typename RegionType::SizeType         SizeType;
  typedef typename RegionRedDimType::IndexType  IndexRedDimType;
  typedef typename RegionRedDimType::SizeType   SizeRedDimType;
  typedef typename ImageType::SpacingType       SpacingType;
  typedef typename ImageType::DirectionType     DirectionType;
  typedef typename ImageType::PointType         OriginType;
  typedef IndexType                             GridOffsetType;

  /** This method specifies the region over which the grid resides. */
  virtual void SetGridRegion( const RegionType& region );

  /** Transform points by a BSpline deformable transformation. 
   * On return, weights contains the interpolation weights used to compute the 
   * deformation and indices of the x (zeroth) dimension coefficient parameters
   * in the support region used to compute the deformation.
   * Parameter indices for the i-th dimension can be obtained by adding
   * ( i * this->GetNumberOfParametersPerDimension() ) to the indices array.
   */
  virtual void TransformPoint(
    const InputPointType & inputPoint,
    OutputPointType & outputPoint,
    WeightsType & weights,
    ParameterIndexArrayType & indices,
    bool & inside ) const;

  /** Compute the Jacobian matrix of the transformation at one point. */
  virtual const JacobianType & GetJacobian( const InputPointType & point ) const;

  /** Compute the Jacobian of the transformation. */
  virtual void GetJacobian(
    const InputPointType & ipp,
    WeightsType & weights,
    ParameterIndexArrayType & indices ) const;

protected:
  CyclicBSplineDeformableTransform();
  virtual ~CyclicBSplineDeformableTransform();

  void ComputeNonZeroJacobianIndices(
    NonZeroJacobianIndicesType & nonZeroJacobianIndices,
    const RegionType & supportRegion ) const;

  /** Check if a continuous index is inside the valid region. */
  bool InsideValidRegion( const ContinuousIndexType& index ) const;

  RegionRedDimType m_ValidRegionRedDim;

  /** Reduce the dimensionality of the valid region with one and store the 
      result in m_ValidRegionRedDim. */
  virtual void ReduceDimensionValidRegion();
  
  /** Split an image region into two regions based on the last dimension. */
  virtual void SplitRegion( 
    const RegionType & imageRegion,
    const RegionType & inRegion, 
    RegionType & outRegion1, 
    RegionType & outRegion2) const;

private:
  CyclicBSplineDeformableTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; //class CyclicBSplineDeformableTransform


}  // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_CyclicBSplineDeformableTransform(_, EXPORT, x, y) namespace itk { \
  _(3(class EXPORT CyclicBSplineDeformableTransform< ITK_TEMPLATE_3 x >)) \
  namespace Templates { typedef CyclicBSplineDeformableTransform< ITK_TEMPLATE_3 x > \
                                                   CyclicBSplineDeformableTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkCyclicBSplineDeformableTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkCyclicBSplineDeformableTransform.txx"
#endif

#endif /* __itkCyclicBSplineDeformableTransform_h */
