#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>
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
struct dugum1 *ilk = NULL,*son = NULL, *son_eklenen;

void ekle(int plaka);
void sil(int x) ;
void listele();
void dosyadan_ekle();
int plaka_bul(char *sehir_ismi,int cursor);
FILE *dosya; // sehirler.txt ye her yerden ulasabilmek icin global tanımladik.
int sehir_sayisi=1;// satir sayisi enter sayisinin bir fazlası olacagi icin 1 den baslattik.


int main() {
    dosya = fopen("sehirler.txt","r");//sehirlerin bulundugu dosya okuma modunda acılıyor.
    if(dosya==NULL) {
        printf("Sehir Bilgilerini Barındiran Dosya Acilamadi! Manuel Sehir Ekleme Aktif...");
    } else {
        printf("\t\t\t\tsehir listesini iceren dosya basarili bir sekilde acildi...\n");
    }
    dosyadan_ekle();
    /*ekle(25,"Eskisehir","AN",5);
    ekle(23,"Erzgkykyk","DA",3);
    ekle(27,"Erzurum","DA",3);
    ekle(29,"Erzurum","DA",3);
    ekle(28,"Duzce","AN",5);
    ekle(24,"Ankara","AN",6);
    ekle(29,"Istanbul","MA",1);
    ekle(26,"Istanbul","MA",1);*/
    printf("\n");
    listele();
    sil(23);
    printf("\n");
    printf("\n");
    listele();
    sil(27);
    sil(36);
    //sil(34);

    printf("\n");
    printf("\n");
    listele();
}


void dosyadan_ekle() {

    char sehir_satiri[100];
    int ch;
    for(ch = fgetc(dosya); ch!=EOF; ch = getc(dosya)) {
        if(ch == '\n') {
            sehir_sayisi++;
        }
    }
    int cursor;
    fseek(dosya,0,SEEK_SET); // cursoru basa almazsak dosyayı okuyamayiz cunku sehir sayisini bulurken cursorun yeri degisti.

    for(int i=0; i<sehir_sayisi; i++) { // elde edilen satirlari(sehirleri) virgule gore parcaliyoruz.
        int plaka;
        int komsu_plaka;
        char * sehir;
        char * bolge;
        int komsu_sayisi;
        fgets(sehir_satiri,100,dosya);
        strtok(sehir_satiri,"\n");
        // printf("\n%s\n",sehir_satiri);
        char *ptr = strtok(sehir_satiri,",");
        int ayrac=0;// her bir satirda cursorun hangi virgulde kaldigini tutmamiz icin.
        komsu_sayisi=0; // komsu sayisini her dongude sifirlamamiz lazim.
        while(ptr != NULL) {
            struct dugum2 *last=NULL;
            cursor=ftell(dosya);
            if(ayrac==0) {
                plaka=atoi(ptr);
                ekle(plaka);
            } else if(ayrac==1) {
                const size_t len_my_str = strlen(ptr) + 1;
                sehir = malloc(len_my_str);
                strncpy(sehir, ptr, len_my_str);
                son_eklenen->sehir=sehir;
            } else if(ayrac==2) {
                const size_t len_my_str = strlen(ptr) + 1;
                bolge = malloc(len_my_str);
                strncpy(bolge, ptr, len_my_str);
                son_eklenen->bolge=bolge;
            } else { //bu kisma komsularin plakasinin da atanmasi gelecek;
                komsu_sayisi++;
                komsu_plaka=plaka_bul(ptr,cursor);

                //bu kisimda son_eklenen dugumun sub sehirlerine atama yapilacak.
            }
            ptr = strtok(NULL, ",");
            ayrac++;
        }
        son_eklenen->komsu_sayisi=komsu_sayisi;
    }
}

int plaka_bul(char *sehir_ismi,int cursor) {
    fseek(dosya,0,SEEK_SET);
    for(int i=0; i<sehir_sayisi; i++) { // elde edilen satirlari(sehirleri) virgule gore parcaliyoruz.
        int plaka;
        char * sehir;
        char sehir_satiri[100];
        fgets(sehir_satiri,100,dosya);
        strtok(sehir_satiri,"\n");
        const char *ptr = strtok(sehir_satiri,",");
        int ayrac=0;// her bir satirda cursorun hangi virgulde kaldigini tutmamiz icin.
        while(ptr != NULL) {
            if(ayrac==0) {
                plaka=atoi(ptr);
            } else if(ayrac==1) {
                const size_t len_my_str = strlen(ptr) + 1;
                sehir = malloc(len_my_str);
                strncpy(sehir, ptr, len_my_str);
                if(strcmp(sehir_ismi,sehir)==0) {
                    fseek(dosya,cursor,SEEK_SET);
                    return plaka;
                }
            }
            ptr = strtok(NULL, ",");
            ayrac++;
        }
    }
    //bu kisima plaka bulunmaz ise eklemek ister misiniz secenegi eklenecek.
    fseek(dosya,cursor,SEEK_SET);
    return 0;
}

void ekle(int plaka) {
    struct dugum1 *yeni = (struct dugum1*) malloc(sizeof(struct dugum1));
    struct dugum1 *eklenecek_yer=ilk;
    yeni->plaka=plaka;
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
            // 3. durum yani plakanin zaten agacta oldugu durum.
            printf("Boyle bir plaka zaten mevcut. Eklenemiyor...");
            free(yeni);
            return;
        }

    }
    son_eklenen=yeni;
}


void sil(int x) {
    struct dugum1 *sil=ilk,*prev_1 = NULL;
    while(sil!=NULL) {
        if(sil->plaka==x)
            break;
        else {
            sil=sil->sonraki;
        }

    }
    if(sil!=NULL) {
        if (sil == ilk) {
            prev_1 = ilk->onceki;
            ilk = ilk->sonraki;
            prev_1->sonraki = ilk;
            sil->onceki = prev_1;
        }  else if(sil==son) {
            sil->onceki->sonraki=sil->sonraki;
            sil->sonraki->onceki=sil->onceki;
            son=son->onceki;
        } else {
            sil->onceki->sonraki=sil->sonraki;
            sil->sonraki->onceki=sil->onceki;
        }

    }
    printf("\n%d plakali %s sehri silindi",sil->plaka,sil->sehir);
    free(sil);

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
    printf("%s\t\n" ,liste->sehir);
}
