SubdivisionQuadEdgeMeshFilter2
==============================

Refactoring of  http://www.insight-journal.org/browse/publication/831

This version refactored the old SubdivisionQuadEdgeMeshFilter in the way as bellow:
1. Remove recursive subdivision from each subdivision class to a new class. That's greatly simplified the implementation.
2. Unified the interface. The procedure of subdivision of mesh was divided into two parts.
   (1). Adding new points.
   (2). Copy/Adding cells.
3. Add edge cell based subdivision classess.
