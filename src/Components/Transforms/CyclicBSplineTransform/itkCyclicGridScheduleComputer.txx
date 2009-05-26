/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __itkCyclicGridScheduleComputer_TXX__
#define __itkCyclicGridScheduleComputer_TXX__

#include "itkCyclicGridScheduleComputer.h"

#include "itkImageRegionExclusionConstIteratorWithIndex.h"


namespace itk
{

/**
 * ********************* Constructor ****************************
 */
  
template < typename TTransformScalarType, unsigned int VImageDimension >
CyclicGridScheduleComputer<TTransformScalarType, VImageDimension>
::CyclicGridScheduleComputer()
{

} // end Constructor()


/**
 * ********************* ComputeBSplineGrid ****************************
 */

template < typename TTransformScalarType, unsigned int VImageDimension >
void
CyclicGridScheduleComputer<TTransformScalarType, VImageDimension>
::ComputeBSplineGrid( void )
{
  /** Call superclass method. */
  //Superclass::ComputeBSplineGrid();

  OriginType imageOrigin;
  SpacingType imageSpacing, finalGridSpacing;

  /** Apply the initial transform. */    
  this->ApplyInitialTransform( imageOrigin, imageSpacing, finalGridSpacing );

  /** Set the appropriate sizes. */
  this->m_GridOrigins.resize( this->GetNumberOfLevels() );
  this->m_GridRegions.resize( this->GetNumberOfLevels() );
  this->m_GridSpacings.resize( this->GetNumberOfLevels() );

  /** Epsilon for 

  /** For all levels ... */
  for ( unsigned int res = 0; res < this->GetNumberOfLevels(); ++res )
  {
    /** For all dimensions ... */
    SizeType size = this->GetImageRegion().GetSize();
    SizeType gridsize;
    for ( unsigned int dim = 0; dim < Dimension; ++dim )
    {
      /** Compute the grid spacings. */
      double gridSpacing
        = finalGridSpacing[ dim ] * this->m_GridSpacingFactors[ res ][ dim ];
      this->m_GridSpacings[ res ][ dim ] = gridSpacing;

      /** Check if the grid spacing matches the cyclic behaviour of this 
       * transform. We want the spacing at the borders for the last dimension
       * to be half the grid spacing.
       */
      if (dim == Dimension - 1) 
      {
        const float lastDimSizeInPhysicalUnits = 
               imageSpacing[ dim ] * this->GetImageRegion().GetSize( dim );
        
        /** Compute closest correct spacing. */
          
        /** Compute number of nodes. */
        const int numNodes = static_cast<int>( lastDimSizeInPhysicalUnits / 
                                     gridSpacing );
          
        /** Compute new (larger) gridspacing. */
        gridSpacing = 
              lastDimSizeInPhysicalUnits / static_cast<float> ( numNodes );
      }

      /** Compute the grid size without the extra grid points at the edges. */
      const unsigned int bareGridSize = static_cast<unsigned int>(
        vcl_ceil( size[ dim ] * imageSpacing[ dim ] / gridSpacing ) );

      /** The number of B-spline grid nodes is the bareGridSize plus the
       * B-spline order more grid nodes (for all dimensions but the last).
       * The last dimension has the bareGridSize.
       */
      gridsize[ dim ] = static_cast<SizeValueType>( bareGridSize );
      if (dim < Dimension - 1)
      {
        gridsize[ dim ] += static_cast<SizeValueType> ( this->GetBSplineOrder() );
      }

      /** Compute the origin of the B-spline grid. */
      this->m_GridOrigins[ res ][ dim ] = imageOrigin[ dim ] -
        ( ( gridsize[ dim ] - 1 ) * gridSpacing
        - ( size[ dim ] - 1 ) * imageSpacing[ dim ] ) / 2.0;
    }

    /** Set the grid region. */
    this->m_GridRegions[ res ].SetSize( gridsize );
  }

} // end ComputeBSplineGrid()

} // end namespace itk

#endif // end #ifndef __itkCyclicGridScheduleComputer_TXX__
