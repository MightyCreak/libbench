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

#include <cstdio>
#include "timeline.h"
#include "libbenchwindow.h"

TimeLine::TimeLine(LibbenchWindow* window)
    : m_mainWindow(window)
    , m_timeStart(0.0)
{
    m_layout = create_pango_layout("");
    m_font.set_family("DejaVu Sans");
    m_font.set_weight(Pango::WEIGHT_LIGHT);
    m_font.set_size(8 * PANGO_SCALE);
    m_font.set_stretch(Pango::STRETCH_NORMAL);

    set_size_request(-1, 25);
}

void TimeLine::SetTimeStart(double timeStart)
{
    m_timeStart = timeStart;
}

double TimeLine::GetTimeStart() const
{
    return m_timeStart;
}

bool TimeLine::on_draw(Cairo::RefPtr<Cairo::Context> const& cr)
{
    if(!m_mainWindow->IsFileOpened())
    {
        return false;
    }

    cr->save();
    cr->set_antialias(Cairo::ANTIALIAS_NONE);

    Gtk::Allocation allocation = get_allocation();

    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->rectangle(0.0, 0.0, allocation.get_width(), allocation.get_height());
    cr->fill();

    cr->set_line_width(1.0);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(0.0, 1.0);
    cr->line_to(allocation.get_width(), 1.0);
    cr->stroke();
    cr->move_to(0.0, allocation.get_height() - 1);
    cr->line_to(allocation.get_width(), allocation.get_height());
    cr->stroke();

    double floorStart = (floor(m_timeStart) - m_timeStart) * m_mainWindow->GetTimeScale();
    int numTicks = ceil(allocation.get_width() / m_mainWindow->GetTimeScale());
    for(int i = 0; i <= numTicks; ++i)
    {
        double x = floorStart + i * m_mainWindow->GetTimeScale();

        if(m_timeStart != 0.0 || i != 0)
        {
            // Get the text dimensions.
            int textWidth;
            int textHeight;
            char timeStr[10];
            sprintf(timeStr, "%d s", int(floor(m_timeStart) + i));
            m_layout->set_text(timeStr);
            m_layout->get_pixel_size(textWidth, textHeight);

            // Position the text in the middle
            cr->move_to(x - textWidth / 2, 0.0);
            m_layout->show_in_cairo_context(cr);

            // Draw tick.
            cr->move_to(x, allocation.get_height() - 8.0);
            cr->line_to(x, allocation.get_height());
            cr->stroke();
        }

        if(i < numTicks)
        {
            // Draw sub-tick.
            double nextX = floorStart + (i + 1) * m_mainWindow->GetTimeScale();
            double tickWidth = nextX - x;

            if(tickWidth > 200.0)
            {
                // Get the text dimensions.
                int textWidth;
                int textHeight;
                char timeStr[10];
                sprintf(timeStr, "%d.5 s", int(floor(m_timeStart) + i));
                m_layout->set_text(timeStr);
                m_layout->get_pixel_size(textWidth, textHeight);

                // Position the text in the middle
                cr->move_to(x + (tickWidth - textWidth) / 2, 0.0);
                m_layout->show_in_cairo_context(cr);
            }

            cr->move_to(x + tickWidth / 2, allocation.get_height() - 4.0);
            cr->line_to(x + tickWidth / 2, allocation.get_height());
            cr->stroke();
        }
    }

    cr->restore();

    return true;
}
