/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef __itkFastMarchingImageFilterBase_h
#define __itkFastMarchingImageFilterBase_h

#include "itkFastMarchingBase.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNeighborhoodIterator.h"
#include "itkFastMarchingTraits.h"

namespace itk
{
/**
 * \class FastMarchingImageFilterBase
 * \brief Fast Marching Method on Image
 *
 * The speed function can be specified as a speed image or a
 * speed constant. The speed image is set using the method
 * SetInput(). If the speed image is NULL, a constant speed function
 * is used and is specified using method the SetSpeedConstant().
 *
 * If the speed function is constant and of value one, fast marching results
 * in an approximate distance function from the initial alive points.
 *
 * There are two ways to specify the output image information
 * ( LargestPossibleRegion, Spacing, Origin):
 * \li it is copied directly from the input speed image
 * \li it is specified by the user.
 * Default values are used if the user does not specify all the information.
 *
 * The output information is computed as follows.
 *
 * If the speed image is NULL or if the OverrideOutputInformation is set to
 * true, the output information is set from user specified parameters. These
 * parameters can be specified using methods
 * \li FastMarchingImageFilterBase::SetOutputRegion(),
 * \li FastMarchingImageFilterBase::SetOutputSpacing(),
 * \li FastMarchingImageFilterBase::SetOutputDirection(),
 * \li FastMarchingImageFilterBase::SetOutputOrigin().
 *
 * Else the output information is copied from the input speed image.
*/
template< unsigned int VDimension,
         typename TInputPixel,
         typename TOutputPixel >
class FastMarchingImageFilterBase :
    public FastMarchingBase<
      ImageFastMarchingTraits< VDimension, TInputPixel, TOutputPixel >
    >
  {
public:
  typedef ImageFastMarchingTraits< VDimension, TInputPixel, TOutputPixel > Traits;

  typedef FastMarchingImageFilterBase            Self;
  typedef FastMarchingBase< Traits >             Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef SmartPointer< const Self >  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FastMarchingImageFilterBase, FastMarchingBase);


  typedef typename Superclass::InputDomainType     InputImageType;
  typedef typename Superclass::InputDomainPointer  InputImagePointer;
  typedef typename Superclass::InputPixelType      InputPixelType;

  typedef typename Superclass::OutputDomainType     OutputImageType;
  typedef typename Superclass::OutputDomainPointer  OutputImagePointer;
  typedef typename Superclass::OutputPixelType      OutputPixelType;
  typedef typename OutputImageType::SpacingType     OutputSpacingType;
  typedef typename OutputImageType::SizeType        OutputSizeType;
  typedef typename OutputImageType::RegionType      OutputRegionType;
  typedef typename OutputImageType::PointType       OutputPointType;
  typedef typename OutputImageType::DirectionType   OutputDirectionType;

  typedef typename Traits::NodeType                 NodeType;
  typedef typename Traits::NodePairType             NodePairType;
  typedef typename Traits::NodePairContainerType    NodePairContainerType;
  typedef typename Traits::NodePairContainerPointer NodePairContainerPointer;
  typedef typename Traits::NodePairContainerConstIterator
    NodePairContainerConstIterator;

  typedef typename Traits::NodeContainerType        NodeContainerType;
  typedef typename Traits::NodeContainerPointer     NodeContainerPointer;
  typedef typename Traits::NodeContainerConstIterator
    NodeContainerConstIterator;

  /*
  typedef typename Superclass::ElementIdentifier ElementIdentifier;

  typedef typename Superclass::PriorityQueueElementType PriorityQueueElementType;

  typedef typename Superclass::PriorityQueueType PriorityQueueType;
  typedef typename Superclass::PriorityQueuePointer PriorityQueuePointer;
  */

  typedef typename Superclass::LabelType LabelType;

  itkStaticConstMacro( ImageDimension, unsigned int, VDimension );

  typedef Image< unsigned char, ImageDimension >  LabelImageType;
  typedef typename LabelImageType::Pointer        LabelImagePointer;

  typedef Image< unsigned int, ImageDimension >
    ConnectedComponentImageType;
  typedef typename ConnectedComponentImageType::Pointer ConnectedComponentImagePointer;

  typedef NeighborhoodIterator<LabelImageType> NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType NeighborhoodRadiusType;

  itkGetObjectMacro( LabelImage, LabelImageType );

  /** \brief Set a binary mask to limit the propagation of front.
    *
    * Only location with null pixel values are used. Corresponding labels are set
    * to Forbidden to limit the propagation of the front and output values are set
    * to 0.
    *
    * \param[in] iImage input image
    * \tparam TPixel PixelType for the input image
   */
  template< typename TPixel >
  void SetBinaryMask( Image< TPixel, ImageDimension >* iImage )
    {
    OutputSizeType nullsize;
    nullsize.Fill( 0 );

    if( this->m_ForbiddenNodes.IsNull() )
      {
      this->m_ForbiddenNodes = NodeContainerType::New();
      }


    //if( m_BufferedRegion.GetSize() == nullsize )
    //  {
    //  itkGenericExceptionMacro( << "m_BufferedRegion has not been set yet" );
    //  }
    //else
      {
      typedef Image< TPixel, ImageDimension > InternalImageType;
      typedef ImageRegionConstIteratorWithIndex< InternalImageType >
        InternalRegionIterator;
      InternalRegionIterator b_it( iImage, iImage->GetLargestPossibleRegion() );
      b_it.GoToBegin();

      //OutputImageType* output = this->GetOutput();

      TPixel zero_value = NumericTraits< TPixel >::Zero;
      NodeType idx;

      while( !b_it.IsAtEnd() )
        {
        if( b_it.Get() == zero_value )
          {
          idx = b_it.GetIndex();

          this->m_ForbiddenNodes->push_back( idx );
          //if ( m_BufferedRegion.IsInside( idx ) )
          //  {
          //  m_LabelImage->SetPixel(idx, Superclass::Forbidden );
          //  output->SetPixel (idx, zero_value );
          //  }
          }
        ++b_it;
        }
      this->Modified();
      }
    }

