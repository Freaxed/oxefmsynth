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

#include <stdio.h>
#include <View.h>
#include <Bitmap.h>

class CHaikuToolkit : public CToolkit , public BView
{
public:
             CHaikuToolkit(void *parentWindow, CEditor *editor);
    virtual ~CHaikuToolkit();
    
    // CToolkit interface
    void CopyRect(int destX, int destY, int width, int height, int origBmp, int origX, int origY);
    void StartMouseCapture();
    void StopMouseCapture();
    void StartWindowProcesses();
    int  WaitWindowClosed() {return 0;}   // standalone only


    //BView interface
    void    Draw(BRect rect);
    void    AttachedToWindow();
    void    Pulse();
    void    MouseDown(BPoint point);
    void    MouseUp(BPoint point);
    void    MouseMoved(BPoint point, uint32 transit, const BMessage* message);
    void    MessageReceived(BMessage* msg);
    void    KeyDown(const char* bytes, int32 numBytes);


private:
    CEditor *fEditor;

    BBitmap* fBitmap[BMP_COUNT];
    BBitmap* fComposed;  
    BView*   fDrawingView;
};


