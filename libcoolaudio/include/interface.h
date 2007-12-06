#ifndef _INTERFACE_H_
#define _INTERFACE_H_

// load file and do file init
typedef int (*fd_load) (const char *);
// play music
typedef int (*fd_play) (void);
// pause music and use resume to resume music
typedef void (*fd_pause) (void);
// stop music and free CURRENT FILE resource,USE XXXFINI to do free all resource
typedef int (*fd_stop) (void);
// resume a pause music
typedef void (*fd_resume)(void);
// seek position (options)
typedef void (*fd_seek) (int);
// return time tick (options)
typedef int (*fd_time) (void);
// return 1 if music is end 
typedef int (*fd_eos) (void);
// set volume (1-128)
typedef int (*fd_volume) (int);


typedef struct {
    fd_load     load;
    fd_play     play;
    fd_pause    pause;
    fd_stop     stop;
    fd_resume   resume;
    fd_seek     seek;
    fd_time     time;
    fd_eos      eos;
	fd_volume   volume;
    char        extension[256];        
} play_ops;

//the interface
#ifdef _cplusplus
extern "C"
#endif

//libcoolaudio default init
void CoolAudioDefaultInit();
//libcoolaudio default finish
void CoolAudioDefaultFini();

//ogg support
void OGGPlayInit(play_ops* ops);
void OGGPlayFini(void);

//mp3 support
void MP3PlayInit(play_ops* ops);
void MP3PlayFini(void);

//pcm support
void PCMPlayInit(play_ops* ops);
void PCMPlayFini(void);

//any others will come soon
//......

#ifdef _cplusplus
}
#endif


#endif
