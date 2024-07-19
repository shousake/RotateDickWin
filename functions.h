#ifndef ROTATEDICK_FUNCTIONS_H
#define ROTATEDICK_FUNCTIONS_H

#include<SDL.h>
#include <stdio.h>
#include <string.h>
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
    SDL_bool loop; // �Ƿ�ѭ������
    SDL_bool play; // �Ƿ����ڲ���
} AudioData;
typedef struct
{
    SDL_AudioSpec wav_spec;
    Uint8* wav_buffer;
    Uint32 wav_length;
} Audio_Info;
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
void audio_callback_once(void* userdata, Uint8* stream, int len);
void mix_audio_callback(void* userdata, Uint8* stream, int len);
Uint8* convert_audio_format(SDL_AudioSpec* src_spec, Uint8* src_buffer, Uint32 src_length, SDL_AudioSpec* dst_spec, Uint32* dst_length);
void end();
SDL_Texture* get_BMPTexture(char* file);
void flipHorizontal(SDL_Surface* surface);
void playWAV(char* file);
SDL_Texture* get_reserveBMPTexture(char* file);
SDL_Surface* get_BMPSerface(char* file);
SDL_AudioSpec wav_spec;
Uint8* wav_buffer;
Uint32 wav_length;
AudioData audio;
AudioData audio_data[5];
Audio_Info audio_info[5];
void audio_callback_once(void* userdata, Uint8* stream, int len)
{
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
void mix_audio_callback(void* userdata, Uint8* stream, int len)
{
    AudioData* audio_data = (AudioData*)userdata;
    SDL_memset(stream, 0, len); // �����Ƶ������

    for (int i = 0; i < 2; ++i) { // 2����Ƶ�����������ֺ���Ч��

        AudioData* audio = &audio_data[i];

        if (!audio->play || audio->length == 0) {
            continue;
        }

        int length = (len > audio->length ? audio->length : len);
        SDL_MixAudioFormat(stream, audio->pos, AUDIO_F32, length, SDL_MIX_MAXVOLUME);
        //SDL_memcpy(stream, audio->pos, len);
        audio->pos += length;
        audio->length -= length;

        if (audio->length == 0) {
            if (audio->loop) {
                audio->pos = audio->start;
                audio->length = audio->total_length;
            }
            else {
                audio->play = SDL_FALSE; // ��Ч���β�����ϣ�ֹͣ����
            }
        }
    }
}
Uint8* convert_audio_format(SDL_AudioSpec* src_spec, Uint8* src_buffer, Uint32 src_length, SDL_AudioSpec* dst_spec, Uint32* dst_length)
{
    SDL_AudioCVT cvt;
    if (SDL_BuildAudioCVT(&cvt, src_spec->format, src_spec->channels, src_spec->freq, dst_spec->format, dst_spec->channels, dst_spec->freq) < 0) {
        fprintf(stderr, "SDL_BuildAudioCVT failed: %s\n", SDL_GetError());
        return NULL;
    }

    cvt.len = src_length;
    cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);
    if (cvt.buf == NULL) {
        fprintf(stderr, "SDL_malloc failed: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_memcpy(cvt.buf, src_buffer, src_length);
    if (SDL_ConvertAudio(&cvt) < 0) {
        fprintf(stderr, "SDL_ConvertAudio failed: %s\n", SDL_GetError());
        SDL_free(cvt.buf);
        return NULL;
    }

    *dst_length = cvt.len_cvt;
    return cvt.buf;
}

SDL_bool load_WAV(char* file, SDL_bool isRepeat)
{

    if (SDL_LoadWAV(file, &wav_spec, &wav_buffer, &wav_length) == NULL) {
        SDL_Log("Could not open wav file: %s\n", SDL_GetError());
        return SDL_FALSE;
    }


    audio.start = wav_buffer;
    audio.pos = wav_buffer;
    audio.length = wav_length;
    audio.total_length = wav_length;
    if (isRepeat)
    {
        wav_spec.callback = audio_callback;
    }
    else {
        wav_spec.callback = audio_callback_once;
    }

    wav_spec.userdata = &audio;
    if (SDL_OpenAudio(&wav_spec, NULL) < 0)
    {
        SDL_Log("SDL_OpenAudio failed: %s\n", SDL_GetError());
        SDL_FreeWAV(wav_buffer);

        return SDL_FALSE;
    }
    return SDL_TRUE;
}
void flipHorizontal(SDL_Surface* surface)
{
    int pitch = surface->pitch; // The length of a row of pixels in bytes
    int height = surface->h;
    Uint8* pixels = (Uint8*)surface->pixels;

    for (int y = 0; y < height; ++y) {
        Uint8* row = pixels + y * pitch;
        for (int x = 0; x < surface->w / 2; ++x) {
            int oppositeX = surface->w - x - 1;
            for (int b = 0; b < surface->format->BytesPerPixel; ++b) {
                Uint8 tmp = row[x * surface->format->BytesPerPixel + b];
                row[x * surface->format->BytesPerPixel + b] = row[oppositeX * surface->format->BytesPerPixel + b];
                row[oppositeX * surface->format->BytesPerPixel + b] = tmp;
            }
        }
    }
}
SDL_Surface* get_BMPSerface(char* file)
{

    return SDL_LoadBMP(file);
}
SDL_Texture* get_BMPTexture(char* file)
{
    return SDL_CreateTextureFromSurface(renderer, get_BMPSerface(file));
}
SDL_Texture* get_reserveBMPTexture(char* file)
{
    SDL_Surface* temp = get_BMPSerface(file);
    flipHorizontal(temp);
    return SDL_CreateTextureFromSurface(renderer, temp);
}
void end()
{
    for (int i = 0; i < 4; i++)
        SDL_FreeSurface(d[i].orDick);
    SDL_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


#endif //ROTATEDICK_FUNCTIONS_H