  /** The output largeset possible, spacing and origin is computed as follows.
   * If the speed image is NULL or if the OverrideOutputInformation is true,
   * the output information is set from user specified parameters. These
   * parameters can be specified using methods SetOutputRegion(), SetOutputSpacing(), SetOutputDirection(),
   * and SetOutputOrigin(). Else if the speed image is not NULL, the output information
   * is copied from the input speed image. */
  virtual void SetOutputSize(const OutputSizeType & size)
  { m_OutputRegion = size; }
  virtual OutputSizeType GetOutputSize() const
  { return m_OutputRegion.GetSize(); }
  itkSetMacro(OutputRegion, OutputRegionType);
  itkGetConstReferenceMacro(OutputRegion, OutputRegionType);
  itkSetMacro(OutputSpacing, OutputSpacingType);
  itkGetConstReferenceMacro(OutputSpacing, OutputSpacingType);
  itkSetMacro(OutputDirection, OutputDirectionType);
  itkGetConstReferenceMacro(OutputDirection, OutputDirectionType);
  itkSetMacro(OutputOrigin, OutputPointType);
  itkGetConstReferenceMacro(OutputOrigin, OutputPointType);
  itkSetMacro(OverrideOutputInformation, bool);
  itkGetConstReferenceMacro(OverrideOutputInformation, bool);
  itkBooleanMacro(OverrideOutputInformation);

protected:

  /** Constructor */
  FastMarchingImageFilterBase();

  /** Destructor */
  ~FastMarchingImageFilterBase();

  struct InternalNodeStructure;

  OutputRegionType m_BufferedRegion;
  NodeType m_StartIndex;
  NodeType m_LastIndex;

  OutputRegionType    m_OutputRegion;
  OutputPointType     m_OutputOrigin;
  OutputSpacingType   m_OutputSpacing;
  OutputDirectionType m_OutputDirection;
  bool                m_OverrideOutputInformation;

  /** Generate the output image meta information. */
  virtual void GenerateOutputInformation();

  virtual void EnlargeOutputRequestedRegion(DataObject *output);

  LabelImagePointer m_LabelImage;
  ConnectedComponentImagePointer                m_ConnectedComponentImage;

  IdentifierType GetTotalNumberOfNodes() const;

  /** Returns the output value for a given node */
  OutputPixelType GetOutputValue( OutputImageType* oImage,
                                  const NodeType& iNode ) const;

  /** Returns the label value for a given node */
  char GetLabelValueForGivenNode( const NodeType& iNode ) const;

  /** Set the label value for a given node */
  void SetLabelValueForGivenNode( const NodeType& iNode,
                                 const LabelType& iLabel );

  /** Update values for the neighbors of a given node */
  virtual void UpdateNeighbors( OutputImageType* oImage, const NodeType& iNode );

  /** Update value for a given node */
  virtual void UpdateValue( OutputImageType* oImage, const NodeType& iValue );

  /** Make sure the given node does not violate any topological constraint*/
  bool CheckTopology( OutputImageType* oImage, const NodeType& iNode );
  void InitializeOutput( OutputImageType* oImage );

  /** Find the nodes were the front will propagate given a node */
  void GetInternalNodesUsed( OutputImageType* oImage,
                            const NodeType& iNode,
                            std::vector< InternalNodeStructure >& ioNodesUsed );

  /** Solve the quadratic equation */
  double Solve( OutputImageType* oImage,
               const NodeType& iNode,
               std::vector< InternalNodeStructure >& ioNeighbors ) const;

  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------

  /**
   * Functions and variables to check for topology changes (2D/3D only).
   */

  // Functions/data for the 2-D case
  void InitializeIndices2D();
  bool IsChangeWellComposed2D( const NodeType& ) const;
  bool IsCriticalC1Configuration2D( const std::vector<bool>& ) const;
  bool IsCriticalC2Configuration2D( const std::vector<bool>& ) const;
  bool IsCriticalC3Configuration2D( const std::vector<bool>& ) const;
  bool IsCriticalC4Configuration2D( const std::vector<bool>& ) const;

  Array<unsigned char>                        m_RotationIndices[4];
  Array<unsigned char>                        m_ReflectionIndices[2];

  // Functions/data for the 3-D case
  void InitializeIndices3D();
  bool IsCriticalC1Configuration3D( const std::vector<bool>& ) const;
  unsigned int IsCriticalC2Configuration3D( const std::vector<bool>& ) const;
  bool IsChangeWellComposed3D( const NodeType& ) const;

  Array<unsigned char>                        m_C1Indices[12];
  Array<unsigned char>                        m_C2Indices[8];

  // Functions for both 2D/3D cases
  bool DoesVoxelChangeViolateWellComposedness( const NodeType& ) const;
  bool DoesVoxelChangeViolateStrictTopology( const NodeType& ) const;

private:
  FastMarchingImageFilterBase( const Self& );
  void operator = ( const Self& );
  };
}

#include "itkFastMarchingImageFilterBase.txx"
#endif // __itkFastMarchingImageFilterBase_h
