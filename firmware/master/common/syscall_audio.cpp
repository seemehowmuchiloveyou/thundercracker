/*
 * Thundercracker Firmware -- Confidential, not for redistribution.
 * Copyright <c> 2012 Sifteo, Inc. All rights reserved.
 */

/*
 * Syscalls for audio playback.
 */

#include <sifteo/abi.h>
#include "audiomixer.h"
#include "svmmemory.h"
#include "svmruntime.h"

extern "C" {

uint32_t _SYS_audio_play(const struct _SYSAudioModule *mod, _SYSAudioChannelID ch, enum _SYSAudioLoopType loop)
{
    _SYSAudioModule modCopy;
    if (SvmMemory::copyROData(modCopy, mod)) {
        return AudioMixer::instance.play(&modCopy, ch, loop);
    }

    SvmRuntime::fault(F_SYSCALL_ADDRESS);
    return false;
}

uint32_t _SYS_audio_isPlaying(_SYSAudioChannelID ch)
{
    return AudioMixer::instance.isPlaying(ch);
}

void _SYS_audio_stop(_SYSAudioChannelID ch)
{
    AudioMixer::instance.stop(ch);
}

void _SYS_audio_pause(_SYSAudioChannelID ch)
{
    AudioMixer::instance.pause(ch);
}

void _SYS_audio_resume(_SYSAudioChannelID ch)
{
    AudioMixer::instance.resume(ch);
}

int32_t _SYS_audio_volume(_SYSAudioChannelID ch)
{
    return AudioMixer::instance.volume(ch);
}

void _SYS_audio_setVolume(_SYSAudioChannelID ch, int32_t volume)
{
    AudioMixer::instance.setVolume(ch, volume);
}

uint32_t _SYS_audio_pos(_SYSAudioChannelID ch)
{
    return AudioMixer::instance.pos(ch);
}

}  // extern "C"
