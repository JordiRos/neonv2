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

#ifndef _MGAPPWINDOW_H
#define _MGAPPWINDOW_H

class CMGApplication;

class IMGAppWindow
{
  public:

    static  IMGAppWindow *Create         (unsigned hInst, int iWidth, int iHeight);

    virtual unsigned      GetHDC         () const = 0;
    virtual unsigned      GetHWnd        () const = 0;
    virtual void          ProcessMessages() = 0;
    virtual void          Flip           () = 0;
};

#endif
