/*
 * Copyright (C) 2014 Romain "Creak" Failliot.
 *
 * This file is part of libbench.
 *
 * libbench is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libbench is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libbench. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOOL_DRAWCORE_H
#define TOOL_DRAWCORE_H

#include <vector>
#include <glibmm/ustring.h>
#include <cairomm/context.h>
#include <pangomm/layout.h>
#include <pangomm/fontdescription.h>
#include "drawthread.h"
#include "rectangle.h"

class DrawCore
{
public:
    DrawCore();
    ~DrawCore();

    void AddThread(DrawThread* thread);
    void Clear();
    void ComputeUiData(int top);
    double GetX() const;
    double GetY() const;
    double GetWidth() const;
    double GetHeight() const;
    void Draw(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
              double timeScale, bool odd, int wndWidth) const;

private:
    void DrawText(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
                  int rectangle_x, int rectangle_y, int rectangle_width, int rectangle_height) const;

public:
    Glib::ustring m_name;
    DrawThreadVector m_threads;

private:
    static int const k_marginLeft;
    static int const k_marginTop;

    Pango::FontDescription m_font;
    Rectangle m_siRect; //< Scale independant rect.
};

typedef std::vector<DrawCore*> DrawCoreVector;

#endif // TOOL_DRAWCORE_H
