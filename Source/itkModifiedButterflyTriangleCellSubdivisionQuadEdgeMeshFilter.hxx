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
#ifndef __itkModifiedButterflyTriangleCellSubdivisionQuadEdgeMeshFilter_hxx
#define __itkModifiedButterflyTriangleCellSubdivisionQuadEdgeMeshFilter_hxx

#include "itkModifiedButterflyTriangleCellSubdivisionQuadEdgeMeshFilter.h"

namespace itk
{
template< typename TInputMesh, typename TOutputMesh >
void
ModifiedButterflyTriangleCellSubdivisionQuadEdgeMeshFilter< TInputMesh, TOutputMesh >
::AddNewPoints( InputCellType *cell )
{
  if ( cell->GetType() != InputCellType::POLYGON_CELL || cell->GetNumberOfPoints() != 3 )
  {
  itkExceptionMacro(<<" The input cell is not a triangle cell");
  }

  const InputMeshType * input = this->GetInput();
  OutputMeshType * output = this->GetOutput();

  InputPointIdentifier n = 0;
  InputPointIdentifier pointIdArray[3];

  InputCoordType pointWeight[8] = {0.5, 0.5, 0.125, 0.125, -0.0625, -0.0625, -0.0625, -0.0625};
  InputPointType pointArray[8];

  OutputPointIdentifier numberOfPoints = output->GetNumberOfPoints();

  InputPointIdIterator it = cell->PointIdsBegin();
  while ( it != cell->PointIdsEnd() )
    {
    pointIdArray[n++] = *it;
    ++it;
    }

  for ( unsigned int ii = 0; ii < 3; ++ii )
    {
    unsigned int jj = ( ii + 1 ) % 3;

    InputQEType *edge = input->FindEdge(pointIdArray[ii], pointIdArray[jj]);

    if ( !this->m_EdgesPointIdentifier->IndexExists( edge ) )
      {
      InputPointType newPoint;
      newPoint.Fill(NumericTraits< typename InputPointType::ValueType >::Zero);

      input->GetPoint(pointIdArray[ii], &pointArray[0]);
      input->GetPoint(pointIdArray[jj], &pointArray[1]);

      if ( edge->GetLnext() )
        {
        input->GetPoint(edge->GetLnext()->GetDestination(), &pointArray[2]);

        if ( edge->GetLnext()->GetRprev() )
          {
          input->GetPoint(edge->GetLnext()->GetRprev()->GetDestination(), &pointArray[4]);
          }
        else
          {
          pointArray[4].Fill(NumericTraits< typename InputPointType::ValueType >::Zero);
          }
        }
      else
        {
        pointArray[2].Fill(NumericTraits< typename InputPointType::ValueType >::Zero);
        }

      if ( edge->GetRprev() )
        {
        input->GetPoint(edge->GetRprev()->GetDestination(), &pointArray[3]);
        if ( edge->GetRprev()->GetLnext() )
          {
          input->GetPoint(edge->GetRprev()->GetLnext()->GetDestination(), &pointArray[5]);
          }
        else
          {
          pointArray[5].Fill(NumericTraits< typename InputPointType::ValueType >::Zero);
          }
        }
      else
        {
        pointArray[3].Fill(NumericTraits< typename InputPointType::ValueType >::Zero);
        }

      if ( edge->GetLprev() && edge->GetLprev()->GetRprev() )
        {
        input->GetPoint( edge->GetLprev()->GetRprev()->GetDestination(), &pointArray[6] );
        }
      else
        {
        pointArray[6].Fill(NumericTraits< typename InputPointType::ValueType >::Zero);
        }

      if ( edge->GetRnext() && edge->GetRnext()->GetLnext() )
        {
        input->GetPoint(edge->GetRnext()->GetLnext()->GetDestination(), &pointArray[7]);
        }
      else
        {
        pointArray[7].Fill(NumericTraits< typename InputPointType::ValueType >::Zero);
        }

      for ( unsigned int kk = 0; kk < 3; ++kk )
        {
        for ( unsigned int mm = 0; mm < 8; ++mm )
          {
          newPoint[kk] += pointWeight[mm] * pointArray[mm][kk];
          }
        }

      this->m_EdgesPointIdentifier->InsertElement( edge, numberOfPoints );
      this->m_EdgesPointIdentifier->InsertElement( edge->GetSym(), numberOfPoints );

      OutputPointType outPoint;
      outPoint.CastFrom( newPoint );
      output->SetPoint( numberOfPoints, outPoint );
      ++numberOfPoints;
      }
    }
}
}
#endif
