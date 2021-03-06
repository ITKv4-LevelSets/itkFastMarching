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

#ifndef __itkFastMarchingTraits_h
#define __itkFastMarchingTraits_h

#include "itkIntTypes.h"

#include "itkImage.h"
#include "itkImageToImageFilter.h"

#include "itkMesh.h"
#include "itkMeshToMeshFilter.h"

namespace itk
{
/**  \class FastMarchingTraits
  \brief Base class traits to be used by the FastMarchingBase
  */
template< class TInputDomain,
          class TNode,
          class TOutputDomain,
          class TSuperclass >
class FastMarchingTraits
  {
public:
  typedef TInputDomain                                        InputDomainType;
  typedef typename InputDomainType::Pointer                   InputDomainPointer;
  typedef typename InputDomainType::PixelType                 InputPixelType;

  typedef TNode                                               NodeType;

  typedef TOutputDomain                                       OutputDomainType;
  typedef typename OutputDomainType::Pointer                  OutputDomainPointer;
  typedef typename OutputDomainType::PixelType                OutputPixelType;

  class NodePair : public std::pair< NodeType, OutputPixelType >
    {
  public:
    typedef NodePair Self;
    typedef std::pair< NodeType, OutputPixelType > Superclass;

    NodePair() : Superclass() {}
    NodePair( const NodeType& iNode, const OutputPixelType& iValue ) :
      Superclass( iNode, iValue ) {}
    NodePair( const Self& iPair ) : Superclass( iPair ) {}

    void SetValue( const OutputPixelType& iValue )
      {
      this->second = iValue;
      }
    OutputPixelType GetValue() const
      {
      return this->second;
      }
    void SetNode( const NodeType& iNode )
      {
      this->first = iNode;
      }
    NodeType GetNode() const
      {
      return this->first;
      }

    bool operator < ( const Self& iRight ) const
      {
      return this->second < iRight.second;
      }

    bool operator > ( const Self& iRight ) const
      {
      return this->second > iRight.second;
      }

    bool operator <= ( const Self& iRight ) const
      {
      return this->second <= iRight.second;
      }

    bool operator >= ( const Self& iRight ) const
      {
      return this->second >= iRight.second;
      }
    };

  typedef NodePair                                         NodePairType;
  typedef VectorContainer< IdentifierType, NodePairType >  NodePairContainerType;
  typedef typename NodePairContainerType::Pointer          NodePairContainerPointer;
  typedef typename NodePairContainerType::Iterator         NodePairContainerIterator;
  typedef typename NodePairContainerType::ConstIterator    NodePairContainerConstIterator;

  typedef VectorContainer< IdentifierType, NodeType >      NodeContainerType;
  typedef typename NodeContainerType::Pointer              NodeContainerPointer;
  typedef typename NodeContainerType::Iterator             NodeContainerIterator;
  typedef typename NodeContainerType::ConstIterator        NodeContainerConstIterator;

  typedef TSuperclass                                      SuperclassType;

#ifdef ITK_USE_CONCEPT_CHECKING
  itkConceptMacro( DoubleConvertibleOutputCheck,
                  ( Concept::Convertible< double, OutputPixelType > ) );

  itkConceptMacro( OutputOStreamWritableCheck,
                  ( Concept::OStreamWritable< OutputPixelType > ) );
#endif
  };

/**  \class ImageFastMarchingTraits
  \brief traits to be used when dealing with Image for FastMarchingBase
*/
template<unsigned int VDimension,
         typename TInputPixel,
         typename TOutputPixel >
class ImageFastMarchingTraits :
    public FastMarchingTraits<
      Image< TInputPixel, VDimension >,
      Index< VDimension >,
      Image< TOutputPixel, VDimension >,
      ImageToImageFilter< Image< TInputPixel, VDimension >,
                          Image< TOutputPixel, VDimension > >
    >
  {
  itkStaticConstMacro(ImageDimension, unsigned int, VDimension);
  };

/**  \class MeshFastMarchingTraits
  \brief traits to be used when dealing with Mesh for FastMarchingBase
*/
template< unsigned int VDimension,
          typename TInputPixel,
          class TInputMeshTraits,
          typename TOutputPixel,
          class TOutputMeshTraits >
class MeshFastMarchingTraits :
    public FastMarchingTraits<
      Mesh< TInputPixel, VDimension, TInputMeshTraits >,
      typename TInputMeshTraits::PointIdentifier,
      Mesh< TOutputPixel, VDimension, TOutputMeshTraits >,
      MeshToMeshFilter< Mesh< TInputPixel, VDimension, TInputMeshTraits >,
                        Mesh< TOutputPixel, VDimension, TOutputMeshTraits > >
    >
  {
  itkStaticConstMacro(PointDimension, unsigned int, VDimension);
  };
}
#endif // __itkFastMarchingTraits_h
