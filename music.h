#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <iostream>
#include <string>

Mix_Music *music;
double total_duration=0;
int passed_duration=0;
bool is_playing=false;
int index_of_music=0;
bool paused=false;
int total_played=0;
bool is_random=false;
int playing_now;

std::vector<std::string>musics;
std::vector<int>random_musics;


void init(){
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096);
}

void destroy_music(){
  Mix_FreeMusic(music);
  Mix_HaltMusic();
  Mix_CloseAudio();
  Mix_Quit();
  SDL_Quit();
  musics.clear();
  std::vector<std::string>().swap(musics);
  random_musics.clear();
  std::vector<int>().swap(random_musics);
}

double get_music_length(const char *filename){
  double duration = Mix_MusicDuration(music);
  return duration;
}


void play(int starting_index){
    std::cout << starting_index << std::endl;
    if (!Mix_PlayingMusic() && is_playing && !paused){
        if (index_of_music == musics.size()){
            std::cout << "Ended\n";
            index_of_music=0;
            return;
        }

        std::string music_file;
        //std::cout << "Random music : " << random_musics[index_of_music] << "\n";
        music_file = musics[index_of_music];
        playing_now = index_of_music;

        if (index_of_music<0) index_of_music=0;
        if (is_random){
          int random_music = random_musics[index_of_music];
          music_file = musics[random_music];
          playing_now= random_music;
          std::cout << "Random music index :" << random_music <<"\n";
          std::cout << "Music total :" << musics.size() << "\n";
        }
        std::cout << starting_index << std::endl;
      

        if (index_of_music < starting_index){
            index_of_music = starting_index; 
            return;
        }

        passed_duration=0;
        Mix_FreeMusic(music);
        music=nullptr;

        std::cout << music_file << std::endl;
        music = Mix_LoadMUS(music_file.c_str());
        if (music == NULL){
            std::cout << Mix_GetError() << std::endl;
            index_of_music++;
            return;
        }
        total_duration = get_music_length(music_file.c_str());
        Mix_PlayMusic(music,0);
        std::cout << total_duration << std::endl;
        index_of_music++;
    }
}



void change_volume(int volume){
    Mix_VolumeMusic(volume);
}


void change_music_duration(int duration){
    Mix_SetMusicPosition(duration);
}


void add_queue(std::string filename){
  musics.push_back(filename);
}

void delete_music(int index){
    if (index_of_music > index){
        index_of_music--;
    }
   musics.erase(musics.begin()+index); 
    std::cout << "erased\n";
}
#endif
