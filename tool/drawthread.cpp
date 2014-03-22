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

#include <assert.h>
#include <limits>
#include <pangomm/layout.h>
#include "drawthread.h"

DrawThread::DrawThread()
{
    m_font.set_family("DejaVu Sans");
    m_font.set_weight(Pango::WEIGHT_NORMAL);
    m_font.set_size(10 * PANGO_SCALE);
    m_font.set_stretch(Pango::STRETCH_CONDENSED);
}

DrawThread::~DrawThread()
{
    Clear();
}

void DrawThread::AddBench(DrawBench* bench)
{
    assert(bench);
    m_benches.push_back(bench);
}

void DrawThread::Clear()
{
    for(DrawBench* core : m_benches)
    {
        delete core;
    }

    m_benches.clear();
}

double DrawThread::GetX() const
{
    return m_siRect.GetX();
}

double DrawThread::GetY() const
{
    return m_siRect.GetY();
}

double DrawThread::GetWidth() const
{
    return m_siRect.GetWidth();
}

double DrawThread::GetHeight() const
{
    return m_siRect.GetHeight();
}

void DrawThread::ComputeUiData(int left, int top)
{
    double xMin = std::numeric_limits<double>::max();
    double xMax = std::numeric_limits<double>::min();
    double height = 0.0;
    for(DrawBench* drawBench : m_benches)
    {
        drawBench->ComputeUiData(left, top);
        xMin = std::min(xMin, drawBench->GetX());
        xMax = std::max(xMax, drawBench->GetX() + drawBench->GetWidth());
        height = std::max(height, drawBench->GetWholeHeight());
    }

    m_siRect.SetX(xMin);
    m_siRect.SetY(top);
    m_siRect.SetWidth(xMax - xMin);
    m_siRect.SetHeight(height);
}

void DrawThread::Draw(Cairo::RefPtr<Cairo::Context> const& cr,
                      Glib::RefPtr<Pango::Layout>& layout,
                      double timeScale) const
{
    // Set layout font.
    layout->set_font_description(m_font);

    for(DrawBench const* bench : m_benches)
    {
        bench->Draw(cr, layout, timeScale);
    }
}

//void DrawThread::DrawText(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
//                          int rectangle_x, int rectangle_y, int rectangle_width, int rectangle_height) const
//{
//    // Get the text dimensions (it updates the variables -- by reference)
//    int text_width;
//    int text_height;
//    layout->set_text(m_name);
//    layout->get_pixel_size(text_width, text_height);

//    // Position the text in the middle
//    cr->save();
//    cr->clip();
//    cr->set_source_rgb(0.0, 0.0, 0.0);
//    cr->move_to(rectangle_x + (rectangle_width - text_width) / 2, rectangle_y + (rectangle_height - text_height) / 2);
//    layout->show_in_cairo_context(cr);
//    cr->reset_clip();
//    cr->restore();
//}
