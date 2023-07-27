#include "frame.h"

Frame::Frame(): wxFrame(nullptr , wxID_ANY , "ASN Music Player" , wxPoint(950,450) , wxSize(345,370)){
    wxPanel *panel = new wxPanel(this , -1);
    play_button = new wxButton(panel , play_id, _T("Play") , wxPoint(0,5) , wxSize(60,30));
    stop_button = new wxButton(panel , stop_id , _T("Stop") , wxPoint(60,5) , wxSize(60,30));
    delete_button = new wxButton(panel , delete_id , _T("Delete") , wxPoint(120,5) , wxSize(60,30));
    pause_button = new wxButton(panel , pause_id , _T("Pause") , wxPoint(180,5),wxSize(60,30));
    slider = new wxSlider(panel , slider_id , 0,0,100,wxPoint(60,230),wxSize(200,40));
    slider->SetValue(50);
    music_duration = new wxSlider(panel , duration_id,0,0,0,wxPoint(60,280),wxSize(200,40));
    list_song = new wxListBox(panel , list_id , wxPoint(0,35) , wxSize(340,200));

    label = new wxStaticText(panel , label_id, _T("Volume : ") , wxPoint(0,240) , wxSize(50,50));
    timer = new wxTimer(this,timer_id);
    passed_duration_label = new wxStaticText(panel , wxID_ANY , _T("0:0:0") , wxPoint(0,290),wxSize(60,50));
    duration_label = new wxStaticText(panel , wxID_ANY , _T("0:0:0") , wxPoint(260,290) , wxSize(60,50));

    menufile = new wxMenu;
    menuothers = new wxMenu;

    menufile->Append(open_id , "Open file\tCtrl-O" , "Open file");
    menufile->Append(open_playlist_id , "Open playlist\tCtrl-Shift-O" , "Open playlist");
    menufile->Append(save_playlist_id , "Save playlist\tCtrl-S" , "Save playlist");
    
    menuothers->Append(wxID_EXIT , "Exit" , "Exit");

    menubar = new wxMenuBar;
    menubar->Append(menufile , "&File");
    menubar->Append(menuothers , "&Others");
    SetMenuBar(menubar);

    Bind(wxEVT_SLIDER , &Frame::SliderGetValue , this , slider_id);
    Bind(wxEVT_SLIDER , &Frame::change_duration , this , duration_id);
    Bind(wxEVT_BUTTON , &Frame::play_music , this , play_id);
    Bind(wxEVT_BUTTON , &Frame::delete_file , this , delete_id);  
    Bind(wxEVT_TIMER , &Frame::timer_event , this , timer_id);
    Bind(wxEVT_BUTTON , &Frame::stop_music , this , stop_id);
    Bind(wxEVT_BUTTON , &Frame::pause_music , this , pause_id);
    Bind(wxEVT_MENU , &Frame::open_file , this , open_id);
    Bind(wxEVT_MENU , &Frame::open_playlist , this , open_playlist_id);
    Bind(wxEVT_MENU , &Frame::save_playlist , this , save_playlist_id);
    Bind(wxEVT_MENU , &Frame::exit_program , this , wxID_EXIT);
    this->SetMaxSize(wxSize(345,370));
    this->SetMinSize(wxSize(345,370));

    init();
}

void Frame::exit_program(wxCommandEvent &event){
    cout << "Closing program..." << endl;
    this->Close(true);
}

void Frame::save_playlist(wxCommandEvent &event){
    wxFileDialog filepath(this , _("Save playlist") , "" , "my_playlist.ply" , "Playlist files(*.ply)|*.ply" , wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (filepath.ShowModal() == wxID_CANCEL){
        return;
    }
    vector<string>::const_iterator vector_begin;
    ofstream file;
    file.open(filepath.GetPath().utf8_str());
    for (vector_begin=musics.begin(); vector_begin != musics.end(); vector_begin++){
        file << *vector_begin+"\n";
    }
}


void Frame::open_playlist(wxCommandEvent &event){
    wxFileDialog filepath(this , _("Open playlist") , "","","Playlist files (*.ply)|*.ply" , wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (filepath.ShowModal() == wxID_CANCEL){
        return;
    }
    wfstream file(filepath.GetPath().utf8_str());
    file.imbue(locale(""));
    wstring files;
    while (getline(file,files)){
        wxString filename(files);
        string str = string(filename.utf8_str());
        list_song->Append(files);
        add_queue(str);
    }
}


// turn duration into format "hour:minute:second"   
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
// pause music
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
    if (!Mix_PlayingMusic() && is_playing){
        play(song_index_played);
    }
    if (Mix_PlayingMusic() && passed_duration==0){
        // if music begin
        //
        // change the starting index
        song_index_played=0;
        cout << total_duration << endl;
        music_duration->SetMax(total_duration);
        list_song->SetSelection(index_of_music-1);
        duration_label->SetLabel(get_duration(total_duration));
    }
    if (is_playing){
        if (!changing_duration){
            
            if(passed_duration++ >= total_duration){
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
    }
    else{
        song_index_played=0;
        timer->Stop();
        music_duration->SetValue(0);
        passed_duration_label->SetLabel("0:0:0");;
    }
}



void Frame::SliderGetValue(wxCommandEvent &event){
    cout << slider->GetValue() << endl;
    change_volume(slider->GetValue());
}

void Frame::play_music(wxCommandEvent &event){
    index_of_music=0;
    song_index_played=list_song->GetSelection();

    if (list_song->GetSelection() < 0){
        song_index_played=0;
    }
    change_volume(slider->GetValue());
    Mix_HaltMusic();
    is_playing=false;
    timer->Stop();
    load_iterator();
    index_of_music=0;
    pause_button->SetLabel("Pause");
    is_playing=true;
    paused=false;
    play(song_index_played);
    timer->Start(1000);
}

void Frame::open_file(wxCommandEvent &event){
    wxFileDialog filename(this,_("Select music"),"","","",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    filename.SetDirectory(filedir);
    if (filename.ShowModal()==wxID_CANCEL){
        return;
    }
    filedir=filename.GetDirectory().utf8_str();

    list_song->Append(filename.GetPath());
    list_index++;
    cout <<"File: " << filename.GetPath().utf8_str() << endl;
    add_queue(string(filename.GetPath().utf8_str()));
}

void Frame::delete_file(wxCommandEvent &event){
    int index_selected = list_song->GetSelection();
    if (index_selected < 0){
        return;
    }
    delete_music(index_selected);
    list_song->Delete(index_selected);

}
