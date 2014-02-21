/*
 * benchmarkarea.cpp
 *
 *  Created on: 2013-10-13
 *      Author: creak
 */

#include <iostream>
#include <limits>
#include <algorithm>
#include <cairomm/context.h>
#include "benchmarkarea.h"
#include "bench/benchmark.h"
#include "bench/thread.h"

namespace
{
    double const kPi = 3.14159265359;
    double const kPiBy2 = kPi / 2;
}

BenchMarkArea::BenchMarkArea()
	: m_benchmark(nullptr)
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
}

void BenchMarkArea::SetBenchMark(libbench::BenchMark const* benchmark)
{
    m_drawCores.clear();
	m_benchmark = benchmark;
	if(!m_benchmark)
	    return;

    // Get minimum start time.
    double globalMinTime = std::numeric_limits<double>::max();
    libbench::ThreadVector const& threads = m_benchmark->GetThreadVector();
    for(libbench::ThreadVector::const_iterator tit = threads.begin(); tit != threads.end(); ++tit)
    {
        libbench::Thread const* thread = *tit;
        libbench::BenchList const& benchList = thread->GetBenchList();
        libbench::BenchVector const& benches = benchList.GetBenches();
        for(libbench::BenchVector::const_iterator bit = benches.begin(); bit != benches.end(); ++bit)
        {
            libbench::Bench const& bench = *bit;
            globalMinTime = std::min(globalMinTime, bench.m_start.tv_sec + bench.m_start.tv_nsec * 1e-9);
        }
    }

    // Fill bench lists.
    unsigned int numCores = m_benchmark->GetNumberOfCores();
    m_drawCores.resize(numCores);
    for(int u = 0; u < static_cast<int>(numCores); ++u)
    {
        DrawCore& drawCore = m_drawCores[u];
        drawCore.m_name = m_benchmark->GetCoreName(u);

        std::vector<libbench::Thread const*> coreThreads;
        coreThreads.reserve(threads.size());
        for(libbench::ThreadVector::const_iterator tit = threads.begin(); tit != threads.end(); ++tit)
        {
            libbench::Thread const* thread = *tit;
            if(thread->GetCoreId() == u)
                coreThreads.push_back(thread);
        }
        
        for(std::vector<libbench::Thread const*>::const_iterator tit = coreThreads.begin(); tit != coreThreads.end(); ++tit)
        {
            libbench::Thread const* thread = *tit;

            libbench::BenchList const& benchList = thread->GetBenchList();
            libbench::BenchVector const& benches = benchList.GetBenches();
            for(libbench::BenchVector::const_iterator bit = benches.begin(); bit != benches.end(); ++bit)
            {
                libbench::Bench const& bench = *bit;
                if(bench.m_parent != -1)
                    continue;

                DrawBench drawBench;
                drawBench.m_parent = nullptr;
                drawBench.m_startTime = (bench.m_start.tv_sec + bench.m_start.tv_nsec * 1e-9) - globalMinTime;
                drawBench.m_stopTime = (bench.m_stop.tv_sec + bench.m_stop.tv_nsec * 1e-9) - globalMinTime;
                drawBench.m_name = benchList.GetBenchName(bench.m_nameId);
                drawCore.m_benches.push_back(drawBench);
            }
        }
    }
}

bool BenchMarkArea::on_draw(Cairo::RefPtr<Cairo::Context> const& cr)
{
	if(!m_benchmark)
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

