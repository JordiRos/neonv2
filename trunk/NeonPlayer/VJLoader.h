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

#ifndef VJLOADER_H
#define VJLOADER_H

class CDisplayDevice;

class CVJLoader
{
  public:
               CVJLoader  () { }
              ~CVJLoader  () { End(); }

    bool       Init       (int iMax);
    void       End        ();

    void       Draw       (CDisplayDevice *pDD);
    void       Next       ();

  private:

    int        m_iMax;
    int        m_iPos;
};

#endif