//������������BUG�����飺
//1.�Ƿ��˫�Ⱥ�д���˵��Ⱥţ����߷�����
//2.�Ƿ��kд��i,j���߷�����
//3.�Ƿ�д�˺�������û�е���
//4.�Ƿ���visual studio�Լ������⣬���п���Ƿ��װ��
//5.�Ƿ��int���͵ı���д����bool
//6.ʵ�ڲ��о�������Ϊ����ĵط��򼸸��ϵ㣬�����ˣ���ϵ�û����ô�ѵġ�
//�ѽ�����⣺
//1.����������϶�ֲ��Ĺ����У��������ƶ��켣�պþ������⣬��ͬʱ������������Ч������ԭ�沢���������ġ����ɽ��������û�У�ÿ�μ������ʱ����break���ϼ��ɣ�
//2.���ڻ�����ֻ��ʬ��ͬһ��ֲ�ֲ�ﱻ����֮��ֻ��һ����ʬ�ָ����ߣ���һֻ��Ȼά�����ڳԵ�״̬���ѽ����ÿһ֡��Ҫ���һ����ֻ��ʬ���ڵ������Ƿ����ֲ�
//3.���ڰ�������鴤����������
//4.����˲�����Ƶʱ�Ῠ�ٵ�����
//5.��������տ������������ҷɵ����⣬��Ȼ��֪���������ô����ˣ������Ǳ�������
//6.�������������տ�����������ٶȵ��ߣ��ͻᷢ��������տ���ͬһ��λ�������������������⣬����һ������ᾲֹ������Ҳû�а취��������֪�������������
//δ�����
//�ҷ������տ�������������񲻻���ʱ����ʧ�ˣ�����ˣ��������տ���������������ʧ��ǰ����Ῠ��
//Ŀǰ�������е�x���궼��ƫС�ģ������������ʵ����̫�鷳�ˣ��пյ�ʱ�������
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<ctime>
#include<graphics.h>
#include<mmsystem.h>
#include<cmath>
#include<math.h>
#include"tools.h"
#include"vector2.h"
#pragma comment(lib,"winmm.lib")

using namespace std;

#define img_width 900
#define img_height 600
#define pool_size 10//����ش�С

enum {wan_dou, xiang_ri_kui, plant_count };//xiang_ri_kui=0��
enum { going, win, fail };
IMAGE img_bg;
IMAGE img_tools_bar;
IMAGE img_plant_card[plant_count];
IMAGE* plant_gif[plant_count][20];//���������Ϊ�˱���ֲ���һϵ�ж�ͼ
IMAGE* sunshine_gif[30];//�������⶯ͼ
IMAGE zombie_gif[22];
IMAGE bullet_normal;//�ӵ�ֻ����������������״̬
IMAGE bullet_blast_gif[4];
IMAGE zombie_dead_gif[20];
IMAGE zombie_eat_gif[21];
IMAGE zombie_stand_gif[11];
IMAGE game_success;
IMAGE game_fail;
int game_state;//��¼��Ϸ��״̬
int kill_count;//��¼��ɱ��ʬ������
int max_count;//��¼��Ҫ��ɱ�Ľ�ʬ��Ŀ��ɱ������һ�ؾ�ͨ����
int plant_kind = 0;//ȫ�ֱ�����������¼���ѡ�е�ֲ�����࣬Ϊ0����ɶҲû��
int cur_x=-100;//������¼����ʵʱλ��
int cur_y=-100;
typedef struct sunshine {
	int value=50;
	int width;//���ڶ���
}sunshine;
sunshine sunshine_own;

typedef struct node_1 {//��ֲ����
	int plant_type;//0˵��û��ֲ��
	int frame_num;//֡���
	int health;
}node_1;
node_1 plant_block[3][9];//���о��е���ֲ�ؿ�

typedef struct node_2 {//������
	int x;
	int y;
	int dest_y;
	int frame_num;
	bool used;
	bool if_click;
	int life_time;
	float dx;//������֮����Ŀ����ƶ�ʱ��dx/dy��Ϊ�켣б�ʣ�dx,dy�ǵ�λʱ������������ƫ����
	float dy;
	bool if_from_sunflower;//�Ƿ����������տ�
	vector2 p1,p2,p3,p4;//���������ߵ��ĸ��ؼ���
	vector2 pur;//��һ���ṹ�壬�����˵�ǰ��x,y���꣬�����ظ��ˣ������޸����������鷳������ֻ������������տ��ϵ����ʱ��ʹ������ṹ��
	float t;//���������߲���
	bool if_ground;//�Ƿ����
}node_2;
node_2 sunshine_ball_pool[10];//�����

typedef struct node_3 {//��ʬ
	int x, y;
	int row;//��¼��ʬ������
	int frame_num;
	bool used;
	int speed;
	int health;
	bool if_dead;
	int dead_frame_num;
	bool if_eat;
	int eat_frame_num;//wrnmd!����տ�ʼ��ʱ�����ó�bool�ˣ�=��
}node_3;
node_3 zombie_pool[10];//��ʬ��

