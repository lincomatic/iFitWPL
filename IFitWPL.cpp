/*
 * Copyright (c) 2016 lincomatic
 *
 * Many thanks to Stefano Livi, who provided the initial decoding of the WPL
 * format
 *
 * This file is part of iFitWPL.

 * iFitWPL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.

 * iFitWPL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with iFitWPL; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.

 */

#include <cstdint>
#include <cstdio>
#include <cstring>

#define WPL_RECTYPE_TITLE 0x0603
#define WPL_RECTYPE_DESC  0x0607
#define WPL_RECTYPE_BASENAME 0x0602
#define WPL_RECTYPE_PCIF 0x2c01
#define WPL_RECTYPE_FCIF 0x2c02
#define WPL_RECTYPE_MCIF 0x2c03
#define WPL_RECTYPE_BCIF 0x2c04
#define WPL_RECTYPE_WAV1 0x0e56
#define WPL_RECTYPE_WAV2 0x0e58
#define WPL_RECTYPE_WORKOUT_START 0x0d02
//maximum inclination in percent multiplied by 10 (3.5% is coded 35)
#define WPL_RECTYPE_MAX_INCLINATION 0x0437
//maximum velocity in miles/hours multiplied by 10 (3.1 m/h is coded 31)
#define WPL_RECTYPE_MAX_VELOCITY 0x043b
// total time in seconds
#define WPL_RECTYPE_TOTAL_TIMEA 0x040a
// total time in seconds
#define WPL_RECTYPE_TOTAL_TIMEB 0x040d
// total distance in miles multiplied by 1,000
#define WPL_RECTYPE_TOT_DISTA 0x046c
// total distance in miles multiplied by 1,000
#define WPL_RECTYPE_TOT_DISTB 0x043a
// calories
#define WPL_RECTYPE_CALORIES 0x046a
// vertical distance in feet
#define WPL_RECTYPE_VERTICAL_DISTANCE 0x0472
// set velocity in miles/hours multiplied by 10
#define WPL_RECTYPE_SET_VELOCITY 0x042f
//set inclination in percent multiplied by 10
#define WPL_RECTYPE_SET_INCLINATION 0x042d
// set end-of-time in seconds
#define WPL_RECTYPE_SET_ENDOFTIME 0x110e


class WplRecord {
  uint8_t contentLen;
  uint16_t recType;
  uint8_t *data;
  uint8_t checkByte;

public:
  WplRecord();
  ~WplRecord();

  int Read(FILE *fp);
  void Dump(FILE *ofp);
};

WplRecord::WplRecord()
{
  data = NULL;
}

WplRecord::~WplRecord()
{
  if (data) delete [] data;
}

int WplRecord::Read(FILE *ifp)
{
  int rc = -1;
  if (fread(&contentLen,1,1,ifp) != 1) return rc;
  if (contentLen == 0) {
    return 1; // end of file record
  }
  uint8_t u8;
  fread(&u8,1,1,ifp);
  recType = u8 << 8;
  fread(&u8,1,1,ifp);
  recType |= u8;
  data = new uint8_t[contentLen-1];
  fread(data,contentLen-1,1,ifp);
  fread(&checkByte,1,1,ifp);
  uint8_t chk = 0;
  for (int i=0;i < contentLen-1;i++) chk += data[i];
  chk += recType >> 8;
  chk += recType & 0xff;
  chk += checkByte;
  chk += contentLen;
  if (!chk) rc = 0;

  return rc;
}

