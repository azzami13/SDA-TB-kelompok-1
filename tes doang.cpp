#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_PLATE_LENGTH 15
#define MAX_TYPE_LENGTH 20
#define MAX_CUCI_TIME 120 // Maksimum waktu tunggu sebelum tiket hangus (dalam menit)
#define OPERATIONAL_START 0 // Jam operasional mulai (00:00)
#define OPERATIONAL_END 24 // Jam operasional selesai (23:59)
#define BREAK_START 12 // Jam istirahat mulai (12 siang)
#define BREAK_END 13 // Jam istirahat selesai (13 siang)

// Struktur data untuk menyimpan informasi pemesanan
typedef struct TiketCuci {
    char nama[MAX_NAME_LENGTH];
    char plat_nomor[MAX_PLATE_LENGTH];
    char jenis_mobil[MAX_TYPE_LENGTH];
    int paket;
    int harga;
    int waktu;
    char pencuci[20];
    struct TiketCuci *prev;
    struct TiketCuci *next;
} TiketCuci;

// Head dan tail dari double linked list
TiketCuci *head = NULL;
TiketCuci *tail = NULL;

// Fungsi untuk membaca data tiket dari file teks
void bacaDataTiket() {
    FILE *file = fopen("tiket.txt", "r");
    if (file == NULL) {
        printf("File tiket.txt tidak ditemukan.\n");
        return;
    }

    while (!feof(file)) {
        TiketCuci *tiket = (TiketCuci *)malloc(sizeof(TiketCuci));
        if (tiket == NULL) {
            printf("Memori penuh.\n");
            return;
        }

        fscanf(file, "%s %s %s %d %d %d %s\n", tiket->nama, tiket->plat_nomor, tiket->jenis_mobil,
               &tiket->paket, &tiket->harga, &tiket->waktu, tiket->pencuci);

        if (head == NULL) {
            head = tail = tiket;
            tiket->prev = NULL;
            tiket->next = NULL;
        } else {
            tail->next = tiket;
            tiket->prev = tail;
            tiket->next = NULL;
            tail = tiket;
        }
    }

    fclose(file);
}

// Fungsi untuk menyimpan data tiket ke dalam file teks
void simpanDataTiket() {
    FILE *file = fopen("tiket.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file tiket.txt untuk ditulis.\n");
        return;
    }

    TiketCuci *current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %s %d %d %d %s\n", current->nama, current->plat_nomor, current->jenis_mobil,
                current->paket, current->harga, current->waktu, current->pencuci);
        current = current->next;
    }

    fclose(file);
}

