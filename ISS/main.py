import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf

MIDIFROM = 24
MIDITO = 108
SKIP_SEC = 0.25
HOWMUCH_SEC = 0.5
WHOLETONE_SEC = 2
TONE_ONE = 31
TONE_TWO = 79
TONE_THREE = 82
howmanytones = MIDITO - MIDIFROM + 1

tones = np.arange(MIDIFROM, MIDITO+1)
orig, Fs = sf.read("klavir.wav")

N = int(Fs * HOWMUCH_SEC)
Nwholetone = int(Fs * WHOLETONE_SEC)
n = np.arange(0, N)
xall = np.zeros((MIDITO+1, N))
samplefrom = int(SKIP_SEC * Fs)
sampleto = samplefrom + N
t = n / Fs
for tone in tones:
    x = orig[samplefrom:sampleto]
    x = x - np.mean(x)
    xall[tone] = x
    samplefrom += Nwholetone
    sampleto += Nwholetone


sf.write("audio/a_orig.wav", xall[TONE_ONE], Fs)
sf.write("audio/b_orig.wav", xall[TONE_TWO], Fs)
sf.write("audio/c_orig.wav", xall[TONE_THREE], Fs)

samplefrom = int(((TONE_ONE - MIDIFROM) * Fs * WHOLETONE_SEC) + (SKIP_SEC * Fs))
sampleto = int(samplefrom + Fs/49.00 * 3)

_, ax = plt.subplots(3,1)

ax[0].plot(orig[samplefrom : sampleto])
ax[0].set_xlabel('t')
ax[0].set_ylabel('x(t)')
ax[0].set_title('Segment signalu 31')
ax[0].grid(alpha=0.5, linestyle='--')

samplefrom = int(((TONE_TWO - MIDIFROM) * Fs * WHOLETONE_SEC) + (SKIP_SEC * Fs))
sampleto = int(samplefrom + Fs/783.99 * 3)

ax[1].plot(orig[samplefrom : sampleto])
ax[1].set_xlabel('t')
ax[1].set_ylabel('x(t)')
ax[1].set_title('Segment signalu 79')
ax[1].grid(alpha=0.5, linestyle='--')

samplefrom = int(((TONE_THREE - MIDIFROM) * Fs * WHOLETONE_SEC) + (SKIP_SEC * Fs))
sampleto = int(samplefrom + Fs/932.33 * 3)

ax[2].plot(orig[samplefrom : sampleto])
ax[2].set_xlabel('t')
ax[2].set_ylabel('x(t)')
ax[2].set_title('Segment signalu 82')
ax[2].grid(alpha=0.5, linestyle='--')

plt.tight_layout()

plt.show()