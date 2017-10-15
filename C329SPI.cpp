/*
 * :vi ts=4 sts=4 sw=4
 *
 * Copyright (c) Jonathan Burchmore
 */

#include <SPI.h>
#include "C329SPI.h"

C329SPI::C329SPI( int _CS, int _HOLD )
	   : CS( _CS ), HOLD( _HOLD ),
		 ack_count( 0 ), nack_count( 0 ),
		 last_error( 0x00 ),
		 picture_remaining( 0 )
{
}

bool C329SPI::Sync()
{
	int attempt;

	SPI.begin();
	pinMode( CS, OUTPUT );

	digitalWrite( CS,	HIGH );
	digitalWrite( HOLD,	LOW );

	// The C329-SPI User Manual says to repeat the SYNC command
	// up to 60 times with a 25ms delay until we receive an ACK
	// from the camera
    for ( attempt = 0; attempt < 60; attempt++ )
    {
		SendCommand( C329_COMMAND_SYNC );
		delay( 25 );

		if ( !VerifyACK( C329_COMMAND_SYNC ) )		continue;
		if ( ReadResponse() != C329_COMMAND_SYNC )	continue;

		SendCommand( C329_COMMAND_ACK, C329_COMMAND_SYNC, ack_count++ );
		return true;
    }

	return false;    
}

bool C329SPI::Initial( byte color_type, byte preview_res, byte jpeg_res )
{
	SendCommand( C329_COMMAND_INITIAL, 0x00, color_type, preview_res, jpeg_res );
	return VerifyACK( C329_COMMAND_INITIAL );
}

bool C329SPI::Quality( byte quality )
{
	SendCommand( C329_COMMAND_QUALITY, quality );
	return VerifyACK( C329_COMMAND_QUALITY );
}

bool C329SPI::Snapshot( byte snapshot_type )
{
	SendCommand( C329_COMMAND_SNAPSHOT, snapshot_type );
	return VerifyACK( C329_COMMAND_SNAPSHOT );
}

bool C329SPI::GetPicture_Start( byte picture_type, long *picture_size )
{
	byte data_type;
	byte length0, length1, length2;

	if ( picture_remaining > 0 )
	{
		while ( picture_remaining-- )
		{
			SPI.transfer( 0x00 );
		}
	}

	SendCommand( C329_COMMAND_GET_PIC, picture_type );

	if ( !VerifyACK( C329_COMMAND_GET_PIC ) )				return false;
	if ( ReadResponse( &data_type,
					   &length0,
					   &length1,
					   &length2 ) != C329_COMMAND_DATA )	return false;
	if ( data_type != picture_type )						return false;

	picture_remaining	= length0 | ( ( unsigned long ) length1 << 8 ) | ( ( unsigned long ) length2 << 16 );
	if ( picture_size )
	{
		*picture_size	= picture_remaining;
	}

	return true;
}

long C329SPI::GetPicture_Data( byte *data, long length )
{
	long offset;

	if ( picture_remaining <= 0 )
	{
		return -1;
	}

	digitalWrite( CS, LOW );

	for ( offset = 0; offset < length && picture_remaining > 0; offset++, picture_remaining-- )
	{
		data[ offset ] = transfer( 0x00 );
	}

	digitalWrite( CS, HIGH );

	return offset;
}

void C329SPI::SendCommand( byte command, byte param1, byte param2, byte param3, byte param4 )
{
    digitalWrite( CS, LOW );

    transfer( 0xFF );
    transfer( 0xFF );
    transfer( 0xFF );
    transfer( command );
    transfer( param1 );
    transfer( param2 );
    transfer( param3 );
    transfer( param4 );

    digitalWrite( CS, HIGH );
}

byte C329SPI::ReadResponse( byte *param1, byte *param2, byte *param3, byte *param4 )
{
	byte command, param;

    digitalWrite( CS, LOW );

    transfer( 0x00 );
    transfer( 0x00 );
    transfer( 0x00 );
    command	= transfer( 0x00 );
    param	= transfer( 0x00 );	if ( *param1 )						*param1		= param;
    param	= transfer( 0x00 );	if ( *param2 )						*param2		= param;
    param	= transfer( 0x00 );	if ( *param3 )						*param3		= param;
								if ( command == C329_COMMAND_NAK )	last_error	= param;
    param	= transfer( 0x00 );	if ( *param4 )						*param4		= param;

    digitalWrite( CS, HIGH );

	return command;
}

bool C329SPI::VerifyACK( byte command )
{
	byte ack_command;

	if ( ReadResponse( &ack_command ) != C329_COMMAND_ACK )	return false;
	if ( ack_command != command )							return false;

	return true;
}

byte C329SPI::transfer( byte out )
{
	if ( digitalRead( HOLD ) == HIGH )
	{
		while ( digitalRead( HOLD ) == HIGH )
		{
			;
		}
	}

	return SPI.transfer( out );
}
