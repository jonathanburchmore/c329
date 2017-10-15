/*
 * :vi ts=4 sts=4 sw=4
 *
 * Copyright (c) Jonathan Burchmore
 */

#include "c329.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "C329SPI.h"

#define SWITCH		14
#define LED_GREEN	15
#define LED_RED		16

Adafruit_ST7735	tft( 10, 7, 6 );
C329SPI			c329;

void setup()
{
	pinMode( LED_GREEN,	OUTPUT );
	pinMode( LED_RED,	OUTPUT );

	digitalWrite( SWITCH,		HIGH );
	digitalWrite( LED_GREEN,	LOW );
	digitalWrite( LED_RED,		LOW );

	tft.initR( INITR_REDTAB );
	tft.setRotation( 1 );
	tft.fillScreen( ST7735_BLACK );

	if ( c329.Sync() )
	{
		digitalWrite( LED_GREEN, HIGH );
	}
	else
	{
		digitalWrite( LED_RED, HIGH );
		for ( ;; ) ;
	}
}

void loop()
{
	byte rgb[ 3 ];
	long picture_size;

	switch ( digitalRead( SWITCH ) )
	{
		case HIGH	: digitalWrite( LED_RED, LOW );	break;
		case LOW	:
		{
			delay( 10 );
			if ( digitalRead( SWITCH ) != LOW )
			{
				break;
			}

			digitalWrite( LED_RED, HIGH );
			c329.Initial( C329_COLOR_RGB24, C329_PREVIEW_RES_160x128, C329_JPEG_RES_640x480 );

			if ( c329.GetPicture_Start( C329_GET_PIC_TYPE_PREVIEW, &picture_size ) )
			{
				while ( c329.GetPicture_Data( rgb, 3 ) > 0 )
				{
					tft.pushColor( tft.Color565( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] ) );
				}

				digitalWrite( LED_RED, LOW );
			}

			while ( digitalRead( SWITCH ) == LOW )
			{
				;
			}

			break;
		}
	}
}
