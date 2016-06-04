#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void tapogat(int **t, int sor, int oszlop, int y, int x) {
	/*맵에 지뢰위치를 저장하는 함수입니다.*/
	/*매개변수: 맵의 크기, x와 y의 필드 좌표.*/
	int akna = 0;
	/*배열 주소에 지뢰를 저장합니다.*/
	if (y - 1 >= 0 && x - 1 >= 0 && t[y - 1][x - 1] == 'B') akna++;
	if (y - 1 >= 0 && 1 && t[y - 1][x] == 'B') akna++;
	if (y - 1 >= 0 && x + 1<oszlop && t[y - 1][x + 1] == 'B') akna++;
	if (1 && x - 1 >= 0 && t[y][x - 1] == 'B') akna++;
	if (1 && x + 1<oszlop && t[y][x + 1] == 'B') akna++;
	if (y + 1<sor &&     x - 1 >= 0 && t[y + 1][x - 1] == 'B') akna++;
	if (y + 1<sor && 1 && t[y + 1][x] == 'B') akna++;
	if (y + 1<sor && x + 1<oszlop && t[y + 1][x + 1] == 'B') akna++;
	t[y][x] = akna + '0'; /*아스키코드 저장*/
	if (t[y][x] == '0') {
		/*재귀함수.*/
		if (y - 1 >= 0 && x - 1 >= 0 && t[y - 1][x - 1] != '0') tapogat(t, sor, oszlop, y - 1, x - 1);
		if (y - 1 >= 0 && 1 && t[y - 1][x] != '0') tapogat(t, sor, oszlop, y - 1, x);
		if (y - 1 >= 0 && x + 1<oszlop && t[y - 1][x + 1] != '0') tapogat(t, sor, oszlop, y - 1, x + 1);
		if (1 && x - 1 >= 0 && t[y][x - 1] != '0') tapogat(t, sor, oszlop, y, x - 1);
		if (1 && x + 1<oszlop && t[y][x + 1] != '0') tapogat(t, sor, oszlop, y, x + 1);
		if (y + 1<sor &&     x - 1 >= 0 && t[y + 1][x - 1] != '0') tapogat(t, sor, oszlop, y + 1, x - 1);
		if (y + 1<sor && 1 && t[y + 1][x] != '0') tapogat(t, sor, oszlop, y + 1, x);
		if (y + 1<sor && x + 1<oszlop && t[y + 1][x + 1] != '0') tapogat(t, sor, oszlop, y + 1, x + 1);
	}
}
int foglal(int ***t, int sor, int oszlop) {
	/*Dinamikus memoriat foglal.
	Sikertelen memfoglalas eseten a pointer lenullazodik, a fv. visszateresi ertekkel is jelez.*/
	/*매개변수: 맵의 크기.
	Visszateresi ertek: 0, ha sikeres, 1, ha sikertelen a memfoglalas.*/
	int i, hiba = 0;
	*t = (int**)malloc(sor * sizeof(int*));
	if (*t == NULL) return 1;
	for (i = 0; i<sor; i++) {
		(*t)[i] = NULL;
		(*t)[i] = (int*)malloc(oszlop * sizeof(int));
		if ((*t)[i] == NULL)
			hiba = 1;
	}
	if (hiba) {
		for (i = 0; i<sor; i++)
			if ((*t)[i])
				free((*t)[i]);
		free(*t);
		*t = NULL;
		return 1;
	}
	return 0;
}
int indit(int ***t, int sor, int oszlop, int akna) {
	/*배열초기화
	맵의 요소 재설정, majd veletlenszer�en kitolti aknakat jelento 'B' karakterekkel(ASCII).
	Ha nincs tomb (a parameterlistan nullpointer van), meghivja a foglal fv.-t.
	Az aknak szama nem haladhatja meg a tomb elemszamanak 2/3-anak egeszreszet.
	Ha a foglal futasa sikertelen, visszateresi ertekkel jelez.*/
	/*Parameterek: tomb cime es merete, aknak elvart szama.
	Visszateres: 0, ha sikeres; 1, ha sikertelen.*/
	int i, x, y;
	if (*t == NULL && foglal(t, sor, oszlop))
		return 1;
	srand(time(0));
	if (0<akna && akna <= (sor*oszlop) / 2) {
		for (y = 0; y<sor; y++)
			for (x = 0; x<oszlop; x++)
				(*t)[y][x] = 0;
		for (i = 0; i<akna; i++) {
			do {
				y = rand() % sor;
				x = rand() % oszlop;
			} while ((*t)[y][x]);
			(*t)[y][x] = 'B';
		}
	}
	else if ((sor*oszlop) / 2<akna && akna <= (2 * sor*oszlop) / 3) {
		/*Ha a mezok tobb mint felere aknat akarunk rakni, celszerubb
		eloszor 'B'-vel kitolteni a tombot, majd veletlenszeruen nullazgatni.*/
		for (y = 0; y<sor; y++)
			for (x = 0; x<oszlop; x++)
				(*t)[y][x] = 'B';
		for (i = sor*oszlop; i>akna; i--) {
			do {
				y = rand() % sor;
				x = rand() % oszlop;
			} while ((*t)[y][x] == 0);
			(*t)[y][x] = 0;
		}
	}
	return 0;
}
int vizsgal(int **t, int sor, int oszlop, int y, int x) {
	/*Megvizsgalja, hogy a megadott mezon van-e akna, es ha nincs,
	a tapogat() fv. seg�ts�g�vel azt is, hogy h�ny szomsz�dos mezon van.
	Visszateresi erteke eldonti, hogy a jatek folytatodik-e,
	es ha nem, gyozott vagy veszitett a jatekos.*/
	/*Argumentumok: tomb es merete, a kerdeses mezo koordinatai.
	Visszateresi ertekek: -1: akna volt; 0: nincs akna, folytatodhat a jatek, 1: nincs akna, a jatekos nyert.*/
	int nyero = 1, i, j;
	if (t[y][x] == 'B') return -1;
	tapogat(t, sor, oszlop, y, x);
	for (i = 0; i<sor; i++)
		for (j = 0; j<oszlop; j++)
			if (t[i][j] == 0)
				nyero = 0;
	return nyero;
}

