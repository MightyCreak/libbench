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

#ifndef TOOL_RECTANGLE_H
#define TOOL_RECTANGLE_H

class Rectangle
{
public:
    void SetX(double x) { m_x = x; }
    double GetX() const { return m_x; }

    void SetY(double y) { m_y = y; }
    double GetY() const { return m_y; }

    void SetWidth(double width) { m_width = width; }
    double GetWidth() const { return m_width; }

    void SetHeight(double height) { m_height = height; }
    double GetHeight() const { return m_height; }

private:
    double m_x;
    double m_y;
    double m_width;
    double m_height;
};

#endif // TOOL_RECTANGLE_H