typedef struct node_4 {//�ӵ�
	int x, y;
	int frame_num;//��ը֡���
	bool if_blast;//�Ƿ�ը
	int row;
	int speed;
	bool used;
}node4;
node4 bullet_pool[30];//�ӵ���

typedef struct node_5 {//վ����ʬ
	int x, y;
	int frame_num;
	int speed;
}node_5;
node_5 stand_zombie[9];//վ����ʬ�أ�ֻ��Ҫ������ƬͷѲ�ӳ�����ʱ���飬��������

//�ж��ļ��Ƿ����
bool file_exist(const char* name) {
	FILE* fp = fopen(name, "r");
	if (fp==NULL) {//���û������ļ�
		return false;
	}
	else {
		fclose(fp);
		return true;
	}
}
int self_rand(int x, int y) {//������������������һ����֮��������,y>x
	int res = rand() % (y - x + 1) + x;
	return res;
}

void creat_sunshine_ball() {//��Ȼ����������
	static int timer_1 = 0;
	int clock_1 = self_rand(100, 150);//��λ��ms
	/*timer_1 += getDelay();//��֣������������ʦ�Զ���ĺ���Ϊʲô����*/
	timer_1++;
	if (timer_1 > clock_1) {//ÿ3-4������һ������
		for (int i = 0; i < pool_size; i++) {
			if (!sunshine_ball_pool[i].used && sunshine_ball_pool[i].if_click == false) {
				sunshine_ball_pool[i].used = true;
				sunshine_ball_pool[i].x = self_rand(253, 700);
				sunshine_ball_pool[i].y = 0;
				sunshine_ball_pool[i].dest_y = self_rand(175, 415);
				sunshine_ball_pool[i].life_time = 0;
				sunshine_ball_pool[i].dx = 0;
				sunshine_ball_pool[i].dy = 0;
				sunshine_ball_pool[i].if_click = false;
				sunshine_ball_pool[i].if_from_sunflower = false;
				sunshine_ball_pool[i].if_ground = false;
				timer_1 = 0;
				break;//ÿ��ѭ��ֻ����һ�����⼴��
			}
		}
	}
};
//���տ��������⣬
void creat_sunshine_by_sunflower() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (plant_block[i][j].plant_type == xiang_ri_kui + 1) {//����ؿ��ֵ������տ�
				int sunshine_max = sizeof(sunshine_ball_pool) / sizeof(sunshine_ball_pool[0]);
				static int timer = 0;
				timer++;
				if (timer > 300) {//����������Ըı����տ�����������ٶ�
					//���Ჹ��һ����ʱ����ÿ�����տ�������Ҫ100֡��������һ������
					for (int k = 0; k < sunshine_max; k++) {
						if (!sunshine_ball_pool[k].used&& sunshine_ball_pool[k].if_click==false) {//���������û�б�ʹ�ã���û�б��������Ϊ���������������false
							timer = 0;
							sunshine_ball_pool[k].used = true;
							sunshine_ball_pool[k].x = 225 + 82 * (j - 1)+50;//��������ߵĻ����������Ե���p2/p3
							sunshine_ball_pool[k].y = 178 + 102 * (i - 1)+40;
							sunshine_ball_pool[k].dest_y = 178 + 102 * (i - 1)+102;//���Ե�p4
							sunshine_ball_pool[k].if_click == false;
							sunshine_ball_pool[k].life_time = 0;
							sunshine_ball_pool[k].dx = 0;
							sunshine_ball_pool[k].dy = 4;
							sunshine_ball_pool[k].if_from_sunflower = true;
							sunshine_ball_pool[k].t = 0;//������
							//��ʼ�����������ߣ�p4�յ㣬p1���
							sunshine_ball_pool[k].p2 = vector2(225 + 82 * (j - 1) + 80, 178 + 102 * (i - 1) + 60);
							sunshine_ball_pool[k].p3 = vector2(225 + 82 * (j - 1) + 40, 178 + 102 * (i - 1) + 100);
							sunshine_ball_pool[k].p1 = vector2(225 + 82 * (j - 1) + 100, 178 + 102 * (i - 1) + 90);
							sunshine_ball_pool[k].p4 = vector2(225 + 82 * (j - 1)+40, 178 + 102 * (i - 1) + 102);
							sunshine_ball_pool[k].if_ground = false;//û�����
							
							break;//һ��ֻ��һ��
						}
					}
				}
			}
		}
	}
}
//��Լ�����������߼���һ˿˿������
void update_sunshine_ball() {//��������߼��Ķ��ˣ�ֱ��������ص��յ㣬״̬�Ż���false
	for (int i = 0; i < pool_size; i++) {
		//���±����е��������״̬
		if (sunshine_ball_pool[i].if_click) {
			//
			////Ϊ����߹켣����
			float angle = atan2(sunshine_ball_pool[i].y ,(sunshine_ball_pool[i].x - 271));
			sunshine_ball_pool[i].dx = 20 * cos(angle);//����������Ըı䱻����������Ծ���ٶ�
			sunshine_ball_pool[i].dy = 20 * sin(angle);

			sunshine_ball_pool[i].x -= sunshine_ball_pool[i].dx;
			sunshine_ball_pool[i].y -= sunshine_ball_pool[i].dy;
			if (sunshine_ball_pool[i].x < 290 && sunshine_ball_pool[i].x>252&& sunshine_ball_pool[i].y <= 0) {//�����յ�֮��//����һ��Ҫ��y<=0ѽ����Ȼ�Ļ������鴤
				sunshine_own.value += 25;//�ټ�������ֵ
				sunshine_ball_pool[i].dx = 0;
				sunshine_ball_pool[i].dy = 0;
				sunshine_ball_pool[i].if_click = false;
				sunshine_ball_pool[i].used = false;//

			}
		}
		//�������ظ����壬�п����Ż�
		else if (sunshine_ball_pool[i].used) {//��������ʹ�ã���û�б����
			sunshine_ball_pool[i].frame_num = (sunshine_ball_pool[i].frame_num + 1) % 29;//����֡����
			
			if (sunshine_ball_pool[i].if_from_sunflower == false && sunshine_ball_pool[i].if_ground == false) {//���������������Ȼ���ɣ�����������ж�д�ɸ�ֵ�ˣ����ˣ�
				if (sunshine_ball_pool[i].life_time == 0) {//��û���䵽����
					sunshine_ball_pool[i].y += 5;
				}
				if (sunshine_ball_pool[i].y >= sunshine_ball_pool[i].dest_y) {//�䵽�����ҹ���100֡
					sunshine_ball_pool[i].life_time++;
					if (sunshine_ball_pool[i].life_time > 100) {
						sunshine_ball_pool[i].used = false;
						sunshine_ball_pool[i].if_click = false;
					}
				}
			}
			else if(sunshine_ball_pool[i].if_from_sunflower == true&&sunshine_ball_pool[i].if_ground == false){//����������������տ�,�һ�û�����
				//sunshine_ball_pool[i].t = 0.1;
				if (sunshine_ball_pool[i].t >= 1) {
					sunshine_ball_pool[i].if_ground = true;//t=1�������
					sunshine_ball_pool[i].life_time = 0;
				}
				else {
					vector2 tmp = calcBezierPoint(sunshine_ball_pool[i].t, sunshine_ball_pool[i].p1, sunshine_ball_pool[i].p2, sunshine_ball_pool[i].p3, sunshine_ball_pool[i].p4);
					sunshine_ball_pool[i].x = tmp.x;
					sunshine_ball_pool[i].y = tmp.y;
					sunshine_ball_pool[i].t += 0.1;
				}
				
			}
			else if (sunshine_ball_pool[i].if_ground == true) {//����������Ѿ����
				sunshine_ball_pool[i].life_time++;

				if (sunshine_ball_pool[i].life_time > 100) {
					sunshine_ball_pool[i].used = false;
					sunshine_ball_pool[i].if_click = false;
				}
			}
		}
	}
};


