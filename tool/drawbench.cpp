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
#include <boost/crc.hpp>
#include <cairomm/context.h>
#include <pangomm/layout.h>
#include "drawbench.h"

int const DrawBench::k_benchHeight = 20;

DrawBench::~DrawBench()
{
    Clear();
}

void DrawBench::AddBench(DrawBench* bench)
{
    assert(bench);
    m_children.push_back(bench);
}

void DrawBench::Clear()
{
    for(DrawBench* drawBench : m_children)
    {
        delete drawBench;
    }

    m_children.clear();
}

void DrawBench::ComputeUiData(int left, int top)
{
    ComputeDepth();
    ComputeDimension(left, top);
}

void DrawBench::ComputeDepth()
{
    m_depth = 1;
    for(DrawBench* drawBench : m_children)
    {
        drawBench->ComputeDepth();
        m_depth = std::max(m_depth, 1 + drawBench->m_depth);
    }
}

void DrawBench::ComputeDimension(int left, int top)
{
    for(DrawBench* drawBench : m_children)
    {
        drawBench->ComputeDimension(left, top + k_benchHeight);
    }

    m_siRect.SetX(m_startTime);
    m_siRect.SetY(top);
    m_siRect.SetWidth(m_stopTime - m_startTime);
    m_siRect.SetHeight(k_benchHeight);
}

double DrawBench::GetX() const
{
    return m_siRect.GetX();
}

double DrawBench::GetY() const
{
    return m_siRect.GetY();
}

double DrawBench::GetWidth() const
{
    return m_siRect.GetWidth();
}

double DrawBench::GetHeight() const
{
    return m_siRect.GetHeight();
}

double DrawBench::GetWholeHeight() const
{
    return m_depth * k_benchHeight;
}

void DrawBench::Draw(Cairo::RefPtr<Cairo::Context> const& cr,
                     Glib::RefPtr<Pango::Layout>& layout,
                     double timeScale) const
{
    double clipLeft;
    double clipTop;
    double clipRight;
    double clipBottom;
    cr->get_clip_extents(clipLeft, clipTop, clipRight, clipBottom);
    double benchLeft = GetX() * timeScale;
    double benchWidth = GetWidth() * timeScale;
    bool inClip = !(benchLeft > clipRight || benchLeft + benchWidth < clipLeft)
               && !(GetY() > clipBottom || GetY() + GetHeight() < clipTop);
    if(benchWidth >= 1.0 && inClip)
    {
        cr->save();

        cr->rectangle(benchLeft, GetY(), benchWidth, GetHeight());
        cr->set_line_width(1.0);

        boost::crc_32_type crc;
        crc.process_bytes(m_name.c_str(), m_name.length());
        uint32_t id = crc.checksum();

        cr->set_source_rgb(((id >> 0) & 0xff) / 255.0, ((id >> 8) & 0xff) / 255.0, ((id >> 16) & 0xff) / 255.0);
        cr->fill_preserve();
        cr->set_source_rgb(0.0, 0.0, 0.0);
        cr->stroke_preserve();
        if(benchWidth > 2.0)
            DrawText(cr, layout, benchLeft, GetY(), benchWidth, GetHeight());

        cr->restore();

        for(DrawBench const* childBench : m_children)
        {
            childBench->Draw(cr, layout, timeScale);
        }
    }
}

void DrawBench::DrawText(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
                         int rect_x, int rect_y, int rect_width, int rect_height) const
{
    // Get the text dimensions.
    int text_width;
    int text_height;
    layout->set_text(m_name);
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle.
    cr->save();
    cr->clip();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(rect_x + (rect_width - text_width) / 2, rect_y + (rect_height - text_height) / 2);
    layout->show_in_cairo_context(cr);
    cr->reset_clip();
    cr->restore();
}
