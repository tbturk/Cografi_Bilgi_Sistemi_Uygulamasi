#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct dugum1 {
    int plaka;
    char *sehir; //Turkiyenin en uzun isme sahip sehri "Kahramanmaras" oldugu icin.
    char *bolge;
    int komsu_sayisi;
    struct dugum1* sonraki;
    struct dugum1* onceki;
    struct dugum2* komsu_plaka;
};
struct dugum2 { //tek boyutlu int arraylist olacak.
    int plaka;
    struct dugum2* onceki;
    struct dugum2* sonraki;
};
struct dugum1* ilk=NULL,*son=NULL;

void ekle(int plaka, const char *sehir, const char *bolge, int komsu_sayisi);
void sil(int x) ;
void listele();
void dosyadan_ekle();

FILE *dosya; // sehirler.txt ye her yerden ulasabilmek icin global tanımladik.

int main() {
    dosya = fopen("sehirler.txt","r");//sehirlerin bulundugu dosya okuma modunda acılıyor.
    if(dosya==NULL) {
        printf("Dosya Acilamadi! Manuel Sehir Ekleme Aktif...");
    } else {
        printf("\t\t\t\tsehir listesini iceren dosya basarili bir sekilde acildi...\n");
    }
    dosyadan_ekle();
    ekle(25,"Eskisehir","AN",5);
    ekle(23,"Erzurum","DA",3);
    ekle(27,"Erzurum","DA",3);
    ekle(29,"Erzurum","DA",3);
    ekle(28,"Duzce","AN",5);
    ekle(24,"Ankara","AN",6);
    ekle(29,"Istanbul","MA",1);
    ekle(26,"Istanbul","MA",1);
    printf("\n");
    listele();
    sil(26);
    printf("\n");
    printf("\n");
    listele();
    sil(29);
    sil(23);

    printf("\n");
    printf("\n");
    listele();
}


void dosyadan_ekle() {
    char sehir[100];
    int sehir_sayisi=1;// satir sayisi enter sayisinin bir fazlası olacagi icin 1 den baslattik.
    int ch;
    for(ch = fgetc(dosya); ch!=EOF; ch = getc(dosya)) {
        if(ch == '\n') {
            sehir_sayisi++;
        }
    }

    fseek(dosya,0,SEEK_SET); // cursoru basa almazsak dosyayı okuyamayiz cunku sehir sayisini bulurken cursorun yeri degisti.

    for(int i=0; i<sehir_sayisi; i++) { // elde edilen satirlari(sehirleri) virgule gore parcaliyoruz.
        fgets(sehir,100,dosya);
        strtok(sehir,"\n");
        printf("\n%s\n",sehir);
        char *ptr = strtok(sehir,",");

        while(ptr != NULL) {
            printf("'%s'\n", ptr);
            ptr = strtok(NULL, ",");
        }
    }
}

void ekle(int plaka, const char *sehir, const char *bolge, int komsu_sayisi) {
    struct dugum1 *yeni = (struct dugum1*) malloc(sizeof(struct dugum1));
    struct dugum1 *eklenecek_yer=ilk;
    yeni->plaka=plaka;
    yeni->sehir=sehir;
    yeni->bolge=bolge;
    if(ilk==NULL) {
        ilk=yeni;
        son=yeni;
        son->sonraki=ilk;
        son->onceki=ilk;
    } else {
        if(yeni->plaka < ilk->plaka) {
            //printf("1");
            son->sonraki=yeni;
            yeni->sonraki=ilk;
            yeni->onceki=son;
            ilk->onceki=yeni;
            ilk=yeni;
        } else if(yeni->plaka >= ilk->plaka && yeni->plaka <= son->plaka) {
            //printf("2");
            while(eklenecek_yer!=NULL) {
                if(eklenecek_yer->plaka > yeni->plaka)
                    break;
                else if(eklenecek_yer->plaka == yeni->plaka) {
                    free(yeni);
                    return;
                } else {
                    eklenecek_yer=eklenecek_yer->sonraki;
                    //printf("%d",eklenecek_yer->plaka);
                }
            }
            yeni->sonraki = eklenecek_yer;
            yeni->onceki = eklenecek_yer->onceki;
            eklenecek_yer->onceki->sonraki = yeni;
            eklenecek_yer->onceki = yeni;

        } else if(yeni->plaka > son->plaka) {
            //printf("3");
            son->sonraki= yeni;
            yeni->onceki=son;
            yeni->sonraki=ilk;
            son=yeni;



        } else {
            // 3. durum yani plakanýn zaten agacta oldugu durum.
            printf("Boyle bir plaka zaten mevcut. Eklenemiyor...");
            free(yeni);
            return;
        }

    }
}

void sil(int x) {
    struct dugum1 *sil=ilk;
    while(sil!=NULL) {
        if(sil->plaka==x)
            break;
        else {
            sil=sil->sonraki;
        }

    }
    if(sil!=NULL) {
        if(sil==ilk) {
            sil->onceki->sonraki=sil->sonraki;
            sil->sonraki->onceki=sil->onceki;
            ilk=ilk->sonraki;
        } else if(sil==son) {
            sil->onceki->sonraki=sil->sonraki;
            sil->sonraki->onceki=sil->onceki;
            son=son->onceki;
        } else {
            sil->onceki->sonraki=sil->sonraki;
            sil->sonraki->onceki=sil->onceki;
        }
        free(sil);
    }

}
void listele() {
    struct dugum1 *liste=ilk;
    while(liste->sonraki!=ilk) {
        printf("%d\t",liste->plaka);
        printf("%s\t",liste->bolge);
        printf("%s\t",liste->sehir);
        liste=liste->sonraki;
        printf("\n");
    }
    printf("%d\t",liste->plaka);
    printf("%s\t",liste->bolge);
    printf("%s\t",liste->sehir);
}
