/*
Oxe FM Synth: a software synthesizer
Copyright (C) 2004-2015  Daniel Moura <oxe@oxesoft.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include "opengltoolkit.h"
    #include "cocoatoolkit.h"
    #define COSToolkit CCocoaToolkit
#elif defined(__linux)
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #ifdef USE_OPENGL
        #include <GL/glx.h>
        #include "opengltoolkit.h"
    #endif
    #include "xlibtoolkit.h"
    #define COSToolkit CXlibToolkit
#elif defined(__HAIKU__)
    #include <haikutoolkit.h>
    #define COSToolkit CHaikuToolkit
#else
    #include <windows.h>
    #include "windowstoolkit.h"
    #define COSToolkit CWindowsToolkit
#endif
