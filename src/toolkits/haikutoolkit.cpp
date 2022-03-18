/*
HaikuToolkit for Oxe FM Synth
Copyright (C) 2022  Andrea Anzani <andrea.anzani@gmail.com>

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
#include "editor.h"
#include "haikutoolkit.h"
#include <Window.h>
#include <DataIO.h>
#include <TranslationUtils.h>
#include "bitmaps.h"
#include <WindowScreen.h>

BBitmap*    
GetBitmap(const char* array)
{
    unsigned int fileSize = *((unsigned int*)(array + 2)); //Bitmap header..
    
    BMemoryIO reader((void*)array, fileSize);

    BBitmap* bmap = BTranslationUtils::GetBitmap(&reader);
    if (bmap == NULL)
            printf("Error: no bitmap!!");

    return bmap;
}


CHaikuToolkit::CHaikuToolkit(void *parentWindow, CEditor *editor) : 
               BView(BRect(0, 0, GUI_WIDTH - 1, GUI_HEIGHT - 1 ), "CHaikuToolkit", B_FOLLOW_NONE, B_WILL_DRAW | B_PULSE_NEEDED) , fEditor(editor)
{
    fComposed = new BBitmap(Bounds(), B_RGB32, true);
    fDrawingView = new BView(Bounds(), "drawing", 0, 0);
    fComposed->AddChild(fDrawingView);

    BView*  parent = (BView*)parentWindow;

   fBitmap[BMP_CHARS]     =  GetBitmap(chars_bmp);
   fBitmap[BMP_KNOB]      =  GetBitmap(knob_bmp); 
   fBitmap[BMP_KNOB2]     =  GetBitmap(knob2_bmp);
   fBitmap[BMP_KNOB3]     =  GetBitmap(knob3_bmp);
   fBitmap[BMP_KEY]       =  GetBitmap(key_bmp);  
   fBitmap[BMP_BG]        =  GetBitmap(bg_bmp);
   fBitmap[BMP_BUTTONS]   =  GetBitmap(buttons_bmp);
   fBitmap[BMP_OPS]       =  GetBitmap(ops_bmp);

    parent->ResizeTo(GUI_WIDTH, GUI_HEIGHT);
    parent->AddChild(this);
}

void
CHaikuToolkit::AttachedToWindow()
{
    Window()->SetPulseRate(TIMER_RESOLUTION_MS * 1000); 
}

void
CHaikuToolkit::MouseUp(BPoint point)
{
    fEditor->OnLButtonUp();
}

void    
CHaikuToolkit::MouseDown(BPoint point)
{
    SetMouseEventMask(B_POINTER_EVENTS,	B_LOCK_WINDOW_FOCUS | B_NO_POINTER_HISTORY);
    MakeFocus(true);
}

void    
CHaikuToolkit::MouseMoved(BPoint point, uint32 transit, const BMessage* message)
{
    fEditor->OnMouseMove(point.x, point.y);
}

void
CHaikuToolkit::KeyDown(const char* bytes, int32 numBytes)
{
    fEditor->OnChar(bytes[0]);
}

void
CHaikuToolkit::MessageReceived(BMessage* msg)
{
    switch(msg->what)
    {
        case B_MOUSE_DOWN:
        {
            BPoint where = msg->GetPoint("where", BPoint(0,0));
            int32 clicks = msg->GetInt32("clicks", 1);
            if (clicks == 1)
                fEditor->OnLButtonDown(where.x, where.y);
            else
                fEditor->OnLButtonDblClick(where.x, where.y);

        }
        break;
        default:
        break;
    }
    BView::MessageReceived(msg);
}

void
CHaikuToolkit::Pulse()
{
    fEditor->Update();
}

CHaikuToolkit::~CHaikuToolkit() 
{
    for (int i = 0; i < BMP_COUNT; i++)
        if ( fBitmap[i] )
            delete fBitmap[i];

}

void 
CHaikuToolkit::CopyRect(int destX, int destY, int width, int height, int origBmp, int origX, int origY)
{
    BRect source(origX, origY, origX + width - 1, origY + height - 1);
    BRect destin(destX, destY, destX + width - 1, destY + height - 1);
    if (fDrawingView->Looper()->Lock())
    {
        fDrawingView->DrawBitmap(fBitmap[origBmp],  source, destin);
        fDrawingView->Sync();
        fDrawingView->Looper()->Unlock();
    }
    Invalidate(destin);
}

void 
CHaikuToolkit::StartMouseCapture() {}

void 
CHaikuToolkit::StopMouseCapture() {}

void 
CHaikuToolkit::StartWindowProcesses() {}

void
CHaikuToolkit::Draw(BRect rect)
{
    DrawBitmap(fComposed, rect, rect);
}