//��ͼƬ�����ڴ棬����������
void gameinit() {
	//���û�ɱ��ʬĿ����
	max_count = 10;
	kill_count = 0;
	game_state = going;
	//�޸��ַ���Ϊ���ֽ��ַ���,û��Ҫ��L
	loadimage(&img_bg,"res/bg.jpg");//�൱���ǰ�ͼƬ�ͱ�������ϵ����
	loadimage(&img_tools_bar, "res/bar5.png");
	//�����ӵ�ͼƬ
	loadimage(&bullet_normal, "res/bullets/bullet_normal.png");
	loadimage(&bullet_blast_gif[3], "res/bullets/bullet_blast.png");
	loadimage(&game_success, "res/gameWin.png");
	loadimage(&game_fail, "res/gameFail.png");
	//��Ϊֻ��һ���ӵ���ը��ͼƬ���������ǽ�ͼƬ��С���ŵ�֡�����У�ʵ�ֱ�ը����
	for (int i = 0; i < 3; i++) {
		int width = bullet_blast_gif[3].getwidth();
		int height = bullet_blast_gif[3].getheight();
		float k = 0.25 * (i + 1);
		loadimage(&bullet_blast_gif[i], "res/bullets/bullet_blast.png", width*k, height*k,true);
	}
	
	memset(sunshine_ball_pool, 0, sizeof(sunshine_ball_pool));
	memset(plant_block, 0, sizeof(plant_block));
	memset(sunshine_gif, 0, sizeof(sunshine_gif));
	memset(plant_gif, 0, sizeof(plant_gif));//��ʼ��plant_gif�ڿռ�Ϊ0������һ������0��֪��������
	memset(zombie_pool, 0, sizeof(zombie_pool));
	memset(bullet_pool, 0, sizeof(bullet_pool));
	memset(stand_zombie, 0, sizeof(stand_zombie));
	//memset(zombie_gif, 0, sizeof(zombie_gif));���������ʼ���������load�ͻ����Խ�磬���Բ�Ҫ��image���͵������ʼ��


	char name[64];
	//����ֲ�ﶯͼ��ֲ�￨Ƭ
	for (int i = 0; i < plant_count; i++) {
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);//name�о������ʽ�����ɵ�·���ַ���
		loadimage(&img_plant_card[i], name);
		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
			if (file_exist(name)) {
				plant_gif[i][j] = new IMAGE;//����Щ����ͼƬ�����ڴ�ռ�
				loadimage(plant_gif[i][j], name);
			}
		}
	}
	//�������⶯ͼ
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		if (file_exist(name)) {
			sunshine_gif[i] = new IMAGE;
			loadimage(sunshine_gif[i], name);
		}
	}
	//���ؽ�ʬ��ͼ
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		loadimage(&zombie_gif[i], name);
	}
	//���ؽ�ʬ������ͼ
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/zm_dead/%d.png", i + 1);
		loadimage(&zombie_dead_gif[i], name);
	}
	//���ؽ�ʬ��ֲ��Ķ�ͼ
	for (int i = 0; i < 21; i++) {
		sprintf_s(name, sizeof(name), "res/zm_eat/%d.png", i + 1);
		loadimage(&zombie_eat_gif[i], name);
	}
	//����վ����ʬ��ͼ
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, sizeof(name), "res/zm_stand/%d.png", i + 1);
		loadimage(&zombie_stand_gif[i], name);
	}
	//��ʼ�����ڣ��������������Ϊ1����ͼ�δ��ںͿ���̨���ڻ�ͬʱ����
	initgraph(img_width, img_height);
	//���ز��޸�����
	LOGFONT f;//����ṹ��
	gettextstyle(&f);
	f.lfHeight = 25;
	f.lfWeight = 10;
	sunshine_own.width = 30;
	strcpy(f.lfFaceName, "Segoe UI BLACK");
	f.lfQuality=ANTIALIASED_QUALITY;//�����
	settextstyle(&f);//����ǰ���ı���ʽ����Ϊf����������ʽ
	setbkmode(TRANSPARENT);
	setcolor(BLACK);
}