// Fungsi untuk memesan tiket pencucian mobil
void pesanTiket() {
    TiketCuci *tiket = (TiketCuci *)malloc(sizeof(TiketCuci));
    if (tiket == NULL) {
        printf("Memori penuh.\n");
        return;
    }

    printf("Masukkan nama pemesan: ");
    fgets(tiket->nama, MAX_NAME_LENGTH, stdin);
    tiket->nama[strcspn(tiket->nama, "\n")] = '\0'; // Menghapus newline character

    printf("Masukkan plat nomor: ");
    fgets(tiket->plat_nomor, MAX_PLATE_LENGTH, stdin);
    tiket->plat_nomor[strcspn(tiket->plat_nomor, "\n")] = '\0'; // Menghapus newline character

    int pilihan_jenis;
    do {
        printf("Pilih jenis mobil:\n");
        printf("1. Sedan\n");
        printf("2. SUV\n");
        printf("3. Offroad\n");
        printf("0. Kembali\n");
        printf("Pilih: ");
        scanf("%d", &pilihan_jenis);
        getchar(); // Membersihkan buffer stdin

        switch (pilihan_jenis) {
            case 1:
                strcpy(tiket->jenis_mobil, "Sedan");
                break;
            case 2:
                strcpy(tiket->jenis_mobil, "SUV");
                break;
            case 3:
                strcpy(tiket->jenis_mobil, "Offroad");
                break;
            case 0:
                printf("Kembali ke menu utama.\n");
                free(tiket);
                return;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pilihan_jenis < 1 || pilihan_jenis > 3);

    printf("Pilih paket cuci (1-3):\n");
    printf("Jenis                                      Waktu (menit)               Harga\n");
    printf("--------------------------------------------------\n");
    if (strcmp(tiket->jenis_mobil, "Sedan") == 0) {
        printf("1. Paket Lengkap (cuci luar dalam)       40                        200,000\n");
        printf("2. Paket cuci luar                       20                        100,000\n");
        printf("3. Paket cuci dalam                      20                        150,000\n");
    } else if (strcmp(tiket->jenis_mobil, "SUV") == 0) {
        printf("1. Paket Lengkap (cuci luar dalam)       60                        300,000\n");
        printf("2. Paket cuci luar                       25                        150,000\n");
        printf("3. Paket cuci dalam                      35                        200,000\n");
    } else if (strcmp(tiket->jenis_mobil, "Offroad") == 0) {
        printf("1. Paket Lengkap (cuci luar dalam)       90                        300,000\n");
        printf("2. Paket cuci l                          40                        300,000\n");
        printf("3. Paket cuci dalam                      50                        500,000\n");
    } else {
        printf("Jenis mobil tidak valid.\n");
        free(tiket);
        return;
    }

    printf("Pilih: ");
    scanf("%d", &tiket->paket);
    getchar(); // Membersihkan buffer stdin

    // Menentukan harga dan waktu berdasarkan jenis mobil dan paket
    if (strcmp(tiket->jenis_mobil, "Sedan") == 0) {
        switch (tiket->paket) {
            case 1:
                tiket->harga = 200000;
                tiket->waktu = 40;
                break;
            case 2:
                tiket->harga = 100000;
                tiket->waktu = 20;
                break;
            case 3:
                tiket->harga = 150000;
                tiket->waktu = 20;
                break;
            default:
                printf("Pilihan paket tidak valid.\n");
                free(tiket);
                return;
        }
    } else if (strcmp(tiket->jenis_mobil, "SUV") == 0) {
        switch (tiket->paket) {
            case 1:
                tiket->harga = 300000;
                tiket->waktu = 60;
                break;
            case 2:
                tiket->harga = 150000;
                tiket->waktu = 25;
                break;
            case 3:
                tiket->harga = 200000;
                tiket->waktu = 35;
                break;
            default:
                printf("Pilihan paket tidak valid.\n");
                free(tiket);
                return;
        }
    } else if (strcmp(tiket->jenis_mobil, "Offroad") == 0) {
        switch (tiket->paket) {
            case 1:
                tiket->harga = 300000;
                tiket->waktu = 90;
                break;
            case 2:
                tiket->harga = 300000;
                tiket->waktu = 40;
                break;
            case 3:
                tiket->harga = 500000;
                tiket->waktu = 50;
                break;
            default:
                printf("Pilihan paket tidak valid.\n");
                free(tiket);
                return;
        }
    }

    // Memilih waktu mencuci
    printf("Masukkan jam berapa untuk mencuci (0-23): ");
    scanf("%d", &tiket->waktu);
    getchar(); // Membersihkan buffer stdin

    // Memeriksa ketersediaan pencuci mobil
    int pencuci1 = 0, pencuci2 = 0, pencuci3 = 0;
    TiketCuci *current = head;
    while (current != NULL) {
        if (current->waktu == tiket->waktu) {
            if (strcmp(current->pencuci, "Pencuci 1") == 0) {
                pencuci1++;
            } else if (strcmp(current->pencuci, "Pencuci 2") == 0) {
                pencuci2++;
            } else if (strcmp(current->pencuci, "Pencuci 3") == 0) {
                pencuci3++;
            }
        }
        current = current->next;
    }

    if (pencuci1 >= 2 || pencuci2 >= 2 || pencuci3 >= 2) {
        printf("Tiket untuk jam tersebut telah habis. Mohon coba di waktu lain.\n");
        free(tiket);
        return;
    }

    // Memilih pencuci mobil secara acak
    srand(time(NULL));
    int randNum = rand() % 3 + 1;
    sprintf(tiket->pencuci, "Pencuci %d", randNum);

    // Menambahkan tiket ke dalam double linked list
    if (head == NULL) {
        head = tail = tiket;
        tiket->prev = NULL;
        tiket->next = NULL;
    } else {
        tail->next = tiket;
        tiket->prev = tail;
        tiket->next = NULL;
        tail = tiket;
    }

    printf("Tiket berhasil dipesan.\n");
}

// Fungsi untuk mengecek tiket berdasarkan nama pelanggan
void cekTiket() {
    char nama[MAX_NAME_LENGTH];
    printf("Masukkan atas nama pelanggan: ");
    fgets(nama, MAX_NAME_LENGTH, stdin);
    nama[strcspn(nama, "\n")] = '\0'; // Menghapus newline character

    TiketCuci *current = head;
    int found = 0;
    while (current != NULL) {
        if (strcmp(current->nama, nama) == 0) {
            printf("Nama: %s\n", current->nama);
            printf("Plat Nomor: %s\n", current->plat_nomor);
            printf("Jenis Mobil: %s\n", current->jenis_mobil);
            printf("Paket: %d\n", current->paket);
            printf("Harga: %d\n", current->harga);
            printf("Waktu Tunggu: %d menit\n", current->waktu);
            printf("Pencuci Mobil: %s\n", current->pencuci);
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Tiket tidak ditemukan. Pastikan nama sesuai.\n");
    }
}

// Fungsi untuk menampilkan kontak call center dan email
void kontak() {
    printf("Call Center: 123456789\n");
    printf("Email: example@example.com\n");
}

// Fungsi untuk memeriksa apakah saat ini dalam jam operasional
int isOperationalTime() {
    time_t now;
    struct tm *localTime;

    now = time(NULL);
    localTime = localtime(&now);

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;

    if ((hour >= OPERATIONAL_START && hour < BREAK_START) || (hour >= BREAK_END && hour < OPERATIONAL_END)) {
        return 1;
    } else if (hour == BREAK_START && minute < 60 - MAX_CUCI_TIME) {
        return 1;
    }

    return 0;
}

int main() {
    // Membaca data tiket dari file teks saat program dimulai
    bacaDataTiket();

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Pesan Tiket\n");
        printf("2. Cek Tiket\n");
        printf("3. Kontak\n");
        printf("0. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &choice);
        getchar(); // Membersihkan buffer stdin

        switch (choice) {
            case 1:
                if (isOperationalTime()) {
                    pesanTiket();
                } else {
                    printf("Maaf, saat ini bukan jam operasional.\n");
                }
                break;
            case 2:
                cekTiket();
                break;
            case 3:
                kontak();
                break;
            case 0:
                printf("Menyimpan data tiket...\n");
                // Menyimpan data tiket ke dalam file teks saat program berakhir
                simpanDataTiket();
                printf("Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (choice != 0);

    // Dealokasi memori untuk setiap node dalam double linked list
    TiketCuci *current = head;
    while (current != NULL) {
        TiketCuci *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}

