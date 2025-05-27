#pragma once

#include <windows.h>
#include <string.h>

#include "Direct3D.h"

class ClassMAPDATA
{
public:

	int	**DATA,**ATT;
	int		WWIDTH,WHEIGHT,WIDTH,HEIGHT,TPH,TPW;
	
	//   Tile 마스크..  11111111000000000000000000000000 (  8비트  0x000000ff )
	void	SetTile(int x,int y,int tile)
			{
				if (y>=HEIGHT || y< 0 || x<0 || x>=WIDTH) return;
				DATA[y][x]&=0xfffff000;
				DATA[y][x]|=tile;
			}

	int		GetTile(int x,int y)
			{
				if (y>=HEIGHT || y< 0 || x<0 || x>=WIDTH) return -1;
				return (DATA[y][x]&0x00000fff);
			}

	/*   속성 00000000111100000000000000000000 (  4비트  0x00000f00 )
	void	SetAttr(int x,int y,int attr)
			{
				if (y>=HEIGHT || y< 0 || x<0 || x>=WIDTH) return;
				DATA[y][x]&=0xfff00fff;
				DATA[y][x]|=((int)attr<<12);
			}

	int		GetAttr(int x,int y)
			{
				if (y>=HEIGHT || y< 0 || x<0 || x>=WIDTH) return -1;
				return ((DATA[y][x]&0x000ff000)>>12);
			}
	
	*/
	//  속성 00000000111100000000000000000000 (  4비트  0x00000f00 )
	void	SetFTile(int x,int y,int attr)
			{
				if (y>=HEIGHT || y< 0 || x<0 || x>=WIDTH) return;
				DATA[y][x]&=0x000fffff;
				DATA[y][x]|=((int)attr<<20);
			}

	int		GetFTile(int x,int y)
			{
				if (y>=HEIGHT || y< 0 || x<0 || x>=WIDTH) return 0;
				return ((DATA[y][x]&0xfff00000)>>20);
			}

	void	SetAttr(int x,int y,int attr)
			{
				if (y>=(HEIGHT*4) || y< 0 || x<0 || x>=(WIDTH*4)) return;
				ATT[y][x] = attr;
			}

	int		GetAttr(int x,int y)
			{
				if (y>=(HEIGHT*4) || y< 0 || x<0 || x>=(WIDTH*4)) return -1;
				return ATT[y][x];
			}

			ClassMAPDATA() {WIDTH=50;HEIGHT=50;DATA=NULL;ATT=NULL;}

			~ClassMAPDATA()
			{
				for (int i=0;i<HEIGHT;i++) 
				{
					if(DATA[i])
					{
						SAFE_DELETE_ARRAY_(DATA[i]);
					}
				}
				SAFE_DELETE_ARRAY_(DATA);

				for (int i=0;i<HEIGHT*4;i++) 
				{
					if(ATT[i])
					{
						SAFE_DELETE_ARRAY_(ATT[i]);
					}
				}
				SAFE_DELETE_ARRAY_(ATT);
			}

};
