#include "frame.h"
#include <string.h>

Frame::Frame(): wxFrame(nullptr , wxID_ANY , "Simple Music Player" , wxPoint(950,450) , wxSize(480,370)){
    srand(time(NULL));
    //SDL_Init(SDL_INIT_EVERYTHING);
    wxPanel *panel = new wxPanel(this , -1);
    play_button = new wxButton(panel , play_id, _T("Play") , wxPoint(0,5) , wxSize(60,30));
    stop_button = new wxButton(panel , stop_id , _T("Stop") , wxPoint(60,5) , wxSize(60,30));
    delete_button = new wxButton(panel , delete_id , _T("Delete") , wxPoint(120,5) , wxSize(60,30));
    pause_button = new wxButton(panel , pause_id , _T("Pause") , wxPoint(180,5),wxSize(60,30));
    previous_button = new wxButton(panel , previous_id , _T("<<") , wxPoint(240,5) , wxSize(60,30));
    next_button = new wxButton(panel , next_id , _T(">>") , wxPoint(300,5) , wxSize(60,30));
    random_button = new wxButton(panel , random_id , _T("Random : off") , wxPoint(360 , 5) , wxSize(100,30));
    clear_button = new wxButton(panel , clear_id , _T("Clear") , wxPoint(460 , 5) , wxSize(60,30));
    slider = new wxSlider(panel , slider_id , 0,0,100,wxPoint(60,230),wxSize(200,40));
    slider->SetValue(50);
    music_duration = new wxSlider(panel , duration_id,0,0,100,wxPoint(60,280),wxSize(200,40));
    list_song = new wxListBox(panel , list_id , wxPoint(0,35) , wxSize(460,200));

    label = new wxStaticText(panel , label_id, _T("Volume : ") , wxPoint(0,240) , wxSize(50,50));
    timer = new wxTimer(this,timer_id);
    passed_duration_label = new wxStaticText(panel , wxID_ANY , _T("0:0:0") , wxPoint(0,290),wxSize(60,50));
    duration_label = new wxStaticText(panel , wxID_ANY , _T("0:0:0") , wxPoint(260,290) , wxSize(60,50));

    menufile = new wxMenu;
    menuothers = new wxMenu;
    menufile->Append(open_id , "Open file\tCtrl-O" , "Open file");
    menufile->Append(open_playlist_id , "Open playlist\tCtrl-Shift-O" , "Open playlist");
    menufile->Append(save_playlist_id , "Save playlist\tCtrl-S" , "Save playlist");
    
    menuothers->Append(wxID_EXIT , "Exit\tCtrl-Q" , "Exit");

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
    Bind(wxEVT_MENU , &Frame::exit_program_from_menu , this , wxID_EXIT);
    Bind(wxEVT_BUTTON , &Frame::next_music , this , next_id);
    Bind(wxEVT_BUTTON , &Frame::previous_music , this , previous_id);
    Bind(wxEVT_BUTTON , &Frame::random_music , this , random_id);
    Bind(wxEVT_BUTTON , &Frame::clear_music , this , clear_id);
    Bind(wxEVT_CLOSE_WINDOW , &Frame::exit_program , this);
    this->SetMaxSize(wxSize(550,370));
    this->SetMinSize(wxSize(550,370));
    init();
}

void Frame::clear_music(wxCommandEvent &event){
  random_musics.clear();
  list_song->Clear();
  musics.clear();
  std::cout << "cleared\n";
}

void Frame::select_random(){
  if (!random_musics.empty()){
    random_musics.clear();
  }
  for(int i=0;i<musics.size();i++){
    random_musics.push_back(i);
  }

  for(int i=1;i<musics.size();i++){
    int j = rand()%i;
    int x = random_musics[i];
    random_musics[i]=random_musics[j];
    random_musics[j]=x;
  }
    std::cout << "total musics : " << musics.size() << "\nrandom : " << random_musics.size() << '\n';

}

void Frame::random_music(wxCommandEvent &event){
  is_random = !is_random;
  if(!is_random){
    random_button->SetLabel("Random : off");
    index_of_music = random_musics[index_of_music-1]+1;
    random_musics.clear();
    return;
  }
  random_button->SetLabel("Random : on");
  select_random();
}

void Frame::next_music(wxCommandEvent &event){
    if (is_random && random_musics.empty()) select_random();
    Mix_HaltMusic();
}

void Frame::previous_music(wxCommandEvent &event){
    if (is_random && random_musics.empty()) select_random();
    Mix_HaltMusic();
    index_of_music -= 2;
    total_played++;
}

void Frame::exit_program_from_menu(wxCommandEvent &event){
    Destroy();  

    delete list_song;
    delete play_button;
    delete next_button;
    delete pause_button;
    delete delete_button;
    delete random_button;
    delete clear_button;
    delete slider;
    delete music_duration;
    delete label;
    delete passed_duration_label;
    delete duration_label;
    timer->Stop();
    cout << "Closing program..." << endl;
    destroy_music();
    this->Close(true);
    cout << "Program closed" << endl;
}

void Frame::exit_program(wxCloseEvent &event){
    Destroy();
    
    delete list_song;
    delete play_button;
    delete next_button;
    delete pause_button;
    delete delete_button;
    delete random_button;
    delete clear_button;
    delete slider;
    delete music_duration;
    delete label;
    delete passed_duration_label;
    delete duration_label;

    timer->Stop();
    cout << "Closing program..." << endl;
    destroy_music();
    event.Skip();
    cout << "Program closed" << endl;
}

