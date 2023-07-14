#include "frame.h"
#include <string>


Frame::Frame(): wxFrame(nullptr , wxID_ANY , "ASN Music Player" , wxPoint(950,450) , wxSize(540,350)){
    wxLogMessage("Welcome");
    wxPanel *panel = new wxPanel(this , -1);

    open_button = new wxButton(panel , open_id , _T("Open") , wxPoint(0,5) , wxSize(60,30));
    play_button = new wxButton(panel , play_id, _T("Play") , wxPoint(60,5) , wxSize(60,30));
    stop_button = new wxButton(panel , stop_id , _T("Stop") , wxPoint(120,5) , wxSize(60,30));
    delete_button = new wxButton(panel , delete_id , _T("Delete") , wxPoint(60,35) , wxSize(60,30));
    pause_button = new wxButton(panel , pause_id , _T("Pause") , wxPoint(0,35),wxSize(60,30));
    slider = new wxSlider(panel , slider_id , 0,0,100,wxPoint(50,230),wxSize(200,40));
    slider->SetValue(50);
    music_duration = new wxSlider(panel , duration_id,0,0,0,wxPoint(50,280),wxSize(200,40));
    list_song = new wxListBox(panel , list_id , wxPoint(180,5) , wxSize(340,200));

    label = new wxStaticText(panel , label_id, _T("Volume : ") , wxPoint(10,240) , wxSize(50,50));
    timer = new wxTimer(this,timer_id);
    passed_duration_label = new wxStaticText(panel , wxID_ANY , _T("0:0:0") , wxPoint(10,290),wxSize(60,50));
    duration_label = new wxStaticText(panel , wxID_ANY , _T("0:0:0") , wxPoint(250,290) , wxSize(60,50));
    Bind(wxEVT_BUTTON , &Frame::play_music , this , play_id);
    Bind(wxEVT_SLIDER , &Frame::SliderGetValue , this , slider_id);
    Bind(wxEVT_SLIDER , &Frame::change_duration , this , duration_id);
    Bind(wxEVT_BUTTON , &Frame::open_file , this , open_id);
    Bind(wxEVT_BUTTON , &Frame::delete_file , this , delete_id);  
    Bind(wxEVT_TIMER , &Frame::timer_event , this , timer_id);
    Bind(wxEVT_BUTTON , &Frame::stop_music , this , stop_id);
    Bind(wxEVT_BUTTON , &Frame::pause_music , this , pause_id);
    init();
}

string Frame::get_duration(uint64_t time){
    if (time == 0){
        return "0:0:0";
    }
    int second=time%60;
    int minute=(time-second)/60;
    int hour=3600/(time*time);
    if (minute>=60){
        minute=minute-60;
    }
    if (minute<60){
        hour=0;
    }
    char data[1024];
    sprintf(data , "%d:%d:%d" , hour , minute , second);
    string time_str= string(data);
    return time_str;
}

void Frame::pause_music(wxCommandEvent &event){
    if (Mix_PausedMusic() == 1){
        paused=false;
        Mix_ResumeMusic();
        timer->Start(1000);
        pause_button->SetLabel("Pause");
        return;
    }
    pause_button->SetLabel("Resume");
    paused=true;
    Mix_PauseMusic();
    timer->Stop();
}


void Frame::stop_music(wxCommandEvent &event){
    index_of_music=0;
    is_playing=false;
    Mix_HaltMusic();
    timer->Stop();
}



void Frame::change_duration(wxCommandEvent &event){
    changing_duration=true;
}


void Frame::timer_event(wxTimerEvent &event){
    cout << "Timer on\n";
    play(song_index_played);
    if (Mix_PlayingMusic() && passed_duration==0){
        cout << total_duration << endl;
        music_duration->SetMax(total_duration);
        list_song->SetSelection(index_of_music-1);
        duration_label->SetLabel(get_duration(total_duration));
    }
    if (is_playing){
        if (!changing_duration){
            if (passed_duration++ >= total_duration){
                passed_duration=0;
            }
        }
        else{
            passed_duration=music_duration->GetValue();
            change_music_duration(passed_duration);
            changing_duration=false;
        }
        music_duration->SetValue(passed_duration);
        passed_duration_label->SetLabel(get_duration(passed_duration));
    }else{
        timer->Stop();
        music_duration->SetValue(0);
        passed_duration_label->SetLabel("0:0:0");
    }
}



void Frame::SliderGetValue(wxCommandEvent &event){
    std::cout << slider->GetValue() << std::endl;
    change_volume(slider->GetValue());
}

void Frame::play_music(wxCommandEvent &event){
    index_of_music=0;
    if (list_song->GetSelection() < 0){
        song_index_played=0;
    }
    else{
        song_index_played=list_song->GetSelection();
    }
    change_volume(slider->GetValue());
    Mix_HaltMusic();
    is_playing=false;
    timer->Stop();
    load_iterator();
    //song_index_played=list_song->GetSelection();
    index_of_music=0;
    pause_button->SetLabel("Pause");
    is_playing=true;
    paused=false;
    timer->Start(1000);
}

void Frame::open_file(wxCommandEvent &event){
    wxFileDialog filename(this,_("Select music"),"","","",wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (filename.ShowModal()==wxID_CANCEL){
        return;
    }

    list_song->Append(filename.GetFilename());
    list_index++;
    cout <<"File: " << filename.GetPath().utf8_str() << endl;
    add_queue(std::string(filename.GetPath().utf8_str()));
}

void Frame::delete_file(wxCommandEvent &event){
    int index_selected = list_song->GetSelection();
    if (index_selected < 0){
        return;
    }
    //musics.erase(string(list_song->GetString(index_selected)));
    delete_music(index_selected);
    list_song->Delete(index_selected);

}
