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

#ifndef __itkConditionalEdgeCellSubdivisionQuadEdgeMeshFilter_hxx
#define __itkConditionalEdgeCellSubdivisionQuadEdgeMeshFilter_hxx

#include "itkConditionalEdgeCellSubdivisionQuadEdgeMeshFilter.h"

namespace itk
{
template< typename TInputMesh, typename TEdgeCellSubdivisionFilter, typename TCriterion >
ConditionalEdgeCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TEdgeCellSubdivisionFilter, TCriterion >
::ConditionalEdgeCellSubdivisionQuadEdgeMeshFilter()
{
  m_EdgeCellSubdivisionFilter = EdgeCellSubdivisionFilterType::New();
}

template< typename TInputMesh, typename TEdgeCellSubdivisionFilter, typename TCriterion >
void
ConditionalEdgeCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TEdgeCellSubdivisionFilter, TCriterion >
::GenerateData()
{
  this->CopyInputMeshToOutputMeshGeometry();
  this->m_SubdivisionCriterion->Compute( this->GetOutput(), this->m_EdgesToBeSubdivided );
  while( !this->m_EdgesToBeSubdivided.empty() )
    {
    this->m_EdgeCellSubdivisionFilter->SetInput( this->GetOutput() );
    this->m_EdgeCellSubdivisionFilter->SetEdgesToBeSubdivided( this->m_EdgesToBeSubdivided );
    this->m_EdgeCellSubdivisionFilter->Update();
    //way 1. graph the output from this->m_EdgeCellSubdivisionFilter
    OutputMeshPointer mesh = this->m_EdgeCellSubdivisionFilter->GetOutput();
    mesh->DisconnectPipeline();
    //way 2. copy the output from this->m_EdgeCellSubdivisionFilter
    /*
    OutputMeshPointer mesh = OutputMeshType::New();
    CopyMeshToMeshPoints( this->m_EdgeCellSubdivisionFilter->GetOutput(), mesh.GetPointer() );
    CopyMeshToMeshEdgeCells( this->m_EdgeCellSubdivisionFilter->GetOutput(), mesh.GetPointer() );
    //Here we must inove CopyMeshToMeshEdgeCells before invoke CopyMeshTOMeshCells()
    //because that the latter call out->AddFaceWithSecurePointList(points, false);
    //with the false, it will not create a new edge.
    CopyMeshToMeshCells( this->m_EdgeCellSubdivisionFilter->GetOutput(), mesh.GetPointer() );
    //SetOutput() was replaced by GraftOutput()
//    this->SetOutput(mesh);
//    this->GetOutput()->Graft(mesh);
    */

    this->GraftOutput(mesh);
    this->m_SubdivisionCriterion->Compute( this->GetOutput(), this->m_EdgesToBeSubdivided );
    }
}

template< typename TInputMesh, typename TEdgeCellSubdivisionFilter, typename TCriterion >
void
ConditionalEdgeCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TEdgeCellSubdivisionFilter, TCriterion >
::PrintSelf( std::ostream & os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}
}
#endif
