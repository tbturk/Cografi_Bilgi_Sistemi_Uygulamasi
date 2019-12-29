#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
struct dugum1 {// sehir dugumumuz
    int plaka;
    char *sehir; //Turkiyenin en uzun isme sahip sehri "Kahramanmaras" oldugu icin.
    char *bolge;
    int komsu_sayisi;
    struct dugum1* sonraki;
    struct dugum1* onceki;
    struct dugum2* komsu_plaka;
};
struct dugum2 { //tek boyutlu int arraylist olacak.  komsuluk dugumumuz.
    int plaka;
    struct dugum2* sonraki;
};
struct dugum1 *ilk = NULL,*son = NULL, *son_eklenen;
struct dugum2 *son_eklenen_komsuluk;

void ekle(int plaka);
void sil(int x) ;
void listele();
void dosyadan_ekle();
int plaka_bul(char *sehir_ismi);
void secimEkrani();
int islemSecim();
void komsuSayisiAl(int komsu_S);
FILE *dosya2;
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
    secimEkrani();
    islemSecim();
    dosyadan_ekle();
    sehirBilgiGoster("26");
    listele();
    /*ekle(25,"Eskisehir","AN",5);
    ekle(23,"Erzgkykyk","DA",3);
    ekle(27,"Erzurum","DA",3);
    ekle(29,"Erzurum","DA",3);
    ekle(28,"Duzce","AN",5);
    ekle(24,"Ankara","AN",6);
    ekle(29,"Istanbul","MA",1);
    ekle(26,"Istanbul","MA",1);*/
    /*printf("\n");
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
    bolgeBul("AK");*/
}


void dosyadan_ekle() {

    char sehir_satiri[100];
    int ch;
    for(ch = fgetc(dosya); ch!=EOF; ch = getc(dosya)) {
        if(ch == '\n') {
            sehir_sayisi++;
        }
    }

    int cursor=0;
    fseek(dosya,0,SEEK_SET); // cursoru basa almazsak dosyayı okuyamayiz cunku sehir sayisini bulurken cursorun yeri degisti.

    for(int i=0; i<sehir_sayisi; i++) { // elde edilen satirlari(sehirleri) virgule gore parcaliyoruz.

        fseek(dosya,cursor,SEEK_SET);
        int plaka;
        int komsu_plaka;
        char * sehir;
        char * bolge;
        char isimm[100]="";
        int komsu_sayisi;
        fgets(sehir_satiri,100,dosya);
        cursor=ftell(dosya);
        strtok(sehir_satiri,"\n");
        // printf("\n%s\n",sehir_satiri);
        char *ptr = strtok(sehir_satiri,",");
        int ayrac=0;// her bir satirda cursorun hangi virgulde kaldigini tutmamiz icin.
        komsu_sayisi=0; // komsu sayisini her dongude sifirlamamiz lazim.
        while(ptr != NULL) {
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
            } else if(ayrac>=3) { //bu kisma komsularin plakasinin da atanmasi gelecek;
                komsu_sayisi++;
                if(komsu_sayisi==1) {
                    strcat(isimm,ptr);
                } else {
                    strcat(isimm,"+");
                    strcat(isimm,ptr);
                }
                // komsu_plaka=plaka_bul("askim");

            }
            ayrac++;
            ptr = strtok(NULL, ",");
        }
        char komsular[komsu_sayisi][20];
        int x=0;
        char *ptric = strtok(isimm,"+");
        while(ptric!=NULL) {
            //komsular[x]=ptric;
            strcpy(komsular[x],ptric);
            ptric = strtok(NULL, "+");
            x++;

        }
        for(int y=0; y<komsu_sayisi; y++) {
            komsu_plaka=plaka_bul(komsular[y]);
            struct dugum2 *yeni=(struct dugum2*)malloc(sizeof(struct dugum2));
            yeni->plaka=komsu_plaka;
            if(y==0) {
                son_eklenen->komsu_plaka=yeni;
                yeni->sonraki=NULL;
                son_eklenen_komsuluk=yeni;
            } else {
                son_eklenen_komsuluk->sonraki=yeni;
                yeni->sonraki=NULL;
                son_eklenen_komsuluk=yeni;
            }
            // printf("%d\n",komsu_plaka);
        }

        son_eklenen->komsu_sayisi=komsu_sayisi;
    }
    fclose(dosya);
}

