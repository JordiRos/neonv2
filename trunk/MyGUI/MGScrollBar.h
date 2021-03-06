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

#ifndef _MGSCROLLBAR_H
#define _MGSCROLLBAR_H

#include "MGControl.h"

class CMGScrollBar : public CMGControl
{
  public:

                      CMGScrollBar(CMGControl *pParent, bool bVertical, boost::function0<void> fOnChange);
                      CMGScrollBar(CMGControl *pParent, bool bVertical, int iX, int iY, int iSize, boost::function0<void> fOnChange);
    virtual          ~CMGScrollBar();

    virtual void      OnMouseMove (int iX, int iY);
    virtual void      OnMouseDown (int iX, int iY, int iButton);
    virtual void      OnMouseUp   (int iX, int iY, int iButton);

    float             GetPos      () const    { return m_Pos; }
    void              SetPos      (float Pos) { m_Pos = Pos; Update(); }

  protected:

    virtual void      DerivDraw   ();

  private:

            void      Update      ();
            void      UpdateFromTB(int Pos);
            int       GetPosRealTB() const;

  private:

    enum
    {
      NONE        = 0,
      LEFTTOP     = 1,
      RIGHTBOTTOM = 2,
      TRACKBAR    = 3,
    };
    bool              m_Vertical;
    float             m_Pos;
    unsigned          m_Pressed;
    boost::function0<void> m_OnChange;
};

#endif
