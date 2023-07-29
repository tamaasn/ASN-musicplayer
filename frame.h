#include <wx/wx.h>
#include <wx/slider.h>
#include <vector>
#include <fstream>
#include <locale>
#include <bits/stdc++.h>
#include "music.h"

using namespace std;

class Frame: public wxFrame{
    public:
        Frame();
    private:
        void SliderGetValue(wxCommandEvent &event);
        void play_music(wxCommandEvent &event);
        void open_file(wxCommandEvent &event);
        void delete_file(wxCommandEvent &event);
        void timer_event(wxTimerEvent &event);
        void change_duration(wxCommandEvent &event);
        void stop_music(wxCommandEvent &event);
        void pause_music(wxCommandEvent &event);
        void open_playlist(wxCommandEvent &event);
        void exit_program(wxCommandEvent &event);
        void save_playlist(wxCommandEvent &event);
        
        string get_duration(uint64_t time);
        string split_string(string text); 

        wxButton *open_button;
        wxButton *play_button;
        wxButton *stop_button;
        wxButton *pause_button;
        wxButton *delete_button;
        wxSlider *slider;
        wxSlider *music_duration;
        wxTimer *timer;
        wxListBox *list_song;
        wxStaticText *label;
        wxStaticText *passed_duration_label;
        wxStaticText *duration_label;
        wxMenuBar *menubar;
        wxMenu *menufile;
        wxMenu *menuothers;

        bool changing_duration=false;
        
        string filedir="/";
        int song_index_played;
        int play_id=1;
        int stop_id=2;
        int slider_id=3;
        int open_id=4;
        int list_id=5;
        int delete_id=6;
        int label_id=7;
        int timer_id=8;
        int duration_id=9;
        int pause_id=10;
        int open_playlist_id=11;
        int save_playlist_id=12;

};
