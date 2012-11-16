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

#ifndef __itkCellSubdivisionQuadEdgeMeshFilter_h
#define __itkCellSubdivisionQuadEdgeMeshFilter_h

#include "itkSubdivisionQuadEdgeMeshFilter.h"

namespace itk
{
/**
 * \class CellSubdivisionQuadEdgeMeshFilter
 *
 * \brief FIXME
 * \ingroup ITK-QuadEdgeMeshFiltering
 */
template< typename TInputMesh, typename TOutputMesh >
class CellSubdivisionQuadEdgeMeshFilter:
  public SubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
{
public:
  typedef CellSubdivisionQuadEdgeMeshFilter                            Self;
  typedef SubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >     Superclass;
  typedef SmartPointer< Self >                                         Pointer;
  typedef SmartPointer< const Self >                                   ConstPointer;

  typedef typename Superclass::InputMeshType                           InputMeshType;
  typedef typename Superclass::InputMeshPointer                        InputMeshPointer;
  typedef typename Superclass::InputMeshConstPointer                   InputMeshConstPointer;
  typedef typename Superclass::InputPointsContainer                    InputPointsContainer;
  typedef typename Superclass::InputPointsContainerPointer             InputPointsContainerPointer;
  typedef typename Superclass::InputPointsContainerConstIterator       InputPointsContainerConstIterator;
  typedef typename Superclass::InputPointsContainerIterator            InputPointsContainerIterator;
  typedef typename Superclass::InputCellsContainer                     InputCellsContainer;
  typedef typename Superclass::InputCellsContainerPointer              InputCellsContainerPointer;
  typedef typename Superclass::InputCellsContainerIterator             InputCellsContainerIterator;
  typedef typename Superclass::InputCellsContainerConstIterator        InputCellsContainerConstIterator;
  typedef typename Superclass::InputPointType                          InputPointType;
  typedef typename Superclass::InputCoordRepType                       InputCoordType;
  typedef typename Superclass::InputPointIdentifier                    InputPointIdentifier;
  typedef typename Superclass::InputCellIdentifier                     InputCellIdentifier;
  typedef typename Superclass::InputCellType                           InputCellType;
  typedef typename Superclass::InputQEType                             InputQEType;
  typedef typename Superclass::InputMeshTraits                         InputMeshTraits;
  typedef typename Superclass::InputPointIdIterator                    InputPointIdIterator;

  typedef typename Superclass::OutputMeshType                          OutputMeshType;
  typedef typename Superclass::OutputMeshPointer                       OutputMeshPointer;
  typedef typename Superclass::OutputPointsContainerPointer            OutputPointsContainerPointer;
  typedef typename Superclass::OutputPointsContainerIterator           OutputPointsContainerIterator;
  typedef typename Superclass::OutputCellsContainer                    OutputCellsContainer;
  typedef typename Superclass::OutputCellsContainerPointer             OutputCellsContainerPointer;
  typedef typename Superclass::OutputCellsContainerIterator            OutputCellsContainerIterator;
  typedef typename Superclass::OutputCellsContainerConstIterator       OutputCellsContainerConstIterator;
  typedef typename Superclass::OutputPointType                         OutputPointType;
  typedef typename Superclass::OutputCoordRepType                      OutputCoordType;
  typedef typename Superclass::OutputPointIdentifier                   OutputPointIdentifier;
  typedef typename Superclass::OutputCellIdentifier                    OutputCellIdentifier;
  typedef typename Superclass::OutputCellType                          OutputCellType;
  typedef typename Superclass::OutputQEType                            OutputQEType;
  typedef typename Superclass::OutputMeshTraits                        OutputMeshTraits;
  typedef typename Superclass::OutputPointIdIterator                   OutputPointIdIterator;

  typedef std::list< OutputCellIdentifier >                            OutputCellIdentifierListType;
  typedef typename OutputCellIdentifierListType::const_iterator        OutputCellIdentifierListConstIterator;

  /** Run-time type information (and related methods).   */
  itkTypeMacro(CellSubdivisionQuadEdgeMeshFilter, SubdivisionQuadEdgeMeshFilter);

  itkGetConstReferenceMacro(CellsToBeSubdivided, OutputCellIdentifierListType);
  void SetCellsToBeSubdivided(const OutputCellIdentifierListType & cellIdList)
    {
    m_CellsToBeSubdivided = cellIdList;
    this->Modified();
    }

  void AddSubdividedCellId(OutputCellIdentifier cellId){m_CellsToBeSubdivided.push_back(cellId);}

protected:
  CellSubdivisionQuadEdgeMeshFilter();

  virtual ~CellSubdivisionQuadEdgeMeshFilter() {}

  virtual void AddNewPoints( InputCellType *cell ) = 0;
  virtual void GenerateOutputPoints( );
  virtual void GenerateOutputCells( );

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  CellSubdivisionQuadEdgeMeshFilter(const Self &); // purposely not implemented
  void operator=(const Self &); // purposely not implemented

protected:
  OutputCellIdentifierListType        m_CellsToBeSubdivided;
  bool                                m_Uniform;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCellSubdivisionQuadEdgeMeshFilter.hxx"
#endif

#endif
