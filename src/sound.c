//Sound conversion
#include <libheart.h>
#include "lang.h"
#include "emu.h"
#include "sound.h"
#include "vram.h"
#include "s2650.h"
#include "vmachine.h"

void playsound()
{
    if
    (    ((memory[A_PITCH] ) &   127)
     &&  ((memory[A_VOLUME]) &     7)
    )
    {   switch ((memory[A_VOLUME]) & 0x18)
        {
        case 0x08: // tone only
            REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR2_ON                , memory[A_VOLUME] & 7);
            REG_SND2FREQ  = SFREQ_RESET | (2048 - (((memory[A_PITCH] & 127) + 1) * 17));
        acase 0x10: // noise only
            REG_SNDDMGCNT = SDMG_BUILD_LR(               SDMG_NOISE_ON, memory[A_VOLUME] & 7);
            REG_SND4FREQ  = 0x80A3;
/* Authentic noise frequency for PITCH of $60 is 81.17526Hz.
   Our approximation is 85.3' Hz. */
        acase 0x18: // both
            REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR2_ON | SDMG_NOISE_ON, memory[A_VOLUME] & 7);
            REG_SND2FREQ  = SFREQ_RESET | (2048 - (((memory[A_PITCH] & 127) + 1) * 17));
            REG_SND4FREQ  = 0x80A3;
        acase 0x00: // neither
            REG_SNDDMGCNT = SDMG_BUILD_LR(0                           , 0                   );
    }   }
    else
    {    REG_SNDDMGCNT    = SDMG_BUILD_LR(0                           , 0                   );
}   }