void creat_zombie() {//ÿ���඼Ҫ�д������ı����ݣ���Ⱦ
	static int timer=0;
	int rander = self_rand(300, 420);
	timer++;
	if (timer > rander) {
		//int pool_max = sizeof(zombie_pool) / sizeof(zombie_pool[0]);
		int pool_max = 10;
		for (int i = 0; i < pool_max; i++) {
			if (!zombie_pool[i].used) {
				zombie_pool[i].used = true;
				zombie_pool[i].x = img_width;
				zombie_pool[i].row = self_rand(0, 2);
				zombie_pool[i].y = 135 + zombie_pool[i].row * 98;//�����������Ϊ̫�����漴���Իᵼ�½�ʬ�ֲ�������
				zombie_pool[i].speed = 1;
				zombie_pool[i].frame_num = 0;
				zombie_pool[i].health = 100;
				zombie_pool[i].dead_frame_num = 0;
				zombie_pool[i].if_eat = false;//һ��ʼ���Զ���
				zombie_pool[i].eat_frame_num = 7;
				timer = 0;
			    break;//һ��ѭ��ֻ��Ҫ����һֻ��ʬ�Ϳ���
			}
			//timer = 0;
			//break;//һ��ѭ��ֻ��Ҫ����һֻ��ʬ�Ϳ���
			//�������зŴ�ط��ˣ�����ÿ��ѭ�������timer��0��Ӧ���ŵ�ifѭ����
		}
	}
};

