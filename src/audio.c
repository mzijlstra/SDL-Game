#include "audio.h"
#include "asset.h"
#include "linkedlist.h"

extern Asset asset;

static SDL_AudioDeviceID audioDeviceID;
static SDL_AudioSpec audioSpec;
static LinkNode sfxList;

void initAudio() {
    SDL_AudioSpec want;
    want.freq = 44100;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.silence = 0;
    want.samples = 4096;
    want.padding = 0;
    want.size = 0;
    want.userdata = &sfxList;
    want.callback = &audioCallback;

    audioDeviceID = SDL_OpenAudioDevice(NULL, 0, &want, &audioSpec,
                                        SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (audioDeviceID == 0) {
        SDL_Log("Failed to open audio %s", SDL_GetError());
        exit(1);
    }

    // setup the sfxList
    sfxList.next = &sfxList;
    sfxList.prev = &sfxList;
    sfxList.data = NULL; // this is the sentinel node

    // start playback
    SDL_PauseAudioDevice(audioDeviceID, 0);
}

void audioCallback(void *userdata, uint8_t *stream, int len) {
    LinkNode *list = (LinkNode *)userdata;
    // first ensure silence in the stream
    SDL_memset(stream, audioSpec.silence, len);

    for (int i = 0; i < len; i++) {
        int64_t val = audioSpec.silence; // should be 0 since AUDIO_S16

        // very basic mixing of sound effects (simply add the waves)
        LinkNode *iter = list->next;
        LinkNode *next = NULL;
        while (iter != &sfxList) {
            next = iter->next;
            SND *sfx = (SND *)iter->data;
            val += sfx->buf[sfx->idx++];

            // remove this sound effect if it's done
            if (sfx->idx >= sfx->len) {
                SDL_free(sfx);
                removeLink(iter);
            }
            iter = next;
        }

        // make sure we don't go beyond int16_t boundaries
        if (val > INT16_MAX) {
            val = INT16_MAX;
        } else if (val < INT16_MIN) {
            val = INT16_MIN;
        }

        // set the value into the stream and go to next
        stream[i] = val;
    }
}

void playSound(WAV *const wav) {
    SND *sfx = SDL_malloc(sizeof(SND));
    sfx->spec = &wav->spec;
    sfx->buf = wav->buf;
    sfx->len = wav->len;
    sfx->idx = 0;
    addLink(&sfxList, sfx);

    // SDL_QueueAudio(audioDeviceID, wav->buf, wav->len);
}