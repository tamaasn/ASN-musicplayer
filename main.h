#include "frame.h"

class App: public wxApp{
    public:
        bool OnInit() override;
};

bool App::OnInit(){
    Frame *frame = new Frame();
    frame->Show(true);
    return true;
}
