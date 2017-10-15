#include <Arduino.h>

#ifndef __C329SPI_h
#define __C329SPI_h

#define C329_COMMAND_PAD						0xFF
#define C329_COMMAND_INITIAL					0x01
#define C329_COMMAND_GET_PIC					0x04
#define C329_COMMAND_SNAPSHOT					0x05
#define C329_COMMAND_RESET						0x08
#define C329_COMMAND_POWER_OFF					0x09
#define C329_COMMAND_DATA						0x0A
#define C329_COMMAND_SYNC						0x0D
#define C329_COMMAND_ACK						0x0E
#define C329_COMMAND_NAK						0x0F
#define C329_COMMAND_QUALITY					0x10

#define C329_COLOR_Y4							0x02
#define C329_COLOR_Y8							0x03
#define C329_COLOR_RGB12						0x05
#define C329_COLOR_RGB16						0x06
#define C329_COLOR_RGB24						0x08
#define C329_COLOR_YUV16						0x09
#define C329_COLOR_JPEG							0x87

#define C329_PREVIEW_RES_80x60					0x01
#define C329_PREVIEW_RES_88x72					0x02
#define C329_PREVIEW_RES_160x120				0x03
#define C329_PREVIEW_RES_176x144				0x04
#define C329_PREVIEW_RES_320x240				0x05
#define C329_PREVIEW_RES_352x288				0x06
#define C329_PREVIEW_RES_640x480				0x07
#define C329_PREVIEW_RES_80x64					0x08
#define C329_PREVIEW_RES_128x96					0x09
#define C329_PREVIEW_RES_128x128				0x0A
#define C329_PREVIEW_RES_160x128				0x0B

#define C329_JPEG_RES_80x60						0x01
#define C329_JPEG_RES_88x72						0x02
#define C329_JPEG_RES_160x120					0x03
#define C329_JPEG_RES_176x144					0x04
#define C329_JPEG_RES_320x240					0x05
#define C329_JPEG_RES_352x288					0x06
#define C329_JPEG_RES_640x480					0x07
#define C329_JPEG_RES_80x64						0x08
#define C329_JPEG_RES_128x96					0x09
#define C329_JPEG_RES_128x128					0x0A
#define C329_JPEG_RES_160x128					0x0B

#define C329_SNAPSHOT_TYPE_JPEG					0x00
#define C329_SNAPSHOT_TYPE_UNCOMPRESSED			0x01

#define C329_GET_PIC_TYPE_SNAPSHOT				0x01
#define C329_GET_PIC_TYPE_PREVIEW				0x02
#define C329_GET_PIC_TYPE_SERIAL_FLASH			0x03
#define C329_GET_PIC_TYPE_JPEG					0x05
#define C329_GET_PIC_TYPE_PLAYBACK				0x06

#define C329_RESET_TYPE_ALL						0x00
#define C329_RESET_TYPE_STATE_MACHINE			0x01

#define C329_DATA_TYPE_REGISTER					0x00
#define C329_DATA_TYPE_SNAPSHOT					0x01
#define C329_DATA_TYPE_PREVIEW					0x02
#define C329_DATA_TYPE_JPEG_PREVIEW				0x04
#define C329_DATA_TYPE_PLAYBACK					0x05

#define C329_QUALITY_LEVEL_BEST					0x00
#define C329_QUALITY_LEVEL_BETTER				0x01
#define C329_QUALITY_LEVEL_NORMAL				0x02

class C329SPI
{
	int		CS, HOLD;
	byte	ack_count, nack_count;
	byte	last_error;
	long	picture_remaining;

public:

	C329SPI( int _CS = 8, int _HOLD = 9 );

public:

	bool Sync();
	bool Initial( byte color_type, byte preview_res, byte jpeg_res );
	bool Quality( byte quality );

public:

	bool Snapshot( byte snapshot_type = C329_SNAPSHOT_TYPE_JPEG );

	bool GetPicture_Start( byte picture_type, long *picture_size = 0 );
	long GetPicture_Data( byte *data, long length );

public:

	byte LastError()	{ return last_error; }

protected:

	void SendCommand( byte command, byte param1 = 0x00, byte param2 = 0x00, byte param3 = 0x00, byte param4 = 0x00 );
	byte ReadResponse( byte *param1 = NULL, byte *param2 = NULL, byte *param3 = NULL, byte *param4 = NULL );
	bool VerifyACK( byte command );

	byte transfer( byte out );
};

#endif
