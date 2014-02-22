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

#include <iostream>
#include <limits>
#include <algorithm>
#include <cairomm/context.h>
#include "benchmarkarea.h"
#include "bench/thread.h"

namespace
{
    double const kPi = 3.14159265359;
    double const kPiBy2 = kPi / 2;
}

BenchMarkArea::BenchMarkArea()
    : m_document(nullptr)
    , m_timeToPx(500.0) // 5ms -> 1px
{
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html

    // Set core font layout.
    Pango::FontDescription coreFont;
    coreFont.set_family("DejaVu Sans");
    coreFont.set_weight(Pango::WEIGHT_BOLD);
    coreFont.set_size(20 * PANGO_SCALE);
    coreFont.set_stretch(Pango::STRETCH_CONDENSED);
    m_coreLayout = create_pango_layout("");
    m_coreLayout->set_font_description(coreFont);

    // Set thread font layout.
    Pango::FontDescription threadFont;
    threadFont.set_family("DejaVu Sans");
    threadFont.set_weight(Pango::WEIGHT_NORMAL);
    threadFont.set_size(10 * PANGO_SCALE);
    threadFont.set_stretch(Pango::STRETCH_CONDENSED);
    m_threadLayout = create_pango_layout("");
    m_threadLayout->set_font_description(threadFont);

    add_events(Gdk::SCROLL_MASK);

    std::cout << "init" << std::endl;
}

void BenchMarkArea::SetBenchMark(bench::Document const* document)
{
    std::cout << "bench" << std::endl;

    m_drawCores.clear();
    m_document = document;
    if(!m_document)
        return;

    // Fill bench lists.
    m_drawCores.reserve(m_document->m_cores.size());
    for(bench::DocumentCore const& docCore : m_document->m_cores)
    {
        std::cout << "core" << std::endl;

        m_drawCores.push_back(DrawCore());
        DrawCore& drawCore = m_drawCores.back();
        drawCore.m_name = docCore.m_name;

        for(bench::DocumentThread const& docThread : docCore.m_threads)
        {
            std::cout << "thread" << std::endl;

            for(bench::DocumentBench const& docBench : docThread.m_benches)
            {
                drawCore.m_benches.push_back(DrawBench());
                DrawBench& drawBench = drawCore.m_benches.back();
                drawBench.m_parent = nullptr;
                drawBench.m_name = docBench.m_name;
                drawBench.m_startTime = docBench.m_start;
                drawBench.m_stopTime = docBench.m_stop;
            }
        }
    }
}

bool BenchMarkArea::on_draw(Cairo::RefPtr<Cairo::Context> const& cr)
{
    if(!m_document)
        return false;

    cr->save();
    cr->set_antialias(Cairo::ANTIALIAS_NONE);

    // Draw cores' background.
    Gtk::Allocation allocation = get_allocation();
    double coreY = 0.0;
    for(std::vector<DrawCore>::const_iterator cit = m_drawCores.begin(); cit != m_drawCores.end(); ++cit)
    {
        DrawCore const& drawCore = *cit;

        double const coreHeight = std::max(5.0 + drawCore.m_benches.size() * 25.0, 150.0);
        double const coreWidth = static_cast<double>(allocation.get_width());
        double const leftMargin = 50.0;
        double const threadHeight = 20.0;
        double const threadBottomMargin = 5.0;
        bool const odd = (cit - m_drawCores.begin()) & 1;

        if(odd)
            cr->set_source_rgb(0.5, 0.5, 0.5);
        else
            cr->set_source_rgb(0.7, 0.7, 0.7);
        cr->rectangle(0.0, coreY, coreWidth, coreHeight);
        cr->fill();

        if(!odd)
            cr->set_source_rgb(0.5, 0.5, 0.5);
        else
            cr->set_source_rgb(0.7, 0.7, 0.7);
        DrawCoreText(cr, 0.0, coreY, coreWidth, coreHeight, drawCore.m_name);

        double threadY = 5.0;
        for(std::vector<DrawBench>::const_iterator bit = drawCore.m_benches.begin(); bit != drawCore.m_benches.end(); ++bit)
        {
            DrawBench const& bench = *bit;

            double benchLeft = leftMargin + bench.m_startTime * m_timeToPx;
            double benchTop = coreY + threadY;
            double benchWidth = (bench.m_stopTime - bench.m_startTime) * m_timeToPx;

            if(benchWidth >= 1.0 && cr->in_clip(benchLeft, benchTop))
            {
                cr->save();

                cr->rectangle(benchLeft, benchTop, benchWidth, 20.0);
                cr->set_line_width(1.0);
                cr->set_source_rgb(1.0, 0.8, 0.5);
                cr->fill_preserve();
                cr->set_source_rgb(0.0, 0.0, 0.0);
                cr->stroke_preserve();
                if(benchWidth > 2.0)
                    DrawThreadName(cr, benchLeft, benchTop, benchWidth, 20.0, bench.m_name);

                cr->restore();
            }

            threadY += threadHeight + threadBottomMargin;
        }

        coreY += coreHeight;
    }

    cr->restore();
    return true;
}

bool BenchMarkArea::on_scroll_event(GdkEventScroll* event)
{
    switch(event->direction)
    {
    case GDK_SCROLL_UP:
        m_timeToPx += 100.0;
        if(m_timeToPx > 20000.0)
            m_timeToPx = 20000.0;
        queue_draw();
        break;
    case GDK_SCROLL_DOWN:
        m_timeToPx -= 100.0;
        if(m_timeToPx < 100.0)
            m_timeToPx = 100.0;
        queue_draw();
        break;
        
    default:
        break;
    }

    return true;
}

void BenchMarkArea::DrawCoreText(Cairo::RefPtr<Cairo::Context> const& cr,
                                 int rectangle_x, int rectangle_y,
                                 int rectangle_width, int rectangle_height,
                                 Glib::ustring const& text)
{
    // Get the text dimensions.
    int text_width;
    int text_height;
    m_coreLayout->set_text(text);
    m_coreLayout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->save();
    cr->move_to(0.0, rectangle_y + (rectangle_height + text_width) / 2);
    cr->rotate(-kPiBy2);
    m_coreLayout->show_in_cairo_context(cr);
    cr->restore();
}

void BenchMarkArea::DrawThreadName(Cairo::RefPtr<Cairo::Context> const& cr,
                                   int rectangle_x, int rectangle_y,
                                   int rectangle_width, int rectangle_height,
                                   Glib::ustring const& text)
{
    // Get the text dimensions (it updates the variables -- by reference)
    int text_width;
    int text_height;
    m_threadLayout->set_text(text);
    m_threadLayout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->save();
    cr->clip();
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(rectangle_x + (rectangle_width - text_width) / 2, rectangle_y + (rectangle_height - text_height) / 2);
    m_threadLayout->show_in_cairo_context(cr);
    cr->reset_clip();
    cr->restore();
}

