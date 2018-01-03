#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
typedef struct _Audios Audios;

typedef enum { PID, NAME } audio_key_t;

//TODO: Decide whether if this is static or not.
static Audios *playing_audios;

/*
 * Plays an audio file, starting a process for it.
 *
 * If the song is repeated, it adds the pid to the global audios variable.
 * Repeating audios will be stopped by killing the process associated to it.
 * If there's no space on playing_audios, the audio will not be played.
 *
 * Input:
 *    char *filename. Path to audio file.
 *    bool repeating. True if audio will be repeated.
 * Output:
 *    pid_t process_id. Returns the process id of the song.
 *                      Returns 0 on error.
 */
pid_t play_audio(char *, bool);

/*
 * Kills the process associated with the key given.
 *
 * Please notice that killing the audio giving the pid as key is more safe.
 * If for any reason there are two different audios with the same name, the
 * first encountered (last played) will be stopped.
 *
 * Input:
 *    void *key. Key, can be a string or a pointer to a pid_t.
 *    audio_key_t type. PID or NAME, depending on key.
 * Output:
 *    int err_hanlder. Returns UINT_ERROR on error.
 */
int stop_audio(void *, audio_key_t);

/*
 * Methods for Audios ADT.
 */

/*
 * This returns a pointer to global variable playing_audios, once alloc'd it.
 */
 Audios *audios_new();
 void audios_destroy();
