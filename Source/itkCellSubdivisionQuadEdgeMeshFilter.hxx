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

#ifndef __itkCellSubdivisionQuadEdgeMeshFilter_hxx
#define __itkCellSubdivisionQuadEdgeMeshFilter_hxx

#include "itkCellSubdivisionQuadEdgeMeshFilter.h"

namespace itk
{
template< typename TInputMesh, typename TOutputMesh >
CellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::CellSubdivisionQuadEdgeMeshFilter()
{
//  this->m_EdgesPointIdentifier = EdgePointIdentifierContainer::New();
  this->m_Uniform = true;
}

/*
template< typename TInputMesh, typename TOutputMesh >
void
CellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::GenerateData()
{
  this->m_Uniform = this->m_CellsToBeSubdivided.empty();

  this->GenerateOutputPoints();

  this->GenerateOutputCells();

}
*/
template< typename TInputMesh, typename TOutputMesh >
void
CellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::GenerateOutputPoints()
{
  this->CopyInputMeshToOutputMeshPoints();

  this->m_EdgesPointIdentifier->Initialize();

  this->m_Uniform = this->m_CellsToBeSubdivided.empty();

  const InputCellsContainer * cells = this->GetInput()->GetCells();

  if( this->m_Uniform )
    {
    for ( InputCellsContainerConstIterator cellIt = cells->Begin(); cellIt != cells->End(); ++cellIt )
      {
      this->AddNewPoints( cellIt->Value() );
      }
    }
  else
    {
    OutputCellIdentifierListConstIterator it  = this->m_CellsToBeSubdivided.begin();
    OutputCellIdentifierListConstIterator end = this->m_CellsToBeSubdivided.end();
    while( it != end )
      {
      InputCellType* cell = cells->GetElement( static_cast<InputCellIdentifier>( *it ) );
      this->AddNewPoints( cell );
      ++it;
      }
    }
}

template< typename TInputMesh, typename TOutputMesh >
void
CellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::GenerateOutputCells()
{
  InputMeshConstPointer input  = this->GetInput();
  OutputMeshPointer     output = this->GetOutput();

  this->m_CellsToBeSubdivided.clear();

  const InputCellsContainer * cells = input->GetCells();

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
    OutputPointIdentifier trianglePointIdArray[3];
    OutputPointIdentifier edgePointIdArray[3];

    InputPointIdIterator it = cell->PointIdsBegin();
    unsigned int          n = 0;

    while ( it != cell->PointIdsEnd() )
      {
      inputPointIdArray[n] = *it;
      trianglePointIdArray[n] = static_cast< OutputPointIdentifier >( inputPointIdArray[n] );
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

      if ( this->m_EdgesPointIdentifier->IndexExists( edge ) )
        {
        edgePointIdArray[ii] = this->m_EdgesPointIdentifier->GetElement( edge );
        splitEdges[n] = ii;
        ++n;
        }
      }

    if( n == 0 )
      {
      // this face has no subdivided face as neighbor, copy it
      output->AddFaceTriangle(trianglePointIdArray[0], trianglePointIdArray[1], trianglePointIdArray[2]);
      }
    else if( n == 1 )
      {
      unsigned int ii = splitEdges[0];
      unsigned int jj = ( ii + 1 ) % 3;
      unsigned int kk = ( ii + 2 ) % 3;

      output->AddFaceTriangle( edgePointIdArray[ii], trianglePointIdArray[jj], trianglePointIdArray[kk] );
      output->AddFaceTriangle( edgePointIdArray[ii], trianglePointIdArray[kk], trianglePointIdArray[ii] );
      }
    else if( n == 2 )
      {
      unsigned int ii = splitEdges[0];
      unsigned int jj = splitEdges[1];

      if( ii == 0 && jj == 1 )
        {
        // ii = 0, jj = 1
        output->AddFaceTriangle( trianglePointIdArray[2], trianglePointIdArray[0], edgePointIdArray[0] );
        output->AddFaceTriangle( trianglePointIdArray[2], edgePointIdArray[0], edgePointIdArray[1] );
        output->AddFaceTriangle( edgePointIdArray[0], trianglePointIdArray[1], edgePointIdArray[1] );
        }
      else if( ii == 0 && jj == 2 )
        {
        // ii = 0, jj = 2
        output->AddFaceTriangle( trianglePointIdArray[1], trianglePointIdArray[2], edgePointIdArray[0] );
        output->AddFaceTriangle( trianglePointIdArray[2], edgePointIdArray[2], edgePointIdArray[0] );
        output->AddFaceTriangle( edgePointIdArray[2], trianglePointIdArray[0], edgePointIdArray[0] );
        }
      else if( ii == 1 && jj == 2 )
        {
        // ii = 1, jj = 2
        output->AddFaceTriangle( trianglePointIdArray[0], trianglePointIdArray[1], edgePointIdArray[1] );
        output->AddFaceTriangle( trianglePointIdArray[0], edgePointIdArray[1], edgePointIdArray[2] );
        output->AddFaceTriangle( edgePointIdArray[1], trianglePointIdArray[2], edgePointIdArray[2] );
        }
      }
    else if( n == 3 )
      {
      // this face was not supposed to be subdivided but all neighbors are
      if( this->m_Uniform )
        {
        output->AddFaceTriangle( trianglePointIdArray[0], edgePointIdArray[0], edgePointIdArray[2] );
        output->AddFaceTriangle( edgePointIdArray[0], trianglePointIdArray[1], edgePointIdArray[1] );
        output->AddFaceTriangle( edgePointIdArray[1], trianglePointIdArray[2], edgePointIdArray[2] );
        output->AddFaceTriangle( edgePointIdArray[0], edgePointIdArray[1], edgePointIdArray[2] );
        }
      else
        {
        this->m_CellsToBeSubdivided.push_back( output->AddFaceTriangle( trianglePointIdArray[0], edgePointIdArray[0], edgePointIdArray[2] )->GetLeft() );
        this->m_CellsToBeSubdivided.push_back( output->AddFaceTriangle( edgePointIdArray[0], trianglePointIdArray[1], edgePointIdArray[1] )->GetLeft() );
        this->m_CellsToBeSubdivided.push_back( output->AddFaceTriangle( edgePointIdArray[1], trianglePointIdArray[2], edgePointIdArray[2] )->GetLeft() );
        this->m_CellsToBeSubdivided.push_back( output->AddFaceTriangle( edgePointIdArray[0], edgePointIdArray[1], edgePointIdArray[2] )->GetLeft() );
        }
      }
    }
}

template< typename TInputMesh, typename TOutputMesh >
void
CellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::PrintSelf( std::ostream & os, Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}
}
#endif