void WplRecord::Dump(FILE *ofp)
{
  int i;
  float f;
  uint32_t u32;

  fprintf(ofp,"----\n");
  fprintf(ofp,"%02X\n",contentLen);
  fprintf(ofp,"%04X\n",recType);
  for (i=0;i < contentLen-1;i++) fprintf(ofp," %02X",data[i]);
  fprintf(ofp,"\n ");
  for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",(char)data[i]);
  fprintf(ofp,"\n%02X\n",checkByte);

  switch(recType) {
  case WPL_RECTYPE_TITLE:
    fprintf(ofp,"Title: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_DESC:
    fprintf(ofp,"Description: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_BASENAME:
    fprintf(ofp,"Base name: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_PCIF:
    fprintf(ofp,"PCIF: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_BCIF:
    fprintf(ofp,"BCIF: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_FCIF:
    fprintf(ofp,"FCIF: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_MCIF:
    fprintf(ofp,"MCIF: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_WAV1:
    fprintf(ofp,"WAV1: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_WAV2:
    fprintf(ofp,"WAV2: ");
    for (i=0;i < contentLen-1;i++) fprintf(ofp,"%c",data[i]);
    fprintf(ofp,"\n");
    break;
	
  case WPL_RECTYPE_WORKOUT_START:
    fprintf(ofp,"Workout start: ");
    fprintf(ofp,"%02x\n",data[0]);
    break;
	
  case WPL_RECTYPE_MAX_INCLINATION:
    fprintf(ofp,"Max inclination: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    f = (float)u32;
    f /= 10.0;
    fprintf(ofp,"%0.2f%%\n",f);
    break;

  case WPL_RECTYPE_MAX_VELOCITY:
    fprintf(ofp,"Max velocity: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    f = (float)u32;
    f /= 10.0;
    fprintf(ofp,"%0.2f mph\n",f);
    break;
  case WPL_RECTYPE_TOTAL_TIMEA:
    fprintf(ofp,"Total time: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    fprintf(ofp,"%lu sec\n",u32);
    break;
    
  case WPL_RECTYPE_TOTAL_TIMEB:
    fprintf(ofp,"Total time: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    fprintf(ofp,"%lu sec\n",u32);
    break;
    
  case WPL_RECTYPE_TOT_DISTA:
    fprintf(ofp,"Total dist: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    f = (float)u32;
    f /= 1000.0;
    fprintf(ofp,"%0.4f mph\n",f);
    break;
    
  case WPL_RECTYPE_TOT_DISTB:
    fprintf(ofp,"Total dist: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    f = (float)u32;
    f /= 1000.0;
    fprintf(ofp,"%0.4f mph\n",f);
    break;
    
  case WPL_RECTYPE_CALORIES:
    fprintf(ofp,"Calories: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    fprintf(ofp,"%lu cal\n",u32);
    fprintf(ofp,"\n");
    break;
    
  case WPL_RECTYPE_VERTICAL_DISTANCE:
    fprintf(ofp,"Vert dist: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    fprintf(ofp,"%lu ft\n",u32);
    break;
    
  case WPL_RECTYPE_SET_VELOCITY:
    fprintf(ofp,"Set velocity: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    f = (float)u32;
    f /= 10.0;
    fprintf(ofp,"%0.2f mph\n",f);
    break;
    
  case WPL_RECTYPE_SET_INCLINATION:
    fprintf(ofp,"Set Inclination: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    f = (float)u32;
    f /= 10.0;
    fprintf(ofp,"%0.2f%%\n",f);
    break;
    
  case WPL_RECTYPE_SET_ENDOFTIME:
    fprintf(ofp,"Set end-of-time: ");
    u32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    fprintf(ofp,"%lu sec\n",u32);
    break;

  default:
    fprintf(ofp,"(UNKNOWN RECORD TYPE)\n");
  }
}


int main(int argc, char *argv[])
{
  const char *ifn = argv[1];
  FILE *ifp = fopen(ifn,"rb");
  char ofn[256];
  strcpy(ofn,ifn);
  strcat(ofn,".dump.txt");
  FILE *ofp = fopen(ofn,"w");
  
  while (1) {
    WplRecord wpl;
    if (wpl.Read(ifp)) break;
    wpl.Dump(ofp);
  }

  fflush(ofp);
  fclose(ofp);
  return 0;
}