void update_zombie() {
	int pool_max = sizeof(zombie_pool) / sizeof(zombie_pool[0]);
	static int timer = 0;
	timer++;
	int clock = 2;
	if (timer >= clock) {//ÿ��֡�Ż����һ�ν�ʬ�����ݣ�����������Ըı佩ʬ�����Ŀ���
		for (int i = 0; i < pool_max; i++) {
			if (zombie_pool[i].if_dead) {//��ʬ�Ƿ��Ѿ�����������һ�£��ҵĶ������ʬ�ڳ�����ʧ֮ǰ���Ǳ�ʹ���ŵ�
				zombie_pool[i].dead_frame_num++;
				if (zombie_pool[i].dead_frame_num > 19) {
					zombie_pool[i].used = false;
					zombie_pool[i].if_dead = false;
				}
			}
			else if (zombie_pool[i].used) {//��ʬ����ʹ��
				//�����ʬû���������ڳԶ���
				if (zombie_pool[i].if_eat) {
					zombie_pool[i].eat_frame_num = (zombie_pool[i].eat_frame_num + 1) % 21;
					timer = 0;
				}
				else {//�����ʬ�����߶�
					//�ƶ����ı�֡��
					zombie_pool[i].x -= zombie_pool[i].speed;
					zombie_pool[i].frame_num = (zombie_pool[i].frame_num + 1) % 22;
					timer = 0;
					if (zombie_pool[i].x < 192) {
						/*printf("game over\n");
						MessageBox(NULL, "over", "over", 0);
						exit(0);*/
						game_state = fail;
					}
				}
			}
		}
	}
};
void creat_bullet() {
	//���ȱ�����ʬ�أ����ݻ��ŵĽ�ʬ���ڵ�������ʼ��if_zombie���飬��������¼��ÿһ���Ƿ��н�ʬ
	int if_zombie[3] = { 0 };
	int pool_max = sizeof(zombie_pool) / sizeof(zombie_pool[0]);
	for (int i = 0; i < pool_max; i++) {
		if (zombie_pool[i].used&& zombie_pool[i].x<850) {
			if_zombie[zombie_pool[i].row] = 1;//��¼һ����һ���н�ʬ
		}
	}
	//������ֲ�ؿ�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (plant_block[i][j].plant_type == wan_dou+1) {//plant_type��0��û��ֲ��
				if (if_zombie[i]) {//�����һ���н�ʬ�����ֲ�����㶹
					int bullet_max = sizeof(bullet_pool) / sizeof(bullet_pool[0]);
					//��ʼ�����Ĵ����ӵ�,��ʮ֡����һ��
					static int timer = 0;//��ȥ������ԭ����27����ͬ��timer
					timer++;
					if (timer > 40) {//���ʣ�static��ʵֻ���ڲ��forѭ���������ðɣ�����ͬһ���е������е��㶹��ʵ������һ��timer������㶹�ĸ����������ӵ�λʱ�����ӵ�������
						timer = 0;
						for (int k = 0; k < bullet_max; k++) {
							if (!bullet_pool[k].used) {//���û�б�ʹ��
								bullet_pool[k].used = true;
								bullet_pool[k].row = i;
								bullet_pool[k].speed = 8;
								bullet_pool[k].if_blast = false;
								bullet_pool[k].frame_num = 0;
								//253 + 82 * j, 177 + 102 * i,������㶹���ֵ����꣬���㶹��ʵ��������Ҫ΢��
								bullet_pool[k].x = 253 + 82 * j + 50;
								bullet_pool[k].y = 177 + 102 * i + 5;
								break;//һ��ֻ���ҵ�һ��δʹ�õ��ӵ�����
							}
						}
					}
				}
			}
		}
	}
	
}
void update_bullet() {//?
	int bullet_max = sizeof(bullet_pool) / sizeof(bullet_pool[0]);
	for (int i = 0; i < bullet_max; i++) {
		if (bullet_pool[i].used) {
			bullet_pool[i].x += bullet_pool[i].speed;
			if (bullet_pool[i].x > img_width) {
				bullet_pool[i].used = false;
			}
			//�����ը
			if (bullet_pool[i].if_blast) {
				bullet_pool[i].frame_num += 1;//ֻ����֡��������ѭ������
				if (bullet_pool[i].frame_num > 3) {
					bullet_pool[i].used = false;//?
					bullet_pool[i].if_blast = false;//���ﲻֹͣ�Ļ������Խ�磬һ��Ҫ�ǵ�����blastΪfalse!!
				}
			}
		}
	}
}

void collision_check() {//��ײ���
	int zombie_max = sizeof(zombie_pool) / sizeof(zombie_pool[0]);
	int bullet_max = sizeof(bullet_pool) / sizeof(bullet_pool[0]);
	for (int j = 0; j < bullet_max; j++) {
		if (bullet_pool[j].used&& bullet_pool[j].if_blast==false) {
			for (int i = 0; i < zombie_max; i++) {
				//�����ʬû���������ҽ�ʬ���㶹�к���ͬ���������ײ
				if (zombie_pool[i].if_dead==false && zombie_pool[i].used && zombie_pool[i].row == bullet_pool[j].row) {
					if (bullet_pool[j].x >= zombie_pool[i].x+60) {
						bullet_pool[j].if_blast = true;
						zombie_pool[i].health -= 10;
						bullet_pool[j].speed = 0;
						if (zombie_pool[i].health <= 0) {//��ʬ����ֵ��0
							zombie_pool[i].if_dead = true;
							//��ɱ������
							kill_count++;
							if (kill_count > max_count) {
								game_state = win;
							}
						}
					}
				}
			}
		}
	}
}
//��⽩ʬ�Ƿ��ڳ�ֲ��Լ�ֲ���Ƿ񱻳Թ�
void eat_check() {
	int zm_max = sizeof(zombie_pool) / sizeof(zombie_pool[0]);
	for (int k = 0; k < zm_max; k++) {
		if (zombie_pool[k].used && zombie_pool[k].if_dead == false) {//�����ʬ����ʹ����û������
			int row = zombie_pool[k].row;//��ʬ���ڵ�����
			if (zombie_pool[k].if_eat&&plant_block[row][(zombie_pool[k].y - 255) / 82].plant_type == 0) {//�����ʬ���ڵ���������ϲ�û��ֲ��
				zombie_pool[k].if_eat = false;//���������2
			}
			for (int j = 0; j < 9; j++) {//ֻ��Ҫ�ж�һ��ͬһ�е�ֲ��Ϳ�����
				int plant_x = 253 + 82 * j;
				if (plant_block[row][j].plant_type >0 && zombie_pool[k].x <= plant_x-15) {//����ʬ����ֲ���ʱ�򣬸ı佩ʬ��״̬
					zombie_pool[k].if_eat = true;
					zombie_pool[k].speed = 0;//�Զ�����ʱ��ͣ����
					/*���ﲻ��ÿһ֡���ԣ������ӳԵ�̫����*/
					static int timer = 0;
					timer++;
					if (timer > 15) {//��ʮ֡��һ��
						timer = 0;
						plant_block[row][j].health -= 10;
						if (plant_block[row][j].health <= 0) {//�Թ�ֲ��
							plant_block[row][j].plant_type = 0;
							zombie_pool[k].if_eat = false;
							zombie_pool[k].speed = 1;
						}
					}
				}
			}
		}
	}
}