int plaka_bul(char *sehir_ismi) {
    dosya2 = fopen("bak.txt","r");
    fseek(dosya2,0,SEEK_SET);
    for(int i=0; i<sehir_sayisi; i++) { // elde edilen satirlari(sehirleri) virgule gore parcaliyoruz.
        int plaka;
        char * sehir;
        char sehir_satiri[100];
        fgets(sehir_satiri,100,dosya2);
        strtok(sehir_satiri,"\n");
        char *ptr = strtok(sehir_satiri,",");
        int ayrac=0;// her bir satirda cursorun hangi virgulde kaldigini tutmamiz icin.
        while(ptr != NULL) {
            if(ayrac==0) {
                plaka=atoi(ptr);
            } else if(ayrac==1) {
                const size_t len_my_str = strlen(ptr) + 1;
                sehir = malloc(len_my_str);
                strncpy(sehir, ptr, len_my_str);
                if(strcmp(sehir_ismi,ptr)==0) {
                    fclose(dosya2);
                    return plaka;
                }
            }
            ptr = strtok(NULL, ",");
            ayrac++;
        }
    }
    //bu kisima plaka bulunmaz ise eklemek ister misiniz secenegi eklenecek.
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
    if(ilk==NULL) {
        printf("Listede herhangi bir sehir olmadigi icin silinemiyor...\n");
        return;
    }
    while(sil!=NULL) {
        if(sil->plaka==x)
            break;
        else {
            sil=sil->sonraki;
            if(sil==ilk) {
                printf("%d plakali sehir listede bulunamadi. Silinemiyor...\n",x);
                return;
            }
        }

    }
    if(sil!=NULL) {
        if (sil == ilk) {
            prev_1 = ilk->onceki;
            ilk = ilk->sonraki;
            prev_1->sonraki = ilk;
            sil->onceki = prev_1;
        } else if(sil==son) {
            sil->onceki->sonraki=sil->sonraki;
            sil->sonraki->onceki=sil->onceki;
            son=son->onceki;
        } else {
            sil->onceki->sonraki=sil->sonraki;
            sil->sonraki->onceki=sil->onceki;
        }

    }

    printf("\n%d plakali %s sehri silindi\n",sil->plaka,sil->sehir);
    free(sil);

}
void listele() {
    struct dugum1 *liste=ilk;
    while(liste!=son) {
        printf("%d\t",liste->plaka);
        printf("%s\t",liste->bolge);
        printf("%s\t",liste->sehir);
        printf("%d\t",liste->komsu_sayisi);
        son_eklenen_komsuluk=liste->komsu_plaka;
        while(son_eklenen_komsuluk!=NULL) {
            printf("\t---> %d",son_eklenen_komsuluk->plaka);
            son_eklenen_komsuluk=son_eklenen_komsuluk->sonraki;
        }
        //printf(" ----->%d ----->%d\n",liste->komsu_plaka->plaka,liste->komsu_plaka->sonraki->plaka);
        liste=liste->sonraki;
        printf("\n");
    }
    printf("%d\t",liste->plaka);
    printf("%s\t",liste->bolge);
    printf("%s\t",liste->sehir);
    printf("%d\t\n",liste->komsu_sayisi);
    while(son_eklenen_komsuluk!=NULL) {
        printf("\t---> %d",son_eklenen_komsuluk->plaka);
        son_eklenen_komsuluk=son_eklenen_komsuluk->sonraki;
    }


    //printf(" ----->%d ----->%d\n",liste->komsu_plaka->plaka,liste->komsu_plaka->sonraki->plaka);

}

void sehirBilgiGoster(char *sehir_ismi) {
    //int x = atoi(sehir_ismi);
    char *carpi;
    const size_t len_my_str = strlen(sehir_ismi) + 1;
    carpi = malloc(len_my_str);
    strncpy(carpi, sehir_ismi, len_my_str);
    int x = atoi(carpi);
    fseek(dosya,0,SEEK_SET);
    char sehir_satiri[100];
    char sehir_satiri_temp[100];
    char *sehir;
    if(x==0) {
        for(int i=0; i<sehir_sayisi; i++) {
            fgets(sehir_satiri,100,dosya);
            strcpy(sehir_satiri_temp,sehir_satiri);
            strtok(sehir_satiri,"\n");
            const char *ptr = strtok(sehir_satiri,",");
            const char *sehir;
            int ayrac=0;// her bir satirda cursorun hangi virgulde kaldigini tutmamiz icin.
            while(ptr != NULL) {
                if(ayrac==1) {
                    const size_t len_my_str = strlen(ptr) + 1;
                    sehir = malloc(len_my_str);
                    strncpy(sehir, ptr, len_my_str);
                    if((strcmp(sehir,sehir_ismi)==0)) {
                        printf("%s",sehir_satiri_temp);
                        return;
                    }

                }
                ptr = strtok(NULL, ",");
                ayrac++;

            }
        }
    } else {
        fseek(dosya,0,SEEK_SET); // Cursor tekrar dosya baslangicina alindi.
        for(int i=0; i<sehir_sayisi; i++) {
            fgets(sehir_satiri,100,dosya);
            strcpy(sehir_satiri_temp,sehir_satiri);
            strtok(sehir_satiri,"\n");
            const char *ptr = strtok(sehir_satiri,",");
            const char *sehir;
            int ayrac=0;// her bir satirda cursorun hangi virgulde kaldigini tutmamiz icin.
            while(ptr != NULL) {
                if(ayrac==0) {
                    if(x==atoi(ptr)) {
                        printf("%s",sehir_satiri_temp);
                        return;
                    }
                }
                ptr = strtok(NULL, ",");
                ayrac++;
            }

        }
    }
    printf("Boyle bir sehir bulunamadi!");
    return;
}


