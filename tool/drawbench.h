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
#ifndef TOOL_DRAWBENCH_H
#define TOOL_DRAWBENCH_H

#include <vector>
#include <glibmm/ustring.h>
#include <glibmm/refptr.h>
#include <cairomm/refptr.h>
#include "rectangle.h"

namespace Cairo
{
    class Context;
}

namespace Pango
{
    class Layout;
}

class DrawBench;
typedef std::vector<DrawBench*> DrawBenchVector;

class DrawBench
{
public:
    ~DrawBench();

    void AddBench(DrawBench* bench);
    void Clear();
    void ComputeUiData(int left, int top);
    double GetX() const;
    double GetY() const;
    double GetWidth() const;
    double GetHeight() const;
    double GetWholeHeight() const;
    void Draw(Cairo::RefPtr<Cairo::Context> const& cr,
              Glib::RefPtr<Pango::Layout>& layout,
              double timeScale) const;

private:
    void ComputeDepth();
    void ComputeDimension(int left, int top);
    void DrawText(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
                  int rect_x, int rect_y, int rect_width, int rect_height) const;

public:
    DrawBench* m_parent;
    double m_startTime;
    double m_stopTime;
    int m_depth;
    Glib::ustring m_name;
    DrawBenchVector m_children;

private:
    static int const k_benchHeight;

    Rectangle m_siRect; //< Scale independant rect.
};

#endif // TOOL_DRAWBENCH_H
