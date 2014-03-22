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

#pragma once
#ifndef TOOL_DRAWTHREAD_H
#define TOOL_DRAWTHREAD_H

#include <vector>
#include <glibmm/ustring.h>
#include <pangomm/fontdescription.h>
#include "drawbench.h"
#include "rectangle.h"

class DrawThread
{
public:
    DrawThread();
    ~DrawThread();

    void AddBench(DrawBench* bench);
    void Clear();
    void ComputeUiData(int left, int top);
    double GetX() const;
    double GetY() const;
    double GetWidth() const;
    double GetHeight() const;
    void Draw(Cairo::RefPtr<Cairo::Context> const& cr,
              Glib::RefPtr<Pango::Layout>& layout,
              double timeScale) const;

private:
//    void DrawText(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
//                  int rectangle_x, int rectangle_y, int rectangle_width, int rectangle_height) const;

public:
    Glib::ustring m_name;
    DrawBenchVector m_benches;

private:
    Pango::FontDescription m_font;
    Rectangle m_siRect; //< Scale independant rect.
};

typedef std::vector<DrawThread*> DrawThreadVector;

#endif // TOOL_DRAWTHREAD_H
