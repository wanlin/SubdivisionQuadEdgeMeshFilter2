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

#ifndef __itkEdgeCellSubdivisionQuadEdgeMeshFilter_hxx
#define __itkEdgeCellSubdivisionQuadEdgeMeshFilter_hxx

#include "itkEdgeCellSubdivisionQuadEdgeMeshFilter.h"

namespace itk
{
template< typename TInputMesh, typename TOutputMesh >
EdgeCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::EdgeCellSubdivisionQuadEdgeMeshFilter()
{
}

template< typename TInputMesh, typename TOutputMesh >
void
EdgeCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::GenerateOutputPoints()
{
  //1. Copy points from input to output
  this->CopyInputMeshToOutputMeshPoints();

  //2. Add new points
  InputEdgeListConstIterator it  = this->m_EdgesToBeSubdivided.begin();
  InputEdgeListConstIterator end = this->m_EdgesToBeSubdivided.end();

  while( it != end )
    {
    this->AddNewPoints( *it );
    ++it;
    }
}

template< typename TInputMesh, typename TOutputMesh >
void
EdgeCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::GenerateOutputCells()
{
  //1. Get input and output mesh
  const InputMeshType * input = this->GetInput();
  OutputMeshType      * output = this->GetOutput();

  //2. Get cells container from input mesh
  const InputCellsContainer * cells = input->GetCells();

  //3.copy cell and dividing cell
 // typename InputCellsContainer::ConstPointer cells = input->GetCells();
  for ( InputCellsContainerConstIterator cellIt = cells->Begin();
        cellIt != cells->End();
        ++cellIt )
    {
    InputCellType* cell = cellIt->Value();

    if ( cell->GetType() != InputCellType::POLYGON_CELL || cell->GetNumberOfPoints() != 3 )
      {
      continue;
      }

    InputPointIdentifier  inputPointIdArray[3];
    OutputPointIdentifier oldPointIdArray[3];
    OutputPointIdentifier newPointIdArray[3];

    InputPointIdIterator it = cell->PointIdsBegin();
    unsigned int          n = 0;

    while ( it != cell->PointIdsEnd() )
      {
      inputPointIdArray[n] = *it;
      oldPointIdArray[n] = static_cast< OutputPointIdentifier >( inputPointIdArray[n] );
      ++it;
      ++n;
      }

    unsigned int splitEdges[3];
    n = 0;
    InputQEType *edge;

    for ( unsigned int ii = 0; ii < 3; ++ii )
      {
      unsigned int jj = ( ii + 1 ) % 3;

      edge = input->FindEdge(inputPointIdArray[ii], inputPointIdArray[jj]);

      if ( this->m_EdgesPointIdentifier->IndexExists(edge) )
        {
        newPointIdArray[ii] = this->m_EdgesPointIdentifier->GetElement( edge );
        splitEdges[n] = ii ;
        ++n;
        }
      }

    if( n == 0 )
      {
      // this face has no subdivided face as neighbor, copy it
      output->AddFaceTriangle(oldPointIdArray[0], oldPointIdArray[1], oldPointIdArray[2]);
      }
    else if( n == 1 )
      {
      unsigned int ii = splitEdges[0];
      unsigned int jj = ( ii + 1 ) % 3;
      unsigned int kk = ( ii + 2 ) % 3;

      output->AddFaceTriangle( newPointIdArray[ii], oldPointIdArray[jj], oldPointIdArray[kk] );
      output->AddFaceTriangle( newPointIdArray[ii], oldPointIdArray[kk], oldPointIdArray[ii] );
      }
    else if( n == 2 )
      {
      unsigned int ii = splitEdges[0];
      unsigned int jj = splitEdges[1];

      if( ii == 0 && jj == 1 )
        {
        // ii = 0, jj = 1
        output->AddFaceTriangle( oldPointIdArray[2], oldPointIdArray[0], newPointIdArray[0] );
        output->AddFaceTriangle( oldPointIdArray[2], newPointIdArray[0], newPointIdArray[1] );
        output->AddFaceTriangle( newPointIdArray[0], oldPointIdArray[1], newPointIdArray[1] );
        }
      else if( ii == 0 && jj == 2 )
        {
        // ii = 0, jj = 2
        output->AddFaceTriangle( oldPointIdArray[1], oldPointIdArray[2], newPointIdArray[0] );
        output->AddFaceTriangle( oldPointIdArray[2], newPointIdArray[2], newPointIdArray[0] );
        output->AddFaceTriangle( newPointIdArray[2], oldPointIdArray[0], newPointIdArray[0] );
        }
      else if( ii == 1 && jj == 2 )
        {
        // ii = 1, jj = 2
        output->AddFaceTriangle( oldPointIdArray[0], oldPointIdArray[1], newPointIdArray[1] );
        output->AddFaceTriangle( oldPointIdArray[0], newPointIdArray[1], newPointIdArray[2] );
        output->AddFaceTriangle( newPointIdArray[1], oldPointIdArray[2], newPointIdArray[2] );
        }
      }
    else if( n == 3 )
      {
      // this face was not supposed to be subdivided but all neighbors are
      output->AddFaceTriangle(oldPointIdArray[0], newPointIdArray[0], newPointIdArray[2]);
      output->AddFaceTriangle(newPointIdArray[0], oldPointIdArray[1], newPointIdArray[1]);
      output->AddFaceTriangle(newPointIdArray[1], oldPointIdArray[2], newPointIdArray[2]);
      output->AddFaceTriangle(newPointIdArray[0], newPointIdArray[1], newPointIdArray[2]);
      }
    }
}

template< typename TInputMesh, typename TOutputMesh >
void
EdgeCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::PrintSelf( std::ostream & os, Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}
}
#endif
