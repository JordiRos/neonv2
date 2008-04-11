//---------------------------------------------------------------------------//
// Neon v2.5
// Copyright (C) 2006,2008 Jordi Ros <shine.3p@gmail.com>
// www.neonv2.com / www.xplsv.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program called license.txt
// If not, see <http://www.gnu.org/licenses/>
//
//---------------------------------------------------------------------------//

#ifndef _MGGRIDPANEL_H
#define _MGGRIDPANEL_H

#include "MGControl.h"
#include "MGGrid.h"
#include "MGGridItemFX.h"

typedef CMGGrid<CMGGridItemFX> CMGGridFX;
class   CMGPageControl;

class CMGGridPanel : public CMGControl
{
  public:
         
                         CMGGridPanel    (CMGControl *pParent);

            void         Clear           ();
            void         Update          ();

            CMGGridFX   *GetMasterGrid   ()          { return m_GridMasterFX;  }
            CMGGridFX   *GetGrid         (int iGrid) { return m_GridFX[iGrid]; }
            void         SetActiveGrid   (int iGrid);

  private:

            void         FXUnload        (CMGEffect *pEffect);

            void         OnSelectGridPage();
            void         OnGridMouseDown (CMGGridFX *pGrid, int iX, int iY, int iButton);
            void         OnGridDragOver  (CMGGridFX *pGrid, CMGControl *pSource, int iX, int iY, bool &Accept);
            void         OnGridDragDrop  (CMGGridFX *pGrid, CMGControl *pSource, int iX, int iY);
            void         OnGridKeyDown   (CMGGridFX *pGrid, int vkey);
  
  private:

    CMGPageControl      *m_PCGrid;
    CMGGridFX           *m_GridMasterFX;
    CMGGridFX           *m_GridFX[10];
    CMGGridFX           *m_GridFXSelected;
};


#endif
