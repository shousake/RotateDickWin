#ifndef ROTATEDICK_FUNCTIONS_H
#define ROTATEDICK_FUNCTIONS_H

#include<SDL.h>
#include <stdlib.h>
#include<time.h>
#include<math.h>
SDL_Renderer* renderer;
SDL_Texture* TRD;
SDL_Texture* Tbk;
SDL_Window* window;
int logical_width = 970;
int logical_height = 545;
typedef struct {
    Uint8* start; // ��Ƶ������ʼλ��
    Uint8* pos;   // ��ǰ��Ƶ����λ��
    Uint32 length; // ��ǰʣ�೤��
    Uint32 total_length; // ��Ƶ�����ܳ���
} AudioData;

struct Dick
{
    double deg;
    SDL_Surface* orDick;
    SDL_bool isDick;
    SDL_Texture* Dtext;
    SDL_Rect DPos;
    int RotateTime;
};
struct Dick d[6];
int rnum;
void audio_callback(void* userdata, Uint8* stream, int len);
void end();
SDL_AudioSpec wav_spec;
Uint8* wav_buffer;
Uint32 wav_length;
AudioData audio;
void audio_callback_once(void* userdata, Uint8* stream, int len) {
    AudioData* audio = (AudioData*)userdata;



    len = (len > audio->length ? audio->length : len);
    SDL_memcpy(stream, audio->pos, len);

    audio->pos += len;
    audio->length -= len;
}
void audio_callback(void* userdata, Uint8* stream, int len) {
    AudioData* audio = (AudioData*)userdata;

    if (audio->length == 0) {
        // �����Ƶ������ϣ����¿�ʼ����
        audio->pos = audio->start;
        audio->length = audio->total_length;
    }

    len = (len > audio->length ? audio->length : len);
    SDL_memcpy(stream, audio->pos, len);

    audio->pos += len;
    audio->length -= len;
}
void end()
{
    SDL_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


#endif //ROTATEDICK_FUNCTIONS_H

