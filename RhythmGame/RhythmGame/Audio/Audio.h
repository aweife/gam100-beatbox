#ifndef AUDIO_H
#define AUDIO_H

void Audio_Create();
void Audio_LoadSoundFromPath(const char *path, int i);
void Audio_Play();
void Audio_Update();
int Audio_CreateSpectrum(int maxBars);

// Spread out the grouping of frequecny in spectrum using logarithmic split
// 0 to 11 Hz
// 11 to 22 Hz
// 22 to 43 Hz
// 43 to 86 Hz
// 86 to 172 Hz
// 172 to 344 Hz
// 344 to 689 Hz
// 689 to 1378 Hz
// 1378 to 2756 Hz
// 2756 to 5512 Hz
// 5512 to 11025 Hz
// 11025 to 22050 Hz
void Audio_GetSpectrum(float *spectrum);

#endif 