#ifndef FRAME_H
#define FRAME_H
#include <wx/wx.h>
#include <wx/slider.h>
#include <vector>
#include <fstream>
#include <locale>
#include <bits/stdc++.h>
#include <time.h>
#include <string.h>
#include "music.h"

using namespace std;


class Frame: public wxFrame{
    public:
        Frame();
        ~Frame();
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
        void exit_program(wxCloseEvent &event);
        void save_playlist(wxCommandEvent &event);
        void next_music(wxCommandEvent &event);
        void previous_music(wxCommandEvent &event);
        void random_music(wxCommandEvent &event);
        void select_random();
        void clear_music(wxCommandEvent &event);
        void exit_program_from_menu(wxCommandEvent &event);

        string get_duration(uint64_t time);
        string split_string(string text); 
        
        wxButton *next_button;
        wxButton *previous_button;
        wxButton *open_button;
        wxButton *play_button;
        wxButton *stop_button;
        wxButton *pause_button;
        wxButton *delete_button;
        wxButton *random_button;
        wxButton *clear_button;


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
        enum{
          play_id , 
          stop_id ,
          slider_id ,
          open_id ,
          list_id , 
          delete_id ,
          label_id ,
          timer_id , 
          duration_id ,
          pause_id , 
          open_playlist_id , 
          save_playlist_id , 
          next_id , 
          previous_id , 
          random_id , 
          clear_id
        };

};
#endif
