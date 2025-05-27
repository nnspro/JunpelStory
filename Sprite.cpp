#include "MapOutput.h"
#include "MiniMap.h"
#include "Sprite.h"

#include <stdio.h>

cSprWnd::cSprWnd()
{
	int ld = 0;
	FILE *usedata;

	ZeroMemory(Name,13);

	if(!(usedata = fopen("txt/user/user.txt","rt")))
	{
		ld = 1;
		
		CopyMemory(Name,"아디",4);
		m_pMapViewWnd->game_map_stage = 0;

		usedata = fopen("txt/user/init.txt","rt");
	}

	if(ld == 0)
	{
		fscanf(usedata,"%s",Name);
		fscanf(usedata,"%d",&m_pMapViewWnd->game_map_stage);
		
		fscanf(usedata,"%d",&Mp);
		fscanf(usedata,"%d",&Hp);
	}
	
	fscanf(usedata,"%d",&Level);
	fscanf(usedata,"%d",&Str);
	fscanf(usedata,"%d",&Dex);
	fscanf(usedata,"%d",&Int);
	fscanf(usedata,"%d",&Mana);
	fscanf(usedata,"%d",&Hana);
	fscanf(usedata,"%d",&Kyunghum);
	fscanf(usedata,"%d",&Money);
	fscanf(usedata,"%d",&stackPoint);

	for(int p=0;p<8;p++)
	{
		fscanf(usedata,"%d",&Interface_[p].itemnum);
		fscanf(usedata,"%d",&Interface_[p].num);
		fscanf(usedata,"%d",&Interface_[p].checked);
	}
	for(int p=0;p<10;p++)
	{
		fscanf(usedata,"%d",&User_Itm_Att[p].itemnum);
		fscanf(usedata,"%d",&User_Itm_Att[p].num);
		fscanf(usedata,"%d",&User_Itm_Att[p].checked);
	}
	for(int p=0;p<5;p++)
	{
		fscanf(usedata,"%d",&User_Itm_Booster[p].itemnum);
		fscanf(usedata,"%d",&User_Itm_Booster[p].num);
		fscanf(usedata,"%d",&User_Itm_Booster[p].checked);
	}
	for(int p=0;p<30;p++)
	{
		fscanf(usedata,"%d",&User_Itm_Etc[p].itemnum);
		fscanf(usedata,"%d",&User_Itm_Etc[p].num);
		fscanf(usedata,"%d",&User_Itm_Etc[p].checked);
	}
	fclose(usedata);

	totalHp          = (Hana*10)+((Level-1)*6);
	totalMp          = (Mana*10)+((Level-1)*3);
	if(ld == 1)
	{
		Mp = totalMp;
		Hp = totalHp;
	}
	//Kyunghum              = 0;

	//( x - 1 )^2 * ( x^2 - 13x + 82 )
	int l = Level+1;
	totalKyunghum         = (int)(pow((float)Level,2) * (pow((float)l,2)-13*l+82));
	view = Turbo          = 0;
	TurboDelayTime        = 30;
	
	Hit_Spr_x = Hit_Spr_y = -1;
	ZeroMemory(damagetxt,5);

	SHOP = FALSE;

	m_pPet = NULL;
}

