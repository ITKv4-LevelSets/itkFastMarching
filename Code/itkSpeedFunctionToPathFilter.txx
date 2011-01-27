/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSpeedFunctionToPathFilter.txx,v $
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkSpeedFunctionToPathFilter_txx
#define __itkSpeedFunctionToPathFilter_txx

#include "vnl/vnl_math.h"
#include "itkSpeedFunctionToPathFilter.h"
#include "itkFastMarchingUpwindGradientImageFilter.h"
#include "itkImageFileWriter.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputPath>
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::SpeedFunctionToPathFilter()
{
  m_CurrentArrivalFunction = NULL;
}


/**
 *
 */
template <class TInputImage, class TOutputPath>
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::~SpeedFunctionToPathFilter()
{
}


/**
 *
 */
template<class TInputImage, class TOutputPath>
unsigned int
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::GetNumberOfPathsToExtract() const
{
  return m_Info.size();
}


/**
 *
 */
template<class TInputImage, class TOutputPath>
const typename SpeedFunctionToPathFilter<TInputImage,TOutputPath>::PointType &
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::GetNextEndPoint()
{
  return m_Info[Superclass::m_CurrentOutput].GetEndPoint();
}


/**
 *
 */
template<class TInputImage, class TOutputPath>
typename SpeedFunctionToPathFilter<TInputImage,TOutputPath>::InputImageType *
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::ComputeArrivalFunction()
{
  // Get the speed image
  InputImagePointer speed =
    const_cast< InputImageType * >( this->GetInput() );

  CriterionPointer criterion = CriterionType::New();
  FloatFMPointer marcher = FloatFMType::New();

  marcher->SetInput( speed );
  marcher->SetGenerateGradientImage( false );
//   marcher->SetTargetOffset( 2.0 * Superclass::m_TerminationValue );

  // Add next and previous front sources as target points to
  // limit the front propagation to just the required zones
  IndexType indexTargetPrevious;
  IndexType indexTargetNext;
  speed->TransformPhysicalPointToIndex
  (
  m_Info[Superclass::m_CurrentOutput].PeekPreviousFront(),
   indexTargetPrevious
   );
   speed->TransformPhysicalPointToIndex
   (
   m_Info[Superclass::m_CurrentOutput].PeekNextFront(),
    indexTargetNext
    );

  // Update the method and set the arrival function
  typename std::vector< NodeType > TargetNodes;
  TargetNodes.push_back( indexTargetPrevious );
  TargetNodes.push_back( indexTargetNext );

  criterion->SetTargetNodes( TargetNodes );
  criterion->SetTargetCondition( CriterionType::AllTargets );

  // Get the next Front source point and add as trial point
  IndexType indexTrial;
  speed->TransformPhysicalPointToIndex
  (
    m_Info[Superclass::m_CurrentOutput].GetCurrentFrontAndAdvance(),
    indexTrial
  );

  typename NodePairContainerType::Pointer TrialNodes = NodePairContainerType::New();
  TrialNodes->push_back( NodePairType( indexTrial, 0.0 ) );
  marcher->SetTrialNodes( TrialNodes );

  marcher->SetStoppingCriterion( criterion );
  marcher->UpdateLargestPossibleRegion( );
  m_CurrentArrivalFunction = marcher->GetOutput( );
  m_CurrentArrivalFunction->DisconnectPipeline( );
  return m_CurrentArrivalFunction;
}


/**
 *
 */
template <class TInputImage, class TOutputPath>
void
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::GenerateData( void )
{
  // Get the speed function
  InputImagePointer speed =
    const_cast< InputImageType * >( this->GetInput() );
  if ( speed.IsNull() )
    {
    itkExceptionMacro( "Speed function image must be provided" );
    }

  // Ensure the user has added at least one path info object
  if ( m_Info.size() == 0 )
    {
    itkExceptionMacro( "No PathInfo objects: at least one must be added." );
    }

  // Extract the path
  Superclass::GenerateData( );
}


/**
 *
 */
template <class TInputImage, class TOutputPath>
void
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::Execute( const Object * object, const EventObject & )
{
  // Cast object to optmizer
  typename OptimizerType::Pointer optimizer = (OptimizerType*)
      dynamic_cast< const OptimizerType* >( object );
  if ( optimizer.IsNull() ) return;

  // Get current position and value
  typename OptimizerType::ParametersType currentParameters = optimizer->GetCurrentPosition();
  unsigned int lenParameters = currentParameters.GetSize();
  if ( lenParameters != InputImageDimension ) return;
  typename OptimizerType::MeasureType currentValue = optimizer->GetValue( currentParameters );

  // Convert parameters to point
  bool valid = false;
  unsigned int numparams = optimizer->GetCurrentPosition().GetSize();
  PointType point; point.Fill( 0.0 );
  for ( unsigned int i=0; i<numparams; i++ )
    {
    point[i] = optimizer->GetCurrentPosition()[i];
    valid = true;
    }
  if ( !valid ) return;

  // Check if we have reached the termination value
  if ( currentValue < Superclass::m_TerminationValue &&
       m_Info[Superclass::m_CurrentOutput].HasNextFront() )
    {
    // We have terminated the current path segment,
    // but there are more fronts to propagate

    // TODO: The path has not actually reached the path point.
    //       Change the next front point to be the current point.

    // Update the arrival function and re-initialise the cost function
    Superclass::m_CostFunction->SetImage( this->ComputeArrivalFunction( ) );
    Superclass::m_CostFunction->Initialize( );
    }
  else if ( currentValue >= Superclass::m_TerminationValue )
    {
    // Convert point to continuous index
    InputImagePointer input = const_cast<InputImageType*>( this->GetInput() );
    ContinuousIndexType cindex;
    input->TransformPhysicalPointToContinuousIndex( point, cindex );

    // Add point as vertex in path
    OutputPathPointer output = this->GetOutput( Superclass::m_CurrentOutput );
    output->AddVertex( cindex );
    }
}


/**
 *
 */
template<class TInputImage, class TOutputPath>
void
SpeedFunctionToPathFilter<TInputImage,TOutputPath>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace itk

#endif
