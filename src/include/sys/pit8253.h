#ifndef __PIT_8253
#define __PIT_8253

#ifndef PIT_8253_DEFAULT_OUT_FREQ
/* set default output frequency to 100HZ */
#define PIT_8253_DEFAULT_OUT_FREQ 0x64
#endif /* PIT_8253_DEFAULT_OUT_FREQ */


#define PIT_8253_INPUT_FREQ 1193180

#define PIT_8253_CMD_PORT           0x43
#define PIT_8253_CHANNEL0_DATA_PORT 0x40

/* set the PIT to repeating mode command, a mode in which
 * the PIT automatically refreshes the counter once it reaches (1)
*/
#define PIT_8253_REPEAT_MOD_CMD 0x36


extern void pit8253_init(void);

#endif /* __PIT_8253 */
