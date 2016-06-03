#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
void sugo() {
	/*File-bol ir a kepernyore.*/
	FILE *tutorial = NULL;
	int karakter;
	tutorial = fopen("Help.txt", "rt");
	if (tutorial != NULL) {
		karakter = 1;
		while (karakter != EOF) {
			karakter = fgetc(tutorial);
			if (karakter != EOF) printf("%c", karakter);
		}
		printf("\n");
		fclose(tutorial);
	}
}
void tapogat(int **t, int sor, int oszlop, int y, int x) {
	/*Letapogatja, hany szomszedos mezon van akna. Az erteket a tombben tarolja.
	Ha 0, rekurzioval megvizsgalja a szomszedos mezoket is.*/
	/*Parameterlista: tomb es merete, kerdeses mezo y es x koordinataja.*/
	int akna = 0;
	/*Ehhez a szamlalashoz inkabb meg se probaltam ciklust irni.*/
	if (y - 1 >= 0 && x - 1 >= 0 && t[y - 1][x - 1] == 'B') akna++;
	if (y - 1 >= 0 && 1 && t[y - 1][x] == 'B') akna++;
	if (y - 1 >= 0 && x + 1<oszlop && t[y - 1][x + 1] == 'B') akna++;
	if (1 && x - 1 >= 0 && t[y][x - 1] == 'B') akna++;
	if (1 && x + 1<oszlop && t[y][x + 1] == 'B') akna++;
	if (y + 1<sor &&     x - 1 >= 0 && t[y + 1][x - 1] == 'B') akna++;
	if (y + 1<sor && 1 && t[y + 1][x] == 'B') akna++;
	if (y + 1<sor && x + 1<oszlop && t[y + 1][x + 1] == 'B') akna++;
	t[y][x] = akna + '0'; /*ASCII-kod tarolasa*/
	if (t[y][x] == '0') {
		/*A rekurziot is kireszleteztem.*/
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
	/*Parameterek:a t�mb cime(nullpointer), merete.
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
	/*Tombinicializalo.
	A tomb elemeit kinullazza, majd veletlenszer�en kitolti aknakat jelento 'B' karakterekkel(ASCII).
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
	/*Mentett jatekot betolto fv.
	Fajlbol beolvassa a tomb meretet, meghivja hozza a foglal fv.-t,
	majd feltolti a tombot a fajlban talalhato elemekkel.
	Ugyel a sor, oszlop es akna valtozok ervenyessegenek felteteleire.
	Ha problema lep fel, a pointer nulla lesz, a visszateresi ertek pedig 1.*/
	/*Parameterek: tomb es meretenek pointerei.
	Visszateres: 0: siker; 1: kudarc*/
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
	/*Jatekmento fuggveny.
	A tomb meretet es elemeit fajlban tarolja.
	Sikeressegerol visszateresi ertekkel jelez.*/
	/*Parameterlista: tomb es merete.
	Visszateres: 1: sikertelen, 0: sikeres*/
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
	/*Ez kommunikal a felhasznaloval a jatek futasakor, ez hivja meg a vizsgal es a tarol fv-t.
	Parameterlistan leadj a vizsgal visszateresi erteket,
	valamint visszateresi ertekkel jelzi a jatekos szandekat:
	ujrakezdes, folytatas, kilepes.*/
	/*Parameterlistan: tomb es merete, cim a vizsgal visszateresi ertekenek.
	Visszateresi ertekek: -1: uj jatek, 0: kilepes, 1: folytatas.*/
	int i, y, x;
	printf("Melyik mezon nincs akna?\nPl.: 5:6\n0:1 Mentes es kilepes\n0:2 Kilepes\n0:3 Uj jatek\n");
	while (1) {
		/*A ciklusra a hibasan beirt adatok miatt van szukseg.
		A ciklus megszakad, ha a felhasznalo ertelmezheto adatokat visz be.*/
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
		printf("A helyes formatum: sor:oszlop\n0:1 Mentes es kilepes\n0:2 Kilepes\n0:3 Uj jatek\n");
	}
	if (i)
		switch (x) {
		case 1:
			if (tarol(t, sor, oszlop, akna)) {
				fprintf(stderr, "Memoriahiba! A jatek nem mentheto.\n");
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
		/*A jatekos 1-tol n-ig adhat meg szamokat, a tomb viszont 0-tol (n-1)-ig van indexelve.*/
		*nyero = vizsgal(t, sor, oszlop, y - 1, x - 1);
	return 1;
}
void kiir(int **t, int sor, int oszlop, int akna) {
	/*Megjeleniti az aknamezot a kepernyon.
	Az int akna parameter logikai erteket var:
	igaz: a 'B'-k is megjelennek (a jatek vegen, amikor kiderul az igazsag)
	hamis: csak a szomsz�dos aknak szamat jelzo szamok jelennek meg*/
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
				fprintf(stderr, "Memoriahiba!\n");
				break;
			}
		}
		else if (betolt(&t, &sor, &oszlop, &akna)) {
			fprintf(stderr, "Nincs folytathato jatek.\n");
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
				printf("Gratulalok! Nyertel!\n");
				break;
			case -1:
				printf("Sajnos most nem nyertel! Probald ujra!\n");
				break;
			}
		}
	}
	if (folytat != -1)
		szabad(&t, sor);
}
void belep() {
	/*Ez kommunikal a felhasznaloval kozvetlenul a jatek indulasa elott, ha a jatekos egyeni nehezsegi szintet akar.
	Keri a sorok, oszlopok, aknak szamat, majd meghivja a tomb fv.-t.*/
	int i, sor, oszlop, akna;
	printf("Adj meretet!\nPl.: 20*30\nMegse: 0*0\n");
	while (1) {
		/*Ez a ciklus is akkor szakad meg, amikor nincs szukseg a hibauzenetre.*/
		scanf("%d*%d", &sor, &oszlop);
		system("cls");
		if (sor == 0 && oszlop == 0) {
			i = 0;
			break;
		}
		if (5 <= sor && 5 <= oszlop && sor <= 35 && oszlop <= 35) {
			i = 1;
			break;
		}
		printf("A helyes formatum: sor*oszlop\nA sorok es oszlopok szama legalabb 5, legfeljebb 35 kehet.\nMegse: 0*0\n");
	}
	if (i) {
		i = (2 * sor*oszlop) / 3;
		printf("Add meg az aknak szamat!\n");
		while (1) {
			scanf("%d", &akna);
			system("cls");
			if (5 <= akna && akna <= i)
				break;
			printf("Ekkora meret eseten az aknak szama legalabb 5, legfeljebb %d lehet.\n", i);
		}
		tomb(sor, oszlop, akna);
	}
}
int main() {
	/*Tartalmazza a fomenut, kepes a belep, a tomb es a sugo fv.-ek meghivasara.*/
	int menu, alm;
	FILE *tutorial = NULL;
	while (5) {
		/*Fomenu. Eleg, ha kilepeskor van vege.*/
		printf("0 kilepes\n1 sugo\n2 uj jatek\n3 jatek folytatasa\n");
		scanf("%d", &menu);
		system("cls");
		switch (menu) {
		case 0:
			return 0;
			break;
		case 1:
			sugo();
			break;
		case 2:
			printf("0 vissza\n1: 10*10; 10 akna\n2: 20*20; 50 akna\n3: 35*35; 200 akna\n4 mas\n");
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
				printf("Ervenytelen billentyu.\n");
				break;
			}
			break;
		case 3:
			tomb(0, 0, 0);
			break;
		default:
			printf("Ervenytelen billentyu.\n");
			break;
		}
	}
}
