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
#include "editor.h"
#include "haikutoolkit.h"
#include <Window.h>
#include <DataIO.h>
#include <TranslationUtils.h>
#include "bitmaps.cpp"
#include <WindowScreen.h>

BBitmap*    
GetBitmap(const char* array, unsigned int len)
{
     BMemoryIO reader((void*)array, len);
    
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

   fBitmap[BMP_CHARS]     =  GetBitmap(chars_bmp, chars_bmp_dim);
   fBitmap[BMP_KNOB]      =  GetBitmap(knob_bmp, knob_bmp_dim); 
   fBitmap[BMP_KNOB2]     =  GetBitmap(knob2_bmp, knob2_bmp_dim);
   fBitmap[BMP_KNOB3]     =  GetBitmap(knob3_bmp, knob3_bmp_dim);
   fBitmap[BMP_KEY]       =  GetBitmap(key_bmp, key_bmp_dim);  
   fBitmap[BMP_BG]        =  GetBitmap(bg_bmp, bg_bmp_dim);
   fBitmap[BMP_BUTTONS]   =  GetBitmap(buttons_bmp, buttons_bmp_dim);
   fBitmap[BMP_OPS]       =  GetBitmap(ops_bmp, ops_bmp_dim);

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