cSprWnd::~cSprWnd()
{
	FILE *save_userdata = fopen("txt/user/user.txt","wt");

	fprintf(save_userdata,"%s\n",Name);
	if(m_pMapViewWnd->quest == 0)
	{
		if(AniCount != 8)
			fprintf(save_userdata,"%d\n",m_pMapViewWnd->game_map_stage);
		else if(AniCount == 8)
		{
			m_pMapViewWnd->game_map_stage = 0;
			fprintf(save_userdata,"%d\n",m_pMapViewWnd->game_map_stage);
			Hp = totalHp/2;
		}
	}else
	{
		int a = m_pMapViewWnd->game_map_stage-(m_pMapViewWnd->quest-1);
		fprintf(save_userdata,"%d\n",a);
	}
	fprintf(save_userdata,"%d\n",Mp);
	fprintf(save_userdata,"%d\n",Hp);
	fprintf(save_userdata,"%d\n",Level);
	fprintf(save_userdata,"%d\n",Str);
	fprintf(save_userdata,"%d\n",Dex);
	fprintf(save_userdata,"%d\n",Int);
	fprintf(save_userdata,"%d\n",Mana);
	fprintf(save_userdata,"%d\n",Hana);
	if(AniCount != 8)
		fprintf(save_userdata,"%d\n",Kyunghum);
	else if(AniCount == 8)
	{
		if(Level > 10)
		{
			Kyunghum = Kyunghum*30/100;
		}
		fprintf(save_userdata,"%d\n",Kyunghum);
	}
	fprintf(save_userdata,"%d\n",Money);
	fprintf(save_userdata,"%d\n",stackPoint);

	for(int p=0;p<8;p++)
	{
		fprintf(save_userdata,"%d ",Interface_[p].itemnum);
		fprintf(save_userdata,"%d ",Interface_[p].num);
		fprintf(save_userdata,"%d ",Interface_[p].checked);
	}
	fprintf(save_userdata,"\n");
	for(int p=0;p<10;p++)
	{
		fprintf(save_userdata,"%d ",User_Itm_Att[p].itemnum);
		fprintf(save_userdata,"%d ",User_Itm_Att[p].num);
		fprintf(save_userdata,"%d ",User_Itm_Att[p].checked);
	}
	fprintf(save_userdata,"\n");
	for(int p=0;p<5;p++)
	{
		fprintf(save_userdata,"%d ",User_Itm_Booster[p].itemnum);
		fprintf(save_userdata,"%d ",User_Itm_Booster[p].num);
		fprintf(save_userdata,"%d ",User_Itm_Booster[p].checked);
	}
	fprintf(save_userdata,"\n");
	for(int p=0;p<30;p++)
	{
		fprintf(save_userdata,"%d ",User_Itm_Etc[p].itemnum);
		fprintf(save_userdata,"%d ",User_Itm_Etc[p].num);
		fprintf(save_userdata,"%d ",User_Itm_Etc[p].checked);
	}

	fclose(save_userdata);

	SAFE_DELETE_(m_pPet);
}

void cSprWnd::Init()
{
	char bf[256];
	int m_posx,m_posy,map_x_p,map_y_p;
	//m_pDirect3D       = D3D;

	//if(!m_pMapViewWnd->MapDataLoad())PostQuitMessage(0);
	
	FILE *mappos;

	sprintf(bf,"txt/mappos/%d-%d.txt",m_pMapViewWnd->game_map_stage,m_pMapViewWnd->game_map_prev);
	mappos = fopen(bf,"rt");
	fscanf(mappos,"%d",&map_x_p);
	fscanf(mappos,"%d",&map_y_p);
	fscanf(mappos,"%d",&m_posx);
	fscanf(mappos,"%d",&m_posy);
	fscanf(mappos,"%d",&y_);
	fscanf(mappos,"%d",&stageSelect);
	fclose(mappos);

	view = AniCount = 0;

	/*m_pMapViewWnd->spr_user_xx = */spr_x = 32*m_posx;
	/*m_pMapViewWnd->spr_user_yy = */spr_y = 32*m_posy;

	m_pMapViewWnd->map_x = map_x_p;
	m_pMapViewWnd->map_y = map_y_p;
	
	m_pMapViewWnd->Map_x = m_pMapViewWnd->map_x/32;
	m_pMapViewWnd->Map_y = m_pMapViewWnd->map_y/32;
		
	m_pMapViewWnd->map_X = m_pMapViewWnd->map_x%32;
	m_pMapViewWnd->map_Y = m_pMapViewWnd->map_y%32;
	//캐릭터 위치...
	m_pMiniMap->Mini_Map_User_x = m_pMapViewWnd->MAP_x = spr_x/8;
	m_pMiniMap->Mini_Map_User_y = m_pMapViewWnd->MAP_y = spr_y/8;
	//                    .........떨어지는 시간을 이용 에너지 차감.
	//                    .
	//                    .
	mis = Hptimes = Mptimes = DropUnit = 0;
}
