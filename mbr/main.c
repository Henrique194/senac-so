#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct {
    uint8_t head;
    uint8_t sector;
    uint8_t cylinder;
} __attribute__((packed)) CHSAddress;

typedef struct {
    uint8_t status;
    CHSAddress chs_first;
    uint8_t type;
    CHSAddress chs_last;
    uint32_t lba;
    uint32_t num_sectors;
} __attribute__((packed)) Partition;

typedef struct {
    uint8_t bootstrap[446];
    Partition part1;
    Partition part2;
    Partition part3;
    Partition part4;
    uint16_t signature;
} __attribute__((packed)) MBR;


MBR* mbr_sector() {
    FILE *file = fopen("mbr.bin", "rb");
    if (!file) {
        perror("Erro ao abrir arquivo \"mbr.bin\"");
        return NULL;
    }

    const size_t size_mbr = sizeof(MBR);
    MBR *mbr = malloc(size_mbr);
    const size_t code = fread(mbr, sizeof(uint8_t), size_mbr, file);
    if (code != size_mbr) {
        perror("Erro ao ler arquivo \"mbr.bin\"");
        return NULL;
    }

    fclose(file);
    file = NULL;

    return mbr;
}

char* tamanho_particao(uint32_t setores) {
    uint64_t bytes = (uint64_t) setores * 512;

    char *tam = malloc(sizeof(char)*6);
    double kib = (double)bytes / 1024;
    double mib = kib / 1024;
    double gib = mib / 1024;
    double tib = gib / 1024;
    double pib = tib / 1024;
    if (kib < 1) {
        sprintf(tam, "%ldB", bytes);
        return tam;
    }
    if (mib < 1) {
        sprintf(tam, "%.2lfK", kib);
        return tam;
    }
    if (gib < 1) {
        sprintf(tam, "%.2lfM", mib);
        return tam;
    }
    if (tib < 1) {
        sprintf(tam, "%.2lfG", gib);
        return tam;
    }
    if (pib < 1) {
        sprintf(tam, "%.2lfT", tib);
        return tam;
    }
    else {
        sprintf(tam, "%.2lfP", tib);
        return tam;
    }
}

void print(MBR *mbr) {
    if (!mbr) return;

//    Device       Start      End  Sectors  Size Type
//    /dev/sda1     2048     4095     2048    1M BIOS boot
//    /dev/sda2     4096  1054719  1050624  513M EFI System
//    /dev/sda3  1054720 52426751 51372032 24,5G Linux filesystem

    Partition parts[4] = { mbr->part1, mbr->part2, mbr->part3, mbr->part4 };
    for (int i = 0; i < 4; i++) {
        if (!parts[i].num_sectors) continue;

        char *nome = malloc(sizeof(char)*13);
        sprintf(nome, "/dev/sda%d", i+1);
        uint32_t comeco = parts[i].lba;
        uint32_t fim = comeco + parts[i].num_sectors - 1;
        uint32_t setores = parts[i].num_sectors;
        char *tamanho = tamanho_particao(setores);

        printf("%-16s%-16s%-16s%-16s%-16s\n", "Device", "Start", "End", "Sectors", "Size");
        printf("%-16s%-16d%-16d%-16d%-16s\n", nome, comeco, fim, setores, tamanho);

        free(tamanho);
        free(nome);
    }
}


int main() {
    MBR *mbr = mbr_sector();
    print(mbr);
    free(mbr);
    mbr = NULL;
    return 0;
}
