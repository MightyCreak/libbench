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
#include <iomanip>
#include <limits>
#include <algorithm>
#include <boost/crc.hpp>
#include "benchmarkarea.h"
#include "libbenchwindow.h"
#include "bench/thread.h"

BenchMarkArea::BenchMarkArea(LibbenchWindow* window)
    : m_mainWindow(window)
    , m_timeLength(0.0)
    , m_ctrlPressed(false)
{
    // Create layout.
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
    m_layout = create_pango_layout("");

    add_events(Gdk::SCROLL_MASK);
}

void BenchMarkArea::SetBenchMark(bench::Document const* document)
{
    m_drawCores.clear();
    if(document)
    {
        // Fill draw data.
        m_drawCores.reserve(document->m_cores.size());
        for(bench::DocumentCore const& docCore : document->m_cores)
        {
            DrawCore* drawCore = CreateCore(docCore);
            m_drawCores.push_back(drawCore);
        }

        ComputeUiData();
    }

    ComputeSize();
}

void BenchMarkArea::SetStartTime(double startTime)
{
    m_startTime = startTime;
}

DrawCore* BenchMarkArea::CreateCore(bench::DocumentCore const& docCore) const
{
    DrawCore* drawCore = new DrawCore();
    drawCore->m_name = docCore.m_name;
    drawCore->m_threads.reserve(docCore.m_threads.size());

    for(bench::DocumentThread const& docThread : docCore.m_threads)
    {
        DrawThread* drawThread = CreateThread(docThread);
        drawCore->AddThread(drawThread);
    }

    return drawCore;
}

DrawThread* BenchMarkArea::CreateThread(bench::DocumentThread const& docThread) const
{
    DrawThread* drawThread = new DrawThread();
    drawThread->m_name = docThread.m_name;
    drawThread->m_benches.reserve(docThread.m_benches.size());
    for(bench::DocumentBench const& docBench : docThread.m_benches)
    {
        DrawBench* drawBench = CreateBenchRec(docBench, nullptr);
        drawThread->AddBench(drawBench);
    }

    return drawThread;
}

DrawBench* BenchMarkArea::CreateBenchRec(bench::DocumentBench const& docBench,
                                         DrawBench* drawParent) const
{
    DrawBench* drawBench = new DrawBench();
    drawBench->m_parent = drawParent;
    boost::crc_32_type crc;
    crc.process_bytes(docBench.m_name.c_str(), docBench.m_name.length());
    drawBench->m_color = crc.checksum() & 0x00ffffff;
    Glib::ustring time = Glib::ustring::format(std::fixed,
                                               std::setprecision(2),
                                               docBench.m_stop - docBench.m_start);
    drawBench->m_name = Glib::ustring::compose("%1 (%2s)",
                                               docBench.m_name,
                                               time);
    drawBench->m_startTime = docBench.m_start;
    drawBench->m_stopTime = docBench.m_stop;
    drawBench->m_children.reserve(docBench.m_benches.size());
    for(bench::DocumentBench const& docBenchChild : docBench.m_benches)
    {
        DrawBench* drawBenchChild = CreateBenchRec(docBenchChild, drawBench);
        drawBench->AddBench(drawBenchChild);
    }

    return drawBench;
}

void BenchMarkArea::ComputeUiData()
{
    m_timeLength = 0.0;
    double top = 0.0;
    for(DrawCore* drawCore : m_drawCores)
    {
        drawCore->ComputeUiData(top);
        top += drawCore->GetHeight();
        m_timeLength = std::max(m_timeLength, drawCore->GetX() + drawCore->GetWidth());
    }
}

void BenchMarkArea::ComputeSize()
{
    if(!m_drawCores.empty())
    {
        double height = 0.0;
        for(DrawCore* drawCore : m_drawCores)
        {
            height += drawCore->GetHeight();
        }

        set_size_request(GetTimeLength() * m_mainWindow->GetTimeScale(), height);
    }
    else
    {
        set_size_request();
    }
}

double BenchMarkArea::GetTimeLength() const
{
    return m_timeLength;
}

bool BenchMarkArea::on_draw(Cairo::RefPtr<Cairo::Context> const& cr)
{
    if(!m_mainWindow->IsFileOpened())
        return false;

    cr->save();
    cr->set_antialias(Cairo::ANTIALIAS_NONE);

    // Draw cores' background.
    int const coreWidth = get_allocation().get_width();
    bool odd = false;
    for(DrawCore const* drawCore : m_drawCores)
    {
        drawCore->Draw(cr, m_layout, m_mainWindow->GetTimeScale(), odd, m_startTime, coreWidth);
        odd = !odd;
    }

    cr->restore();
    return true;
}