void Frame::save_playlist(wxCommandEvent &event){
    wxFileDialog filepath(this , _("Save playlist") , "" , "my_playlist.ply" , "Playlist files(*.ply)|*.ply" , wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (filepath.ShowModal() == wxID_CANCEL) return;
    ofstream file;
    file.open(filepath.GetPath().utf8_str());
    for(int i=0;i<musics.size();i++){
      file << musics[i]+"\n";
    }
}


void Frame::open_playlist(wxCommandEvent &event){
    wxFileDialog filepath(this , _("Open playlist") , "","","Playlist files (*.ply)|*.ply" , wxFD_OPEN| wxFD_FILE_MUST_EXIST);
    if (filepath.ShowModal() == wxID_CANCEL){
        return;
    }
    wfstream file(filepath.GetPath().utf8_str());
    file.imbue(locale(""));
    wstring files;
    while (getline(file,files)){
        wxString filename(files);
        string str = string(filename.utf8_str());
        char *x;
        x = strtok((char*)str.c_str() , "/");
        int length=0;
        char *filedir[512];
        while (x != NULL && length < 512){
          filedir[length] = x;
          x = strtok(NULL , "/");
          length++;
        }
        list_song->Append(wxString::FromUTF8(filedir[length-1]));
        add_queue(string(filename.utf8_str()));
    }
}


// turn duration into format "hour:minute:second"   
string Frame::get_duration(uint64_t time){
    if (time == 0){
        return "0:0:0";
    }
    uint64_t second=time%60;
    uint64_t minute=(time-second)/60;
    uint64_t hour=3600/(time*time);
    if (minute>=60) minute=minute-60;
    if (minute<60) hour=0;
    char data[1024*5];
    snprintf(data , sizeof(data) , "%lu:%lu:%lu" , hour , minute , second);
    string time_str= string(data);
    return time_str;
}
// pause music
void Frame::pause_music(wxCommandEvent &event){
  paused = !paused;  
  if (Mix_PausedMusic()){
        Mix_ResumeMusic();
        timer->Start(1000);
        pause_button->SetLabel("Pause");
        return;
    }
    pause_button->SetLabel("Resume");
    Mix_PauseMusic();
    timer->Stop();
}


void Frame::stop_music(wxCommandEvent &event){
    index_of_music=0;
    total_played=0;
    is_playing=false;
    Mix_HaltMusic();
    timer->Stop();
}



void Frame::change_duration(wxCommandEvent &event){
    changing_duration=true;
}


void Frame::timer_event(wxTimerEvent &event){
    if (!Mix_PlayingMusic() && is_playing){
        try{
          if (random_musics.empty() && is_random) select_random();
          play(song_index_played);
        }
        catch(...){
          std::cout << "oops\n";
        }
    }
    if (Mix_PlayingMusic() && passed_duration==0){
        // if music begin
        //
        // change the starting index
        song_index_played=0;
        cout << total_duration << endl;
        music_duration->SetMax(total_duration);
        list_song->SetSelection(playing_now);
        duration_label->SetLabel(get_duration(total_duration));
    }
    if (is_playing){
        if (!changing_duration){
            if(passed_duration++ >= total_duration) passed_duration=0;
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
    Mix_HaltMusic();
    timer->Stop();
    is_playing=false;
    index_of_music=0;
    song_index_played=list_song->GetSelection();

    if (list_song->GetSelection() < 0){
        song_index_played=0;
    }
    if (is_random) select_random();
    change_volume(slider->GetValue());
    pause_button->SetLabel("Pause");
    is_playing=true;
    paused=false;
    play(song_index_played);
    timer->Start(1000);
}

void Frame::open_file(wxCommandEvent &event){
    wxFileDialog filename(this,_("Select music"),"","","",wxFD_MULTIPLE|wxFD_FILE_MUST_EXIST);
    filename.SetDirectory(filedir);
    if (filename.ShowModal()==wxID_CANCEL){
        return;
    }
    filedir=filename.GetDirectory().utf8_str();
    wxArrayString filepaths;
    filename.GetPaths(filepaths);
    size_t totalfiles = filepaths.GetCount();
    cout << "Total files : " << totalfiles << endl;
    for(size_t i=0;i<totalfiles;i++){
        cout <<"File: " << filepaths.Item(i) << endl;
        char *filepath[512];
        int length=0;
        char *file_;
        char buffer[1024];
        string a = string(filepaths.Item(i).utf8_str());
        strncpy(buffer , a.c_str() , sizeof(buffer));
        char *x = (char*)a.c_str();
        file_ = strtok(buffer , "/");
        while(file_ != NULL && length < 512){
          filepath[length] = file_;
          file_ = strtok(NULL , "/");
          length++;
          //printf(">>%s" , file_);
        }
        //filename[0]='\0';

       // list_song->Append(filepaths.Item(i));
        printf("length = %d" , length);
        list_song->Append(wxString::FromUTF8(filepath[length-1]));
        add_queue(string(filepaths.Item(i).utf8_str()));
    }
}

void Frame::delete_file(wxCommandEvent &event){
    int index_selected = list_song->GetSelection();
    if (index_selected < 0){
        return;
    }
    delete_music(index_selected);
    list_song->Delete(index_selected);  

}

Frame::~Frame(){
}