void bolgeBul(char *bolge_ismi) {
    fseek(dosya,0,SEEK_SET); // Cursor tekrar dosya baslangicina alindi.
    char sehir_satiri[100];
    char sehir_satiri_temp[100];
    for(int i=0; i<sehir_sayisi; i++) {
        fgets(sehir_satiri,100,dosya);
        strcpy(sehir_satiri_temp,sehir_satiri);
        strtok(sehir_satiri,"\n");
        const char *ptr = strtok(sehir_satiri,",");
        const char *bolge;
        int ayrac=0;// her bir satirda cursorun hangi virgulde kaldigini tutmamiz icin.
        while(ptr != NULL) {
            if(ayrac==2) {
                const size_t len_my_str = strlen(ptr) + 1;
                bolge = malloc(len_my_str);
                strncpy(bolge, ptr, len_my_str);

                if((strcmp(bolge_ismi,bolge))==0) {
                    printf("%s",sehir_satiri_temp);
                }
            }
            ptr = strtok(NULL, ",");
            ayrac++;
        }
    }
}
void secimEkrani() {

    printf("\nISLEMLER \n----------------------------------------- \n"
           "	1.) Dosya Icerigini Bas\n"
           "	2.) Sehir - Plaka Sorgulama\n"
           "	3.) Sehir Sil \n"
           "	4.) Bolgeye Gore Sehir Listele\n"
           "	5.) Komsu Sayisina Gore Listele \n"
           "	6.) Listeyi Dosyaya Bas\n"
           "    \t7.) Programi Kapat\n"
           "-----------------------------------------\n"
           "(Secim yapmak istediginiz islemin numarisini girin [1,7]):  ");
}

int islemSecim() {
    int secim;
    int devam;

    while(1) {
        scanf("%d",&secim);
        int plaka;
        int komsu_sayisi;
        char sehirBilgisi[10];
        char bolgeBilgisi[10];

        switch(secim) {
        case 1:
            printf("Dosya icerigi yaziliyor...\n");
            listele();
            break;
        case 2:
            printf("Sorgulama yapmak istediginiz sehrin plakasini ya da ismini giriniz: ");
            scanf("%s",sehirBilgisi);
            printf("Sehir - Plaka sorgulaniyor...\n");
            sehirBilgiGoster(sehirBilgisi);
            break;
        case 3:
            printf("Silinecek sehrin plakasini girin: ");
            scanf("%d",&plaka);
            printf("Sehir siliniyor...\n");
            sil(plaka);
            break;
        case 4:
            printf("Bolge Giriniz [AK, DA, EG, GA, IA, MA, KA] : ");
            scanf("%s",bolgeBilgisi);
            printf("Ayni bolgedeki sehirler listeleniyor...\n");
            bolgeBul(bolgeBilgisi);
            break;
        case 5:

            printf("Komsu sayisini giriniz: ");
            scanf("%d",&komsu_sayisi);
            printf("%d ve %d den fazla komsuya sahip sehirler listeleniyor...\n",komsu_sayisi);
            komsuSayisiAl(komsu_sayisi);
            break;
        case 6:
            // bu kisma dosyaya basma fonksiyonu gelecek.
            break;
        case 7:
            printf("Program kapatiliyor...\n");
            exit(0);
        default:
            printf("Yanlis bir giris yaptiniz lutfen [1,6] arasinda giris yapiniz\n");
            secim = 0;
            break;
        }

        if(secim != 0) { // yanlis giris olmadigi zaman while dan cik.
            break;
        }

    }

    printf("Yeni bir islem yapmak icin \"1\" giriniz: ");
    scanf("%d",&devam);
    if(devam == 1 ) {
        return 1;
    } else {
        printf("Yanlis giris yaptiniz program kapatiliyor...\n");
        return 0;
    }

}
void komsuSayisiAl(int komsu_S) {
    struct dugum1 *iter = ilk;

    while(iter->sonraki != ilk) {
        if(iter->komsu_sayisi >= komsu_S) {
            printf("%s'nin komsu sayisi %d\n",iter->sehir,iter->komsu_sayisi);
        }
        iter = iter->sonraki;
    }
    printf("%s'nin komsu sayisi %d\n",iter->sehir,iter->komsu_sayisi);
    /* Bu fonksiyon tamamlandiktan sonra Duzce sehri basiliyor mu basilmiyor mu kontrol et
        cunku duzceyi basarken de sorun yasiyorduk onu mutlaka kontrol et.*/


}
