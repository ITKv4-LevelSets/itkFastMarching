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

#ifndef __itkFastMarchingThresholdStoppingCriterion_h
#define __itkFastMarchingThresholdStoppingCriterion_h

#include "itkFastMarchingStoppingCriterionBase.h"
#include "itkObjectFactory.h"

namespace itk
{
  template< class TTraits >
  class FastMarchingReachedTargetNodesStoppingCriterion :
      public FastMarchingStoppingCriterionBase< TTraits >
    {
  public:
    typedef FastMarchingReachedTargetNodesStoppingCriterion Self;
    typedef FastMarchingStoppingCriterionBase< TTraits > Superclass;
    typedef SmartPointer< Self >              Pointer;
    typedef SmartPointer< const Self >        ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(FastMarchingReachedTargetNodesStoppingCriterion,
                 FastMarchingStoppingCriterionBase );

    typedef typename Superclass::OutputPixelType  OutputPixelType;
    typedef typename Superclass::NodeType         NodeType;

    enum TargetConditionType { NoTargets = 0,
                               OneTarget,
                               SomeTargets,
                               AllTargets };

    /** Set/Get boolean macro indicating whether the user wants to check topology. */
    void SetTargetCondition( TargetConditionType iCondition )
      {
      m_TargetCondition = iCondition;
      m_Initialized = false;
      this->Modified();
      }

    itkGetConstReferenceMacro( TargetCondition, TargetConditionType );

    void SetNumberOfTargetsToBeReached( size_t iN )
      {
      m_NumberOfTargetsToBeReached = iN;
      m_Initialized = false;
      this->Modified();
      }

    /** \brief Set Target Nodes*/
    virtual void SetTargetNodes( std::vector< NodeType > iNodes )
      {
      m_TargetNodes = iNodes;
      m_Initialized = false;
      this->Modified();
      }

    virtual void AddTargetNode( NodeType iNode )
      {
      {
      m_TargetNodes.push_back( iNode );
      this->Modified();
      }
      }

    void SetCurrentNode( const NodeType& iNode )
      {
      if( !m_Initialized )
        {
        Initialize();
        }
      // Only check for reached targets if the mode is not NoTargets and
      // there is at least one TargetPoint.
      if ( m_TargetCondition != NoTargets &&  !m_TargetNodes.empty() )
        {
        typename std::vector< NodeType >::const_iterator
            pointsIter = m_TargetNodes.begin();
        typename std::vector< NodeType >::const_iterator
            pointsEnd = m_TargetNodes.end();

        while( pointsIter != pointsEnd )
          {
          if ( *pointsIter == iNode )
            {
            this->m_ReachedTargetNodes.push_back( iNode );
            m_Satisfied =
                ( m_ReachedTargetNodes.size() == m_NumberOfTargetsToBeReached );
            }
          ++pointsIter;
          }
        }
      m_Satisfied = false;
      }

    bool IsSatisfied() const
      {
      return m_Satisfied;
      }

  protected:
    FastMarchingThresholdStoppingCriterion() : Superclass()
    {
      m_TargetCondition = NoTargets;
      m_Satisfied = false;
      m_Initialized = false;
    }
    ~FastMarchingThresholdStoppingCriterion() {}

    TargetConditionType m_TargetCondition;
    std::vector< NodeType > m_TargetNodes;
    size_t m_NumberOfTargetsToBeReached;
    bool m_Satisfied;
    bool m_Initialized;

    void Initialize()
      {
      if( m_TargetCondition != NoTargets )
        {
        if( m_TargetCondition == OneTarget )
          {
          m_NumberOfTargetsToBeReached = 1;
          }
        if( m_TargetCondition == AllTargets )
          {
          m_NumberOfTargetsToBeReached = m_TargetNodes.size();
          }
        if( m_NumberOfTargetsToBeReached < 1 )
          {
          itkExceptionMacro(
            <<"Number of target nodes to be reached is null" );
          }
        if( m_NumberOfTargetsToBeReached > m_TargetNodes.size() )
          {
          itkExceptionMacro(
            <<"Number of target nodes to be reached is above the provided number of target nodes" );
          }
        m_ReachedTargetNodes.clear();
        }
      m_Initialized = true;
      }

  private:
    FastMarchingThresholdStoppingCriterion( const Self& );
    void operator = ( const Self& );
    };
}
#endif // __itkFastMarchingThresholdStoppingCriterion_h