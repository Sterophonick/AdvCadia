extern void playsound();
// === SOUND REGISTERS ===
// sound regs, partially following pin8gba's nomenclature
// --- square wave with sweep ---
#define REG_SND1SWEEP     *(vu16*)(REG_BASE+0x0060)
#define REG_SND1CNT       *(vu16*)(REG_BASE+0x0062)
#define REG_SND1FREQ      *(vu16*)(REG_BASE+0x0064)
// --- simple square wave ---
#define REG_SND2CNT       *(vu16*)(REG_BASE+0x0068)
#define REG_SND2FREQ      *(vu16*)(REG_BASE+0x006C)
// --- wave player ---
#define REG_SND3SEL       *(vu16*)(REG_BASE+0x0070)
#define REG_SND3CNT       *(vu16*)(REG_BASE+0x0072)
#define REG_SND3FREQ      *(vu16*)(REG_BASE+0x0074)
// --- noise generator ---
#define REG_SND4CNT       *(vu16*)(REG_BASE+0x0078)
#define REG_SND4FREQ      *(vu16*)(REG_BASE+0x007C)
// -- master sound controls ---
#define REG_SNDDMGCNT     *(vu16*)(REG_BASE+0x0080)
#define REG_SNDDSCNT      *(vu16*)(REG_BASE+0x0082)
#define REG_SNDSTAT       *(vu16*)(REG_BASE+0x0084)
#define REG_SNDBIAS       *(vu16*)(REG_BASE+0x0088)

/// === DEFINES ===

// --- REG_SND1SWEEP / REG_SOUND1CNT_L / REG_SG10_L (0400:0060) ---

// Function of sweep: R += (-1)^dir * (R>>shift)
#define _SSW_INC             0        // pitch up
#define SSW_DEC         0x0008        // pitch down

#define SSW_SHIFT_MASK  0x0007        // shift nr. NOT nr of shifts!
#define SSW_SHIFT_SHIFT      0
#define SSW_TIME_MASK   0x0070        // times in n/128 s
#define SSW_TIME_SHIFT       4

#define SSW_OFF         0x0008        // disable sweep altogether
#define SSW_BUILD(_shift, _dir, _time) \
    ( (((_time)&7)<<4) | ((_dir)<<3) | ((_shift)&7) )

// --- REG_SND1CNT / REG_SOUND1CNT_H / REG_SG10_H (0400:0062)
//  &  REG_SND2CNT / REG_SOUND2CNT_L / REG_SG20   (0400:0068)
//  &  REG_SND4CNT / REG_SOUND4CNT_L / REG_SG40   (0400:0068) ---

#define _SSQR_DUTY1_8        0        // #_______#_______
#define SSQR_DUTY1_4    0x0040        // ##______##______
#define SSQR_DUTY1_2    0x0080        // ####____####____
#define SSQR_DUTY3_4    0x00C0        // ######__######__
#define _SSQR_ENV_DEC        0
#define SSQR_ENV_INC    0x0800

#define SSQR_LEN_MASK   0x003F        // (w) length= (64-len)
#define SSQR_LEN_SHIFT       0
#define SSQR_DUTY_MASK  0x00C0
#define SSQR_DUTY_SHIFT      6
#define SSQR_TIME_MASK  0x0700        // 0: env off;
#define SSQR_TIME_SHIFT      8
#define SSQR_IVOL_MASK  0xF000
#define SSQR_IVOL_SHIFT     12

#define SSQR_ENV_BUILD(_ivol, _dir, _time)    \
    ( (((_time)&7)<<8) | ((_dir)<<11) | ((_ivol)<<12) )

#define SSQR_BUILD(_ivol, _dir, _step, _duty, _len)    \
    ( SSQR_ENV_BUILD(_ivol,_dir,_step) | (((_duty)&3)<<6) | ((_len)&63) )


// --- REG_SND1FREQ / REG_SOUND1CNT_X / REG_SG11 (0400:0064)
//  &  REG_SND2FREQ / REG_SOUND2CNT_H / REG_SG21 (0400:006C)
//  &  REG_SND3FREQ / REG_SOUND3CNT_X / REG_SG31 (0400:0074) ---

// Equations between frequency f and rate r
//   f(r)= 2^17/(2048-r) Hz
//   r(f) = 2048 - 2^17/f
#define _SFREQ_HOLD           0
#define SFREQ_TIMED      0x4000        // uses length from REGSNDxCNT
#define SFREQ_RESET      0x8000        // (w) reset sound

#define SFREQ_RATE_MASK  0x07FF        // (w)
#define SFREQ_RATE_SHIFT      0

#define SFREQ_BUILD(_rate, _timed, _reset)    \
    ( ((_rate)&0x7FF) | ((_timed)<<14) | ((_reset)<<15) )

// --- REG_SNDDMGCNT / REG_SOUNDCNT_L / REG_SGCNT0_L (0400:0080) ---

// base defines
#define SDMG_SQR1_ON    0x01
#define SDMG_SQR2_ON    0x02
#define SDMG_WAVE_ON    0x04
#define SDMG_NOISE_ON   0x08

// 'real' bit flags
#define SDMG_LSQR1_ON   0x0100
#define SDMG_LSQR2_ON   0x0200
#define SDMG_LWAVE_ON   0x0400
#define SDMG_LNOISE_ON  0x0800
#define SDMG_RSQR1_ON   0x1000
#define SDMG_RSQR2_ON   0x2000
#define SDMG_RWAVE_ON   0x4000
#define SDMG_RNOISE_ON  0x8000

#define SDMG_LVOL_MASK  0x0007
#define SDMG_LVOL_SHIFT      0
#define SDMG_RVOL_MASK  0x0070
#define SDMG_RVOL_SHIFT      4

#define SDMG_BUILD(_lmode, _rmode, _lvol, _rvol)    \
    ( ((_lvol)&7) | (((_rvol)&7)<<4) | ((_lmode)<<8) | ((_rmode)<<12) )

#define SDMG_BUILD_LR(_mode, _vol) SDMG_BUILD(_mode, _mode, _vol, _vol)

// --- REG_SNDDSCNT / REG_SOUNDCNT_H / REG_SGCNT0_H (0040:0082) ---
// DMG sound ratio
#define _SDS_DMG25        0
#define SDS_DMG50    0x0001
#define SDS_DMG100   0x0002
// DSound ratios
#define _SDS_A50          0
#define SDS_A100     0x0004
#define _SDS_B50          0
#define SDS_B100     0x0008
// DSound A flags
#define SDS_AR       0x0100
#define SDS_AL       0x0200
#define _SDS_ATMR0        0
#define SDS_ATMR1    0x0400
#define SDS_ARESET   0x0800
// DSound B flags
#define SDS_BR       0x1000
#define SDS_BL       0x2000
#define _SDS_BTMR0        0
#define SDS_BTMR1    0x4000
#define SDS_BRESET   0x8000

// --- REG_SNDSTAT / REG_SOUNDCNT_X / REG_SGCNT1 (0400:0084) ---
#define SSTAT_SQR1  0x0001    // (r)
#define SSTAT_SQR2  0x0002    // (r)
#define SSTAT_WAVE  0x0004    // (r)
#define SSTAT_NOISE 0x0008    // (r)
#define _SSTAT_OFF       0
#define SSTAT_ON    0x0080
#define SSTAT_OFF   0x0000
