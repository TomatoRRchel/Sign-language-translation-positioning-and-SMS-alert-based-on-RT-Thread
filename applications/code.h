#ifndef _CODE_H_
#define _CODE_H_

/* Includes ------------------------------------------------------------------*/
/*typedef unsigned char rt_uint8_t;
typedef unsigned short rt_uint16_t;
typedef unsigned int u32;
typedef char s8;
typedef short rt_int16_t;
typedef int s32;    */


rt_uint16_t UnicodeToGB2312(rt_uint16_t unicode);
rt_uint16_t GB2312ToUnicode(rt_uint16_t data);
void GB2312ToUnicode_Str(rt_uint16_t * const pOutUnicodeStr,rt_uint8_t * const pGB2312Str);
rt_int16_t NetworkByteOrder(rt_uint8_t * const pOut,rt_uint16_t * const pIn,rt_uint16_t  const nByte);
#endif
