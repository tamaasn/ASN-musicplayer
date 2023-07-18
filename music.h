#ifndef MUSIC_H
#define MUSIC_H


#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <iostream>
#include <string>

Mix_Music *music=NULL;
int highest_music_id=0;
int total_duration=0;
int passed_duration=0;
bool is_playing=false;
int index_of_music=0;
bool paused=false;

std::vector<std::string>musics;
std::vector<std::string>::const_iterator it;
std::vector<std::string>::const_iterator it_end = musics.end();

void init(){
    SDL_Init(SDL_INIT_EVERYTHING);

    Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
}

void play(int starting_index){
    std::cout << starting_index << std::endl;
    if (!Mix_PlayingMusic() && is_playing && !paused){
        it = musics.begin()+index_of_music;
        std::cout << starting_index << std::endl;

        if (it==it_end){
            std::cout << "Ended\n";
            index_of_music=0;
            return;
        }



        if (index_of_music < starting_index){
            std::cout << index_of_music << std::endl;
            index_of_music++;
            return;
        }

        passed_duration=0;
        Mix_FreeMusic(music);

        const char*music_file=&*it->c_str();
        std::cout << music_file << std::endl;
        std::cout << music_file << std::endl;
        music = Mix_LoadMUS(music_file);
        if (music == NULL){
            std::cout << Mix_GetError() << std::endl;
        }
        total_duration = (int)Mix_MusicDuration(music);
        std::cout << total_duration << std::endl;
        Mix_PlayMusic(music,0);
        //it++;
        index_of_music++;
    }
}

void load_iterator(){
    //it = musics.begin();
    it_end = musics.end();
}

void change_volume(int volume){
    Mix_VolumeMusic(volume);
}


void change_music_duration(int duration){
    Mix_SetMusicPosition(duration);
}


void add_queue(std::string filename){
  musics.push_back(filename);
  load_iterator();
}

void delete_music(int index){
    if (index_of_music > index){
        index_of_music--;
        //it--;
    }
   musics.erase(musics.begin()+index); 
   load_iterator();
    std::cout << "erased\n";
}

#endif