void update_game() {//������Ϸ����
	//����ֲ��֡���
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (plant_block[i][j].plant_type > 0) {
				plant_block[i][j].frame_num++;
				if (plant_gif[plant_block[i][j].plant_type - 1][plant_block[i][j].frame_num] == NULL) {
					plant_block[i][j].frame_num = 0;//֡�������Ϊ0
				}
			}
		}
	}
	//�������������
	//������ʱʱ�̶̿�����
	
		creat_sunshine_ball();
		creat_sunshine_by_sunflower();
		update_sunshine_ball();
		creat_zombie();
		update_zombie();
		creat_bullet();
		update_bullet();
		collision_check();
		eat_check();
}

void draw_zombie() {
	//int pool_max = sizeof(zombie_pool) / sizeof(zombie_pool[0]);
	int pool_max = 10;
	for (int i = 0; i < pool_max; i++) {
		if (zombie_pool[i].if_dead) {//��ʬ�������Ա�ʹ�ã���ʬ��ɻң�20֡��֮�������
			putimagePNG(zombie_pool[i].x, zombie_pool[i].y,&zombie_dead_gif[zombie_pool[i].dead_frame_num]);
		}
		else if (zombie_pool[i].used) {
			if (zombie_pool[i].if_eat) {//�Զ���
				putimagePNG(zombie_pool[i].x, zombie_pool[i].y, &zombie_eat_gif[zombie_pool[i].eat_frame_num]);
			}
			else {//������·
				putimagePNG(zombie_pool[i].x, zombie_pool[i].y, &zombie_gif[zombie_pool[i].frame_num]);
			}
		}
	}
}
void draw_bullet() {
	int bullet_max = sizeof(bullet_pool) / sizeof(bullet_pool[0]);
	for (int i = 0; i < bullet_max; i++) {
		if (bullet_pool[i].if_blast) {
			
			putimagePNG(bullet_pool[i].x, bullet_pool[i].y, &bullet_blast_gif[bullet_pool[i].frame_num]);

		}
		if (bullet_pool[i].used) {
			putimagePNG(bullet_pool[i].x, bullet_pool[i].y, &bullet_normal);
		}
	}
}



