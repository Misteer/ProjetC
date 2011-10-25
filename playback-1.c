/* This example reads standard from input and writes to the default PCM device for 5 seconds of data.*/

/* Use the newer ALSA API */

#define ALSA_PCM_NEW_HW_PARAMS_API

#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <alsa/asoundlib.h>
#include "playback.h"
#include <string.h>

int makingSound(char*buffer) {
  long loops;
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;


  /* Open PCM device for playback. */
  rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
  if (rc < 0) {
    fprintf(stderr,"unable to open pcm device: %s\n",snd_strerror(rc));
    exit(1);
  }

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 2);

  /* 44100 bits/second sampling rate (CD quality) */
  val = 44100;
  snd_pcm_hw_params_set_rate_near(handle,params,&val,&dir);

  /* Set period size to 32 frames. */
  frames = 32;
  snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  size = frames * 4; /* 2 bytes/sample, 2 channels */

  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(params,&val, &dir);
  /* 5 seconds in microseconds divided by *period time */
  loops = 5000000 / val;
     int taille = strlen(buffer);
     char buffer1[129];
 while(taille > 130){// while (loops > 0) {
    loops--;
          if(taille > 130){
            memcpy(buffer1,buffer,128);
            buffer = buffer + 128;
            taille = taille - 128;
            rc = snd_pcm_writei(handle, buffer1, frames);
            if (rc == -EPIPE) {
              /* EPIPE means underrun */
              fprintf(stderr, "underrun occurred\n");
              snd_pcm_prepare(handle);
              return -1;
            } else if (rc < 0) {
              fprintf(stderr,"error from writei: %s\n",snd_strerror(rc));
                return -1;
            } else if (rc != (int)frames) {
              fprintf(stderr,"short write, write %d frames\n", rc);
                return -1;
            }
            }
  }

  snd_pcm_drain(handle);
  snd_pcm_close(handle);


  return 0;
}

