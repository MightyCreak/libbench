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
#include "drawcore.h"

namespace
{
    double const kPi = 3.14159265359;
    double const kPiBy2 = kPi / 2;
}

int const DrawCore::k_marginLeft = 50;
int const DrawCore::k_marginTop = 5;

DrawCore::DrawCore()
{
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
    m_font.set_family("DejaVu Sans");
    m_font.set_weight(Pango::WEIGHT_BOLD);
    m_font.set_size(20 * PANGO_SCALE);
    m_font.set_stretch(Pango::STRETCH_CONDENSED);
}

DrawCore::~DrawCore()
{
    Clear();
}

void DrawCore::AddThread(DrawThread* thread)
{
    assert(thread);
    m_threads.push_back(thread);
}

void DrawCore::Clear()
{
    for(DrawThread* thread : m_threads)
    {
        delete thread;
    }

    m_threads.clear();
}

void DrawCore::ComputeUiData(int top)
{
    double xMin = std::numeric_limits<double>::max();
    double xMax = std::numeric_limits<double>::min();
    double height = k_marginTop;
    for(DrawThread* drawThread : m_threads)
    {
        drawThread->ComputeUiData(k_marginLeft, top + height);
        xMin = std::min(xMin, drawThread->GetX());
        xMax = std::max(xMax, drawThread->GetX() + drawThread->GetWidth());
        height += drawThread->GetHeight() + k_marginTop;
    }

    m_siRect.SetX(0.0);
    m_siRect.SetY(top);
    m_siRect.SetWidth(xMax - xMin + 0.2);
    m_siRect.SetHeight(std::max(height, 150.0));
}

double DrawCore::GetX() const
{
    return m_siRect.GetX();
}

double DrawCore::GetY() const
{
    return m_siRect.GetY();
}

double DrawCore::GetWidth() const
{
    return m_siRect.GetWidth();
}

double DrawCore::GetHeight() const
{
    return m_siRect.GetHeight();
}

void DrawCore::Draw(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
                    double timeScale, bool odd, int wndWidth) const
{
    // Set layout font.
    layout->set_font_description(m_font);

    // Draw core background.
    if(odd)
        cr->set_source_rgb(0.5, 0.5, 0.5);
    else
        cr->set_source_rgb(0.7, 0.7, 0.7);
    cr->rectangle(GetX(), GetY(), wndWidth, GetHeight());
    cr->fill();

    // Draw core text.
    if(!odd)
        cr->set_source_rgb(0.5, 0.5, 0.5);
    else
        cr->set_source_rgb(0.7, 0.7, 0.7);
    DrawText(cr, layout, GetX(), GetY(), wndWidth, GetHeight());

    // Draw benches.
    for(DrawThread const* drawThread : m_threads)
    {
        drawThread->Draw(cr, layout, timeScale);
    }
}

void DrawCore::DrawText(Cairo::RefPtr<Cairo::Context> const& cr, Glib::RefPtr<Pango::Layout>& layout,
                        int rectangle_x, int rectangle_y, int rectangle_width, int rectangle_height) const
{
    // Get the text dimensions.
    int text_width;
    int text_height;
    layout->set_text(m_name);
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->save();
    cr->move_to(0.0, rectangle_y + (rectangle_height + text_width) / 2);
    cr->rotate(-kPiBy2);
    layout->show_in_cairo_context(cr);
    cr->restore();
}