void update_window() {//��֡����


	BeginBatchDraw();//�����Ŀ���ǽ���Щһ���������ͼƬ����һ�����

	putimage(0, 0, &img_bg);//������0,0�������ϽǶ���Ϊ��׼������
	putimagePNG(250, 5, &img_tools_bar);//������
	for (int i = 0; i < plant_count; i++) {//ֲ�￨
		putimagePNG(333+65*i,10,&img_plant_card[i]);
	}
	
	//ѭ��������ֲ���죬��Ⱦ������ֲ��
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (plant_block[i][j].plant_type) {
				putimagePNG(253 + 82 * j, 177 + 102 * i, plant_gif[plant_block[i][j].plant_type-1][plant_block[i][j].frame_num]);
			}
		}
	}

	//��Ⱦ����϶�;�е�ͼƬ���ŵ���Ⱦ���̵�������������϶�ֲ��ʱ�����������ͼ�㣬�����ᱻ����
	if (plant_kind > 0) {
		IMAGE* img = plant_gif[plant_kind - 1][0];
		putimagePNG(cur_x - img->getwidth() / 2, cur_y - img->getheight() / 2, img);
	}
	for (int i = 0; i < pool_size; i++) {
		if (sunshine_ball_pool[i].used|| sunshine_ball_pool[i].dx|| sunshine_ball_pool[i].dy) {
			putimagePNG(sunshine_ball_pool[i].x, sunshine_ball_pool[i].y, sunshine_gif[sunshine_ball_pool[i].frame_num]);
		}
	}
	char score_sunshine[8];
	sprintf_s(score_sunshine, sizeof(score_sunshine), "%d", sunshine_own.value);
	outtextxy(279, 76, score_sunshine);//�������

	draw_zombie();//���ƽ�ʬ
	draw_bullet();

	EndBatchDraw();
};
//���������ʱ�����д������������߼����涨������ֻ�е����յ�ʱ�����Ϊ����
void deal_with_sunshine(ExMessage* mes) {
	int dx = sunshine_gif[0]->getwidth();
	int dy = sunshine_gif[0]->getheight();
	for (int i = 0; i < pool_size; i++) {//��������أ��ж���Щ������û�б�ѡ��
		if (sunshine_ball_pool[i].used&& sunshine_ball_pool[i].if_click==false) {//����ʹ����û�б����
			if (mes->x > sunshine_ball_pool[i].x && mes->x< sunshine_ball_pool[i].x + dx && mes->y>sunshine_ball_pool[i].y && mes->y < sunshine_ball_pool[i].y + dy) {
				
				//mciSendString("play res/sunshine.mp3", NULL, 0, NULL);//����Ҫ�����٣��滻Ϊ�첽��Ƶ���ţ������������̡߳�
				mciSendString("play sunshine_audio from 0", NULL, 0, NULL);
				float angle = atan2(mes->y, mes->x - 271);//ҪС�ĳ���Ϊ0,���Ը�����atan2
				//float angle = atan2(mes->x - sunshine_ball_pool[i].x - 290, mes->y - sunshine_ball_pool[i].y);
				sunshine_ball_pool[i].dx = 15 * cos(angle);
				sunshine_ball_pool[i].dy = 15 * sin(angle);
				//sunshine_ball_pool[i].used = false;
				sunshine_ball_pool[i].if_click = true;//������
				///break;
				//������������һ��break���Ϳ���ʵ��һ��ֻ�ռ�һ�����⣿
			}

		}
	}
}