void szabad(int ***t, int sor) {
	/*Felszabaditja a megadott tombot.*/
	/*Parameterlista: tomb cime es sorainak szama.*/
	int i;
	for (i = 0; i<sor; i++)
		free((*t)[i]);
	free(*t);
	*t = NULL;
}
int betolt(int ***t, int *sor, int *oszlop, int *akna) {
	/*저장된 게임을 불러옵니다.*/
	FILE *game;
	int y, x, hiba = 0;
	game = fopen("Save.txt", "rt");
	if (game) {
		fscanf(game, "%d*%d;%d\n", sor, oszlop, akna);
		system("cls");
		if (*sor<5 || *oszlop<5 || 35<*sor || 35<*oszlop || *akna<1 || *akna>(2 * (*sor)*(*oszlop)) / 3)
			hiba = 1;
		else if (foglal(t, *sor, *oszlop)) {
			hiba = 1;
		}
		else {
			for (y = 0; y<*sor; y++) {
				for (x = 0; x<*oszlop; x++) {
					(*t)[y][x] = getc(game);
					if (((*t)[y][x]<'0' && (*t)[y][x] != 0) || ('8'<(*t)[y][x] && (*t)[y][x] != 'B')) {
						hiba = 1;
						szabad(t, *sor);
						break;
					}
				}
				getc(game);
				if (hiba)
					break;
			}
		}
		fclose(game);
	}
	else
		hiba = 1;
	return hiba;
}
int tarol(int **t, int sor, int oszlop, int akna) {
	/*게임을 저장합니다*/
	FILE *game = NULL;
	int y, x, hiba = 0;
	game = fopen("Save.txt", "wt");
	if (game) {
		fprintf(game, "%d*%d;%d\n", sor, oszlop, akna);
		for (y = 0; y<sor; y++) {
			for (x = 0; x<oszlop; x++)
				putc(t[y][x], game);
			if (putc('\n', game) == EOF) {
				hiba = 1;
				break;
			}
		}
		fclose(game);
	}
	else
		hiba = 1;
	return hiba;
}
int olvas(int **t, int sor, int oszlop, int akna, int *nyero) {
	/*반환값이 -1일 경우 재시작, 0일 경우 종료, 1일 경우 저장.*/
	int i, y, x;
	printf("지뢰가 없는 곳은?\n예시: 5:6\n0:1 저장하고 메인메뉴\n0:2 메인메뉴\n0:3 재시작\n");
	while (1) {
		/*사용자의 키입력을 받습니다. y값이 0일 경우와 아닐 경우로 나누어지며 y와 x값이 행열의 범위를 넘어가면 안됩니다.*/
		scanf("%d:%d", &y, &x);
		system("cls");
		if (y == 0 && (x == 1 || x == 2 || x == 3)) {
			i = 1;
			break;
		}
		if (0<y && y <= sor && 0<x && x <= oszlop) {
			i = 0;
			break;
		}
		printf("행과 열을 벗어났습니다. 다시 입력하십시오.\n0:1 저장하고 메인메뉴\n0:2 종료\n0:3 새 게임\n");
	}
	if (i)
		//게임을 저장하거나 재시작합니다.
		switch (x) {
		case 1:
			if (tarol(t, sor, oszlop, akna)) {
				fprintf(stderr, "메모리 오류! 이 게임은 저장할 수 없습니다.\n");
				return 1;
			}
			else return 0;
			break;
		case 2:
			return 0;
			break;
		case 3:
			return -1;
			break;
		}
	else
		*nyero = vizsgal(t, sor, oszlop, y - 1, x - 1);
	return 1;
}
void kiir(int **t, int sor, int oszlop, int akna) {
	/*화면에 지뢰밭을 표시합니다.*/
	int x, y;
	if (akna) {
		putchar(' ');
		putchar(' ');
		for (x = 1; x <= oszlop; x++) {
			if (x % 10)
				printf("%2d", x % 10);
			else
				printf("%2d", x);
		}
		putchar('\n');
		for (y = 0; y<sor; y++) {
			if ((y + 1) % 10)
				printf("%2d", (y + 1) % 10);
			else
				printf("%2d", y + 1);
			for (x = 0; x<oszlop; x++) {
				putchar(' ');
				if (t[y][x])
					putchar(t[y][x]);
				else putchar('-');
			}
			putchar('\n');
		}
	}
	else {
		putchar(' ');
		putchar(' ');
		for (x = 1; x <= oszlop; x++) {
			if (x % 10)
				printf("%2d", x % 10);
			else
				printf("%2d", x);
		}
		putchar('\n');
		for (y = 0; y<sor; y++) {
			if ((y + 1) % 10)
				printf("%2d", (y + 1) % 10);
			else printf("%2d", y + 1);
			for (x = 0; x<oszlop; x++) {
				putchar(' ');
				if ('0' <= t[y][x] && t[y][x] <= '8') {
					putchar(t[y][x]);
				}
				else putchar('-');
			}
			putchar('\n');
		}
	}
}
void tomb(int sor, int oszlop, int akna) {
	/*Ez vezerli a jatekot.
	Kepes hivni az indit, a betolt es a kiir fv.-eket.*/
	int **t = NULL, folytat = -1, nyero;
	while (folytat == -1) {
		if (5 <= sor && sor <= 35 && 5 <= oszlop && oszlop <= 35 && akna <= (2 * sor*oszlop) / 3) {
			if (indit(&t, sor, oszlop, akna)) {
				fprintf(stderr, "메모리 오류!\n");
				break;
			}
		}
		else if (betolt(&t, &sor, &oszlop, &akna)) {
			fprintf(stderr, "저장된 게임이 없습니다.\n");
			break;
		}
		folytat = 1;
		nyero = 0;
		kiir(t, sor, oszlop, nyero);
		while (folytat == 1 && nyero == 0) {
			folytat = olvas(t, sor, oszlop, akna, &nyero);
			if (folytat == 1)
				kiir(t, sor, oszlop, nyero);
			switch (nyero) {
			case 1:
				printf("축하합니다! 승리하셨습니다.\n");
				break;
			case -1:
				printf("안타깝습니다. 다시 시도해보세요.!\n");
				break;
			}
		}
	}
	if (folytat != -1)
		szabad(&t, sor);
}
void belep() {
	/*사용자지정 맵을 만듭니다.*/
	int i, sor, oszlop, akna;
	printf("크기 입력!\n형식 : 행*열\n메인메뉴 : 0*0\n");
	while (1) {
		scanf("%d*%d", &sor, &oszlop);
		system("cls");
		//행열의 값이 0*0이면 메인메뉴로 돌아갑니다.
		if (sor == 0 && oszlop == 0) {
			i = 0;
			break;
		}
		//행열의 크기는 5~35사이로 제한합니다.
		if (5 <= sor && 5 <= oszlop && sor <= 35 && oszlop <= 35) {
			i = 1;
			break;
		}
		printf("형식 : 행*열\n행과 열은 최소 5, 최대 35 입니다.\n메인메뉴 : 0*0\n");
	}
	if (i) {
		i = (2 * sor*oszlop) / 3;
		printf("지뢰 개수를 입력!\n");
		while (1) {
			scanf("%d", &akna);
			system("cls");
			if (5 <= akna && akna <= i)
				break;
			printf("지뢰의 최소 5, 최대 %d 입니다.\n", i);
		}
		tomb(sor, oszlop, akna);
	}
}
int main() {
	int menu, alm;
	FILE *tutorial = NULL;
	while (5) {
		/*메인 메뉴*/
		printf("0 종료\n1 새 게임\n2 게임 불러오기\n");
		scanf("%d", &menu);
		system("cls");
		switch (menu) {
		case 0:
			return 0;
			break;
		case 1:
			printf("0 뒤로 가기\n1: 10*10; 10 지뢰\n2: 20*20; 50 지뢰\n3: 35*35; 200 지뢰\n4 사용자 지정\n");
			scanf("%d", &alm);
			system("cls");
			switch (alm) {
			case 0:
				;
				break;
			case 1:
				tomb(10, 10, 10);
				break;
			case 2:
				tomb(20, 20, 50);
				break;
			case 3:
				tomb(35, 35, 200);
				break;
			case 4:
				belep();
				break;
			default:
				printf("잘못된 입력입니다..\n");
				break;
			}
			break;
		case 2:
			tomb(0, 0, 0);
			break;
		default:
			printf("잘못된 입력입니다..\n");
			break;
		}
	}
}
