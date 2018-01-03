#include "audio.h"

#include "error_handling.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_AUDIOS_REPEATING 20 //TODO: Load these from file.
#define MAX_AUDIO_NAME_LEN 30
#define OGG123_PATH "/usr/bin/ogg123"

struct _Audios{
  pid_t pids[MAX_AUDIOS_REPEATING];
  char name[MAX_AUDIOS_REPEATING][MAX_AUDIO_NAME_LEN];
  int num_pids;
  int empty_record;
};
/* Private functions */
int _add_rec(pid_t pid, char *name);
int _del_rec(pid_t pid);
int _search(void *key, audio_key_t type);
bool _file_exists(const char *name);

pid_t play_audio(char *filename, bool repeating){
  if(!filename || !_file_exists(filename)){
    HE("Audio file does not exists", "play_audio")
    return 0;
  }
  pid_t pid;
  if(repeating){
    if (playing_audios->num_pids == MAX_AUDIOS_REPEATING){
      HE("Cannot play another repeating audio", "play_audio")
      return 0;
    }
    if((pid = fork()) == 0){ //Child process until end of this if.
      int ret = execl(OGG123_PATH, OGG123_PATH, "-q", "-r", filename, NULL);
      if (ret == -1){//TODO: Think about this.
        return 0;
      }
      exit(0);
    }
    if (UINT_ERROR == _add_rec(pid, filename)){
      HE("Error adding record", "play_audio")
      return 0;
    }
    return pid;
  }
  if((pid = fork()) == 0){ //Child process until end of this if.
    int ret = execl(OGG123_PATH, OGG123_PATH, "-q", filename, NULL);
    if (ret == -1){//TODO: Think about this.
      return 0;
    }
    exit(0);
  }
  return pid;
}

int stop_audio(void *key, audio_key_t type){
  if(!key){
    HE("Key is null", "stop_audio")
    return UINT_ERROR;
  }
  pid_t pid;
  switch (type) {
    case PID:
      pid = *(pid_t *)key;
      playing_audios->empty_record=_del_rec(pid);
      if(playing_audios->empty_record == -1){
        HE("Error deleting record", "stop_audio")
        return UINT_ERROR;
      }
    break;
    case NAME:;
      int index = _search(key, type);
      if(index == -1){
        HE("Key not found", "stop_audio")
        return UINT_ERROR;
      }
      pid = playing_audios->pids[index];
      playing_audios->empty_record=_del_rec(pid);
      if(playing_audios->empty_record == -1){
        HE("Error deleting record", "stop_audio")
        return UINT_ERROR;
      }
    break;
  }
  char command[20];
  sprintf(command, "kill %d", pid);
  system(command); //TODO: Shall we fork this?
  return !UINT_ERROR;
}

Audios *audios_new(){
  playing_audios = calloc(1, sizeof(Audios));
  playing_audios->empty_record = -1;
  playing_audios->num_pids = 0;
  return playing_audios;
}

void audios_destroy(){
  for(int i = 0; i < playing_audios->num_pids; ++i){
    if (playing_audios->pids[i] == 0) continue;
    stop_audio(&playing_audios->pids[i], PID);
  }
  free(playing_audios);
};

/* Private functions definitions */
int _add_rec(pid_t pid, char *name){
  if(playing_audios->empty_record != -1){
    playing_audios->pids[playing_audios->empty_record] = pid;
    strcpy(playing_audios->name[playing_audios->empty_record], name);
    playing_audios->empty_record = -1;
    return !UINT_ERROR;
  }
  if(playing_audios->num_pids < MAX_AUDIOS_REPEATING){
    playing_audios->pids[playing_audios->num_pids] = pid;
    strcpy(playing_audios->name[playing_audios->num_pids], name);
    playing_audios->num_pids++;
    return !UINT_ERROR;
  }
  return UINT_ERROR;
}
int _del_rec(pid_t pid){
  int index = _search(&pid, PID);
  if(index == -1){
    HE("Cannot find given pid", "_del_rec")
    return -1;
  }
  playing_audios->pids[index]= 0;
  playing_audios->name[index][0] = '\0';
  return index;
}
int _search(void *key, audio_key_t type){
  if(type == PID){
    for (int i = playing_audios->num_pids-1; i >= 0; i--) {
      if(playing_audios->pids[i] == * (pid_t *) key){
        return i;
      }
    }
  }
  if(type == NAME){
    for (int i = playing_audios->num_pids-1; i >= 0; i--) {
      if(!strcmp(playing_audios->name[i], key)){
        return i;
      }
    }
  }
  return -1;
}

bool _file_exists(const char * filename){
  FILE * fp;
  if ((fp = fopen(filename, "r"))){
      fclose(fp);
      return true;
  }
  return false;
}