void user_click() {//���ڶ����������д���
	ExMessage mes;//����һ�����ڱ������״̬�Ľṹ��
	
	if (peekmessage(&mes)) {//�����궯��
		if (mes.message == WM_LBUTTONDOWN) {//��������
			cur_x = mes.x;//����Ҳ����һ�µ�ǰ�������Ļ��Ͳ�����ֲ�Ӱ�˰�
			cur_y = mes.y;
			if (mes.x > 333 && mes.x < 333 + 65 * plant_count && mes.y>10 && mes.y < 100) {//�ж��Ƿ��Ǵӹ�����ѡ��ֲ��
				plant_kind = (mes.x - 333) / 65+1;//��ȡֲ�����࣬plant_kind=0ʱ������û��ֲ��
				//printf("%d\n", plant_kind);
			}
			deal_with_sunshine(&mes);
		}
		else if (mes.message == WM_MOUSEMOVE&&plant_kind>0) {//����ƶ�
			cur_x = mes.x;
			cur_y = mes.y;

		}
		else if (mes.message == WM_LBUTTONUP) {//���̧��
			int row = (mes.y - 177) / 102;
			int col = (mes.x - 253) / 82;
			if (row >= 0 && row <= 2 && col >= 0 && col <= 8&&plant_block[row][col].plant_type==0) {//����û�б�ռ�ã�����ѡ��ֲ�ռ���������
				//
					plant_block[row][col].plant_type= plant_kind;
					plant_block[row][col].frame_num = 0;
					plant_block[row][col].health = 100;//ֲ���Ѫ��������Ϊ100
				
				/*printf("%d, %d\n", row, col);*/
			}
			plant_kind = 0;//����ɿ���ȫ�ֱ�����0
		}
	}
}
void start_ui() {//��ʼ���棬���棬ѭ����Ⱦ����������Ϣ�ı���������ߣ���������߼�����ʦ�Ĳ�һ��
	IMAGE ui_bg, ui_menu1, ui_menu2;
	loadimage(&ui_bg, "res/menu.png");
	loadimage(&ui_menu1, "res/menu1.png");
	loadimage(&ui_menu2, "res/menu2.png");
	//int flag = 0;
	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &ui_bg);
		
		putimagePNG(473, 70, &ui_menu1);
		
		ExMessage meg;
		if (peekmessage(&meg)) {
			cur_x = meg.x;//ȫ�ֱ����������λ�ã�������겻����ʱ��Ҳ��֪������λ��
			cur_y = meg.y;
		}

		if (cur_x > 473 && cur_x < 973 && cur_y>70 && cur_y < 210) {//������ȥʱ����ɫ�䰵
			putimagePNG(473, 70, &ui_menu2);
			if (meg.message == WM_LBUTTONDOWN) {//������ʱ�����������ȥ
				return;//����������
			}
		}
		EndBatchDraw();
	}
}
void view_scenes_2() {

}
//ƬͷѲ��
//�ٴ�����һ�£�putimage���������ͼƬ���ϽǶ����ڴ����е�����
void view_scenes_1() {
	vector2 pointer[9] = { {550,80},{530,160},{630,170},{530,200},{515,270},{565,370},{605,304},{705,280},{690,340} };
	for (int i = 0; i < 9; i++) {//����Ϊ�˷�����⣬����ʵû��Ҫ�����
		stand_zombie[i].x = pointer[i].x + 500;//���ǽ�ʬ���ܱ���ͼƬ�ϵ�λ��
		stand_zombie[i].y = pointer[i].y;
		stand_zombie[i].frame_num = self_rand(0,11);
	}
	//����������������ƶ�
	int offset_screen = img_width - img_bg.getwidth();//900-1400=-500,
	int timer = 0;
	for (int x = 0; x >= offset_screen; x-=2) {
		BeginBatchDraw();
		putimage(x, 0, &img_bg);//��������ʱ�������ƶ�	
		timer++;
		for (int i = 0; i < 9; i++) {
			putimagePNG(stand_zombie[i].x + x, stand_zombie[i].y, &zombie_stand_gif[stand_zombie[i].frame_num]);
		}
		//ÿ��10֡�����½�ʬ�Ķ���֡
		if (timer > 2) {
			for (int i = 0; i < 9; i++) {
				stand_zombie[i].frame_num = (stand_zombie[i].frame_num + 1) % 11;
				timer = 0;
			}
		}

		Sleep(10);
		EndBatchDraw();
	}
	//Sleep(300);//�������Ҫ���Ż�һ��������α��ֽ�ʬ�����޷��νӵ������ͣ��300֡��
	//����Ҳ���Ҹ�����ʦ�Ľ����һ�����ģ�����������Ĵ������ʦ�Ĵ���˿��
	for (int i = 0; i < 100; i++) {
		BeginBatchDraw();
		putimage(-500, 0, &img_bg);
		for (int i = 0; i < 9; i++) {
			putimagePNG(stand_zombie[i].x-500, stand_zombie[i].y, &zombie_stand_gif[stand_zombie[i].frame_num]);
		}
		timer++;
		if (timer > 2) {
			for (int i = 0; i < 9; i++) {
				stand_zombie[i].frame_num = (stand_zombie[i].frame_num + 1) % 11;
				timer = 0;
			}
		}
		Sleep(10);
		EndBatchDraw();
	}
	//����������
	for (int x = 0; x <= -1 * offset_screen; x += 2) {//x>0,
		BeginBatchDraw();
		putimage(-500+x, 0, &img_bg);//��������ʱ�������ƶ�	
		timer++;
		for (int i = 0; i < 9; i++) {
			putimagePNG(stand_zombie[i].x + x-500, stand_zombie[i].y, &zombie_stand_gif[stand_zombie[i].frame_num]);
		}
		//ÿ��10֡�����½�ʬ�Ķ���֡
		if (timer > 2) {
			for (int i = 0; i < 9; i++) {
				stand_zombie[i].frame_num = (stand_zombie[i].frame_num + 1) % 11;
				timer = 0;
			}
		}
		Sleep(10);
		EndBatchDraw();
	}

}
void bars_down() {
	for (int y = -1*img_tools_bar.getheight(); y <= 0; y++) {
		BeginBatchDraw();
		putimage(0, 0, &img_bg);
		putimagePNG(250, y+5, &img_tools_bar);
		for (int i = 0; i < plant_count; i++) {
			putimagePNG(333 + 65 * i, y + 10, &img_plant_card[i]);
		}
		Sleep(10);
		EndBatchDraw();
	}
}
bool check_game_state() {
	if (game_state == going) {
		return true;
	}
	else {
		BeginBatchDraw();
		if (game_state == win) {
			putimage(0, 0, &game_success);
		}
		else if (game_state == fail) {
			putimage(0, 0, &game_fail);
		}
		EndBatchDraw();
		Sleep(100);
		return false;
	}
}
//������������ֻ��Ҫ��1�Ļ����ϸ�һ�¾ͺ���
int main() {
	int timer = 0;//��ʱ��
	mciSendString("open res/sunshine.mp3 alias sunshine_audio", NULL, 0, NULL);//Ԥ�ȼ�����Ƶ�ļ�����ֹ����
	srand(time(0));
	gameinit();
	start_ui();
	view_scenes_1();
	bars_down();
	//view_scenes_2();
	while (1) {//ѭ��user_click��Ϊ��һֱ�����û�������������ѭ��ͼƬ���ش����Ϊ��ʵ�ֶ�ֲ̬���Ч��
		timer += getDelay();//��þ�����һ�β�����ʱ���
		update_window();
		user_click();
		if (timer > 40) {//ÿ��40ms��Ⱦһ֡��ͼ
			update_game();//���ڸ�����Ϸ����
			timer = 0;
			if (check_game_state()==false) {
				break;
			}
		}
	}
	system("pause");
	mciSendString("close sunshine_audio", NULL, 0, NULL);//�ر���Ƶ�ļ�
	return 0;
}